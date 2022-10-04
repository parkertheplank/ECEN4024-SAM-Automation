void mechSetup()
{
  pinMode(vibrator, OUTPUT);
  pinMode(tilter, OUTPUT);
  
  digitalWrite(vibrator, off);  
  digitalWrite(tilter, off); 
}

void tilt(int state)
{
  digitalWrite(tilter, state);  
} 

void vibrate(int state) 
{
   digitalWrite(vibrator, state);  
}
