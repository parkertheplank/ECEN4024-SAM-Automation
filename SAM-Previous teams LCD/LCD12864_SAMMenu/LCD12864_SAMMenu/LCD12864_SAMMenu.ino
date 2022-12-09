/*

  Menu.pde
  
  Simple Menu Selection

  >>> Before compiling: Please remove comment from the constructor of the 
  >>> connected graphics display (see below).
  
  Universal 8bit Graphics Library, https://github.com/olikraus/u8glib/
  
  Copyright (c) 2012, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  

*/


#include "U8glib.h"
#include "Button.h"
#include <stdio.h>

U8GLIB_KS0108_128 u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 14, 15, 17, 16); 		// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs1=14, cs2=15,di=17,rw=16

float error = 5.00;
float minval = 13.00;
float maxval = 50.00;
float Vc = 1;
float Vbowl = 1;
float Presscorr = 1.00;

Button up(22, true);//previous screen
Button down(26, true);//next screen
Button enter(30, true);
Button back(34, true);

float measure[5];
float meas_error[5];
float P1avg,P2avg,P3avg,P4avg;
char *PoiP1avg = "####";
char *PoiP2avg = "####";
char *PoiP3avg = "####";
char *PoiP4avg = "####";
char strP1avg[10],strP2avg[10], strP3avg[10], strP4avg[10];//item number must be 10, otherwise there'll be garbled after transfer into string
float P1meas, P2meas, P3meas, P4meas, P5meas, P6meas;
char strP1meas[10], strP2meas[10], strP3meas[10], strP4meas[10], strP5meas[10], strP6meas[10];
float air;
char strair[10];
char *Poiair = "####";
float air_T;
char strair_T[10];
char *Poiair_T = "####";
float SAM;
char strSAM[10];
char *PoiSAM = "####";
float P1measmod;
float pressure;
char strpressure[10];
char *Poipressure;


char *menu_main[6] = { "1.TEST ", "2.CALIB", "3.RESET", "4.LIGHT","","Zero"};
char *menu_test[2] = {"1.SAM", "2.TYPEB"};
char *menu_calib[2] = {"1.START", "2.VIEW"};
char *menu_reset[2] = {"1.CNFRM", "2.CANCL"};
char *menu_light[6] = {"OFF", "ON", "PRESS", "20MIN", "5MIN", "1MIN"};
char *menu_view[10] = {"P1-A:", "P2-A:", "P3-A:", "P4-A:", "VTOP", "VBOWL",PoiP1avg,PoiP2avg,PoiP3avg,PoiP4avg};
char *menu_vbowl[1] = {"RECAL"};
char *calstart_e[4] = {"","PRESS ENTER TO BEGIN", "SAM CALIB OR PRESS", "BACK TO ABORT"};
char *PSI_145[12] = {"", "", "APPLY 14.50 PSI THEN", "PRESS ENTER","","Pressure:","","","","","",""};
char *PSI_OK[12] = {"","","HOLD LEVER THEN","PRESS ENTER","","Pressure:","","","","","",""};
char *error_message[4] = {"","","ERROR! NOT STABLE!","PRESS ENTER TO EXIT"};
char *PSI_30[12] = {"", "", "APPLY 30.00 PSI THEN", "PRESS ENTER","","Pressure:","","","","","",""};
char *PSI_45[12] = {"", "", "APPLY 45.00 PSI THEN", "PRESS ENTER","","Pressure:","","","","","",""};
char *calib_can[4] = {"","","PURGE ADD ONE CAN THEN", "PRESS ENTER"};
char *calib_done[4] = {"","SAM CALIB DONE","PRESS ENTER TO SAVE /","PRESS BACK TO ABORT"};
char *SAM_e[4] = {"","PRESS ENTER TO BEGIN", "SAM TEST OR PRESS", "BACK TO ABORT"};
char *SAMPSI_OK[12] = {"","","HOLD LEVER HIT THEN","PRESS ENTER","","Pressure:","","","","","",""};
char *SAMP1_right[4] = {"","","PRESS ENTER TO VIEW","AIR VALUE"};
char *view_airval[6] = {"","AIR VALUE:","PRESS ENTER OR","PRESS BACK TO ABORT","",Poiair};
char *SAM_RERUN[4] = {"","","PURGE AND RERUN","THEN PRESS ENTER"};
char *_done[4] = {"","DONE","PRESS ENTER TO VIEW /","OR PRESS BACK TO EXIT"};
char *air_SAM[10] = {"","","AIR:","SAM:","","ABORT","","",Poiair,PoiSAM};
char *hold_count[10] = {"HOLD9","HOLD8","HOLD7","HOLD6","HOLD5","HOLD4","HOLD3","HOLD2","HOLD1","HOLD0"};
char *typeb_e[4] = {"","PRESS ENTER TO BEGIN","TYPEB TEST OR PRESS","BACK TO ABORT"};
char *air_val[8] = {"","","AIR:","","ABORT","","",Poiair_T};

