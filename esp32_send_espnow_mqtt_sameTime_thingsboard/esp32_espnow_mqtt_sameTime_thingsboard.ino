/*
 * Syed Razwanul Haque (Nabil)
 * https://www.nabilbd.com , https://www.github.com/Nabilphysics , https://www.fb.com/Nabilphysics, https://www.cruxbd.com
 * Credit: Thingsboard Library, https://randomnerdtutorials.com/esp32-esp-now-wi-fi-web-server/, https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/
*/

#include "ThingsBoard.h"
#include <esp_now.h>

// Replace it with you Receiver Mac Address
uint8_t broadcastAddress[] = {0x7C, 0x9E, 0xBD, 0xF7, 0x7C, 0x58};// 
//To Find Mac Address
//  WiFi.mode(WIFI_MODE_STA);
//  Serial.println(WiFi.macAddress());

// Structure example to send data(Must Match with Receiver Structure)

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

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
int i=0;

void setup()
{ 
 
  WiFi.persistent(false);
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  delay(100);

  thingsboard_Init();
  Serial.begin(115200);
  Serial.setDebugOutput(true);
   // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 1; // Its important to stay at same channel as your wifi channel  
  peerInfo.encrypt = false;
  
  //uint8_t primaryChan = WiFi.channel();
  //wifi_second_chan_t secondChan = WIFI_SECOND_CHAN_NONE;
  
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  Serial.println("Device Started");
}

void loop()
{
  
  String data;
  const size_t bufferSize = JSON_OBJECT_SIZE(8); 
  //https://arduinojson.org/v5/assistant/
  //https://arduinojson.org/v5/faq/how-to-determine-the-buffer-size/
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();
  
  // Set values to send to Thingsboard
  strcpy(myData.studentId, "Covid 22");
  strcpy(myData.studentNo, "Shovon Hasib"); //strcpy(copy to, copy from)
  myData.analog = 2025;
  myData.temp = 36.6;
  myData.rate = 70;
  myData.marks = 95.1;
  myData.randomNumber = random(1,100); 

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }

  
 
  root["studentId "] = myData.studentId;
  root["analog "] = String(myData.analog);
  root["temp "]  = String(myData.temp);
  root["rate "] = String(myData.rate);
  root["marks "] = String(myData.marks);
  root["random "] = String(myData.randomNumber);
  
  root.printTo(data);
  
  thingsboard_Publish(data);

  delay(100);
  Serial.println(data);

  
  Serial.print("JSON Buffer Size: ");
  Serial.println(jsonBuffer.size());

  WiFi.printDiag(Serial);
  client.loop();
}
