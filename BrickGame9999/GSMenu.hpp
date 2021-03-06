#pragma once

#include "GameState.hpp"
#include "Ticker.hpp"

#include <string>
#include "Explosion.hpp"

using namespace std;

namespace GSMenu
{
	class State : public GameState
	{
	public:
		State(Device &dev);
		~State();
		void tick(Device &dev) override;
		void parseEvent(Device &dev, Key k, KeyState state) override;
		void render(Device &dev) override;

		GameStates getSelectedState();

	private:

		enum letters
		{
			A,
			B,
			C,
			D,
			R,
			LETTER_COUNT
		};

		char letterChars[5] = { 'A', 'B', 'C', 'D', 'R' };

		void defineGraphics();

		int stage = 0;

		// numbers
		string numbers[10];
		int numCount = 10;

		map<int, string[4]> anims;

		const int numberH = 5;
		const int numberW = 3;
		const int numberX = 1;
		const int numberY = 15;

		static int currentL;

		void drawNumber(Device &dev);

		// letter related stuffs
		string letters[LETTER_COUNT];

		const int letterX = 2;
		const int letterY = 0;
		const int letterCount = LETTER_COUNT;
		const int letterW = 6;
		const int letterH = 5;

		void drawLetter(Device &dev);
		void renderAnim(Device &dev);
		void resetAnim();

		Ticker letterTurnDelay;
		Ticker letterTurn;

		Ticker animTicker;
		int animFrame = 0;
		int animW = 8, animH = 8;
	};
}
