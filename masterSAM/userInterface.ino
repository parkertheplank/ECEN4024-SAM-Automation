void uiSetup(){
  Serial.begin(9600);
  Serial.println("-----START-------");
  pinMode(sBut, INPUT);
  pinMode(vBut, INPUT);
  attachInterrupt(digitalPinToInterrupt(sBut), start_test, HIGH);   //interupt for start button
  attachInterrupt(digitalPinToInterrupt(vBut), start_vib, HIGH);   //interupt for start button
  lcd.init();
  lcd.backlight();
  line[0]="  Super Air Meter";
  lcdPrint(manFlag, manFlag);
}

void sPrint(String phase)
{
  Serial.print(phase);
  Serial.print("  adcVolt:  ");
  Serial.print(volts,4);
  Serial.print("  psi sample:   ");
  Serial.print(psi[tail],2);
  Serial.print("  psi avg:   ");
  Serial.println(psi_avg,2);
}

void lcdPrint(int state, int dataState)
{
  switch (state)
  {
    case manFlag:       line[1] = "    Prepare SAM"; break;
    case bleedFlag:     line[1] = "    Bleeding Air"; break;
    case waterFlag:     line[1]= "   Water Pumping"; break;
    case pressFlag:     line[1] = "    Pressurizing"; break;
    case equibFlag:     line[1] = "     Equalizing"; break;
    case balanFlag:     line[1] = "     Balancing"; break;
  }
  switch (dataState)
  {
    case manFlag:       line[2] = "  Then Press Start"; break;
    case pValFlag:      line[2] = "   Top PSI: " + String(pVal[(i*3)+j],2); break;
    case eValFlag:      line[2] = "   Equib PSI: " + String(eVal[(i*3)+j],2); break;
    case usbFlag:       line[2] = "   Insert SD card"; break;
    case noDatFlag:     line[2] = " "; break;
  }
  if (state == manFlag || state == waterFlag)
    line[3] = " ";
  else
    line[3]= "     PSI: " + String(psi_avg,2);
  
  lcd.clear();
  lcd.print(line[0]); 
  lcd.setCursor(0,1);
  lcd.print(line[1]); 
  lcd.setCursor(0,2);
  lcd.print(line[2]);
  lcd.setCursor(0,3);
  lcd.print(line[3]); 
}

void lcdPSI()
{
  line[3]= "     PSI: " + String(psi_avg,2);
  lcd.setCursor(0,3);
  lcd.print(line[3]); 
}

void sdWrite() 
{
  pinMode(10, OUTPUT);
<<<<<<< HEAD
  myFile = SD.open("SAM_RESULTS.txt", FILE_WRITE);
  if (myFile) {
    myFile.println(val);
=======
  if (SD.exists("SAM_RESULTS.txt")) 
  {
	myFile = SD.open("SAM_RESULTS.txt", FILE_WRITE);  
	myfile.println(" ");
	myFile.println("Pre-punch Values: ", pVals[0], ", ", pVals[1], ", ", pVals[2], ", ", pVals[3], ", ", pVals[4], ", ", pVals[5]); //prepunch vals
	myFile.println("Equilibrium Values: ", eVals[0], ", ", eVals[1], ", ", eVals[2], ", ", eVals[3], ", ", eVals[4], ", ", eVals[5]); //equib vals
	myFile.println("SAM Number: ", samVal); //sam num
	myFile.println("Air Volume: ", airVol); //air volume
	myFile.close();
  }
  else
  {
    myFile = SD.open("SAM_RESULTS.txt", FILE_WRITE);
  
    if (myFile) 
    {
      myFile.println("Pre-punch Values: ", pVals[0], ", ", pVals[1], ", ", pVals[2], ", ", pVals[3], ", ", pVals[4], ", ", pVals[5]); //prepunch vals
      myFile.println("Equilibrium Values: ", eVals[0], ", ", eVals[1], ", ", eVals[2], ", ", eVals[3], ", ", eVals[4], ", ", eVals[5]); //equib vals
      myFile.println("SAM Number: ", samVal); //sam num
      myFile.println("Air Volume: ", airVol); //air volume
>>>>>>> a27fcc951594aee6b1f2ae6d9c460dbfd1cda2c3
	// close the file:
      myFile.close();
    } 
    else 
    {
    // if the file didn't open, print an error:
    Serial.println("error opening SAM_RESULTS.txt");
    }
  }
} 
