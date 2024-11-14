#include "websocket.h"

#include "motor.h"

using namespace websockets;

extern motor myMotor;

WebsocketsClient client;

WebSocket::WebSocket(const char *ssid, const char *password, const char *serverAddress, int port)
    : _ssid(ssid), _password(password), _serverAddress(serverAddress), _port(port), _isConnected(false) {}

void WebSocket::begin()
{
    Serial.print("Connecting to WiFi...");
    WiFi.begin(_ssid, _password);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("\nConnected to WiFi");

    client.onMessage([this](WebsocketsMessage message)
                     { onMessage(message); });
    client.onEvent([this](WebsocketsEvent event, String data)
                   { onEvent(event, data); });

    connectToWebSocket();
}

void WebSocket::loop()
{
    if (_isConnected)
    {
        client.poll();
    }
    else
    {
        Serial.println("Attempting to reconnect to WebSocket server...");
        connectToWebSocket();
        delay(5000);
    }
}

void WebSocket::connectToWebSocket()
{
    _isConnected = client.connect(_serverAddress, _port, "/");

    if (_isConnected)
    {
        _isConnected = true;
        Serial.println("WebSocket connection opened");
    }
    else
    {
        Serial.println("WebSocket connection failed, retrying...");
    }
}

void WebSocket::send(const String &message)
{
    if (_isConnected)
    {
        client.send(message);
    }
    else
    {
        Serial.println("Unable to send, WebSocket not connected");
    }
}

void WebSocket::onMessage(WebsocketsMessage message)
{

    Serial.printf("Received: %s\n", message.data().c_str());

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, message.data());

    if (error)
    {
        Serial.print("Failed to parse message as JSON: ");
        Serial.println(error.c_str());
        return;
    }

    // Extract initialSpeed value and assign it to distanceBack
    if (doc.containsKey("initialSpeed"))
    {
        myMotor.maxSpeed = doc["initialSpeed"];
            ledcWrite(myMotor.pin1, doc["initialSpeed"]);
            ledcWrite(myMotor.pin3, doc["initialSpeed"]);        
    }
    else
    {
        Serial.println("initialSpeed not found in the message");
    }
}

int WebSocket::getBrakingCategory() const
{
    return _brakingCategory;
}

void WebSocket::onEvent(WebsocketsEvent event, String data)
{
    switch (event)
    {
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
