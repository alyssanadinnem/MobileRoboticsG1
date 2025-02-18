#include <esp_now.h>
#include <WiFi.h>

//"kabikis75windfall" - MAC: - old esp wifi - Channel: 11
//"needlings84wheezily" - MAC: 48:ca:43:06:13:fc new esp wifi - Channel: 11

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    int angle;
    int distance;
} struct_message;

// Create a struct_message called distanceAngle
struct_message distanceAngle;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&distanceAngle, incomingData, sizeof(distanceAngle));

  Serial.print(distanceAngle.angle);
  Serial.print(",");
  Serial.print(distanceAngle.distance);
  Serial.println(".");
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}
 
void loop() {

  }