#include <LiquidCrystal.h>
#include "LiquidMenu.h"
#include <EEPROM.h>
#include "Button.h"

// Pin mapping for the display
LiquidCrystal lcd(12,11,5,4,3,2);

// Button objects instantiation
Button up(8, true);//previous screen
Button down(9, true);//next screen
Button select(10, true);
Button enter(7, true);

// The analog reading sample period in seconds.
// It is later overwritten by it's EEPROM value.
unsigned short sample_period = 1;

int i=1;

int keyVal;
int P1avg=0;
int P2avg=0;
int P3avg=0;
int P4avg=0;
int P1avgtest=0;
int P2avgtest=0;
int P3avgtest=0;
int P4avgtest=0;

enum FunctionTypes { ENTER = 1,BACK = 2,};

LiquidLine KEYVAL(0,0,keyVal);

LiquidLine goback_main(13,1,"BA");
LiquidLine goback_calib(13,1,"BA");
LiquidLine goback_view(13,1,"BA");
LiquidLine goback_start(13,0, "BA");

//LiquidLine enter_start(13,0,"EN");

LiquidLine test(0, 0, "1.TEST ");
LiquidLine calib(0, 1, "2.CALIB");
LiquidLine reset(8, 0, "3.RESET");
LiquidLine light(8, 1, "4.LIGHT");
LiquidScreen screen_main(test, calib, reset, light);
LiquidMenu menu_main(lcd, screen_main);

LiquidLine sam(0, 0, "->SAM");
LiquidLine typeb(0, 1, "->TYPEB");
LiquidScreen screen_test(sam, typeb, goback_main);
LiquidMenu menu_test(lcd, screen_test);

LiquidLine start(0, 0, "->START:");
LiquidLine view(0, 1, "->VIEW:");
LiquidScreen screen_calib(start,view, goback_main);
LiquidMenu menu_calib(lcd, screen_calib);

char*  showhint="PRESS";
LiquidLine hint(0,1,showhint);
LiquidScreen screen_hint( hint,  KEYVAL);
LiquidMenu menu_hint(lcd, screen_hint);

LiquidLine cnfrm(0, 0, "->CNFRM:");
LiquidLine cancl(0, 1, "->CANCL:");
LiquidScreen screen_reset(cnfrm,cancl, goback_main);
LiquidMenu menu_reset(lcd, screen_reset);

LiquidLine off(0, 0, "->OFF:");
LiquidLine on(0, 1, "->ON:");
LiquidScreen screen_light1(off,on, goback_main);
LiquidLine Press(0, 0, "->PRESS:");
LiquidLine T20min(0, 1, "->20MIN:");
LiquidScreen screen_light2(Press,T20min, goback_main);
LiquidLine T5min(0, 0, "->5MIN:");
LiquidLine T1min(0, 1, "->1MIN:");
LiquidScreen screen_light3(T5min,T1min, goback_main);
LiquidMenu menu_light(lcd, screen_light1, screen_light2, screen_light3);

LiquidLine p1a(0, 0, "->P1-A:",P1avg);
LiquidLine p2a(0, 1, "->P2-A:",P2avg);
LiquidScreen screen_view1(p1a,p2a, goback_calib);
LiquidLine p3a(0, 0, "->P3-A:",P3avg);
LiquidLine p4a(0, 1, "->P4-A:",P4avg);
LiquidScreen screen_view2(p3a,p4a, goback_calib);
LiquidLine vtop(0, 0, "->VTOP:");
LiquidLine vbowl(0, 1, "->VBOWL:");
LiquidScreen screen_view3(vtop,vbowl, goback_calib);
LiquidMenu menu_view(lcd, screen_view1, screen_view2, screen_view3);

LiquidLine recal(0, 0, "->RECAL:");
LiquidScreen screen_vbowl(recal, goback_view);
LiquidMenu menu_vbowl(lcd, screen_vbowl);

LiquidSystem menu_system(menu_main, menu_test, menu_calib,menu_reset);

void buttonsCheck() {
  if (up.check() == LOW) {
    menu_system.previous_screen();
  }
  if (down.check() == LOW) {
    menu_system.next_screen();
  }
   if (select.check() == LOW) {
   menu_system.switch_focus();
  }
  if (enter.check() == LOW) {
    menu_system.call_function(ENTER);
  }
  return;
}

void screenshowcontrol(){
   static unsigned long lastMillis_sample01 = 0;
   keyVal= analogRead(A0);
   if (millis() - lastMillis_sample01 > (sample_period * 200)) {
      lastMillis_sample01 = millis(); 
      menu_system.update();}
   return;
}

void gobackto_menu_main() {
  menu_system.change_menu(menu_main);
  return;
}
void goto_menu_test() {
  menu_system.change_menu(menu_test);
  return;
}
void goto_menu_calib() {
  menu_system.change_menu(menu_calib);
  return;
}
void goto_menu_reset() {
  menu_system.change_menu(menu_reset);
  return;
}
void goto_menu_light() {
  menu_system.change_menu(menu_light);
  return;
}
void goto_menu_view() {
  menu_system.change_menu(menu_view);
  return;
}
void goto_menu_vbowl() {
  menu_system.change_menu(menu_vbowl);
  return;
}

