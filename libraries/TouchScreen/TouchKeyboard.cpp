//
// Copyright (c) 2013 - Allen Bauer - http://blog.thereadoracleatdelphi.com
// Under MIT License
//
#include <stdlib.h>
#include "Arduino.h"
#include <avr/pgmspace.h>
#include "TouchScreen.h"
#include "TouchKeyboard.h"
#include <EEPROM.h>

PROGMEM prog_uchar numberRow[14] = 
/*    `   1   2   3   4   5   6   7   8   9   0   -   =  del */
	{60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 100};
PROGMEM prog_uchar qwertyRow[14] =
/*   \t   Q   W   E   R   T   Y   U   I   O   P   [   ]   \ */
	{100, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60};
PROGMEM prog_uchar asdfRow[13] =
/*   cl   A   S   D   F   G   H   J   K   L   ;   '  <enter> */
   {110, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 110};
PROGMEM prog_uchar zxcvRow[12] =
/*  shf   Z   X   C   V   B   N   M   ,   .   /  shf*/
   {140, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 140};
PROGMEM prog_uchar spacebarRow[6] =
/*   ctl  alt  spc  spc  alt  ctl */
    {120, 120, 200, 200, 120, 120};

PROGMEM prog_uchar numberRowCodes[14] = 
/*      `     1     2     3     4     5     6     7     8     9     0     -     =   del */
	{0x0E, 0x16, 0x1E, 0x26, 0x25, 0x2E, 0x36, 0x3D, 0x3E, 0x46, 0x45, 0x4E, 0x55, 0x66};
PROGMEM prog_uchar qwertyRowCodes[14] =
/*     \t     Q     W     E     R     T     Y     U     I     O     P     [     ]   \ */
	{0x0D, 0x15, 0x1D, 0x24, 0x2D, 0x2C, 0x35, 0x3C, 0x43, 0x44, 0x4D, 0x54, 0x5B, 0x5D};
PROGMEM prog_uchar asdfRowCodes[13] =
/*    cl     A     S     D     F     G     H     J     K     L     ;     '  <enter> */
   {0x58, 0x1C, 0x1B, 0x23, 0x2B, 0x34, 0x33, 0x3B, 0x42, 0x4B, 0x4C, 0x52, 0x5A};
PROGMEM prog_uchar zxcvRowCodes[12] =
/*   shf     Z     X     C     V     B     N     M     ,     .     /   shf*/
   {0x12, 0x1A, 0x22, 0x21, 0x2A, 0x32, 0x31, 0x3A, 0x41, 0x49, 0x4A, 0x59};
PROGMEM prog_uchar spacebarRowCodes[6] =
/*    ctl   alt   spc   spc   alt   ctl */
    {0x14, 0x11, 0x29, 0x29, 0x11, 0x14};

KeyboardRow PROGMEM keyboardRow[5] = 
{
	{spacebarRow, spacebarRowCodes, sizeof(spacebarRow)},
	{zxcvRow, zxcvRowCodes, sizeof(zxcvRow)},
	{asdfRow, asdfRowCodes, sizeof(asdfRow)},
	{qwertyRow, qwertyRowCodes, sizeof(qwertyRow)},
	{numberRow, numberRowCodes, sizeof(numberRow)}
};

#define VK_APOS VK_APOSTROPHE

PROGMEM prog_uchar scanCodeToVK[0x67] = {
	  /*        00          01          02          03          04          05          06          07 */
	  /*        08          09          0A          0B          0C          0D          0E          0F */
/*00*/	      0x00,       0x00,       0x00,       0x00,       0x00,       0x00,       0x00,       0x00, 
/*08*/        0x00,       0x00,       0x00,       0x00,       0x00,     VK_TAB,  VK_BQUOTE,       0x00,
/*10*/	      0x00,     VK_ALT,   VK_SHIFT,       0x00, VK_CONTROL,       VK_Q,       VK_1,       0x00, 
/*18*/        0x00,       0x00,       VK_Z,       VK_S,       VK_A,       VK_W,       VK_2,       0x00, 
/*20*/	      0x00,       VK_C,       VK_X,       VK_D,       VK_E,       VK_4,       VK_3,       0x00, 
/*28*/        0x00,   VK_SPACE,       VK_V,       VK_F,       VK_T,       VK_R,       VK_5,       0x00, 
/*30*/	      0x00,       VK_N,       VK_B,       VK_H,       VK_G,       VK_Y,       VK_6,       0x00, 
/*38*/        0x00,       0x00,       VK_M,       VK_J,       VK_U,       VK_7,       VK_8,       0x00, 
/*40*/	      0x00,   VK_COMMA,       VK_K,       VK_I,       VK_O,       VK_0,       VK_9,       0x00, 
/*48*/        0x00,  VK_PERIOD,   VK_SLASH,       VK_L,    VK_SEMI,       VK_P,   VK_MINUS,       0x00, 
/*50*/	      0x00,       0x00,    VK_APOS,       0x00,  VK_LBRACK,   VK_EQUAL,       0x00,       0x00, 
/*58*/  VK_CAPITAL,   VK_SHIFT,  VK_RETURN,  VK_RBRACK,       0x00,  VK_BSLASH,       0x00,       0x00, 
/*60*/	      0x00,       0x00,       0x00,       0x00,       0x00,       0x00,    VK_BACK 
};

