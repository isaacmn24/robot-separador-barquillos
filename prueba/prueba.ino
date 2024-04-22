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
const int anguloLimite = 70;                    // Angulo del limite del arco a recorrer
const int limitePosicion = floor(pasosPorRev/360.0*anguloLimite);                // Pasos que hay desde el 0 hasta el (aproximadamente) angulo limite
unsigned int cantidadPasos = 0;                 // Cuenta cuantos pasos se ha movido 
int sensorHallThreshold = 400;                  // Umbral para detectar campo magnético
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
  // Se presiona el botón de búsqueda y se activa la memoria de búsqueda, asi como 
  // pone la dirección del motor en antihorario
  if(digitalRead(botonBusqueda)) {
      busqueda = 1;                 // Memoria de búsqueda

      delay(2000);
      myservo.write(cierre);        // Cerrar garra
      delay(2000);

      direccion = 0;                // Empieza girando antihorario
      digitalWrite(DIR, direccion);
  }

  // Si la memoria búsqueda está activada, comienza la rutina de búsqueda
  if(busqueda) {
    int medicionHall = medirHall();       // Medir sensor Hall
    Serial.println(medicionHall);

    // Si las mediciones son menores al threshold, es decir, no ha encontrado el imán
    if(medicionHall > sensorHallThreshold) {
      moverStepper(1, 5);               // Mover stepper un paso
      posActual++;

      // Si alcanza el límite se devuelve a la posición inicial y resetea memoria de posición
      if(posActual >= limitePosicion && !noEncontroIman) {
        direccion = 1;                  
        digitalWrite(DIR, direccion);       // Se mueve en sentido horario para devolverse
        moverStepper(posActual, 250);       // Se devuelve a posición inicial

        posActual = 0;                      // Se resetea memoria de posición
        noEncontroIman = 1;                 // Se activa memoria de que ya hizo un recorrido
      }

      // Si alcanzó el límite una vez más, se devuelve al centro y termina la rutina
      else if(posActual >= limitePosicion && noEncontroIman) {
        direccion = 0;
        digitalWrite(DIR, direccion);        // Se mueve en antihorario
        moverStepper(posActual, 250);        // Se devuelve al origen

        posActual = 0;                       // Resetea memorias
        noEncontroIman = 0;
        busqueda = 0;

        delay(2000);
        myservo.write(apertura);             // Abre garra
      }
    }

    // Si se encontró el imán
    else {
      delay(2000);
      myservo.write(apertura);        // Abre la garra
      delay(2000);

      direccion = !direccion;         // Invierto la dirección, donde sea que quedó la garra
      digitalWrite(DIR, direccion);
      moverStepper(posActual, 250);  // Me devuelvo los pasos dados

      noEncontroIman = 0;             // Reseteo memorias
      busqueda = 0;
      posActual = 0;
    }
  }

  // Mientras se presione el botón derecho, el motor se va a mover un paso antihorario
  if(digitalRead(botonDerecha)) {
    direccion = 0;
    digitalWrite(DIR, direccion);
    while(digitalRead(botonDerecha)) {
      moverStepper(1, 400);
    }
  }

  // Mientras se presione el botón izuierdo, el motor se va a mover un paso horario
  if(digitalRead(botonIzquierda)) {
    direccion = 1;
    digitalWrite(DIR, direccion);
    while(digitalRead(botonIzquierda)) {
      moverStepper(1, 400);
    }
  }
}

// Esta función mueve el motor los pasos que entren como parámetro a una velocidad definida
// por el delayStep que entra como parámetro
// Lo que hace es mandar un pulso por paso a la entrada PUL del controlador, con una duración delayStep
void moverStepper(int pasos, int delayStep) {
  for(int paso = 0; paso < pasos; paso++) {
    digitalWrite(PUL, HIGH);
    delayMicroseconds(delayStep);
    digitalWrite(PUL, LOW);
    delayMicroseconds(delayStep);
  }
}

// Esta función toma una muestra de 10 mediciones del sensor Hall y obtiene el promedio
int medirHall() {
  unsigned int mediciones = 0;
  int cantidadMediciones = 30;
  for (int medicion = 0; medicion < cantidadMediciones; medicion++) {
    mediciones += analogRead(sensorHall);
  }
  return mediciones/cantidadMediciones;
}
