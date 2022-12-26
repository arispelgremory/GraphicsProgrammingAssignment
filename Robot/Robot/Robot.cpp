
#include <iostream>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "glu32.lib")

#define WINDOW_TITLE "Robot"

float angle = 0.0f;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
		break;

		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE) PostQuitMessage(0);
			else if (wParam == VK_LEFT) angle -= 0.5f;
			else if (wParam == VK_RIGHT) angle += 0.5f;
		break;

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

// Trapezium function
void DrawTriangularPrism()
{
	// Front
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.0f, 0.5f, 0.5f);
	glEnd();
	
	// Left
	glBegin(GL_LINE_LOOP);
	rgb(255, 255, 255);
	glVertex3f(0.0f, 0.5f, 0.5f);
	glVertex3f(0.0f, 0.0f, 0.5f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();

	// Bottom
	glBegin(GL_LINE_LOOP);
	rgb(0, 255, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.5f);
	glVertex3f(0.5f, 0.0f, 0.5f);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glEnd();

	// Right
	glBegin(GL_LINE_LOOP);
	rgb(0, 0, 255);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glEnd();

	// Back
	glBegin(GL_LINE_LOOP);
	rgb(255, 255, 0);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.0f, 0.5f, 0.5f);
	glVertex3f(0.0f, 0.0f, 0.5f);
	glVertex3f(0.5f, 0.0f, 0.5f);
	glEnd();
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
	glVertex3f(x,y + height,z);
	glVertex3f(x,y,z);
	glVertex3f(x + width,y,z);

	// Right face
	glVertex3f(x,y + height,z);
	glVertex3f(x + width,y,z);
	glVertex3f(x + width,y,z + depth);

	// Back face
	glVertex3f(x, y + height, z);
	glVertex3f(x + width,y,z + depth);
	glVertex3f(x,y,z + depth);

	// Left face
	glVertex3f(x,y + height,z);
	glVertex3f(x,y,z + depth);
	glVertex3f(x,y,z);

	// Bottom
	glVertex3f(x,y,z);
	glVertex3f(x + width,y,z);
	glVertex3f(x + width,y,z + depth);
	glVertex3f(x,y,z + depth);
	
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


void display()
{
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// glMatrixMode(GL_MODELVIEW);

	angle = 0.1f;
	if (angle > 360.0f)
	{
		angle = 0.0f;
	}
	
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	std::cout << "Angle: " << angle << std::endl;
	// DrawTriangularPrism();

	
	
	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------
}
//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.hInstance		= GetModuleHandle(NULL);
	wc.lpfnWndProc		= WindowProcedure;
	wc.lpszClassName	= WINDOW_TITLE;
    wc.style			= CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(	WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
								NULL, NULL, wc.hInstance, NULL );
	
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

	while(true)
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