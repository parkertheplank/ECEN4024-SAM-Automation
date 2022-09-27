void pneumSetup(){
   pinMode(13, OUTPUT);
   pinMode(3, OUTPUT);
   pinMode(5, OUTPUT);
   pinMode(airPump,OUTPUT);
   pinMode(airBleed,OUTPUT);
   airBleed = HIGH;
   
}

void feedbackLoop( double targetPsi){
  //inititalize air readings
  float psi1 = airRead();
  float psi2 = airRead();
  float psi3 = airRead();
  float avgPsi = (psi1+psi2+psi3)/3;

  //start air pump
  airPump = HIGH;

  //read pressure
  while (avgPsi < targetPsi){
    //create running average of last 3 values
    psi1 = psi2;
    psi2 = psi3;
    psi3 = airRead();
    avgPsi = (psi1+psi2+psi3)/3;
  }

  //stop pump
  airPump = LOW;
  
  //bleed back
  airBleed(targetPsi);
}

float airRead()//Read the pressure sensor analog output to match 14.5 psi. 
{
  float value = (float)analogRead(A1);
  float realValue = (((value / 1024.0)* 5.09) + 0.01);
  float psi = ((realValue - 0.99595617)/0.066932271);
  return psi;
}

void airBleed( double targetPsi, float psi1, float psi2, float psi3, float avgPsi)//Decrease air pressure to desired value
{
  //read pressure
  while (avgPsi < targetPsi){
    //create running average of last 3 values
    psi1 = psi2;
    psi2 = psi3;
    psi3 = airRead();
    avgPsi = (psi1+psi2+psi3)/3;
    //bleed some
    airBleed = LOW;
    delay(10);
    airBleed = HIGH;
  }
} 
/*
void airPressurize(){} //Turn on air pump and two-way air valve to begin pressurizing the SAM.
void airHalt() {} //raise air pressure to desired value
void airLever() {} //Push top lever to release pressure into the chamber for 10-20 seconds to allow the pressure to reach equilibrium. */
