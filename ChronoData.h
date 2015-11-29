// ChronoData.h
// Copyright (c) 2013 - Allen Bauer - http://blog.thereadoracleatdelphi.com
// Under MIT License

#ifndef _CHRONODATA_h
#define _CHRONODATA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
#include <Printable.h>

#define MAX_CHRONO_DATA 2048 

class ChronoData : public Printable
{
 public:
	struct ShotData
	{
		float Velocity[2];
		ShotData() {};
		ShotData(float velocity1, float velocity2);
		float AvgVelocity() const { return (Velocity[0] + Velocity[1]) / 2.0; };
	};

	void init(uint8_t *eeAddress);
	void ClearAllSequences();
	void ClearCurrentSequence();
	int8_t NewSequence(bool saveCurrent = true);
	int8_t SequencesCount();
	bool LoadSequence(uint8_t);
	bool SaveCurrentSequence();
	int8_t CurrentSequence();
	int8_t ShotsInSequence(int8_t seq = -1) const;
	float SequenceMax(int8_t seq = -1) const;
	float SequenceMin(int8_t seq = -1) const;
	float SequenceAverage(int8_t seq = -1) const;
	float SequenceSpread(int8_t seq = -1) const;
	float SequenceDeviation(int8_t seq = -1) const;
	ShotData SequenceShotData(int8_t shot, int8_t seq = -1) const;
	bool ReadOnly();
	int8_t AddShot(ShotData shotData);
	bool DeleteShot(int8_t shot);
	int8_t MaxShotsInSequence() { return sizeof(_sequenceShotData) / sizeof(ShotData); };
	size_t printSeqShotTo(Print& p) const;
	size_t printStatsTo(Print& p) const;
    virtual size_t printTo(Print& p) const;
private:
	uint8_t *_eeAddress;
	int8_t _savedSeqCount;
	int8_t _thisSeqNum;
	bool _readOnly;
	int8_t _shotCount;
	ShotData _sequenceShotData[32];
	typedef void (*ProcessData)(const ChronoData::ShotData &data, void *param);
	void ForEachShot(ProcessData processData, void *param, int8_t seq = -1) const;
	uint8_t *GetSeqAddress(uint8_t seq) const;
};

extern ChronoData chronoData;

#endif