boolean TouchKeyboard::GetKeyboardCalibration(TouchScreen &screen, Rect &bounds, CalibrationPrompt &prompt)
{
	Point lb, tr;
	prompt.PromptUser(leftBottom);
	lb = screen.getPressAndRelease(true);
	prompt.PromptUser(rightTop);
	tr = screen.getPressAndRelease(true);
	if (lb.x < tr.x && lb.y < tr.y)
	{
		bounds.left = lb.x;
		bounds.bottom = lb.y;
		bounds.right = tr.x;
		bounds.top = tr.y;
		return true;
	}
	else
		return false;
}

TouchKeyboard::TouchKeyboard(TouchScreen &screen) : _screen(&screen), _stateChange(NULL) { }

TouchKeyboard::TouchKeyboard(TouchScreen &screen, MetakeyStateChange &stateChange) : _screen(&screen), _stateChange(&stateChange) { }

bool TouchKeyboard::Initialize(CalibrationData &calibration, CalibrationPrompt &prompt, bool forceCalibration)
{
	if (!forceCalibration && calibration.signature[0] == 't' && calibration.signature[1] == 'k' && 
		calibration.signature[2] == 'b' && calibration.signature[3] == 'd')
	{
		_bounds = calibration.bounds;
		return false;
	}
	else
	{
		Rect bounds;
		while (!GetKeyboardCalibration(*_screen, bounds, prompt))
			prompt.PromptUser(badData);
		_bounds = bounds;
		strncpy(calibration.signature, "tkbd", 4);
		calibration.bounds = bounds;
		return true;
	}
}

bool TouchKeyboard::isMetaKey(uint8_t keyCode)
{
	return (keyCode == VK_CONTROL) || (keyCode == VK_ALT) || (keyCode == VK_SHIFT) || (keyCode == VK_CAPITAL);
}

void TouchKeyboard::metaStateChanged(void)
{
	if (_stateChange != NULL)
		_stateChange->Changed(_metaKeys);
}

void TouchKeyboard::clearMetaState(void)
{
	_metaKeyBits._shift = false;
	_metaKeyBits._ctrl = false;
	_metaKeyBits._alt = false;
	metaStateChanged();
}

void TouchKeyboard::toggleMetaKeyState(MetaKeys metaKeys)
{
	if (metaKeys & mkShift)
		_metaKeyBits._shift = !_metaKeyBits._shift;
	if (metaKeys & mkCtrl)
		_metaKeyBits._ctrl = !_metaKeyBits._ctrl;
	if (metaKeys & mkAlt)
		_metaKeyBits._alt = !_metaKeyBits._alt;
	if (metaKeys & mkCaps)
		_metaKeyBits._caps = !_metaKeyBits._caps;
	metaStateChanged();
}

bool TouchKeyboard::getKeyPressed(uint8_t &row, uint8_t &key, bool wait, Point &pressPt)
{
	do
	{
		if (_screen->isTouching())
		{
			int8_t r, k;
			Point point = _screen->getPressAndRelease();
			pressPt = point;
			if (!_bounds.ContainsPoint(point))
				return false;
			// normalize point relative to 0
			point.x -= _bounds.left;
			point.y -= _bounds.bottom;
			// now scale the point to the keyboard bounds
			point.x = Scale(point.x, 880, _bounds.getWidth());
			point.y = Scale(point.y, 5 * ROW_HEIGHT, _bounds.getHeight());
			for (r = 0; r < sizeof(keyboardRow) / sizeof(keyboardRow[0]); r++)
			{
				if ((r + 1) * ROW_HEIGHT > point.y)
				{
					int16_t keyPos = 0;
					uint8_t keys = pgm_read_byte_near(&keyboardRow[r].keys);
					uint16_t rowData = pgm_read_word_near(&keyboardRow[r].row);
					for (k = 0; k < keys; k++)
					{
						keyPos += pgm_read_byte_near(rowData + k);
						if (keyPos > point.x)
						{
							row = r;
							key = k;
							return true;
						}
					}
				}
			}
			return false;
		}
	} while (wait);
	return false;
}

bool TouchKeyboard::getKeyPressed(uint8_t &row, uint8_t &key, bool wait)
{
	Point pressPt;
	return getKeyPressed(row, key, wait, pressPt);
}

