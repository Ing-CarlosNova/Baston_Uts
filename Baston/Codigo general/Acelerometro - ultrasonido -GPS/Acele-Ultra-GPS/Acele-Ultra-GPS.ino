#include <Wire.h>
#include <SparkFun_ADXL345.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

// Pines para el sensor ultrasónico
const int Trigger = 12;   // Pin digital 12 para el Trigger del sensor
const int Echo = 13;      // Pin digital 13 para el Echo del sensor
const int Buzzer = 14;    // Pin digital 14 para el Buzzer o motor de vibración
const int AlertDistance = 100; // Distancia de alerta en cm (ajústala según necesites)

// Inicialización del acelerómetro
ADXL345 adxl = ADXL345(); // Usa I2C por defecto

// Inicialización del GPS
TinyGPSPlus gps;
SoftwareSerial ss(1, 3);  // Pines RX, TX para la comunicación con el GPS

void setup() {
  Serial.begin(115200);
  ss.begin(9600); // Inicializa la comunicación con el GPS
  
  Serial.println("Iniciando sistema...");

  // Configuración del sensor ultrasónico
  pinMode(Trigger, OUTPUT); // Pin como salida
  pinMode(Echo, INPUT);     // Pin como entrada
  pinMode(Buzzer, OUTPUT);  // Pin como salida para el Buzzer/motor de vibración
  digitalWrite(Trigger, LOW); // Inicializamos el pin con 0

  // Configuración del acelerómetro
  adxl.powerOn();            // Encender el ADXL345
  adxl.setRangeSetting(16);   // Configura el rango a ±16g
}

void loop() {
  // *** Lectura del sensor ultrasónico ***
  long t; // Tiempo que demora en llegar el eco
  long d; // Distancia en centímetros

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);    // Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);

  t = pulseIn(Echo, HIGH);  // Obtenemos el ancho del pulso
  d = t / 58.2;             // Convertimos el tiempo a distancia en cm
  
  Serial.print("Distancia: ");
  Serial.print(d);          // Enviamos serialmente el valor de la distancia
  Serial.println(" cm");
  
  if (d < AlertDistance) {
    // Activamos el buzzer o motor de vibración
    digitalWrite(Buzzer, HIGH);
    Serial.println("¡Obstáculo detectado!");
  } else {
    // Desactivamos el buzzer o motor de vibración
    digitalWrite(Buzzer, LOW);
  }

  // *** Lectura del acelerómetro ***
  int x, y, z;
  adxl.readAccel(&x, &y, &z);
  
  // Imprimir los valores de aceleración en el monitor serial
  Serial.print("Aceleración X: ");
  Serial.print(x);
  Serial.print(" | Y: ");
  Serial.print(y);
  Serial.print(" | Z: ");
  Serial.println(z);

  // *** Lectura del GPS ***
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }

  Serial.print(F("Location: "));
  if (gps.location.isValid()) {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    Serial.print('\n');
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid()) {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid()) {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.println();

  delay(500); // Espera medio segundo antes de la próxima lectura
}
