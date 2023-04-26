#ifndef TIMERS_328_H_
#define TIMERS_328_H_

#define clock_frec 16000000U  //Frecuencia en Hz de reloj base arduino
/************************************************************************
   CONFIGURACIÓN PRESCALERS DE TIMER1 Y TIMER2 (NO HAY QUE MOVER TIMER0) 
*************************************************************************/
#define T1_PRE_1    1U
#define T1_PRE_8    2U
#define T1_PRE_64   3U
#define T1_PRE_256  4U
#define T1_PRE_1024 5U

#define T2_PRE_1    1U
#define T2_PRE_8    2U
#define T2_PRE_32   3U
#define T2_PRE_64   4U
#define T2_PRE_128  5U
#define T2_PRE_256  6U
#define T2_PRE_1024 7U

uint8_t t1_prescaler_to_bits(uint16_t prescaler) {
    switch(prescaler) {
        case 1:     return T1_PRE_1;
        case 8:     return T1_PRE_8;
        case 64:    return T1_PRE_64;
        case 256:   return T1_PRE_256;
        case 1024:  return T1_PRE_1024;
        default:    return T1_PRE_1;
    }
}

uint8_t t2_prescaler_to_bits(uint16_t prescaler) {
    switch(prescaler) {
        case 1:     return T2_PRE_1;
        case 8:     return T2_PRE_8;
        case 32:    return T2_PRE_32;
        case 64:    return T2_PRE_64;
        case 128:   return T2_PRE_128;
        case 256:   return T2_PRE_256;
        case 1024:  return T2_PRE_1024;
        default:    return T2_PRE_1;
    }
}

#define SET_TIMER1_SCALER(PRESCALER) \
    ({  TCCR1B &= 0xF8;\
        TCCR1B |= t1_prescaler_to_bits(PRESCALER);\
    })

#define SET_TIMER2_SCALER(PRESCALER) \
    ({  TCCR2B &= 0xF8;\
        TCCR2B |= t2_prescaler_to_bits(PRESCALER);\
    })

/***************
    Modos WGM
****************/
//Aplica para todos
#define WGM_NORMAL  0U

//Opciones WGM TIMER0 y TIMER2
#define T02_CTC_OC  2U   //WGM CTC (Clear timer on compare) con tope en OCR0A/OCR2A (timer 0 y 2)

#define T02_FPWM    3U   //WGM Fast PWM (timer 0 y 2)
#define T02_FPWM_OC 7U   //WGM Fast PWM con tope en OCR0A/OCR2A (timer 0 y 2)

#define T02_CPWM    1U   //WGM Phase correct PWM (timer 0 y 2)
#define T02_CPWM_OC 5U   //WGM Phase correct PWM con tope en OCR0A/OCR2A (timer 0 y 2)

//Opciones WGM TIMER1
#define T1_CTC_IC      12U  //WGM CTC (Clear timer on compare) con tope en ICR1 (input capture) (timer1 )
#define T1_CTC_OC      4U   //WGM CTC (Clear timer on compare) con tope en OCR1A  (timer1 )

#define T1_FPWM_8BIT   5U   //WGM Fast PWM con tope en 0x00FF (timer1 )
#define T1_FPWM_9BIT   6U   //WGM Fast PWM con tope en 0X01FF (timer1 )
#define T1_FPWM_10BIT  7U   //WGM Fast PWM con tope en 0x03FF (timer1 )
#define T1_FPWM_IC     14U  //WGM Fast PWM con tope en ICR1 (input capture) (timer1 )
#define T1_FPWM_OC     15U  //WGM Fast PWM con tope en OCR1A (timer1 )

#define T1_CPWM_8BIT   1U   //WGM Phase correct PWM Tope en 0x00FF (timer1 )
#define T1_CPWM_9BIT   2U   //WGM Phase correct PWM Tope en 0X01FF (timer1 )
#define T1_CPWM_10BIT  3U   //WGM Phase correct PWM Tope en 0x03FF (timer1 )
#define T1_CPWM_IC     10U  //WGM Phase correct PWM tope en ICR1 (input capture) (timer1 )
#define T1_CPWM_OC     11U  //WGM Phase correct PWM tope en OCR1A (timer1 )

#define T1_FCPWM_IC    8U   //WGM Phase and frequency correct, tope en ICR1 (input capture)
#define T1_FCPWM_OC    9U   //WGM Phase and frequency correct, tope en OCR1A

//Define modo PWM de timer0 en pines
//-OC0A (pin 6 en UNO)
//-OC0B (pin 5 en UNO)
#define SET_WGM_TIMER0(MODE) \
({  TCCR0B &= ~_BV(WGM02);\
    TCCR0A &= ~(_BV(WGM01) | _BV(WGM00));\
    TCCR0B |= (MODE & 0x04) << 1;\
    TCCR0A |= (MODE & 0x03);\
})

