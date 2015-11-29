// 
// Copyright (c) 2013 - Allen Bauer - http://blog.thereadoracleatdelphi.com
// Under MIT License
// 

#include "ChronoScreen.h"

volatile unsigned long _screen12Delta = 0;
volatile unsigned long _screen13Delta = 0;
volatile bool _timingStarted = false;
volatile bool _timingFinished = false;
volatile bool _screen2Detected = false;

FastTimer fastTimer;
Stopwatch stopWatch(fastTimer);

void Screen2Interrupt();
void Screen3Interrupt();

void Screen1Interrupt()
{
  EIMSK &= ~(1 << INT2);
  EIFR = (1 << INTF2) | (1 << INTF1);
  stopWatch.Start(SW1);
  _timingStarted = true;
  attachInterrupt(SCREEN2PULSE, Screen2Interrupt, RISING);
}

void Screen2Interrupt()
{
  EIMSK &= ~(1 << INT1);
  EIFR = (1 << INTF1) | (1 << INTF0);
  _screen12Delta = stopWatch.Elapsed(SW1);
  _screen2Detected = true;
  attachInterrupt(SCREEN3PULSE, Screen3Interrupt, RISING);
}

void Screen3Interrupt()
{
  EIMSK &= ~(1 << INT0);
  EIFR = 1 << INTF0;
  _screen13Delta = stopWatch.Stop(SW1);
  _timingFinished = true;
}

ChronoScreen::ChronoScreen() : timingFinished(&_timingFinished), timingPending(false) {}

void ChronoScreen::init()
{
  stopWatch = Stopwatch(fastTimer);
  pinMode(SCREEN1IR, OUTPUT);
  pinMode(SCREEN2IR, OUTPUT);
  pinMode(SCREEN3IR, OUTPUT);
  pinMode(SCREEN1PIN, INPUT);
  pinMode(SCREEN2PIN, INPUT);
  pinMode(SCREEN3PIN, INPUT);
//  SetScreenLEDState(true);
}

void ChronoScreen::SetScreenLEDState(bool state)
{
  digitalWrite(SCREEN1IR, (int)state);
  digitalWrite(SCREEN2IR, (int)state);
  digitalWrite(SCREEN3IR, (int)state);
}

void ChronoScreen::CancelTiming(void)
{
	detachInterrupt(SCREEN1PULSE);
	detachInterrupt(SCREEN2PULSE);
	detachInterrupt(SCREEN3PULSE);
	EIFR = (1 << INTF0) | (1 << INTF1) | (1 << INTF2);
	stopWatch.Stop(SW1);
	timingPending = false;
}

bool ChronoScreen::StartTiming(void)
{
	if (!timingPending)
	{
		_timingStarted = false;
		_timingFinished = false;
		_screen2Detected = false;
		EIFR = (1 << INTF0) | (1 << INTF1) | (1 << INTF2);
		attachInterrupt(SCREEN1PULSE, Screen1Interrupt, RISING);
		timingPending = true;
		return true;
	}
	return false;
}

bool ChronoScreen::GetScreenData(unsigned long &screen12Count, unsigned long &screen13Count)
{
	if (timingPending && _timingFinished)
	{
		screen12Count = _screen12Delta;
		screen13Count = _screen13Delta;
		EIFR = (1 << INTF0) | (1 << INTF1) | (1 << INTF2);
		timingPending = false;
		return true;
	}
	return false;
}

bool ChronoScreen::GetVelocityData(float &VelocStage1, float &VelocStage2)
{
  boolean result = false;
  unsigned long screen12Count;
  unsigned long screen13Count;
  unsigned long screen23Count;
  if (GetScreenData(screen12Count, screen13Count))
  {
    screen23Count = screen13Count - screen12Count;
    VelocStage1 = 1000000.0 / screen12Count;
    VelocStage2 = 1000000.0 / screen23Count;
	return true;
  }
  return false;
}

size_t ChronoScreen::printTo(Print& p) const
{
	p.print("Pending: ");
	p.println(timingPending ? "true" : "false");
	p.print("Started: ");
	p.println(_timingStarted ? "true" : "false");
	p.print("Finished: ");
	p.println(*timingFinished ? "true" : "false");
}

void CheckTimingStarted(volatile bool &condition)
{
	condition = _timingStarted;
}

void CheckTimingFinished(volatile bool &condition)
{
	condition = _timingFinished;
}

ChronoScreen chronoScreen;

