#include "Figure.h"
#include <Windows.h>
#include <mmsystem.h>
Figure::Figure()
{

}

void Figure::Clear()
{
	Now.West[0] = 0;
	Now.West[1] = 0;
	Now.North[0] = 0;
	Now.North[1] = 0;
	Now.South[0] = 0;
	Now.South[1] = 0;
	Now.East[0] = 0;
	Now.East[1] = 0;
}

void Figure::Inception()
{
	data figure;
	figure.West[0] = -0.3f;
	figure.North[0] = 0;
	figure.East[0] = 0.3f;
	figure.South[0] = 0;
	for (int i = 0; i < 6; i++)
	{
		figure.R = RandColor();
		figure.G = RandColor();
		figure.B = RandColor();
		figure.West[1] = -0.5f + i*0.05f;
		figure.North[1] = -0.3f + i*0.05f;
		figure.East[1] = -0.5f + i*0.05f;
		figure.South[1] = -0.7f + i*0.05f;
		List.insert(List.end(), figure);
	}
}

float Figure::RandColor()
{
	return 0.001f*(rand() % 1000 + 1);
}

void Figure::Move()
{
	Now.West[0] += Params->X;
	Now.North[0] += Params->X;
	Now.East[0] += Params->X;
	Now.South[0] += Params->X;
	Now.West[1] += Params->Y;
	Now.North[1] += Params->Y;
	Now.East[1] += Params->Y;
	Now.South[1] += Params->Y;
}

void Figure::Start()
{
	Now = List.back();
	Now.R = RandColor();
	Now.G = RandColor();
	Now.B = RandColor();
	Now.East[1] += 0.05f;
	Now.North[1] += 0.05f;
	Now.West[1] += 0.05f;
	Now.South[1] += 0.05f;
	if (Params->Left)
	{
		Params->X = XPARAM; Params->Y = YPARAM;
		while (Now.East[0] < 1)
			Move();
	}
	else
	{
		Params->X = -XPARAM; Params->Y = YPARAM;
		while (Now.West[0] > -1)
			Move();
	}
	Params->Up = false;
}

void Figure::Sound(bool full)
{
	if (full)
		PlaySound(L"Success.wav", NULL, SND_FILENAME | SND_ASYNC);
	else
		PlaySound(L"Fail.wav", NULL, SND_FILENAME | SND_ASYNC);
}

int Figure::Hit()
{
	if (Params->Left)
	{
		if ((Params->rf(Now.West[0]) >= Params->rf(List.back().West[0])) && (Params->rf(Now.West[0]) < Params->rf(List.back().North[0])))
		{
			if (Params->Sound)
				if (Params->rf(Now.West[0]) == Params->rf(List.back().West[0]))
					Sound(true);
				else
					Sound(false);
			Now.North[0] = List.back().North[0];
			Now.North[1] = List.back().North[1] + 0.05f;
			Now.East[0] = List.back().East[0];
			Now.East[1] = List.back().East[1] + 0.05f;
			return 0;
		}
		else
		if ((Params->rf(Now.North[0]) > Params->rf(List.back().West[0])) && (Params->rf(Now.North[0]) <= Params->rf(List.back().North[0])))
		{
			if (Params->Sound)
				Sound(false);
			Now.West[0] = List.back().West[0];
			Now.West[1] = List.back().West[1] + 0.05f;
			Now.South[0] = List.back().South[0];
			Now.South[1] = List.back().South[1] + 0.05f;
			return 0;
		}
		return -1;
	}
	else
	{
		if ((Params->rf(Now.South[0]) > Params->rf(List.back().West[0])) && (Params->rf(Now.South[0]) <= Params->rf(List.back().South[0])))
		{
			if (Params->Sound)
				if (Params->rf(Now.South[0]) == Params->rf(List.back().South[0]))
					Sound(true);
				else
					Sound(false);
			Now.North[0] = List.back().North[0];
			Now.North[1] = List.back().North[1] + 0.05f;
			Now.West[0] = List.back().West[0];
			Now.West[1] = List.back().West[1] + 0.05f;
			return 1;
		}
		else
		if ((Params->rf(Now.West[0]) >= Params->rf(List.back().West[0])) && (Params->rf(Now.West[0]) < Params->rf(List.back().South[0])))
		{
			if (Params->Sound)
				Sound(false);
			Now.East[0] = List.back().East[0];
			Now.East[1] = List.back().East[1] + 0.05f;
			Now.South[0] = List.back().South[0];
			Now.South[1] = List.back().South[1] + 0.05f;
			return 1;
		}
		return -1;
	}
}