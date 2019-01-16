//#include <Key.h>
//#include <Keypad.h>

#include "EEPROM2.h"
#include "Key.h"
#include "Keypad.h"
#include <Wire.h>
#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x27, 16, 2); 
const byte ROWS = 4;                
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {11, 10, 9, 8}; 
byte colPins[COLS] = {7, 6, 5, 4}; 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

int temp, tempC;
int pressure;
float pressurePa, pressNeed, n;
bool pressDig, flagHash = false;
char key;
unsigned long lcdPr;
bool flagWork = false;
bool flagTime;
bool needToWrite;
bool firstRun=false;
bool buzzTimSt;
unsigned long startWork,lig;
unsigned long wait, blinkTime;
unsigned long eeprTimer;
long t = 00,o = 5;
bool buzz=true, back_timer=false;
int tNeed=110, paNeed=0, i=0;
int minutesNeed, hoursNeed=00;
unsigned int temperatureAnalogAverage = 0, AnalogSampleTime;                                            // analog average на температуру
const byte numReadings = 20;
unsigned int AnalogSampleInterval=250, readings[numReadings];
unsigned long AnalogValueTotal = 0;
byte index = 0;   
unsigned int paAnalogAverage = 550, paAnalogSampleTime;                                            // analog average на давление
const byte paNumReadings = 20;
unsigned int paAnalogSampleInterval=25, paReadings[paNumReadings];
unsigned long paAnalogValueTotal = 0;
byte paIndex = 0;   
int hTemp, hTempC;
unsigned int hTemperatureAnalogAverage = 0, hAnalogSampleTime;                                            // analog average на х-температуру
unsigned int hAnalogSampleInterval=250, hReadings[numReadings];
unsigned long hAnalogValueTotal = 0;
byte hIndex = 0;  
bool hype=false;
unsigned long endWork=0;

byte page=0;
long antiHype;
