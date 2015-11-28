#include <SD.h>
#include "ButtonMenu.h"
#include "ChronoData.h"
#include "Events.h"
#include "ChronoScreen.h"
#include <EEPROM.h>
#include <FastTimer.h>
#include <TouchScreen.h>
#include <TouchKeyboard.h>
#include <TouchScreenButton.h>
#include <T6963Console.h>
#include <T6963.h>
#include <gfxdata.h>
#include <font.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <WString.h>

#define YP A0  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM A2  // can be a digital pin
#define XP A1  // can be a digital pin
#define VREF 4.98 // Actual +5V as measured
#define VBATT A4 // Scaled battery voltage on this pin
#define VRATIO 3.127 // Divide ratio of (100K+47K)/47K

#define KEYBOARD_CALIBRATION_DATA_ADDRESS 0x0000
#define REGION_CALIBRATION_ADDRESS sizeof(CalibrationData)
#define CHRONODATA_EEPROM_ADDRESS ((uint8_t *)0x0200)

extern const struct FONT_DEF Arial__27;
extern const struct FONT_DEF font6x8;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 600);
T6963 LCD(240, 64, 6, 8);
LCDConsole Console = LCDConsole(&LCD);
TouchRegion rgn = TouchRegion(ts);

struct RegionCalibration : public TouchRegion::Calibration
{
	RegionCalibration(T6963 &lcd) : _lcd(&lcd) {};
	virtual void Clear();
	virtual uint16_t getWidth();
	virtual uint16_t getHeight();
	virtual void DrawTextPGM(uint16_t x, uint16_t y, PGM_P text);
	virtual void DrawCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color);
	virtual void FillCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color);
private:
	T6963 *_lcd;
};

RegionCalibration cal = RegionCalibration(LCD);

#define Print(P) {\
	Serial.print(P);\
	Console.print(P); }

#define Println(P) {\
	Serial.println(P); \
	Console.println(P); }

struct Prompt : public TouchKeyboard::CalibrationPrompt
{
	virtual void PromptUser(TouchKeyboard::Prompt prompt);
};

struct MetakeyStateChange : public TouchKeyboard::MetakeyStateChange
{
	virtual void Changed(TouchKeyboard::MetaKeys metaKeys);
};

CalibrationData calibration;

MetakeyStateChange metaStateChange;

TouchKeyboard keyboard = TouchKeyboard(ts, metaStateChange);

struct EventLoop
{
private:
	TouchRegion *_rgn;
	TouchKeyboard *_keyboard;
	TouchScreen *_ts;
	bool exitLoop;
public:
	typedef void (*KeyPressHandler)(EventLoop *loop, uint8_t keycode, TouchKeyboard::MetaKeys metaKeys);
	typedef void (*TouchRegionHandler)(EventLoop *loop, Point rgnPt);
	EventLoop() : _rgn(NULL), _keyboard(NULL), _ts(NULL), exitLoop(false) {};
	EventLoop(TouchRegion &rgn, TouchKeyboard &keyboard, TouchScreen &ts) : _rgn(&rgn), _keyboard(&keyboard), _ts(&ts), exitLoop(false) {};
	void DoEvents(KeyPressHandler keyPressHandler, TouchRegionHandler regionHandler);
	bool DoEvent(KeyPressHandler keyPressHandler, TouchRegionHandler regionHandler);
	void ExitLoop() { exitLoop = true; };
};

void EventLoop::DoEvents(KeyPressHandler keyPressHandler, TouchRegionHandler regionHandler)
{
	while (!DoEvent(keyPressHandler, regionHandler)) {};
}

bool EventLoop::DoEvent(KeyPressHandler keyPressHandler, TouchRegionHandler regionHandler)
{
	uint8_t keycode;
	TouchKeyboard::MetaKeys metaKeys;
	Point pressPt;
	bool exitReturn;
	bool saveExitLoop = exitLoop;
	exitLoop = false;
	bool keyHit = _keyboard->getKeyCode(keycode, metaKeys, true, pressPt);
	if (pressPt.z > _ts->pressureThreshhold)
	{
		if (keyHit)	{
			if (keyPressHandler != NULL) keyPressHandler(this, keycode, metaKeys); }
		else {
			if (regionHandler != NULL) regionHandler(this, _rgn->rawPointToRegionPoint(pressPt)); }
	}
	exitReturn = exitLoop;
	exitLoop = saveExitLoop;
	return exitReturn;
}


EventLoop eventLoop = EventLoop(rgn, keyboard, ts);

void Prompt::PromptUser(TouchKeyboard::Prompt prompt)
{
	switch (prompt)
	{
	case TouchKeyboard::leftBottom: Println(F("Touch keyboard left bottom")); break;
	case TouchKeyboard::rightTop: Println(F("Touch keyboard right top")); break;
	case TouchKeyboard::badData: Println(F("Bad calibration data")); break;
	}
}

void ShowMetaKeyState(uint8_t Col, prog_char *onString, prog_char *offString, bool on)
{
	LCD.TextGoTo(Col, LCD.getTextRows() - 1); 
	if (on)
		LCD.WriteStringPgm(onString);
	else
		LCD.WriteStringPgm(offString);
}

prog_char MetaOff[] PROGMEM = "          ";
prog_char CapsOn[] PROGMEM  = "Caps      ";
prog_char ShiftOn[] PROGMEM = "Shift     ";
prog_char CtrlOn[] PROGMEM  = "Ctrl      ";
prog_char AltOn[] PROGMEM   = "Alt       ";