void goto_menu_start(){ 
 while(enter.check() == HIGH){
    menu_system.change_menu(menu_hint);
    long lastMillis_test =millis();
    int t=0;
     while (t<8800){
        t=millis()-lastMillis_test;
       // keyVal= analogRead(A0);
        if (t < 800) {  
          showhint=(char*)"          PRESS/"; 
          screenshowcontrol();}
        if (t > 800 && t< 1600) { 
          showhint=(char*)"          ENTER/"; 
          screenshowcontrol();}
        if (t > 1600 && t< 2400) { 
          showhint=(char*)"             TO/"; 
          screenshowcontrol();}
        if (t > 2400 && t< 3200) { 
          showhint=(char*)"         BEGAIN/";
         screenshowcontrol();}
        if (t > 3200 && t< 4000) { 
          showhint=(char*)"            SAM/";
          screenshowcontrol();}
        if (t > 4000 && t< 4800) { 
          showhint=(char*)"          CALIB/";
          screenshowcontrol();}
        if (t > 4800 && t< 5600) { 
          showhint=(char*)"             OR/";
         screenshowcontrol();}
        if (t > 5600 && t< 6400) { 
          showhint=(char*)"           HOLD/";
          screenshowcontrol();}
        if (t > 6400 && t< 7200) { 
          showhint=(char*)"           MENU/";
          screenshowcontrol();}
        if (t > 7200 && t< 8000) { 
          showhint=(char*)"             TO/";
          screenshowcontrol();}
        if (t > 8000 && t< 8800) {
          showhint=(char*)"           ABORT";
          screenshowcontrol();}
        if (enter.check() == LOW) {goto_menu_startEnter(); return;}}}
 }

void goto_menu_startEnter(){
  goto_menu_apply1450();
  goto_menu_startEnterEnter();
  return;
}

void goto_menu_apply1450(){ 
  while(enter.check() == HIGH){   
    menu_system.change_menu(menu_hint);
    long lastMillis_test =millis();
    int t=0;
     while (t<4800){
        t=millis()-lastMillis_test;
        if (t < 800) {  
          showhint=(char*)"          APPLY/"; 
          screenshowcontrol();}
        if (t > 800 && t< 1600) { 
          showhint=(char*)"          14.50/"; 
          screenshowcontrol();}
        if (t > 1600 && t< 2400) { 
          showhint=(char*)"            PSI/"; 
          screenshowcontrol();}
        if (t > 2400 && t< 3200) { 
          showhint=(char*)"           THEN/";
          screenshowcontrol();}
        if (t > 3200 && t< 4000) { 
          showhint=(char*)"          PRESS/";
          screenshowcontrol();}
        if (t > 4000 && t< 4800) { 
          showhint=(char*)"           ENTER";
          screenshowcontrol();}
        if (enter.check()==LOW){return;}
 }}}
 
void goto_menu_startEnterEnter(){ 
  menu_system.change_menu(menu_hint); 
  /****************************************************************************************************
   * capture five measurements while flashing “READ”
  *****************************************************************************************************/
  long lastMillis_test =millis();
  int t=0;
  while (t<7100){ 
    t=millis()-lastMillis_test;
    if (t < 5000) {
      int a=t/400;
      if ((a%2)!=0){  
        showhint=(char*)"            READ";  
        screenshowcontrol();}
        else {
          showhint=(char*)" ";  
          screenshowcontrol();}}
     if (t > 5000 && t< 7000) { 
         if(keyVal>800){
            showhint=(char*)"      NOT STABLE"; 
            screenshowcontrol();}
         else{
          if(keyVal>200 && keyVal<600){               
              showhint=(char*)"              OK"; 
              screenshowcontrol();}
          if(keyVal<200){
              showhint=(char*)"             LOW"; 
              screenshowcontrol();}
           if(keyVal>600 && keyVal<800){
              showhint=(char*)"            HIGH"; 
              screenshowcontrol();}
            }}
     if (t > 7000 && t <7100){
         if(keyVal<200 || keyVal>600 ){goto_menu_startEnter(); return;}
         if(keyVal>200 && keyVal<600){goto_menu_startEEOK();return;}
}}}

void goto_menu_startEEOK(){
    goto_menu_holdhint();
    goto_menu_holdcount();
    goto_menu_startEEEnter();
    return;
 }

void goto_menu_holdhint(){  
  while(enter.check()==HIGH){
    menu_system.change_menu(menu_hint);
    long lastMillis_test =millis();
    int t=0;
    while (t<4000){
        t=millis()-lastMillis_test;
        if (t < 800) { 
              showhint=(char*)"           HOLD/"; 
              screenshowcontrol();}
        if (t > 800 && t< 1600) { 
              showhint=(char*)"          LEVER/"; 
              screenshowcontrol();}
        if (t > 1600 && t< 2400) { 
              showhint=(char*)"           THEN/";
              screenshowcontrol();}
        if (t > 2400 && t< 3200) { 
               showhint=(char*)"          PRESS/";
              screenshowcontrol();}
        if (t > 3200 && t< 4000) { 
              showhint=(char*)"           ENTER";
              screenshowcontrol();}
        if (enter.check() == LOW) {
              return;}
 }}}

void goto_menu_holdhithint(){  
  while(enter.check()==HIGH){
    menu_system.change_menu(menu_hint);
    long lastMillis_test =millis();
    int t=0;
    while (t<4800){
        t=millis()-lastMillis_test;
        if (t < 800) { 
              showhint=(char*)"           HOLD/"; 
              screenshowcontrol();}
        if (t > 800 && t< 1600) { 
              showhint=(char*)"          LEVER/"; 
              screenshowcontrol();}
        if (t > 1600 && t< 2400) { 
              showhint=(char*)"            HIT/";
              screenshowcontrol();}
        if (t > 2400 && t< 3200) { 
              showhint=(char*)"           THEN/";
              screenshowcontrol();}
        if (t > 3200 && t< 4000) { 
              showhint=(char*)"          PRESS/";
              screenshowcontrol();}
        if (t > 4000 && t< 4800) { 
              showhint=(char*)"           ENTER";
              screenshowcontrol();}
        if (enter.check() == LOW) {
              return;}
 }}}

