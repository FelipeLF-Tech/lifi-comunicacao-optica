/*
 * ==================================================
 * PROJETO LI-FI - MÓDULO RECETOR (RX) PARA ARDUINO
 * ==================================================
 * LIGAÇÕES (Arduino Uno / Nano):
 * - VCC (Cátodo do Fotodíodo e Resistor 10k) -> Pino 5V
 * - GND -> Pino GND
 * - SINAL (Ponto de união dos Coletores dos BC548) -> Pino Digital 2
 */

const int RX_PIN = 2; // Pino Digital 2 no Arduino
const int BIT_DELAY = 50; 

// Variáveis para agrupar a mensagem
String mensagemRecebida = "";
unsigned long ultimoTempoChar = 0;
const int TIMEOUT_MENSAGEM = 500; // 500ms de silêncio = fim da mensagem

void setup() {
  Serial.begin(9600); 
  pinMode(RX_PIN, INPUT); 
  
  Serial.println("=========================================");
  Serial.println("     MÓDULO RECETOR (ARDUINO) PRONTO     ");
  Serial.println("=========================================");
  Serial.println("A aguardar sinais de luz...");
  Serial.println("-----------------------------------------");
  
  // Teste de segurança para luz ambiente no arranque
  delay(1000);
  if (digitalRead(RX_PIN) == LOW) {
    Serial.println("\n[ALERTA] Detetada luz ambiente a interferir com o sensor!");
    Serial.println("Tente criar uma sombra em redor do fotodiodo preto.");
  }
}

void loop() {
  // O recetor fica à espera, num ciclo infinito, por um "Start Bit"
  // No nosso circuito: Luz recebida = Pino LOW (0V)
  if (digitalRead(RX_PIN) == LOW) {
    
    // Assim que deteta a luz, espera METADE do tempo do bit.
    // Isso é feito para ler os dados bem no "meio" do pulso de luz, 
    // evitando erros se o pulso de luz alargar um pouco.
    delay(BIT_DELAY / 2);
    
    // Confirma se ainda é o Start Bit (anti-ruído falso)
    if (digitalRead(RX_PIN) == LOW) {
      byte char_recebido = 0;
      
      // Lê os 8 bits de dados
      for (int b = 0; b < 8; b++) {
        delay(BIT_DELAY); // Salta para o meio do próximo bit
        
        int bit_lido = digitalRead(RX_PIN);
        
        if (bit_lido == LOW) { // Luz acesa = 1
          bitWrite(char_recebido, b, 1);
        } else {               // Escuro = 0
          bitWrite(char_recebido, b, 0);
        }
      }
      
      // O ciclo dos 8 bits acabou, espera passar o "Stop Bit"
      delay(BIT_DELAY);
      
      // Guarda a letra na memória em vez de imprimir imediatamente
      mensagemRecebida += (char)char_recebido;
      ultimoTempoChar = millis(); // Regista o momento em que a última letra chegou
    }
  }

  // Verifica se temos uma mensagem acumulada E se já passou o tempo limite (silêncio)
  if (mensagemRecebida.length() > 0 && (millis() - ultimoTempoChar > TIMEOUT_MENSAGEM)) {
    Serial.print("[MENSAGEM RECEBIDA] : ");
    Serial.println(mensagemRecebida);
    
    // Limpa a variável para estar pronta para a próxima mensagem
    mensagemRecebida = "";
  }
}