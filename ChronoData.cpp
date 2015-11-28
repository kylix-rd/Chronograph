// 
// 
// 

#include <math.h>
#include <avr/eeprom.h>
#include <Print.h>
#include "ChronoData.h"

ChronoData::ShotData::ShotData(float velocity1, float velocity2)
{
	Velocity[0] = velocity1; 
	Velocity[1] = velocity2; 
}

void ChronoData::init(uint8_t *eeAddress)
{
	_eeAddress = eeAddress;
	if (eeprom_read_byte(_eeAddress) != 'c' || eeprom_read_byte(_eeAddress + 1) != 'r')
	{
		_savedSeqCount = 0;
		eeprom_write_byte(_eeAddress, 'c');
		eeprom_write_byte(_eeAddress + 1, 'r');
		eeprom_write_byte(_eeAddress + 2, _savedSeqCount);
	} 
	else
		_savedSeqCount = eeprom_read_byte(_eeAddress + 2);
	_thisSeqNum = _savedSeqCount;
	_readOnly = false;
}

void ChronoData::ClearAllSequences()
{
	ClearCurrentSequence();
	_savedSeqCount = 0;
	_thisSeqNum = 0;
	_readOnly = false;
	eeprom_write_byte(_eeAddress + 2, _savedSeqCount);
}

void ChronoData::ClearCurrentSequence()
{
	_shotCount = 0;
}

int8_t ChronoData::NewSequence(bool saveCurrent)
{
	if ((_shotCount > 0) && !_readOnly && saveCurrent)
		SaveCurrentSequence();
	_thisSeqNum = _savedSeqCount;
	_readOnly = false;
	_shotCount = 0;
	return _thisSeqNum;
}

int8_t ChronoData::SequencesCount()
{
	return _savedSeqCount;
}

bool ChronoData::LoadSequence(uint8_t seq)
{
	uint8_t *eeAddress = GetSeqAddress(seq);
	_shotCount = eeprom_read_byte(eeAddress++);
	eeprom_read_block((void*)&_sequenceShotData[0], (void *)eeAddress, _shotCount * sizeof(ShotData));
	_thisSeqNum = seq;
	_readOnly = true;
	return true;
}

bool ChronoData::SaveCurrentSequence()
{
	if (!_readOnly && (_shotCount > 0))
	{
		uint8_t *eeAddress = GetSeqAddress(_thisSeqNum);
		eeprom_write_byte(_eeAddress + 2, ++_savedSeqCount);
		eeprom_write_byte(eeAddress++, _shotCount);
		eeprom_write_block((void*)&_sequenceShotData[0], (void *)eeAddress, _shotCount * sizeof(ShotData));
		_readOnly = true;
		return true;
	} else
		return false;
}

int8_t ChronoData::CurrentSequence()
{
	return _thisSeqNum;
}

int8_t	ChronoData::ShotsInSequence(int8_t seq)	const
{
	if (seq < 0)
		return _shotCount;
	uint8_t *eeAddress = GetSeqAddress(seq);
	return eeprom_read_byte(eeAddress);
}

struct MinMax
{
	float _min, _max;
	MinMax() : _min(3.4e38), _max(0.0) {};
};

void getSequenceMinMax(const ChronoData::ShotData &data, void *param)
{
	MinMax *minMax = (MinMax *)param;
	float cur = data.Velocity[0];
	minMax->_max = fmax(cur, minMax->_max);
	minMax->_min = fmin(cur, minMax->_min);
}

float ChronoData::SequenceMax(int8_t seq) const
{
	MinMax minMax;
	ForEachShot(getSequenceMinMax, &minMax, seq);
	return minMax._max;
}

float ChronoData::SequenceMin(int8_t seq) const
{
	MinMax minMax;
	ForEachShot(getSequenceMinMax, &minMax, seq);
	return minMax._min;
}

struct AvgData
{
	float Avg;
	int8_t items;
	AvgData() : Avg(0.0), items(0) {};
};

void getSequenceAvg(const ChronoData::ShotData &data, void *param)
{
	AvgData *avg = (AvgData *)param;
	float cur = data.Velocity[0];
	avg->items++;
	avg->Avg += cur;
}

float ChronoData::SequenceAverage(int8_t seq) const
{
	AvgData avg;
	ForEachShot(getSequenceAvg, &avg, seq);
	return avg.Avg / avg.items;
}

float ChronoData::SequenceSpread(int8_t seq) const
{
	MinMax minMax;
	ForEachShot(getSequenceMinMax, &minMax, seq);
	return minMax._max - minMax._min;
}

