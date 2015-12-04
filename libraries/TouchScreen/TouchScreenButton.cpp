//
// Derived from https://code.google.com/p/arduino-touch-gui/
// Modifications by Allen Bauer to work with a T6963c display - http://blog.thereadoracleatdelphi.com
// GPL v3 + changes MIT licensed
//

#include "TouchScreenButton.h"

extern const struct FONT_DEF font6x8;

// abstraction of interface
#define TOUCH_LCD_WIDTH sTheLcd->getWidth()
#define TOUCH_LCD_HEIGHT sTheLcd->getHeight()
#define FONT_WIDTH 6
#define FONT_HEIGHT 8

TouchButton * TouchButton::sListStart;
T6963 *TouchButton::sTheLcd;
uint8_t TouchButton::sDefaultTouchBorder = TOUCHBUTTON_DEFAULT_TOUCH_BORDER;
uint16_t TouchButton::sDefaultButtonColor = TOUCHBUTTON_DEFAULT_COLOR;
uint16_t TouchButton::sDefaultCaptionColor = TOUCHBUTTON_DEFAULT_CAPTION_COLOR;

TouchButton::~TouchButton() {
	// makes no sense on Arduino
}

TouchButton::TouchButton() {
	mTouchBorder = sDefaultTouchBorder;
	mButtonColor = sDefaultButtonColor;
	mCaptionColor = sDefaultCaptionColor;
	mOnlyCaption = false;
	mNextObject = sListStart;
	sListStart = this;
}

void TouchButton::setDefaultTouchBorder(const uint8_t aDefaultTouchBorder) {
	sDefaultTouchBorder = aDefaultTouchBorder;
}
void TouchButton::setDefaultButtonColor(const uint16_t aDefaultButtonColor) {
	sDefaultButtonColor = aDefaultButtonColor;
}
void TouchButton::setDefaultCaptionColor(const uint16_t aDefaultCaptionColor) {
	sDefaultCaptionColor = aDefaultCaptionColor;
}

void TouchButton::init(const T6963 &aTheLCD) {
	sTheLcd = (T6963 *)&aTheLCD;
}

/*
 * Set parameters (exept colors aund tochborder size) for touch button
 * if aWidthX == 0 render only text no background box
 */
int8_t TouchButton::initSimpleButton(const uint16_t aPositionX, const uint16_t aPositionY, const uint8_t aWidthX,
		const uint8_t aHeightY, PGM_P aCaption, const uint8_t aCaptionSize, const int16_t aValue,
		OnTouchHandler aOnTouchHandler) {
	return initButton(aPositionX, aPositionY, aWidthX, aHeightY, aCaption, aCaptionSize, sDefaultTouchBorder,
			sDefaultButtonColor, sDefaultCaptionColor, aValue, aOnTouchHandler);
}

/*
 * Set parameters for touch button
 * if aWidthX == 0 render only text no background box
 * if aCaptionSize == 0 dont render anything, just chexk touch area -> transparent button ;-)
 */
int8_t TouchButton::initButton(const uint16_t aPositionX, const uint16_t aPositionY, const uint8_t aWidthX,
		const uint8_t aHeightY, PGM_P aCaption, const uint8_t aCaptionSize, const uint8_t aTouchBorder,
		const uint16_t aButtonColor, const uint16_t aCaptionColor, const int16_t aValue,
		OnTouchHandler aOnTouchHandler) {

	mWidth = aWidthX;
	mHeight = aHeightY;
	mButtonColor = aButtonColor;
	mCaptionColor = aCaptionColor;
	mTouchBorder = aTouchBorder;
	mCaption = aCaption;
	mCaptionSize = aCaptionSize;
	if (aWidthX == 0) {
		mOnlyCaption = true;
	}
	mOnTouchHandler = aOnTouchHandler;
	mValue = aValue;
	return setPosition(aPositionX, aPositionY);
}

int8_t TouchButton::setPosition(const uint16_t aPositionX, const uint16_t aPositionY) {
	int8_t tRetValue = 0;
	mPositionX = aPositionX;
	mPositionY = aPositionY;
	if (mOnlyCaption) {
		// print only string, no enclosing box
		uint8_t tLength = 0;
		char * tPtr = (char *) mCaption;
		while (*tPtr++ != 0) {
			tLength += (FONT_WIDTH * mCaptionSize);
		}

		mPositionXRight = aPositionX + tLength - 1;
		mPositionYBottom = aPositionY + (FONT_HEIGHT * mCaptionSize) - 1;
	} else {
		mPositionXRight = aPositionX + mWidth - 1;
		mPositionYBottom = aPositionY + mHeight - 1;
	}

	// check values
	if (mPositionXRight > TOUCH_LCD_WIDTH) {
		mPositionXRight = TOUCH_LCD_WIDTH - 1;
		tRetValue = TOUCHBUTTON_ERROR_X_RIGHT;
	}
	if (mPositionYBottom >= TOUCH_LCD_HEIGHT) {
		mPositionYBottom = TOUCH_LCD_HEIGHT - 1;
		tRetValue = TOUCHBUTTON_ERROR_Y_BOTTOM;
	}
	return tRetValue;
}
/*
 * renders the button on lcd
 */
