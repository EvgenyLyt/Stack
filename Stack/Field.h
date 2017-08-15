#ifndef FIELD_H
#define FIELD_H
#include "Figure.h"
#pragma comment(lib, "opengl32.lib")
#include "glut.h"
#include <Windows.h>
#include <tchar.h>
#include <time.h>
#include <vector>
#include <string>

class Field
{
	public:
		TCHAR *TitleGame = L"Stack";
		Figure *Game = new Figure();
		HDC hDC;
		Field();
		int Check();
		void GLRenderScene();
	private:
		void DrawFigure(data Data);
		void DrawEntry();
		void Print(int x, int y, std::wstring amount);
		void Redraw();
		void DrawTotal();
};

#endif