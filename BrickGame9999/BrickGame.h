#pragma once

#include <SDL.h>
#include <SDL_image.h>

// main class of the simulator

#include "ResourceStore.h"
#include "BackgroundRenderer.h"
#include "Screen.h"
#include "ScreenRenderer.h"
#include "Device.h"
#include "Global.h"

#include "GameState.h"
#include "GSRain.h"
#include "GSMenu.h"
#include "GSSnake.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace pt = boost::property_tree;

class BrickGame
{
public:
	BrickGame();
	~BrickGame();
	void run();
private:
	enum frControl
	{
		FRC_NONE,
		FRC_250,
		FRC_VSYNC
	};

	ResourceStore *res;

	SDL_Window *w;
	SDL_Renderer *r;

	SDL_Surface *icon;

	BackgroundRenderer *bgRenderer;
	ScreenRenderer *scRenderer;

	Device device;

	GameState *gameState = nullptr;
	GameStates currentState;

	int windowScale = 1;
	void setWindowScale(int scale);

	void readSave();
	void defaultSave();
	void writeSave();

	const unsigned int magicVal = 0xFFFFFFFF;
	unsigned int calcMagic();

	// 0 - nothing, 1 - limit to 250, 2 - vsync
	int framerateControl = 2;
};