void MetakeyStateChange::Changed(TouchKeyboard::MetaKeys metaKeys)
{
	ShowMetaKeyState(0, CapsOn, MetaOff, (metaKeys & TouchKeyboard::mkCaps) != 0);
	ShowMetaKeyState(10, ShiftOn, MetaOff, (metaKeys & TouchKeyboard::mkShift) != 0);
	ShowMetaKeyState(20, CtrlOn, MetaOff, (metaKeys & TouchKeyboard::mkCtrl) != 0);
	ShowMetaKeyState(30, AltOn, MetaOff, (metaKeys & TouchKeyboard::mkAlt) != 0);
	Console.restoreRowCol();
}

void CaptureShotData(void);
void ReviewShotData(void);
void DownloadShotData(void);
void TestMenu(void);
void ReturnToMainMenu(void);

prog_char CaptureMsg[] PROGMEM = {"Capture Shot Data"};
prog_char ReviewDataMsg[] PROGMEM = {"Review Shot Data"};
prog_char DownloadDataMsg[] PROGMEM = {"Save Shot Data"};
prog_char TestMenuMsg[] PROGMEM = {"Test Menu"};

MenuItem mainMenu[] = {
	{CaptureMsg, CaptureShotData},
	{ReviewDataMsg, ReviewShotData},
	{DownloadDataMsg, DownloadShotData},
	{TestMenuMsg, TestMenu},
};

#define numMainMenuItems (sizeof(mainMenu) / sizeof(MenuItem))

prog_char MainMenuMsg[] PROGMEM = {"Main Menu"};

prog_char TestFontMsg[] PROGMEM = {"Tst font"};
prog_char CalibrateKeyboardMsg[] PROGMEM = {"Cal kybd"};
prog_char TestChronoScreensMsg[] PROGMEM = {"Tst Scrn"};
prog_char TestEventsMsg[] PROGMEM = {"Tst Evnt"};
prog_char TestTouchRegionMsg[] PROGMEM = {"Tst Rgn"};
prog_char RecalibrateRegionMsg[] PROGMEM = {"Cal Rgn"};
prog_char TestChronoDataMsg[] PROGMEM = {"Tst Data"};
//prog_char TestSDCardMsg[] PROGMEM = {"Test SD"};

MenuItem menu[] = {
	{MainMenuMsg, ReturnToMainMenu},
	{TestFontMsg, TestFont},
	{CalibrateKeyboardMsg, CalibrateKeyboard},
	{TestChronoScreensMsg, TestChronoScreens},
	{TestEventsMsg, TestEvents},
	{TestTouchRegionMsg, TestTouchRegion},
//	{RecalibrateRegionMsg, RecalibrateRegion},
	{TestChronoDataMsg, TestChronoData},
//	{TestSDCardMsg, TestSDCard},
};

#define numMenuItems (sizeof(menu) / sizeof(MenuItem))

MenuItem chronoMenu[] = {
	{MainMenuMsg, NULL}
};

#define numChronoMenuItems (sizeof(chronoMenu) / sizeof(MenuItem))

prog_char SaveDataMsg[] PROGMEM = {"Save"};
prog_char LoadDataMsg[] PROGMEM = {"Load"};
prog_char ClearAllDataMsg[] PROGMEM = {"Clr Data"};

MenuItem chronoDataMenu[] = {
	{MainMenuMsg, NULL},
	{SaveDataMsg, NULL},
	{LoadDataMsg, NULL},
	{ClearAllDataMsg, NULL},
};

#define numChronoDataMenuItems (sizeof(chronoDataMenu) / sizeof(MenuItem))

prog_char DeleteShotMsg[] PROGMEM = {"Delete"};
prog_char ResetSeqMsg[] PROGMEM = {"Reset"};
prog_char SaveSeqMsg[] PROGMEM = {"Save"};

MenuItem shotCaptureMenu[] = {
	{MainMenuMsg, NULL},
	{DeleteShotMsg, NULL},
	{ResetSeqMsg, NULL},
	{SaveSeqMsg, NULL},
};

#define numShotCaptureMenuItems (sizeof(shotCaptureMenu) / sizeof(MenuItem))

prog_char SelectSeqMsg[] PROGMEM = {"Select"};
prog_char SeqStatsMsg[] PROGMEM = {"Shots"};
prog_char ClearSeqMsg[] PROGMEM = {"Clear"};

MenuItem reviewShotDataMenu[] = {
	{MainMenuMsg, NULL},
	{SelectSeqMsg, NULL},
	{SeqStatsMsg, NULL},
	{ClearSeqMsg, NULL},
};

#define numReviewShotDataMenuItems (sizeof(reviewShotDataMenu) / sizeof(MenuItem))

prog_char SaveToSDMsg[] PROGMEM = {"SD Card"};
prog_char SaveToSerialMsg[] PROGMEM = {"Serial"};
prog_char SaveToConsoleMsg[] PROGMEM = {"Console"};

MenuItem saveShotDataMenu[] = {
	{MainMenuMsg, NULL},
	{SaveToSDMsg, NULL},
	{SaveToSerialMsg, NULL},
	{SaveToConsoleMsg, NULL},
};

#define numSaveShotDataMenuItems (sizeof(saveShotDataMenu) / sizeof(MenuItem))

void ButtonMenuClick(TouchButton *button, int16_t value)
{
	menu[value].Select();
}

void MainMenuButtonClick(TouchButton *button, int16_t value)
{
	mainMenu[value].Select();
}

volatile bool returnToMainMenu = false;

