#include <SoftwareSerial.h>

const byte BTrx = A5; //PIN que recibe datos del bluetooth (conectar a pin Tx de bluetooth) (No importa)
const byte BTtx = 13; //PIN que transmite al bluetooth (conectar a pin Rx de bluetooth)
SoftwareSerial BT(BTrx,BTtx);


void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(9600);
  BT.begin(9600);
}

void loop() {
  char dato;
  //Recibe datos desde terminal arduino
  while(Serial.available() > 0)
  {
    dato=Serial.read();
    BT.print(dato);
    if(!Serial.available()) {
      BT.print('\r');
    }
  }
  #define divisor 1
  //Lee datos pulso
  int avg = 0;
  for(int i=0;i<divisor;i++){
    avg+=analogRead(A0);
  }
  BT.print(avg/divisor, DEC);
  BT.write("\r\n");
  delay(4);
}
