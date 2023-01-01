
#include <iostream>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
// #include <GL/glut.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "glu32.lib")

#define WINDOW_TITLE "Robot"

float angle = 0.0f, rotationX, rotationY, rotationZ;
float ONear = -10.0, OFar = 10.0;
float PNear = 1.0, PFar = 20.0;
float Ptx = 0.0 , Pty = 0.0 , PtSpeed = 1.0;
float Pry =0.0 , PrSpeed=1.0;

bool isOrtho = true;
float tx = 0.0, ty = 0.0, tz = 0.0, tSpeed = 1.0;
float rS = 0.0,rSpeed = 1.0;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
		break;

		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE) PostQuitMessage(0);
			else if (wParam == VK_LEFT) angle -= 1.0f;
			else if (wParam == VK_RIGHT) angle += 1.0f;
			else if (wParam == 'X')
			{
				rotationX = 1.0f;
				rotationY = 0.0f;
				rotationZ = 0.0f;
			} else if (wParam == 'Y')
			{
				rotationY = 1.0f;
				rotationX = 0.0f;
				rotationZ = 0.0f;
			} else if (wParam == 'Z')
			{
				rotationZ = 1.0f;
				rotationX = 0.0f;
				rotationY = 0.0f;
			} else if (wParam == VK_SPACE)
			{
				rotationY = 1.0;
				rotationX = 0.0;
				rotationZ = 0.0;
				angle = 0.0;
				glLoadIdentity();
			}
			else if (wParam == VK_UP) {
				if (isOrtho){
					if (tz > ONear)
						tz -= tSpeed;
				}
				else {
					if (tz > PNear)
						tz -= tSpeed;
				}
			}
			else if (wParam == VK_DOWN) {
				if (isOrtho) {
					if (tz < OFar)
						tz += tSpeed;
				}
				else {
					if (tz < PFar)
						tz += tSpeed;
				}
			}
			else if (wParam == VK_LEFT) {
				if (isOrtho) {
					if (tx > ONear)
						tx -= tSpeed;
				}
				else {
					if (tx > PNear)
						tx -= tSpeed;
				}
			}
			else if (wParam == VK_RIGHT) {
				if (isOrtho) {
					if (tx < OFar)
						tx += tSpeed;
				}
				else {
					if (tx < PFar)
						tx += tSpeed;
				}
			}
			else if (wParam == 'A') {
				Ptx -= PtSpeed;
			}
			else if (wParam == 'D') {
				Ptx += PtSpeed;
			}
			else if (wParam == 'L') {
				Pry += PrSpeed;
			}
			else if (wParam == 'R') {
				Pry -= PrSpeed;
			}
			else if (wParam == 0x5A) //Z
				{
				isOrtho = false;
				tz = PNear;
				}
		else if (wParam == 0x58) //X
			{
			isOrtho = true;
			}
			
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
void DrawTriangularPrism(float x, float y, float z, float height, float width, float depth, GLenum shape)
{
	// Front
	glBegin(shape);
	glVertex3f(x, y, z);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x, y + height, z + depth);
	glEnd();
	
	// Left
	glBegin(shape);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x, y, z + depth);
	glVertex3f(x, y, z);
	glEnd();

	// Bottom
	glBegin(shape);
	glVertex3f(x, y, z);
	glVertex3f(x, y, z + depth);
	glVertex3f(x + width, y, z + depth);
	glVertex3f(x + width, y, z);
	glEnd();

	// Right
	glBegin(shape);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y, z + depth);
	glVertex3f(x + width, y + height, z + depth);
	glEnd();

	// Back
	glBegin(shape);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x, y, z + depth);
	glVertex3f(x + width, y, z + depth);
	glEnd();
}


// Cube function
void DrawCube(float x, float y, float z, float height, float width, float depth, GLenum shape)
{
	// Front face
	glBegin(shape);
	glVertex3f(x, y, z);
	glVertex3f(x, y, z + depth);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x, y + height, z);
	glEnd();

	// Right face
	glBegin(shape);
	glVertex3f(x, y, z);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y, z + depth);
	glVertex3f(x, y, z + depth);
	glEnd();

	// Back face
	glBegin(shape);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y + height, z);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x + width, y, z + depth);
	glEnd();

	// Left face
	glBegin(shape);
	glVertex3f(x + width, y + height, z);
	glVertex3f(x, y + height, z);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x + width, y + height, z + depth);
	glEnd();

	// Top face
	glBegin(shape);
	glVertex3f(x, y + height, z + depth);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x + width, y, z + depth);
	glVertex3f(x, y, z + depth);
	glEnd();

	// Bottom face
	glBegin(shape);
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
	// GLUQuadricLineWidth(cylinder, 2.0);
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

