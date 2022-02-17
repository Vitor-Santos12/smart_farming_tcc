//Escopo das funções
void OnDataRecv();
// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  incomingNome = incomingReadings.nome;
  incomingTemp = incomingReadings.temp;
  incomingHum = incomingReadings.hum;
  incomingPres = incomingReadings.pres;
  Serial.print("Nome:");
  Serial.println(incomingNome);
  Serial.print("Temperatura:");
  Serial.println(incomingTemp);
  Serial.print("Umidade:");
  Serial.println(incomingHum);
  Serial.print("Pressão:");
  Serial.println(incomingPres);
}
