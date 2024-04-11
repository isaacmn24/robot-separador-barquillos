#define PUL 7               // Cada pulso a este pin es un paso (o micropaso) del motor
#define DIR 6               // Define la dirección del motor, HIGH es horario
#define botonReinicio 2     // Botón para volver el motor a posición inicial (0 grados)
#define botonBusqueda 3     // Botón para iniciar secuencia de clasificación de barquillos
#define sensorHall A0       // Pin de lectura del sensor Hall

const int stepsPorRev = 200;                    // Motor estándar de 200 pasos por revolución
const int microsteps = 32;                      // Cantidad de micropasos configurados manualmente en el driver
const int Za = 16;                              // Dientes del engranaje central (Sol)
const int Zc = 48;                              // Dientes del engranaje anular
const float relacionEngranes = Zc/Za + 1;       // Relación de engranajes para sistema planetario con carrier fijo
const float pasosPorRev = stepsPorRev * microsteps * relacionEngranes;        // Pasos que requiere dar el motor para dar una revolución completa
const int delayStep = 50;                       // Tiempo en microsegundos que hay entre cada pulso de paso. Entre más pequeño, más rápido
float posActual = 0;                            // Posición actual del motor que se irá actualizando
const int anguloLimite = 55;                    // Angulo del limite del arco a recorrer
const int limitePosicion = floor(pasosPorRev/360.0*anguloLimite);                // Pasos que hay desde el 0 hasta el (aproximadamente) angulo limite
unsigned int cantidadPasos = 0;                 // Cuenta cuantos pasos se ha movido 

void setup() {
  pinMode(PUL, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(botonReinicio, INPUT);
  pinMode(botonBusqueda, INPUT);
}

void loop() {
  bool modoReinicio = digitalRead(botonReinicio);
  bool modoBusqueda = digitalRead(botonBusqueda);
  if (modoReinicio) {
    secuenciaReinicio();
  }
  if(modoReinicio) {
    secuenciaBusqueda();
  }
}

void secuenciaBusqueda() {
  // INSERTAR LECTURA DEL HALL

  digitalWrite(DIR,0);        // Primero gira hacia la derecha
  for (int paso = 0; paso < limitePosicion; paso++)       // Gira hasta el final
  {
    // CONDICIÓN DE SI DETECTA HALL, PARE Y ABRA GARRA

    digitalWrite(PUL,HIGH);
    delayMicroseconds(delayStep);
    digitalWrite(PUL,LOW);
    delayMicroseconds(delayStep);
    cantidadPasos++;
  }
  digitalWrite(DIR,1);        // Primero gira hacia la izquierda
  for (int paso = 0; paso < 2*limitePosicion; paso++)       // Gira hasta el otro lado (doble recorrido)
  {
    // CONDICIÓN DE SI DETECTA HALL, PARE Y ABRA GARRA

    digitalWrite(PUL,HIGH);
    delayMicroseconds(delayStep);
    digitalWrite(PUL,LOW);
    delayMicroseconds(delayStep);
    cantidadPasos++;
  }


}

void secuenciaReinicio() {
  if (posActual > 0) {
    digitalWrite(DIR,1);
  }
  else if (posActual < 0) {
    digitalWrite(DIR,0);
  }
  for (int paso = 0; paso < abs(posActual); paso++)    
  {
    digitalWrite(PUL,HIGH);
    delayMicroseconds(delayStep);
    digitalWrite(PUL,LOW);
    delayMicroseconds(delayStep);
  }
  posActual = 0;
}