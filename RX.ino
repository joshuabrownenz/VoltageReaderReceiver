// Libraries
#include <SPI.h>
#include "RF24.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <toneAC.h>
#include <EEPROM.h>

// Encoder Pins
#define outputA 2
#define outputB 4
#define switchPin 5

// LCD Data
LiquidCrystal_I2C LCD(0x27,20,4);
char LCDData[4][19] = {"                  ", "                 ", "                  ", "                  "};
char prevLCDData[4][19] = {"                  ", "                  ", "                  ", "                  "};

// Wireless Data
RF24 radio(7, 8);
byte address[3][6] = {"0NODE", "1NODE", "2NODE"};
bool activePipes[3] = {false, false, false};
bool timeout[3] = {false, false, false};
bool silentPipes[3] = {false, true, false};
unsigned long receiveTimes[3];
unsigned long resetTime;
byte received;

// Received data
float data[3] = {0,0,0};

// Encoder Data
volatile int deltaEncoder;
volatile bool dataChanged;

// Buzzer Data
unsigned long alarmTime;
unsigned long alarmWaitTime;
bool alarm = false;
short alarmCount;

// Paramaters
const float minVoltage = 3.5f, warningVoltage = 3.5f, maxVoltage = 4.2f;
int clearHoldTime = 5000;
int silentHoldTime = 2000;
int numOfCells = 3;

// UI
int menu = 0;
int menuItem = 0;
byte addressLetters[3] = {48,49,50};
bool menuOneCharacterOn = true;
int editingARddress;

//Symbols
byte Bell[8]
{
  B00000,
  B00100,
  B01110,
  B01110,
  B01110,
  B11111,
  B00100,
  B00000
};
byte silentBell[8]
{
  B00000,
  B00101,
  B01111,
  B01110,
  B01110,
  B11111,
  B10100,
  B00000
};


void setup() {
    //Init LCD screen
    LCD.init();
    LCD.backlight();
    LCD.home();
    LCD.createChar(0, Bell);
    LCD.createChar(1, silentBell);
    rewriteLCD();

    //Init wireless module 
    radio.begin();
    radio.openReadingPipe(1, address[0]);
    radio.openReadingPipe(2, address[1]);
    radio.openReadingPipe(3, address[2]);
    radio.setPALevel(RF24_PA_MAX);
    radio.maskIRQ(1,1,0);
    radio.setDataRate(RF24_250KBPS);
    attachInterrupt(digitalPinToInterrupt(outputA), isr, LOW);

    // Get saved letters from storage
    addressLetters[0] = EEPROM.read(0);
    addressLetters[1] = EEPROM.read(1);
    addressLetters[2] = EEPROM.read(2);
}

void loop() {
  //Refresh wireless module every 5 seconds
  if(resetTime + 5000 <millis())
  {
    radio.stopListening();
    radio.startListening(); 
    resetTime = millis();
  }

  // Get data from wireless module and store in the correct index in array
  // 'received' is the number of the pipe with data
  if (radio.available(&received) > 0) {
    // Get data
    short newData;
    radio.read(&newData, sizeof(short));
    
    // Convert the value from 0-1023 to voltage
    float floatData = (newData/204.6f*1330)/330 - 0.15f;
    data[received - 1] = floatData;

    // Time of receive (for buzzer)
    receiveTimes[received - 1] = millis();

    // If it has timed out or has not connected before refresh the display
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
  // Controls letter selection of addresses
  menuOne();
  // Gets player input
  checkEncoder();
  // Checks if any batteries have dropped too low
  checkMinVoltages();
  // Check if any modules have lost connection
  checkTimeout();
  // Check and then display any changes to the LCD screen
  displayLCD();
  // Alarm controller
  alarm();
}
