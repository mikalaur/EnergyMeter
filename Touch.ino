#include <TouchScreen.h>
#include "common.h"

static SCalValues tsCalValues = {200, 900, 900, 200};
static TouchScreen TS(XP, YP, XM, YM, 300);
static SCalPoints calPoints; 
static TSPoint tp;

void tsReadRawTouchPoint(void)
{
  tp = TS.getPoint();
  pinMode(YP, OUTPUT);      //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);   //because TFT control pins
  digitalWrite(XM, HIGH);
}
  
void tsReadTouchPoint(void)
{
  int16_t tempX;
  int16_t tempY;
  tsReadRawTouchPoint();
  // Map values to display size using calibration values
  tempX = tft.height() - map(tp.x, tsCalValues.bottom, tsCalValues.top, 0, tft.height());
  tempY = map(tp.y, tsCalValues.left, tsCalValues.right, 0, tft.width());
  // swap X and Y because screen is in landscape mode
  tp.x = tempY;
  tp.y = tempX;
}
  
bool tsIsPressed(void)
{
    // .kbv this was too sensitive !!
    // now touch has to be stable for 50ms
    int count = 0;
    bool state, oldstate;
    while (count < 5) {
        tsReadRawTouchPoint();
        state = tp.z > PRESSURE_MIN && tp.z < PRESSURE_MAX;
        if (state == oldstate) count++;
        else count = 0;
        oldstate = state;
        delay(5);
    }
    return oldstate;
}   
/*
void tsDrawCrossHair(int x, int y, int color)
{
  tft.drawRect(x - 20, y - 20, 40, 40, color);
  tft.drawLine(x - 10, y, x + 10, y, color);
  tft.drawLine(x, y - 10, x, y + 10, color);
}
  
void tsReadCoordinates()
{
    int failcount = 0;
    int cnt = 0;
    uint32_t tx = 0;
    uint32_t ty = 0;
    boolean OK = false;
    
    while (OK == false)
    {
        tft.setTextColor(WHITE, BLUE);
        tft.setCursor(100, 110);
        tft.print("*  PRESS  *");
        while (tsIsPressed() == false) {}
        tft.setCursor(100, 110);
        tft.print("*  HOLD!  *");
        cnt = 0;
        do
        {
            tsReadRawTouchPoint();
            tft.setCursor(110, 200);
            tft.print(" " + String(tp.x) + "," + String(tp.y) + " ");
            if (tp.z > PRESSURE_MIN && tp.z < PRESSURE_MAX)
            {
                tx += tp.x;
                ty += tp.y;
                cnt++;
            }
            else
                failcount++;
        } while ((cnt < ITERATIONS) && (failcount < 10000));
        if (cnt >= ITERATIONS)
        {
            OK = true;
        }
        else
        {
            tx = 0;
            ty = 0;
            cnt = 0;
        }
        if (failcount >= 10000) return;
    }

    // Switch X and Y because landscape orientation
    tp.x = ty / ITERATIONS;
    tp.y = tx / ITERATIONS;
    tp.z = 0;
}
  
bool tsCalibratePoint(int x, int y, SCalPoint *pCal)
{ 
  tp = {0,0,0};
  tsDrawCrossHair(x, y, GREEN);
  tsReadCoordinates();
  if ((tp.x == 0) && (tp.y == 0) && (tp.z == 0)) return false; // failed to read TS
  tsDrawCrossHair(x, y, GREY);
  pCal->x = tp.x;
  pCal->y = tp.y;
  tft.setCursor(100, 110);
  tft.print("* RELEASE *");  
  while (tsIsPressed() == true) {}
  return true;
}
  
bool tsCalibration()
{
  bool ok;
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE, RED);
  tft.setTextSize(2);
  tft.setCursor(0, 60);
  tft.print(" TOUCHSCREEN CALIBRATION  ");
  tft.setCursor(0, 160);
  tft.print("   TOUCH THE CROSSHAIRS   ");

  ok = tsCalibratePoint(20, 20, &calPoints.leftTop);
  if (!ok) return false;
  tsCalibratePoint(20, tft.height()-20, &calPoints.leftBottom);
  if (!ok) return false;
  tsCalibratePoint(tft.width()-20, 20, &calPoints.rightTop);
  if (!ok) return false;  
  tsCalibratePoint(tft.width()-20, tft.height()-20, &calPoints.rightBottom);
  if (!ok) return false;  

  tsCalValues.left = (calPoints.leftTop.x + calPoints.leftBottom.x) / 2;
  tsCalValues.right = (calPoints.rightTop.x + calPoints.rightBottom.x) / 2;
  tsCalValues.top = (calPoints.leftTop.y + calPoints.rightTop.y) / 2;
  tsCalValues.bottom = (calPoints.leftBottom.y + calPoints.rightBottom.y) / 2;
#ifdef DEBUG    
  String s = "TS cal values: " + String(tsCalValues.left) + " " + String(tsCalValues.right) + " " + String(tsCalValues.top) + " " + String(tsCalValues.bottom);
  Serial.println(s);
#endif  
  return true;
}
*/
void tsLoadCalValues() {
  EEPROM.get(ADDR_TS_CAL, tsCalValues);
#ifdef DEBUG    
    Serial.println("Loaded from eeprom:");
    Serial.println(tsCalValues.left);
    Serial.println(tsCalValues.right);
    Serial.println(tsCalValues.top);
    Serial.println(tsCalValues.bottom);
#endif  
}

void tsSaveCalValues() {
  EEPROM.put(ADDR_TS_CAL, tsCalValues);
}

bool tsTouchWasInArea(int x, int y, int w, int h) {
  if ( (tp.x > x) && (tp.x < (x+w)) &&
       (tp.y > y) && (tp.y < (y+h)) ) return true;
  return false;    
}

