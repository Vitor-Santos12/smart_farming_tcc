
// #Biblioteca
#include <iostream>
#include <string>

// Desenvolvendo o perfil do ESP 32 através da variáveis
// Canais PWM
const int PinPWM = 27;
// Numeros de pinos PWM---------------------------------------------------------------------------
//  Pinos na placa =   {     VP,     VN,    D34,    D35,    D32,    D33,    D25,    D26,    D27,    D14,    D12,    D13,    D15,     D2,     D4,    RX2,    TX2,    D18,    D19,    D21,    RX0,    TX0,    D23} 
//  Valores do Indices={ GPIO36, GPIO39, GPIO34, GPIO35, GPIO32, GPIO33, GPIO25, GPIO26, GPIO27, GPIO14, GPIO12, GPIO13, GPIO15,  GPIO2,  GPIO4, GPIO16, GPIO17, GPIO18, GPIO19, GPIO21,  GPIO3,  GPIO1, GPIO23}
//  Indices do Vetor = {      0,      1,      2,      3,      4,      5,      6,      7,      8,      9,     10,     11,     12,     13,     14,     15,     16,     17,     19,     21,     22,     23,     24}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CanalPWM[PinPWM] = {     36,     39,     34,     35,     32,     33,     25,     26,     27,     14,     12,     13,     15,      2,      4,     16,     17,     18,     19,     21,      3,      1,     23};
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
//    Variáriveis para leitura
float ValorAnalogico[5];
int   ValorDigital[5];
float tensao[5]; 


void setup() {
Serial.begin(9600);           //  setup serial
//Define pinos como saida digital
pinMode(CanalADC2[0], OUTPUT);  
//Define o pino de leitura analogica como entrada
for(int i = 1; i <=3; i++){
pinMode(CanalADC2[i], INPUT);
}  
//Define o pino de leitura digital como entrada
for(int i = 1; i <=3; i++){
pinMode(CanalADC1[i], INPUT);
}  
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(CanalADC2[0],HIGH);
  delay(500);
  digitalWrite(CanalADC2[0],LOW);
  delay(500);
  // Insere os GPIO4, GPIO15 e GPIO13 com entradas Analógicas
  Serial.println("--------------------");
  Serial.println("Entradas Analógicas!");
  Serial.println("--------------------");
  for(int x = 1; x<=3; x++){
  ValorAnalogico[x] = analogRead(CanalADC2[x]);
  Serial.print("Valor "+String(x)+":");
  Serial.print(ValorAnalogico[x]);
  Serial.print("\t");
  tensao[x] = (ValorAnalogico[x]/4096)*5;
  Serial.print("Tensão "+String(x)+":");
  Serial.print(tensao[x]);
  Serial.print("V");
  Serial.println("\t");
  }
  // Insere os GPIO32, GPIO33, GPIO34, GPIO35 com entradas digitais 
  Serial.println("------------------");
  Serial.println("Entradas digitais!");
  Serial.println("------------------");
  for(int y = 1; y<=3; y++){
  ValorDigital[y] = digitalRead(CanalADC1[y]);
  Serial.print("Valor Digital "+String(y)+":");
  Serial.print(ValorDigital[y]);
  Serial.println("\t");
  }
  Serial.flush();
}
