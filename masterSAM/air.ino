void airSetup()
{
   if (!ads.begin()) { //default address 0x48
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

   //initialize airReadings
   psi[0] = airRead();
   psi_avg = psi[0];
   for (int n=1; n<len;n++){ psi[n]= psi[0];}
}

float airRead()
{
  int16_t adc0 = ads.readADC_SingleEnded(0);
  volts0 = adc0 * .0001875;
  //return (-0.0066*pow(volts0,5))+(.0837*pow(volts0,4))+(-.3928*pow(volts0,3))+(.8268*pow(volts0,2))+(14.2527*volts0)-14.8514; //60 data points
  //return (-0.0014*pow(volts0,5))+(0.0140*pow(volts0,4))+(-0.0404*pow(volts0,3))+(-0.0049*pow(volts0,2))+(15.1391*volts0)-15.1497; //100 data points
  //return (0.0022*pow(volts0,6))+(-0.0400*pow(volts0,5))+(0.2898*pow(volts0,4))+(-1.0442*pow(volts0,3))+(1.9512*pow(volts0,2))+(13.2158*volts0)-14.4094; //100 data points
  //return //100 data points
  if(volts<=2.5) //1-2.5V 22.5psi
    return (14.9696*volts0)-14.9433;
  else if(volts<=3.5) //2.5-3.5V 37.5psi
    return (14.9904*volts0)-15.0356;
  else //3.5-5V
    return (15.0025*volts0)-15.0379;
}

void delayAndUpdate(int updates, int delay_ms)
{
  for (int n=0; n < updates;n++)
  {
      airAverage();
      sPrint("Updating: ");
      delay(delay_ms/updates);
  }
}

void airAverage()
{
  float run_sum = 0;
  psi[tail] = airRead();
  for (int n=0; n<len ;n++){
    run_sum+=psi[n];
  }
  psi_avg = run_sum/len;
  if(tail==len-1)
    tail=0;
  else
    tail ++;
}

void airBleed()
{
  digitalWrite(airBleeder,off); //bleed air
  
  while (psi_avg > targ){
    airAverage();
    sPrint("Bleeding: ");
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

void airEqualize(int state) {digitalWrite(airLever,state);} 
