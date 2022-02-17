//Escopo das funções
void send_message();
//Lista de MacAddres 

extern uint8_t broadcastAddressEsp32_1[];
extern uint8_t broadcastAddressEsp32_2[];
extern uint8_t broadcastAddressEsp32_3[];

// Create a struct_message called BME280Readings to hold sensor readings
struct_message envio;

void send_message(){

// Set values to send
envio.temp = 30.6;
envio.hum = 50.0;
envio.pres = 0.6;
  
// Send message via ESP-NOW -> Esp32_1
envio.nome = "Esp32_1"; 
esp_err_t resultEsp32_1 = esp_now_send(broadcastAddressEsp32_1, (uint8_t *) &envio, sizeof(envio));   
if (resultEsp32_1 == ESP_OK) {
    Serial.println("Envia para o Esp32_1 com sucesso");
}
else {
    Serial.println("Erro ao enviar os dados");
}
// Send message via ESP-NOW -> Esp32_2
envio.nome = "Esp32_2"; 
esp_err_t resultEsp32_2 = esp_now_send(broadcastAddressEsp32_2, (uint8_t *) &envio, sizeof(envio));   
if (resultEsp32_2 == ESP_OK) {
    Serial.println("Envia para o Esp32_2 com sucesso");
}
else {
    Serial.println("Erro ao enviar os dados");
}

//// Send message via ESP-NOW -> Esp32_3
//envio.nome = "Esp32_3"; 
//esp_err_t resultEsp32_3 = esp_now_send(broadcastAddressEsp32_3, (uint8_t *) &envio, sizeof(envio));   
//if (resultEsp32_3 == ESP_OK) {
//    Serial.println("Envia para o Esp32_3 com sucesso");
//}
//else {
//    Serial.println("Erro ao enviar os dados");
//}
//----------------------------------------------
}
