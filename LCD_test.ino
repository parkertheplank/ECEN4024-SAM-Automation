#include <LiquidCrystal.h>
#include "LiquidMenu.h"
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = A3, en = A5, d4 = A9, d5 = A10, d6 = A11, d7 = A12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void setup() {
 pinMode(A14,OUTPUT); //pins lol
 pinMode(A13,OUTPUT);
  pinMode(A4,OUTPUT);
  pinMode(A0,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A1,OUTPUT);
 digitalWrite(A14,LOW); 
 digitalWrite(A13,HIGH); 
  digitalWrite(A4,LOW); 
  digitalWrite(A0,LOW);
  digitalWrite(A2,LOW);
  digitalWrite(A1,HIGH);
 lcd.begin(16, 2);
 // Print a message to the LCD.
 lcd.print("TEAM SOLENOID");
}
void loop() {
 // set the cursor to column 0, line 1
 // (note: line 1 is the second row, since counting begins with 0):
 lcd.setCursor(0, 1);
 // message to user on line 1 (the second row)
lcd.print("Hello SAM user!");

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