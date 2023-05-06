/*
    Sensor_Pulso.h - librería para medir pulso con piezoeléctrico
    Creada por Abraham Ramírez Moreno - 5 de mayo del 2023
*/

#ifndef _SENSOR_PULSO_H_
#define _SENSOR_PULSO_H_
#include "Arduino.h"

// * retorna ritmo cardiaco o -1 en error
// * pin_lectura conectado a sensor, debe ser adc
// * callback son las función a ejecutar en las etapas del pulso
//   una se ejecuta al inicio de cada pulso, otro al fin de cada pulso
//   NOTA: Solo usar con un pin en todo el programa, usa variables globales (estáticas)
int get_ritmo_cardiaco(int pin_lectura, void (*callback_inicio)(void), void (*callback_fin)(void));
#endif