void mechSetup()
{
  pinMode(vib, OUTPUT);
  pinMode(tilt1, OUTPUT);
  pinMode(tilt2, OUTPUT);
  
  digitalWrite(vib, off);  
  digitalWrite(tilt1, off); 
  digitalWrite(tilt2, off);
}

void tilt(int state)
{
  digitalWrite(tilt1, state);
  digitalWrite(tilt2, !state);  
  delay(5500); //roughly 30 degrees
  digitalWrite(tilt1, off);
  digitalWrite(tilt2, off);  
} 

void vibrate(int state) { digitalWrite(vib, state); }
