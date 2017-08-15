#ifndef GAME_PARAM_H
#define GAME_PARAM_H

#define XPARAM 0.3f/12
#define YPARAM 0.2f/12

class GameParametres
{
	public:
		GameParametres();
		float rf(float digit);
		int Speed();
		void NewGame();
		float X;
		float Y;
		int Total;
		int MaxTotal;
		bool Up;
		bool Left;
		bool Game;
		bool Sound;
		bool _3D;
};

#endif