#include <Arduino.h>
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_SPI.h> //Hardware-specific library
#include "colors.h"
#include "GyverEncoder.h"
#include <beep.h>


#define CLK 6
#define DT 5
#define SW 4


//------------------------
Encoder enc1( DT,CLK, SW, TYPE1);
LCDWIKI_SPI mylcd(SSD1283A,10,9,8,A3); //hardware spi,cs,cd,reset
//------------------------

volatile static uint8_t param1 = 250;
volatile static uint8_t param2 = 250;

String menu[10]={"COLOR","CONTRAST","POWER","TEXT","M5","EXIT"};
#define menun 6

String menuc[10]={"COLORc","CONTRASTc","POWERc","TEXTc","M5c","EXITc"};
#define menunc 6


#define tsize 2
#define mh 17 //tsize*8+1 //2-16, 1-8

void showmenuitem(String s, uint8_t n,bool act){
 act?mylcd.Set_Text_Back_colour(0, 255, 0),mylcd.Set_Text_colour(RED):
     mylcd.Set_Text_Back_colour(0, 0, 0),mylcd.Set_Text_colour(WHITE);
 mylcd.Set_Text_Size(tsize);  
 mylcd.Print_String(s, 2, mh*n+1);
}

void showmenuall(String menu[], uint8_t num){ 
 mylcd.Fill_Screen(BLACK); 
 for (int i=0;i<num;i++){
  showmenuitem(menu[i],i,0==i);
 } 
}

void showmenu(String menu[], uint8_t num,uint8_t cur,uint8_t crpr){ 
  showmenuitem(menu[crpr],crpr,false);
  showmenuitem(menu[cur],cur,true);
}

void Init_menu(void){
 uint8_t t= menu[0].length();
 for(int i=0;i<menun;i++){
  menu[i].length()>t?t=menu[i].length():t=t;
 }
 for (int i=0;i<menun;i++){
   while(menu[i].length()<t){
     menu[i]+=' ';
   }
 }
}


#define vx 50
#define vy 50
#define sz 2

int selectvalue(int min,int max,int cur){
 mylcd.Fill_Screen(BLACK); 
 uint8_t t=cur;
 mylcd.Set_Text_Size(3);
 mylcd.Set_Text_colour(RED);
 mylcd.Set_Text_Back_colour(0, 0, 0);  
 mylcd.Print_Number_Int(t, vx, vy, sz, 0,DEC);// x,y
 bool act=true;
 while (act){
  if (enc1.isLeft()){
   t<max?t++:t=t;
   mylcd.Fill_Screen(BLACK);
   mylcd.Print_Number_Int(t, vx, vy, sz, 0,DEC);
   Serial.println(t);
   }        
if (enc1.isRight()){
    t>min?t--:t=t;
    mylcd.Fill_Screen(BLACK);
    mylcd.Print_Number_Int(t, vx, vy, sz, 0,DEC);
   Serial.println(t);
  }
 act=!enc1.isPress(); 
 }
 return t;
}

//------------------------------
int getmenu(String menu[], uint8_t num){
int cr=0;int crpr=1;
showmenuall(menu,menun);

bool exitcon=true;
while(exitcon){

if (enc1.isRight()){
   cr<menun-1?cr++:cr=cr;
   Serial.println(menu[cr]);}        
if (enc1.isLeft()){
    cr>0?cr--:cr=cr;
   Serial.println(menu[cr]);
  }
if (crpr!=cr){  
  showmenu(menu,menun,cr,crpr);
  crpr=cr;
 }
if (enc1.isPress()) {
  Serial.println("Press");
   switch(cr){
    case 0: param1=selectvalue(1,255,param1);showmenuall(menu,menun);cr=0;
     break;
    case 1: param2=selectvalue(1,255,param2);showmenuall(menu,menun);cr=0;
     break; 
    case 2: longbeep();showmenuall(menu,menun);cr=0;
     break;  
    case 3:
     break;
    case 4:beep(250,250);delay(500);/*showmenuall(menu,menun);*/cr=0;
     break;
    case 5: exitcon=false; cr=-1;
     break;  
   }  
  } 
}

return cr;
}
//------------------------------

void setup() 
{
  initBeep(0,7);
  longbeep();
  mylcd.Init_LCD();
  mylcd.Set_Rotation(4);//3
  mylcd.Fill_Screen(BLACK);
  Init_menu();
  Serial.begin(9600);
  enc1.setTickMode(AUTO);
}

void loop() 
{

 int res=getmenu(menu,6);
 //int res=selectvalue(0,100,3);
}