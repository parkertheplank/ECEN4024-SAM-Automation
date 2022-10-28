void airSetup()
{
   ads1115.begin();  // Initialize ads1015 at the default address 0x48
   
   pinMode(airPump, OUTPUT);
   pinMode(airBleed, OUTPUT);
   pinMode(airValve, OUTPUT);
   pinMode(airLever, OUTPUT);
   
   digitalWrite(airBleed, off); //bleeding on. 
   digitalWrite(airValve, off); 
   digitalWrite(airPump, off); 
   digitalWrite(airLever, off);

   airRead(); //initialize psi reading
}

//Read the pressure sensor analog output to match 14.5 psi. 
void airRead()
{
  adcVal = ads1115.readADC_SingleEnded(1);
  adcVolt = ads1115.computeVolts(adcVal);
  psi = (60.000*((adcVolt-.98)/(vRef-.98))); 
  
  monPrintData();//testing
}

//Decrease air pressure to desired value
void airBleed(float targetpsi)
{
  digitalWrite(airBleed,off); //bleed air
  
  while (psi > targetpsi){
    airRead();
    delay(100);
  }
  digitalWrite(airBleed,on); //close bleeder
} 

void airPressurize()
{
  digitalWrite(airBleed, on); //close bleeder
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
