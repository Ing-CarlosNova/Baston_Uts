#include <HCSR04.h>

#include <HCSR04.h>

#include <SoftwareSerial.h>

// Configurar los pines para la comunicación serial con el módulo SIM800L
SoftwareSerial SIM800L(13, 12); // RX, TX

void setup() {
  // Inicializar la comunicación serial con el módulo SIM800L
  SIM800L.begin(9600);
  
  // Inicializar la comunicación serial para el monitor serie
  Serial.begin(9600);
  
  // Esperar a que el módulo SIM800L esté listo
  delay(1000);
  
  // Enviar un comando AT básico y esperar la respuesta
  testATCommand();
}

void loop() {
  // Verificar si hay datos del módulo SIM800L y enviarlos al monitor serie
  if (SIM800L.available() > 0) {
    Serial.write(SIM800L.read());
  }
  
  // Verificar si hay datos del monitor serie y enviarlos al módulo SIM800L
  if (Serial.available() > 0) {
    SIM800L.write(Serial.read());
  }
}

void testATCommand() {
  Serial.println("Enviando comando AT...");
  SIM800L.println("AT"); // Enviar el comando AT
  delay(1000); // Esperar un segundo por la respuesta
  
  while (SIM800L.available()) {
    char c = SIM800L.read();
    Serial.write(c); // Imprimir la respuesta del módulo SIM800L en el monitor serie
  }
  
  Serial.println("Comando AT enviado.");
}
