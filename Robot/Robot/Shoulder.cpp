
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "glu32.lib")

#define WINDOW_TITLE "Robot"

bool isOrtho = true;
int Rx, Ry, Rz,Rs, RotS = 0.5;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		else if (wParam == 0x5A) //Z
			isOrtho = false;
		else if (wParam == 0x58) //X
			isOrtho = true;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

// RGB color function
inline float cc(int color)
{
	return color / 255.0f;
}

void rgb(int r, int g, int b)
{
	glColor3f(cc(r), cc(g), cc(b));
}

// Cube function
void DrawCube(float x, float y, float z, float height, float width, float depth, GLenum shape)
{
	// Front face
	glBegin(shape);
	rgb(255, 255, 255);
	glVertex3f(x, y, z);
	glVertex3f(x, y, z + depth);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x, y + height, z);
	glEnd();

	// Right face
	glBegin(shape);
	// rgb(0, 255, 0);
	glVertex3f(x, y, z);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y, z + depth);
	glVertex3f(x, y, z + depth);
	glEnd();

	// Back face
	glBegin(shape);
	// rgb(0,0,255);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y + height, z);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x + width, y, z + depth);
	glEnd();

	// Left face
	glBegin(shape);
	// rgb(0, 255, 0);
	glVertex3f(x + width, y + height, z);
	glVertex3f(x, y + height, z);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x + width, y + height, z + depth);
	glEnd();

	// Top face
	glBegin(shape);
	rgb(255, 255, 0);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x + width, y, z + depth);
	glVertex3f(x, y, z + depth);
	glEnd();

	// Bottom face
	glBegin(shape);
	// rgb(0, 255, 255);
	glVertex3f(x, y, z);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y + height, z);
	glVertex3f(x, y + height, z);
	glEnd();
}

void DrawPyramid(float x, float y, float z, float height, float width, float depth, GLenum shape)
{
	// Draw 3D Pyramid
	glBegin(shape);
	// Depth Test
	glEnable(GL_DEPTH_TEST);

	// Front face
	glVertex3f(x, y + height, z);
	glVertex3f(x, y, z);
	glVertex3f(x + width, y, z);

	// Right face
	glVertex3f(x, y + height, z);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y, z + depth);

	// Back face
	glVertex3f(x, y + height, z);
	glVertex3f(x + width, y, z + depth);
	glVertex3f(x, y, z + depth);

	// Left face
	glVertex3f(x, y + height, z);
	glVertex3f(x, y, z + depth);
	glVertex3f(x, y, z);

	// Bottom
	glVertex3f(x, y, z);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y, z + depth);
	glVertex3f(x, y, z + depth);

	glEnd();

}

void DrawSphere(float radius, int slices, int stacks, GLenum shape)
{

	GLUquadric* sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, shape);
	gluSphere(sphere, radius, slices, stacks);
	gluDeleteQuadric(sphere);

}

void DrawCylinder(float topRadius, float baseRadius, float height, int slices, int stacks)
{
	GLUquadricObj* cylinder = NULL;
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluCylinder(cylinder, baseRadius, topRadius, height, slices, stacks);
	gluDeleteQuadric(cylinder);
}

void DrawCone(int baseRadius, float height, int slices, int stacks)
{
	GLUquadricObj* cone = NULL;
	cone = gluNewQuadric();
	gluQuadricDrawStyle(cone, GLU_FILL);
	gluCylinder(cone, baseRadius, 0, height, slices, stacks);
	gluDeleteQuadric(cone);
}