void goto_menu_holdcount(){
    menu_system.change_menu(menu_hint);
    long lastMillis_test =millis();
    int t=0;
    while (t<8100){
        t=millis()-lastMillis_test; 
        if ( t< 800) { 
              showhint=(char*)"           HOLD9";
              screenshowcontrol();}
        if (t > 800 && t< 1600) { 
              showhint=(char*)"           HOLD8";
              screenshowcontrol();}
        if (t > 1600 && t< 2400) { 
              showhint=(char*)"           HOLD7";
              screenshowcontrol();}
        if (t > 2400 && t< 3200) { 
              showhint=(char*)"           HOLD6";
              screenshowcontrol();}
        if (t > 3200 && t< 4000) { 
              showhint=(char*)"           HOLD5";
              screenshowcontrol();}
        if (t > 4000 && t< 4800) { 
              showhint=(char*)"           HOLD4";
              screenshowcontrol();}
        if (t > 4800 && t< 5600) { 
              showhint=(char*)"           HOLD3";
              screenshowcontrol();}
        if (t > 5600 && t< 6400) { 
              showhint=(char*)"           HOLD2";
              screenshowcontrol();}
        if (t > 6400 && t< 7200) { 
              showhint=(char*)"           HOLD1";
              screenshowcontrol();}
        if (t > 7200 && t< 8000) { 
              showhint=(char*)"           HOLD0";
              screenshowcontrol();}
}}

void goto_menu_startEEEnter(){
    goto_menu_readhint();
    if(keyVal>800){goto_Error();return;}
    else{
        P1avgtest=keyVal;
        goto_menu_OKhint();
        goto_menu_startEEEEnter();
        return;
        }
}

void goto_menu_readhint(){
  menu_system.change_menu(menu_hint);
  long lastMillis_test =millis();
  int t=0;
  while (t<5000){ 
    t=millis()-lastMillis_test;
    int a=t/400;
    if ((a%2)!=0){  
        showhint=(char*)"            READ";  
        screenshowcontrol();}
    else {
        showhint=(char*)" ";  
        screenshowcontrol();}}}

void goto_menu_OKhint(){
  menu_system.change_menu(menu_hint);
  long lastMillis_test =millis();
  int t=0;
  while (t<2000){ 
    t=millis()-lastMillis_test;
    showhint=(char*)"              OK";  
    screenshowcontrol();
   }
}

void goto_Error(){ 
  while(enter.check() == HIGH){   
    menu_system.change_menu(menu_hint);
    long lastMillis_test =millis();
    int t=0;
    while (t<5600){
        t=millis()-lastMillis_test;
        if (t < 800) {  
          showhint=(char*)"          ERROR/"; 
          screenshowcontrol();}
        if (t > 800 && t< 1600) { 
          showhint=(char*)"            NOT/"; 
          screenshowcontrol();}
        if (t > 1600 && t< 2400) { 
          showhint=(char*)"         STABLE/"; 
          screenshowcontrol();}
        if (t > 2400 && t< 3200) { 
          showhint=(char*)"          PRESS/";
          screenshowcontrol();}
        if (t > 3200 && t< 4000) { 
          showhint=(char*)"          ENTER/";
         screenshowcontrol();}
        if (t > 4000 && t< 4800) { 
          showhint=(char*)"             TO/";
          screenshowcontrol();}
        if (t > 4800 && t< 5600) { 
          showhint=(char*)"            EXIT";
          screenshowcontrol();}
        if (enter.check() == LOW) {gobackto_menu_main();return;}
 }}}

void goto_menu_startEEEEnter(){
  goto_menu_apply3000();
  goto_menu_P2Enter();
  return;
}

void goto_menu_apply3000(){ 
  while(enter.check() == HIGH){   
    menu_system.change_menu(menu_hint);
    long lastMillis_test =millis();
    int t=0;
     while (t<4800){
       // keyVal= analogRead(A0);
        t=millis()-lastMillis_test;
        if (t < 800) {  
          showhint=(char*)"          APPLY/"; 
          screenshowcontrol();}
        if (t > 800 && t< 1600) { 
          showhint=(char*)"          30.00/"; 
         screenshowcontrol();}
        if (t > 1600 && t< 2400) { 
          showhint=(char*)"            PSI/"; 
          screenshowcontrol();}
        if (t > 2400 && t< 3200) { 
          showhint=(char*)"           THEN/";
          screenshowcontrol();}
        if (t > 3200 && t< 4000) { 
          showhint=(char*)"          PRESS/";
         screenshowcontrol();}
        if (t > 4000 && t< 4800) { 
          showhint=(char*)"           ENTER";
          screenshowcontrol();}
        if (enter.check() == LOW) { return;}
 }}}

