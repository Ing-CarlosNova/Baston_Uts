#include <WiFi.h>            // Librería para manejar Wi-Fi en ESP32
#include <HTTPClient.h>
#include <Wire.h>
#include <SparkFun_ADXL345.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Pines para el sensor ultrasónico
const int Trigger = 12;         // Pin digital 12 para el Trigger del sensor
const int Echo = 13;            // Pin digital 13 para el Echo del sensor
const int Buzzer = 14;          // Pin digital 14 para el Buzzer o motor de vibración
const int AlertDistance = 100;  // Distancia de alerta en cm (ajústala según necesites)

// Datos de conexión Wi-Fi
const char* ssid = "RUSSI";        // El nombre de la red Wi-Fi
const char* password = "28268754";  // La contraseña de la red Wi-Fi

// Inicialización del acelerómetro
ADXL345 adxl = ADXL345();  // Usa I2C por defecto
String posicion = "";      // Variable para almacenar la posición del acelerómetro

// Inicialización del GPS
TinyGPSPlus gps;
SoftwareSerial ss(4, 3);  // RX en el pin 4, TX en el pin 3
float dat_latitud, dat_longitud, dat_altura;

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
  ss.begin(9600); // Velocidad de la comunicación serie con el GPS

  // Conectar a Wi-Fi
  connectToWiFi();
}

void loop() {
  // Llamar a la función que maneja el sensor ultrasónico
  handleUltrasonic();

  // Llamar a la función que maneja el acelerómetro y enviar la posición a la base de datos
  handleAccelerometer();

  // Llamar a la función que maneja el GPS
  getGPSData();

  delay(500); // Espera medio segundo antes de la próxima lectura
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
  sendDataToServer(posicion);
}

// Función para obtener los datos del GPS
void getGPSData() {
  while (ss.available() > 0) {
    gps.encode(ss.read());

    if (gps.location.isUpdated()) {
      dat_latitud = gps.location.lat();
      dat_longitud = gps.location.lng();
      dat_altura = gps.altitude.meters();

      Serial.print("Latitud: ");
      Serial.println(dat_latitud, 6);
      Serial.print("Longitud: ");
      Serial.println(dat_longitud, 6);
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
void sendDataToServer(String pos) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Construir la URL con la posición
    String serverPath = "http://192.168.1.106/prueba.php?pos=" + pos;
    http.begin(serverPath);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error en la solicitud: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Error al conectar al WiFi");
  }
}
