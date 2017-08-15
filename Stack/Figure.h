#ifndef FIGURE_H
#define FIGURE_H
#include <vector>
#include "GameParam.h"
struct data
{
	float R = 0, G = 0, B = 0;
	float West[2];
	float North[2];
	float East[2];
	float South[2];
};
class Figure{
	public:
		Figure();
		std::vector <data> List;
		data Now;
		GameParametres *Params = new GameParametres();
		void Start();
		void Move();
		void Inception();
		void Clear();
		int Hit();
	private:
		void Sound(bool full);
		float RandColor();
};

#endif // !FIGURE_H
