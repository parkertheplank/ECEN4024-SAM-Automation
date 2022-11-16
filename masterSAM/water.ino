void waterSetup() 
{
  pinMode(waterPump, OUTPUT);
  pinMode(waterValveIn, OUTPUT);
  pinMode(waterValveOut, OUTPUT);
  
  digitalWrite(waterValveIn, off);
  digitalWrite(waterValveOut, off);
  digitalWrite(waterPump, off);
  
  waterClose();
}

void waterFill(bool second) 
{
  digitalWrite(waterValveOut, on);
  if (second){delay(4000);}//time depressurize after second interation
  digitalWrite(waterValveIn, on);
  digitalWrite(waterPump, on);
} 

void waterClose()
{
  digitalWrite(waterPump, off);
  digitalWrite(waterValveIn, off);
  delay(500);
  digitalWrite(waterValveOut, off);
} 
