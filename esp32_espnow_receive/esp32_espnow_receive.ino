/*
 * Syed Razwanul Haque (Nabil)
 * https://www.nabilbd.com , https://www.github.com/Nabilphysics , https://www.fb.com/Nabilphysics, https://www.cruxbd.com
 * Credit: Thingsboard Library, https://randomnerdtutorials.com/esp32-esp-now-wi-fi-web-server/, https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/
*/

#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  char studentId[32];
  char studentNo[32];
  int analog;
  float temp;
  int rate;
  float marks;
  int randomNumber;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("studentNo: ");
  Serial.println(myData.studentNo);
  Serial.print("studentNo: ");
  Serial.println(myData.studentNo);
  Serial.print("analog: ");
  Serial.println(myData.analog);
  Serial.print("temp: ");
  Serial.println(myData.temp);
  Serial.print("rate: ");
  Serial.println(myData.rate);
  Serial.print("marks: ");
  Serial.println(myData.marks);
  Serial.print("Random Number: ");
  Serial.println(myData.randomNumber);
  Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
  WiFi.printDiag(Serial);

}
 
void loop() {

}
