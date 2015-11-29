//
// Copyright (c) 2013 - Allen Bauer - http://blog.thereadoracleatdelphi.com
// Under MIT License

#ifndef TouchKeyboard_h
#define TouchKeyboard_h

#include <stdint.h>
#include "Arduino.h"
#include "TouchScreen.h"

extern PROGMEM prog_uchar numberRow[14];
extern PROGMEM prog_uchar qwertyRow[14];
extern PROGMEM prog_uchar asdfRow[13];
extern PROGMEM prog_uchar zxcvRow[12];
extern PROGMEM prog_uchar spacebarRow[6];

extern PROGMEM prog_uchar numberRowCodes[14];
extern PROGMEM prog_uchar qwertyRowCodes[14];
extern PROGMEM prog_uchar asdfRowCodes[13];
extern PROGMEM prog_uchar zxcvRowCodes[12];
extern PROGMEM prog_uchar spacebarRowCodes[6];

typedef struct KeyboardRow
{
	PROGMEM prog_uchar *row;
	PROGMEM prog_uchar *codes;
	PROGMEM prog_uchar keys;
} KeyboardRow;

extern KeyboardRow PROGMEM keyboardRow[5];

#define ROW_HEIGHT 72

#define VK_BACK           0x08
#define VK_TAB            0x09
#define VK_RETURN         0x0D
#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_ALT            0x12
#define VK_CAPITAL        0x14
#define VK_ESCAPE         0x1B
#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E

#define VK_0              0x30
#define VK_1              0x31
#define VK_2              0x32
#define VK_3              0x33
#define VK_4              0x34
#define VK_5              0x35
#define VK_6              0x36
#define VK_7              0x37
#define VK_8              0x38
#define VK_9              0x39

#define VK_A              0x41
#define VK_B              0x42
#define VK_C              0x43
#define VK_D              0x44
#define VK_E              0x45
#define VK_F              0x46
#define VK_G              0x47
#define VK_H              0x48
#define VK_I              0x49
#define VK_J              0x4A
#define VK_K              0x4B
#define VK_L              0x4C
#define VK_M              0x4D
#define VK_N              0x4E
#define VK_O              0x4F
#define VK_P              0x50
#define VK_Q              0x51
#define VK_R              0x52
#define VK_S              0x53
#define VK_T              0x54
#define VK_U              0x55
#define VK_V              0x56
#define VK_W              0x57
#define VK_X              0x58
#define VK_Y              0x59
#define VK_Z              0x5A

#define VK_SEMI           0xBA   // ';:' for US
#define VK_EQUAL          0xBB   // '=+' any country
#define VK_COMMA          0xBC   // ',' any country
#define VK_MINUS          0xBD   // '-' any country
#define VK_PERIOD         0xBE   // '.' any country
#define VK_SLASH          0xBF   // '/?' for US
#define VK_BQUOTE         0xC0   // '`~' for US

#define VK_LBRACK         0xDB  //  '[{' for US
#define VK_BSLASH         0xDC  //  '\|' for US
#define VK_RBRACK         0xDD  //  ']}' for US
#define VK_APOSTROPHE     0xDE  //  ''"' for US

class TouchKeyboard
{
public:
	typedef enum Prompt
	{
		leftBottom,
		rightTop,
		badData
	} Prompt;

	typedef enum MetaKeys
	{
		mkShift = 0x01,
		mkCtrl = 0x02,
		mkAlt = 0x04,
		mkCaps = 0x08
	} MetaKeys;

	struct CalibrationPrompt
	{
		virtual void PromptUser(Prompt prompt) = 0;
	};

	struct MetakeyStateChange
	{
		virtual void Changed(MetaKeys metaKeys) = 0;
	};
public:
	static boolean GetKeyboardCalibration(TouchScreen &screen, Rect &bounds, CalibrationPrompt &prompt);

	TouchKeyboard(TouchScreen &screen);
	TouchKeyboard(TouchScreen &screen, MetakeyStateChange &stateChange);
	// Return true if the calibration data needs to be written
	bool Initialize(CalibrationData &calibration, CalibrationPrompt &prompt, bool forceCalibration);

	bool isMetaKey(uint8_t keyCode);
	bool getKeyPressed(uint8_t &row, uint8_t &key, bool wait);
	bool getKeyPressed(uint8_t &row, uint8_t &key, bool wait, Point &pressPt);
	bool getScanCode(uint8_t &scanCode, bool wait, Point &pressPt);
	bool getKeyCode(uint8_t &keyCode, MetaKeys &metaKeys, bool wait, Point &pressPt, bool returnMeta = false);
	void toggleMetaKeyState(MetaKeys metaKeys);

	char ToASCII(uint8_t keyCode, MetaKeys metaKeys, bool clearMetaState = true);

private:
	typedef struct MetaKeyBits {
		bool _shift:1, _ctrl:1, _alt:1, _caps:1;
	} MetaKeyBits;
	TouchScreen *_screen;
	MetakeyStateChange *_stateChange;
	Rect _bounds;
	union {
		MetaKeys _metaKeys;
		MetaKeyBits _metaKeyBits;
	};
	void clearMetaState(void);
	void metaStateChanged(void);
};

#endif