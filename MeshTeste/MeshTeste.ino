// Biblioteca Mesh
#include "painlessMesh.h"
/*  Programa para ESP32 depois da atualização OTA */
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
 
/* Constantes - conexão wi-fi e webserver */
const char* host = "esp32-2-Arquivo";
const char* ssid = "Vitor Oi Fibra 2G"; /* coloque aqui o nome da rede wi-fi que o ESP32 deve se conectar */
const char* password = "32010750"; /* coloque aqui a senha da rede wi-fi que o ESP32 deve se conectar */
const int port     = 5555; 
 
/* Variáveis globais */
int contador_ms = 0;
 
/* Webserver para se comunicar via browser com ESP32  */
WebServer server(80);
 
/* Variáveis da página html*/
extern const char* loginIndex;
extern const char* serverIndex;

//
////Defines
//
//#define MESH_PREFIX "Vitor Oi Fibra 2G"   // Troque para o nome da sua rede
//#define MESH_PASSWORD "32010750"   // coloque a senha da sua rede
//#define MESH_PORT 5555


Scheduler userScheduler;  
painlessMesh mesh;

void sendMessage();
Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &sendMessage);

void sendMessage() {
  String msg = "Hello from node ";
  msg += mesh.getNodeId();
  mesh.sendBroadcast(msg);
  taskSendMessage.setInterval(random(TASK_SECOND * 1, TASK_SECOND * 5));
}

// Needed for painless library
void receivedCallback(uint32_t from, String &msg) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() { Serial.printf("Changed connections\n"); }

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}
 
void setup(void) 
{
    Serial.begin(115200);

    /* Conecta-se a rede wi-fi */
    WiFi.begin(ssid, password);
    Serial.println("");
 
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
     
    Serial.println("");
    Serial.print("Conectado a rede wi-fi ");
    Serial.println(ssid);
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

   //Comumincação Mesh.-------------------------------------------------------------------------
   // mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC |
   // COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
   mesh.setDebugMsgTypes(ERROR | STARTUP | DEBUG);  // set before init() so that you can see startup messages    
   
   mesh.init(ssid, password, &userScheduler, 5555);
   mesh.onReceive(&receivedCallback);
   mesh.onNewConnection(&newConnectionCallback);
   mesh.onChangedConnections(&changedConnectionCallback);
   mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
   
   mesh.initOTAReceive("otareceiver");
   
   userScheduler.addTask(taskSendMessage);
   taskSendMessage.enable();
   //-------------------------------------------------------------------------------------------

}
 
void loop() 
{
    server.handleClient();
    delay(1);
 
    contador_ms++;
 
    if (contador_ms >= 1000)
    {    
        Serial.println("Programa depois da atualizacao OTA");
        contador_ms = 0;   
        //Sinaliza que funcionou    
        digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(1000);                       // wait for a second
        digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
        delay(1000); 
        mesh.update();
    }
}
