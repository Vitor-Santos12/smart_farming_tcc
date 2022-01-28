/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/

// Desenvolvendo o perfil do ESP 32 através da variáveis
// Canais PWM

// Numeros de pinos PWM
int PinPWM = 27
//  Valores do Indices { 1,2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,19,21,22,23,24,26} 
int CanalPWM[PinPWM] = { 5,8,10,11,12,13,14,15,16,17,18,20,21,22,24,25,27,35,38,42,40,41,36}

// Pinos ADC -> Conversor analógico-digital 
int PinADC1
//    Valores do Indices { GPIO36,2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,19,21,22,23,24,26} 
int CanalADC1[PinADC1] = { 5     ,8,10,11,12,13,14,15,16,17,18,20,21,22,24,25,27,35,38,42,40,41,36}


int PinInput[input] = 


// the setup function runs once when you press reset or power the board

int LED_BUILTIN =2;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
