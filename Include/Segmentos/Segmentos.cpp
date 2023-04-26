/*
    segmentos.cpp - librería para mostrar número en pantallas de 7 segmentos
    Creada por Abraham Ramírez Moreno - 12 de abril del 2023
*/

#include "Segmentos.h"

const unsigned char encendidos_numero[] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6};
 
//numero máximo 999,999,999 (Si es mayor causa un desborde de memoria)
//regresa el numero en base decimal (No es imprimible), donde cada digito ocupa un byte
lista_digitos uint32_a_digitos(uint32_t numero) {
    lista_digitos digitos = {0};
    for(int i = MAXIMOS_DIGITOS - 1; numero > 0; i--) {
        digitos.valor[i] = numero % 10;
        numero /= 10;
    }
    return digitos;
}

//dado un número lo imprime en la pantalla
void PantallaSegmentos::escribir_digito(unsigned char numero) 
{
    unsigned char encendidos = encendidos_numero[numero];
    unsigned char segmento_act = 0;
    for(unsigned char i = 1<<7; i != 1; i = i>>1) {
        if(i & encendidos)
            digitalWrite(pines_segmentos[segmento_act], this->valor_encendido);
        else
            digitalWrite(pines_segmentos[segmento_act], this->valor_apagado);
        segmento_act++; 
    }
}

PantallaSegmentos::PantallaSegmentos(
    bool catodo_comun,
    const int * const pines_segmentos,
    unsigned char cantidad_digitos,
    const int * const pines_digitos
): 
    pines_segmentos{pines_segmentos},
    cant_digitos{cantidad_digitos},
    pines_digitos{pines_digitos},
    valor_apagado{catodo_comun ? LOW: HIGH},
    valor_encendido{catodo_comun ? HIGH: LOW}
{}

//Define el numero a mostrar cuando se llama mostrar_numero()
void PantallaSegmentos::set_numero(uint32_t numero) {
    ultimo_numero = uint32_a_digitos(numero);
}

//Va a mostrar el número digitado en el campo ultimo_numero
void PantallaSegmentos::mostrar_numero() {
    unsigned char digito_incial = MAXIMOS_DIGITOS - cant_digitos;
    for(unsigned char i=0; i < cant_digitos; i++) {
        escribir_digito(ultimo_numero.valor[digito_incial + i]);
        digitalWrite(pines_digitos[i], valor_apagado);
        delay(1);
        digitalWrite(pines_digitos[i], valor_encendido);
    }
}

string_digitos PantallaSegmentos::digitos_a_chars() {
    string_digitos digitos;
    uint8_t indice_numero = 0;
    
    //omite ceros de la izquierda
    while((this->ultimo_numero.valor[indice_numero] == 0) && (indice_numero < MAXIMOS_DIGITOS - 1)){
        indice_numero++;
    }
    
    //crea cadena
    uint8_t indice_cadena = 0;
    while(indice_numero < MAXIMOS_DIGITOS) {
        digitos.cadena[indice_cadena] = '0' + this->ultimo_numero.valor[indice_numero];
        indice_numero++;
        indice_cadena++; 
    }
    digitos.cadena[indice_cadena] = '\0';
    return digitos;
}