// Touch screen library with X Y and Z (pressure) readings as well
// as oversampling to avoid 'bouncing'
// (c) ladyada / adafruit
// Code under MIT License

#include <pins_arduino.h>
#include <wiring_private.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "TouchScreen.h"

// increase or decrease the touchscreen oversampling. This is a little different than you make think:
// 1 is no oversampling, whatever data we get is immediately returned
// 2 is double-sampling and we only return valid data if both points are the same
// 3+ uses insert sort to get the median value.
// We found 2 is precise yet not too slow so we suggest sticking with it!

#define NUMSAMPLES 2

Point::Point(void) {
  x = y = 0;
}

Point::Point(int16_t x0, int16_t y0, int16_t z0) {
  x = x0;
  y = y0;
  z = z0;
}

bool Point::operator==(Point p1) {
  return  ((p1.x == x) && (p1.y == y) && (p1.z == z));
}

bool Point::operator!=(Point p1) {
  return  ((p1.x != x) || (p1.y != y) || (p1.z != z));
}

bool Rect::ContainsPoint(Point point)
{
	return (point.x >= left && point.x < right && point.y >= bottom && point.y < top);
}

int16_t Rect::getWidth()
{
	return right - left;
}

int16_t Rect::getHeight()
{
	return top - bottom;
}

void CalibrationData::eepromLoad(int eeprom_address)
{
//	int i;
	eeprom_read_block((void*)this, (void *)eeprom_address, sizeof(*this));
//	uint8_t *thisData = (uint8_t *)this;
//	for	(i = 0; i < sizeof(*this); i++)
//		thisData[i] = EEPROM.read(eeprom_address + i);
}

void CalibrationData::eepromStore(int eeprom_address)
{
//	int i;
	eeprom_write_block((void*)this, (void *)eeprom_address, sizeof(*this));
//	uint8_t *thisData = (uint8_t *)this;
//	for	(i = 0; i < sizeof(*this); i++)
//		EEPROM.write(eeprom_address + i, thisData[i]);
}

#if (NUMSAMPLES > 2)
static void insert_sort(int array[], uint8_t size) {
  uint8_t j;
  int save;
  
  for (int i = 1; i < size; i++) {
    save = array[i];
    for (j = i; j >= 1 && save < array[j - 1]; j--)
      array[j] = array[j - 1];
    array[j] = save; 
  }
}
#endif

