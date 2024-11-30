#include <WiFi.h>             // Correct for ESP32 Wi-Fi functionality
#include <HTTPClient.h>       // For HTTP requests
     // For MQTT protocol
// #include <LiquidCrystal.h>    // For interfacing with LCD displays
      // Correct capitalization for ThingSpeak library




// WiFi configuration
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Firebase configuration
const char* firebaseHost = "https://iv-drip-a2206-default-rtdb.asia-southeast1.firebasedatabase.app/"; // Firebase Database URL
const char* firebaseApiKey = "AIzaSyAu7M_esvv8UdsSYrMRsD9LlBxFsOkxo5U"; // Firebase API Key

// ThingSpeak configuration
const char* thingspeakUrl = "https://api.thingspeak.com/update";
const char* thingspeakApiKey = "W02TUL9BWBNE04TU";

// Variable for volumeInLiters (simulating)
float volumeInLiters = 1.0;  // Sample value for testing

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected to WiFi!");
}

void loop() {
  // Reconnect WiFi if disconnected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, reconnecting...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("WiFi reconnected.");
  }

  // Send data to Firebase
  String firebaseUrl = String(firebaseHost) + "/volumeInLiters.json?auth=" + firebaseApiKey;
  HTTPClient http;
  http.begin(firebaseUrl);
  http.addHeader("Content-Type", "application/json");
  int firebaseResponseCode = http.PUT(String(volumeInLiters, 2));
  http.end();

  if (firebaseResponseCode > 0) {
    Serial.print("Firebase response code: ");
    Serial.println(firebaseResponseCode);
  } else {
    Serial.print("Error sending data to Firebase: ");
    Serial.println(firebaseResponseCode);
  }

  // Send data to ThingSpeak
  HTTPClient tsHttp;
  String payload = "api_key=" + String(thingspeakApiKey) + "&field1=" + String(volumeInLiters, 2);
  tsHttp.begin(thingspeakUrl);
  tsHttp.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int tsResponseCode = tsHttp.POST(payload);
  tsHttp.end();

  if (tsResponseCode > 0) {
    Serial.print("ThingSpeak response code: ");
    Serial.println(tsResponseCode);
  } else {
    Serial.print("Error posting to ThingSpeak: ");
    Serial.println(tsResponseCode);
  }

  // Print volume to serial monitor
  Serial.print("Volume: ");
  Serial.print(volumeInLiters, 2);
  Serial.println(" L");

  delay(1000); // Adjust delay as needed
}