void goto_menu_P2Enter(){ 
  menu_system.change_menu(menu_hint); 
  /****************************************************************************************************
   * capture five measurements while flashing “READ”
  *****************************************************************************************************/
  long lastMillis_test =millis();
  int t=0;
  while (t<7100){ 
    t=millis()-lastMillis_test;
    if (t < 5000) {
      int a=t/400;
      if ((a%2)!=0){  
        showhint=(char*)"            READ";  
        screenshowcontrol();}
      else {
        showhint=(char*)" ";  
        screenshowcontrol();}}
    if (t > 5000 && t< 7000) { 
         if(keyVal>200 && keyVal<600){               
            showhint=(char*)"              OK"; 
            screenshowcontrol();}
         if(keyVal<200){
            showhint=(char*)"             LOW"; 
            screenshowcontrol();}
         if(keyVal>600 && keyVal<800){
            showhint=(char*)"            HIGH"; 
            screenshowcontrol();}
            }
     if (t > 7000 && t <7100){
         if(keyVal<200 || keyVal>600){goto_menu_startEEEEnter();return;}
         if(keyVal>200 && keyVal<600){goto_menu_P2EOK();return;}
}}}

void goto_menu_P2EOK(){
  goto_menu_holdhint();
  goto_menu_holdcount();
  goto_menu_P2EEnter();
  return;
}

void goto_menu_P2EEnter(){
  goto_menu_readhint();
  if(keyVal>800){goto_Error();return;}
  else{
        P2avgtest=keyVal;
        goto_menu_OKhint();
        goto_menu_P2EEEnter();return;
        }
}

void goto_menu_P2EEEnter(){
  goto_menu_apply4500();
  goto_menu_P3Enter();
  return;
}
         
void goto_menu_apply4500(){
 while(enter.check() == HIGH){   
    menu_system.change_menu(menu_hint);
    long lastMillis_test =millis();
    int t=0;
     while (t<4800){
        t=millis()-lastMillis_test;
        if (t < 800) {  
          showhint=(char*)"          APPLY/"; 
          screenshowcontrol();}
        if (t > 800 && t< 1600) { 
          showhint=(char*)"          45.00/"; 
          screenshowcontrol();}
        if (t > 1600 && t< 2400) { 
          showhint=(char*)"            PSI/"; 
          screenshowcontrol();}
        if (t > 2400 && t< 3200) { 
          showhint=(char*)"           THEN/";
          screenshowcontrol();}
        if (t > 3200 && t< 4000) { 
          showhint=(char*)"          PRESS/";
          screenshowcontrol();}
        if (t > 4000 && t< 4800) { 
          showhint=(char*)"           ENTER";
          screenshowcontrol();}
        if (enter.check() == LOW) {return;}
 }}}

 void goto_menu_P3Enter(){
  menu_system.change_menu(menu_hint); 
  /****************************************************************************************************
   * capture five measurements while flashing “READ”
  *****************************************************************************************************/
  long lastMillis_test =millis();
  int t=0;
  while (t<7100){ 
    t=millis()-lastMillis_test;
    if (t < 5000) {
      int a=t/400;
      if ((a%2)!=0){  
        showhint=(char*)"            READ";  
        screenshowcontrol();}
      else {
        showhint=(char*)" ";  
        screenshowcontrol();}}
    if (t > 5000 && t< 7000) { 
         if(keyVal>200 && keyVal<600){               
            showhint=(char*)"              OK"; 
            screenshowcontrol();}
         if(keyVal<200){
            showhint=(char*)"             LOW"; 
            screenshowcontrol();}
         if(keyVal>600 && keyVal<800){
            showhint=(char*)"            HIGH"; 
            screenshowcontrol();}
            }
     if (t > 7000 && t <7100){
         if(keyVal<200 || keyVal>600){goto_menu_P2EEEnter();return;}
         if(keyVal>200 && keyVal<600){goto_menu_P3EOK();return;}
         }}}

void goto_menu_P3EOK(){
  goto_menu_holdhint();
  goto_menu_holdcount();
  goto_menu_P3EEnter();
  return;
}

void goto_menu_P3EEnter(){
    goto_menu_readhint();
    if(keyVal>800){goto_Error();return;}
    else{
        P3avgtest=keyVal;
        // This is where the user removes the lid and places a calibration can in the chamber.
        goto_menu_OKhint();
        goto_menu_P3EEEnter();
        return;
        }
    }

void goto_menu_P3EEEnter(){
  // This is where the user removes the lid and places a calibration can in the chamber.
   while(enter.check() == HIGH){   
    menu_system.change_menu(menu_hint);
    long lastMillis_test =millis();
    int t=0;
     while (t<5600){
        t=millis()-lastMillis_test;
        if (t < 800) {  
          showhint=(char*)"          PURGE/"; 
          screenshowcontrol();}
        if (t > 800 && t< 1600) { 
          showhint=(char*)"            ADD/"; 
          screenshowcontrol();}
        if (t > 1600 && t< 2400) { 
          showhint=(char*)"            ONE/"; 
          screenshowcontrol();}
        if (t > 2400 && t< 3200) { 
          showhint=(char*)"            CAN/";
          screenshowcontrol();}
        if (t > 3200 && t< 4000) { 
          showhint=(char*)"           THEN/";
          screenshowcontrol();}
        if (t > 4000 && t< 4800) { 
          showhint=(char*)"           PRESS";
          screenshowcontrol();}
        if (t > 4800 && t< 5600) { 
          showhint=(char*)"           ENTER";
          screenshowcontrol();}
        if (enter.check() == LOW) {goto_menu_P3EEEEnter();return;/*if select button has been poshed,then stop showing the hint.*/}
 }}}

void goto_menu_P3EEEEnter(){
   goto_menu_apply1450();
   goto_menu_P4Enter();
   return;
 }

