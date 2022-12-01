
void lcdTest()
{
  lcdPrint(manFlag, manFlag);
  delay(2000);
  lcdPrint(bleedFlag);
  delay(2000);
  lcdPrint(pressFlag);
  delay(2000);
  pVal[0]=66.6;
  eVal[0]=13;
  lcdPrint(pressFlag,pValFlag);
  delay(2000);
  lcdPrint(equibFlag,eValFlag);
  delay(2000);
}

void airTest()
{
    lcdPrint(pressFlag);
    j=1;
    targ = (j==0) ? 14.5 : ((j==1) ? 30 : 45); //set target pressure 
    int calib = (j==0) ? 1 : ((j==1) ? 1 : 1);   //and calibration offset
    airPressurize(); 
    while (psi_avg < targ + calib) 
    {
       sPrint("Pressurizing: ");
       airAverage(); 
       delay(50); 
    }
    airHalt();
    delayAndUpdate(100, 4000); //give time for psi to settle, update average

//---------------------AIR BLEED---------------------------
    lcdPrint(bleedFlag);
    airBleed();
    delayAndUpdate(200, 5000);
    pVal[(3*i)+j] = psi_avg; //record pre-punch psi
    sPrint("Pre-Punch Val: ");
}

void waterTest()
{
  waterFill(1);
  digitalWrite(tilt1, on);
  digitalWrite(tilt2, off);  
  delay(2000); 
  digitalWrite(tilt1, off); 
  for (int n = 0; n<18; n++)
  {
    digitalWrite(tilt1, on);
    digitalWrite(tilt2, off);  
    delay(500); 
    digitalWrite(tilt1, off); 
    Serial.println(n);
    delay(5000);
  }
  vibrating = false;
  //return to start position
  digitalWrite(tilt1, off);
  digitalWrite(tilt2, on);  
  delay(11000); 
  digitalWrite(tilt2, off); 
  waterClose();
  delay(2000);
  lcdPrint(pressFlag);
  j=0;
  targ = (j==0) ? 14.5 : ((j==1) ? 30 : 45); //set target pressure 
  int calib = (j==0) ? 1 : ((j==1) ? 1 : 1);   //and calibration offset
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
  pVal[(3*i)+j] = psi_avg; //record pre-punch psi
  sPrint("Pre-Punch Val: ");
  
//----------------AIR PUNCH AND VIBRATE-----------------------
  //lcdPrint(equibFlag, pValFlag); //display pre punch psi
  airEqualize(on);
  for(int k=0;k<3;k++){
    vibrate(on);
    delay(1000);
    vibrate(off);
    delay(1000);
  }
  
  delayAndUpdate(100, 3000);
  eVal[(3*i)+j] = psi_avg; //record equib psi
  sPrint("Equib Val: ");
  //lcdPrint(equibFlag, eValFlag); //display equib psi
  airEqualize(off);
  delay(3000); //time to see value
}
