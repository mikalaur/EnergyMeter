#include "common.h"

void drawIcon_mainBattery(int col) {
  tft.drawRect(120, 100, 80, 40, col);
  tft.fillRect(125, 96, 5, 5, col);
  tft.fillRect(190, 96, 5, 5, col);  
}

void drawIcon_solarPanel(int col) {
  tft.drawLine(130, 20, 145, 5, col);
  tft.drawLine(145, 5, 175, 5, col);
  tft.drawLine(175, 5, 190, 20, col);
  tft.drawLine(190, 20, 130, 20, col);
  tft.drawLine(160, 5, 160, 20, col);
  tft.drawLine(138, 12, 182, 12, col); 
  tft.drawLine(153, 5, 145, 20, col);
  tft.drawLine(167, 5, 175, 20, col);
  tft.drawRect(130, 20, 60, 3, col);
}

void drawIcon_lightBulb(int col) {
  // light bulb (load)
  tft.drawCircle(160, 215, 12, col); // bulb
  tft.drawLine(157, 229, 162, 229, col); // base
  tft.drawLine(157, 231, 162, 231, col);
  tft.drawLine(157, 233, 162, 233, col);
  tft.drawLine(158, 235, 161, 235, col);
  tft.drawLine(159, 227, 155, 219, col); // filament
  tft.drawLine(160, 227, 164, 219, col);
  tft.drawLine(156, 220, 163, 220, col);
}

void drawIcon_mainsCharger(int col) {
  tft.drawRoundRect(270, 110, 30, 20, 3, col);
  tft.drawLine(300, 120, 310, 120, col);
  tft.fillCircle(313, 120, 3, col);
  tft.fillRect(315, 117, 3, 10, BLACK);
  tft.drawLine(314, 117, 314, 123, col);
  tft.drawLine(314, 119, 317, 119, col);
  tft.drawLine(314, 121, 317, 121, col);
  //tft.setCursor(255, 112);
  //tft.setTextColor(GREY, BLACK);
  //tft.print("230");
  //tft.setCursor(257, 112);
  //tft.setCursor(257, 120);
  //tft.print("/12");
}

void drawIcon_alternator(int col) {
  tft.drawRect(5, 95, 5, 10, col);
  tft.drawLine(7, 95, 7, 104, col);
  tft.drawPixel(10, 98, col);
  tft.drawPixel(10, 101, col);
  tft.drawRoundRect(11, 90, 22, 20, 3, col);
  tft.drawRect(32, 94, 3, 12, col);
  tft.drawLine(14, 92, 18, 92, col);
  tft.drawLine(14, 94, 18, 94, col);
  tft.drawLine(14, 97, 18, 97, col);
  tft.drawLine(14, 102, 18, 102, col);
  tft.drawLine(14, 105, 18, 105, col);
  tft.drawLine(14, 107, 18, 107, col);  
}

void drawIcon_starterBattery(int col) {
  tft.drawRect(5, 130, 30, 20, col);
  tft.fillRect(9, 126, 5, 5, col);
  tft.fillRect(26, 126, 5, 5, col);
}

void printValue(float f, int x, int y, int precision, char unit, bool rightJustify, int textSize, int color, int bgcolor) {  
  int pad = 0;
  if (f > 9999) f = 9999;
  if (f < -9999) f = -9999;
  int r = round(f);
  if (rightJustify) {
    // justify to right
    if (f >= 1000) pad = 0;       // 1000.0
    else if (f >= 100) pad = 1;   // _100.0
    else if (f >= 10) pad = 2;    // __10.0
    else if (f > 0) pad = 3;      // ___0.1
    else if (f == 0) pad = 3;     // ___0.0
    else if (f < 0) pad = 2;      // __-0.1
    else if (f <= -10) pad = 1;   // _-10.0
    else if (f <= -100) pad = 0;  // -100.0
//    if ((r >= 10) && (r < 100))spaces = 3;
//    if (((r > -10) && (f < 0)) || (f >= 10)) spaces = 1;
//    else if ((f >= 0.0) && (f < 10.0)) spaces = 2;
  }
  tft.setTextSize(textSize);
  if (bgcolor == CLEAR) tft.setTextColor(color);
  else tft.setTextColor(color, bgcolor);
  tft.setCursor(x+(pad*(textSize*FONTWIDTH)), y);
  if (rightJustify) tft.print(" "); // empty space at start to clear previous text
  tft.print(f, precision);
  tft.print(unit);
  if (!rightJustify) tft.print(" ");
}

void fillCable(int x1, int y1, int w, int h, float f, int color)
{
  int cableColor = GREY;
  f = abs(f); // absolute value
  if (f > LOW_CURRENT_TOLERANCE) cableColor = color;
  tft.fillRect(x1, y1, w, h, cableColor);  // draw connection
}

void updateInfo_solar()
{
  int textColor = WHITE;
  fillCable(158, 23, 4, 77, powerSensor[Solar].i, GREEN);
  printValue(powerSensor[Solar].v, 95, 40, 2, 'V', true, 1, textColor, BLACK);
  printValue(powerSensor[Solar].i, 95, 48, 2, 'A', true, 1, textColor, BLACK);
  printValue(powerSensor[Solar].p, 170, 40, 0, 'W', false, 2, textColor, BLACK);
}

