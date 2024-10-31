#include "WebSocket.h"
#include <ArduinoWebsockets.h>
#include <WiFi.h>

using namespace websockets;

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
    Serial.println("Connecting to WebSocket server...");
    _isConnected = client.connect(_serverAddress, _port, "/");

    if (_isConnected) {
        Serial.println("WebSocket connection opened");
    } else {
        Serial.println("WebSocket connection failed, retrying...");
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
