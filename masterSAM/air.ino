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
  volts = adc0 * .0001875;
  if (volts <= 2.5) //0-22.5 psi
    return (14.9848*volts)-15.0104+.041;
  else if (volts <= 3.5) //22.5-37.5psi
    return (14.9848*volts)-15.0104+.015;
  else //37.5-60psi
    return (14.9848*volts)-15.0104+.033;
}

void delayAndUpdate(int updates, int delay_ms)
{
  lcdPrint(balanFlag);
  for (int n=0; n < updates;n++)
  {
      airAverage();
      sPrint("Updating: ");
      lcdPSI();
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
  bool done = false;
  int calib = (j==2) ? .03 : .015; //need wider tolerance for 45 psi
  
  digitalWrite(airBleeder,off); //bleed air
  while (!done){
    airAverage();
    
    //Display data
    sPrint("Bleeding: ");
    lcdPSI();
    
    if(psi_avg <= targ + calib)
    {
      digitalWrite(airBleeder,on); //close bleeder
      delayAndUpdate(50, 2000);
      if(psi_avg <= targ + calib)
        done=true;
      else
        digitalWrite(airBleeder,off); //bleed air
    }
  }
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
