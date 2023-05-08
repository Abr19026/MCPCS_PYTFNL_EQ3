#define array_size(array) (sizeof(array)/sizeof((array)[0]))
#include <Segmentos.h>

//Segmentos A, B, C, D, E, F, G
int pin_segmentos[] = {11, 10, 9, 8, 7, 6, 5};
//dígitos de izquierda a derecha
int pin_digitos[] = {4, 3, 2};

//Inicializa la clase pantalla
PantallaSegmentos pantalla_1(true, pin_segmentos, array_size(pin_digitos), pin_digitos);


void setup() {
    Serial.begin(9600);
    // pone pines de la pantalla como output
    for(int i=0; i < array_size(pin_segmentos); i++) {
        pinMode(pin_segmentos[i], OUTPUT);
    }

    for(int i=0; i < array_size(pin_digitos); i++) {
        pinMode(pin_digitos[i], OUTPUT);
        digitalWrite(pin_digitos[i], HIGH);
    }
    
}

uint32_t numb = 0;
void loop() {
    pantalla_1.set_numero(numb);
    for(int i=0; i <= 20; i++) {
        pantalla_1.mostrar_numero();
        delay(20);
    }
    numb++;
}