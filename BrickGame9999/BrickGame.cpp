#include "BrickGame.hpp"

#include "GSMenu.hpp"

#include "SaveManager.hpp"
#include "Global.hpp"

#include <iostream>

using namespace std;

BrickGame::BrickGame()
{
	res = new ResourceStore(scrRect);
	bgRenderer = new BackgroundRenderer(scrRect);
	scRenderer = new ScreenRenderer();
	soundPlayer = new SoundPlayer(res);

	device = new Device(soundPlayer);
	
	SaveManager::readSave(windowScale, framerateControl, *device);

	bgRenderer->setBackground(device->getCurrentBG());

	initSystem();

	configElements();
	configKeyMap();

	initDevice();

	// switch which gamestate is the startup one
#ifdef DEBUG_9999
	gameState = new GSMenu(*device);
	currentState = GS_MENU;
#else
	gameState = new GSMenu(*device);
	currentState = GS_MENU;
#endif
}

void BrickGame::run()
{
	SDL_Event ev;

	while (!quitting)
	{
		if (doReset)
		{
			doReset = false;
			gameState->nextState = GS_MENU;
			device->paused = false;
			device->reset();
		}

		gameLoopStartTicks = SDL_GetTicks();

		if (gameState->nextState != GS_NONE)
		{
			handleNextGameState(gameState->nextState);
		}

		while (SDL_PollEvent(&ev) != 0)
		{
			processEvent(ev);
		}

		if (doReset)
		{
			continue;
		}

		if (!isMinimized)
		{
			// parse game keys
			if (!device->paused)
			{
				checkKeyboardState();
				gameState->postEvents(*device);
			}
			render();
			deviceTick();
			gameLoopEndTicks = SDL_GetTicks();
		}

		updateWindowTitle();
	}

	try
	{
		l.logPartial(Logger::INFO, "Attempting to save...");
		SaveManager::writeSave(windowScale, framerateControl, *device);
		l.logLabel(Logger::StatusLabel::OK);
	}
	catch (...)
	{
		l.logLabel(Logger::StatusLabel::FAIL);
		l.log(Logger::WARN, "Reverting to default save file");
		SaveManager::defaultSave(windowScale, framerateControl, *device);
		throw string("Writing save data failed, reverting to default...");
	}
	
}

void BrickGame::updateWindowTitle()
{
	// form window title
	char title[255];

	if (isMinimized)
	{
		sprintf_s(title, "BrickGame-9999 (paused)");
	}
	else
	{
		if (gameLoopEndTicks - gameLoopStartTicks != 0)
		{
			sprintf_s(title, "BrickGame-9999 FPS:%d", int(1000.0 / (gameLoopEndTicks - gameLoopStartTicks)));
		}
		else
		{
			sprintf_s(title, "BrickGame-9999 FPS:1000+");
		}
	}

	SDL_SetWindowTitle(res->getWindow(), reinterpret_cast<char*>(&title));
}

void BrickGame::setWindowScale(int scale)
{
	SDL_SetWindowSize(res->getWindow(), res->windowSize.w * scale, res->windowSize.h * scale);
	SDL_RenderSetScale(r, static_cast<float>(scale), static_cast<float>(scale));
}

void BrickGame::setFullscreen(bool isFullscreen)
{
	this->fullscreen = isFullscreen;

	if (isFullscreen)
	{
		l.log(Logger::INFO, "Initializing fullScreen mode");

		SDL_DisplayMode displayMode;
		if (SDL_GetCurrentDisplayMode(0, &displayMode) < 0)
		{
			throw string("Failed retrieve display mode.");
		}

		SDL_SetWindowSize(this->w, displayMode.w, displayMode.h);
		SDL_SetWindowFullscreen(this->w, SDL_WINDOW_FULLSCREEN);
		l.log("Setting window size to: %dx%d (fullscreen)", displayMode.w, displayMode.h);
	}
	else
	{
		SDL_SetWindowFullscreen(this->w, 0);
		SDL_SetWindowSize(this->w, res->windowSize.w * windowScale, res->windowSize.h * windowScale);
		l.log("Setting window size to: %dx%d (windowed)", res->windowSize.w * windowScale, res->windowSize.h * windowScale);
	}
}