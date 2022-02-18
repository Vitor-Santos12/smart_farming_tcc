/*----Bibliotecas-----------------------------------------------------------*/
/*----MESH + JSON-----------------------------------------------------------*/
#include "painlessMesh.h"
#include <Arduino_JSON.h>
/*----Uso de vetores e string-----------------------------------------------*/
#include <iostream>
#include <string>
/*----Bibliotecas Esp Servidor Web Assíncrono-------------------------------*/
#include "ESPAsyncWebServer.h"
#include <AsyncTCP.h>
#include <WiFi.h>
/*----Configuração do Mesh--------------------------------------------------*/
#define MESH_PREFIX "Smart_Farm"   // Nome da rede Mesh
#define MESH_PASSWORD "201209523"   // Senha da rede Mesh
#define MESH_PORT 5555
/*----Configuração do Web Server--------------------------------------------*/
const char* ssid = "Vitor Oi Fibra 2G";
const char* password = "32010750";
/*----Set do Websever na porta 80-------------------------------------------*/
JSONVar board;
AsyncWebServer server(80);
AsyncEventSource events("/events");
/*----Constante-------------------------------------------------------------*/
const char* host = "esp32-2"; //Host
extern const char index_html[]; // pagina HTML
/*Funções de páginas externas-----------------------------------------------*/
//extern void piscaled();
//extern void ContDelay();
//extern void EntradasAnalogicas();
//extern void EntradasDigitais();
/*----Webserver para se comunicar via browser com ESP32---------------------*/
/*----Variáveis globais ->> Variáriveis para leitura------------------------*/
//float  ValorAnalogico[5];
//int    ValorDigital[5];
//float  tensao[5]; 
//String MsgValores;
//int contador_ms = 0;
/*----Numero do nó----------------------------------------------------------*/
int nodeNumber = 2; //Numero do nó
/*----String de leitura do JSON---------------------------------------------*/
String readings;
/*----Funções Mesh----------------------------------------------------------*/
Scheduler userScheduler;  
painlessMesh mesh;
/*----User stub-------------------------------------------------------------*/
void sendMessage(); // Prototype so PlatformIO doesn't complain
String getReadings(); // Prototype for sending sensor readings

/*----Create tasks: to send messages and get readings;----------------------*/
Task taskSendMessage(TASK_SECOND * 5 , TASK_FOREVER, &sendMessage);
/*----Estrutura JSON--------------------------------------------------------*/
String getReadings(){
  JSONVar jsonReadings;
  jsonReadings["node"] = nodeNumber;
  jsonReadings["temp"] = 10.3;
  jsonReadings["hum"] = 12.5;
  jsonReadings["pres"] = 2000;
  readings = JSON.stringify(jsonReadings);
  return readings;
}
/*----Função de Envio--------------------------------------------------------*/
void sendMessage () {
  String msg = getReadings();
  mesh.sendBroadcast(msg);
}
/*----Estrutura de recebimentos da mensagem----------------------------------*/
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("Received from %u msg=%s\n", from, msg.c_str());
  JSONVar myObject = JSON.parse(msg.c_str());
  int node = myObject["node"];
  double temp = myObject["temp"];
  double hum = myObject["hum"];
  double pres = myObject["pres"];
  Serial.print("Node: ");
  Serial.println(node);
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" C");
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.println(" %");
  Serial.print("Pressure: ");
  Serial.print(pres);
  Serial.println(" hpa");
}
/*---------------------------------------------------------------------------*/
void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("New Connection, nodeId = %u\n", nodeId);
}
/*---------------------------------------------------------------------------*/
void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}
/*---------------------------------------------------------------------------*/
void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

/*----Void Setup--------------------------------------------------------------*/
void setup() {

/*------Inicialização do moitor Serial----------------------------------------*/
    Serial.begin(115200);
/*------------------------------------Server Wifi-----------------------------*/
/*----Inicia um ponto de acesso simultaneamente-------------------------------*/
  WiFi.mode(WIFI_AP_STA);      
/*----Conecta-se a rede wi-fi-------------------------------------------------*/
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Iniciando estação Wi-Fi..");
  }
  Serial.print("Endereço IP estatico: ");
  Serial.println(WiFi.localIP());
  Serial.print("Canal Wi-Fi: ");
  Serial.println(WiFi.channel());
/*----Carrega a página webserver-----------------------------------------------*/
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
/*----Envia mensagem serial ao usuário----------------------------------------*/   
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Usuário conectado! Ultima mensagem ID adiquirida: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  server.begin();
  /* Mesh--------------------------------------------------------------------------------------------------------------*/  
  // mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes(ERROR | STARTUP);// | DEBUG);  // set before init() so that you can see startup messages
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  //mesh.initOTAReceive("otareceiver");

  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();

/*----Pino para pisca led Esp32---------------------------------------------------------------------------------------*/
//  pinMode(CanalADC2[0], OUTPUT);  
//  //Define o pino de leitura analogica como entrada
//  for(int i = 1; i <=3; i++){
//  pinMode(CanalADC2[i], INPUT);
//  }  
//  //Define o pino de leitura digital como entrada
//  for(int j = 1; j <=3; j++){
//  pinMode(CanalADC1[j], INPUT);
//  }   
}

void loop(){
      mesh.update();
      static unsigned long lastEventTime = millis();
      static const unsigned long EVENT_INTERVAL_MS = 5000;
      if ((millis() - lastEventTime) > EVENT_INTERVAL_MS) {
      events.send("ping",NULL,millis());
      lastEventTime = millis();
      }
    
  
}  