void fillBattery(float voltage)
{
  float minVoltage = 10.5;
  float maxVoltage = 13.0;
  int fillHeight;
  int h = 38;
  int w = 80;
  int color = GREEN;

  static float v;
  if (v == voltage) return; // value not changed
  v = voltage;
  
  if (v < 12) color = ORANGE;
  if (v < 11) color = RED;
  if (v > 14) color = ORANGE;
  if (v > 15) color = RED;

  if (v < minVoltage) v = minVoltage;
  if (v > maxVoltage) v = maxVoltage;
  fillHeight = ((v - minVoltage) / (maxVoltage - minVoltage)) * h;

  tft.fillRect(121, 101, 78, (h-fillHeight), BLACK);  
  tft.fillRect(121, (101+(h-fillHeight)), 78, fillHeight, color); 
  printValue(voltage, 125, 112, 2, 'V', false, 2, WHITE, CLEAR);  
}

void updateInfo_load()
{
  int textColor = WHITE;
  int cableColor = YELLOW;

  // Service battery voltage
  fillBattery(powerSensor[Load].v);
  
  fillCable(158, 140, 4, 63, powerSensor[Load].i, cableColor);    
  printValue(powerSensor[Load].i, 50, 180, 1, 'A', true, 2, textColor, BLACK);   
  printValue(powerSensor[Load].p, 170, 180, 0, 'W', false, 2, textColor, BLACK); 
}

void updateInfo_alt()
{
  int cableColor = GREEN;
  int textColor = WHITE;

  if (powerSensor[Alt].i < 0) cableColor = BLUE; // negative current; solar panel charging starter battery

  fillCable(18, 110, 4, 20, powerSensor[Alt].i, cableColor); // alternator to starter battery
  fillCable(22, 118, 98, 4, powerSensor[Alt].i, cableColor); // alternator to main battery
  printValue(powerSensor[Alt].v, 50, 100, 2, 'V', false, 1, textColor, BLACK);
  printValue(powerSensor[Alt].i, 50, 108, 1, 'A', false, 1, textColor, BLACK);
  printValue(powerSensor[Alt].p, 50, 126, 0, 'W', false, 2, textColor, BLACK);
}

void updateInfo_mains()
{
  tft.fillRect(200, 118, 70, 4, RED);
  int cableColor = GREEN;
  int textColor = WHITE;
  
  fillCable(200, 118, 70, 4, powerSensor[Mains].i, cableColor);
  //printValue(powerSensor[Mains].v, 210, 100, 2, 'V', true, 1, textColor, BLACK);  // Not needed, same voltage as Load
  printValue(powerSensor[Mains].i, 210, 108, 2, 'A', true, 1, textColor, BLACK);
  printValue(powerSensor[Mains].p, 190, 126, 0, 'W', true, 2, textColor, BLACK);
}

void updateInfo_temp()
{
  int textColor = WHITE;
  tft.setTextSize(2);
  tft.setCursor(5, 5);
  if (currentTemp < LIMIT_TEMP_LOW) textColor = CYAN;
  else if (currentTemp > LIMIT_TEMP_HIGH) textColor = ORANGE;
  tft.setTextColor(textColor, BLACK);
  tft.print(currentTemp, 1);  
  tft.println(SUFFIX_TEMP);

  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(1);
  tft.setCursor(5, 22);
  tft.print(currentHum, 0);  
  tft.println(SUFFIX_HUM);
}

void printTime(int h, int m) 
{
  if (h < 10) tft.print("0");
  tft.print(h);
  tft.print(":");
  if (m < 10) tft.print("0");
  tft.print(m);
}

void printDate(int d, int m, int y)
{
  if (d < 10) tft.print("0");
  tft.print(d);
  tft.print(".");
  if (m < 10) tft.print("0");
  tft.print(m);
  tft.print(".");
  tft.print(y); 
}

void updateInfo_time()
{
  int textColor = WHITE;
  tft.setTextSize(2);
  tft.setCursor(255, 5);
  tft.setTextColor(textColor, BLACK);
  printTime(hour(), minute());

  tft.setTextSize(1);
  tft.setCursor(255, 22);
  printDate(day(), month(), year());
}

void drawMainScreen(void) {
  int x, y, w, h;
  int col = WHITE;
  tft.fillScreen(BLACK);
  //tft.drawLine(159, 0, 159, 239, GREY);
  //tft.drawLine(0, 119, 319, 119, GREY);

  drawIcon_mainBattery(col);
  drawIcon_solarPanel(col);
  drawIcon_lightBulb(col);
  drawIcon_mainsCharger(col);
  drawIcon_alternator(col);
  drawIcon_starterBattery(col);
    
  // draw "cable" outlines
  tft.drawLine(17, 110, 17, 129, col); // starter
  tft.drawLine(22, 110, 22, 117, col);
  tft.drawLine(22, 122, 22, 130, col);
  tft.drawLine(23, 117, 119, 117, col);
  tft.drawLine(23, 122, 119, 122, col);

  tft.drawLine(157, 23, 157, 99, col); // solar
  tft.drawLine(162, 23, 162, 99, col);

  tft.drawLine(200, 117, 269, 117, col); // mains
  tft.drawLine(200, 122, 269, 122, col);

  tft.drawLine(157, 140, 157, 203, col); // bulb
  tft.drawLine(162, 140, 162, 203, col);
}

void refreshMainScreen() {
  readPowerSensors();
  readTempSensor();

  updateInfo_solar();
  updateInfo_load();
  updateInfo_alt();
  updateInfo_mains();  
  
  updateInfo_temp();  
  updateInfo_time();   
}

