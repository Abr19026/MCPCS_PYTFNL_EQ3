#include <timers328.h>
#include <Segmentos.h>
#define array_size(array) (sizeof(array)/sizeof((array)[0]))

//#define EMULADOR_SENSOR_AZAR
//#define EMULADOR_SENSOR_SERIAL
#define RANDOM_INTERVAL
//#define SIMULAR_REAL

// Configuración para refresh de pantalla 7 segmentos
#define PRESCALER1 1024
#define TICKS_UPDATE_PANTALLA 50

// Configuración de pantalla de 7 segmentos
const int pines_segmentos[] = {11, 10, 9, 8, 7, 6, 5};
const int pines_digitos[] = {4, 3, 2};
const int pin_sensor = A0;

PantallaSegmentos pantalla(true,(int*)pines_segmentos,(unsigned char)array_size(pines_digitos),(int*)pines_digitos);

// Configuración bluetooth (pin 0 y 1)
// Antes de subir un programa mantener bluetooth presionado
// Sólo es necesario conectar rx del bluetooth a tx del arduino
#define BAUD_BT 9600

void setup() {
  //lecturas de prueba
  #if defined(EMULADOR_SENSOR_AZAR) or defined(RANDOM_INTERVAL)
        randomSeed(analogRead(A0));
  #endif
  //configura pines de pantalla como output
  for(int i=0; i < array_size(pines_segmentos); i++) {
      pinMode(pines_segmentos[i], OUTPUT);
  }

  for(int i=0; i < array_size(pines_digitos); i++) {
      pinMode(pines_digitos[i], OUTPUT);
      digitalWrite(pines_digitos[i], HIGH);
  }
  //configuro timer para encender pantalla
  SET_TIMER1_SCALER(PRESCALER1);
  SET_WGM_TIMER1(T1_CTC_OC);
  OCR1A = TICKS_UPDATE_PANTALLA;
  TIMSK1 |= 1 << OCIE1A;
  //inicializa bluetooth
  Serial.begin(BAUD_BT);
  sei();
}

unsigned int lectura = 0;
unsigned int obtener_pulso(int pin) {
    #ifdef RANDOM_INTERVAL
    unsigned int micros = random(500, 2000);
    delay(micros);
    return micros;
    #endif
}

void loop() {
    unsigned int nueva_lectura = 0xFF;
    // Por 1 segundo lee el sensor
    #ifdef EMULADOR_SENSOR_AZAR
        nueva_lectura = 50 + random(0,100);
        delay(600);
    #elif defined(EMULADOR_SENSOR_SERIAL)
        if (unsigned char cantidad = Serial.available() >= 2) {
            nueva_lectura = Serial.read() << 8;
            nueva_lectura += Serial.read();
            while(Serial.available()) {
                Serial.read();
            }
            delay(600);
        }
        else {
            return;
        }
    #elif defined(SIMULAR_REAL) or defined(RANDOM_INTERVAL)
        nueva_lectura = obtener_pulso(pin_sensor);
    #endif
    #if defined(RANDOM_INTERVAL)
    Serial.write("a");
    //Serial.write("\r\n");
    #else
    lectura = nueva_lectura;
    // Actualiza valor mostrado por la pantalla
    pantalla.set_numero(lectura);
    // Lo manda por bluetooth
    digitos_string cadena_lectura= pantalla.digitos_a_chars();
    Serial.write(cadena_lectura.cadena);
    Serial.write("\r\n");
    #endif
}

/*
// Retorna la frecuencia cardiaca por minuto
// dado que el pin análogo esté conectado al sensor
unsigned int obtener_pulso(int pin) {
    static unsigned char tiempos_crestas[20] = {0};
    // cuenta tiempos de crestas en 2 segundos en decisegundos
    // halla tiempo promedio (periodo) entre crestas en decisegundos
    // retorna frecuencia por minuto
    // return periodo/ 1 segundo * 60
}*/

// Actualiza pantalla cada vez que timer1 alcance TICKS_UPDATE_PANTALLA
// Cada Tick dura PRESCALER1/16,000,000 segundos
ISR(TIMER1_COMPA_vect) {
    pantalla.mostrar_numero();
}