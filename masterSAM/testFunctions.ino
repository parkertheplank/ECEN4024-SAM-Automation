
void testing()
{
  digitalWrite(airValve, on); 
    Serial.println("air valve");
   delay(4000);
   digitalWrite(airValve, off);
}

void waterTest()
{
  waterFill(0);
  digitalWrite(tilt1, on);
  digitalWrite(tilt2, off);  
  delay(2000); //roughly 30 degrees
  digitalWrite(tilt1, off); 
  for(int i=0; i<10; i++)
  {
    digitalWrite(tilt1, on);
    digitalWrite(tilt2, off);  
    delay(500); //roughly 30 degrees
    digitalWrite(tilt1, off); 
    Serial.println(i);
    delay(4000);
  }
  //return to start position
  digitalWrite(tilt1, off);
  digitalWrite(tilt2, on);  
  delay(8000); //roughly 30 degrees
  digitalWrite(tilt2, off); 

  lcdPrint(pressFlag);
      targ = 14.5; //set target pressure 
      calib = 2.5;   //and calibration offset
      airPressurize(); 
      while (psi_avg < targ + calib) 
      {
         airAverage(); delay(100); 
         sPrint("Pressurizing: ");
      }
      airHalt();
      delayAndUpdate(100, 4000); //give time for psi to settle, update average

//---------------------AIR BLEED---------------------------
      lcdPrint(bleedFlag);
      airBleed();
      delayAndUpdate(200, 5000);
      sPrint("Pre-Punch Val: ");
      
//----------------AIR PUNCH AND VIBRATE-----------------------
      lcdPrint(equibFlag, pValFlag); //display pre punch psi
      airEqualize(on);
      for(int k=0;k<3;k++){
        vibrate(on);
        delay(1000);
        vibrate(off);
        delay(1000);
      }
      
      delayAndUpdate(100, 3000);
      sPrint("Equib Val: ");
      lcdPrint(equibFlag, eValFlag); //display equib psi
      airEqualize(off);
      delay(3000); //time to see value
}

void airCalib()
{
      airPressurize(); 
      while (psi_avg < 55) 
      {
         airAverage(); delay(100); 
         sPrint("Pressurizing: ");
      }
      airHalt();
         digitalWrite(airBleeder, off); //bleeding on. 
      while(true)
      {
        airAverage();
        sPrint("Calib: ");
        lcd.clear();
        lcd.print(psi_avg);
        delay(100);
      }
}