//////////////////////////////////////////////////////////////////////////////////////////////
void goto_addpoint(char *a){
  a[4] = a[3];
  a[3] = a[2];
  a[2] = '.';
}
///////////////////////////// basic set && get the value of h and w   
uint8_t h;
u8g_uint_t w;  
void basicset(){  
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  
  h = u8g.getFontAscent()-u8g.getFontDescent();// h=9
  w = u8g.getWidth();//w=128
  //Serial.println(h);
  //Serial.println(w);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void getpressure(){
  pressure = analogRead(A8);
  int newF1 = round(pressure*100);
  sprintf(strpressure,"%03d",newF1);
  goto_addpoint(strpressure);
  Poipressure = &strpressure[0];
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void MENU_SHOW(int MENU_ITEMS01, int menu_current, int MENU_ITEMS02, int val_start_posi,  char *menu_content[]){
  u8g.firstPage();
  do{ uint8_t i;
      for( i = 0; i < MENU_ITEMS01; i++ ) {
        u8g.setDefaultForegroundColor();    
        if ( i == menu_current ) {
          u8g.drawBox(0, i*h, 64, h);
          u8g.setDefaultBackgroundColor();
        }    
        u8g.drawStr(1, i*h, menu_content[i]);
      }
      for( i = 0; i < MENU_ITEMS02; i++ ) {
        u8g.setDefaultForegroundColor();        
        u8g.drawStr(val_start_posi, i*h, menu_content[i+MENU_ITEMS01]);
      }
  }while(u8g.nextPage());  
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void DYNAMIC_SHOW(int x, int y, float timespan, char *showcontent, bool showpre){
  uint8_t draw_state = 0;
  long lastMillis_test =millis();
  int t=0;
  while (t<(timespan*1000)){ 
    t=millis()-lastMillis_test;
    u8g.firstPage();  
    do {
        switch(draw_state >> 3) { //右移运算符 ">>" 将第一个操作数向右移动第二个操作数指定的位数。
          case 0: u8g.drawStr( x, y, showcontent);
          case 1: u8g.drawStr( 0, 0, "");}

        if (showpre == 1){
          u8g.drawStr(1,45,"Pressure:");
          getpressure();  
          u8g.drawStr(61,45,Poipressure); 
        }
              
       } while( u8g.nextPage() );  
    draw_state++;
    if ( draw_state >= 15*1 ){draw_state = 0;}
  }  
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void goto_main0(void) {
  while(!(up.check()==LOW||down.check()==LOW || enter.check()==LOW)){
  MENU_SHOW(6,0,0,0,menu_main);   
  if (down.check() == LOW){goto_main1();}
  if (up.check()==LOW){goto_main4();}
  if (enter.check()==LOW){goto_test0();}
  }
}

void goto_main1(void) {
  while(!(up.check()==LOW||down.check()==LOW || enter.check()==LOW)){
  MENU_SHOW(6,1,0,0,menu_main);
  if (down.check() == LOW){goto_main2();}
  if (up.check()==LOW){goto_main0();}
  if (enter.check()==LOW){goto_calib0();}
  }
}

void goto_main2(void) {
  while(!(up.check()==LOW||down.check()==LOW || enter.check()==LOW)){
  MENU_SHOW(6,2,0,0,menu_main);
  if (down.check() == LOW){goto_main3();}
  if (up.check()==LOW){goto_main1();}
  if (enter.check()==LOW){goto_reset0();}
  }
}

void goto_main3(void) {
  while(!(up.check()==LOW ||down.check()==LOW || enter.check()==LOW)){
  MENU_SHOW(6,3,0,0,menu_main);
  if (down.check() == LOW){goto_main4();}
  if (up.check()==LOW){goto_main2();}
  if (enter.check()==LOW){goto_light0();}
  }
}

void goto_main4(void){
  while(!(up.check()==LOW ||down.check()==LOW || enter.check()==LOW)){
  MENU_SHOW(6,5,0,0,menu_main);
  if (down.check() == LOW){goto_main0();}
  if (up.check()==LOW){goto_main3();}
  if (enter.check()==LOW){goto_zero();}
  }
}

void goto_zero(){
  
}
//////////////////////////////////////////////////////////////////////////////////////////////////
void goto_test0(){
  while(!(up.check()==LOW||down.check()==LOW || enter.check()==LOW || back.check()==LOW)){
  MENU_SHOW(2,0,0,0,menu_test); 
  if (down.check() == LOW){goto_test1();}
  if (up.check()==LOW){goto_test1();}
  if (back.check()==LOW){goto_main0();}
  if (enter.check()==LOW){goto_SAM_e();}
  }
}

void goto_test1(){
  while(!(up.check()==LOW || enter.check()==LOW || down.check()==LOW || back.check()==LOW)){
  MENU_SHOW(2,1,0,0,menu_test); 
  if (down.check() == LOW){goto_test0();}
  if (up.check()==LOW){goto_test0();}
  if (back.check()==LOW){goto_main0();}
  if (enter.check()==LOW){goto_typeb_e();}
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void goto_calib0(){
  while(!(up.check()==LOW || down.check()==LOW || enter.check()==LOW || back.check()==LOW)){
  MENU_SHOW(2,0,0,0,menu_calib); 
  if (down.check() == LOW){goto_calib1();}
  if (up.check()==LOW){goto_calib1();}
  if (back.check()==LOW){goto_main1();}
  if (enter.check()==LOW){goto_calstart_e();}
  }
}
void goto_calib1(){
  while(!(up.check()==LOW || down.check()==LOW || enter.check()==LOW || back.check()==LOW)){
  MENU_SHOW(2,1,0,0,menu_calib); 
  if (down.check() == LOW){goto_calib0();}
  if (up.check()==LOW){goto_calib0();}
  if (back.check()==LOW){goto_main1();}
  if (enter.check()==LOW){goto_view0();}
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void goto_reset0(){
  while(!(up.check()==LOW||down.check()==LOW || back.check()==LOW)){
  MENU_SHOW(2,0,0,0,menu_reset); 
  if (down.check() == LOW){goto_reset1();}
  if (up.check()==LOW){goto_reset1();}
  if (back.check()==LOW){goto_main2();}
  }
}
void goto_reset1(){
  while(!(up.check()==LOW||down.check()==LOW || back.check()==LOW)){
  MENU_SHOW(2,1,0,0,menu_reset); 
  if (down.check() == LOW){goto_reset0();}
  if (up.check()==LOW){goto_reset0();}
  if (back.check()==LOW){goto_main2();}
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////
void goto_light0(){
  while(!(up.check()==LOW||down.check()==LOW || back.check()==LOW)){
  MENU_SHOW(6,0,0,0,menu_light); 
  if (down.check() == LOW){goto_light1();}
  if (up.check()==LOW){goto_light5();}
  if (back.check()==LOW){goto_main3();}
  }
}

void goto_light1(){
  while(!(up.check()==LOW||down.check()==LOW || back.check()==LOW)){
  MENU_SHOW(6,1,0,0,menu_light);  
  if (down.check() == LOW){goto_light2();}
  if (up.check()==LOW){goto_light0();}
  if (back.check()==LOW){goto_main3();}
  }
}

void goto_light2(){
  while(!(up.check()==LOW||down.check()==LOW || back.check()==LOW)){
  MENU_SHOW(6,2,0,0,menu_light); 
  if (down.check() == LOW){goto_light3();}
  if (up.check()==LOW){goto_light1();}
  if (back.check()==LOW){goto_main3();}
  }
}

void goto_light3(){
  while(!(up.check()==LOW||down.check()==LOW || back.check()==LOW)){
  MENU_SHOW(6,3,0,0,menu_light);  
  if (down.check() == LOW){goto_light4();}
  if (up.check()==LOW){goto_light2();}
  if (back.check()==LOW){goto_main3();}
  }
}

void goto_light4(){
  while(!(up.check()==LOW||down.check()==LOW || back.check()==LOW)){
  MENU_SHOW(6,4,0,0,menu_light);  
  if (down.check() == LOW){goto_light5();}
  if (up.check()==LOW){goto_light3();}
  if (back.check()==LOW){goto_main3();}
  }
}

void goto_light5(){
  while(!(up.check()==LOW||down.check()==LOW || back.check()==LOW)){
  MENU_SHOW(6,5,0,0,menu_light); 
  if (down.check() == LOW){goto_light0();}
  if (up.check()==LOW){goto_light4();}
  if (back.check()==LOW){goto_main3();}
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////
void goto_view0(){
  while(!(enter.check()==LOW || back.check()==LOW)){
  MENU_SHOW(6,5,4,31,menu_view); 
  if (enter.check()==LOW){goto_vbowl0();}
  if (back.check()==LOW){goto_calib1();}
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
void goto_vbowl0(){
  while(!(back.check()==LOW)){
  MENU_SHOW(1,0,0,0,menu_vbowl); 
  if (back.check()==LOW){goto_view0();}
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////
void goto_calstart_e(){
  while(!(enter.check()==LOW || back.check()==LOW)){
  MENU_SHOW(4,4,0,0,calstart_e); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (back.check()==LOW){goto_calib0();}
  if (enter.check()==LOW){goto_PSI145();}
  }
}

void goto_PSI145(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(6,6,6,61,PSI_145); //"menu_current = MENU_ITEMS" means there is no need to draw box  
  getpressure();
  PSI_145[11] = &strpressure[0];
  
  if (enter.check()==LOW){
    for (int i=0; i<5; i++){
      if ((i%2) != 0){DYNAMIC_SHOW(52,20,0.8,"READ",1);}      
      measure[i]=analogRead(A8);
      Serial.println(measure[i]);} 
    Serial.println("/**********************/");
    goto_read145();}
  }
}

void goto_read145(){
  float sum=0, meas_max=measure[0], meas_min=measure[0];
  for(int i=0; i<5; i++){
    sum+=measure[i];
    if(meas_max < measure[i]){meas_max = measure[i];}
    if(meas_min > measure[i]){meas_min = measure[i];}
    }
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]=measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){DYNAMIC_SHOW(34,20,1.6,"NOT STABLE",1);
    goto_PSI145();}
    }
  if (meas_min < minval){DYNAMIC_SHOW(55,20,1.6,"LOW",1);goto_PSI145();}
  if (meas_max > maxval){DYNAMIC_SHOW(52,20,1.6,"HIGH",1);goto_PSI145();}
  
  DYNAMIC_SHOW(58,20,1.6,"OK",1); 
  goto_read145OK();
  
}

void goto_read145OK(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(6,6,6,61,PSI_OK); //"menu_current = MENU_ITEMS" means there is no need to draw box
  getpressure();
  PSI_OK[11] = &strpressure[0];
  if (enter.check()==LOW){
    goto_hold();
    goto_P1avg();}
  }
}

void goto_hold(){
  for(int i=0; i<10; i++){DYNAMIC_SHOW(46,20,0.8,hold_count[i],1);}
  for (int i=0; i<5; i++){
      if ((i%2) != 0){DYNAMIC_SHOW(52,20,0.8,"READ",1);}  
      measure[i]=analogRead(A8);
      Serial.println(measure[i]);}      
  Serial.println("/**********************/");
}

void goto_P1avg(){
  float sum=0;
  for(int i=0; i<5; i++){sum+=measure[i];}
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]=measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){goto_error();}
    }
  P1avg = avg;
  int newF1 = round(P1avg*100);
  sprintf(strP1avg,"%03d",newF1);
  goto_addpoint(strP1avg);
  
  Serial.print("P1avg:");
  Serial.print("\t");
  Serial.println(P1avg);
  goto_P1right(); // get the P1avg
}

void goto_error(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(4,4,0,0,error_message); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (enter.check()==LOW){goto_calstart_e();}
  }
}

void goto_P1right(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(6,6,6,61,PSI_30); //"menu_current = MENU_ITEMS" means there is no need to draw box
  getpressure();
  PSI_30[11] = &strpressure[0];
  if (enter.check()==LOW){
    for (int i=0; i<5; i++){
      if ((i%2) != 0){DYNAMIC_SHOW(52,20,0.8,"READ",1);}  
      measure[i]=analogRead(A8);
      Serial.println(measure[i]);}     
    Serial.println("/**********************/");
    goto_read30();}
  }
}

void goto_read30(){
  float sum=0, meas_max=measure[0], meas_min=measure[0];
  for(int i=0; i<5; i++){
    sum+=measure[i];
    if(meas_max < measure[i]){meas_max = measure[i];}
    if(meas_min > measure[i]){meas_min = measure[i];}
    }
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]=measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){DYNAMIC_SHOW(34,20,1.6,"NOT STABLE",1);
    goto_P1right();}
  }
  if (meas_min < minval){DYNAMIC_SHOW(55,20,1.6,"LOW",1);goto_P1right();}
  if (meas_max > maxval){DYNAMIC_SHOW(52,20,1.6,"HIGH",1);goto_P1right();}
  
  DYNAMIC_SHOW(58,20,1.6,"OK",1); 
  goto_read30OK();
}

void goto_read30OK(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(6,6,6,61,PSI_OK); //"menu_current = MENU_ITEMS" means there is no need to draw box
  getpressure();
  PSI_OK[11]=&strpressure[0];
  if (enter.check()==LOW){
    goto_hold();
    goto_P2avg();}
  }
}

void goto_P2avg(){
  float sum=0;
  for(int i=0; i<5; i++){sum+=measure[i];}
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]=measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){goto_error();}
    }
  P2avg = avg;
  int newF2 = round(P2avg*100);
  sprintf(strP2avg,"%03d",newF2);
  goto_addpoint(strP2avg);
  
  Serial.print("P2avg:");
  Serial.print("\t");
  Serial.println(P2avg);
  goto_P2right(); // get the P1avg
}