void MenuReturnButtonClick(TouchButton *button, int16_t value)
{
	returnToMainMenu = true;
}

void ReturnToMainMenu(void)
{
	returnToMainMenu = true;
}

bool YesNoPrompt(const prog_char *prompt)
{
	bool YesNo = false;
	LCD.drawrectbyte(0, 41, 8, 40, 0);
	LCD.glcd_print2_P(0, 41, prompt, &font6x8, 1);
	while (true)
	{
		uint8_t keycode;
		TouchKeyboard::MetaKeys metaKeys;
		Point pressPt;
		while (!keyboard.getKeyCode(keycode, metaKeys, true, pressPt)) {}
		if (pressPt.z > ts.pressureThreshhold)
		{
			if (keycode == VK_Y)
			{
				YesNo = true;
				break;
			}
			else if (keycode == VK_N)
			{
				YesNo = false;
				break;
			}
		}			
	}
	LCD.drawrectbyte(0, 41, 8, 40, 0);
	return YesNo;
}

void ShowMessage_P(const prog_char* message)
{
	LCD.drawrectbyte(0, 41, 8, 40, 0);
	LCD.glcd_print2_P(0, 41, message, &font6x8, 1);
}

void ShowMessage(const char* message)
{
	LCD.drawrectbyte(0, 41, 8, 40, 0);
	LCD.glcd_print2(0, 41, message, &font6x8, 1);
}

void chronoDataButtonClick(TouchButton *button, int16_t value);
void shotCaptureButtonClick(TouchButton *button, int16_t value);
void reviewDataButtonClick(TouchButton *button, int16_t value);
void saveShotDataButtonClick(TouchButton *button, int16_t value);
void AnyKeyPress(struct EventLoop *loop, uint8_t keycode, TouchKeyboard::MetaKeys metaKeys);
void EnterKeyPress(struct EventLoop *loop, uint8_t keycode, TouchKeyboard::MetaKeys metaKeys);

void setup()
{
  Prompt prompt;
  // This set the font select to 6x8 for 40 cols x 8 rows of text
  pinMode(29, OUTPUT);
  digitalWrite(29, HIGH);
  Serial.begin(19200);
  LCD.Initialize();
  Console.initialize();
  Console.hideCaret();
//  Console.setConsoleWindow(0, 0, LCD.getTextCols() - 1, LCD.getTextRows() - 2);
  chronoScreen.init();
  ts.pressureThreshhold = 150;
  calibration.eepromLoad(KEYBOARD_CALIBRATION_DATA_ADDRESS);
  if (keyboard.Initialize(calibration, prompt, false))
  {
	  Println("Storing keyboard calibration data");
	  calibration.eepromStore(KEYBOARD_CALIBRATION_DATA_ADDRESS);
  }
  rgn.doCalibration(cal, REGION_CALIBRATION_ADDRESS, true);
  TouchButton::init(LCD);
  chronoData.init(CHRONODATA_EEPROM_ADDRESS);
  SD.begin();
  //Console.setConsoleWindow(LCD.getTextCols() / 2, 0, LCD.getTextCols() - 1, LCD.getTextRows() - 2);
  buttonMenu.init(LCD.getWidth(), LCD.getHeight(), LCD);
  buttonMenu.ArrangeButtons(ButtonMenu::Horizontal, 115, 20, 1, 1, 4, 2, mainMenu, numMainMenuItems, MainMenuButtonClick);
  buttonMenu.ArrangeButtons(ButtonMenu::Vertical, 60, 14, 5, 1, 0, 0, menu, numMenuItems, ButtonMenuClick);
  buttonMenu.ArrangeButtons(ButtonMenu::Horizontal, 55, 14, 1, 1, 0, LCD.getHeight() - 14, chronoMenu, numChronoMenuItems, MenuReturnButtonClick);
  buttonMenu.ArrangeButtons(ButtonMenu::Horizontal, 55, 14, 1, 1, 0, LCD.getHeight() - 14, chronoDataMenu, numChronoDataMenuItems, chronoDataButtonClick);
  buttonMenu.ArrangeButtons(ButtonMenu::Horizontal, 55, 14, 1, 1, 0, LCD.getHeight() - 14, shotCaptureMenu, numShotCaptureMenuItems, shotCaptureButtonClick);
  buttonMenu.ArrangeButtons(ButtonMenu::Vertical, 55, 14, 1, 1, 0, 0, reviewShotDataMenu, numReviewShotDataMenuItems, reviewDataButtonClick);
  buttonMenu.ArrangeButtons(ButtonMenu::Vertical, 55, 14, 1, 1, 0, 0, saveShotDataMenu, numSaveShotDataMenuItems, saveShotDataButtonClick);
}

void MainMenuKeyPress(struct EventLoop *loop, uint8_t keycode, TouchKeyboard::MetaKeys metaKeys)
{
	if (keycode >= VK_1 && keycode <= VK_9 && ((keycode - VK_0 - 1) < numMainMenuItems))
	{
		mainMenu[keycode - VK_0 - 1].Select();
		loop->ExitLoop();
	}
};

void ButtonMenuTouch(struct EventLoop *loop, Point tp)
{
	if (TouchButton::checkAllButtons(tp.x, tp.y))
		loop->ExitLoop();
}

float lastVoltage = 0.0;

