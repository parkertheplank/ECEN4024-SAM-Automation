void pneumSetup(){
   ads1115.begin();  // Initialize ads1015 at the default address 0x48
   
   pinMode(airPump, OUTPUT);
   pinMode(airBleed, OUTPUT);
   
   digitalWrite(airBleed, LOW);
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
  Serial.print("hi");
  adcRef = ads1115.readADC_SingleEnded(2); //wire voltage reference channel 0
  Serial.print("hi2");
  vRef = ads1115.computeVolts(adcRef);
  Serial.print("hi3");
  adcVal = ads1115.readADC_SingleEnded(3);
  adcVolt = ads1115.computeVolts(adcVal);
  
  psi = 60.000*((adcVolt-.98)/(vRef-.98));
}

void bleed(float targetpsi)//Decrease air pressure to desired value
{
  airRead();
  //read pressure
  while (psi < targetpsi){
    //create running average of last 3 values
    airRead();
    //bleed some
    digitalWrite(airBleed,LOW);
    delay(10);
    digitalWrite(airBleed,HIGH);
  }
} 
/*
void airPressurize(){} //Turn on air pump and two-way air valve to begin pressurizing the SAM.
void airHalt() {} //raise air pressure to desired value
void airLever() {} //Push top lever to release pressure into the chamber for 10-20 seconds to allow the pressure to reach equilibrium. */
