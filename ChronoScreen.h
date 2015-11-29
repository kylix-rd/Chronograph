// ChronoScreen.h
// Copyright (c) 2013 - Allen Bauer - http://blog.thereadoracleatdelphi.com
// Under MIT License

#ifndef _CHRONOSCREEN_h
#define _CHRONOSCREEN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
#include <Printable.h>
#include <FastTimer.h>

#define SCREEN1IR 42
#define SCREEN2IR 44
#define SCREEN3IR 46
#define SCREEN1PULSE 4
#define SCREEN2PULSE 3
#define SCREEN3PULSE 2
#define SCREEN1PIN 19
#define SCREEN2PIN 20
#define SCREEN3PIN 21

class ChronoScreen : public Printable
{
private:
	bool timingPending;
public:
	ChronoScreen();
	void init();
	void SetScreenLEDState(bool state);
	bool StartTiming(void);
	bool GetScreenData(unsigned long &screen12Count, unsigned long &screen13Count);
	bool GetVelocityData(float &VelocStage1, float &VelocStage2);
	void CancelTiming(void);
    virtual size_t printTo(Print& p) const;
	volatile bool *timingFinished;
};

void CheckTimingStarted(volatile bool &condition);
void CheckTimingFinished(volatile bool &condition);

extern ChronoScreen chronoScreen;

#endif