void goto_menu_P4Enter(){
  menu_system.change_menu(menu_hint); 
  /****************************************************************************************************
   * capture five measurements while flashing “READ”
  *****************************************************************************************************/
  long lastMillis_test =millis();
  int t=0;
  while (t<7100){ 
    t=millis()-lastMillis_test;
    if (t < 5000) {
      int a=t/400;
      if ((a%2)!=0){  
        showhint=(char*)"            READ";  
        screenshowcontrol();}
      else {
        showhint=(char*)" ";  
        screenshowcontrol();}}
    if (t > 5000 && t< 7000) { 
         if(keyVal>200 && keyVal<600){               
            showhint=(char*)"              OK"; 
            screenshowcontrol();}
         if(keyVal<200){
            showhint=(char*)"             LOW"; 
            screenshowcontrol();}
         if(keyVal>600 && keyVal<800){
            showhint=(char*)"            HIGH"; 
            screenshowcontrol();}
            }
     if (t > 7000 && t <7100){
         if(keyVal<200 || keyVal>600){goto_menu_P3EEEEnter();return;}
         if(keyVal>200 && keyVal<600){goto_menu_P4EOK();return;}
}}}

void goto_menu_P4EOK(){
  goto_menu_holdhint();
  goto_menu_holdcount();
  goto_menu_P4EEnter();
  return;
}

void goto_menu_P4EEnter(){
  goto_menu_readhint();
  if(keyVal>800){goto_Error();return;}
  else{
        P4avgtest=keyVal;
        // PressCorr = 7.51 – P4avg 
        // Note: This finds the correction to the equilibrium pressure to force meter to read 4.9% air +/-0.1% with a Pro Cali Can
        goto_menu_OKhint();
        goto_menu_P4EEEnter();return;}
}

void goto_menu_P4EEEnter(){
   while(enter.check() == HIGH){   
    menu_system.change_menu(menu_hint);
    long lastMillis_test =millis();
    int t=0;
     while (t<9600){
        t=millis()-lastMillis_test;
        if (t < 800) {  
          showhint=(char*)"            SAM/"; 
          screenshowcontrol();}
        if (t > 800 && t< 1600) { 
          showhint=(char*)"          CALIB/"; 
          screenshowcontrol();}
        if (t > 1600 && t< 2400) { 
          showhint=(char*)"           DONE/"; 
          screenshowcontrol();}
        if (t > 2400 && t< 3200) { 
          showhint=(char*)"          PRESS/";
          screenshowcontrol();}
        if (t > 3200 && t< 4000) { 
          showhint=(char*)"          ENTER/";
          screenshowcontrol();}
        if (t > 4000 && t< 4800) { 
          showhint=(char*)"             TO/";
          screenshowcontrol();}
        if (t > 4800 && t< 5600) { 
          showhint=(char*)"           SAVE/";
          screenshowcontrol();}
        if (t > 5600 && t< 6400) { 
          showhint=(char*)"             OR/";
          screenshowcontrol();}
        if (t > 6400 && t< 7200) { 
          showhint=(char*)"           HOLD/";
          screenshowcontrol();}
        if (t > 7200 && t< 8000) { 
          showhint=(char*)"           MENU/";
          screenshowcontrol();}
        if (t > 8000 && t< 8800) { 
          showhint=(char*)"             TO/";
          screenshowcontrol();}
        if (t > 8800 && t< 9600) { 
          showhint=(char*)"           ABORT";
          screenshowcontrol();}
        if (enter.check() == LOW) {
          P1avg=P1avgtest;
          P2avg=P2avgtest;
          P3avg=P3avgtest;
          P4avg=P4avgtest;
          goto_menu_P4EEEEnter();return;/*if select button has been poshed,then stop showing the hint.*/}
 }}}

void goto_menu_P4EEEEnter(){
  menu_system.change_menu(menu_hint);
  long lastMillis_test =millis();
  int t=0;
  while (t<2100){ 
    t=millis()-lastMillis_test;
    if (t < 2000) {
            showhint=(char*)"           SAVED";  
            screenshowcontrol();}
    if ( t>2000 && t<2100 ){
      gobackto_menu_main();return;}
}}
//*SAM TEST BELOW ******************************************************************************************************************
void goto_menu_sam(){
  while(enter.check() == HIGH){
    menu_system.change_menu(menu_hint);
    long lastMillis_test =millis();
    int t=0;
     while (t<8800){
        t=millis()-lastMillis_test;
       // keyVal= analogRead(A0);
        if (t < 800) {  
          showhint=(char*)"          PRESS/"; 
          screenshowcontrol();}
        if (t > 800 && t< 1600) { 
          showhint=(char*)"          ENTER/"; 
          screenshowcontrol();}
        if (t > 1600 && t< 2400) { 
          showhint=(char*)"             TO/"; 
          screenshowcontrol();}
        if (t > 2400 && t< 3200) { 
          showhint=(char*)"         BEGAIN/";
         screenshowcontrol();}
        if (t > 3200 && t< 4000) { 
          showhint=(char*)"            SAM/";
          screenshowcontrol();}
        if (t > 4000 && t< 4800) { 
          showhint=(char*)"           TEST/";
          screenshowcontrol();}
        if (t > 4800 && t< 5600) { 
          showhint=(char*)"             OR/";
         screenshowcontrol();}
        if (t > 5600 && t< 6400) { 
          showhint=(char*)"           HOLD/";
          screenshowcontrol();}
        if (t > 6400 && t< 7200) { 
          showhint=(char*)"           MENU/";
          screenshowcontrol();}
        if (t > 7200 && t< 8000) { 
          showhint=(char*)"             TO/";
          screenshowcontrol();}
        if (t > 8000 && t< 8800) {
          showhint=(char*)"           ABORT";
          screenshowcontrol();}
        if (enter.check() == LOW) {goto_menu_samEnter(); return;}}}
}