Point TouchScreen::getPoint(void) {
  int x, y, z;
  int samples[NUMSAMPLES];
  uint8_t i, valid;
  

  uint8_t xp_port = digitalPinToPort(_xp);
  uint8_t yp_port = digitalPinToPort(_yp);
  uint8_t xm_port = digitalPinToPort(_xm);
  uint8_t ym_port = digitalPinToPort(_ym);

  uint8_t xp_pin = digitalPinToBitMask(_xp);
  uint8_t yp_pin = digitalPinToBitMask(_yp);
  uint8_t xm_pin = digitalPinToBitMask(_xm);
  uint8_t ym_pin = digitalPinToBitMask(_ym);


  valid = 1;

  pinMode(_yp, INPUT);
  pinMode(_ym, INPUT);
  
  *portOutputRegister(yp_port) &= ~yp_pin;
  *portOutputRegister(ym_port) &= ~ym_pin;
  //digitalWrite(_yp, LOW);
  //digitalWrite(_ym, LOW);
   
  pinMode(_xp, OUTPUT);
  pinMode(_xm, OUTPUT);
  //digitalWrite(_xp, HIGH);
  //digitalWrite(_xm, LOW);
  *portOutputRegister(xp_port) |= xp_pin;
  *portOutputRegister(xm_port) &= ~xm_pin;
   
   for (i=0; i<NUMSAMPLES; i++) {
     samples[i] = analogRead(_yp);
   }
#if NUMSAMPLES > 2
   insert_sort(samples, NUMSAMPLES);
#endif
#if NUMSAMPLES == 2
   if (samples[0] != samples[1]) { valid = 0; }
#endif
   x = (1023-samples[NUMSAMPLES/2]);

   pinMode(_xp, INPUT);
   pinMode(_xm, INPUT);
   *portOutputRegister(xp_port) &= ~xp_pin;
   //digitalWrite(_xp, LOW);
   
   pinMode(_yp, OUTPUT);
   *portOutputRegister(yp_port) |= yp_pin;
   //digitalWrite(_yp, HIGH);
   pinMode(_ym, OUTPUT);
  
   for (i=0; i<NUMSAMPLES; i++) {
     samples[i] = analogRead(_xm);
   }

#if NUMSAMPLES > 2
   insert_sort(samples, NUMSAMPLES);
#endif
#if NUMSAMPLES == 2
   if (samples[0] != samples[1]) { valid = 0; }
#endif

   y = (1023-samples[NUMSAMPLES/2]);

   // Set X+ to ground
   pinMode(_xp, OUTPUT);
   *portOutputRegister(xp_port) &= ~xp_pin;
   //digitalWrite(_xp, LOW);
  
   // Set Y- to VCC
   *portOutputRegister(ym_port) |= ym_pin;
   //digitalWrite(_ym, HIGH); 
  
   // Hi-Z X- and Y+
   *portOutputRegister(yp_port) &= ~yp_pin;
   //digitalWrite(_yp, LOW);
   pinMode(_yp, INPUT);
  
   int z1 = analogRead(_xm); 
   int z2 = analogRead(_yp);

   if (_rxplate != 0) {
     // now read the x 
     float rtouch;
     rtouch = z2;
     rtouch /= z1;
     rtouch -= 1;
     rtouch *= x;
     rtouch *= _rxplate;
     rtouch /= 1024;
     
     z = rtouch;
   } else {
     z = (1023-(z2-z1));
   }

   if (! valid) {
     z = 0;
   }

   return Point(x, y, z);
}

TouchScreen::TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym) {
  _yp = yp;
  _xm = xm;
  _ym = ym;
  _xp = xp;
  _rxplate = 0;
  pressureThreshhold = 10;
}


TouchScreen::TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym,
			 uint16_t rxplate) {
  _yp = yp;
  _xm = xm;
  _ym = ym;
  _xp = xp;
  _rxplate = rxplate;

  pressureThreshhold = 10;
}

int TouchScreen::readTouchX(void) {
   pinMode(_yp, INPUT);
   pinMode(_ym, INPUT);
   digitalWrite(_yp, LOW);
   digitalWrite(_ym, LOW);
   
   pinMode(_xp, OUTPUT);
   digitalWrite(_xp, HIGH);
   pinMode(_xm, OUTPUT);
   digitalWrite(_xm, LOW);
   
   return (1023-analogRead(_yp));
}


int TouchScreen::readTouchY(void) {
   pinMode(_xp, INPUT);
   pinMode(_xm, INPUT);
   digitalWrite(_xp, LOW);
   digitalWrite(_xm, LOW);
   
   pinMode(_yp, OUTPUT);
   digitalWrite(_yp, HIGH);
   pinMode(_ym, OUTPUT);
   digitalWrite(_ym, LOW);
   
   return (1023-analogRead(_xm));
}


uint16_t TouchScreen::pressure(void) {
  // Set X+ to ground
  pinMode(_xp, OUTPUT);
  digitalWrite(_xp, LOW);
  
  // Set Y- to VCC
  pinMode(_ym, OUTPUT);
  digitalWrite(_ym, HIGH); 
  
  // Hi-Z X- and Y+
  digitalWrite(_xm, LOW);
  pinMode(_xm, INPUT);
  digitalWrite(_yp, LOW);
  pinMode(_yp, INPUT);
  
  int z1 = analogRead(_xm); 
  int z2 = analogRead(_yp);

  if (_rxplate != 0) {
    // now read the x 
    float rtouch;
    rtouch = z2;
    rtouch /= z1;
    rtouch -= 1;
    rtouch *= readTouchX();
    rtouch *= _rxplate;
    rtouch /= 1024;
    
    return rtouch;
  } else {
    return (1023-(z2-z1));
  }
}

