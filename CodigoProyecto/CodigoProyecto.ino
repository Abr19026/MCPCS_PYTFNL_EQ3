#include <timers328.h>
#include <Segmentos.h>
#include <Sensor_Pulso.h>

#define array_size(array) (sizeof(array)/sizeof((array)[0]))

// Permite ver resultados por lector serial
#define PRUEBA_SERIAL

// Configuración para refresh de pantalla 7 segmentos
#define PRESCALER1 1024
#define TICKS_UPDATE_PANTALLA 50

// Configuración de pantalla de 7 segmentos
const int pines_segmentos[] = {11, 10, 9, 8, 7, 6, 5};
const int pines_digitos[] = {4, 3, 2};

// Configuración de sensor piezoelectrico
const int pin_sensor = A0;
// Pin de led que muestra pulsos
const int pin_led = LED_BUILTIN;

PantallaSegmentos pantalla(
    true,
    (int*)pines_segmentos,
    (unsigned char)array_size(pines_digitos),
    (int*)pines_digitos
);

// Configuración bluetooth (pin 0 y 1)
// Antes de subir un programa mantener bluetooth presionado
// Sólo es necesario conectar rx del bluetooth a tx del arduino
#define BAUD_BT 9600

// Callback al final de cada pulso
void inicio_pulso() {
    digitalWrite(pin_led, HIGH);
    // Manda aviso por bluetooth
    Serial.write("a\r\n");
}
// Callback al inicio de cada pulso
void fin_pulso() {
    digitalWrite(pin_led, LOW);
}

void setup() {
  //configura pines de pantalla como output
  for(int i=0; i < array_size(pines_segmentos); i++) {
      pinMode(pines_segmentos[i], OUTPUT);
  }

  for(int i=0; i < array_size(pines_digitos); i++) {
      pinMode(pines_digitos[i], OUTPUT);
      digitalWrite(pines_digitos[i], HIGH);
  }

  pinMode(pin_led, OUTPUT);

  //configuro timer para encender pantalla
  SET_TIMER1_SCALER(PRESCALER1);
  SET_WGM_TIMER1(T1_CTC_OC);
  OCR1A = TICKS_UPDATE_PANTALLA;
  TIMSK1 |= 1 << OCIE1A;
  //inicializa bluetooth
  Serial.begin(BAUD_BT);
  //activa interrupciones
  //sei();
}

void loop() {
    int nueva_lectura = get_ritmo_cardiaco(pin_sensor, inicio_pulso, fin_pulso);
    if (nueva_lectura != -1) {
        // Actualiza valor mostrado por la pantalla
        pantalla.set_numero(nueva_lectura);
        #ifdef PRUEBA_SERIAL
        Serial.println(nueva_lectura);
        #endif
        /*
        // Lo manda por bluetooth
        digitos_string cadena_lectura = pantalla.digitos_a_chars();
        Serial.write(cadena_lectura.cadena);
        Serial.write("\r\n");
        */
    } else {
        pantalla.set_error();
        #ifdef PRUEBA_SERIAL
        Serial.write("error\r\n");
        #endif
    }
}


ISR(TIMER1_COMPA_vect) {
    pantalla.mostrar_numero();
}