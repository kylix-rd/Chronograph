#include "T6963Console.h"

LCDConsole::LCDConsole(T6963 *lcd) : _LCD(lcd), _row(0), _col(0), _winTop(0), _winLeft(0), _winRight(0), _winBottom(0), _caretVisible(true) { }

void LCDConsole::updateRowCol()
{
	_LCD->setCursorPointer(_winLeft + _col, _winTop + _row);
	_LCD->TextGoTo(_winLeft + _col, _winTop + _row);
}

void LCDConsole::restoreRowCol()
{
	updateRowCol();
}

size_t LCDConsole::write(uint8_t c)
{
	switch (c)
	{
		case '\r': { _col = 0; updateRowCol(); return 1; }
		case '\n': { incrementRow(); updateRowCol(); return 1; }
		case '\t': 
			{
				uint8_t spacesToTab = (8 - _col % 8);
				while (spacesToTab-- > 0)
					write(' ');
				return 1; 
			}
	}
	_LCD->writeChar(c);
	incrementCol();
	updateRowCol();
	return 1;
}

void LCDConsole::initialize(void)
{
	_LCD->setCursorPattern(1);
	_LCD->setDispMode(true, true, true, true);
	_winRight = _LCD->getTextCols() - 1;
	_winBottom = _LCD->getTextRows() - 1;
	clearScreen();
}

void LCDConsole::showCaret()
{
	_caretVisible = true;
	_LCD->setDispMode(true, true, true, true);
}

void LCDConsole::hideCaret()
{
	_caretVisible = false;
	_LCD->setDispMode(true, true, false, false);
}


void LCDConsole::incrementCol()
{
	if (++_col >= getConsoleCols())
	{
		_col = 0;
		incrementRow();
	}
}

void LCDConsole::incrementRow()
{
	if (++_row >= getConsoleRows())
	{
		_row = getConsoleRows() - 1;
		scrollView();
	}
}

void LCDConsole::decrementCol()
{
	if (--_col < 0)
	{
		_col = getConsoleCols() - 1;
		decrementRow();
	}
}

void LCDConsole::decrementRow()
{
	if (--_row < 0)
		_row = 0;
}

void LCDConsole::clearScreen(void)
{
	_col = 0;
	_row = 0;
	if (getConsoleCols() == _LCD->getTextCols() && getConsoleRows() == _LCD->getTextRows())
		_LCD->clearText();
	else
	{
		int r, c;
		int tr = getConsoleRows();
		int tc = getConsoleCols();
		for (r = 0; r < tr; r++)
		{
			_LCD->TextGoTo(_winLeft, _winTop + r);
			_LCD->writeCommand(T6963_SET_DATA_AUTO_WRITE);
			for (c = 0; c < tc; c++)
			{
				while(!(_LCD->checkStatus()&0x8));
				_LCD->writeData(' ' - 32);
			}
			while(!(_LCD->checkStatus()&0x8));
			_LCD->writeCommand(T6963_AUTO_RESET);
		}
	}
	updateRowCol();
}

void LCDConsole::scrollView(void)
{
	uint8_t buffer[80]; // local temp buffer for each line
	int r, c;
	int tr = getConsoleRows();
	int tc = getConsoleCols();
	for (r = 0; r < tr - 1; r++)
	{
		// read the "next" row and copy to "this" row;
		_LCD->TextGoTo(_winLeft, _winTop + r + 1);
		_LCD->writeCommand(T6963_SET_DATA_AUTO_READ);
		for (c = 0; c < tc; c++)
		{
			while(!(_LCD->checkStatus()&0x4));
			buffer[c] = _LCD->ReadData();
		}
		while(!(_LCD->checkStatus()&0x4));
		_LCD->writeCommand(T6963_AUTO_RESET);
		_LCD->TextGoTo(_winLeft, _winTop + r);
		_LCD->writeCommand(T6963_SET_DATA_AUTO_WRITE);
		for (c = 0; c < tc; c++)
		{
			while(!(_LCD->checkStatus()&0x8));
			_LCD->writeData(buffer[c]);
		}
		while(!(_LCD->checkStatus()&0x8));
		_LCD->writeCommand(T6963_AUTO_RESET);
	}
	// now "clear" the last line on the screen
	_LCD->TextGoTo(_winLeft, _winTop + tr - 1);
	_LCD->writeCommand(T6963_SET_DATA_AUTO_WRITE);
	for (c = 0; c < tc; c++)
	{
		while(!(_LCD->checkStatus()&0x8));
		_LCD->writeData(' ' - 32);
	}
	while(!(_LCD->checkStatus()&0x8));
	_LCD->writeCommand(T6963_AUTO_RESET);
	updateRowCol();
}

int LCDConsole::getConsoleLeft(void)
{
	return _winLeft;
}

int LCDConsole::getConsoleTop(void)
{
	return _winTop;
}

int LCDConsole::getConsoleCols(void)
{
	return (_winRight - _winLeft) + 1;
}

int LCDConsole::getConsoleRows(void)
{
	return (_winBottom - _winTop) + 1;
}

bool LCDConsole::setConsoleWindow(int winLeft, int winTop, int winRight, int winBottom)
{
	if ((winLeft >= 0) && (winTop >= 0) && 
		(winRight > winLeft) && (winBottom > winTop) && 
		(winRight < _LCD->getTextCols()) && (winBottom < _LCD->getTextRows()))
	{
		int width, height;
		_winLeft = winLeft;
		_winTop = winTop;
		_winRight = winRight;
		_winBottom = winBottom;
		width = getConsoleCols();
		height = getConsoleRows();
		_col = (_col >= width) ? width - 1 : _col;
		_row = (_row >= height) ? height - 1 : _row;
		updateRowCol();
		return true;
	}
	return false;
}

String LCDConsole::readLine(GetKey getKey)
{
	char buffer[80];
	int8_t index = 0;
	bool wasCaretVisible = _caretVisible;
	showCaret();
	while (true)
	{
		char c = getKey();
		switch (c)
		{
		case 13:
			{
				buffer[index] = 0;
				String res = String(buffer);
				if (!wasCaretVisible)
					hideCaret();
				return res;
			}
		case 8: // backspace
			{
				if (index > 0)
				{
					index--;
					decrementCol();
					updateRowCol();
					_LCD->writeChar(' ');
					updateRowCol();
				}
				break;
			}
		default:
			if (index < sizeof(buffer) && c >= ' ' && c <= '~')
			{
			  buffer[index++] = c;
			  write(c);
			}
		}
	}
}
