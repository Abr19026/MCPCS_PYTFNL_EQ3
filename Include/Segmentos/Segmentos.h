/*
    segmentos.h - librería para mostrar número en pantallas de 7 segmentos
    Creada por Abraham Ramírez Moreno - 12 de abril del 2023
*/

#ifndef _SEGMENTOS_H_
#define _SEGMENTOS_H_

#define MAXIMOS_DIGITOS 9   //Máxima cantidad de dígitos, no cambiar
#include "Arduino.h"

typedef struct retorno_digitos {
    unsigned char valor[MAXIMOS_DIGITOS];
} lista_digitos;

typedef struct digitos_string {
    char cadena[MAXIMOS_DIGITOS + 1];
} string_digitos;

class PantallaSegmentos {
    private:
    const int valor_encendido;
    const int valor_apagado;
    
    const int* const pines_segmentos;   //Tienen que ser 7

    const unsigned char cant_digitos;
    const int* const pines_digitos;   //Maximo 9
    
    //numero debe ser de 0 a 9
    //Pines de los segmentos y del digito deben estar en output
    void escribir_digito(unsigned char numero);

    public:
    //último numero mostrado por la pantalla en base 10, cada dígito ocupa 1 byte
    lista_digitos ultimo_numero;

    //Máximo 9 digitos
    //Los pines de segmentos tienen que ser 7
    PantallaSegmentos(
        bool catodo_comun,
        const int * pines_segmentos,
        unsigned char cantidad_digitos,
        const int * pines_digitos
    );

    //Máximo numero es 999,999,999
    //Define el numero a mostrar cuando se llama mostrar_numero()
    void set_numero(uint32_t numero);
    
    //Muestra el numero en la pantalla (Máximo 999,999,999)
    //Pines de los segmentos y del digito deben estar en output
    void mostrar_numero();

    //Retorna el numero actual de la pantalla como un string ascii terminado en \0
    digitos_string digitos_a_chars();
};
#endif //_SEGMENTOS_H