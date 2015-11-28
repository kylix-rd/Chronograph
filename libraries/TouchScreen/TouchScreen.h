// Touch screen library with X Y and Z (pressure) readings as well
// as oversampling to avoid 'bouncing'
// (c) ladyada / adafruit
// Code under MIT License

#ifndef _ADAFRUIT_TOUCHSCREEN_H_
#define _ADAFRUIT_TOUCHSCREEN_H_

#include <stdint.h>

class Point {
 public:
  Point(void);
  Point(int16_t x, int16_t y, int16_t z);
  
  bool operator==(Point);
  bool operator!=(Point);

  int16_t x, y, z;
};

class Rect 
{
public:
	Rect(void): left(0), bottom(0), right(0), top(0) {};
	Rect(int16_t left, int16_t bottom, int16_t right, int16_t top) : left(left), bottom(bottom), right(right), top(top) {};

	bool ContainsPoint(Point);

	bool operator==(Rect);
	bool operator!=(Rect);

	int16_t getWidth();
	int16_t getHeight();

	int16_t left, bottom, right, top;
};

struct CalibrationData
{
	char signature[4];
	Rect bounds;
	void eepromLoad(int eeprom_address);
	void eepromStore(int eeprom_address);
};

class TouchScreen {
 public:
  TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym);
  TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym, uint16_t rx);

  bool isTouching(void);
  uint16_t pressure(void);
  int readTouchY();
  int readTouchX();
  Point getPoint();
  Point getPressAndRelease(bool waitForValid = false);
  int16_t pressureThreshhold;

private:
  uint8_t _yp, _ym, _xm, _xp;
  uint16_t _rxplate;
};

typedef struct 
{
	long x;
	long y;
} CAL_POINT;


typedef struct 
{
	long a;
	long b;
	long c;
	long d;
	long e;
	long f;
	long div;
} CAL_MATRIX;

#define CAL_POINT_X1 (20)
#define CAL_POINT_Y1 (20)
#define CAL_POINT1   {CAL_POINT_X1,CAL_POINT_Y1}

#define CAL_POINT_X2 (300)
#define CAL_POINT_Y2 (120)
#define CAL_POINT2   {CAL_POINT_X2,CAL_POINT_Y2}

#define CAL_POINT_X3 (160)
#define CAL_POINT_Y3 (220)
#define CAL_POINT3   {CAL_POINT_X3,CAL_POINT_Y3}

class TouchRegion {
public:
	struct Calibration
	{
		virtual void Clear() = 0;
		virtual uint16_t getWidth() = 0;
		virtual uint16_t getHeight() = 0;
		virtual void DrawTextPGM(uint16_t x, uint16_t y, PGM_P text) = 0;
		virtual void DrawCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color) = 0;
		virtual void FillCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color) = 0;
	};

	TouchRegion(TouchScreen &ts) : _ts(&ts), _orientation(0) {};
    void setOrientation(uint16_t o); //0 90 180 270
    bool setCalibration(CAL_POINT *lcd, CAL_POINT *tp);
    bool writeCalibration(uint16_t eeprom_addr);
    bool readCalibration(uint16_t eeprom_addr);
    bool doCalibration(Calibration &lcd, uint16_t eeprom_addr, bool check_eeprom);
	Point getPressAndRelease(bool waitForValid = false);
	Point rawPointToRegionPoint(Point &rawPt);
    CAL_MATRIX tp_matrix;       //calibrate matrix
private:
	TouchScreen *_ts;
    uint16_t _orientation;      //lcd_orientation
    void calibrate(void);
};

extern int16_t Scale(int16_t nNumber, int16_t nNumerator, int16_t nDenominator);

#endif