void Draw1() {
	//front
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.2,0.0,0.0);
	glVertex3f(0.2,-0.2,0.0);
	glEnd();
	//line of front
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.2, 0.0, 0.0);
	glVertex3f(0.2, -0.2, 0.0);
	glEnd();

	//upper
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,0.0,-0.1);
	glVertex3f(0.2,0.0,-0.1);
	glVertex3f(0.2,0.0,0.0);
	glEnd();
	//Line of upper
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,0.0,-0.1);
	glVertex3f(0.2,0.0,-0.1);
	glVertex3f(0.2,0.0,0.0);
	glEnd();

	//right
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.2, 0.0, 0.0);
	glVertex3f(0.2, 0.0, -0.1);
	glVertex3f(0.2, -0.2, -0.1);
	glVertex3f(0.2, -0.2, 0.0);
	glEnd();
	//Line of right
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.2, 0.0, 0.0);
	glVertex3f(0.2, 0.0, -0.1);
	glVertex3f(0.2, -0.2, -0.1);
	glVertex3f(0.2, -0.2, 0.0);
	glEnd();

	//Down
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, -0.1);
	glVertex3f(0.2, -0.2, -0.1);
	glVertex3f(0.2, -0.2, 0.0);
	glEnd();
	//Line Of Down
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, -0.1);
	glVertex3f(0.2, -0.2, -0.1);
	glVertex3f(0.2, -0.2, 0.0);
	glEnd();

	//back
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0, 0.0, -0.1);
	glVertex3f(0.2, 0.0, -0.1);
	glVertex3f(0.2, -0.2, -0.1);
	glEnd();
	//Line Of back
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, 0.0, -0.1);
	glVertex3f(0.2, 0.0, -0.1);
	glVertex3f(0.2, -0.2, -0.1);
	glEnd();
}

void Draw2() {
	//Front
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.2, 0.0, 0.0);
	glVertex3f(0.8, 0.0, 0.0);
	glVertex3f(0.8, -0.2, 0.0);
	glVertex3f(0.2, -0.2, 0.0);
	glEnd();
	//Line of Front
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.2, 0.0, 0.0);
	glVertex3f(0.8, 0.0, 0.0);
	glVertex3f(0.8, -0.2, 0.0);
	glVertex3f(0.2, -0.2, 0.0);
	glEnd();

	//Upper
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.2, 0.0, 0.0);
	glVertex3f(0.8, 0.0, 0.0);
	glVertex3f(0.8, 0.0, -0.1);
	glVertex3f(0.2, 0.0, -0.1);
	glEnd();
	//Line Of Upper
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.2, 0.0, 0.0);
	glVertex3f(0.8, 0.0, 0.0);
	glVertex3f(0.8, 0.0, -0.1);
	glVertex3f(0.2, 0.0, -0.1);
	glEnd();

	//Right
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.8, 0.0, 0.0);
	glVertex3f(0.8, -0.2, 0.0);
	glVertex3f(0.8, -0.2, -0.1);
	glVertex3f(0.8, 0.0, -0.1);
	glEnd();
	//Line Of Right
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.8, 0.0, 0.0);
	glVertex3f(0.8, -0.2, 0.0);
	glVertex3f(0.8, -0.2, -0.1);
	glVertex3f(0.8, 0.0, -0.1);
	glEnd();

	//Left
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.2, 0.0, 0.0);
	glVertex3f(0.2, 0.0, -0.1);
	glVertex3f(0.2, -0.2, -0.1);
	glVertex3f(0.2, -0.2, 0.0);
	glEnd();
	//Line of Left
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.2, 0.0, 0.0);
	glVertex3f(0.2, 0.0, -0.1);
	glVertex3f(0.2, -0.2, -0.1);
	glVertex3f(0.2, -0.2, 0.0);
	glEnd();

	//Down
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.2, -0.2, 0.0);
	glVertex3f(0.8, -0.2, 0.0);
	glVertex3f(0.8, -0.2, -0.1);
	glVertex3f(0.2, -0.2, -0.1);
	glEnd();
	//Line Of Down
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.2, -0.2, 0.0);
	glVertex3f(0.8, -0.2, 0.0);
	glVertex3f(0.8, -0.2, -0.1);
	glVertex3f(0.2, -0.2, -0.1);
	glEnd();

	//Back
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.2, 0.0, -0.1);
	glVertex3f(0.8, 0.0, -0.1);
	glVertex3f(0.8, -0.2, -0.1);
	glVertex3f(0.2, -0.2, -0.1);
	glEnd();
	//Line Of Back
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.2, 0.0, -0.1);
	glVertex3f(0.8, 0.0, -0.1);
	glVertex3f(0.8, -0.2, -0.1);
	glVertex3f(0.2, -0.2, -0.1);
	glEnd();
}

