// T6963Console.h
// Copyright (c) 2013 - Allen Bauer - http://blog.thereadoracleatdelphi.com
// Under MIT License

#ifndef T6963Console_h
#define T6963Console_h

#include <inttypes.h>
#include <Arduino.h>
#include <T6963.h>
#include <Print.h>
#include <WString.h>

class LCDConsole : public Print
{
private:
	T6963 *_LCD;
	int _row;
	int _col;
	int _winLeft;
	int _winTop;
	int _winRight;
	int _winBottom;
	bool _caretVisible;
	void updateRowCol();
	void incrementRow();
	void incrementCol();
	void decrementCol();
	void decrementRow();
public:
	typedef char (*GetKey)(void);
	LCDConsole(T6963 *lcd);
	virtual size_t write(uint8_t c);
	void clearScreen(void);
	void initialize(void);
	void scrollView(void);
	bool setConsoleWindow(int winLeft, int winTop, int winRight, int winBottom);
	void restoreRowCol();
	void showCaret();
	void hideCaret();
	String readLine(GetKey getKey);

	int getConsoleLeft(void);
	int getConsoleTop(void);
	int getConsoleCols(void);
	int getConsoleRows(void);
};

#endif