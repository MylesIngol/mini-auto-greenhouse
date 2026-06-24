#include "connectivity.h"
#include "../include/config.h"
#include "../include/secrets.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUDP.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

// ── Clients ───────────────────────────────────────────────────────────────────
static WiFiClientSecure  wifiClient;
static PubSubClient      mqttClient(wifiClient);
static WiFiUDP           ntpUDP;
static NTPClient         timeClient(ntpUDP, "pool.ntp.org", 0, 60000);
static InfluxDBClient    influxClient(INFLUX_URL, INFLUX_ORG, INFLUX_BUCKET, INFLUX_TOKEN, InfluxDbCloud2CACert);

// ── Timing ────────────────────────────────────────────────────────────────────
static unsigned long lastPublishMs = 0;
#define PUBLISH_INTERVAL_MS 30000

// ── Public ────────────────────────────────────────────────────────────────────
bool Connectivity::begin() {
    _connectWifi();
    if (!isWifiConnected()) return false;

    // NTP
    timeClient.begin();
    timeClient.update();
    Serial.println("[NTP] Time synced.");

    // InfluxDB
influxClient.setWriteOptions(WriteOptions().writePrecision(WritePrecision::S));    if (influxClient.validateConnection()) {
        Serial.printf("[INFLUX] Connected: %s\n", influxClient.getServerUrl().c_str());
    } else {
        Serial.printf("[INFLUX] Connection failed: %s\n", influxClient.getLastErrorMessage().c_str());
    }

    // MQTT
    mqttClient.setServer(MQTT_HOST, MQTT_PORT);
    mqttClient.setBufferSize(512);
    _connectMQTT();

    return true;
}

void Connectivity::update(const SystemState& state) {
    if (!isWifiConnected()) return;

    // Reconnect MQTT if dropped
    if (!mqttClient.connected()) {
        _connectMQTT();
    }
    mqttClient.loop();

    // Publish on interval
    unsigned long now = millis();
    if (now - lastPublishMs >= PUBLISH_INTERVAL_MS) {
        lastPublishMs = now;
        timeClient.update();
        _publishMQTT(state);
        _publishInflux(state);
    }
}

bool Connectivity::isWifiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

// ── Private ───────────────────────────────────────────────────────────────────
void Connectivity::_connectWifi() {
    Serial.printf("[WIFI] Connecting to %s", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("\n[WIFI] Connected. IP: %s\n", WiFi.localIP().toString().c_str());
    } else {
        Serial.println("\n[WIFI] Failed to connect.");
    }
}

void Connectivity::_connectMQTT() {
    wifiClient.setInsecure(); // TLS without cert validation — acceptable for portfolio project
    Serial.printf("[MQTT] Connecting to %s...\n", MQTT_HOST);

    if (mqttClient.connect("greenhouse-esp32", MQTT_USERNAME, MQTT_PASSWORD)) {
        Serial.println("[MQTT] Connected.");
        _mqttConnected = true;
    } else {
        Serial.printf("[MQTT] Failed. State: %d\n", mqttClient.state());
        _mqttConnected = false;
    }
}

void Connectivity::_publishMQTT(const SystemState& s) {
    char payload[256];
    snprintf(payload, sizeof(payload),
        "{\"tempF\":%.1f,\"humidity\":%.1f,\"heatIndexF\":%.1f,\"soilPct\":%d,\"waterOk\":%s}",
        s.tempF, s.humidity, s.heatIndexF, s.soilPercent,
        s.waterOk ? "true" : "false"
    );

    bool ok = mqttClient.publish("greenhouse/sensors", payload);
    Serial.printf("[MQTT] Publish %s: %s\n", ok ? "OK" : "FAILED", payload);
}

void Connectivity::_publishInflux(const SystemState& s) {
    Point p("environment");
    p.addTag("device", "esp32-greenhouse");
    p.addField("temp_f",       s.tempF);
    p.addField("humidity",     s.humidity);
    p.addField("heat_index_f", s.heatIndexF);
    p.addField("soil_pct",     s.soilPercent);
    p.addField("water_ok",     s.waterOk ? 1 : 0);
    p.setTime(timeClient.getEpochTime());

    if (!influxClient.writePoint(p)) {
        Serial.printf("[INFLUX] Write failed: %s\n", influxClient.getLastErrorMessage().c_str());
    } else {
        Serial.println("[INFLUX] Write OK.");
    }
}