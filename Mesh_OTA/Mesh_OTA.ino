/* Bibliotecas */
/*MESH*/
#include "painlessMesh.h"
/*Uso de vetores e string*/
#include <iostream>
#include <string>
/* Bibliotecas WebServer e OTA */
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
/*Configuração do Wifi Local*/
#define MESH_PREFIX "Vitor Oi Fibra 2G"   // Troque para o nome da sua rede
#define MESH_PASSWORD "32010750"   // coloque a senha da sua rede
#define MESH_PORT 5555

/*Constante*/
const char* host = "esp32-3"; //Host
extern const char* loginIndex; //Primeira página
extern const char* serverIndex; // Página de Upload

//Funções de páginas externas
extern void piscaled();
extern void ContDelay();
extern void EntradasAnalogicas();
extern void EntradasDigitais();
/* Webserver para se comunicar via browser com ESP32  */
WebServer server(42);

// Declaração dos pinos
// Pinos ADC1 -> Conversor analógico-digital------------------------------------------------------
// Resolução de 12 bits com até 4096 valores distintos
// Valores de entrada opcionalmente 0-1V, 0-1.34V, 0-2V ou 0-3.6V
const int PinADC1 = 7;
//    Pinos na placa =   {     VP,     VN,    D32,    D33,    D34,    D35} 
//    Valores do Indices { GPIO36, GPIO39, GPIO32, GPIO33, GPIO34, GPIO35}
//    Indices do Vetor = {      0,      1,      2,      3,      4,      5} 
//------------------------------------------------------------------------------------------------
int CanalADC1[PinADC1] = {     36,     39,     32,     33,     34,     35};
// Pinos ADC2 -> Conversor analógico-digital------------------------------------------------------
const int PinADC2 = 9;
//    Pinos na placa =   {     D2,     D4,    D15,    D13,    D12,    D14,    D27,    D25,     D26} 
//    Valores do Indices {  GPIO2,  GPIO2, GPIO15, GPIO13, GPIO12, GPIO14, GPIO27, GPIO25,  GPIO26}
//    Indices do Vetor = {      0,      4,      2,      3,      4,      5,      6,      7,       8} 
//------------------------------------------------------------------------------------------------
int CanalADC2[PinADC2] = {      2,      4,      15,     13,     12,     14,     27,     25,     26};
// Pinos GPIO DAC(Digital to Analog Converter)----------------------------------------------------
// Conversore digitais com saidas de tensão analógica
const int PinGPIODAC = 3;
//    Pinos na placa         = {     D25,     D26} 
//    Valores do Indices       {  GPIO25,  GPIO26}
//    Indices do Vetor       = {       0,       1} 
//------------------------------------------------------------------------------------------------
int CanalGPIODAC[PinGPIODAC] = {      25,      26};
// Pinos apenas de Entrada -----------------------------------------------------------------------
const int PinIn = 5;
//    Pinos na placa =   {    D34,    D35,     VP,     VN} 
//    Valores do Indices { GPIO34, GPIO35, GPIO36, GPIO39}
//    Indices do Vetor = {      0,      1,      2,      3} 
//-------------------------------------------------------
int CanalIn[PinIn] =     {     34,     35,     36,     39};
//------------------------------------------------------------------------------------------------
// /* Variáveis globais */ -> Variáriveis para leitura
float  ValorAnalogico[5];
int    ValorDigital[5];
float  tensao[5]; 
String MsgValores;
int contador_ms = 0;


//Funções Mesh

Scheduler userScheduler;  
painlessMesh mesh;
void sendMessage();

Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &sendMessage);

//  Função para enviar mensagem
void sendMessage() {
  String msg = MsgValores+"\n Vindo do nó:";
  msg += mesh.getNodeId();
  mesh.sendBroadcast(msg);
  taskSendMessage.setInterval(random(TASK_SECOND * 1, TASK_SECOND * 5));
}

// Função de recebe mensagem
void receivedCallback(uint32_t from, String &msg) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: Nova conexao, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() { Serial.printf("Changed connections\n"); }

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Tempo de Ajuste %u. Offset = %d\n", mesh.getNodeTime(), offset);
}
/*Void Setup-------------------------------------------------------------------*/
void setup() {
  /*Serial Begin*/
  Serial.begin(115200);
  /* Conecta-se a rede wi-fi */
  WiFi.begin(MESH_PREFIX, MESH_PASSWORD);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) 
  {
      delay(500);
      Serial.print(".");
  }
  /*Mensagem de conexão*/ 
  Serial.println("");
  Serial.print("Dispositivo ");
  Serial.println(host);
  Serial.print("Conectado a rede wi-fi ");
  Serial.println(MESH_PREFIX);
  Serial.print("IP obtido: ");
  Serial.println(WiFi.localIP());

  /* Usa MDNS para resolver o DNS */
  if (!MDNS.begin(host)) 
  { 
      //http://esp32.local
      Serial.println("Erro ao configurar mDNS. O ESP32 vai reiniciar em 1s...");
      delay(1000);
      ESP.restart();        
  }

  Serial.println("mDNS configurado e inicializado;");
   
  /* Configfura as páginas de login e upload de firmware OTA */
  server.on("/", HTTP_GET, []() 
  {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", loginIndex);
  });
   
  server.on("/serverIndex", HTTP_GET, []() 
  {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", serverIndex);
  });
 
  /* Define tratamentos do update de firmware OTA */
  server.on("/update", HTTP_POST, []() 
  {
      server.sendHeader("Connection", "close");
      server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
  }, []() {
      HTTPUpload& upload = server.upload();
       
      if (upload.status == UPLOAD_FILE_START) 
      {
          /* Inicio do upload de firmware OTA */
          Serial.printf("Update: %s\n", upload.filename.c_str());
          if (!Update.begin(UPDATE_SIZE_UNKNOWN)) 
              Update.printError(Serial);
      } 
      else if (upload.status == UPLOAD_FILE_WRITE) 
      {
          /* Escrevendo firmware enviado na flash do ESP32 */
          if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) 
              Update.printError(Serial);      
      } 
      else if (upload.status == UPLOAD_FILE_END) 
      {
          /* Final de upload */
          if (Update.end(true))             
              Serial.printf("Sucesso no update de firmware: %u\nReiniciando ESP32...\n", upload.totalSize);
          else
              Update.printError(Serial);
      }   
  });
  server.begin();
  
  // Pino para pisca led Esp32
  pinMode(CanalADC2[0], OUTPUT);  
  //Define o pino de leitura analogica como entrada
  for(int i = 1; i <=3; i++){
  pinMode(CanalADC2[i], INPUT);
  }  
  //Define o pino de leitura digital como entrada
  for(int j = 1; j <=3; j++){
  pinMode(CanalADC1[j], INPUT);
  }  

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

}

void loop(){
    server.handleClient();
    delay(1);

    if (contador_ms >= 1000)
    {    
        Serial.println("Programa antes da atualizacao OTA");
        contador_ms = 0;
    }
    
    contador_ms++;
    
//    piscaled();
//    EntradasAnalogicas();
//    EntradasDigitais();
//    mesh.update();
  
}  
