// ButtonMenu.h
// Copyright (c) 2013 - Allen Bauer - http://blog.thereadoracleatdelphi.com
// Under MIT License

#ifndef _BUTTONMENU_h
#define _BUTTONMENU_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <stdint.h>
#include <T6963.h>
#include "TouchScreenButton.h"

typedef struct _MenuItem
{
	PGM_P caption;
	void (*Select)(void);
	TouchButton button;
} MenuItem;	

class ButtonMenu
{
private:
	uint8_t _maxX;
	uint8_t _maxY;
	T6963 *_LCD;
public:
	typedef enum Direction { Horizontal, Vertical };
	void init(uint8_t maxX, uint8_t maxY, T6963 &LCD) { _maxX = maxX; _maxY = maxY; _LCD = &LCD; };
	void ArrangeButtons(Direction dir, uint8_t buttonWidth, uint8_t buttonHeight, 
		uint8_t bufferX, uint8_t bufferY, uint8_t startX, uint8_t startY, 
		MenuItem *items, uint8_t numButtons, OnTouchHandler onTouchHandler);
	void ShowButtonMenu(MenuItem *items, uint8_t numButtons);
	void HideButtonMenu(MenuItem *items, uint8_t numButtons);
};

extern ButtonMenu buttonMenu;

#endif

