#include <WiFi.h>  // Librería para manejar Wi-Fi en ESP32

const char* ssid = "RUSSI";        // El nombre de la red Wi-Fi
const char* password = "28268754";  // La contraseña de la red Wi-Fi

void setup() {
  // Inicializa la comunicación serie para depuración
  Serial.begin(115200);
  
  // Espera un segundo para iniciar la conexión
  delay(1000);
  
  // Inicia la conexión Wi-Fi
  Serial.println("Conectando al WiFi...");
  WiFi.begin(ssid, password);

  // Bucle hasta que el ESP32 se conecte a la red Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Una vez conectado, imprime la dirección IP
  Serial.println("");
  Serial.println("Conectado al WiFi");

}

void loop() {
  // El código dentro de loop se ejecutará continuamente
}