void CheckBatteryVoltage(void)
{
	float curVoltage;
	uint16_t pinData = analogRead(VBATT); // read several time to overcome any lingering charges
	pinData = analogRead(VBATT);
	pinData = analogRead(VBATT);
	pinData = analogRead(VBATT);
	pinData = analogRead(VBATT);
	curVoltage = ((pinData * VREF) / 1023) * VRATIO; // scale voltage based on the divider
	if (abs(lastVoltage - curVoltage) >= 0.05)
	{
		char buffer[40];
		char voltStr[6];
		lastVoltage = curVoltage;
		LCD.clearText();
		if (curVoltage >= 0.1)
		{
			dtostrf(curVoltage, 5, 2, voltStr);
			LCD.TextGoTo(1, 7);
			sprintf_P(buffer, PSTR("Battery Voltage: %s"), voltStr);
			LCD.writeString(buffer);
			if (curVoltage < 8.5) 
			{
				LCD.setTextAttrMode('8'); // blink as a warning
				LCD.WriteStringPgm(PSTR(" !!LOW!!"));
				LCD.setTextAttrMode('0');
			}
		}
	}
}

void loop()
{
//	int i;
	LCD.clearGraphic();
	LCD.clearText();
	Console.clearScreen();
	TouchButton::deactivateAllButtons();
	buttonMenu.ShowButtonMenu(mainMenu, numMainMenuItems);
	while (!eventLoop.DoEvent(MainMenuKeyPress, ButtonMenuTouch))
	{
		CheckBatteryVoltage();
	}
}

void TestMenuKeyPress(struct EventLoop *loop, uint8_t keycode, TouchKeyboard::MetaKeys metaKeys)
{
	if (keycode >= VK_1 && keycode <= VK_9 && ((keycode - VK_0 - 1) < numMenuItems))
	{
		menu[keycode - VK_0 - 1].Select();
		loop->ExitLoop();
	}
};

void SubMenuTouch(struct EventLoop *loop, Point tp)
{
	if (TouchButton::checkAllButtons(tp.x, tp.y))
		loop->ExitLoop();
}

void TestMenu(void)
{
	returnToMainMenu = false;
	while (!returnToMainMenu)
	{
		LCD.clearGraphic();
		LCD.clearText();
		Console.clearScreen();
		TouchButton::deactivateAllButtons();
		buttonMenu.ShowButtonMenu(menu, numMenuItems);
		eventLoop.DoEvents(TestMenuKeyPress, SubMenuTouch);
	}
}

void TestFont(void)
{
	uint8_t keycode;
	TouchKeyboard::MetaKeys metaKeys;
	Point pressPt;
	LCD.clearText();
	LCD.clearGraphic();
	LCD.glcd_print2_P(0, 0, PSTR("2400.00"), &Arial__27, 0);
	LCD.glcd_print2_P(120, 0, PSTR("2400.00"), &Arial__27, 0);
	LCD.TextGoTo(0, 5);
	LCD.writeString("Press any key to return");
	eventLoop.DoEvents(EnterKeyPress, NULL);
	//while (!keyboard.getKeyCode(keycode, metaKeys, true, pressPt)) {};
}

void CalibrateKeyboard(void)
{
	Prompt prompt;
	Rect bounds;
	uint8_t keycode;
	TouchKeyboard::MetaKeys metaKeys;
	Point pressPt;
	LCD.clearText();
	LCD.clearGraphic();
	Console.clearScreen();
	if (keyboard.GetKeyboardCalibration(ts, bounds, prompt))
		Console.println("Obtained calibration data");
	else
		Console.println("Invalid calibration data");
	Console.println("Press any key to continue");
	while (!keyboard.getKeyCode(keycode, metaKeys, true, pressPt)) {};
}

void AnyKeyPress(struct EventLoop *loop, uint8_t keycode, TouchKeyboard::MetaKeys metaKeys)
{
	loop->ExitLoop();
}

void EnterKeyPress(struct EventLoop *loop, uint8_t keycode, TouchKeyboard::MetaKeys metaKeys)
{
	if (keycode == VK_RETURN)
	{
		returnToMainMenu = true;
		loop->ExitLoop();
	}
}

void ChronoButtonTouch(struct EventLoop *loop, Point pt)
{
	if (TouchButton::checkAllButtons(pt.x, pt.y))
		if (returnToMainMenu)
			loop->ExitLoop();
}

void CheckForKeypress(volatile bool &condition)
{
	condition = eventLoop.DoEvent(EnterKeyPress, ChronoButtonTouch);
}

void DisplayVelocities(float Stage1, float Stage2, int8_t flashCount = 1)
{
	char floatBuf[20];
	if (flashCount < 1)
		flashCount = 1;
	LCD.drawrectbyte(0, 0, 40, 40, 0);
	do
	{
		LCD.Rectangle(0, 0, 240, 40, 1);
		LCD.createLine(120, 0, 120, 39, 1);
		dtostrf(min(9999.99, Stage1), 7, 2, floatBuf);
		LCD.glcd_print2(2, 1, floatBuf, &Arial__27, 1);
		dtostrf(min(9999.99, Stage2), 7, 2, floatBuf);
		LCD.glcd_print2(122, 1, floatBuf, &Arial__27, 1);
		if (flashCount > 1)
		{
			delay(50);
			LCD.drawrectbyte(0, 0, 40, 40, 0);
			delay(50);
		}
	} while (--flashCount > 0);
}

void DisplaySeriesNumber(int8_t seriesNum)
{
	char intBuf[8];
	itoa(seriesNum, intBuf, 10);
	LCD.fillRect(225, 49, 239, 63, 0);
	LCD.glcd_print2(228, 53, intBuf, &font6x8, 1);
}

