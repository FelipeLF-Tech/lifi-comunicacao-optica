/*
 * ==================================================
 * PROJETO LI-FI - MÓDULO EMISSOR (TX)
 * ==================================================
 * LIGAÇÕES:
 * - Pino Digital 3 -> Resistência 330Ω -> Gate do MOSFET (Pino 1)
 */

const int TX_PIN = 3;

// Velocidade da transmissão (50ms garante estabilidade)
const int BIT_DELAY = 50; 

void setup() {
  Serial.begin(9600); 
  pinMode(TX_PIN, OUTPUT);
  digitalWrite(TX_PIN, LOW); // Garante emissor apagado no início
  
  Serial.println("=========================================");
  Serial.println("       MÓDULO EMISSOR (TX) PRONTO        ");
  Serial.println("=========================================");
  Serial.println("Digite a sua mensagem e prima ENTER...");
  Serial.println("-----------------------------------------");
}

void loop() {
  // Verifica se o utilizador digitou algo no Monitor Serial
  if (Serial.available() > 0) {
    String msg = Serial.readStringUntil('\n');
    msg.trim(); // Remove espaços vazios no final
    
    if (msg.length() > 0) {
      Serial.print("\n[ENVIANDO] : ");
      Serial.println(msg);
      
      // Ciclo para transmitir caractere a caractere
      for (int i = 0; i < msg.length(); i++) {
        char c = msg[i];
        
        // --- START BIT (Avisa o Recetor que uma letra vai começar) ---
        digitalWrite(TX_PIN, HIGH); 
        delay(BIT_DELAY); // Espera o tempo de 1 bit inteiro
        
        // --- DADOS (8 Bits que formam a letra) ---
        for (int b = 0; b < 8; b++) {
          int bit_atual = bitRead(c, b); 
          
          if (bit_atual == 1) {
            digitalWrite(TX_PIN, HIGH); // Luz acesa  = 1
          } else {
            digitalWrite(TX_PIN, LOW);  // Luz apagada = 0
          }
          delay(BIT_DELAY);
        }
        
        // --- STOP BIT (Fim da letra) ---
        digitalWrite(TX_PIN, LOW); // Apaga a luz para descansar e separar
        delay(BIT_DELAY * 2);      // Tempo duplo no escuro para garantir sincronização
      }
      
      Serial.println("[STATUS]   : Mensagem enviada com sucesso!");
    }
  }
}