bool TouchScreen::isTouching()
{
	return (pressure() >= pressureThreshhold);
}

Point TouchScreen::getPressAndRelease(bool waitForValid)
{
	Point result;
	uint8_t tries = 10;
	while (!isTouching()) {};
	do {
		result = getPoint();
	} while ((tries-- > 0 || waitForValid) && result.z < pressureThreshhold);
	while (isTouching()) {};
	return result;
}

int16_t Scale(int16_t nNumber, int16_t nNumerator, int16_t nDenominator)
{
	return ((int32_t)nNumber * nNumerator) / nDenominator;
}

void TouchRegion::setOrientation(uint16_t o)
{
  switch(o)
  {
    case   0: _orientation =   0; break;
    case  90: _orientation =  90; break;
    case 180: _orientation = 180; break;
    case 270: _orientation = 270; break;
  }
}

bool TouchRegion::setCalibration(CAL_POINT *lcd, CAL_POINT *tp)
{
  tp_matrix.div = ((tp[0].x - tp[2].x) * (tp[1].y - tp[2].y)) -
                  ((tp[1].x - tp[2].x) * (tp[0].y - tp[2].y));

  if(tp_matrix.div == 0)
  {
    return false;
  }

  tp_matrix.a = ((lcd[0].x - lcd[2].x) * (tp[1].y - tp[2].y)) -
                ((lcd[1].x - lcd[2].x) * (tp[0].y - tp[2].y));

  tp_matrix.b = ((tp[0].x - tp[2].x) * (lcd[1].x - lcd[2].x)) -
                ((lcd[0].x - lcd[2].x) * (tp[1].x - tp[2].x));

  tp_matrix.c = (tp[2].x * lcd[1].x - tp[1].x * lcd[2].x) * tp[0].y +
                (tp[0].x * lcd[2].x - tp[2].x * lcd[0].x) * tp[1].y +
                (tp[1].x * lcd[0].x - tp[0].x * lcd[1].x) * tp[2].y;

  tp_matrix.d = ((lcd[0].y - lcd[2].y) * (tp[1].y - tp[2].y)) -
                ((lcd[1].y - lcd[2].y) * (tp[0].y - tp[2].y));

  tp_matrix.e = ((tp[0].x - tp[2].x) * (lcd[1].y - lcd[2].y)) -
                ((lcd[0].y - lcd[2].y) * (tp[1].x - tp[2].x));

  tp_matrix.f = (tp[2].x * lcd[1].y - tp[1].x * lcd[2].y) * tp[0].y +
                (tp[0].x * lcd[2].y - tp[2].x * lcd[0].y) * tp[1].y +
                (tp[1].x * lcd[0].y - tp[0].x * lcd[1].y) * tp[2].y;

  return true;
}

bool TouchRegion::writeCalibration(uint16_t eeprom_addr)
{
  if(tp_matrix.div != 0)
  {
    eeprom_write_byte((uint8_t *)eeprom_addr++, 0x55);
    eeprom_write_block((void*)&tp_matrix, (void*)eeprom_addr, sizeof(CAL_MATRIX));
    return true;
  }
  return false;
}

bool TouchRegion::readCalibration(uint16_t eeprom_addr)
{
  uint8_t c;

  c = eeprom_read_byte((uint8_t*)eeprom_addr++);
  if(c == 0x55)
  {
    eeprom_read_block((void*)&tp_matrix, (void*)eeprom_addr, sizeof(CAL_MATRIX));
    return true;
  }

  return false;
}