void TestChronoScreens(void)
{
	Events events;
	int timingStarted, timingFinished, keypressEvent;
	LCD.clearGraphic();
	TouchButton::deactivateAllButtons();
	DisplayVelocities(0.0, 0.0);
	timingStarted = events.AddEvent(CheckTimingStarted);
	timingFinished = events.AddEvent(CheckTimingFinished);
	keypressEvent = events.AddEvent(CheckForKeypress);
	returnToMainMenu = false;
//	LCD.clearText();
//	LCD.TextGoTo(0, 5);
//	LCD.writeString("Waiting for shot. Any key to return.");
	chronoScreen.SetScreenLEDState(true);
	delay(100);
	while (!returnToMainMenu)
	{
		LCD.clearText();
		buttonMenu.ShowButtonMenu(shotCaptureMenu, numShotCaptureMenuItems);
		events.SetEnabled(timingStarted, true);
		if (chronoScreen.StartTiming())
		{
			unsigned long timeout = (unsigned long)0xFFFFFFFF;
			while (true)
			{
				int eventID = events.WaitForAnyEvent(timeout);
				if (eventID == timingStarted)
				{
					events.SetEnabled(timingStarted, false);
					LCD.TextGoTo(30, 5);
					LCD.writeString("Timing...");
					timeout = 2000000;
				} else if (eventID == timingFinished)
				{
					float Stage1Velocity;
					float Stage2Velocity;
					LCD.TextGoTo(0, 6);
					LCD.writeString("          ");
					if (chronoScreen.GetVelocityData(Stage1Velocity, Stage2Velocity))
					{
						DisplayVelocities(Stage1Velocity, Stage2Velocity);
						break;
					}
				} else if (eventID == keypressEvent || eventID < 0)
				{
					chronoScreen.CancelTiming();
					if (eventID < 0)
					{
						LCD.TextGoTo(30, 5);
						LCD.writeString("Timeout...");
					}
					break;
				}
			}
		}
		else
		{
			chronoScreen.CancelTiming();
			Serial.println("Error starting timing...");
			break;
		}
	}
	chronoScreen.SetScreenLEDState(false);
}

void DisplaySeriesAndShot(int8_t seq, int8_t shotCount, bool invalid)
{
	char msg[80];
	if (shotCount > 0)
		sprintf(msg, "Series #%d - Shot #%d%s", seq, shotCount, invalid ? " Invalid!" : "");
	else
		msg[0] = '\0';
	ShowMessage(msg);
}

void shotCaptureButtonClick(TouchButton *button, int16_t value)
{
	ChronoData::ShotData data;
	switch (value)
	{
	case 0:
		{
			returnToMainMenu = true;
			break;
		}
	case 1:
		{
			if ((chronoData.ShotsInSequence() > 0) && YesNoPrompt(PSTR("Delete last shot? (Y/N)")))
				chronoData.DeleteShot(chronoData.ShotsInSequence() - 1);
			break;
		}
	case 2:
		{
			if ((chronoData.ShotsInSequence() > 0) && YesNoPrompt(PSTR("Reset current series? (Y/N)")))
				chronoData.ClearCurrentSequence();
			break;
		}
	case 3:
		{
			if ((chronoData.ShotsInSequence() > 0) && YesNoPrompt(PSTR("Save current series? (Y/N)")))
				chronoData.NewSequence();
			DisplaySeriesNumber(chronoData.CurrentSequence());
			break;
		}
	}
	if (chronoData.ShotsInSequence() > 0)
	{
		data = chronoData.SequenceShotData(chronoData.ShotsInSequence() - 1);
		DisplayVelocities(data.Velocity[0], data.Velocity[1], 1);
		DisplaySeriesAndShot(chronoData.CurrentSequence(), chronoData.ShotsInSequence(), false);
	} else
		ShowMessage("");
}

void CaptureShotData(void)
{
	Events events;
	int timingStarted, timingFinished, keypressEvent;
	LCD.clearGraphic();
	TouchButton::deactivateAllButtons();
	DisplayVelocities(0.0, 0.0);
	timingStarted = events.AddEvent(CheckTimingStarted);
	timingFinished = events.AddEvent(CheckTimingFinished);
	keypressEvent = events.AddEvent(CheckForKeypress);
	returnToMainMenu = false;
	buttonMenu.ShowButtonMenu(shotCaptureMenu, numShotCaptureMenuItems);
	if ((chronoData.ShotsInSequence() > 0) && (chronoData.ReadOnly() || YesNoPrompt(PSTR("Start new series? (Y/N)"))))
	{
		if (!chronoData.ReadOnly() && YesNoPrompt(PSTR("Save current series? (Y/N)")))
			chronoData.NewSequence();
		else
			chronoData.NewSequence(false);
	}
	chronoScreen.SetScreenLEDState(true);
	delay(100);
	while (!returnToMainMenu)
	{
		DisplaySeriesNumber(chronoData.CurrentSequence());
		LCD.clearText();
		events.SetEnabled(timingStarted, true);
		if (chronoScreen.StartTiming())
		{
			unsigned long timeout = (unsigned long)0xFFFFFFFF;
			while (true)
			{
				int eventID = events.WaitForAnyEvent(timeout);
				if (eventID == timingStarted)
				{
					events.SetEnabled(timingStarted, false);
					ShowMessage_P(PSTR("Timing..."));
					timeout = 2000000;
				} else if (eventID == timingFinished)
				{
					float Stage1Velocity;
					float Stage2Velocity;
					if (chronoScreen.GetVelocityData(Stage1Velocity, Stage2Velocity))
					{
						bool Invalid = (Stage1Velocity < Stage2Velocity) || 
							((Stage1Velocity <= 1000.0) && ((Stage1Velocity - Stage2Velocity) >= 42.0)) ||
							((Stage1Velocity <= 2000.0) && ((Stage1Velocity - Stage2Velocity) >= 83.0)) ||
							((Stage1Velocity <= 3000.0) && ((Stage1Velocity - Stage2Velocity) >= 125.0)) ||
							((Stage1Velocity <= 4000.0) && ((Stage1Velocity - Stage2Velocity) >= 166.0));
						DisplayVelocities(Stage1Velocity, Stage2Velocity, Invalid ? 5 : 1);
						if (chronoData.ShotsInSequence() < chronoData.MaxShotsInSequence())
							chronoData.AddShot(ChronoData::ShotData(Stage1Velocity, Stage2Velocity));
						DisplaySeriesAndShot(chronoData.CurrentSequence(), chronoData.ShotsInSequence(), Invalid);
						break;
					}
				} else if (eventID == keypressEvent || eventID < 0)
				{
					chronoScreen.CancelTiming();
					if (eventID < 0)
					{
						ShowMessage_P(PSTR("Timeout..."));
						DisplayVelocities(0.0, 0.0, 3);
					}
					break;
				}
			}
		}
		else
		{
			chronoScreen.CancelTiming();
			Serial.println("Error starting timing...");
			break;
		}
	}
	chronoScreen.SetScreenLEDState(false);
}

