#define DIVISOR 16
#define INTERVALO_PROM 32
#define CICLOS_ACTUALIZACION 128
#define MILLIS_ACTUALIZACION (INTERVALO_PROM * CICLOS_ACTUALIZACION)
#define LATIDOS_MINIMOS_VALIDOS 2


//#define PRUEBA_GRAFICA
#define PRUEBA_TEXTO

struct v_latids {
    unsigned int ciclo_inicio[CICLOS_ACTUALIZACION/2];
    unsigned int cantidad_latidos;
};
typedef struct v_latids vector_latidos;

// Obtiene la lectura promedio, debe ser un pin con adc
// por los siguientes INTERVALO_PROM ms
int get_promedio(int pin_lectura) {
  int avg = 0;
  for(int i = 0;i < DIVISOR;i++){
    avg+=analogRead(pin_lectura);
    delay(INTERVALO_PROM/DIVISOR);
  }
  return avg/DIVISOR;
}

void registrar_latido(struct v_latids &vector, unsigned int ciclo) {
    vector.ciclo_inicio[vector.cantidad_latidos] = ciclo;
    vector.cantidad_latidos++;
}

// * retorna ritmo cardiaco o -1 en error
// * pin_lectura conectado a sensor, debe ser adc
// * callback son las función a ejecutar en las etapas del pulso
//   una se ejecuta al inicio de cada pulso, otro al fin de cada pulso
int get_ritmo_cardiaco(int pin_lectura, void (*callback_inicio)(void), void (*callback_fin)(void)) {
    static unsigned int umbral = 1;
    static bool en_latido = false;
    /*
      NOTA; Los latidos tienen la siguiente forma
         /\  __
       _/  \/  \__
       la variable primer pulso se da cuenta en cual
       parte vamos
    */
    static bool primer_pulso = true;
    static vector_latidos reg_latidos;
   
    reg_latidos.cantidad_latidos = 0;
    #ifdef PRUEBA_GRAFICA
    union {
    #endif
        unsigned int avg;
    #ifdef PRUEBA_GRAFICA
        uint8_t arr[sizeof(int)];
    };
    #endif
    // Cuenta la cantidad de latidos por MILLIS_ACTUALIZACION ms
    for (int ciclo = 0; ciclo < CICLOS_ACTUALIZACION; ciclo++) {  
        // Obtiene promedio por INTERVALO_PROM ms
        avg = get_promedio(pin_lectura);
        // Manda valor leído
        #if defined (PRUEBA_GRAFICA) or defined (PRUEBA_TEXTO)
        if (ciclo % 2 == 0) {
            #ifdef PRUEBA_GRAFICA
            Serial.write(arr, sizeof(int));
            #endif
            #ifdef PRUEBA_TEXTO
            Serial.write("-");
            #endif
        }
        #endif
        //actualiza estado de latido y LED
        if (avg >= umbral) {
            if (!en_latido) {
                #ifdef PRUEBA_TEXTO
                Serial.write("a");
                #endif
                en_latido = true;
                if (primer_pulso)
                {
                  registrar_latido(reg_latidos, ciclo);
                  callback_inicio();
                }
            }
        } else if (en_latido){
            en_latido = false;
            if (!primer_pulso)
              callback_fin();
            primer_pulso = !primer_pulso;
        }
    }
    // Valida distancias entre latidos
    if (reg_latidos.cantidad_latidos >= LATIDOS_MINIMOS_VALIDOS) {
        int ritmo_cardiaco;
        // Cuenta el promedio de la cantidad de intervalos entre latidos
        int separacion_prom = 0;
        for (unsigned int i = 1; i < reg_latidos.cantidad_latidos; i++) {
            separacion_prom += reg_latidos.ciclo_inicio[i] - reg_latidos.ciclo_inicio[i-1];
        }
        separacion_prom = separacion_prom/reg_latidos.cantidad_latidos;
        
        // Halla la frecuencia cardiaca,
        ritmo_cardiaco = (unsigned int)60000/(INTERVALO_PROM * separacion_prom);
        #ifdef PRUEBA_TEXTO
        Serial.print(separacion_prom);
        Serial.write(", ");
        Serial.print(reg_latidos.cantidad_latidos);
        Serial.write(", ");
        Serial.println(ritmo_cardiaco);
        #endif
        return ritmo_cardiaco;
    } else {
        return -1;
        #ifdef PRUEBA_TEXTO
        Serial.write("NO VALIDO\r\n");
        #endif
    }
}

int pinLed = LED_BUILTIN;

void encender_led() {
  digitalWrite(pinLed, HIGH);
}

void apagar_led() {
  digitalWrite(pinLed, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(pinLed, OUTPUT);
  digitalWrite(pinLed, LOW);
}

void loop() {
    get_ritmo_cardiaco(A0, encender_led, apagar_led);
}
