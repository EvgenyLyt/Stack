#include "GameParam.h"
#include <math.h>
GameParametres::GameParametres()
{
	X = 0;
	Y = 0;
	Up = true;
	Left = true;
	Game = false;
	Total = 0;
	MaxTotal = 0;
	Sound = true;
	_3D = true;
}

float GameParametres::rf(float digit)
{
	return round(digit * 1000) / 1000.0f;
}

int GameParametres::Speed()
{
	if (Total / 10 < 25)
		return 25 - Total / 10;
	else
		return 1;
}

void GameParametres::NewGame()
{
	if (Total > MaxTotal) MaxTotal = Total;
	X = 0;
	Y = 0;
	Game = false;
	Total = 0;
	Left = true;
	Up = true;
}
