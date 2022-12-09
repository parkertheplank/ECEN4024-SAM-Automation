#define vib 30

const int on=LOW;
const int off=HIGH;

void setup() {
  // put your setup code here, to run once:
  pinMode(vib, OUTPUT);
  digitalWrite(vib, off);  
}

void loop() {
  // put your main code here, to run repeatedly:
  vibrate(on);
  delay(2000);
  vibrate(off);
  delay(4000);
}


void vibrate(int state) 
{
   digitalWrite(vib, state);  
}
