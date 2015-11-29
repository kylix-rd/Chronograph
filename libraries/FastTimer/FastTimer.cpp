//
// Copyright (c) 2013 - Allen Bauer - http://blog.thereadoracleatdelphi.com
// Under MIT License
//

#include <stdio.h>
#include <avr/io.h>
#include "Arduino.h"
#include "FastTimer.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

volatile unsigned long timer4_overflow_count = 0;

#if defined(TCCR4A)
ISR(TIMER4_OVF_vect)
#elif defined(TCCR1A)
ISR(TIMER1_OVF_vect)
#endif
{
	++timer4_overflow_count;
}

/*
FastTimer::FastTimer()
{
#if defined(TCCR4A)
	TCCR4B &= ~_BV(CS40);
	TCCR4B |= _BV(CS41);
	TCCR4B &= ~_BV(CS42);
	TCCR4A &= ~_BV(WGM40);
	TIMSK4 |= _BV(TOIE4);
#elif defined(TCCR2A)
	TCCR2B &= ~_BV(CS20);
	TCCR2B |= _BV(CS21);
	TCCR2B &= ~_BV(CS22);
	TCCR2A &= ~_BV(WGM20);
	TIMSK2 |= _BV(TOIE2);
#endif
}
*/

void FastTimer::init()
{
#if defined(TCCR4A)
	TCCR4B &= ~_BV(CS40);
	TCCR4B |= _BV(CS41);
	TCCR4B &= ~_BV(CS42);
	TCCR4A &= ~_BV(WGM40);
	TIMSK4 |= _BV(TOIE4);
#elif defined(TCCR1A)
	TCCR1B &= ~_BV(CS10);
	TCCR1B |= _BV(CS11);
	TCCR1B &= ~_BV(CS12);
	TCCR1A &= ~_BV(WGM10);
	TIMSK1 |= _BV(TOIE1);
#endif
}

unsigned long FastTimer::micros()
{
	unsigned long m;
	uint16_t t;
	uint8_t oldSREG = SREG;
	
	cli();
	m = timer4_overflow_count;
#if defined(TCNT4)
	t = TCNT4;
#elif defined(TCNT1)
	t = TCNT1;
#else
	#error TIMER 1 or 4 not defined
#endif

  
#if defined(TIFR4)
	if ((TIFR4 & _BV(TOV4)) && (t < 65535))
		m++;
#elif defined(TIFR1)
	if ((TIFR1 & _BV(TOV1)) && (t < 65535))
		m++;
#endif

	SREG = oldSREG;
	
	return ((m << 16) + t) / 2;
}

Stopwatch::Stopwatch(FastTimer timer)
{
	_timer = timer;
	memset(_stopwatches, 0, sizeof(_stopwatches));
}

boolean Stopwatch::Start(Stopwatches stopwatch)
{
	if (!_stopwatches[stopwatch].running)
	{
		_stopwatches[stopwatch].startCount = _timer.micros();
		_stopwatches[stopwatch].running = true;
		return true;
	}
	else
		return false;
}

unsigned long Stopwatch::Stop(Stopwatches stopwatch)
{
	if (_stopwatches[stopwatch].running)
	{
		_stopwatches[stopwatch].stopCount = _timer.micros();
		_stopwatches[stopwatch].running = false;
	}
	return _stopwatches[stopwatch].stopCount - _stopwatches[stopwatch].startCount;
}

unsigned long Stopwatch::Elapsed(Stopwatches stopwatch)
{
	unsigned long stopCount;
	if (_stopwatches[stopwatch].running)
		stopCount = _timer.micros();
	else
		stopCount = _stopwatches[stopwatch].stopCount;
	return stopCount - _stopwatches[stopwatch].startCount;
}