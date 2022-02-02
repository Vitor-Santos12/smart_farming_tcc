//protótipo das funções
void piscaled();
void ContDelay();
// Função apenas para sinalizar que está tudo correto com programa

// Contagem para cada 10 mili segundos...
void ContDelay(int s){
for (int i = 0; i < s; i++){ 
delay(10);
}
  
}
 
void piscaled(){
    digitalWrite(2,HIGH);
    ContDelay(10); // Cont Delay 50 x 10 mili segundo
    digitalWrite(2,LOW);
    ContDelay(10);
}
