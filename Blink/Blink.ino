
// #Biblioteca
#include <iostream>
#include <string>

// Desenvolvendo o perfil do ESP 32 através da variáveis
// Canais PWM
int PinPWM = 27;
// Numeros de pinos PWM---------------------------------------------------------------------------
//  Pinos na placa =   {     VP,     VN,    D34,    D35,    D32,    D33,    D25,    D26,    D27,    D14,    D12,    D13,    D15,    D2,      D4,    RX2,    TX2,    D18,    D19,    D21,    RX0,    TX0,    D23} 
//  Valores do Indices={ GPIO36, GPIO39, GPIO34, GPIO35, GPIO32, GPIO33, GPIO25, GPIO26, GPIO27, GPIO14, GPIO12, GPIO13, GPIO15,  GPIO2,  GPIO4, GPIO16, GPIO17, GPIO18, GPIO19, GPIO21,  GPIO3,  GPIO1, GPIO23}
//  Indices do Vetor = {      0,      1,      2,      3,      4,      5,      6,      7,      8,      9,     10,     11,     12,     13,     14,     15,     16,     17,     19,     21,     22,     23,     24}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CanalPWM[PinPWM] = {      5,      8,     10,     11,     12,     13,      14,    15,     16,     17,     18,     20,     21,     22,     24,     25,     27,     35,     38,     42,     40,      41,    36};
// Pinos ADC1 -> Conversor analógico-digital------------------------------------------------------
// Resolução de 12 bits com até 4096 valores distintos
// Valores de entrada opcionalmente 0-1V, 0-1.34V, 0-2V ou 0-3.6V
int PinADC1 = 7;
//    Pinos na placa =   {     VP,     VN,    D32,    D33,    D34,    D35} 
//    Valores do Indices { GPIO36, GPIO39, GPIO32, GPIO33, GPIO34, GPIO35}
//    Indices do Vetor = {      0,      1,      2,      3,      4,      5} 
//------------------------------------------------------------------------------------------------
int CanalADC1[PinADC1] = {      5,      8,     12,     13,     10,     11};
// Pinos ADC2 -> Conversor analógico-digital------------------------------------------------------
int PinADC2 = 9;
//    Pinos na placa =   {     D4,     D2,    D15,    D13,    D12,    D14,    D27,    D25,     D26} 
//    Valores do Indices {  GPIO4,  GPIO2, GPIO15, GPIO13, GPIO12, GPIO14, GPIO27, GPIO25,  GPIO26}
//    Indices do Vetor = {     0,      1,       2,      3,      4,      5,      6,      7,       8} 
//------------------------------------------------------------------------------------------------
int CanalADC2[PinADC2] = {     24,    22,      21,     20,     18,     17,     16,     14,      15};
// Pinos GPIO DAC(Digital to Analog Converter)----------------------------------------------------
// Conversore digitais com saidas de tensão analógica
int PinGPIODAC = 3;
//    Pinos na placa         = {     D25,     D26} 
//    Valores do Indices       {  GPIO25,  GPIO26}
//    Indices do Vetor       = {       0,       1} 
//------------------------------------------------------------------------------------------------
int CanalGPIODAC[PinGPIODAC] = {      14,      15};
// Pinos apenas de Entrada -----------------------------------------------------------------------
int PinIn = 5;
//    Pinos na placa =   {    D34,    D35,     VP,     VN} 
//    Valores do Indices { GPIO34, GPIO35, GPIO36, GPIO39}
//    Indices do Vetor = {      0,      1,      2,      3} 
//-------------------------------------------------------
int CanalIn[PinIn] =     {     10,     11,     12,     13};

// the setup function runs once when you press reset or power the board

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(2, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