int8_t TouchButton::drawButton() {
	mIsActive = true;

	int8_t tRetValue = 0;
	if (mOnlyCaption)
	{
		// print only string, no enclosing box
		sTheLcd->glcd_print2_P(mPositionX, mPositionY, (PGM_P) mCaption, &font6x8, 0);
	} else if (mCaptionSize > 0) 
	{ // dont render anything if caption size == 0
		// Draw rect
		uint16_t tXCaptionPosition;
		uint16_t tYCaptionPosition;
		if (mButtonColor == mCaptionColor)
			sTheLcd->Rectangle(mPositionX, mPositionY, mWidth, mHeight, mButtonColor);
		else
			sTheLcd->fillRect(mPositionX, mPositionY, mPositionXRight, mPositionYBottom, mButtonColor);

		if (mCaption != NULL) {
			// try to position the string in the middle of the box
			uint8_t tLength = 0;
			PGM_P tPtr = mCaption;
			while (pgm_read_byte(tPtr++) != 0) {
				tLength += (FONT_WIDTH * mCaptionSize);
			}
			if (mPositionX + tLength >= mPositionXRight) { // unsigned arithmetic
				// String too long here
				tXCaptionPosition = mPositionX;
				tRetValue = TOUCHBUTTON_ERROR_CAPTION_TOO_LONG;
			} else {
				tXCaptionPosition = mPositionX + 1 + ((mWidth - tLength) / 2);
			}

			if (mPositionY + (FONT_HEIGHT * mCaptionSize) >= mPositionYBottom) { // unsigned arithmetic
				// Font height to big
				tYCaptionPosition = mPositionY;
				tRetValue = TOUCHBUTTON_ERROR_CAPTION_TOO_HIGH;
			} else {
				tYCaptionPosition = mPositionY + 1 + ((mHeight - (FONT_HEIGHT * mCaptionSize)) / 2);
			}
			sTheLcd->glcd_print2_P(tXCaptionPosition, tYCaptionPosition, (PGM_P) mCaption, &font6x8, 1);
		}
	}
	return tRetValue;
}

/*
 * Check if touch event is in button area
 * if yes - call callback function and return true
 * if no - return false
 */
bool TouchButton::checkButton(const uint16_t aTouchPositionX, const uint16_t aTouchPositionY) {
	uint16_t tPositionBorderX = mPositionX - mTouchBorder;
	if (mTouchBorder > mPositionX) {
		tPositionBorderX = 0;
	}
	uint16_t tPositionBorderY = mPositionY - mTouchBorder;
	if (mTouchBorder > mPositionY) {
		tPositionBorderY = 0;
	}
	if (!mIsActive || aTouchPositionX < tPositionBorderX || aTouchPositionX > mPositionXRight + mTouchBorder
			|| aTouchPositionY < tPositionBorderY || aTouchPositionY > mPositionYBottom + mTouchBorder) {
		return false;
	}
	/*
	 *  Touch position is in button - call callback function
	 */
	if (mOnTouchHandler != NULL) {
		mOnTouchHandler(this, mValue);
	}
	return true;
}

/*
 * Static convenience method - checks all buttons for matching touch position.
 */
bool TouchButton::checkAllButtons(const uint16_t aTouchPositionX, const uint16_t aTouchPositionY) {
	TouchButton * tObjectPointer;
	for (tObjectPointer = sListStart; tObjectPointer != NULL; tObjectPointer = tObjectPointer->mNextObject)
	{
		if (tObjectPointer->checkButton(aTouchPositionX, aTouchPositionY)) 
			return true;
	}
	return false;
}

/*
 * Static convenience method - deactivate all buttons (e.g. before switching screen)
 */
void TouchButton::deactivateAllButtons() {
	TouchButton * tObjectPointer;
	for (tObjectPointer = sListStart; tObjectPointer != NULL; tObjectPointer = tObjectPointer->mNextObject)
		tObjectPointer->deactivate();
}

# ifdef DEBUG
/*
 * for debug purposes
 * needs char aStringBuffer[23+<CaptionLength>]
 */
void TouchButton::toString(char * aStringBuffer) const {
	sprintf(aStringBuffer, "X=%03u Y=%03u X1=%03u Y1=%03u B=%02u %s", mPositionX, mPositionY, mPositionXRight,
			mPositionYBottom, mTouchBorder, mCaption);
}
# endif

PGM_P TouchButton::getCaption() const {
	return mCaption;
}

/*
 * Set caption and redraws button
 */
void TouchButton::setCaption(PGM_P aCaption) {
	mCaption = aCaption;
}

/*
 * changes box color and redraws button
 */
void TouchButton::setColor(const uint16_t aColor) {
	mButtonColor = aColor;
}

void TouchButton::setCaptionColor(const uint16_t aColor) {
	mCaptionColor = aColor;
}

void TouchButton::setValue(const int16_t aValue) {
	mValue = aValue;

}
uint16_t TouchButton::getPositionX() const {
	return mPositionX;
}

uint16_t TouchButton::getPositionY() const {
	return mPositionY;
}
uint16_t TouchButton::getPositionXRight() const {
	return mPositionXRight;
}

uint16_t TouchButton::getPositionYBottom() const {
	return mPositionYBottom;
}

/*
 * activate for touch checking
 */
void TouchButton::activate() {
	mIsActive = true;
}

/*
 * deactivate for touch checking
 */
void TouchButton::deactivate() {
	mIsActive = false;
}

void TouchButton::setTouchBorder(uint8_t touchBorder) {
	mTouchBorder = touchBorder;
}

