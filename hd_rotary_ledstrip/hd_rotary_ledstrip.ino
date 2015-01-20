/*
HD rotary with ledStrip

HD code from Oscurato: https://code.google.com/p/oscurato
*/

// led strip defines
#include "FastLED.h"
#define NUM_LEDS 16
#define DATA_PIN 4
CRGB leds[NUM_LEDS];

//HD variables
int h0 = A0;
int h1 = A1;
int h2 = A2;

int actual = -1;
int encActual = -1;
int lastActual = 0;
int mapped = 0;


void setup(){
  Serial.begin(115200);
   FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop(){
  hd();
  int ledIndex = mapped;
  int hue = map(mapped, 0, 15, 0, 255);
    for(int i = 0;i<NUM_LEDS;i++){
      if(i < ledIndex){
         leds[i] = CRGB::Black;
      } else if(i == ledIndex){
        //set red
        leds[i] = CRGB::Red;
       
        //setGlow(i);
      } else {
        leds[i] = CRGB::Black;
      }
    }
    FastLED.show();
}


void hd() {
  if (interpolate()){
    if(actual != lastActual){
      //Serial.println(actual);
      Serial.println(mapped);
    }
    lastActual = actual;
    
    
  }
}

int readEncoder() {
  int p1 = analogRead(h0);
  int p2 = analogRead(h1);
  int p3 = analogRead(h2);

  if (p1+p2+p3==0)
    return -1;

  int l = p1+p2+p3;    
  int x = (p2-p3)*86;
  int y = p1*100 - (p2+p3)*50;
  int p=-1;
  if (y>0) {
    if (abs(x) < y*57/100) 
      p=0;
    else 
      p=(x<0) ? 5 : 1;
  }
  else {
    if (abs(x) < -y*57/100) 
      p=3;
    else 
      p=(x<0) ? 4 : 2;
  }
  return p;
}

boolean interpolate() {
  int delta[] = {
    -1, -2, 0, +2, +1, 0, -1, -2, 0, +2, +1
  };
  int v = readEncoder();
  if (v==-1)
    return false;
  if (actual==-1) {
    actual = 0;
    encActual = v;
    return true;
  }

  int d = delta[v-encActual+5];
  actual += d;
  mapped += d;
  if(mapped > 15){
     mapped = 0; 
  } else if (mapped < 0) {
    mapped = 15; 
  }
  encActual = v;
  return true;
}



