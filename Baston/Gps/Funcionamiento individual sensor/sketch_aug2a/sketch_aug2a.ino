#include <SoftwareSerial.h>

SoftwareSerial gps(3,1);

char dato=' ';

void setup()
{
 Serial.begin(115200);            
 gps.begin(9600); 
}


void loop()
{
  if(gps.available())
  {
    dato=gps.read();
    Serial.print(dato);
  }
}

// pagina:https://naylampmechatronics.com/blog/18_tutorial-modulo-gps-con-arduino.html