void DrawRightFoot(float centerX, float centerY, float centerZ)
{
	// Toes
	rgb(255,255,255);
	
	glPushMatrix();
		glTranslatef(centerX, centerY, centerZ);
		DrawTriangularPrism(0,0,0, 0.5,1.0,0.5,GL_LINE_LOOP);
	glPopMatrix();
	
	DrawCube(centerX, centerY, 0.55, 0.5, 1.0, 0.5, GL_LINE_LOOP);
	// left part 
	DrawCube(centerX + 1.0, centerY, 0.5, 0.7, 0.2, 0.80, GL_LINE_LOOP);
	// Right part (More thin)
	DrawCube(centerX + -0.1, centerY, 0.5, 0.7, 0.1, 0.80, GL_LINE_LOOP);
	// Behind part
	DrawCube( centerX,centerY, 1.0, 0.7, 1.0, 0.3, GL_LINE_LOOP);
	
	// Ankle
	rgb(0,0,255);
	DrawCube(centerX + -0.1,centerY + 0.7, 0.5, 3.0, 1.3, 0.8, GL_LINE_LOOP);
	// Left Leg, Tyre at left side
	rgb(255,255,0);
	glPushMatrix();
		
		// Lower Tyre
		glTranslatef(centerX + 1.0, centerY + 1.2, 0.7);
		glRotatef(90, 0, 1, 0);
		DrawCylinder(0.3, 0.3, 0.2, 20, 20);
		glPushMatrix();
			// Upper Tyre
			glTranslatef(0, 0.7, 0);
			DrawCylinder(0.3, 0.3, 0.2, 20, 20);
		glPopMatrix();
	glPopMatrix();
}

void DrawLeftFoot(float centerX, float centerY, float centerZ)
{
	// Toes
	rgb(255,255,255);
	
	glPushMatrix();
	glTranslatef(centerX, centerY, centerZ);
	DrawTriangularPrism(0,0,0, 0.5,1.0,0.5,GL_LINE_LOOP);
	glPopMatrix();
	
	DrawCube(centerX, centerY, 0.55, 0.5, 1.0, 0.5, GL_LINE_LOOP);
	// left part (More thin)
	DrawCube(centerX - 0.1,centerY, 0.5, 0.7, 0.1, 0.80, GL_LINE_LOOP);
	// Right part 
	DrawCube(centerX + 1.0,centerY, 0.5, 0.7, 0.2, 0.80, GL_LINE_LOOP);
	// Behind part
	DrawCube( centerX,centerY, 1.0, 0.7, 1.0, 0.3, GL_LINE_LOOP);
	
	// Ankle
	rgb(0,0,255);
	DrawCube(centerX + -0.1,centerY + 0.7, 0.5, 3.0, 1.3, 0.8, GL_LINE_LOOP);
	
	// Right Leg, Tyre at Right side
	rgb(255,255,0);
	glPushMatrix();
		// Lower Tyre
		glTranslatef(centerX - 0.1, centerY + 1.2, 0.7);
		glRotatef(90, 0, 1, 0);
		DrawCylinder(0.3, 0.3, 0.2, 20, 20);
		glPushMatrix();
		// Upper Tyre
			glTranslatef(0, 0.7, 0);
			DrawCylinder(0.3, 0.3, 0.2, 20, 20);
		glPopMatrix();
	glPopMatrix();
}

void display()
{
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// glMatrixMode(GL_MODELVIEW);
	
	if (angle > 360.0f)
	{
		angle = 0.0f;
	}
	
	glRotatef(angle, rotationX, rotationY, rotationZ);
	std::cout << "Angle: " << angle << std::endl;
	DrawLeftFoot(-1.5f, -5.0f, 0.0f);
	DrawRightFoot(1.5f, -5.0f, 0.0f);
	
	
	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------
}
//--------------------------------------------------------------------

void projection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glTranslatef(Ptx, Pty, 0.0);
	glRotatef(Pry, 0.0, 1.0, 0.0);
	if (isOrtho) {
		//ortho view
		glOrtho(-10.0,10.0, -10.0,10.0,ONear,OFar);
	}
	else{
		//perspective view
		gluPerspective(20.0, 1.0, -1.0, 1.0);
		glFrustum(-10.0, 10.0, -10.0, 10.0, PNear, PFar);
	}
}

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

		projection();
		display();
		
		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);
	
	return true;
}
//--------------------------------------------------------------------