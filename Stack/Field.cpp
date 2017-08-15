#include "Field.h"

Field::Field()
{

}

void Field::DrawFigure(data Data)
{
	glColor3f(Data.R, Data.G, Data.B);
	glBegin(GL_POLYGON);
	glVertex3f(Data.West[0], Data.West[1], 0);
	glVertex3f(Data.North[0], Data.North[1], 0);
	glVertex3f(Data.East[0], Data.East[1], 0);
	glVertex3f(Data.South[0], Data.South[1], 0);
	glEnd();
	if (Game->Params->_3D)
	{
		glColor3f(0.7*Data.R, 0.7*Data.G, 0.7*Data.B);
		glBegin(GL_POLYGON);
		glVertex3f(Data.West[0], Data.West[1], 0);
		glVertex3f(Data.South[0], Data.South[1], 0);
		glVertex3f(Data.South[0], Data.South[1] - 0.05, 0);
		glVertex3f(Data.West[0], Data.West[1] - 0.05, 0);
		glEnd();
		glColor3f(0.8*Data.R, 0.8*Data.G, 0.8*Data.B);
		glBegin(GL_POLYGON);
		glVertex3f(Data.South[0], Data.South[1], 0);
		glVertex3f(Data.East[0], Data.East[1], 0);
		glVertex3f(Data.East[0], Data.East[1] - 0.05, 0);
		glVertex3f(Data.South[0], Data.South[1] - 0.05, 0);
		glEnd();
	}
}

void Field::DrawEntry()
{
	for (std::vector<data>::iterator it = Game->List.begin(); it != Game->List.end(); ++it)
		DrawFigure(*it);
}

void Field::DrawTotal()
{
	std::wstring word = std::to_wstring(Game->Params->Total);
	glMatrixMode(GL_PROJECTION);
	double *matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0, 600, 600, 0, -24, 24);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	Print(290, 50, word);
	word = L"Best: " + std::to_wstring(Game->Params->MaxTotal);
	Print(270, 80, word);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}

void Field::GLRenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawTotal();
	glLoadIdentity();
	DrawEntry();
	Game->Move();
	DrawFigure(Game->Now);
	SwapBuffers(hDC);
}

void Field::Print(int x, int y, std::wstring amount)
{
	glRasterPos2i(x, y);
	for (int i = 0; i<amount.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)amount.c_str()[i]);
}
int Field::Check()
{
	int Result = Game->Hit();
	if (Result != -1)
		Redraw();
	return Result;
}
void Field::Redraw()
{
	Game->Params->X = 0;
	Game->Params->Y = 0;
	Game->Params->Left = !Game->Params->Left;
	GLRenderScene();
	Game->List.insert(Game->List.end(), Game->Now);
	Game->Params->Total++;
	if (Game->Params->Total > 7)
	{
		for (int i = 0; i < Game->List.size(); i++)
		{
			Game->List[i].East[1] -= 0.05f;
			Game->List[i].North[1] -= 0.05f;
			Game->List[i].West[1] -= 0.05f;
			Game->List[i].South[1] -= 0.05f;
		}
	}
	Game->Start();
}
