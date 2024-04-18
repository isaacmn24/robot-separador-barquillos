const int sensorPin = A0; // Pin analógico donde está conectado el sensor Hall
int sensorValue = 0;      // Variable para almacenar el valor leído del sensor

void setup() {
  Serial.begin(9600); // Inicializar comunicación serial a 9600 baudios
}

void loop() {
  sensorValue = analogRead(sensorPin); // Leer el valor del sensor Hall
  //Serial.print("Valor magnético: ");
  Serial.println(sensorValue); // Imprimir el valor leído en el puerto serial
  delay(1000); // Esperar 100 milisegundos antes de la próxima lectura
}