char GetKey(void)
{
	uint8_t keycode;
	TouchKeyboard::MetaKeys metaKeys;
	Point pressPt;
	while (true)
	{
		bool keyhit = keyboard.getKeyCode(keycode, metaKeys, true, pressPt);
		if (keyhit)
			return keyboard.ToASCII(keycode, metaKeys);
	}
}

void ShowSeqStats(void)
{
	Console.clearScreen();
	chronoData.printSeqShotTo(Console);
	chronoData.printStatsTo(Console);
}

void PrintShotData(const ChronoData::ShotData& shot, int8_t shotNum)
{
	char buffer[80];
	char V0[8];
	char V1[8];
	char Av[8];
	dtostrf(min(9999.99, shot.Velocity[0]), 7, 2, V0);
	dtostrf(min(9999.99, shot.Velocity[1]), 7, 2, V1);
	dtostrf(min(9999.99, shot.AvgVelocity()), 7, 2, Av);
	sprintf_P(buffer, PSTR("%2d %s %s %s"), shotNum + 1, V0, V1, Av);
	Console.println(buffer);
}

void reviewDataButtonClick(TouchButton *button, int16_t value)
{
	switch (value)
	{
	case 0:
		{
			returnToMainMenu = true;
			break;
		}
	case 1:
		{
			if (chronoData.SequencesCount() > 0 && (chronoData.ReadOnly() || chronoData.ShotsInSequence() == 0))
			{
				String input;
				Console.clearScreen();
				Console.print(F("Load Series("));
				Console.print(0); Console.print("-"); Console.print(chronoData.SequencesCount() - 1); Console.print(F(")"));
				input = Console.readLine(GetKey);
				if (input.length() > 0 && input[0] >= '0' && input[0] <= '9')
					chronoData.LoadSequence(input.toInt());
				ShowSeqStats();
			}
			break;
		}
	case 2:
		{
			Console.clearScreen();
			if (chronoData.ShotsInSequence() > 0)
			{
				int8_t i = 0;
				int8_t lines = 7;
				while (i < chronoData.ShotsInSequence())
				{
					if (lines == 7)
					{
						Console.println(F(" # Screen1 Screen2 Average"));
						lines--;
					}
					PrintShotData(chronoData.SequenceShotData(i), i);
					i++;
					lines--;
					if (lines == 0)
					{
						Console.print(F("-- More --"));
						eventLoop.DoEvents(AnyKeyPress, NULL);
						lines = 7;
						Console.clearScreen();
					}
				}
				Console.print(F("-- Done --"));
				eventLoop.DoEvents(AnyKeyPress, NULL);
			}
			else
			{
				Console.print(F("No shots in series."));
				eventLoop.DoEvents(AnyKeyPress, NULL);
			}
			ShowSeqStats();
			break;
		}
	case 3:
		{
			String input;
			Console.clearScreen();
			Console.print(F("Clear all data? (Y/N)"));
			input = Console.readLine(GetKey);
			input.toLowerCase();
			if (input.length() > 0 && input[0] == 'y')
			{
				Console.println();
				Console.println(F("All data will be lost."));
				Console.print(F("Proceed? (Y/N)"));
				input = Console.readLine(GetKey);
				input.toLowerCase();
				if (input.length() > 0 && input[0] == 'y')
					chronoData.ClearAllSequences();
			}
			ShowSeqStats();
			break;
		}
	}
}

void ReviewShotData(void)
{
	LCD.clearGraphic();
	LCD.clearText();
	TouchButton::deactivateAllButtons();
	buttonMenu.ShowButtonMenu(reviewShotDataMenu, numReviewShotDataMenuItems);
	Console.setConsoleWindow(10, 0, 39, 7);
	ShowSeqStats();
	returnToMainMenu = false;
	eventLoop.DoEvents(EnterKeyPress, ChronoButtonTouch);
}

void IncrementLine(Print &p, int8_t &lines, int8_t screenSize)
{
	if (screenSize > 1)
	{
		if (++lines >= screenSize)
		{
			lines = 0;
			p.print(F("-- More --"));
			eventLoop.DoEvents(AnyKeyPress, NULL);
			p.println();
		}
	}
}

