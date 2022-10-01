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
  
  Serial.println("\npressurize");
  airPressurize();
  
  //read pressure
  while (psi < (targetpsi - .5)){
    airRead();
  }
  
  Serial.println("off");
  airHalt();
  
  //bleed back
  delay(2000);
  Serial.println("\nbleeding");
  bleed(targetpsi);
  delay(2000);
  airRead();
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
  float realValue = (((readings[readIndex] / 1024.0)* vref) + 0.01);
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings){
    readIndex = 0;
  }

  average = total/numReadings;
  
  
  psi = ((realValue - 0.99595617)/0.066932271);
  
  Serial.print("   adcval:  ");
  Serial.print(average);
  Serial.print("\tpsi:   ");
  Serial.println(psi);

}

void bleed(float targetpsi)//Decrease air pressure to desired value
{
  airRead();
  digitalWrite(airBleed,off); //bleed air
  
  while (psi > targetpsi ){
    airRead();
    delay(100);
  }
  digitalWrite(airBleed,on); //close bleeder
} 

void airPressurize() //Pressurize Top Chamber
{
  digitalWrite(airBleed, on); //close bleeder
  digitalWrite(airValve, on); //valve open
  digitalWrite(airPump, on); //start pump
} 

void airHalt() //Stop Pressurizing and close
{
  digitalWrite(airValve, off); //close valve
  digitalWrite(airPump, off); //stop pump
}

void airPunch() //reach equib pressure between two chambers
{
  digitalWrite(airLever, on);
  delay(5000);
  digitalWrite(airLever,off);
} 