void goto_P2right(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(6,6,6,61,PSI_45); //"menu_current = MENU_ITEMS" means there is no need to draw box
  getpressure();
  PSI_45[11] = &strpressure[0];
  if (enter.check()==LOW){
    for (int i=0; i<5; i++){
      if ((i%2) != 0){DYNAMIC_SHOW(52,20,0.8,"READ",1);}  
      measure[i]=analogRead(A8);
      Serial.println(measure[i]);} 
    Serial.println("/**********************/");
    goto_read45();}
  }
}

void goto_read45(){
  float sum=0, meas_max=measure[0], meas_min=measure[0];
  for(int i=0; i<5; i++){
    sum+=measure[i];
    if(meas_max < measure[i]){meas_max = measure[i];}
    if(meas_min > measure[i]){meas_min = measure[i];}
    }
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]=measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){DYNAMIC_SHOW(34,20,1.6,"NOT STABLE",1);goto_P2right();}}  
  if (meas_min < minval){DYNAMIC_SHOW(55,20,1.6,"LOW",1);goto_P2right();}
  if (meas_max > maxval){DYNAMIC_SHOW(52,20,1.6,"HIGH",1);goto_P2right();}

  DYNAMIC_SHOW(58,20,1.6,"OK",1); 
  goto_read45OK();
}

