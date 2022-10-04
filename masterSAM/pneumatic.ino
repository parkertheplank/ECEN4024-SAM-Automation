void pneumSetup(){
   //ads1115.begin();  // Initialize ads1015 at the default address 0x48
   for (int thisReading = 0; thisReading < numReadings; thisReading++)
   {
    readings[thisReading] = 0;
   }
   
   pinMode(airPump, OUTPUT);
   pinMode(airBleed, OUTPUT);
   pinMode(airValve, OUTPUT);
   
   digitalWrite(airBleed, off); //bleeding on. 
   digitalWrite(airValve, off); //valve closed. solenoid without power closed relay inv
   digitalWrite(airPump, off); //pump off

   airRead();
}

void feedbackLoop(float targetpsi){
  Serial.println("\nfeedback start");
  //inititalize relays
  digitalWrite(airBleed, on); //close bleeder
  digitalWrite(airValve, on); //valve open
  digitalWrite(airPump, on); //start pump
  
  Serial.println("power on");
  //read pressure
  while (psi < targetpsi){
    airRead();
    delay(100);
  }
  
  Serial.println("power off");
  digitalWrite(airValve, off); //close valve
  digitalWrite(airPump, off); //stop pump
  
  //bleed back
  bleed(targetpsi);
}

void airRead()//Read the pressure sensor analog output to match 14.5 psi. 
{

  /*
  adcRef = ads1115.readADC_SingleEnded(2); //wire voltage reference channel 0
  vRef = ads1115.computeVolts(adcRef);
  adcVal = ads1115.readADC_SingleEnded(3);
  adcVolt = ads1115.computeVolts(adcVal);
  psi = 60.000*((adcVolt-.98)/(vRef-.98)); 
  */
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(inputPin);
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings){
    readIndex = 0;
  }

  average = total/numReadings;
  
  float realValue = (((average / 1024.0)* vref) + 0.01);
  psi = ((realValue - 0.99595617)/0.066932271);
  
  Serial.print("adcval:  ");
  Serial.print(average);
  Serial.print("\tpsi:   ");
  Serial.println(psi);

}

void bleed(float targetpsi)//Decrease air pressure to desired value
{
  Serial.println("\nbleeding");
  digitalWrite(airBleed,off); //bleed air
  
  while (psi > targetpsi){
    Serial.print("\n\n");
    airRead();
    delay(100);
  }
  digitalWrite(airBleed,on); //close bleeder
} 
/*
void airPressurize(){} //Turn on air pump and two-way air valve to begin pressurizing the SAM.
void airHalt() {} //raise air pressure to desired value
void airLever() {} //Push top lever to release pressure into the chamber for 10-20 seconds to allow the pressure to reach equilibrium. */
