#include <Servo.h>          // Libreria para servo

#define PUL 7               // Cada pulso a este pin es un paso (o micropaso) del motor
#define DIR 6               // Define la dirección del motor, HIGH es horario
#define botonBusqueda 3     // Botón para iniciar secuencia de clasificación de barquillos
#define botonDerecha 4
#define botonIzquierda 5
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
int sensorHallThreshold = 555;                  // Umbral para detectar campo magnético
bool direccion;                                 // Direccion de giro del stepper
bool noEncontroIman = 0;

Servo myservo;                                  // Se define el objeto servo
const int cierre = 55;                          // Valor determinado experimentalmente con el cual la garra puede tomar un barquillo
const int apertura = 155;                       // Valor determinado experimentalmente con el cual la garra abre lo sudiciente para tomar un barquillo
bool busqueda = 0;                              // Memoria de botón

void setup() {
  pinMode(PUL, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(botonBusqueda, INPUT);
  pinMode(botonDerecha, INPUT);
  pinMode(botonIzquierda, INPUT);
  myservo.attach(9);                            // Salida para control del servo
  Serial.begin(115200);
}

void loop() {
  if(digitalRead(botonBusqueda)) {
      busqueda = 1;
      delay(2000);
      myservo.write(cierre);        // Cerrar garra
      delay(2000);
      direccion = 0;                // Empieza girando a la derecha
      digitalWrite(DIR, direccion);
  }
  Serial.println(busqueda);
  if(busqueda) {
    int medicionHall = analogRead(sensorHall);
    if(medicionHall < sensorHallThreshold) {
      moverStepper(1, 350);        // Mover stepper un paso
      posActual++;
      if(posActual >= limitePosicion && !noEncontroIman) {
        direccion = 1;
        digitalWrite(DIR, direccion);
        posActual = 0;
        noEncontroIman = 1;
      }
      else if(posActual >= 2*limitePosicion && noEncontroIman) {
        direccion = 0;
        digitalWrite(DIR, direccion);
        moverStepper(limitePosicion, 250);
        posActual = 0;
        noEncontroIman = 0;
        busqueda = 0;
        delay(2000);
        myservo.write(apertura);
      }
    }
    else {
      noEncontroIman = 0;
      delay(2000);
      myservo.write(apertura);
      delay(2000);
      busqueda = 0;
      if(direccion) {
        direccion = 0;
        digitalWrite(DIR, direccion);
        if(posActual <= limitePosicion/2) {
          moverStepper(limitePosicion/2 - posActual, 250);
        }
        else {
          moverStepper(posActual - limitePosicion/2, 250);
        }
      }
      else {
        direccion = 1;
        digitalWrite(DIR, direccion);
        moverStepper(posActual, 250);
      }
    }
  }
  if(digitalRead(botonDerecha)) {
    direccion = 0;
    digitalWrite(DIR, direccion);
    while(digitalRead(botonDerecha)) {
      moverStepper(1, 400);
    }
  }
  if(digitalRead(botonIzquierda)) {
    direccion = 1;
    digitalWrite(DIR, direccion);
    while(digitalRead(botonIzquierda)) {
      moverStepper(1, 400);
    }
  }

}

void moverStepper(int pasos, int delayStep) {
  for(int paso = 0; paso < pasos; paso++) {
    digitalWrite(PUL, HIGH);
    delayMicroseconds(delayStep);
    digitalWrite(PUL, LOW);
    delayMicroseconds(delayStep);
  }
}

int medirHall() {
  unsigned int mediciones = 0;
  int cantidadMediciones = 10;
  for (int medicion = 0; medicion < cantidadMediciones; medicion++) {
    mediciones += analogRead(sensorHall);
  }
  return mediciones/cantidadMediciones;
}
