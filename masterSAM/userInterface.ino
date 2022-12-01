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
  if (!SD.exists("SAM_RESULTS.csv")) 
  {
	myFile = SD.open("SAM_RESULTS.csv", FILE_WRITE);  
	myFile.println("SAM Num, Air Vol, pVal[0], pVal[1], pVal[2], pVal[3], pVal[4], pVal[5], eVal[0], eVal[1], eVal[2], eVal[3], eVal[4], eVal[5]");
  }
  else
  {
  myFile = SD.open("SAM_RESULTS.csv", FILE_WRITE);
  }
  String data = String(samVal,2) + "," + String(airVol,2) + "," + String(pVal[0],2) + "," + String(pVal[1],2) + "," + String(pVal[2],2) + "," + String(pVal[3],2) + "," +
                String(pVal[4],2) + "," + String(pVal[5],2) + "," + String(eVal[0],2) + "," + String(eVal[1],2) + "," + String(eVal[2],2) + "," + String(eVal[3],2) + "," +
                String(eVal[4],2) + "," + String(eVal[5],2);
      myFile.println(data); //prints out all the data
      //myFile.println("Pre-punch Values: " + pVals[0] + ", " + pVals[1], ", " + pVals[2], ", ", pVals[3], ", ", pVals[4], ", ", pVals[5]); //prepunch vals
      //myFile.println("Equilibrium Values: ", eVals[0], ", ", eVals[1], ", ", eVals[2], ", ", eVals[3], ", ", eVals[4], ", ", eVals[5]); //equib vals
      //myFile.println("SAM Number: ", samVal); //sam num
      //myFile.println("Air Volume: ", airVol); //air volume
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
