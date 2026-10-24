/*
 * ==================================================
 * PROJETO LI-FI - MÓDULO RECETOR (RX) COM ECRÃ LCD 16X2
 * ==================================================
 * Atualizado: Agora imprime bit a bit e confirma no Serial
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int RX_PIN = 2; 
const int BIT_DELAY = 50; 

LiquidCrystal_I2C lcd(0x27, 16, 2); 

String mensagemRecebida = "";
unsigned long ultimoTempoChar = 0;
const int TIMEOUT_MENSAGEM = 500; 

void setup() {
  Serial.begin(9600); 
  pinMode(RX_PIN, INPUT); 
  
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print(" SISTEMA LI-FI ");
  lcd.setCursor(0, 1);
  lcd.print("A aguardar luz...");
  
  Serial.println("=========================================");
  Serial.println("    MÓDULO RECETOR COM LCD PRONTO        ");
  Serial.println("=========================================");
}

void loop() {
  // Aguarda Start Bit
  if (digitalRead(RX_PIN) == LOW) {
    delay(BIT_DELAY / 2);
    
    if (digitalRead(RX_PIN) == LOW) {
      byte char_recebido = 0;
      
      if (mensagemRecebida.length() == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("A receber...");
      }
      
      // Lê os 8 bits
      for (int b = 0; b < 8; b++) {
        delay(BIT_DELAY); 
        int bit_lido = digitalRead(RX_PIN);
        
        if (bit_lido == LOW) { 
          bitWrite(char_recebido, b, 1);
        } else {               
          bitWrite(char_recebido, b, 0);
        }
      }
      
      delay(BIT_DELAY); // Stop Bit
      
      mensagemRecebida += (char)char_recebido;
      ultimoTempoChar = millis(); 
      
      // DEBUG: Confirmação no Serial bit a bit
      Serial.print("Char recebido: ");
      Serial.println((char)char_recebido);
      
      lcd.setCursor(0, 1);
      lcd.print(mensagemRecebida);
    }
  }

  // Finalização da mensagem
  if (mensagemRecebida.length() > 0 && (millis() - ultimoTempoChar > TIMEOUT_MENSAGEM)) {
    Serial.print("[MENSAGEM FINAL] : ");
    Serial.println(mensagemRecebida);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Recebido:");
    lcd.setCursor(0, 1);
    lcd.print(mensagemRecebida.substring(0, 16));
    
    mensagemRecebida = "";
  }
}