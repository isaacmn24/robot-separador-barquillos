#define PUL 7               // Cada pulso a este pin es un paso (o micropaso) del motor
#define DIR 6               // Define la dirección del motor, HIGH es horario
#define botonReinicio 2    // Botón para volver el motor a posición inicial (0 grados)
#define botonBusqueda 3    // Botón para iniciar secuencia de clasificación de barquillos
#define HallDerecho A1      // Pin de lectura del sensor Hall
#define HallIzquierdo A2

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

void setup() {
  pinMode(PUL, OUTPUT);
  pinMode(DIR, OUTPUT);
  Serial.begin(115200);
  Serial.print("Pasos"); Serial.print(","); Serial.print("HallDerecho"); Serial.print(","); Serial.print("HallIzquierdo"); Serial.print(","); Serial.println("diferenciaHall");
}

void loop() {
  digitalWrite(DIR,0);        // Luego gira hacia la izquierda
  for (int paso = 0; paso < 2*limitePosicion; paso++)       // Gira hasta el otro lado (doble recorrido)
  {
    //Serial.print(paso); Serial.print(","); Serial.print(analogRead(HallDerecho)); Serial.print(","); Serial.print(analogRead(HallIzquierdo)); Serial.print(","); Serial.println(analogRead(HallDerecho)-analogRead(HallIzquierdo));
    int hola = analogRead(A1);
    digitalWrite(PUL,HIGH);
    delayMicroseconds(delayStep);
    digitalWrite(PUL,LOW);
    delayMicroseconds(delayStep);
  }
  delay(20000);
  digitalWrite(DIR,1);
  for (int paso = 0; paso < 2*limitePosicion; paso++)       // Gira hasta el otro lado (doble recorrido)
  {
    Serial.print(paso); Serial.print(","); Serial.print(analogRead(HallDerecho)); Serial.print(","); Serial.print(analogRead(HallIzquierdo)); Serial.print(","); Serial.println(analogRead(HallDerecho)-analogRead(HallIzquierdo));
    //Serial.println(paso);
    digitalWrite(PUL,HIGH);
    delayMicroseconds(delayStep);
    digitalWrite(PUL,LOW);
    delayMicroseconds(delayStep);
  }

  
}