void goto_read45OK(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(6,6,6,61,PSI_OK); //"menu_current = MENU_ITEMS" means there is no need to draw box
  getpressure();
  PSI_OK[11]=&strpressure[0];
  if (enter.check()==LOW){
    goto_hold();
    goto_P3avg();}
  }
}

void goto_P3avg(){
  float sum=0;
  for(int i=0; i<5; i++){sum+=measure[i];}
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]=measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){goto_error();}
    }
  P3avg = avg;
  int newF3 = round(P3avg*100);
  sprintf(strP3avg,"%03d",newF3);
  goto_addpoint(strP3avg);
  
  Serial.print("P3avg:");
  Serial.print("\t");
  Serial.println(P3avg);  
  Serial.print(strP3avg[0]);
  Serial.print(strP3avg[1]);
  Serial.print(strP3avg[2]);
  Serial.print(strP3avg[3]);
  Serial.print(strP3avg[4]);
  Serial.println(strP3avg[5]);
  goto_P3right(); // get the P1avg
}

void goto_P3right(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(4,4,0,0,calib_can); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (enter.check()==LOW){goto_calibcan();}
  }
}

void goto_calibcan(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(4,4,0,0,PSI_145); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (enter.check()==LOW){
    for (int i=0; i<5; i++){
      if ((i%2) != 0){DYNAMIC_SHOW(52,20,0.8,"READ",1);}      
      measure[i]=analogRead(A8);
      Serial.println(measure[i]);} 
    Serial.println("/**********************/"); 
    goto_calib145();}
  }
}

