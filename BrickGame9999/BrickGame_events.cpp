#include "BrickGame.hpp"
#include "GSMenu.hpp"

void BrickGame::processEvent(SDL_Event &ev)
{
	switch (ev.type)
	{
	case SDL_QUIT:
		quitting = true;
		break;
	case SDL_KEYDOWN:
		if (altDown)
		{
			if (ev.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				this->setFullscreen(!this->fullscreen);
			}
			break;
		}

		switch (ev.key.keysym.scancode)
		{
		case SDL_SCANCODE_F8:
			device->nextBG();
			bgRenderer->setBackground(device->getCurrentBG());
			break;

		case SDL_SCANCODE_F7:
			device->prevBG();
			bgRenderer->setBackground(device->getCurrentBG());
			break;

		case SDL_SCANCODE_M:
			device->speaker.setMuted(!device->speaker.isMuted());
			break;

		case SDL_SCANCODE_KP_PLUS:
			if (this->fullscreen)
			{
				break;
			}
			windowScale++;
			setWindowScale(windowScale);
			break;

		case SDL_SCANCODE_KP_MINUS:
			if (this->fullscreen)
			{
				break;
			}

			if (windowScale > 1)
			{
				windowScale--;
			}
			setWindowScale(windowScale);
			break;

		case SDL_SCANCODE_LALT:
			this->altDown = true;
			break;

		default: 
			if (ev.key.keysym.scancode == keyMap[KEY_RESET])
			{
				doReset = true;
			}
			else if (ev.key.keysym.scancode == keyMap[KEY_START])
			{
				if (currentState == GS_MENU)
				{
					gameState->nextState = static_cast<GSMenu::State*>(gameState)->getSelectedState();
				}
				else
				{
					if (device->inGame && device->pauseable)
					{
						device->paused = !device->paused;
					}
				}
			};
		}

		break;

	case SDL_KEYUP:
		switch (ev.key.keysym.scancode)
		{
		case SDL_SCANCODE_LALT:
			this->altDown = false;
			break;

		default: break;
		}

		break;

	case SDL_WINDOWEVENT:
		switch (ev.window.event)
		{
		case SDL_WINDOWEVENT_MINIMIZED:
			isMinimized = true;
			break;
		case SDL_WINDOWEVENT_RESTORED:
			isMinimized = false;
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			previousPauseState = device->paused;
			devicePausedExternally = true;
			device->paused = true;
			break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			if (devicePausedExternally)
			{
				devicePausedExternally = false;
				device->paused = previousPauseState;
			}

		default: break;
		}

	default: break;
	}
}

void BrickGame::checkKeyboardState()
{
	const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);

	for (int i = KEY_UP; i <= KEY_ACTION; i++)
	{
		if (keyboardState[keyMap[i]] != prevKeyboardState[i])
		{
			gameState->parseEvent(*device, static_cast<Key>(i), keyboardState[keyMap[i]] == 0 ? STATE_UP : STATE_DOWN);
		}
		else if (keyboardState[keyMap[i]] != 0)
		{
			gameState->parseEvent(*device, static_cast<Key>(i), STATE_HELD);
		}

		prevKeyboardState[i] = keyboardState[keyMap[i]];
	}
}