bool TouchRegion::doCalibration(Calibration &lcd, uint16_t eeprom_addr, bool check_eeprom)
{
  uint8_t i;
  uint16_t w = lcd.getWidth();
  uint16_t h = lcd.getHeight();
  CAL_POINT lcd_points[3] = {CAL_POINT1, CAL_POINT2, CAL_POINT3}; //calibration point postions
  CAL_POINT tp_points[3], *p;

  for (i = 0; i < 3; i++)
  {
    lcd_points[i].x = Scale(lcd_points[i].x, w, 320);
	lcd_points[i].y = Scale(lcd_points[i].y, h, 240);
  }

  //calibration data in EEPROM?
  if(readCalibration(eeprom_addr) && check_eeprom)
  {
    return false;
  }

  //clear screen and wait for touch release
  lcd.Clear();
  lcd.DrawTextPGM((lcd.getWidth()/2)-50, (lcd.getHeight()/2)-10, PSTR("Calibration"));
  while(_ts->isTouching()){};

  //show calibration points
  for(i=0; i<3; )
  {
    //draw points
    lcd.DrawCircle(lcd_points[i].x, lcd_points[i].y, Scale(2, w, 320), 1);
    lcd.DrawCircle(lcd_points[i].x, lcd_points[i].y, Scale(5, w, 320), 1);
    lcd.DrawCircle(lcd_points[i].x, lcd_points[i].y, Scale(10, w, 320), 1);

    Point pt = _ts->getPressAndRelease();

    if(pt.z > _ts->pressureThreshhold)
    {
      lcd.FillCircle(lcd_points[i].x, lcd_points[i].y, 2, 1);
      tp_points[i].x = pt.x;
      tp_points[i].y = pt.y;
      i++;

      //wait and redraw screen
      delay(100);
      lcd.Clear();
      lcd.DrawTextPGM((lcd.getWidth()/2)-50, (lcd.getHeight()/2)-10, PSTR("Calibration"));
    }
  }

  //calulate calibration matrix
  setCalibration(lcd_points, tp_points);

  //save calibration matrix
  writeCalibration(eeprom_addr);

  //wait for touch release
  while(_ts->pressure() > _ts->pressureThreshhold){};

  return true;
}

Point TouchRegion::getPressAndRelease(bool waitForValid)
{
	Point rawPt = _ts->getPressAndRelease(waitForValid);
	return rawPointToRegionPoint(rawPt);
}

#define LCD_WIDTH (240)
#define LCD_HEIGHT (64)

Point TouchRegion::rawPointToRegionPoint(Point &rawPt)
{
	long x, y;
	Point lcd, result;
	lcd.z = rawPt.z;

	//calc x pos
	x = rawPt.x;
	y = rawPt.y;
	x = ((tp_matrix.a * x) + (tp_matrix.b * y) + tp_matrix.c) / tp_matrix.div;
	if(x < 0) 
	{ 
		x = 0; 
	}
	else if(x >= LCD_WIDTH) 
	{ 
		x = LCD_WIDTH-1; 
	}
	lcd.x = x;

	//calc y pos
	x = rawPt.x;
	y = rawPt.y;
	y = ((tp_matrix.d * x) + (tp_matrix.e * y) + tp_matrix.f) / tp_matrix.div;
	if(y < 0)
	{ 
		y = 0; 
	}
	else if(y >= LCD_HEIGHT)
	{ 
		y = LCD_HEIGHT-1; 
	}
	lcd.y = y;

	//set orientation
	switch(_orientation)
	{
		case   0: { result.y = lcd.y; result.x = lcd.x; break;}
		case  90: { result.y = LCD_WIDTH - lcd.x; result.x = lcd.y; break; }
		case 180: { result.y = LCD_HEIGHT - lcd.y; result.x = LCD_WIDTH - lcd.x; break; }
		case 270: { result.y = lcd.x; result.x = LCD_HEIGHT - lcd.y; break;}
	}

	return result;
}