void goto_calib145(){
  float sum=0, meas_max=measure[0], meas_min=measure[0];
  for(int i=0; i<5; i++){
    sum+=measure[i];
    if(meas_max < measure[i]){meas_max = measure[i];}
    if(meas_min > measure[i]){meas_min = measure[i];}
    }
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]= measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){
      DYNAMIC_SHOW(34,20,1.6,"NOT STABLE",1);
      goto_calibcan();}
    }
  if (meas_min < minval){DYNAMIC_SHOW(55,20,1.6,"LOW",1);goto_calibcan();}
  if (meas_max > maxval){DYNAMIC_SHOW(52,20,1.6,"HIGH",1);goto_calibcan();}
  
  DYNAMIC_SHOW(58,20,1.6,"OK",1); 
  goto_calib145OK();  
}

void goto_calib145OK(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(6,6,6,61,PSI_OK); //"menu_current = MENU_ITEMS" means there is no need to draw box
  getpressure();
  PSI_OK[11]=&strpressure[0];
  if (enter.check()==LOW){
    goto_hold();
    goto_P4avg();}
  }
}

void goto_P4avg(){
  float sum=0;
  for(int i=0; i<5; i++){sum+=measure[i];}
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]=measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){goto_error();}
    }
  P4avg = avg;
  int newF4 = round(P4avg*100);
  sprintf(strP4avg,"%03d",newF4);
  goto_addpoint(strP4avg);
  
  Serial.print("P4avg:");
  Serial.print("\t");
  Serial.println(P4avg);
  goto_P4right(); // get the P1avg
}

void goto_P4right(){
  while(!(enter.check()==LOW || back.check() == LOW )){
  MENU_SHOW(4,4,0,0,calib_done); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (enter.check()==LOW){ 
     menu_view[6]=&strP1avg[0];
     menu_view[7]=&strP2avg[0];
     menu_view[8]=&strP3avg[0];
     menu_view[9]=&strP4avg[0];   
      
     DYNAMIC_SHOW(49,20,1.6,"SAVED",0); 
     goto_view0();}
  if (back.check()==LOW){goto_main1();}
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////
void goto_SAM_e(){
  while(!(enter.check()==LOW || back.check()==LOW)){
  MENU_SHOW(4,4,0,0,SAM_e); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (back.check()==LOW){goto_test0();}
  if (enter.check()==LOW){goto_SAMPSI145();}
  }
}

void goto_SAMPSI145(){
  while(!(enter.check()==LOW)){
  MENU_SHOW(4,4,0,0,PSI_145); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (enter.check()==LOW){
    for (int i=0; i<5; i++){
      if ((i%2) != 0){DYNAMIC_SHOW(52,20,0.8,"READ",1);}      
      measure[i]=analogRead(A8);
      Serial.println(measure[i]);} 
    Serial.println("/**********************/"); 
    goto_readSAM145();}
  }
}

void goto_readSAM145(){
  float sum=0, meas_max=measure[0], meas_min=measure[0];
  for(int i=0; i<5; i++){
    sum+=measure[i];
    if(meas_max < measure[i]){meas_max = measure[i];}
    if(meas_min > measure[i]){meas_min = measure[i];}
    }
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]= measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){DYNAMIC_SHOW(34,20,1.6,"NOT STABLE",1);
    goto_SAMPSI145();}
    }  
  if (meas_min < minval){DYNAMIC_SHOW(55,20,1.6,"LOW",1);goto_SAMPSI145();}
  if (meas_max > maxval){DYNAMIC_SHOW(52,20,1.6,"HIGH",1);goto_SAMPSI145();}

  DYNAMIC_SHOW(58,20,1.6,"OK",1); 
  goto_readSAM145OK();
}

void goto_readSAM145OK(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(6,6,6,61,SAMPSI_OK); //"menu_current = MENU_ITEMS" means there is no need to draw box
  getpressure();
  SAMPSI_OK[11]=&strpressure[0];
  if (enter.check()==LOW){
    goto_hold();
    goto_P1meas();}
  }
}

void goto_P1meas(){
  float sum=0;
  for(int i=0; i<5; i++){sum+=measure[i];}
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]= measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){goto_SAMerror();}
    }
  P1meas = avg;
  int newF1 = round(P1meas*100);
  sprintf(strP1meas,"%03d",newF1);
  goto_addpoint(strP1meas);

  //P1measmod = PressCorr + P1meas;
  //air = 100*Vc/Vbowl*((14.5/14.69595+1)/(P1measmod/14.69595+1)-1)*(1+14.69595/P1measmod);
  air = P1meas + 10;
  int newF2 = round(air*100);
  sprintf(strair,"%03d",newF2);
  goto_addpoint(strair);
  
  view_airval[5] = &strair[0];
  
  Serial.print("P1meas:");
  Serial.print("\t");
  Serial.println(P1meas);
  Serial.print("strP1meas:");
  Serial.print("\t");
  Serial.println(strP1meas);
  Serial.print("air:");
  Serial.print("\t");
  Serial.println(air);
  Serial.print("strair:");
  Serial.print("\t");
  Serial.println(strair);
  goto_SAMP1right(); // get the P1meas
}

