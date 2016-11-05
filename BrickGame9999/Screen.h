#pragma once

/* Screen that holds pixel arrays, various other screen elements */

#include "Types.h"
#include "ScreenNumber.h"
#include "PixelArray.h"

class Screen
{
public:
	Screen();
	~Screen();

	void clear();

	// number displays
	ScreenNumber score;
	ScreenNumber highScore;
	ScreenNumber level;
	ScreenNumber speed;

	// pixel arrays
	PixelArray mainArray;
	PixelArray hintArray;
};