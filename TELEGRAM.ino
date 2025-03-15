#include "BluetoothSerial.h"
#include <WiFi.h>
#include <UniversalTelegramBot.h>
// #include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "Geethanjali";
const char* password = "campus@123456";

// Telegram Bot credentials
#define BOTtoken "8011737567:AAEkhZfVFfi04g3HmvvJEigJ8KjyDfvP6xc"  
#define CHAT_ID "8098165875"

BluetoothSerial SerialBT;
WiFiClient client;  // Change from WiFiClientSecure to WiFiClient
UniversalTelegramBot bot(BOTtoken, client);

const char helpMessage[] PROGMEM = "🚨 EMERGENCY! User needs help! 🚨";

void setup() {
    Serial.begin(115200);
    SerialBT.begin("ESP32_BT"); // Bluetooth device name
    Serial.println("Bluetooth started! Ready to pair...");

    // Connect to WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) { // Timeout after 20 attempts
        Serial.print(".");
        delay(100);  // Reduced delay
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected");
        bot.sendMessage(CHAT_ID, "Smart Safety Wristband Started", "");
    }
}

void loop() {
    if (SerialBT.available()) {
        String command = "";
        while (SerialBT.available()) {
            char incomingChar = SerialBT.read();
            command += incomingChar;
            delay(2); // Small delay to prevent data loss
        }

        command.trim(); // Remove unwanted spaces

        Serial.println("Received: " + command);

        if (command.equalsIgnoreCase("help")) {  
            Serial.println("Emergency detected! Sending Telegram alert...");
            bot.sendMessage(CHAT_ID, helpMessage, "");
        }
    }
}
