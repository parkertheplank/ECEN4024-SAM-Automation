void pneumSetup(){
   //ads1115.begin();  // Initialize ads1015 at the default address 0x48
   for (int thisReading = 0; thisReading < numReadings; thisReading++)
   {
    readings[thisReading] = 0;
   }
   
   pinMode(airPump, OUTPUT);
   pinMode(airBleed, OUTPUT);
   pinMode(airValve, OUTPUT);
   
   digitalWrite(airBleed, LOW); 
   digitalWrite(airValve, HIGH);
   digitalWrite(airPump, HIGH); 
   
}

void feedbackLoop( float targetpsi){
  //inititalize air readings
  airRead();

  //start air pump
  digitalWrite(airPump,HIGH);

  //read pressure
  while (psi < targetpsi){
    airRead();
  }

  //stop pump
  digitalWrite(airPump,LOW);
  
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
  
  float realValue = (((average / 1024.0)* 5.09) + 0.01);
  psi = ((realValue - 0.99595617)/0.066932271);
}

void bleed(float targetpsi)//Decrease air pressure to desired value
{
  while (psi < targetpsi){
    airRead();
    digitalWrite(airBleed,HIGH); //bleed air
  }
  digitalWrite(airBleed,LOW);
} 
/*
void airPressurize(){} //Turn on air pump and two-way air valve to begin pressurizing the SAM.
void airHalt() {} //raise air pressure to desired value
void airLever() {} //Push top lever to release pressure into the chamber for 10-20 seconds to allow the pressure to reach equilibrium. */
