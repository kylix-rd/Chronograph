// 
// Copyright (c) 2013 - Allen Bauer - http://blog.thereadoracleatdelphi.com
// Under MIT License
// 

#include "ButtonMenu.h"

void ButtonMenu::ArrangeButtons(Direction dir, uint8_t buttonWidth, uint8_t buttonHeight, 
	uint8_t bufferX, uint8_t bufferY, uint8_t startX, uint8_t startY, MenuItem *items, 
	uint8_t numButtons, OnTouchHandler onTouchHandler)
{
	int i, x = startX, y = startY;
	for	(i = 0; i < numButtons; i++)
	{
		items[i].button.initSimpleButton(x, y, buttonWidth, buttonHeight, items[i].caption, 1, i, onTouchHandler);
		if (dir == Vertical)
		{
			y += (buttonHeight + bufferY);
			if (y + buttonHeight > _maxY)
			{
				x += buttonWidth + bufferX;
				y = startY;
			}
		} else
		{
			x += (buttonWidth + bufferX);
			if (x + buttonWidth > _maxX)
			{
				y += buttonHeight + bufferY;
				x = startX;
			}
		}
	}
}

void ButtonMenu::ShowButtonMenu(MenuItem *items, uint8_t numButtons)
{
	while (numButtons-- > 0)
	{
		items->button.drawButton();
		items++;
	}
}

void ButtonMenu::HideButtonMenu(MenuItem *items, uint8_t numButtons)
{
	while (numButtons-- > 0)
	{
		items->button.deactivate();
		items++;
	}
	_LCD->clearGraphic();
	_LCD->clearText();
}


ButtonMenu buttonMenu;