void Draw3() {
	//Front
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.8, 0.0, 0.0);
	glVertex3f(0.8, -0.2, 0.0);
	glVertex3f(1.0, -0.2, 0.0);
	glEnd();
	//line of Front
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.8, 0.0, 0.0);
	glVertex3f(0.8, 0.0, 0.0);
	glVertex3f(1.0, -0.2, 0.0);
	glEnd();

	//upper
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.8, 0.0, 0.0);
	glVertex3f(0.8, 0.0, -0.1);
	glVertex3f(1.0, -0.2, -0.1);
	glVertex3f(1.0, -0.2, 0.0);
	glEnd();
	//Line of upper
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.8, 0.0, 0.0);
	glVertex3f(0.8, 0.0, -0.1);
	glVertex3f(1.0, -0.2, -0.1);
	glVertex3f(1.0, -0.2, 0.0);
	glEnd();

	//left
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.8, 0.0, 0.0);
	glVertex3f(0.8, 0.0, -0.1);
	glVertex3f(0.8, -0.2, -0.1);
	glVertex3f(0.8, -0.2, 0.0);
	glEnd();
	//Line of left
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.8, 0.0, 0.0);
	glVertex3f(0.8, 0.0, -0.1);
	glVertex3f(0.8, -0.2, -0.1);
	glVertex3f(0.8, -0.2, 0.0);
	glEnd();

	//Down
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.8, 0.0, 0.0);
	glVertex3f(0.8, 0.0, -0.1);
	glVertex3f(1.0, -0.2, -0.1);
	glVertex3f(1.0, -0.2, 0.0);
	glEnd();
	//Line Of Down
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.8, 0.0, 0.0);
	glVertex3f(0.8, 0.0, -0.1);
	glVertex3f(1.0, -0.2, -0.1);
	glVertex3f(1.0, -0.2, 0.0);
	glEnd();

	//Back
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.8, 0.0, -0.1);
	glVertex3f(0.8, -0.2, -0.1);
	glVertex3f(1.0, -0.2, -0.1);
	glEnd();
	//line of Back
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.8, 0.0, -0.1);
	glVertex3f(0.8, 0.0, -0.1);
	glVertex3f(1.0, -0.2, -0.1);
	glEnd();
}

void Draw4() {
	//Front
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.2, -0.2, 0.0);
	glVertex3f(2.0, -0.2, 0.0);
	glVertex3f(2.0, -0.3, 0.0);
	glVertex3f(0.2, -0.3, 0.0);
	glEnd();
	//Line of Front
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.2, -0.2, 0.0);
	glVertex3f(2.0, -0.2, 0.0);
	glVertex3f(2.0, -0.3, 0.0);
	glVertex3f(0.2, -0.3, 0.0);
	glEnd();

	//Upper
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.2, -0.2, 0.0);
	glVertex3f(2.0, -0.2, 0.0);
	glVertex3f(2.0, -0.2, -0.1);
	glVertex3f(0.2, -0.2, -0.1);
	glEnd();
	//Line Of Upper
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.2, -0.2, 0.0);
	glVertex3f(2.0, -0.2, 0.0);
	glVertex3f(2.0, -0.2, -0.1);
	glVertex3f(0.2, -0.2, -0.1);
	glEnd();

	//Right
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(2.0, -0.2, 0.0);
	glVertex3f(2.0, -0.3, 0.0);
	glVertex3f(2.0, -0.3, -0.1);
	glVertex3f(2.0, -0.2, -0.1);
	glEnd();
	//Line Of Right
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(2.0, -0.2, 0.0);
	glVertex3f(2.0, -0.3, 0.0);
	glVertex3f(2.0, -0.3, -0.1);
	glVertex3f(2.0, -0.2, -0.1);
	glEnd();

	//Left
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.2, -0.2, 0.0);
	glVertex3f(0.2, -0.2, -0.1);
	glVertex3f(0.2, -0.3, -0.1);
	glVertex3f(0.2, -0.3, 0.0);
	glEnd();
	//Line of Left
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.2, -0.2, 0.0);
	glVertex3f(0.2, -0.2, -0.1);
	glVertex3f(0.2, -0.3, -0.1);
	glVertex3f(0.2, -0.3, 0.0);
	glEnd();

	//Down
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.2, -0.3, 0.0);
	glVertex3f(2.0, -0.3, 0.0);
	glVertex3f(2.0, -0.3, -0.1);
	glVertex3f(0.2, -0.3, -0.1);
	glEnd();
	//Line Of Down
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.2, -0.3, 0.0);
	glVertex3f(2.0, -0.3, 0.0);
	glVertex3f(2.0, -0.3, -0.1);
	glVertex3f(0.2, -0.3, -0.1);
	glEnd();

	//Back
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.2, -0.2, -0.1);
	glVertex3f(2.0, -0.2, -0.1);
	glVertex3f(2.0, -0.3, -0.1);
	glVertex3f(0.2, -0.3, -0.1);
	glEnd();
	//Line Of Back
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.2, -0.2, -0.1);
	glVertex3f(2.0, -0.2, -0.1);
	glVertex3f(2.0, -0.3, -0.1);
	glVertex3f(0.2, -0.3, -0.1);
	glEnd();
}

