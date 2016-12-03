#include <iostream>
#include <Windows.h>

using namespace std;

#include "BrickGame.hpp"
#include "Logger.hpp"
#include "Global.hpp"

// main file for the program, runs Tetris
// catches all string exceptions
// catches all exceptions when not in debug

int main(int argc, char* argv[])
{
	try
	{
		BrickGame b;
		b.run();
	}
	catch (string ex)
	{
		l.log(Logger::ERR, ex.c_str());
		MessageBox(nullptr, ex.c_str(), "Program failed", MB_ICONERROR | MB_OK);
	}
#ifndef DEBUG_9999
	catch (...)
	{
		MessageBox(nullptr, "Uncaught exception", "Program failed", MB_ICONERROR | MB_OK);
	}
#endif

	l.log(Logger::INFO, "Brick Game ended its run.");
	printf("Press any key to continue...");
	getchar();

	return 0;
}