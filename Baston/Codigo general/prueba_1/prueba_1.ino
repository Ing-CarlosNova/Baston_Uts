#include <Wire.h>
#include <Adafruit_ADXL345_U.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

const int Trigger = 2;
const int Echo = 3;
const int Buzzer = 4;

// GPS
TinyGPSPlus gps;
SoftwareSerial ss(16, 17);

// SIM800L
SoftwareSerial sim800l(27, 26);
const char apn[] = "your_apn";
const char user[] = "";
const char pass[] = "";

// MQTT
const char* mqttServer = "io.adafruit.com";
const int mqttPort = 1883;
const char* mqttUser = "localizadorgpsuts";
const char* mqttPassword = "aio_ZCsg73yEvb8dpE8mg4G6UmsY3FX4";
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, mqttServer, mqttPort, mqttUser, mqttPassword);

Adafruit_MQTT_Publish latitude = Adafruit_MQTT_Publish(&mqtt, "localizadorgpsuts/feeds/latitude");
Adafruit_MQTT_Publish longitude = Adafruit_MQTT_Publish(&mqtt, "localizadorgpsuts/feeds/longitude");

void setup() {
  Serial.begin(9600);
  sim800l.begin(9600);
  ss.begin(9600);

  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Trigger, LOW);

  connectToSIM800L();
}

void loop() {
  // Ultrasonic sensor
  long t = pulseIn(Echo, HIGH);
  long d = t / 59;
  if (d <= 50) {
    playMelody();
  }

  // GPS
  while (ss.available() > 0) {
    gps.encode(ss.read());
    if (gps.location.isUpdated()) {
      sendMQTTData(gps.location.lat(), gps.location.lng());
    }
  }

  delay(100);
}

void playMelody() {
  tone(Buzzer, 1000, 200);
  delay(200);
  noTone(Buzzer);
}

void connectToSIM800L() {
  sim800l.println("AT");
  delay(1000);
  sim800l.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(2000);
  sim800l.println("AT+SAPBR=3,1,\"APN\",\"" + String(apn) + "\"");
  delay(2000);
  sim800l.println("AT+SAPBR=1,1");
  delay(2000);
  sim800l.println("AT+SAPBR=2,1");
  delay(2000);
  sim800l.println("AT+CGATT=1");
  delay(2000);
}

void sendMQTTData(double lat, double lng) {
  if (mqtt.connect()) {
    latitude.publish(lat);
    longitude.publish(lng);
    mqtt.disconnect();
  } else {
    Serial.println("MQTT connection failed");
  }
}