void goto_SAMerror(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(4,4,0,0,error_message); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (enter.check()==LOW){goto_SAM_e();}
  }
}

void goto_SAMP1right(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(4,4,0,0,SAMP1_right); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (enter.check()==LOW){goto_viewair();}
  }
}

void goto_viewair(){
  while(!(enter.check()==LOW || back.check()==LOW )){
  MENU_SHOW(4,4,2,67,view_airval); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (enter.check()==LOW){goto_SAMPSI30();}
  if (back.check()==LOW){goto_test0();}
  }
}

void goto_SAMPSI30(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(4,4,0,0,PSI_30); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (enter.check()==LOW){
    for (int i=0; i<5; i++){
      if ((i%2) != 0){DYNAMIC_SHOW(52,20,0.8,"READ",1);}      
      measure[i]=analogRead(A8);
      Serial.println(measure[i]);} 
    Serial.println("/**********************/");
    goto_readSAM30();}
  }
}

void goto_readSAM30(){
  float sum=0, meas_max=measure[0], meas_min=measure[0];
  for(int i=0; i<5; i++){
    sum+=measure[i];
    if(meas_max < measure[i]){meas_max = measure[i];}
    if(meas_min > measure[i]){meas_min = measure[i];}
    }
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]=measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){DYNAMIC_SHOW(34,20,1.6,"NOT STABLE",1);
    goto_SAMPSI30();}
    }  
  if (meas_min < minval){DYNAMIC_SHOW(55,20,1.6,"LOW",1);goto_SAMPSI30();}
  if (meas_max > maxval){DYNAMIC_SHOW(52,20,1.6,"HIGH",1);goto_SAMPSI30();}

  DYNAMIC_SHOW(58,20,1.6,"OK",1); 
  goto_readSAM30OK();
}

void goto_readSAM30OK(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(6,6,6,61,SAMPSI_OK); //"menu_current = MENU_ITEMS" means there is no need to draw box
  getpressure();
  SAMPSI_OK[11]=&strpressure[0];
  if (enter.check()==LOW){
    goto_hold();
    goto_P2meas();}
  }
}

void goto_P2meas(){
  float sum=0;
  for(int i=0; i<5; i++){sum+=measure[i];}
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]=measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){goto_SAMerror();}
    }
  P2meas = avg;
  int newF2 = round(P2meas*100);
  sprintf(strP2meas,"%03d",newF2);
  goto_addpoint(strP2meas);
  
  Serial.print("P2meas:");
  Serial.print("\t");
  Serial.println(P2meas);
  goto_SAMP2right(); // get the P1meas
}

void goto_SAMP2right(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(4,4,0,0,PSI_45); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (enter.check()==LOW){
    for (int i=0; i<5; i++){
      if ((i%2) != 0){DYNAMIC_SHOW(52,20,0.8,"READ",1);}      
      measure[i]=analogRead(A8);
      Serial.println(measure[i]);} 
    Serial.println("/**********************/");
    goto_readSAM45();}
  }
}

void goto_readSAM45(){
  float sum=0, meas_max=measure[0], meas_min=measure[0];
  for(int i=0; i<5; i++){
    sum+=measure[i];
    if(meas_max < measure[i]){meas_max = measure[i];}
    if(meas_min > measure[i]){meas_min = measure[i];}
    }
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]=measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){DYNAMIC_SHOW(34,20,1.6,"NOT STABLE",1);
    goto_SAMP2right();}
    } 
  if (meas_min < minval){DYNAMIC_SHOW(55,20,1.6,"LOW",1);goto_SAMP2right();}
  if (meas_max > maxval){DYNAMIC_SHOW(52,20,1.6,"HIGH",1);goto_SAMP2right();}
    
  DYNAMIC_SHOW(58,20,1.6,"OK",1); 
  goto_readSAM45OK();
}

void goto_readSAM45OK(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(6,6,6,61,SAMPSI_OK); //"menu_current = MENU_ITEMS" means there is no need to draw box
  getpressure();
  SAMPSI_OK[11]=&strpressure[0];
  if (enter.check()==LOW){
    goto_hold();
    goto_P3meas();}
  }
}

void goto_P3meas(){
  float sum=0;
  for(int i=0; i<5; i++){sum+=measure[i];}
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]=measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){goto_SAMerror();}
    }
  P3meas = avg;
  int newF3 = round(P3meas*100);
  sprintf(strP3meas,"%03d",newF3);
  goto_addpoint(strP3meas);
  
  Serial.print("P3meas:");
  Serial.print("\t");
  Serial.println(P3meas);
  goto_SAMP3right(); // get the P1meas
}

void goto_SAMP3right(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(4,4,0,0,SAM_RERUN); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (enter.check()==LOW){goto_SAMrerun145();}
  }
}

void goto_SAMrerun145(){
  while(!(enter.check()==LOW)){
  MENU_SHOW(4,4,0,0,PSI_145); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (enter.check()==LOW){
    for (int i=0; i<5; i++){
      if ((i%2) != 0){DYNAMIC_SHOW(52,20,0.8,"READ",1);}      
      measure[i]=analogRead(A8);
      Serial.println(measure[i]);} 
    Serial.println("/**********************/"); 
    goto_readSAMrerun145();}
  }
}