void goto_menu_samEnter(){
  goto_menu_apply1450();
  goto_menu_samEnterEnter();
  return;
}

void goto_menu_samEnterEnter(){
  menu_system.change_menu(menu_hint); 
  /****************************************************************************************************
   * capture five measurements while flashing “READ”
  *****************************************************************************************************/
  long lastMillis_test =millis();
  int t=0;
  while (t<7100){ 
    t=millis()-lastMillis_test;
    if (t < 5000) {
      int a=t/400;
      if ((a%2)!=0){  
        showhint=(char*)"            READ";  
        screenshowcontrol();}
        else {
          showhint=(char*)" ";  
          screenshowcontrol();}}
     if (t > 5000 && t< 7000) { 
         if(keyVal>800){
            showhint=(char*)"      NOT STABLE"; 
            screenshowcontrol();}
         else{
          if(keyVal>200 && keyVal<600){               
              showhint=(char*)"              OK"; 
              screenshowcontrol();}
          if(keyVal<200){
              showhint=(char*)"             LOW"; 
              screenshowcontrol();}
           if(keyVal>600 && keyVal<800){
              showhint=(char*)"            HIGH"; 
              screenshowcontrol();}
            }}
     if (t > 7000 && t <7100){
         if(keyVal<200 || keyVal>600 ){goto_menu_samEnter(); return;}
         if(keyVal>200 && keyVal<600){goto_menu_samEEOK();return;}
}}}

void goto_menu_samEEOK(){
  goto_menu_holdhithint();
  goto_menu_holdcount();
  goto_menu_samEEEnter();
  return;
}

void goto_menu_samEEEnter(){
   goto_menu_readhint();
   if(keyVal>800){goto_Error();return;}
   else{
        //P1meas=keyVal;
        goto_menu_OKhint();
        goto_menu_samEEEEnter();return;}
}

void goto_menu_samEEEEnter(){
  goto_menu_viewairvalue();
  goto_menu_P1meas();
}

void goto_menu_viewairvalue(){
  while(enter.check() == HIGH){   
    menu_system.change_menu(menu_hint);
    long lastMillis_test =millis();
    int t=0;
     while (t<4800){
        t=millis()-lastMillis_test;
        if (t < 800) {  
          showhint=(char*)"          PRESS/"; 
          screenshowcontrol();}
        if (t > 800 && t< 1600) { 
          showhint=(char*)"          ENTER/"; 
          screenshowcontrol();}
        if (t > 1600 && t< 2400) { 
          showhint=(char*)"             TO/"; 
          screenshowcontrol();}
        if (t > 2400 && t< 3200) { 
          showhint=(char*)"           VIEW/";
          screenshowcontrol();}
        if (t > 3200 && t< 4000) { 
          showhint=(char*)"            AIR/";
          screenshowcontrol();}
        if (t > 4000 && t< 4800) { 
          showhint=(char*)"           VALUE";
          screenshowcontrol();}
        if (enter.check() == LOW) {return;}
}}}

void goto_menu_P1meas(){
  goto_menu_airvalue();
  goto_menu_P1measEnter();
}

void goto_menu_airvalue(){
  while(enter.check() == HIGH){   
    menu_system.change_menu(menu_hint);
    long lastMillis_test =millis();
    int t=0;
     while (t<7200){
        t=millis()-lastMillis_test;
        if (t < 800) {  
          showhint=(char*)"             AIR/"; 
          screenshowcontrol();}
        if (t > 800 && t< 1600) { 
          showhint=(char*)"          VALUE/"; 
          screenshowcontrol();}
        if (t > 1600 && t< 2400) { 
          showhint=(char*)"          PRESS/"; 
          screenshowcontrol();}
        if (t > 2400 && t< 3200) { 
          showhint=(char*)"           ENTER/";
          screenshowcontrol();}
        if (t > 3200 && t< 4000) { 
          showhint=(char*)"             OR/";
          screenshowcontrol();}
        if (t > 4000 && t< 4800) { 
          showhint=(char*)"            HOLD";
          screenshowcontrol();}
        if (t > 4800 && t< 5600) { 
          showhint=(char*)"            MENU";
          screenshowcontrol();}
        if (t > 5600 && t< 6400) { 
          showhint=(char*)"              TO";
          screenshowcontrol();}
        if (t > 6400 && t< 7200) { 
          showhint=(char*)"           ABORT";
          screenshowcontrol();}
        if (enter.check() == LOW) {return;}
}}}

void goto_menu_P1measEnter(){
  goto_menu_apply3000();
  goto_menu_P1measEEnter();
}

void goto_menu_P1measEEnter(){
  menu_system.change_menu(menu_hint); 
  /****************************************************************************************************
   * capture five measurements while flashing “READ”
  *****************************************************************************************************/
  long lastMillis_test =millis();
  int t=0;
  while (t<7100){ 
    t=millis()-lastMillis_test;
    if (t < 5000) {
      int a=t/400;
      if ((a%2)!=0){  
        showhint=(char*)"            READ";  
        screenshowcontrol();}
        else {
          showhint=(char*)" ";  
          screenshowcontrol();}}
     if (t > 5000 && t< 7000) { 
         if(keyVal>800){
            showhint=(char*)"      NOT STABLE"; 
            screenshowcontrol();}
         else{
          if(keyVal>200 && keyVal<600){               
              showhint=(char*)"              OK"; 
              screenshowcontrol();}
          if(keyVal<200){
              showhint=(char*)"             LOW"; 
              screenshowcontrol();}
           if(keyVal>600 && keyVal<800){
              showhint=(char*)"            HIGH"; 
              screenshowcontrol();}
            }}
     if (t > 7000 && t <7100){
         if(keyVal<200 || keyVal>600 ){goto_menu_P1measEnter(); return;}
         if(keyVal>200 && keyVal<600){goto_menu_P1measEEOK();return;}
}}}

