#define divisor 1
void setup() {
  Serial.begin(9600);
}
void loop() {
  int avg = 0;
  for(int i=0;i<divisor;i++){
    avg+=analogRead(A0);
  }
  Serial.println(avg/divisor,DEC);
  delay(4);
}