struct DeviationData
{
	float sumSq;
	float sum;
	DeviationData() : sumSq(0.0), sum(0.0) {};
};

void getDeviationData(const ChronoData::ShotData &data, void *param)
{
	DeviationData *deviationData = (DeviationData *)param;
	float cur = data.Velocity[0];
	deviationData->sumSq += cur * cur;
	deviationData->sum += cur;
}

float ChronoData::SequenceDeviation(int8_t seq) const
{
	DeviationData deviation;
	float sd;
	uint8_t shotCount = ShotsInSequence(seq);
	if (shotCount < 2)
		return 0.0;
	ForEachShot(getDeviationData, &deviation, seq);
	sd = sqrt((deviation.sumSq - (square(deviation.sum) / shotCount)) / (shotCount - 1));
	return sd;
}

ChronoData::ShotData ChronoData::SequenceShotData(int8_t shot, int8_t seq) const
{
	if (seq < 0)
		return _sequenceShotData[shot];
	else
	{
		ShotData data;
		uint8_t *eeAddress = GetSeqAddress(seq);
		uint8_t count = eeprom_read_byte(eeAddress++);
		shot = min(shot, count - 1);
		eeprom_read_block((void*)&data, (void*)(eeAddress + shot * sizeof(ShotData)), sizeof(ShotData));
		return data;
	}
}

bool ChronoData::ReadOnly()
{
	return _readOnly;
}

int8_t ChronoData::AddShot(ShotData shotData)
{
	if (!_readOnly)
	{
		_sequenceShotData[_shotCount++] = shotData;
		return _shotCount - 1;
	}
	else
		return -1;
}

bool ChronoData::DeleteShot(int8_t shot)
{
	if (!_readOnly && _shotCount > 0)
	{
		if (shot < 0)
			shot = 0;
		else if (shot >= _shotCount)
			shot = _shotCount - 1;
		if (shot < _shotCount - 1)
			memmove((void*)&_sequenceShotData[shot], (void *)&_sequenceShotData[shot + 1], (_shotCount - shot - 1) * sizeof(ShotData));
		_shotCount--;
		return true;
	}
	else
		return false;
}

void ChronoData::ForEachShot(ProcessData processData, void * param, int8_t seq) const
{
	int i;
	if (seq < 0)
	{
		for	(i = 0; i < _shotCount; i++)
			processData(_sequenceShotData[i], param);
	}
	else
	{
		uint8_t *eeAddress = GetSeqAddress(seq);
		uint8_t shotCount = eeprom_read_byte(eeAddress++);
		for	(i = 0; i < shotCount; i++)
		{
			ShotData data;
			eeprom_read_block((void*)&data, (void*)eeAddress, sizeof(ShotData));
			processData(data, param);
			eeAddress += sizeof(ShotData);
		}
	}
}

uint8_t *ChronoData::GetSeqAddress(uint8_t seq) const
{
	uint8_t *eeAddress = _eeAddress + 3;
	while (seq-- > 0)
	{
		uint8_t shotCount = eeprom_read_byte(eeAddress++);
		eeAddress += shotCount * sizeof(ShotData);
	}
	return eeAddress;
}

size_t ChronoData::printStatsTo(Print& p) const
{
	size_t size = 0;
	if (_shotCount != 0)
	{
		size += p.print(F("Min/Max:   ")); size += p.print(SequenceMin()); size += p.print("/"); size += p.println(SequenceMax());
		size += p.print(F("Spread:    ")); size += p.println(SequenceSpread());
		size += p.print(F("Average:   ")); size += p.println(SequenceAverage());
		size += p.print(F("Deviation: ")); size += p.print(SequenceDeviation());
	}
	else
		size += p.print(F("<empty>"));
	return size;
}

size_t ChronoData::printSeqShotTo(Print& p) const
{
	size_t size = p.print(F("Series #")); size += p.println(_thisSeqNum);
	size += p.print(F("# Shots:   ")); size += p.println(_shotCount);
	return size;
}

size_t ChronoData::printTo(Print& p) const
{
	int i;
	const ShotData *data;
	size_t size = p.println(F("---ChronoData---"));
	size += printSeqShotTo(p);
	data = _sequenceShotData;
	for	(i = 0; i < _shotCount; i++)
	{
		size += p.print(i); size += p.print(": ");
		size += p.print(data->Velocity[0]); size += p.print(", ");
		size += p.print(data->Velocity[1]); size += p.print(", ");
		size += p.println(data->AvgVelocity());
		data++;
	}
	size += p.println(F("--Statistics--"));
	size += printStatsTo(p);
	return size;
}

ChronoData chronoData;