void goto_menu_P1measEEOK(){
  goto_menu_holdhithint();
  goto_menu_holdcount();
  goto_menu_P1measEEEnter();
  return;
}

void goto_menu_P1measEEEnter(){
  goto_menu_readhint();
  if(keyVal>800){goto_Error();return;}
  else{
        //P2meas=keyVal;
        goto_menu_OKhint();
        goto_menu_P2meas();return;}
}

void goto_menu_P2meas(){
  goto_menu_apply4500();
  goto_menu_P2measEnter();
  return;
}

void goto_menu_P2measEnter(){
  menu_system.change_menu(menu_hint); 
  /****************************************************************************************************
   * capture five measurements while flashing “READ”
  *****************************************************************************************************/
  long lastMillis_test =millis();
  int t=0;
  while (t<7100){ 
    t=millis()-lastMillis_test;
    if (t < 5000) {
      int a=t/400;
      if ((a%2)!=0){  
        showhint=(char*)"            READ";  
        screenshowcontrol();}
        else {
          showhint=(char*)" ";  
          screenshowcontrol();}}
     if (t > 5000 && t< 7000) { 
         if(keyVal>800){
            showhint=(char*)"      NOT STABLE"; 
            screenshowcontrol();}
         else{
          if(keyVal>200 && keyVal<600){               
              showhint=(char*)"              OK"; 
              screenshowcontrol();}
          if(keyVal<200){
              showhint=(char*)"             LOW"; 
              screenshowcontrol();}
           if(keyVal>600 && keyVal<800){
              showhint=(char*)"            HIGH"; 
              screenshowcontrol();}
            }}
     if (t > 7000 && t <7100){
         if(keyVal<200 || keyVal>600 ){goto_menu_P2meas(); return;}
         if(keyVal>200 && keyVal<600){goto_menu_P2measEOK();return;}
}}}

void goto_menu_P2measEOK(){
  goto_menu_holdhithint();
  goto_menu_holdcount();
  goto_menu_P2measEEnter();
  return;
}

void goto_menu_P2measEEnter(){
  goto_menu_readhint();
  if(keyVal>800){goto_Error();return;}
  else{
        //P3meas=keyVal;
        goto_menu_OKhint();
        goto_menu_P3meas();return;}
}

void goto_menu_P3meas(){
  goto_menu_Purge();
  if(i=1){goto_menu_samEnter();i++;return;}
  else {goto_menu_P3measEnter();return;}
}

void goto_menu_Purge(){
  while(enter.check() == HIGH){   
    menu_system.change_menu(menu_hint);
    long lastMillis_test =millis();
    int t=0;
     while (t<4800){
        t=millis()-lastMillis_test;
        if (t < 800) {  
          showhint=(char*)"          PURGE/"; 
          screenshowcontrol();}
        if (t > 800 && t< 1600) { 
          showhint=(char*)"            AND/"; 
          screenshowcontrol();}
        if (t > 1600 && t< 2400) { 
          showhint=(char*)"          RERUN/"; 
          screenshowcontrol();}
        if (t > 2400 && t< 3200) { 
          showhint=(char*)"           THEN/";
          screenshowcontrol();}
        if (t > 3200 && t< 4000) { 
          showhint=(char*)"          PRESS/";
          screenshowcontrol();}
        if (t > 4000 && t< 4800) { 
          showhint=(char*)"           ENTER";
          screenshowcontrol();}
        if (enter.check() == LOW) {return;}
 }}}

void goto_menu_P3measEnter(){
   while(enter.check() == HIGH){   
    menu_system.change_menu(menu_hint);
    long lastMillis_test =millis();
    int t=0;
     while (t<9600){
        t=millis()-lastMillis_test;
        if (t < 800) {  
          showhint=(char*)"            SAM/"; 
          screenshowcontrol();}
        if (t > 800 && t< 1600) { 
          showhint=(char*)"           TEST/"; 
          screenshowcontrol();}
        if (t > 1600 && t< 2400) { 
          showhint=(char*)"           DONE/"; 
          screenshowcontrol();}
        if (t > 2400 && t< 3200) { 
          showhint=(char*)"          PRESS/";
          screenshowcontrol();}
        if (t > 3200 && t< 4000) { 
          showhint=(char*)"          ENTER/";
          screenshowcontrol();}
        if (t > 4000 && t< 4800) { 
          showhint=(char*)"             TO/";
          screenshowcontrol();}
        if (t > 4800 && t< 5600) { 
          showhint=(char*)"           SAVE/";
          screenshowcontrol();}
        if (t > 5600 && t< 6400) { 
          showhint=(char*)"             OR/";
          screenshowcontrol();}
        if (t > 6400 && t< 7200) { 
          showhint=(char*)"           HOLD/";
          screenshowcontrol();}
        if (t > 7200 && t< 8000) { 
          showhint=(char*)"           MENU/";
          screenshowcontrol();}
        if (t > 8000 && t< 8800) { 
          showhint=(char*)"             TO/";
          screenshowcontrol();}
        if (t > 8800 && t< 9600) { 
          showhint=(char*)"            EXIT";
          screenshowcontrol();}
        if (enter.check() == LOW) {
         // P1avg=P1avgtest;
         // P2avg=P2avgtest;
         // P3avg=P3avgtest;
         // P4avg=P4avgtest;
          goto_menu_P3measEEnter();return;/*if select button has been poshed,then stop showing the hint.*/}
}}}

