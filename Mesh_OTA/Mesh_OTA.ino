#include "painlessMesh.h"

#define MESH_PREFIX "Vitor Oi Fibra 2G"   // Troque para o nome da sua rede
#define MESH_PASSWORD "32010750"   // coloque a senha da sua rede
#define MESH_PORT 5555

//Funções de páginas externas
extern void piscaled();
extern void ContDelay();

//Pinos que serão utilizados 
#define SenPin 12
#define IOPin 14

//Variáveis para leitura dos pinos 12, 14
int potValue = 0;
int digValue = 0;

void clearAndHome()
{
  //Serial.print(27, byte); // ESC
  Serial.print("[2J"); // clear screen
  //Serial.print(27, byte); // ESC
  Serial.print("[H"); // cursor to home
}

//Funções Mesh

Scheduler userScheduler;  
painlessMesh mesh;
void sendMessage();

Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &sendMessage);

//  Função para enviar mensagem
void sendMessage() {
  String msg = String("***Esp 32-1 Comunica que*** \n Valor Digital:\t"+String(potValue)+"Valor Analogico:"+String(digValue)+"\nVem do no:"); 
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

void setup() {
  Serial.begin(115200);

  // mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC |
  // COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes(
      ERROR | STARTUP |
      DEBUG);  // set before init() so that you can see startup messages

  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  mesh.initOTAReceive("otareceiver");

  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();
//--> Leitura Sensor de chuva
pinMode(SenPin,INPUT);
pinMode(IOPin,INPUT);

}

void loop(){
  //--------------------------------------------------------
  Serial.flush();    // cursor Home (início da tela)  
  //---------------------------------------------------
  mesh.update();
  //piscaled();
  // Lendo Pino 12
  Serial.println("Pino Analógico");
  potValue = analogRead(SenPin);
  Serial.println(potValue);
  ContDelay(5);
  Serial.println("\n-----------------");
  // Lendo Pino 14
  Serial.println("Pino Digital");
  digValue = digitalRead(IOPin);
  Serial.println(digValue);
  Serial.println("\n-----------------");
  ContDelay(5);  
  //---------------------------------------------------
  Serial.flush();    // comando para limpar a tela
  //---------------------------------------------------
  
}  
