//void EntradasAnalogicas();
//void EntradasDigitais();
//
//// Função para entradas analógicas
//void EntradasAnalogicas(){
//  // Insere os GPIO4, GPIO15 e GPIO13 com entradas Analógicas
////  Serial.println("\n--------------------");
////  Serial.println("Entradas Analógicas!");
////  Serial.println("--------------------");
//  MsgValores = String("\n***Esp 32-2 Comunica que***\n");
//  
//  for(int x = 1; x<=3; x++){
//  ValorAnalogico[x] = analogRead(CanalADC2[x]);
////  Serial.print("Valor Analogico"+String(x)+":");
////  Serial.print(ValorAnalogico[x]);
////  Serial.print("\t");
//  MsgValores = MsgValores + String("\nValor Analogico"+String(x)+":"+String(ValorAnalogico[x])+"\t");  
//  tensao[x] = (ValorAnalogico[x]/4096)*5;
////  Serial.print("Tensão "+String(x)+":");
////  Serial.print(tensao[x]);
////  Serial.print("V");
////  Serial.println("\t");
//  MsgValores = MsgValores + String("Tensão"+String(x)+":"+String(tensao[x])+"V\n");
//  }
//  //Serial.print(MsgValores);
//}
//
//// Função para entradas digitais
//void EntradasDigitais(){
//  // Insere os GPIO32, GPIO33, GPIO34, GPIO35 com entradas digitais 
////Serial.println("\n------------------");
////Serial.println("Entradas digitais!");
////Serial.println("------------------");
//  for(int y = 1; y<=3; y++){
//  ValorDigital[y] = digitalRead(CanalADC1[y]);
////  Serial.print("Valor Digital "+String(y)+":");
////  Serial.print(ValorDigital[y]);
////  Serial.println("\t");
//  MsgValores = MsgValores + String("\nValor Digital"+String(y)+":"+String(ValorDigital[y])+"\n");
//  }
//  //Serial.print(MsgValores);
//}
