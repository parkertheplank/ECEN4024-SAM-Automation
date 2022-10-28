#define tilt1 51
#define tilt2 53
const int on=LOW;
const int off=HIGH;

void setup() {
  // put your setup code here, to run once:
  pinMode(tilt1, OUTPUT);
  pinMode(tilt2, OUTPUT);

  digitalWrite(tilt1, off); 
  digitalWrite(tilt2, off);

  tilt(off);
}

void tilt(int state)
{
  digitalWrite(tilt1, state);
  digitalWrite(tilt2, !state);  

  delay(5500); //roughly 30 degrees

  digitalWrite(tilt1, off);
  digitalWrite(tilt2, off);  
} 

void loop() {
  // put your main code here, to run repeatedly:
 
}
