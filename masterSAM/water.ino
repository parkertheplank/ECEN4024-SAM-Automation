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
  delay(10000);
} 

void waterClose()
{
  digitalWrite(waterPump, off);
  digitalWrite(waterValveIn, off);
  delay(500);
  digitalWrite(waterValveOut, off);
} 
