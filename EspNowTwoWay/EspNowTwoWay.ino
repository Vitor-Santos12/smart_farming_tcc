/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <esp_now.h>
#include <WiFi.h>

//Lista de MacAddres 
extern uint8_t broadcastAddressEsp32_1[];
extern uint8_t broadcastAddressEsp32_2[];
extern uint8_t broadcastAddressEsp32_3[];

// Define variables to store incoming readings
String  incomingNome;
float   incomingTemp;
float   incomingHum;
float   incomingPres;

// Variable to store if sending data was successful
String success;

// Funções externas 
extern void send_message();

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
    String  nome;
    float   temp;
    float   hum;
    float   pres;
} struct_message;

// Create a struct_message to hold incoming sensor readings
struct_message incomingReadings;

// Callback when data is received
extern void OnDataRecv();

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
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
  memcpy(peerInfo.peer_addr, broadcastAddressEsp32_1, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Falha no aparelhamento do Esp32_1");
    return;
  }

  // Register peer Esp32_2
  //esp_now_peer_info_t peerInfo; ->> Não precisa repetir
  memcpy(peerInfo.peer_addr, broadcastAddressEsp32_2, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Falha no aparelhamento do Esp32_2");
    return;
  }

  // Register peer Esp32_3
  //esp_now_peer_info_t peerInfo; ->> Não precisa repetir
  memcpy(peerInfo.peer_addr, broadcastAddressEsp32_3, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Falha no aparelhamento do Esp32_3");
    return;
  }
 
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}
//Laço principal  
void loop() {
  extern void send_message();
  delay(10000);
}
