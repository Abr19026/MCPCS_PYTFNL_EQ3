/*
    Sensor_Pulso.cpp - librería para medir pulso con piezoeléctrico
    Creada por Abraham Ramírez Moreno - 5 de mayo del 2023
*/
#include "Sensor_Pulso.h"
#define PRUEBAS_POR_INTERVALO 16
#define INTERVALO_PROM 32
#define CICLOS_ACTUALIZACION 128
#define MILLIS_ACTUALIZACION (INTERVALO_PROM * CICLOS_ACTUALIZACION)
#define LATIDOS_MINIMOS_VALIDOS 2

struct v_latids {
    unsigned int ciclo_inicio[CICLOS_ACTUALIZACION/2];
    unsigned int cantidad_latidos;
};
typedef struct v_latids vector_latidos;

int get_promedio(int pin_lectura) {
  int avg = 0;
  for(int i = 0;i < PRUEBAS_POR_INTERVALO;i++){
    avg += analogRead(pin_lectura);
    delay(INTERVALO_PROM/PRUEBAS_POR_INTERVALO);
  }
  return avg/PRUEBAS_POR_INTERVALO;
}

void registrar_latido(struct v_latids &vector, unsigned int ciclo) {
    vector.ciclo_inicio[vector.cantidad_latidos] = ciclo;
    vector.cantidad_latidos++;
}

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
    unsigned int avg;
    
    // Cuenta la cantidad de latidos por MILLIS_ACTUALIZACION ms
    for (int ciclo = 0; ciclo < CICLOS_ACTUALIZACION; ciclo++) {  
        // Obtiene promedio por INTERVALO_PROM ms
        avg = get_promedio(pin_lectura);
        //actualiza estado de latido y LED
        if (avg >= umbral) {
            if (!en_latido) {
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
        return ritmo_cardiaco;
    } else {
        return -1;
    }
}