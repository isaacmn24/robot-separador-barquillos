#define PUL 7               // Cada pulso a este pin es un paso (o micropaso) del motor
#define DIR 6               // Define la dirección del motor, HIGH es horario
#define botonReinicio 2;    // Botón para volver el motor a posición inicial (0 grados)
#define botonBusqueda 3;    // Botón para iniciar secuencia de clasificación de barquillos
#define sensorHall A0       // Pin de lectura del sensor Hall

const int stepsPorRev = 200;                    // Motor estándar de 200 pasos por revolución
const int microsteps = 32;                      // Cantidad de micropasos configurados manualmente en el driver
const int Za = 16;                              // Dientes del engranaje central (Sol)
const int Zc = 48;                              // Dientes del engranaje anular
const float relacionEngranes = Zc/Za + 1;       // Relación de engranajes para sistema planetario con carrier fijo
const float pasosPorRev = stepsPorRev * microsteps * relacionEngranes;        // Pasos que requiere dar el motor para dar una revolución completa
const int delayStep = 500;                      // Tiempo en microsegundos que hay entre cada pulso de paso. Entre más pequeño, más rápido
float posActual = 0;                            // Posición actual del motor que se irá actualizando
const int anguloLimite = 55;                    // Angulo del limite del arco a recorrer
const int limitePosicion = floor(pasosPorRev/360.0*anguloLimite);                // Pasos que hay desde el 0 hasta el (aproximadamente) angulo limite
unsigned int cantidadPasos = 0;                 // Cuenta cuantos pasos se ha movido 
int sensorHallThreshold = 529;                  // Umbral para detectar campo magnético

void setup() {
  pinMode(PUL, OUTPUT);
  pinMode(DIR, OUTPUT);
  //pinMode(sensorHall, INPUT); // Configura el pin del sensor Hall como entrada
  Serial.begin(9600);
}

void loop() {
  digitalWrite(DIR, 0);        // Primero gira hacia la derecha
  for (int paso = 0; paso < limitePosicion; paso++)       // Gira hasta el final
  {
    digitalWrite(PUL, HIGH);
    delayMicroseconds(delayStep);
    digitalWrite(PUL, LOW);
    delayMicroseconds(delayStep);
    
    // Leer valor del sensor Hall
    int hallValue = analogRead(sensorHall);

    Serial.println(hallValue);

    // Si el valor del sensor Hall indica la presencia de un campo magnético, detener el motor
    if (hallValue > sensorHallThreshold) {
      break; // Salir del bucle
    }
  }
  
  digitalWrite(DIR, 1);        // Luego gira hacia la izquierda
  for (int paso = 0; paso < 2 * limitePosicion; paso++)       // Gira hasta el otro lado (doble recorrido)
  {
    digitalWrite(PUL, HIGH);
    delayMicroseconds(delayStep);
    digitalWrite(PUL, LOW);
    delayMicroseconds(delayStep);
    
    // Leer valor del sensor Hall
    int hallValue = analogRead(sensorHall);

    // Si el valor del sensor Hall indica la presencia de un campo magnético, detener el motor
    if (hallValue > sensorHallThreshold) {
      break; // Salir del bucle
    }
  }
  
  digitalWrite(DIR, 0);        // Primero gira hacia la derecha
  for (int paso = 0; paso < limitePosicion; paso++)       // Gira hasta el final
  {
    digitalWrite(PUL, HIGH);
    delayMicroseconds(delayStep);
    digitalWrite(PUL, LOW);
    delayMicroseconds(delayStep);
    
    // Leer valor del sensor Hall
    int hallValue = analogRead(sensorHall);

    // Si el valor del sensor Hall indica la presencia de un campo magnético, detener el motor
    if (hallValue < sensorHallThreshold) {
      break; // Salir del bucle
    }
  }
  delay(5000);
}



