// 
// Copyright (c) 2013 - Allen Bauer - http://blog.thereadoracleatdelphi.com
// Under MIT License
//
#include <stdint.h>
#include "Events.h"
#include "FastTimer.h"

bool Events::EventData::Check()
{
	bool result = false;
	if (conditionRef != NULL)
		result = (volatile bool)*conditionRef;
	else if (conditionProc != NULL)
		conditionProc(result);
	return result;
}

Events::~Events()
{
	EventData *data = eventData;
	while (data != NULL)
	{
		EventData *next = data->next;
		delete data;
		data = next;
	}
}

uint8_t Events::AddEvent(ConditionRef conditionRef)
{
	EventData *data = new EventData();
	data->conditionRef = conditionRef;
	data->eventID = ++_eventID;
	data->next = eventData;
	eventData = data;
	return data->eventID;
}

uint8_t Events::AddEvent(ConditionProc conditionProc)
{
	EventData *data = new EventData();
	data->conditionProc = conditionProc;
	data->eventID = ++_eventID;
	data->next = eventData;
	eventData = data;
	return data->eventID;
}

void Events::RemoveEvent(uint8_t eventID)
{
	EventData *data;
	EventData *last = NULL;
	for (data = eventData; data != NULL; last = data, data = data->next)
		if (data->eventID == eventID)
		{
			if (last != NULL)
				last->next = data->next;
			if (eventData == data)
				eventData = data->next;
			delete data;
			break;
		}
}

void Events::SetEnabled(uint8_t eventID, bool enabled)
{
	EventData *data;
	for (data = eventData; data != NULL; data = data->next)
		if (data->eventID == eventID)
		{
			data->enabled = enabled;
			break;
		}
}

int Events::WaitForAnyEvent(unsigned long timeout)
{
	FastTimer tm;
	unsigned long startTime = tm.micros();
	while ((timeout == (unsigned long)0xFFFFFFFF) || (tm.micros() - startTime < timeout))
	{
		EventData *data;
		for (data = eventData; data != NULL; data = data->next)
			if (data->enabled && data->Check())
				return data->eventID;
	}
	return -1;
}