void goto_menu_P3measEEnter(){
  menu_system.change_menu(menu_hint);
  long lastMillis_test =millis();
  int t=0;
  while (t<2100){ 
    t=millis()-lastMillis_test;
    if (t < 2000) {
            showhint=(char*)"           SAVED";  
            screenshowcontrol();}
    if ( t>2000 && t<2100 ){
      gobackto_menu_main();return;}
}}
//***TYPEB TEST*****************************************************************

void goto_menu_typeb(){
  while(enter.check() == HIGH){
    menu_system.change_menu(menu_hint);
    long lastMillis_test =millis();
    int t=0;
     while (t<8800){
        t=millis()-lastMillis_test;
       // keyVal= analogRead(A0);
        if (t < 800) {  
          showhint=(char*)"          PRESS/"; 
          screenshowcontrol();}
        if (t > 800 && t< 1600) { 
          showhint=(char*)"          ENTER/"; 
          screenshowcontrol();}
        if (t > 1600 && t< 2400) { 
          showhint=(char*)"             TO/"; 
          screenshowcontrol();}
        if (t > 2400 && t< 3200) { 
          showhint=(char*)"          BEGIN/";
         screenshowcontrol();}
        if (t > 3200 && t< 4000) { 
          showhint=(char*)"          TYPEB/";
          screenshowcontrol();}
        if (t > 4000 && t< 4800) { 
          showhint=(char*)"           TEST/";
          screenshowcontrol();}
        if (t > 4800 && t< 5600) { 
          showhint=(char*)"             OR/";
         screenshowcontrol();}
        if (t > 5600 && t< 6400) { 
          showhint=(char*)"           HOLD/";
          screenshowcontrol();}
        if (t > 6400 && t< 7200) { 
          showhint=(char*)"           MENU/";
          screenshowcontrol();}
        if (t > 7200 && t< 8000) { 
          showhint=(char*)"             TO/";
          screenshowcontrol();}
        if (t > 8000 && t< 8800) {
          showhint=(char*)"           ABORT";
          screenshowcontrol();}
        if (enter.check() == LOW) {goto_menu_typebEnter(); return;}}}
}

void goto_menu_typebEnter(){
  goto_menu_apply1450();
  goto_menu_typebEnterEnter();
  return;
}

void goto_menu_typebEnterEnter(){
  menu_system.change_menu(menu_hint); 
  /****************************************************************************************************
   * capture five measurements while flashing “READ”
  *****************************************************************************************************/
  long lastMillis_test =millis();
  int t=0;
  while (t<7100){ 
    t=millis()-lastMillis_test;
    if (t < 5000) {
      int a=t/400;
      if ((a%2)!=0){  
        showhint=(char*)"            READ";  
        screenshowcontrol();}
        else {
          showhint=(char*)" ";  
          screenshowcontrol();}}
     if (t > 5000 && t< 7000) { 
         if(keyVal>800){
            showhint=(char*)"      NOT STABLE"; 
            screenshowcontrol();}
         else{
          if(keyVal>200 && keyVal<600){               
              showhint=(char*)"              OK"; 
              screenshowcontrol();}
          if(keyVal<200){
              showhint=(char*)"             LOW"; 
              screenshowcontrol();}
           if(keyVal>600 && keyVal<800){
              showhint=(char*)"            HIGH"; 
              screenshowcontrol();}
            }}
     if (t > 7000 && t <7100){
         if(keyVal<200 || keyVal>600 ){goto_menu_typebEnter(); return;}
         if(keyVal>200 && keyVal<600){goto_menu_typebEEOK();return;}
}}}

void goto_menu_typebEEOK(){
  goto_menu_holdhithint();
  goto_menu_holdcount();
  goto_menu_typebEEEnter();
  return;
}

void goto_menu_typebEEEnter(){
  goto_menu_readhint();
  if(keyVal>800){goto_Error();return;}
  else{
        //P3meas=keyVal;
        goto_menu_OKhint();
        goto_menu_typebEEEEnter();return;}
}

void goto_menu_typebEEEEnter(){
  goto_menu_viewairvalue();
  goto_menu_typebEEEEEnter();
}

void goto_menu_typebEEEEEnter(){
  goto_menu_airvalue();
  gobackto_menu_main();
}

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  menu_system.add_menu(menu_light);
  menu_system.add_menu(menu_view);
  menu_system.add_menu(menu_vbowl);
  menu_system.add_menu(menu_hint);
  
  test.attach_function(ENTER, goto_menu_test);
  calib.attach_function(ENTER, goto_menu_calib);
  reset.attach_function(ENTER, goto_menu_reset);
  light.attach_function(ENTER, goto_menu_light);
  view.attach_function(ENTER, goto_menu_view);
  vbowl.attach_function(ENTER, goto_menu_vbowl);
  start.attach_function(ENTER, goto_menu_start);
  sam.attach_function(ENTER, goto_menu_sam);
  typeb.attach_function(ENTER, goto_menu_typeb);
  
  goback_main.attach_function(ENTER, gobackto_menu_main);
  goback_calib.attach_function(ENTER, goto_menu_calib);
  goback_view.attach_function(ENTER, goto_menu_view);
  goback_start.attach_function(ENTER, goto_menu_calib);

  menu_system.update();
}

void loop() {
  buttonsCheck();
  keyVal= analogRead(A0);
  Serial.print("keyVal:");
  Serial.println(keyVal);
  screenshowcontrol();}
