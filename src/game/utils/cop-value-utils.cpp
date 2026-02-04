#include "cop-value-utils.h"

Game::byte Game::Utils::CopValueToStars(short copValue)
{
	if (copValue < 600) return 0;
	else if (copValue < 1600) return 1;
	else if (copValue < 3000) return 2;
	else if (copValue < 5000) return 3;
	else if (copValue < 8000) return 4;
	else if (copValue < 12000) return 5;
	else return 6;
}

short Game::Utils::StarsToCopValue(byte stars)
{
	switch (stars) {
	case 0:
		return 0;
	case 1:
		return 600;
	case 2:
		return 1600;
	case 3:
		return 3000;
	case 4:
		return 5000;
	case 5:
		return 8000;
	default:
		return 12000;
	}
}

