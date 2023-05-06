#define DIVISOR 16
#define INTERVALO_PROM 32
#define CICLOS_ACTUALIZACION 128
#define MILLIS_ACTUALIZACION (INTERVALO_PROM * CICLOS_ACTUALIZACION)
#define LATIDOS_MINIMOS_VALIDOS 2

/*
  NOTA; Los latidos tienen la siguiente forma
    /\  __
  _/  \/  \__
*/

//#define PRUEBA_GRAFICA
#define PRUEBA_TEXTO

struct v_latids {
    unsigned int ciclo_inicio[CICLOS_ACTUALIZACION/2];
    unsigned int cantidad_latidos;
};
typedef struct v_latids vector_latidos;

int pinLed = LED_BUILTIN;
unsigned int umbral = 1;
bool en_latido = false;
bool primer_pulso = true;

void setup() {
  Serial.begin(9600);
  pinMode(pinLed, OUTPUT);
  digitalWrite(pinLed, LOW);
}

// Obtiene la lectura promedio
// por los siguientes INTERVALO_PROM ms
int get_promedio() {
  int avg = 0;
  for(int i = 0;i < DIVISOR;i++){
    avg+=analogRead(A0);
    delay(INTERVALO_PROM/DIVISOR);
  }
  return avg/DIVISOR;
}

void registrar_latido(struct v_latids &vector, unsigned int ciclo) {
    vector.ciclo_inicio[vector.cantidad_latidos] = ciclo;
    vector.cantidad_latidos++;
}

vector_latidos reg_latidos;

void loop() {
    reg_latidos.cantidad_latidos = 0;
    union {
        unsigned int avg;
        uint8_t arr[sizeof(int)];
    };
    // Cuenta la cantidad de latidos por MILLIS_ACTUALIZACION ms
    for (int ciclo = 0; ciclo < CICLOS_ACTUALIZACION; ciclo++) {  
        // Obtiene promedio por INTERVALO_PROM ms
        avg = get_promedio();
        // Manda valor leído
        if (ciclo % 2 == 0) {
            #ifdef PRUEBA_GRAFICA
            Serial.write(arr, sizeof(int));
            #endif
            #ifdef PRUEBA_TEXTO
            Serial.write("-");
            #endif
        }
        //actualiza estado de latido y LED
        if (avg >= umbral) {
            if (!en_latido) {
                #ifdef PRUEBA_TEXTO
                Serial.write("a");
                #endif
                en_latido = true;
                registrar_latido(reg_latidos, ciclo);
                if (primer_pulso)
                  digitalWrite(pinLed, HIGH);
            }
        } else if (en_latido){
            en_latido = false;
            if (!primer_pulso)
              digitalWrite(pinLed, LOW);
            primer_pulso = !primer_pulso;
        }
    }
    unsigned int ritmo_cardiaco = 0;
    // Valida distancias entre latidos
    if (reg_latidos.cantidad_latidos >= LATIDOS_MINIMOS_VALIDOS) {
        
        // Cuenta el promedio de la cantidad de intervalos entre latidos
        int separacion_prom = 0;
        for (unsigned int i = 1; i < reg_latidos.cantidad_latidos; i++) {
            separacion_prom += reg_latidos.ciclo_inicio[i] - reg_latidos.ciclo_inicio[i-1];
        }
        separacion_prom = separacion_prom/reg_latidos.cantidad_latidos;
        
        // Halla la frecuencia cardiaca, la divide entre 2 porque cada pulso toma 2 intervalos
        ritmo_cardiaco = 60000/(2 * INTERVALO_PROM * separacion_prom);
        #ifdef PRUEBA_TEXTO
        Serial.print(separacion_prom);
        Serial.write(", ");
        Serial.print(reg_latidos.cantidad_latidos);
        Serial.write(", ");
        Serial.println(ritmo_cardiaco);
        #endif
    } else {
        #ifdef PRUEBA_TEXTO
        Serial.write("NO VALIDO\r\n");
        #endif
    }
}
