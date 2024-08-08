const int Trigger = 12;   // Pin digital 12 para el Trigger del sensor
const int Echo = 13;      // Pin digital 13 para el Echo del sensor
const int AlertDistance = 100; // Distancia de alerta en cm (ajústala según necesites)

void setup() {
  Serial.begin(9600);       // Inicializamos la comunicación serial
  pinMode(Trigger, OUTPUT); // Pin como salida
  pinMode(Echo, INPUT);     // Pin como entrada
  digitalWrite(Trigger, LOW); // Inicializamos el pin con 0
}

void loop() {
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
    // Aquí puedes añadir código para activar una alarma, vibración, etc.
    Serial.println("¡Obstáculo detectado!");
  }

  delay(100);               // Hacemos una pausa de 100ms
}
