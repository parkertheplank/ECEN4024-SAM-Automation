void airSetup()
{
   if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
   }
   ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
   
   pinMode(airPump, OUTPUT);
   pinMode(airBleeder, OUTPUT);
   pinMode(airValve, OUTPUT);
   pinMode(airLever, OUTPUT);
   
   digitalWrite(airBleeder, off); //bleeding on. 
   digitalWrite(airValve, off); 
   digitalWrite(airPump, off); 
   digitalWrite(airLever, off);

   psi[0]=airRead();
   psi_avg= psi[0];
   for (int n=1; n<len;n++){
    psi[n]= psi[0];
   }
   run_sum= psi[0]*len;
}

float airRead()
{
  adc0 = ads.readADC_SingleEnded(0);
  volts0 = adc0 * .0001875;
  return (14.985078*volts0)-14.99510943;
}

void airAverage()
{
  psi[tail] = airRead();

  run_sum=0;
  for (int n=0; n<len;n++){
    run_sum+=psi[n];
  }
  psi_avg = run_sum/len;

  if(tail==len-1)
  {
    tail=0;
  }
  else
  {
    tail ++;
  }
}

//Decrease air pressure to desired value
void airBleed()
{
  digitalWrite(airBleeder,off); //bleed air
  
  while (psi_avg > targ){
    airAverage();
    monPrintData();
  }
  digitalWrite(airBleeder,on); //close bleeder
} 

void airPressurize()
{
  digitalWrite(airBleeder, on); //close bleeder
  digitalWrite(airValve, on); //valve open
  digitalWrite(airPump, on); //start pump
} 

void airHalt() 
{
  digitalWrite(airValve, off); //close valve
  digitalWrite(airPump, off); //stop pump
} 

void airEqualize(int state) 
{
  digitalWrite(airLever,state);
} 