void WriteShotData(ChronoData& data, Print& p, int8_t screenSize = -1)
{
	int8_t i, j;
	int8_t lines = 0;
	for (i = 0; i < data.SequencesCount(); i++)
	{
		p.print(F("Series #")); p.println(i);
		IncrementLine(p, lines, screenSize);
		for	(j = 0; j < data.ShotsInSequence(i); j++)
		{
			ChronoData::ShotData shot = data.SequenceShotData(j, i);
			p.print(shot.Velocity[0]); p.print(","); p.println(shot.Velocity[1]);
			IncrementLine(p, lines, screenSize);
		}
	}
	if (screenSize > 1)
	{
		p.print(F("-- Done --"));
		eventLoop.DoEvents(AnyKeyPress, NULL);
		p.println();
	}
}

void saveShotDataButtonClick(TouchButton *button, int16_t value)
{
	switch (value)
	{
	case 0:
		{
			returnToMainMenu = true;
			break;
		}
	case 1:
		{
			char filepath[80];
			char filename[9];
			String input;
			File file;
			Console.print(F("Enter filename: "));
			input = Console.readLine(GetKey);
			input.toCharArray(filename, 8);
			Console.println();
			if (filename[0] == '\0')
				break;
			SD.mkdir("/ShotData");
			sprintf(filepath, "/ShotData/%s.txt", filename);
			if (SD.exists(filepath))
			{
				Console.print("Overwrite "); Console.print(filename); Console.print("? (Y/N)");
				input = Console.readLine(GetKey);
				input.toLowerCase();
				Console.println();
				if (input.length() == 0 || input[0] != 'y')
					break;
				SD.remove(filepath);
			}
			Console.println(F("Saving to file"));
			Console.print(filepath);
			file = SD.open(filepath, FILE_WRITE);
			WriteShotData(chronoData, file);
			file.close();
			Console.println();
			Console.print(F("File saved."));
			eventLoop.DoEvents(AnyKeyPress, NULL);
			Console.clearScreen();
			break;
		}
	case 2:
		{
			WriteShotData(chronoData, Serial);
			break;
		}
	case 3:
		{
			WriteShotData(chronoData, Console, 7);
			Console.clearScreen();
			break;
		}
	}
}

void DownloadShotData(void)
{
	LCD.clearGraphic();
	LCD.clearText();
	TouchButton::deactivateAllButtons();
	buttonMenu.ShowButtonMenu(saveShotDataMenu, numSaveShotDataMenuItems);
	Console.setConsoleWindow(10, 0, 39, 7);
	Console.clearScreen();
	returnToMainMenu = false;
	eventLoop.DoEvents(EnterKeyPress, ChronoButtonTouch);
}

void TestEvents(void)
{
	Events events;
	int keypressEvent;
	bool condition = false;
	CheckForKeypress(condition);
	if (condition)
		return;
	LCD.clearGraphic();
	keypressEvent = events.AddEvent(CheckForKeypress);
	while (true)
	{
		LCD.clearText();
		LCD.TextGoTo(0, 5);
		LCD.writeString("Waiting for Event. Press any key .");
		chronoScreen.printTo(Serial);
		int eventID = events.WaitForAnyEvent((unsigned long)0xFFFFFFFF);
		if (eventID == keypressEvent)
			break;
	}
}

void RegionCalibration::Clear()
{ 
	_lcd->clearText(); 
	_lcd->clearGraphic();
}

uint16_t RegionCalibration::getWidth() {return 240; };

uint16_t RegionCalibration::getHeight() { return 64; };

void RegionCalibration::DrawTextPGM(uint16_t x, uint16_t y, PGM_P text) 
{
	_lcd->glcd_print2_P(x, y, text, &font6x8, 1);
}

void RegionCalibration::DrawCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color)
{
	_lcd->createCircle(x, y, radius, (color != 0) ? 1 : 0);
}

void RegionCalibration::FillCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color) {};

void TestRegion(bool recalibrate = false)
{
	uint8_t keycode;
	TouchKeyboard::MetaKeys metaKeys;
	Point pressPt;
	TouchRegion rgn = TouchRegion(ts);
	RegionCalibration cal = RegionCalibration(LCD);
	rgn.setOrientation(0);
	rgn.doCalibration(cal, REGION_CALIBRATION_ADDRESS, !recalibrate);
	LCD.clearText();
	LCD.clearGraphic();
	Console.clearScreen();
	Console.println(F("Calibration matrix"));
	Console.println(F("  a   b   c   d   e   f  div"));
	Console.print(rgn.tp_matrix.a); Console.print(F(" "));
	Console.print(rgn.tp_matrix.b); Console.print(F(" "));
	Console.print(rgn.tp_matrix.c); Console.print(F(" "));
	Console.print(rgn.tp_matrix.d); Console.print(F(" "));
	Console.print(rgn.tp_matrix.e); Console.print(F(" "));
	Console.print(rgn.tp_matrix.f); Console.print(F(" "));
	Console.println(rgn.tp_matrix.div);
	while (!keyboard.getKeyCode(keycode, metaKeys, true, pressPt)) 
	{
		if (pressPt.z > ts.pressureThreshhold)
		{
			Point tp = rgn.rawPointToRegionPoint(pressPt);
			Serial.print("tp.x: "); Serial.println(tp.x);
			Serial.print("tp.y: "); Serial.println(tp.y);
		}
	};
}

void TestTouchRegion()
{
	TestRegion();
}

void RecalibrateRegion()
{
	TestRegion(true);
}