void goto_readSAMrerun145(){
  float sum=0, meas_max=measure[0], meas_min=measure[0];
  for(int i=0; i<5; i++){
    sum+=measure[i];
    if(meas_max < measure[i]){meas_max = measure[i];}
    if(meas_min > measure[i]){meas_min = measure[i];}
    }
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]=measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){DYNAMIC_SHOW(34,20,1.6,"NOT STABLE",1);
    goto_SAMrerun145();}
    } 
  if (meas_min < minval){DYNAMIC_SHOW(55,20,1.6,"LOW",1);goto_SAMrerun145();}
  if (meas_max > maxval){DYNAMIC_SHOW(52,20,1.6,"HIGH",1);goto_SAMrerun145();}

  DYNAMIC_SHOW(58,20,1.6,"OK",1); 
  goto_readSAMrerun145OK();
}

void goto_readSAMrerun145OK(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(6,6,6,61,SAMPSI_OK); //"menu_current = MENU_ITEMS" means there is no need to draw box
  getpressure();
  SAMPSI_OK[11]=&strpressure[0];
  if (enter.check()==LOW){
    goto_hold();
    goto_P4meas();}
  }
}

void goto_P4meas(){
  float sum=0;
  for(int i=0; i<5; i++){sum+=measure[i];}
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]=measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){goto_SAMerror();}
    }
  P4meas = avg;
  int newF4 = round(P4meas*100);
  sprintf(strP4meas,"%03d",newF4);
  goto_addpoint(strP4meas);
  
  Serial.print("P4meas:");
  Serial.print("\t");
  Serial.println(P4meas);
  goto_SAMP4right(); // get the P1avg
}

void goto_SAMP4right(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(4,4,0,0,PSI_30); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (enter.check()==LOW){
    for (int i=0; i<5; i++){
      if ((i%2) != 0){DYNAMIC_SHOW(52,20,0.8,"READ",1);}      
      measure[i]=analogRead(A8);
      Serial.println(measure[i]);} 
    Serial.println("/**********************/");
    goto_readSAMrerun30();}
  }
}

void goto_readSAMrerun30(){
  float sum=0, meas_max=measure[0], meas_min=measure[0];
  for(int i=0; i<5; i++){
    sum+=measure[i];
    if(meas_max < measure[i]){meas_max = measure[i];}
    if(meas_min > measure[i]){meas_min = measure[i];}
    }
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]=measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){DYNAMIC_SHOW(34,20,1.6,"NOT STABLE",1);
    goto_SAMP4right();}
    }  
  if (meas_min < minval){DYNAMIC_SHOW(55,20,1.6,"LOW",1);goto_SAMP4right();}
  if (meas_max > maxval){DYNAMIC_SHOW(52,20,1.6,"HIGH",1);goto_SAMP4right();}
  
  DYNAMIC_SHOW(58,20,1.6,"OK",1);
  goto_readSAMrerun30OK();
}

void goto_readSAMrerun30OK(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(6,6,6,61,SAMPSI_OK); //"menu_current = MENU_ITEMS" means there is no need to draw box
  getpressure();
  SAMPSI_OK[11]=&strpressure[0];
  if (enter.check()==LOW){
    goto_hold();
    goto_P5meas();}
  }
}

void goto_P5meas(){
  float sum=0;
  for(int i=0; i<5; i++){sum+=measure[i];}
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]=measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){goto_SAMerror();}
    }
  P5meas = avg;
  int newF5 = round(P5meas*100);
  sprintf(strP5meas,"%03d",newF5);
  goto_addpoint(strP5meas);
  
  Serial.print("P5meas:");
  Serial.print("\t");
  Serial.println(P5meas);
  goto_SAMP5right(); // get the P1avg
}

void goto_SAMP5right(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(4,4,0,0,PSI_45); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (enter.check()==LOW){
    for (int i=0; i<5; i++){
      if ((i%2) != 0){DYNAMIC_SHOW(52,20,0.8,"READ",1);}      
      measure[i]=analogRead(A8);
      Serial.println(measure[i]);} 
    Serial.println("/**********************/");
    goto_readSAMrerun45();}
  }
}

void goto_readSAMrerun45(){
  float sum=0, meas_max=measure[0], meas_min=measure[0];
  for(int i=0; i<5; i++){
    sum+=measure[i];
    if(meas_max < measure[i]){meas_max = measure[i];}
    if(meas_min > measure[i]){meas_min = measure[i];}
    }
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]=measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){DYNAMIC_SHOW(34,20,1.6,"NOT STABLE",1);
    goto_SAMP5right();}
    }  
  if (meas_min < minval){DYNAMIC_SHOW(55,20,1.6,"LOW",1);goto_SAMP5right();}
  if (meas_max > maxval){DYNAMIC_SHOW(52,20,1.6,"HIGH",1);goto_SAMP5right();}
  
  DYNAMIC_SHOW(58,20,1.6,"OK",1); 
  goto_readSAMrerun45OK();
}

void goto_readSAMrerun45OK(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(6,6,6,61,SAMPSI_OK); //"menu_current = MENU_ITEMS" means there is no need to draw box
  getpressure();
  SAMPSI_OK[11]=&strpressure[0];
  if (enter.check()==LOW){
    goto_hold();
    goto_P6meas();}
}
}

