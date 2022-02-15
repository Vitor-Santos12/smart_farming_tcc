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
      digitalWrite(CanalADC2[0],HIGH);
      ContDelay(50);
      digitalWrite(CanalADC2[0],LOW);
      ContDelay(50);
}
