#include <SoftwareSerial.h> //Se incluye la librería de "SoftwareSerial" 
SoftwareSerial SIM800L(7, 8); // Se define los pines para la comunicación serial 
void setup() { SIM800L.begin(9600); // Configuración de la velocidad en baudios del módulo SIM900 
Serial.begin(9600); // Configuración de la velocidad en baudios del monitor serie (Arduino) 
} 
void loop() { 
  if (Serial.available()>0) // Verifica si hay datos en el puerto serie de Arduino. 
      switch(Serial.read()) 
        { 
          case 's': // Si hay algo en el monitor serie, si es una letra "s" se ejecutara los comando AT que se definen en la función "envio_sms" 
          envio_sms();
          break; 
          case 'c': // Pero si una letra "c" se ejecutara los comando AT que se definen en la función "hacer_llamada" 
          hacer_llamada(); 
          break; 
        } 
if (SIM800L.available()>0) 
  Serial.write(SIM800L.read()); // Imprime los datos recopilados del puerto serie 
} 
  void envio_sms() 
    { 
      Serial.println("Enviado sms..."); 
      SIM800L.print("AT+CMGF=1\r"); // Establecer en modo envió de sms 
      delay(100); /* Se le asigna el numero, debes cambiar la clave de acuerdo al país si eres de México solo agrega el numero al que enviaras el sms */ 
      SIM800L.print("AT+CMGS=\"+5255.........\"\r"); 
      delay(200); 
      SIM800L.print("Hola, Saludos de UNIT Electronics"); // Contenido que llevara el sms 
      SIM800L.print("\r"); 
      delay(500); 
      SIM800L.print((char)26); 
      delay(100); 
      SIM800L.println(); 
      Serial.println("SMS Enviado..."); 
      delay(500); } 
void hacer_llamada() { /* Se marca al numero, debes cambiar la clave de acuerdo al país si eres de México solo agrega el numero al que llamaras */ 
SIM800L.println("ATD+52155.........;"); 
delay(100); 
SIM800L.println();
}

// pagina: https://uelectronics.com/producto/sim800l-gsm-gprs-v2-0-con-antena/