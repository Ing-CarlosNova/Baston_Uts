#include <SPI.h>
#include <Wire.h>
#include <SparkFun_ADXL345.h>

ADXL345 adxl = ADXL345();

void setup()
{
   Serial.begin(9600);            
   Serial.println("Iniciar");
   Serial.println();
   adxl.powerOn();           
   adxl.setRangeSetting(16);       //Definir el rango, valores 2, 4, 8 o 16
}

void loop()
{
   //leer los valores e imprimirlos
   int x, y, z;
   adxl.readAccel(&x, &y, &z); 
   Serial.print(x);
   Serial.print(", ");
   Serial.print(y);
   Serial.print(", ");
   Serial.println(z);
}
//https://solectroshop.com/es/content/74-como-usar-un-acelerometro-de-3-ejes-adxl345-con-arduino