/*  Programa para ESP32 depois da atualização OTA */
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
 
/* Constantes - conexão wi-fi e webserver */
const char* host = "esp32-1-Arquivo";
// Credencial da Rede Local 
const char* ssid = "REPLACE_WITH_YOUR_SSID";/* coloque aqui o nome da rede wi-fi que o ESP32 deve se conectar */
const char* password = "REPLACE_WITH_YOUR_PASSWORD"; /* coloque aqui a senha da rede wi-fi que o ESP32 deve se conectar */
 
/* Variáveis globais */
int contador_ms = 0;
 
/* Webserver para se comunicar via browser com ESP32  */
WebServer server(80);
 
/* Códigos da página que será aberta no browser 
   (quando comunicar via browser com o ESP32) 
   Esta página exigirá um login e senha, de modo que somente 
   quem tenha estas informações consiga atualizar o firmware
   do ESP32 de forma OTA */
const char* loginIndex = 
"<!DOCTYPE html>"
"<html lang='en'>"
"<head>"
"<title>Esp Login</title>"
"<meta charset='utf-8'>"
 "<link rel='shortcut icon' href='https://iconape.com/wp-content/files/tp/348391/png/348391.png'  width='50' height='50'>"
"<meta name='viewport' content='width=device-width, initial-scale=1'>"
"<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css' rel='stylesheet'>"
"<script src='https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js'></script>"
"<link rel='stylesheet' href='https://www.w3schools.com/w3css/4/w3.css'>" 
"<style>"
"div[name=card]{"
  "margin: auto;"
  "width: 80%;"
  "style=background-color:hsla(200,100%,90%,0.3);"
"}"
"body {"
  "background-image: url('https://image.shutterstock.com/shutterstock/photos/637978216/display_1500/stock-vector-smart-farm-icon-logo-vector-illustration-637978216.jpg');"
  "text-align:'center'"
"}"
"</style>" 
"</head>"
"<body>"
"<div class='container'>"
    "<div class='row'>"
      "<div class='col-md-6 offset-md-3'>"
        "<h1 class='text-center text-dark mt-5 display-4'>SmartFarm</h1>"
        "<div class='text-center mb-5 text-dark display-4'>Dispositivo 1</div>"
        "<div name = 'card' class='w3-hover-shadow w3-padding-64 w3-center' >"
          "<center>"  
          "<form class='card-body cardbody-color p-lg-5 text-center'>"
            "<div class='text-center'>"
              "<img src='https://files.cercomp.ufg.br/weby/up/1/o/Marca_UFG_cor_horizontal-04.png' class='rounded' alt='Cinque Terre' width='200' height='150'>"
            "</div>"
            "<div class='mb-3'>"
              "<input type='text' class='form-control' id='Username' name='userid' aria-describedby='emailHelp'"
                "placeholder='Nome'>"
            "</div>"
            "<div class='mb-3'>"
              "<input type='password' class='form-control' id='password' name='pwd' placeholder='Senha'>"
            "</div>"
            "<div class='text-center'><button type='button' class='btn btn-primary' onclick='check(this.form)'value='Identificar' >Login</button></div>"
          "</form>"
          "</center>"
        "</div>"
      "</div>"
    "</div>"
  "</div>"
"<script>"
    "function check(form)"
    "{"
    "if(form.userid.value=='admin' && form.password.value=='admin')"
    "{"
    "window.open('serverIndex')"
    "}"
    "else"
    "{"
     "alert('Login ou senha inválidos')"
    "}"
    "}"
"</script>"
  "</body>"
"</html>";

  
const char* serverIndex = 
"<!DOCTYPE html>"
"<html>"
  "<head>"
  "<meta charset='utf-8'>"
  "<title>Esp Update</title>"
  "<link rel='shortcut icon' href='https://iconape.com/wp-content/files/tp/348391/png/348391.png'  width='50' height='50'>"
  "<meta name='viewport' content='width=device-width, initial-scale=1'>"
  "<link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/bootstrap@4.6.1/dist/css/bootstrap.min.css'>"
  "<script src='https://cdn.jsdelivr.net/npm/jquery@3.5.1/dist/jquery.slim.min.js'></script>"
  "<script src='https://cdn.jsdelivr.net/npm/popper.js@1.16.1/dist/umd/popper.min.js'></script>"
  "<script src='https://cdn.jsdelivr.net/npm/bootstrap@4.6.1/dist/js/bootstrap.bundle.min.js'></script>"
  "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
  "</head>"
"<body>"
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<div class='jumbotron text-center'>"  
  "<h1>Update</h1>"
  "<div id = 'update'>"
  "<img src='https://cdn-icons-png.flaticon.com/512/87/87443.png' width='50' height='50' background-color: '#cccccc'>"
  "</div>"
  "<p>Insira o arquivo .bin</p>" 
"</div>"
"<div class='container'>"
  "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
    "<div class='custom-file mb-3'>"
      "<input type='file' class='custom-file-input' id='customFile' name='filename'>"
      "<label class='custom-file-label' for='customFile'>Escolha o arquivo</label>"
    "</div>"
    "<!-- <input type='file' name='update' > -->"
    "<center><input type='submit' value='update' class='btn btn-primary'></center>"
  "</form>"
  "<center><div id='prg'>Progresso: 0%</div></center>"
"</div>"
"<script>"
"$('form').submit(function(e){"
"e.preventDefault();"
"var form = $('#upload_form')[0];"
"var data = new FormData(form);"
"$.ajax({"
"url: '/update',"
"type: 'POST',"
"data: data,"
"  contentType: false,"
"  processData:false,"
"  xhr: function() {"
"  var xhr = new window.XMLHttpRequest();"
"  xhr.upload.addEventListener('progress', function(evt) {"
"  if(evt.lengthComputable) {"
"  var per = evt.loaded / evt.total;"
"  $('#prg').html('Progresso:' + Math.round(per*100) + '%');"
"  }"
"  }, false);"
"  return xhr;"
"  },"
"  success:function(d, s) {"
"  console.log('Sucesso!')"
"  },"
" error: function (a, b, c){"
" }"
" });"
" });"
"// Add the following code if you want the name of the file appear on select"
"$('.custom-file-input').on('change', function() {"
"  var fileName = $(this).val().split('\\').pop();"
"  $(this).siblings('.custom-file-label').addClass('selected').html(fileName);"
"});"
" </script>"
" <!-- Animação Update -->"
"<style>"
"#update {"
"  margin: auto;"
"  animation: mymove 3s;"
"  animation-fill-mode: forwards;"
"}"
"@keyframes mymove {"
"  100% {transform: rotate(90deg);}"
"}"
"</style>"
"</body>"
"<footer>"
"</br>"
"</br>"
"</br>"
"<div class='col-sm-12' style='border-bottom:1px solid black' ><center>Universidade Federal de Goiás<center></div>"
"</footer>"
"</html>";
 
void setup(void) 
{
    Serial.begin(115200);

   //Comumincação Mesh.



    
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
    }
}
