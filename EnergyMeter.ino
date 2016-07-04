#include <MCUFRIEND_kbv_9325.h>      // TFT shield (chinese MCUFRIEND)
#include <DS3232RTC.h>    //http://github.com/JChristensen/DS3232RTC
#include <Time.h>         //http://www.arduino.cc/playground/Code/Time
#include <Wire.h>               // I2C library
#include "common.h"

// ----- Globals -----
MCUFRIEND_kbv tft(A3, A2, A1, A0, 10);
SPowerData powerSensor[4];    
float currentTemp = 0;
float currentHum = 0;

#define REFRESH_INTERVAL 1000

int currentScreen = -1;
long prevMillis = 0;

//***************************************************************************
// functions
//***************************************************************************
/*
bool tsIsPressed();
bool initPowerSensor_Load();
bool initPowerSensor_Solar();
bool initPowerSensor_Alt();
bool initPowerSensor_Mains();
bool initTempSensor();
void eeErase(void);
unsigned long eeCalcCRC(void);
unsigned long eeReadCRC(void);
unsigned long eeWriteCRC(unsigned long crc);
void tsLoadCalValues();
void tsSaveCalValues();
*/
//***************************************************************************
void printResult(bool result) {
  if (result) {
    tft.setTextColor(GREEN, BLACK); tft.println("OK");
  } else {
    tft.setTextColor(RED, BLACK); tft.println("FAIL");
    tft.setTextColor(GREEN, BLACK);
  }
}

//***************************************************************************
void systemFailureHalt() {
  tft.setTextSize(3);  
  tft.setCursor(12, 110);
  tft.setTextColor(RED);
  tft.print(" SYSTEM FAILURE ");
  while(1);
}

//***************************************************************************
void powerOnSelfTest(void) {  
  char str[64];
  char str2[8];
  char* pstr = str;
  bool touchCal = false;

  // Screen settings
  tft.setRotation(1); // Landscape (0 = portrait, 1 = landscape, 2 = portrait_rev, 3 = landscape_rev)
  tft.setTextSize(1);
  tft.setTextColor(GREEN, BLACK);
  tft.fillScreen(BLACK);
  tft.setCursor(0,0);
  tft.println(VER_STR);

  if (tsIsPressed()) touchCal = true;
  /*
  // Screen test  
  //tft.fillScreen(RED);
  //tft.fillScreen(GREEN);
  //tft.fillScreen(BLUE);    
  //tft.fillScreen(BLACK);
  //tft.setCursor(0,0);
  */

  /*
  // Print all font characters
  for (uint8_t y = 2; y < 32; y++) {    
    for (uint8_t x = 0; x < 8; x++) {
      uint8_t ch = (8*y) + x;
      pstr += sprintf(pstr, "%03d:%c ", ch, ch);       
    }
    *(pstr+1) = 0;
    tft.println(str);
    pstr = str;
  }
  while(1);
  */  
  
  //tft.drawRect(0, 0, tft.width(), tft.height(), GREEN);

  tft.println("\nInit power meters:");
  bool result;
  bool error = false;
  tft.print("- Load....");
  result = initPowerSensor_Load();
  printResult(result);
  error |= !result;
  tft.print("- Solar...");
  result = initPowerSensor_Solar();
  printResult(result);
  error |= !result;
  tft.print("- Alt.....");
  result = initPowerSensor_Alt();
  printResult(result);
  error |= !result;  
  tft.print("- Mains...");
  result = initPowerSensor_Mains();
  printResult(result);
  error |= !result;

  tft.print("Init temp sensor...");
  result = initTempSensor();
  printResult(result);
  error |= !result;

  if (error) systemFailureHalt(); // HALT

  //tft.print("Setting clock...");
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  setSyncInterval(10); 
  /*
  if (timeStatus() == timeSet) {
    //printResult(true);
  } else {
    //printResult(false);
  }
  */
  if (!tsIsPressed()) touchCal = false; // check if screen is still pressed...
    
  //tft.print("EEPROM CRC check...");
  unsigned long crc_calc = eeCalcCRC();
  unsigned long crc_read = eeReadCRC();
  if (crc_calc != crc_read) {    
    //printResult(false);
    //tft.print("Clearing EEPROM...");
    eeErase(); 
    touchCal = true;
  } else {  
    //printResult(true);
    tsLoadCalValues();  
  }
  /* calibration disabled!
  if (touchCal) {
    // To be fail-safe, calibrate touch screen if checksum fails
    if (tsCalibration()) {
      tsSaveCalValues();  
      crc_calc = eeCalcCRC();    
      eeWriteCRC(crc_calc);
    } // If calibration fails, default values are used      
  }
  */
  delay(3000);
}

