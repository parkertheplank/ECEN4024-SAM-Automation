void airSetup()
{
   //ads1115.begin();  // Initialize ads1015 at the default address 0x48
   
   pinMode(airPump, OUTPUT);
   pinMode(airBleeder, OUTPUT);
   pinMode(airValve, OUTPUT);
   pinMode(airLever, OUTPUT);
   
   digitalWrite(airBleeder, off); //bleeding on. 
   digitalWrite(airValve, off); 
   digitalWrite(airPump, off); 
   digitalWrite(airLever, off);

   airRead(); //initialize psi reading
}

//Read the pressure sensor analog output to match 14.5 psi. 
void airRead()
{
  float value = (float)analogRead(A15);
  float realValue = (((value / 1024.0)* 5.09) + 0.01);
  psi = ((realValue - 0.99595617)/0.066932271);
  /*
  adcVal = ads1115.readADC_SingleEnded(1);
  adcVolt = ads1115.computeVolts(adcVal);
  psi = (60.000*((adcVolt-.98)/(vRef-.98))); 
  
  monPrintData();//testing
  */
}

//Decrease air pressure to desired value
void airBleed()
{
  digitalWrite(airBleeder,off); //bleed air
  
  while (psi > targ){
    airRead();
    delay(100);
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
