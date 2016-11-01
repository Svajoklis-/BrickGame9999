#include "BrickGame.h"

#include <SDL_image.h>
#include <SDL_mixer.h>

void BrickGame::initSystem()
{
	l.logPartial(Logger::INFO, "SDL Init");

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		l.logLabel(Logger::FAIL);
		throw string("Could not initialize SDL.\nSDL_Error: " + string(SDL_GetError()));
	}

	l.logLabel(Logger::OK);

	l.logPartial(Logger::INFO, "IMG Init");

	if (IMG_Init(IMG_INIT_PNG) < 0)
	{
		l.logLabel(Logger::FAIL);
		throw string("Failed to init SDL_Image.");
	}

	l.logLabel(Logger::OK);

	l.logPartial(Logger::INFO, "MIX Init");

	if (Mix_Init(0) < 0)
	{
		l.logLabel(Logger::FAIL);
		throw string("Failed to initialize SDL_Mixer.");
	}

	l.logLabel(Logger::OK);

	l.logPartial(Logger::INFO, "Mix_OpenAudio");

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 1024) < 0)
	{
		l.logLabel(Logger::FAIL);
		throw string("Failed to open audio for SDL_Mixer.");
	}

	l.logLabel(Logger::OK);

	l.logPartial(Logger::INFO, "Creating SDL window...");

	w = SDL_CreateWindow("9999-in-1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, res->windowSize.w * windowScale, res->windowSize.h * windowScale, SDL_WINDOW_SHOWN);
	if (!w)
	{
		l.logLabel(Logger::FAIL);
		throw string("Failed to create SDL window.");
	}

	l.logLabel(Logger::OK);

	l.logPartial(Logger::INFO, "Creating SDL renderer... ");

	if (framerateControl == FRC_VSYNC)
	{
		l.logContinue("(vsynced)");
		r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		SDL_RenderSetScale(r, static_cast<float>(windowScale), static_cast<float>(windowScale));
	}
	else
	{
		l.logContinue("(non-vsynced)");
		r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
		SDL_RenderSetScale(r, static_cast<float>(windowScale), static_cast<float>(windowScale));
	}

	l.logLabel(Logger::OK);

	l.logPartial(Logger::INFO, "Loading icon...");

	icon = IMG_Load("Resources\\icon.png");

	if (icon == nullptr)
	{
		l.logLabel(Logger::FAIL);
		throw string("Unable to load image icon.png.\nSDL_image Error: ") + IMG_GetError();
	}

	l.logLabel(Logger::OK);

	SDL_SetWindowIcon(w, icon);

	if (!r)
	{
		throw string("Failed to create SDL renderer.");
	}

	res->setWindow(w);
	res->setRenderer(r);

	l.log(Logger::INFO, "Initialization complete.");
}

void BrickGame::deinitSystem()
{
	IMG_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
}

void BrickGame::initDevice()
{
	device->screen.highScore.dash();
	device->screen.score.dash();
	device->screen.level.setLink(&device->stage);
	device->screen.speed.setLink(&device->stage);
}

BrickGame::~BrickGame()
{
	delete res;
	delete bgRenderer;
	delete scRenderer;
	delete soundPlayer;

	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);

	deinitSystem();
}