//***************************************************************************
void setup(void) {
  //Serial.begin(9600);
  //Serial.println(VER_STR);

  // LCD reset (shield modified, reset connected to pin10)
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  delay(500);
  digitalWrite(10, HIGH);
  
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW); // Turn off LED
  
  //Se<rial.print("Init TFT...");
  uint16_t tftID = tft.readID();
  tft.begin(tftID);
  //Serial.println(tftID, HEX);
  //tft.reset();  // not needed when reset line is connected to arduino reset

  //Serial.print("Init I2C...");
  Wire.begin();
  Wire.setClock(100000);

  powerOnSelfTest();
  drawMainScreen();
}

void clockSetScreen() {
  bool done = false;
  tmElements_t tm;
  int h,m,d,mo,y;
  RTC.read(tm);
  h = tm.Hour;
  m = tm.Minute;
  d = tm.Day;
  mo = tm.Month;
  y = tm.Year+1970;
  
  tft.fillScreen(BLACK);
  tft.setTextSize(3);  
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(15, 70);
  tft.print("++ ++ ++ ++ ++++");
  tft.drawRect(10,65,45,30,GREEN);
  tft.drawRect(65,65,45,30,GREEN);
  tft.drawRect(115,65,45,30,GREEN);
  tft.drawRect(170,65,45,30,GREEN);
  tft.drawRect(225,65,80,30,GREEN);
  tft.setCursor(15, 140);
  tft.print("-- -- -- -- ----");
  tft.drawRect(10,135,45,30,YELLOW);
  tft.drawRect(65,135,45,30,YELLOW);
  tft.drawRect(115,135,45,30,YELLOW);
  tft.drawRect(170,135,45,30,YELLOW);
  tft.drawRect(225,135,80,30,YELLOW);
  tft.setCursor(280, 210);
  tft.print("OK");
  tft.drawRect(275,205,44,34,RED);
  do {
    tft.setCursor(15, 105);
    printTime(h,m);
    tft.print(" ");
    printDate(d,mo,y);

    if (tsIsPressed()) {
      tsReadTouchPoint();
      
      if (tsTouchWasInArea(10,  65,  45, 30)) h++;
      if (tsTouchWasInArea(60,  65,  45, 30)) m++;
      if (tsTouchWasInArea(115, 65,  45, 30)) d++;
      if (tsTouchWasInArea(165, 65,  45, 30)) mo++;
      if (tsTouchWasInArea(215, 65,  80, 30)) y++;
           
      if (tsTouchWasInArea(10,  135, 45, 30)) h--;
      if (tsTouchWasInArea(60,  135, 45, 30)) m--;
      if (tsTouchWasInArea(115, 135, 45, 30)) d--;
      if (tsTouchWasInArea(165, 135, 45, 30)) mo--;
      if (tsTouchWasInArea(215, 135, 80, 30)) y--; 
           
      if (tsTouchWasInArea(275, 205, 44, 34)) done = true; 
    }

    
    if (h > 23) h = 0;
    if (h < 0) h = 23;
    if (m > 59) m = 0;
    if (m < 0) m = 59;
    if (d > 31) d = 1;
    if (d < 1) d = 31;
    if (mo > 12) mo = 1;
    if (mo < 1) mo = 12;
    if (y < 2016) y = 2016; // 2016
    if (y > 2050) y = 2050; // 2050

    delay(100);
    
  } while(!done);
  
  tm.Hour = h;
  tm.Minute = m;
  tm.Second = 0;
  tm.Day = d;
  tm.Month = mo;
  tm.Year = y-1970;
  RTC.write(tm);
  now(); // synchronize clock from RTC
}

//***************************************************************************
void loop(void) {  
  if ((millis() - prevMillis) > REFRESH_INTERVAL) {
    refreshMainScreen();
    prevMillis = millis();
  }

  // Go to clock set screen if clock is touched
  if (tsIsPressed()) {
    tsReadTouchPoint();
    if (tsTouchWasInArea(250, 0, 70, 30)) {
      clockSetScreen(); 
      drawMainScreen();
      prevMillis = millis();
    }
  }
}


