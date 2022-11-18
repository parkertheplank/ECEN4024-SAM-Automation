

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
#include <LiquidCrystal.h>

const int rs = 8, en = 2, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 2);
  lcd.print("TEAM SOLENOID");
  //lcd.setCursor(0,1);
  //lcd.print("SAM NUMBER: 43.6"); // up to 3 decimal places in total
}

void loop() {
 

//idea: after each iteration, we print out the SAM values for like 5 seconds, then go directly into the next iteration

//i, t: variables to ouptut the iteration # and target PSI 
//tiltFlag, waterFlag, pumpFlag, bleedFlag: all flags to indicate processes
//i is instantiated to 1
//t is instantiated to 14.5

/* possible code 

  if(tiltFlag)
  {
    lcd.setCursor(0, 0);
    lcd.print("Iteration ", i, ": Water is Flowing");
  }

  if(waterFlag)
  {
    lcd.setCursor(0, 0);
    lcd.print("Iteration ", i, ": Tilting");

  }

  if(pumpFlag)
  {
    lcd.setCursor(0, 0);
    if (Pcount == 1)
      tri =14.5;
    if(Pcount ==  2)
      tri = 30;
    if(Pcount == 3)
      tri = 45;
    lcd.print("Iteration ", i, ", Trial for ", tri ," psi : Air Pumping"); 
    //after one trial at 14.5
    Pcount++;
    
  }

  if(bleedFlag)
  {
    lcd.setCursor(0, 0);
    if (Pcount == 1)
      tri =14.5;
    if(Pcount ==  2)
      tri = 30;
    if(Pcount == 3)
      tri = 45;
    lcd.print("Iteration ", i, ", Trial for ", tri ," psi : Bleeding");
    Bcount++;
  }

//after all are completed, i increases by 1
//after all are completed, Pcount and Bcount are reset to 1

*/
}
