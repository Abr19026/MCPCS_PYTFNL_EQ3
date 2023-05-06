#define DIVISOR 16
#define INTERVALO_PROM 32
#define CICLOS_ACTUALIZACION 128
#define MILLIS_ACTUALIZACION INTERVALO_PROM*CICLOS_ACTUALIZACION
#define LATIDOS_MINIMOS_VALIDOS 2

int pinLed = LED_BUILTIN;

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



int umbral = 1;
bool en_latido = false;
int ultimo_latido = 0;

typedef struct v_latids {
    unsigned int ciclo_inicio[CICLOS_ACTUALIZACION/2];
    unsigned int cantidad_latidos;
} vector_latidos;

void registrar_latido(vector_latidos vector, unsigned int ciclo) {
    vector.ciclo_inicio[vector.cantidad_latidos] = ciclo;
    vector.cantidad_latidos++;
}

void loop() {
    vector_latidos reg_latidos;
    reg_latidos.cantidad_latidos = 0;
    union {
        int avg;
        uint8_t arr[sizeof(int)];
    };
    // Cuenta la cantidad de latidos por MILLIS_ACTUALIZACION ms
    for (int ciclo = 0; ciclo < CICLOS_ACTUALIZACION; ciclo++) {  
        // Obtiene promedio por INTERVALO_PROM ms
        avg = get_promedio();
        // Manda valor leído
        if (ciclo % 2 == 0) Serial.write(arr, sizeof(int));
        //actualiza estado de latido y LED
        if (avg >= umbral) {
            if (!en_latido) {
                en_latido = true;
                registrar_latido(reg_latidos, ciclo);
                digitalWrite(pinLed, HIGH);
            }
        } else if (en_latido){
            en_latido = false;
            digitalWrite(pinLed, LOW);
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
        // Halla la frecuencia cardiaca
        ritmo_cardiaco = 60000/(INTERVALO_PROM*separacion_prom);
        Serial.println(ritmo_cardiaco);
    } else {
        Serial.write("NO VALIDO\r\n");
    }
}
