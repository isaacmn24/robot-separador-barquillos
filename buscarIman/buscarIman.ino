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
//const float hallDesado;                       // Intensidad de campo magnético para la que se considera que el brazo llegó a la posición deseada
//int pasos;                                // Cantidad de pasos que se van calculando

// CONSTANTES DE CONTROL
//float error = 100.0;                          // El error empieza alto pero luego se actualiza
//float suma_error = 0;                         // Suma del error en el tiempo
//float error_anterior = 0;                     // Error anterior para luego restárselo al errora ctual
//float diferencia_error;                   // Diferencia entre el error actual y el anterior
//float kp = 1;                                 // Constante proporcional
//float ki = 1;                                 // Constante integral
//float kd = 1;                                 // Constante derivativa
//float pid;                                // PID inicia en uno (no hace nada)

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
  /*
  while(error >= 0.1) {

    
    // hall_derecha = medir hall derecha;
    // hall_izquierda = medir hall izquierda;
    if(hall_derecha - hall_izquierda > 0) {
      int direccion = 0;
      hall = hall_derecha;
    }
    if(hall_derecha - hall_izquierda < 0) {
      int direccion = 1;
      hall = hall_izquierda;
    }
    else {
      // soltar garra
      // return (no retorna nada, es para salir de la función)
    }
    
    digitalWrite(DIR,direccion);        // Primero gira hacia la derecha

    //error = hallDeseado - hall;
    //suma_error += error;
    //diferncia_error = error - error_anterior;
    //pid = kp*error + ki*suma_error + kd*diferencia_error;
    //pasos = pid;

    for (int paso = 0; paso < pasos; paso++)       // Gira hasta el final
    {
      digitalWrite(PUL,HIGH);
      delayMicroseconds(delayStep);
      digitalWrite(PUL,LOW);
      delayMicroseconds(delayStep);
      cantidadPasos++;
    }

    //error_anterior = error;

  }
  */
  
  // ABRE GARRA PARA SOLTAR BARQUILLO
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