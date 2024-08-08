#include <Wire.h>
#include <SparkFun_ADXL345.h>

ADXL345 adxl = ADXL345(); // Usa I2C por defecto

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando ADXL345...");

  adxl.powerOn();            // Encender el ADXL345

  // Configura el rango a ±16g
  adxl.setRangeSetting(16);   // Rango de 2g, 4g, 8g, o 16g
}

void loop() {
  // Leer los valores de aceleración
  int x, y, z;
  adxl.readAccel(&x, &y, &z);
  
  // Imprimir los valores en el monitor serial
  Serial.print("X: ");
  Serial.print(x);
  Serial.print(" | Y: ");
  Serial.print(y);
  Serial.print(" | Z: ");
  Serial.println(z);
  
  delay(500); // Espera medio segundo antes de la próxima lectura
}