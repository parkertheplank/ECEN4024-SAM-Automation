void waterSetup() 
{
  pinMode(waterPump, OUTPUT);
  pinMode(waterValveIn, OUTPUT);
  pinMode(waterValveOut, OUTPUT);

  waterClose();
}

void waterFill() 
{
  digitalWrite(waterValveIn, on);
  digitalWrite(waterValveOut, on);
  digitalWrite(waterPump, on);
} 

void waterClose()
{
  digitalWrite(waterPump, off);
  digitalWrite(waterValveIn, off);
  digitalWrite(waterValveOut, off);
} 
