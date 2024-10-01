#include <WiFi.h>            // Librería para manejar Wi-Fi en ESP32
#include <HTTPClient.h>
#include <Wire.h>
#include <SparkFun_ADXL345.h>
#include <TinyGPS++.h>


// variables para tiempo de muestreo
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;

// Intervalos de tiempo en milisegundos
const unsigned long interval1 = 1000;      // 1 segundo
const unsigned long interval2 = 2000;      // 2 segundos
const unsigned long interval3 = 20000;    // 20 segundos (20,000 ms)

// Pines para el sensor ultrasónico
const int Trigger = 12;         // Pin digital 12 para el Trigger del sensor
const int Echo = 13;            // Pin digital 13 para el Echo del sensor
const int Buzzer = 14;          // Pin digital 14 para el Buzzer o motor de vibración
const int AlertDistance = 100;  // Distancia de alerta en cm (ajústala según necesites)
String dis;

// Datos de conexión Wi-Fi
const char* ssid = "WMSAS-TALLER";        // El nombre de la red Wi-Fi
const char* password = "303wm2021";  // La contraseña de la red Wi-Fi

// URL pública de tu servidor Railway
const char* serverName = "https://nodejs-production-f54f.up.railway.app";  

// Inicialización del acelerómetro
ADXL345 adxl = ADXL345();  // Usa I2C por defecto
String posicion = "";      // Variable para almacenar la posición del acelerómetro

// Inicialización del GPS
TinyGPSPlus gps;
#define RXD2 16  // RX del ESP32 conectado al TX del GPS
#define TXD2 17  // TX del ESP32 conectado al RX del GPS
float dat_latitud, dat_longitud, dat_altura;
String lati,longi;

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando sistema...");

  // Configuración del sensor ultrasónico
  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Trigger, LOW);

  // Configuración del acelerómetro
  adxl.powerOn();
  adxl.setRangeSetting(16);  // Configura el rango a ±16g

  // Configuración del GPS
  // Iniciar la comunicación en UART 1 para GPS
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // Conectar a Wi-Fi
  connectToWiFi();
}

void loop() {

  // Obtener el tiempo actual
  unsigned long currentMillis = millis();

//  // Ejecutar función1 cada 1 segundo
//  if (currentMillis - previousMillis1 >= interval1) {
//    previousMillis1 = currentMillis; 
//    // Llamar a la función que maneja el sensor ultrasónico
//    handleUltrasonic();
//  }
//
//  // Ejecutar función2 cada 2 segundos
//  if (currentMillis - previousMillis2 >= interval2) {
//    previousMillis2 = currentMillis;
//    // Llamar a la función que maneja el acelerómetro y enviar la posición a la base de datos
//    handleAccelerometer();
//  }

  // Ejecutar función3 cada 20 segundos
  if (currentMillis - previousMillis3 >= interval3) {
    previousMillis3 = currentMillis;
    // Llamar a la funciones
    handleUltrasonic();
    handleAccelerometer();
    getGPSData();
    //Enviar datos de GPS y estado del baston
    sendDataToServer(posicion,lati,longi,dis);
  }
  
  delay(1000); // Espera medio segundo antes de la próxima lectura
}

// Función para conectar a Wi-Fi
void connectToWiFi() {
  Serial.println("Conectando al WiFi...");
  WiFi.begin(ssid, password);

  // Bucle hasta que el ESP32 se conecte a la red Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado al WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

// Función para manejar el sensor ultrasónico
void handleUltrasonic() {
  long t;
  long d;

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);

  t = pulseIn(Echo, HIGH);
  d = t / 58.2;
  dis = d;
  
  Serial.print("Distancia: ");
  Serial.print(d);
  Serial.println(" cm");

  if (d < AlertDistance) {
    digitalWrite(Buzzer, HIGH);
    Serial.println("¡Obstáculo detectado!");
  } else {
    digitalWrite(Buzzer, LOW);
  }
}

// Función para manejar el acelerómetro con verificación de posición
void handleAccelerometer() {
  int zSum = 0;
  const int numReadings = 100;

  // Calibrar el eje Z del acelerómetro
  for (int i = 0; i < numReadings; i++) {
    int x, y, z;
    adxl.readAccel(&x, &y, &z);
    zSum += z;
    delay(10);
  }

  float zOffset = zSum / (float)numReadings;

  // Leer y procesar los valores del acelerómetro
  int x, y, z;
  adxl.readAccel(&x, &y, &z);
  float zCalibrated = z - zOffset;

  Serial.print("Aceleración X: ");
  Serial.print(x);
  Serial.print(" | Y: ");
  Serial.print(y);
  Serial.print(" | Z: ");
  Serial.println(zCalibrated);

  // Verificar la posición del bastón
  if (abs(x) <= 5 && abs(y) <= 5) {
    posicion = "CAIDO";
    Serial.println("¡El bastón está CAIDO!");

  } else {
    posicion = "PARADO";
    Serial.println("¡El bastón está PARADO!");

  }
  
  // Enviar la posición a la base de datos
  //sendDataToServer(posicion);
}

// Función para obtener los datos del GPS
void getGPSData() {
  // Leer datos del GPS desde Serial2
  while (Serial2.available() > 0) {
    gps.encode(Serial2.read());

    if (gps.location.isUpdated()) {
      dat_latitud = gps.location.lat();
      dat_longitud = gps.location.lng();
      dat_altura = gps.altitude.meters();

      Serial.print("Latitud: ");
      Serial.println(dat_latitud, 6);
      lati = dat_latitud;
      Serial.print("Longitud: ");
      Serial.println(dat_longitud, 6);
      longi = dat_longitud;
      Serial.print("Altura: ");
      Serial.print(dat_altura);
      Serial.println(" metros");

      Serial.print("Número de satélites: ");
      Serial.println(gps.satellites.value());

      Serial.println();
    }
  }
}

// Función para enviar datos al servidor
void sendDataToServer(String pos, String latit,String longit, String dis) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

// Iniciar la solicitud POST
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

   // Crear un objeto JSON para enviar los datos
    String jsonData = "{\"lon\": " + String(longit) + 
                      ", \"lat\": " + String(latit) + 
                      ", \"pos\": " + String(pos) + 
                      ", \"ult\": " + String(dis) + "}";

    // Enviar la solicitud POST
    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);  // Código de respuesta HTTP
      Serial.println(response);  // Respuesta del servidor
    } else {
      Serial.println("Error en la solicitud POST");
    }

    http.end();  // Finalizar la conexión
  }
    //// Construir la URL con la posición
    //String serverPath = "http://192.168.0.107/prueba.php?lon=" + longit + "&lat=" + latit + "&pos=" + pos + "$ult=" + dis;
    //http.begin(serverPath);

    //int httpResponseCode = http.GET();

    //if (httpResponseCode > 0) {
    //  String response = http.getString();
    //  Serial.println(httpResponseCode);
    //  Serial.println(response);
    //} else {
    //  Serial.print("Error en la solicitud: ");
    //  Serial.println(httpResponseCode);
    //}

    //http.end();
  } else {
    Serial.println("Error al conectar al WiFi");
  }
}
