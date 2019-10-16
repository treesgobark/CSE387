#pragma once

#include "Game.h"
#include "MathLibsConstsFuncs.h"

int main(int argc, char** argv)
{
	Game game;
	bool success = game.initialize();

	if (success) {
		game.gameLoop();
	}

	return 0;

}