void goto_P6meas(){
  float sum=0;
  for(int i=0; i<5; i++){sum+=measure[i];}
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]=measure[i]-avg;}

  for(int i=0; i<5; i++){
    if (meas_error[i]>error){goto_SAMerror();}
    }
  P6meas = avg;
  int newF6 = round(P6meas*100);
  sprintf(strP6meas,"%03d",newF6);
  goto_addpoint(strP6meas);
  
  //SAM = P6meas-P3meas;
  SAM = P6meas + 10;
  int newF7 = round(SAM*100);
  sprintf(strSAM,"%03d",newF7);
  goto_addpoint(strSAM);
  
  air_SAM[9] = &strSAM[0];
  
  Serial.print("P6meas:");
  Serial.print("\t");
  Serial.println(P6meas);
  Serial.print("SAM:");
  Serial.print("\t");
  Serial.println(SAM);
  Serial.print("strSAM:");
  Serial.print("\t");
  Serial.println(strSAM);
  goto_rerundone(); // get the P1avg
}

void goto_rerundone(){
  while(!(enter.check()==LOW || back.check()==LOW )){
  MENU_SHOW(4,4,0,0,_done); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (enter.check()==LOW){goto_SAMdisplay();}
  if (back.check()==LOW){goto_test0();}
  }
}
void goto_SAMdisplay(){
  while(!(enter.check()==LOW)){
  MENU_SHOW(6,5,4,31,air_SAM); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (enter.check()==LOW){goto_test0();}
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////
void goto_typeb_e(){
  while(!(enter.check()==LOW || back.check()==LOW)){
  MENU_SHOW(4,4,0,0,typeb_e); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (back.check()==LOW){goto_test1();}
  if (enter.check()==LOW){goto_typeb145();}
  }
}

void goto_typeb145(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(6,6,6,61,PSI_145); //"menu_current = MENU_ITEMS" means there is no need to draw box
  getpressure();
  PSI_145[11]=&strpressure[0];
  if (enter.check()==LOW){
    for (int i=0; i<5; i++){
      if ((i%2) != 0){DYNAMIC_SHOW(52,20,0.8,"READ",1);}      
      measure[i]=analogRead(A8);
      Serial.println(measure[i]);} 
    Serial.println("/**********************/");
    goto_readtypeb145();}
  }
}

void goto_readtypeb145(){
  float sum=0, meas_max=measure[0], meas_min=measure[0];
  for(int i=0; i<5; i++){
    sum+=measure[i];
    if(meas_max < measure[i]){meas_max = measure[i];}
    if(meas_min > measure[i]){meas_min = measure[i];}
    }
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]=measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){DYNAMIC_SHOW(34,20,1.6,"NOT STABLE",1);
    goto_typeb145();}
    }
  if (meas_min < minval){DYNAMIC_SHOW(55,20,1.6,"LOW",1);goto_typeb145();}
  if (meas_max > maxval){DYNAMIC_SHOW(52,20,1.6,"HIGH",1);goto_typeb145();}
  
  DYNAMIC_SHOW(58,20,1.6,"OK",1); 
  goto_readtypeb145OK();
}

void goto_readtypeb145OK(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(6,6,6,61,SAMPSI_OK); //"menu_current = MENU_ITEMS" means there is no need to draw box
  getpressure();
  SAMPSI_OK[11]=&strpressure[0];
  if (enter.check()==LOW){
    goto_hold();
    goto_typebP1meas();}
  }
}

void goto_typebP1meas(){
  float sum=0;
  for(int i=0; i<5; i++){sum+=measure[i];}
  float avg=sum/5;
  for(int i=0; i<5; i++){meas_error[i]=measure[i]-avg;}
  
  for(int i=0; i<5; i++){
    if (meas_error[i]>error){goto_typeberror();}
    }
  P1meas = avg;
  P1measmod = Presscorr+P1meas;

  //air_T = 100*Vc/Vbowl*((14.5/14.69595+1)/(P1measmod/14.69595+1)-1)*(1+14.69595/P1measmod)
  air_T = P1measmod + 5;
  int newF1 = round(air_T*100);
  sprintf(strair_T,"%03d",newF1);
  goto_addpoint(strair_T);

  air_val[7] = &strair_T[0];
  
  Serial.print("air_T:");
  Serial.print("\t");
  Serial.println(air_T);
  Serial.print("strair_T:");
  Serial.print("\t");
  Serial.println(strair_T);
  goto_typebdone(); // get the P1avg
}

void goto_typeberror(){
  while(!(enter.check()==LOW )){
  MENU_SHOW(4,4,0,0,error_message); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (enter.check()==LOW){goto_typeb_e();}
  }
}

void goto_typebdone(){
  while(!(enter.check()==LOW || back.check()==LOW)){
  MENU_SHOW(4,4,0,0,_done); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (enter.check()==LOW){goto_airval();}
  if (back.check()==LOW){goto_test1();}
  }
}

void goto_airval(){
  while(!(enter.check()==LOW)){
  MENU_SHOW(5,4,3,31,air_val); //"menu_current = MENU_ITEMS" means there is no need to draw box
  if (enter.check()==LOW){goto_test1();}
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {  
  Serial.begin(9600);  
  basicset();
  goto_main0();
}

void loop() { 
  
}
