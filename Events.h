// Events.h

#ifndef _EVENTS_h
#define _EVENTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

typedef volatile bool * ConditionRef;
typedef void (*ConditionProc)(volatile bool &condition);

class Events
{
private:
	struct EventData
	{
		ConditionRef conditionRef;
		ConditionProc conditionProc;
		int eventID;
		bool enabled;
		EventData *next;
		EventData() : conditionRef(NULL), conditionProc(NULL), eventID(0), enabled(true), next(NULL) {}
		bool Check();
	};
	EventData *eventData;
	uint8_t _eventID;
public:
	Events() : eventData(NULL), _eventID(0) { }
	~Events();

	uint8_t AddEvent(ConditionRef conditionRef);
	uint8_t AddEvent(ConditionProc conditionProc);
	void RemoveEvent(uint8_t eventID);
	void SetEnabled(uint8_t eventID, bool enabled);

	int WaitForAnyEvent(unsigned long timeout);
};

#endif

