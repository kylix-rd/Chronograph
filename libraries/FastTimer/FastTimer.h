#ifndef FastTimer_h
#define FastTimer_h

#include "Arduino.h"

class FastTimer
{
public:
	FastTimer() {};
	static void init();
	unsigned long micros();
};

typedef enum Stopwatches {
	SW1,
	SW2,
	SW3,
	SW4,
	SW5,
	SW6,
	SW7,
	SW8
} Stopwatches;

class Stopwatch
{
public:
	Stopwatch(FastTimer timer);
	boolean Start(Stopwatches stopwatch);
	unsigned long Stop(Stopwatches stopwatch);
	unsigned long Elapsed(Stopwatches stopwatch);
private:
	typedef struct InternalStopwatch
	{
		unsigned long startCount;
		unsigned long stopCount;
		boolean running;
	};
	FastTimer _timer;
	InternalStopwatch _stopwatches[SW8 + 1];
};

#endif