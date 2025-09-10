#include "headers.h"

#include "game.h"


int main()
{
	Game game(1000, 1000, "Chess Game", PL_WHITE);
	game.run();
	return 0;
}

