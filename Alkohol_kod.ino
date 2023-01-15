#include "DFRobot_GDL.h"

#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4

int buttonPin = A0;
int sensorPin = A1;
int ButtonVolt = 7;
int potent = 8; 
int potentRead = A2;
int pipSpeaker = 9;
int sensorVolt = 10;


DFRobot_ST7789_240x240_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST); //Enligt github

float Map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float BRAC(int value){
  return ((value-300UL)*(700UL)/(670UL)); 
}

float BAC(float promille){
  return promille * 20; 
}

float BACapprox(int vikt){
  if (vikt <= 65){
    return 0.011;
  }
  else if (vikt <= 80){
   return 0.018;
  }
  else{
    return 0.021;
  }
}

void LCDscreen(){
  screen.setTextSize(2);
  //Set screen color 
  screen.fillScreen(COLOR_RGB565_BLACK);
  screen.setFont(&FreeMono12pt7b);//Set font to FreeMono12pt7b
  screen.setCursor(/*x=*/10,/*y=*/120);
  //Set text color 
  screen.setTextColor(COLOR_RGB565_LGRAY);
  //Set to text auto-wrap mode, true=Auto-wrap, false=No auto-wrap
  screen.setTextWrap(true);
}

void setup() {
  Serial.begin(9600);
 
  screen.begin();
  pinMode(buttonPin, INPUT);
  pinMode(ButtonVolt, OUTPUT);
  pinMode(potent, OUTPUT);
  pinMode(potentRead, INPUT);
  pinMode(pipSpeaker, OUTPUT);
  pinMode(sensorVolt, OUTPUT);

  for(int timer = 0; timer < 240; timer++){
    LCDscreen();
    screen.print("Stand by...");
  
  }
}

void loop(){
  int volt = analogRead(buttonPin);
  int val;
  float promille;
  int vikt = 0;
  digitalWrite(sensorVolt, 1023);
  digitalWrite(ButtonVolt, 682);
  digitalWrite(potent, HIGH);
  
  if(volt > 800){
    for(int a = 0; a < 4; a++){
    LCDscreen();
    analogWrite(pipSpeaker, 613);
    
    val = analogRead(sensorPin);
    screen.print(val);

    delay(1000);
    }
    for(int show = 0; show < 1; show++){
      analogWrite(pipSpeaker, 0);
      if(val <= 300){
      LCDscreen();
      screen.print("value:  ");
      screen.print(0.00);
      delay(10000);
      return;
      }
    
    //Visar alkohol mängden i andningen i promille
    promille = (BRAC(val)/1000);
    LCDscreen();
    screen.print("BRAC:   ");
    screen.print(promille);
    delay(10000);

    //Visar alkohol mängden i blodet i procent 
    float _BAC_ = BAC(promille) - BACapprox(vikt);
    LCDscreen();
    screen.print("BAC:    ");
    screen.print(_BAC_, 3);
    screen.print("%");
    delay(10000);
    
  }

  }
  else{
    int potentVal = analogRead(potentRead);
    
    vikt = Map(potentVal, 0, 926, 50, 120);

    LCDscreen();
    screen.print("Vikt:");
    screen.println(vikt);
    screen.print("KG");
    delay(500);
  }
  
 }