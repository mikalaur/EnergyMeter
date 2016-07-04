#ifndef COMMON_H
#define COMMON_H

#define DEBUG 0

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
//#define GREEN   0x07E0
#define GREEN   0x0680
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0xC618
//#define ORANGE  0xFBE0
#define ORANGE  0xfE00
#define CLEAR   0xCAFE

// LCD wiring
#define LCD_CS A3               // Chip Select goes to Analog 3
#define LCD_CD A2               // Command/Data goes to Analog 2
#define LCD_WR A1               // LCD Write goes to Analog 1
#define LCD_RD A0               // LCD Read goes to Analog 0
//#define LCD_RESET A4          // Can alternately just connect to Arduino's reset pin

// Strings
#define SUFFIX_TEMP "\xf7\x63"  // °C
#define SUFFIX_HUM "%RH"          // %
#define VER_STR "EnergyMeter v1.0 " __DATE__ " " __TIME__

#define MAX_LINEOFFSET 6

#define FONTWIDTH 6

#define LIMIT_TEMP_LOW 0
#define LIMIT_TEMP_HIGH 30

#define LOW_CURRENT_TOLERANCE 0.13

// Touchscreen pins
#define YP A3   
#define YM 9    
#define XM A2   
#define XP 8    

// Touchscreen calibration limits
#define PRESSURE_MIN  20
#define PRESSURE_MAX  1000
#define ITERATIONS    20

// Touchscreen data types
typedef struct SCalPoint {
  uint16_t x,y;
} SCalPoint;

typedef struct SCalPoints {
  SCalPoint leftTop;
  SCalPoint leftBottom;
  SCalPoint rightTop;
  SCalPoint rightBottom;
} SCalPoints;

typedef struct SCalValues {
  uint16_t left;
  uint16_t right;
  uint16_t top;
  uint16_t bottom;
} SCalValues;

typedef struct SPowerData {
  float v;  
  float i;
  float p;
} SPowerData;

enum Sensor { Load, Solar, Alt, Mains };

#endif // COMMON_H
