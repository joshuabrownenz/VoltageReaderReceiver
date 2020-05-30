#include <SPI.h>
#include "RF24.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <toneAC.h>

#define outputA 2
#define outputB 4
#define switchPin 5

LiquidCrystal_I2C LCD(0x27,20,4);
byte address[3][6] = {"0NODE", "1NODE", "2NODE"};
bool activePipes[3] = {false, false, false};
bool timeout[3] = {false, false, false};
unsigned long receiveTimes[3];

float data[3] = {0,0,0};
RF24 radio(7, 8);

unsigned long lastDot;
int index = 0;

unsigned long alarmTime;
unsigned long alarmWaitTime;
bool alarm = false;
short alarmCount;

int numOfCells = 3;
float minVoltage = 3.5f, warningVoltage = 3.5f, maxVoltage = 4.2f;

unsigned long resetTime;

char LCDData[4][20] = {"                   ", "                   ", "                   ", "                   "};
char prevLCDData[4][20] = {"                   ", "                   ", "                   ", "                   "};

volatile int deltaEncoder;
volatile bool dataChanged;

bool beenPressed;

unsigned long pressTime = 0;
int holdTime = 1000;
int menu = 0;
int menuItem = 0;

bool menuOneCharacterOn = true;
char addressLetter;


void setup() {
  // put your setup code here, to run once:
    LCD.init();
    LCD.backlight();
    LCD.home();
    rewriteLCD();
    
    radio.begin();
    radio.openReadingPipe(1, address[0]);
    radio.openReadingPipe(2, address[1]);
    radio.openReadingPipe(3, address[2]);
    radio.setPALevel(RF24_PA_MAX);
    radio.maskIRQ(1,1,0);
    radio.setDataRate(RF24_250KBPS);
    
    attachInterrupt(digitalPinToInterrupt(outputA), isr, LOW);
}

void loop() {
  
  if(resetTime + 5000 <millis())
  {
    radio.stopListening();
    radio.startListening(); 
    resetTime = millis();
  }
  byte received;
  if (radio.available(&received) > 0) {
    short newData;
    radio.read( &newData, sizeof(short));
    float floatData = (newData/204.6f*1330)/330 - 0.15f;
    if(floatData > 2*numOfCells)
    {
      data[received - 1] = floatData;
      receiveTimes[received - 1] = millis();
      if(!activePipes[received - 1])
      {
        activePipes[received - 1] = true;
        rewriteLCD();
      }
      if(timeout[received - 1])
        {
          timeout[received - 1] = false;
          rewriteLCD();
        }
    }
  }
  menuOne();
  checkEncoder();
  checkMinVoltages();
  checkTimeout();
  displayLCD();
  if(alarm && alarmWaitTime + 30000 < millis())
  {
    if(alarmTime + 300 < millis())
    {
      toneAC(400, 10, 150, true);
      alarmCount++;
    }
    if(alarmCount>10)
    {
      alarmWaitTime = millis();
      alarm = false;
      alarmCount = 0;
    }
  }
//  LCD.setCursor(0,3);
//  LCD.print(((String)menu + ", " +(String)menuItem));
}
