#include <Arduino.h>
//Set the pins to the correct ones for your development shield or breakout board.
//when using the BREAKOUT BOARD only and using these software spi lines to the LCD,
//there is no MISO pin and You can use any free pin to define the pins,for example
//pin usage as follow:
//             CS  CD  RST  MOSI  MISO  CLK  LED 
//Arduino Uno  A5  A3  A4   A2    NONE  A1   A3
//Arduino Mega A5  A3  A4   A2    NONE  A1   A3

//when using the BREAKOUT BOARD only and using these hardware spi lines to the LCD,
//there is no MISO pin 
//the MOSI pin and CLK pin is defined by the system and can't be modified.
//other pins can be defined by youself,for example
//pin usage as follow:
//             CS  CD  RST  MOSI  MISO  CLK  LED 
// мой Arduino Uno  10  9   8    11    NONE  13   A3
//Arduino Mega 10  9   8    51    NONE  52   A3

//Remember to set the pins to suit your display module!

#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_SPI.h> //Hardware-specific library

//the definiens of software spi mode as follow:
//if the IC model is known or the modules is unreadable,you can use this constructed function
//LCDWIKI_SPI mylcd(ST7735S,A5,A3,-1,A2,A4,A1,A3);//software spi,model,cs,cd,miso,mosi,reset,clk,led
//LCDWIKI_SPI mylcd(SSD1283A,A5,A3,-1,A2,A4,A1,A3);//software spi,model,cs,cd,miso,mosi,reset,clk,led

//if the IC model is not known and the modules is readable,you can use this constructed function
//LCDWIKI_SPI mylcd(160,128,A5,A3,-1,A2,A4,A1,A3);//software spi,model,cs,cd,miso,mosi,reset,clk
//LCDWIKI_SPI mylcd(130,130,A5,A3,-1,A2,A4,A1,A3);//software spi,model,cs,cd,miso,mosi,reset,clk

//the definiens of hardware spi mode as follow:
//if the IC model is known or the modules is unreadable,you can use this constructed function
//LCDWIKI_SPI mylcd(ST7735S,10,9,8,A3); //hardware spi,cs,cd,reset
LCDWIKI_SPI mylcd(SSD1283A,10,9,8,A3); //hardware spi,cs,cd,reset

//if the IC model is not known and the modules is readable,you can use this constructed function
//LCDWIKI_SPI mylcd(160,128,10,9,8,A3); //hardware spi,cs,cd,reset
//LCDWIKI_SPI mylcd(130,130,10,9,8,A3); //hardware spi,cs,cd,reset



#define CLK 6
#define DT 5
#define SW 4

#include "GyverEncoder.h"
Encoder enc1(CLK, DT, SW, TYPE1);

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

int color=RED;

String menu[10]={"COLOR","CONTRAST","POWER","TEXT","M5","EXIT"};
#define menun 6

#define vx 50
#define vy 50
#define sz 2

int selectvalue(int min,int max,int cur){
 mylcd.Fill_Screen(BLACK); 
 int t=cur;
 mylcd.Set_Text_Size(3);
 mylcd.Set_Text_colour(RED);  
 mylcd.Print_Number_Int(t, vx, vy, sz, 0,DEC);// x,y
 while (1){
  if (enc1.isRight()){
   t<max?t++:t=t;
   mylcd.Fill_Screen(BLACK);
   mylcd.Print_Number_Int(t, vx, vy, sz, 0,DEC);
   Serial.println(t);
   }        
if (enc1.isLeft()){
    t>min?t--:t=t;
    mylcd.Fill_Screen(BLACK);
    mylcd.Print_Number_Int(t, vx, vy, sz, 0,DEC);
   Serial.println(t);
  }
 }
 return t;
}

#define tsize 2
#define mh 17 //tsize*8+1 //2-16, 1-8

void showmenuitem(String s, uint8_t n,bool act){
 act?mylcd.Set_Text_Back_colour(0, 255, 0),mylcd.Set_Text_colour(RED):mylcd.Set_Text_Back_colour(0, 0, 0),mylcd.Set_Text_colour(WHITE);
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
    case 0: color=selectvalue(0,255,color);
     break;
    case 1:
     break;
    case 2:
     break;  
    case 3:
     break;
    case 4:
     break;
    case 5: exitcon=false; cr=-1;
     break;  
   }  
  } 
}
return cr;
}
//------------------------------
void Init_menu(void){
 int t= menu[0].length();
 for(int i=0;i<menun;i++){
  menu[i].length()>t?t=menu[i].length():t=t;
 }
 for (int i=0;i<menun;i++){
   while(menu[i].length()<t){
     menu[i]+=' ';
   }
 }
}

void setup() 
{
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