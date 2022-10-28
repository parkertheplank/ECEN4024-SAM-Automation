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