void Draw5() {
	//Front
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
	glVertex3f(2.0, -0.2, 0.0);
	glVertex3f(2.0, -0.3, 0.0);
	glVertex3f(2.2, -0.3, 0.0);
	glEnd();
	//line of Front
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(2.0, -0.2, 0.0);
	glVertex3f(2.0, -0.2, 0.0);
	glVertex3f(2.2, -0.3, 0.0);
	glEnd();

	//upper
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(2.0, -0.2, 0.0);
	glVertex3f(2.0, -0.2, -0.1);
	glVertex3f(2.2, -0.3, -0.1);
	glVertex3f(2.2, -0.3, 0.0);
	glEnd();
	//Line of upper
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(2.0, -0.2, 0.0);
	glVertex3f(2.0, -0.2, -0.1);
	glVertex3f(2.2, -0.3, -0.1);
	glVertex3f(2.2, -0.3, 0.0);
	glEnd();

	//left
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(2.0, -0.2, 0.0);
	glVertex3f(2.0, -0.2, -0.1);
	glVertex3f(2.0, -0.3, -0.1);
	glVertex3f(2.0, -0.3, 0.0);
	glEnd();
	//Line of left
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(2.0, -0.2, 0.0);
	glVertex3f(2.0, -0.2, -0.1);
	glVertex3f(2.0, -0.3, -0.1);
	glVertex3f(2.0, -0.3, 0.0);
	glEnd();

	//Down
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(2.0, -0.2, 0.0);
	glVertex3f(2.0, -0.2, -0.1);
	glVertex3f(2.2, -0.3, -0.1);
	glVertex3f(2.2, -0.3, 0.0);
	glEnd();
	//Line Of Down
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(2.0, -0.2, 0.0);
	glVertex3f(2.0, -0.2, -0.1);
	glVertex3f(2.2, -0.3, -0.1);
	glVertex3f(2.2, -0.3, 0.0);
	glEnd();

	//Back
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
	glVertex3f(2.0, -0.2, -0.1);
	glVertex3f(2.0, -0.3, -0.1);
	glVertex3f(2.2, -0.3, -0.1);
	glEnd();
	//line of Back
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(2.0, -0.2, -0.1);
	glVertex3f(2.0, -0.2, -0.1);
	glVertex3f(2.2, -0.3, -0.1);
	glEnd();
}

void display()
{
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glRotatef(0.2, 1.0, 0.0, 0.0);
	Draw1();
	Draw2();
	Draw3();
	Draw4();
	Draw5();

}

void projection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (isOrtho) {
		//ortho view
		glOrtho(-2.5, 2.5, -2.5, 2.5, -2.5, 2.5);
	}
	else {
		//perspective view
		gluPerspective(20.0, 1.0, -1.0, 1.0);
		glFrustum(-10.0, 10.0, -10.0, 10.0, 1.0, 20.0);
	}
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	projection();

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------