//Define modo PWM de timer1 en pines
//-OC1A (pin 9 en UNO)
//-OC1B (pin 10 en UNO)
#define SET_WGM_TIMER1(MODE) \
({  TCCR1B &= ~(_BV(WGM13) | _BV(WGM12));\
    TCCR1A &= ~(_BV(WGM11) | _BV(WGM10));\
    TCCR1B |= (MODE & 0x0C) << 1;\
    TCCR1A |= (MODE & 0x03);\
})

//Define modo PWM de timer2 en pines
//-OC2A (pin 11 en UNO)
//-OC2B (pin 3 en UNO)
#define SET_WGM_TIMER2(MODE) \
({  TCCR2B &= ~_BV(WGM22);\
    TCCR2A &= ~(_BV(WGM21) | _BV(WGM20));\
    TCCR2B |= (MODE & 0x04) << 1;\
    TCCR2A |= (MODE & 0x03);\
})

//Opciones WGM TIMER2

/**************************
 Modos COM (compare output)
**************************/
//Configuración COM PARA TODOS LOS MODOS
#define NO_COMPARE   0U //Desactiva PWM en el pin dado (Aplica para todos WGM)
/*  En modo Normal y CTC: 
    -Togglea el PIN cuando hay match

    En modos PWM, SOLO usar en OCXA
    -En modos que no son PWM-OC actúa como NO_COMPARE
    -En PWM-OC, OCxA se toglea, OCXB como NO_COMPARE*/
#define TOGGLE_MATCH 1U 

//Configuración COM (COMPARE OUTPUT MODE) en modo Normal ó CTC
#define CLEAR_MATCH  2U // Pone pin dado en LOW cuando hay match (aplica para WGM Normal y CTC)
#define SET_MATCH    3U // Pone pin dado en HIGH cuando hay match (aplica para WGM Normal y CTC)

//Configuración COM (COMPARE OUTPUT MODE) en modo FAST-PWM
#define FPWM_NO_INVERT   2U // Pone el pin en modo Fast PWM no invertido (aplica para WGM Fast PWM)
#define FPWM_INVERT      3U // Pone el pin en modo Fast PWM invertido (aplica para WGM Fast PWM)

//Configuración COM (COMPARE OUTPUT MODE) en modo PWM Corregido
#define CPWM_SET_DOWN    2U // Pone el pin en modo Phase correct PWM invertido (aplica para WGM Phase Correct PWM)
#define CPWM_SET_UP      3U // Pone el pin en modo Phase correct PWM (aplica para WGM Phase Correct PWM)


// Define COM en pin OC0A (pin 6 en UNO), Modo WGM definido por timer 0
// Compare match definido por OCR0A
#define SET_COMA_T0(MODE) \
({  TCCR0A &= ~(_BV(COM0A1) | _BV(COM0A0));\
    TCCR0A |= (MODE & 0x03) << 6;\
})
// Define COM en pin OC0B (pin 5 en UNO), Modo WGM definido por timer 0
// Compare match definido por OCR0B
#define SET_COMB_T0(MODE) \
({  TCCR0A &= ~(_BV(COM0B1) | _BV(COM0B0));\
    TCCR0A |= (MODE & 0x03) << 4;\
})

// Define COM en pin OC1A (pin 9 en UNO), Modo WGM definido por timer 1
// Compare match definido por OCR1A
#define SET_COMA_T1(MODE) \
({  TCCR1A &= ~(_BV(COM1A1) | _BV(COM1A0));\
    TCCR1A |= (MODE & 0x03) << 6;\
})
// Define COM en pin OC1B (pin 10 en UNO), Modo WGM definido por timer 1
// Compare match definido por OCR1B
#define SET_COMB_T1(MODE) \
({  TCCR1A &= ~(_BV(COM1B1) | _BV(COM1B0));\
    TCCR1A |= (MODE & 0x03) << 4;\
})

// Define COM en pin OC2A (pin 11 en UNO), Modo WGM definido por timer 2
// Duty cycle y Compare match definido por OCR2A
#define SET_COMA_T2(MODE) \
({  TCCR2A &= ~(_BV(COM2A1) | _BV(COM2A0));\
    TCCR2A |= (MODE & 0x03) << 6;\
})
// Define COM en pin OC2B (pin 3 en UNO), Modo WGM definido por timer 2
// Duty cycle y Compare match definido por OCR2B
#define SET_COMB_T2(MODE) \
({  TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));\
    TCCR2A |= (MODE & 0x03) << 4;\
})

#endif