bool TouchKeyboard::getScanCode(uint8_t &scanCode, bool wait, Point &pressPt)
{
	uint8_t row, key;
	bool result = getKeyPressed(row, key, wait, pressPt);
	if (result)
	{
		uint16_t scanCodes = pgm_read_word_near(&keyboardRow[row].codes);
		scanCode = pgm_read_byte_near(scanCodes + key);
	}
	return result;
}

bool TouchKeyboard::getKeyCode(uint8_t &keyCode, MetaKeys &metaKeys, bool wait, Point &pressPt, bool returnMeta)
{
	uint8_t scanCode;
	bool result;
	do
	{
		result = getScanCode(scanCode, wait, pressPt);
		if (result)
		{
			if (scanCode > sizeof(scanCodeToVK) - 1)
				return false;
			keyCode = pgm_read_byte_near(&scanCodeToVK[scanCode]);
			if (isMetaKey(keyCode))
			{
				switch (keyCode)
				{
					case VK_CONTROL: toggleMetaKeyState(mkCtrl); break;
					case VK_ALT: toggleMetaKeyState(mkAlt); break;
					case VK_SHIFT: toggleMetaKeyState(mkShift); break;
					case VK_CAPITAL: toggleMetaKeyState(mkCaps); break;
				}
			} else break;
		} else break;
	} while (!returnMeta && isMetaKey(keyCode));
	metaKeys = _metaKeys;
	if (result && !returnMeta)
		clearMetaState();
	return result;
}

PROGMEM prog_uchar ShiftNumerals[VK_9 - VK_0 + 1] = 
{
	')', '!', '@', '#', '$', '%', '^', '&', '*', '('
};

PROGMEM prog_uchar SpecialKeys[VK_BQUOTE - VK_SEMI + 1] =
{
	';', '=', ',', '-', '.', '/', '`'
};

PROGMEM prog_uchar ShiftSpecialKeys[VK_BQUOTE - VK_SEMI + 1] =
{
	':', '+', '<', '_', '>', '?', '~'
};

char TouchKeyboard::ToASCII(uint8_t keyCode, MetaKeys metaKeys, bool clearMetaState)
{
	char ascii = 0;
	if (keyCode >= VK_0 && keyCode <= VK_9)
	{
		if (((mkCtrl | mkAlt) & metaKeys) == 0)
		{
			if (mkShift & metaKeys)
				ascii = (char)pgm_read_byte_near(&ShiftNumerals[keyCode - VK_0]);
			else
				ascii = keyCode;
		}
	}						  
	else if ((keyCode >= VK_A && keyCode <= VK_Z) || (keyCode >= VK_LBRACK && keyCode <= VK_RBRACK))
	{
		if (keyCode >= VK_LBRACK)
			keyCode -= 0x80;
		if ((mkAlt & metaKeys) == 0)
		{
			if ((mkCtrl & metaKeys) != 0 && (mkShift & metaKeys) == 0)
			{
				ascii = keyCode - 0x40;
			}
			else if (keyCode <= VK_Z)
			{
				if  ((mkCtrl & metaKeys) == 0 && (mkShift & metaKeys) != 0)
					ascii = keyCode | ((metaKeys & mkCaps) << 2);
				else
					ascii = keyCode | (((metaKeys & mkCaps) << 2) ^ 0x20);
			}
			else
			{
				ascii = keyCode;
				if ((mkCtrl & metaKeys) == 0 && (mkShift & metaKeys) != 0)
					ascii |= 0x20;
			}
		}
	}
	else
	{
		switch (keyCode)
		{
			case VK_BACK: 
			case VK_TAB:
			case VK_RETURN:
			case VK_ESCAPE:         
			case VK_SPACE:
				if ((metaKeys & (mkCtrl | mkShift | mkAlt)) == 0) ascii = keyCode; break;

			case VK_SEMI:
			case VK_EQUAL:
			case VK_COMMA:
			case VK_MINUS:
			case VK_PERIOD:
			case VK_SLASH:
			case VK_BQUOTE:
				if ((metaKeys & (mkCtrl | mkAlt)) == 0)
				{
					if ((metaKeys & mkShift) == 0)
						ascii = (char)pgm_read_byte_near(&SpecialKeys[keyCode - VK_SEMI]);
					else
						ascii = (char)pgm_read_byte_near(&ShiftSpecialKeys[keyCode - VK_SEMI]);
				}
				break;

			case VK_APOSTROPHE:
				if ((metaKeys & (mkCtrl | mkAlt)) == 0)
				{
					if ((metaKeys & mkShift) == 0)
						ascii = '\'';
					else
						ascii = '"';
				}
				break;

		}
	}
	if (clearMetaState)
		this->clearMetaState();
	return ascii;
}

 