void chronoDataButtonClick(TouchButton *button, int16_t value)
{
	switch (value)
	{
	case 0:
		{
			returnToMainMenu = true;
			break;
		}
	case 1: 
		{
			Print(F("Saving Chrono Data..."));
			if (chronoData.SaveCurrentSequence())
			{
				Println(F("Saved."));
			}
			else
			{
				Println(F("Not saved."));
			}
			break;
		}
	case 2:
		{
			uint8_t keycode;
			TouchKeyboard::MetaKeys metaKeys;
			Point pressPt;
			if (chronoData.SequencesCount() > 0 && (chronoData.ReadOnly() || chronoData.ShotsInSequence() == 0))
			{
				Print(F("Load Chrono Data...("));
				Print(0); Print("-"); Print(chronoData.SequencesCount() - 1); Print(F(")"));
				while (true)
				{
					bool keyhit = keyboard.getKeyCode(keycode, metaKeys, true, pressPt);
					if (keyhit && ((keycode >= VK_0 && keycode < (VK_0 + min(chronoData.SequencesCount(), 9))) || keycode == VK_RETURN))
					{
						Println();
						if (keycode == VK_RETURN)
							return;
						Print(F("Loading Dataset #"));
						Print(keycode - VK_0); Print(F("..."));
						chronoData.LoadSequence(keycode - VK_0);
						Println(F("Loaded."));
						Println(chronoData);
						break;
					}
				}
			}
			break;
		}
	case 3:
		{
			uint8_t keycode;
			TouchKeyboard::MetaKeys metaKeys;
			Point pressPt;
			Print(F("Clear Chrono Data? (Y/N/A)"));
			while (true)
			{
				bool keyhit = keyboard.getKeyCode(keycode, metaKeys, true, pressPt);
				if (keyhit && (keycode == VK_Y || keycode == VK_N || keycode == VK_A))
				{
					Println();
					if (keycode == VK_Y)
					{
						chronoData.ClearCurrentSequence();
						Println(F("Chrono data cleared"));
						Println(chronoData);
					}
					if (keycode == VK_A)
					{
						chronoData.ClearAllSequences();
						Println(F("All chrono data cleared"));
						Println(chronoData);
					}
					break;
				}
			}
		break;
		}
	}
}

typedef float PROGMEM prog_float;

prog_float TestShotData[] PROGMEM = {
	1234.0, 1233.0,
	1254.0, 1250.0,
	1334.0, 1333.0,
	2254.0, 2250.0,
	1634.0, 1633.0,
	1754.0, 1750.0,
	234.0, 233.0,
	254.0, 250.0,
};

void TestChronoData (void)
{
	int i = 0;
	int8_t seq;
	Console.setConsoleWindow(0, 0, LCD.getTextCols() - 1, LCD.getTextRows() - 3);
	Console.clearScreen();
	TouchButton::deactivateAllButtons();
	buttonMenu.ShowButtonMenu(chronoDataMenu, numChronoDataMenuItems);
	chronoData.ClearCurrentSequence();
	seq = chronoData.NewSequence();
	Print(F("New Shot Data: ")); Println(seq);
	while (i < sizeof(TestShotData) / sizeof(float))
	{
		ChronoData::ShotData shotData = ChronoData::ShotData(pgm_read_float(&TestShotData[i]), pgm_read_float(&TestShotData[i + 1]));
		Print(F("Adding data: ")); Print(shotData.Velocity[0]); Print(F(", ")); Print(shotData.Velocity[1]); Print(F(", ")); Println(shotData.AvgVelocity());
		chronoData.AddShot(shotData);
		i += 2;
		delay(1000);
	}
	Println(F("--Statistics--"));
	Print(F("Max Velocity: ")); Println(chronoData.SequenceMax());
	Print(F("Min Velocity: ")); Println(chronoData.SequenceMin());
	Print(F("Spead:        ")); Println(chronoData.SequenceSpread());
	Print(F("Average:      ")); Println(chronoData.SequenceAverage());
	Print(F("Deviation:    ")); Println(chronoData.SequenceDeviation());
	Print(F("Shot Count:   ")); Println(chronoData.ShotsInSequence());
	returnToMainMenu = false;
	eventLoop.DoEvents(AnyKeyPress, ChronoButtonTouch);
	buttonMenu.HideButtonMenu(chronoDataMenu, numChronoDataMenuItems);
}

// set up variables using the SD utility library functions:
//Sd2Card card;
//SdVolume volume;
//SdFile root;

//const int chipSelect = 53;    

/*
void TestSDCard(void)
{
	LCD.clearText();
	LCD.clearGraphic();
  TouchButton::deactivateAllButtons();
  Print("\nInitializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  pinMode(53, OUTPUT);     // change this to 53 on a mega


  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Println("initialization failed. Things to check:");
    Println("* is a card is inserted?");
    Println("* Is your wiring correct?");
    Println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    Println("Wiring is correct and a card is present."); 
  }

  // print the type of card
  Print("\nCard type: ");
  switch(card.type()) {
    case SD_CARD_TYPE_SD1:
      Println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Println("SDHC");
      break;
    default:
      Println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }


  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Print("\nVolume type is FAT");
  Println(volume.fatType());
  Println();
  
  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  Print("Volume size (bytes): ");
  Println(volumesize);
  Print("Volume size (Kbytes): ");
  volumesize /= 1024;
  Println(volumesize);
  Print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Println(volumesize);

  
  Println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);
  
  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
  eventLoop.DoEvents(AnyKeyPress, NULL);
}
*/