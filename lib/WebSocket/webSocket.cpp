#include "WebSocket.h"
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <ArduinoJson.h> 

using namespace websockets;

extern int distanceBack;

WebsocketsClient client;

WebSocket::WebSocket(const char* ssid, const char* password, const char* serverAddress, int port)
    : _ssid(ssid), _password(password), _serverAddress(serverAddress), _port(port), _isConnected(false) {}

void WebSocket::begin() {
    // Start Wi-Fi
    Serial.print("Connecting to WiFi...");
    WiFi.begin(_ssid, _password);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("\nConnected to WiFi");
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());

    // Setup WebSocket event handlers
    client.onMessage([this](WebsocketsMessage message) { onMessage(message); });
    client.onEvent([this](WebsocketsEvent event, String data) { onEvent(event, data); });

    // Connect to WebSocket server
    connectToWebSocket();
}

void WebSocket::loop() {
    if (_isConnected) {
        client.poll();
    } else {
        Serial.println("Attempting to reconnect to WebSocket server...");
        connectToWebSocket();
        delay(5000);
    }
}

void WebSocket::connectToWebSocket() {
    Serial.print("Connecting to WebSocket server at ");
    Serial.print(_serverAddress);
    Serial.print(":");
    Serial.println(_port);

    _isConnected = client.connect(_serverAddress, _port, "/");

    if (_isConnected) {
        Serial.println("WebSocket connection opened");
    } else {
        Serial.println("WebSocket connection failed");
        Serial.print("WiFi Status: ");
        switch (WiFi.status()) {
            case WL_CONNECTED: Serial.println("Connected"); break;
            case WL_NO_SSID_AVAIL: Serial.println("No SSID Available"); break;
            case WL_CONNECT_FAILED: Serial.println("Connect Failed"); break;
            case WL_CONNECTION_LOST: Serial.println("Connection Lost"); break;
            case WL_DISCONNECTED: Serial.println("Disconnected"); break;
            default: Serial.println("Unknown Status"); break;
        }
    }
}

void WebSocket::send(const String &message) {
    if (_isConnected) {
        client.send(message);
    } else {
        Serial.println("Unable to send, WebSocket not connected");
    }
}

void WebSocket::onMessage(WebsocketsMessage message) {
    Serial.printf("Received: %s\n", message.data().c_str());

    // Parse JSON message
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, message.data());

    if (error) {
        Serial.print("Failed to parse message as JSON: ");
        Serial.println(error.c_str());
        return;
    }

    // Extract initialSpeed value and assign it to distanceBack
    if (doc.containsKey("initialSpeed")) {
        distanceBack = doc["initialSpeed"];
        Serial.print("initialSpeed set to distanceBack: ");
        Serial.println(distanceBack);
    } else {
        Serial.println("initialSpeed not found in the message");
    }
}

void WebSocket::onEvent(WebsocketsEvent event, String data) {
    switch (event) {
        case WebsocketsEvent::ConnectionOpened:
            Serial.println("Connection Opened");
            _isConnected = true;
            break;
        case WebsocketsEvent::ConnectionClosed:
            Serial.println("Connection Closed");
            _isConnected = false;
            break;
        default:
            break;
    }
}
