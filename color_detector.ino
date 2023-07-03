#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Pick analog outputs, for the UNO these three work well
// use ~560  ohm resistor between Red & Blue, ~1K for green (its brighter)
#define redpin 3
#define greenpin 5
#define bluepin 6
// for a common anode LED, connect the common pin to +5V
// for common cathode, connect the common to ground

// set to false if using a common cathode LED
#define commonAnode true

// our RGB -> eye-recognized gamma color
byte gammatable[256];


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  Serial.begin(9600);
  //Serial.println("Color View Test!");

  if (tcs.begin()) {
    //Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }

  // use these three pins to drive an LED
#if defined(ARDUINO_ARCH_ESP32)
  ledcAttachPin(redpin, 1);
  ledcSetup(1, 12000, 8);
  ledcAttachPin(greenpin, 2);
  ledcSetup(2, 12000, 8);
  ledcAttachPin(bluepin, 3);
  ledcSetup(3, 12000, 8);
#else
  pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
#endif

  // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
  for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;

    if (commonAnode) {
      gammatable[i] = 255 - x;
    } else {
      gammatable[i] = x;
    }
    //Serial.println(gammatable[i]);
  }
}
int pinA = 13;
int pinB = 12;
int pinC = 11;
int pinD = 10;
int pinE = 9;
int pinF = 8;
int pinG = 7;

int t = 1000;

// The commented out code in loop is example of getRawData with clear value.
// Processing example colorview.pde can work with this kind of data too, but It requires manual conversion to 
// [0-255] RGB value. You can still uncomments parts of colorview.pde and play with clear value.
void loop() {
  
  float red, green, blue;
  
  tcs.setInterrupt(false);  // turn on LED

  delay(60);  // takes 50ms to read

  tcs.getRGB(&red, &green, &blue);
  
  tcs.setInterrupt(true);  // turn off LED

  Serial.print("R:\t"); Serial.print(int(red)); 
  Serial.print("\tG:\t"); Serial.print(int(green)); 
  Serial.print("\tB:\t"); Serial.print(int(blue));

  if(red>100 && green<70 && blue<70){
    Serial.print("\tColor:\t"); Serial.print("Red");
    display(2);
  }else if(green>100 && red<70 && blue<70){
    Serial.print("\tColor:\t"); Serial.print("Green");
    display(3);
  }  else if(blue>100 && red<70 && green<100){
    Serial.print("\tColor:\t"); Serial.print("Blue");
    display(4);
  }  else if(blue<100 && red<100 && green<100){
    Serial.print("\tColor:\t"); Serial.print("Black");
    display(5);
  }  else if(blue<100 && red<100 && green>100){
    Serial.print("\tColor:\t"); Serial.print("White");
    display(1);
  }else {
    Serial.print("\tColor:\t"); Serial.print("Not identified");
    display(0);
  }

//  Serial.print("\t");
//  Serial.print((int)red, HEX); Serial.print((int)green, HEX); Serial.print((int)blue, HEX);
  Serial.print("\n");

//  uint16_t red, green, blue, clear;
//  
//  tcs.setInterrupt(false);  // turn on LED
//
//  delay(60);  // takes 50ms to read
//
//  tcs.getRawData(&red, &green, &blue, &clear);
//  
//  tcs.setInterrupt(true);  // turn off LED
//
//  Serial.print("C:\t"); Serial.print(int(clear)); 
//  Serial.print("R:\t"); Serial.print(int(red)); 
//  Serial.print("\tG:\t"); Serial.print(int(green)); 
//  Serial.print("\tB:\t"); Serial.print(int(blue));
//  Serial.println();


#if defined(ARDUINO_ARCH_ESP32)
  ledcWrite(1, gammatable[(int)red]);
  ledcWrite(2, gammatable[(int)green]);
  ledcWrite(3, gammatable[(int)blue]);
#else
  analogWrite(redpin, gammatable[(int)red]);
  analogWrite(greenpin, gammatable[(int)green]);
  analogWrite(bluepin, gammatable[(int)blue]);
#endif
}

void display(int number){
  digitalWrite(pinA,0);
  digitalWrite(pinB,0);
  digitalWrite(pinC,0);
  digitalWrite(pinD,0);
  digitalWrite(pinE,0);
  digitalWrite(pinF,0);
  digitalWrite(pinG,0);
  if(number == 1) {
    digitalWrite(pinB, 1);
    digitalWrite(pinC, 1);
  }else if(number == 2) {
    digitalWrite(pinG, 1);
    digitalWrite(pinE, 1);
    digitalWrite(pinD, 1);
    digitalWrite(pinB, 1);
    digitalWrite(pinA, 1);
  }else if (number == 3) {
    digitalWrite(pinG, 1);
    digitalWrite(pinD, 1);
    digitalWrite(pinC, 1);
    digitalWrite(pinA, 1);
    digitalWrite(pinB, 1);
  }else if (number == 4) {
    digitalWrite(pinB, 1);
    digitalWrite(pinC, 1);
    digitalWrite(pinG, 1);
    digitalWrite(pinF, 1);
  }else if (number == 5) {
    digitalWrite(pinA, 1);
    digitalWrite(pinC, 1);
    digitalWrite(pinD, 1);
    digitalWrite(pinG, 1);
    digitalWrite(pinF, 1);
  }else if (number == 0) {
    digitalWrite(pinA, 1);
    digitalWrite(pinB, 1);
    digitalWrite(pinC, 1);
    digitalWrite(pinD, 1);
    digitalWrite(pinE, 1);
    digitalWrite(pinF, 1);
  }
}
