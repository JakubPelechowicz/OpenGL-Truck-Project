#define  _CRT_SECURE_NO_WARNINGS

#ifdef _MSC_VER                         // Check if MS Visual C compiler
#  pragma comment(lib, "opengl32.lib")  // Compiler-specific directive to avoid manually configuration
#  pragma comment(lib, "glu32.lib")     // Link libraries
#endif

#ifdef _MSC_VER        // Check if MS Visual C compiler
#   ifndef _MBCS
#      define _MBCS    // Uses Multi-byte character set
#   endif
#   ifdef _UNICODE     // Not using Unicode character set
#      undef _UNICODE 
#   endif
#   ifdef UNICODE
#      undef UNICODE 
#   endif
#endif
#include <vector>
#include <windows.h>            // Window defines
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
#include <math.h>				// Define for sqrt
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "resource.h"           // About box resource identifiers.

struct vek3
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};
struct vek2
{
	GLfloat x;
	GLfloat y;
};
struct triangle
{
	int x1;
	int x2;
	int x3;
};
struct face
{
	triangle v;
	triangle vt;
};
struct object
{
	std::vector <vek3> vertices;
	std::vector <vek2> verticesT;
	std::vector <face> faces;
};
class UFO
{
private:
	vek3 pos;
	vek3 rotation;
	GLfloat ringrot;
	vek3 facing;
	GLfloat friction;
public:
	UFO();
	void UFO::Render();
	void move(float x, float y, float z);
	vek3 getpos();
	void Turn(GLfloat t);
	void RotateRing(float g);
	void UpdatePhys();
	void fly(float x);
	GLfloat skret;
	GLfloat ped;
};
#define glRGB(x, y, z)	glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)
#define BITMAP_ID 0x4D42		// identyfikator formatu BMP
#define GL_PI 3.141
#define ilosc_asteroid 100

object uforing;
object ufocab;
object stone;
object skybox;

std::vector <vek3> asteroidy;
std::vector <float> scale;

UFO ufo = UFO();
vek3 position;
HPALETTE hPalette = NULL;

static LPCTSTR lpszAppName = "GL Template";
static HINSTANCE hInstance;



static GLfloat xRot = 45.0f;
static GLfloat yRot = 90.0f;

static GLfloat xMove = 0.0f;
static GLfloat yMove = 0.8f;

static GLsizei lastHeight;
static GLsizei lastWidth;

class KeyboardController
{
public:
	bool up, down, left, right;
	bool w, a, s, d;
	bool e, q;
	void Update();
	KeyboardController();
};
void KeyboardController::Update()
{
	if (up)
	{
		yMove -= 0.1f;
	}
	if (down)
	{
		yMove += 0.1f;
	}
	if (left)
	{
		xMove -= 0.1f;
	}
	if (right)
	{
		xMove += 0.1f;
	}
	if (w)
	{
		ufo.fly(0.2);
	}
	if (a)
	{
		ufo.Turn(2);
	}
	if (s)
	{
		ufo.fly(-0.2);
	}
	if (d)
	{
		ufo.Turn(-2);
	}
	if (e)
	{
		ufo.move(0, 2, 0);
	}
	if (q)
	{
		ufo.move(0, -2, 0);
	}
}
KeyboardController::KeyboardController()
{
	up =
	down =
	left =
	right =
	w =
	a =
	s =
	d =
	e =
	q =
	false;
}
KeyboardController keyboard;

BITMAPINFOHEADER	bitmapInfoHeader;	// nag≥Ûwek obrazu
unsigned char*		bitmapData;			// dane tekstury
unsigned int		texture[10];			// obiekt tekstury

LRESULT CALLBACK WndProc(   HWND    hWnd,
							UINT    message,
							WPARAM  wParam,
							LPARAM  lParam);
object ImportObject(const char* filename)
{
	FILE* file = fopen(filename, "r");
	object newobject;
	while (1)
	{

		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) break;
		if (strcmp(lineHeader, "v") == 0)
		{
			vek3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			newobject.vertices.push_back(vertex);
		}
		if (strcmp(lineHeader, "f") == 0)
		{
			int tmp;
			face tempface;
			fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &tempface.v.x1, &tempface.vt.x1, &tmp, &tempface.v.x2, &tempface.vt.x2, &tmp, &tempface.v.x3, &tempface.vt.x3, &tmp);
			newobject.faces.push_back(tempface);
		}
		if (strcmp(lineHeader, "vt") == 0)
		{
			vek2 vertex;
			fscanf(file, "%f %f\n", &vertex.x, &vertex.y);
			newobject.verticesT.push_back(vertex);
		}
	}
	fclose(file);
	return newobject;
}
void RenderObject(GLfloat offx, GLfloat offy, GLfloat offz, GLfloat scale, object o, int texture_id)
{
	for (int i = 0; i < o.faces.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, texture[texture_id]);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_TRIANGLES);
		vek3 t1[3] = { o.vertices.at((o.faces.at(i).v.x1) - 1), o.vertices.at((o.faces.at(i).v.x2) - 1),  o.vertices.at((o.faces.at(i).v.x3) - 1) };
		GLfloat x1[3] = { t1[0].x * scale + offx, (t1[0].y * scale) + offy ,t1[0].z * scale + offz };
		GLfloat x2[3] = { t1[1].x * scale + offx, (t1[1].y * scale) + offy ,t1[1].z * scale + offz };
		GLfloat x3[3] = { t1[2].x * scale + offx, (t1[2].y * scale) + offy,t1[2].z * scale + offz };
		vek2 tex[3] = { o.verticesT.at((o.faces.at(i).vt.x1) - 1),o.verticesT.at((o.faces.at(i).vt.x2) - 1),o.verticesT.at((o.faces.at(i).vt.x3) - 1) };
		GLdouble tex1[2] = { tex[0].x,tex[0].y };
		GLdouble tex2[2] = { tex[1].x,tex[1].y };
		GLdouble tex3[2] = { tex[2].x,tex[2].y };

		glTexCoord2d(tex1[0], tex1[1]); glVertex3d(x1[0], x1[1], x1[2]);
		glTexCoord2d(tex2[0], tex2[1]); glVertex3d(x2[0], x2[1], x2[2]);
		glTexCoord2d(tex3[0], tex3[1]); glVertex3d(x3[0], x3[1], x3[2]);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}
void UFO::move(float x, float y, float z)
{
	pos = { pos.x + x,pos.y + y,pos.z + z };
}
void UFO::fly(float x) {
	
	if (ped < 4 && x>0)
		ped += x;
	if (ped > -4 && x < 0)
		ped += x;
	
}
void UFO::UpdatePhys()
{
	move(ped * facing.x, ped * facing.y, ped * facing.z);
	if (ped > 0)
		ped -= friction;
	if (ped < 0)
		ped += friction;
	if (ped <0.1 && ped>-0.1)
		ped = 0;
	if (skret > 360)
		skret = skret - 360;
	if (skret < -360)
		skret = skret + 360;
	ufo.RotateRing(0.2);
	facing.x = cos(skret * 0.017453);//0.017 pi/180 zamiana stopni na radiany
	facing.z = sin(skret * 0.017453);
}
void UFO::RotateRing(float g)
{
	ringrot -= g * 45;
	ringrot = GLfloat(int(ringrot)% 360);
}
vek3 UFO::getpos()
{
	return { pos.x,pos.y,pos.z };
}
void UFO::Render()
{
	glPushMatrix();
	RenderObject(0, 0, 0, 10.0f, ufocab, 0);
	glRotatef(ringrot, 0, 1, 0);
	RenderObject(0, 0, 0, 10.0f, uforing, 1);
	glPopMatrix();
}
void UFO::Turn(GLfloat t)
{
	skret -= t;
	if (skret > 360)
		skret = skret - 360;
	if (skret < -360)
		skret = skret + 360;
}
UFO::UFO()
{
	pos = { 0,0,0 };
	rotation = { 0,0,0 };
	ringrot = 0.0f;
	skret = 0.0f;
	friction = 0.05f;
	facing = { 1, 0, 0 };
}

void Update()
{
	ufo.UpdatePhys();
	keyboard.Update();
}
void SetDCPixelFormat(HDC hDC);

void ReduceToUnit(float vector[3])
	{
	float length;
	
	// Calculate the length of the vector		
	length = (float)sqrt((vector[0]*vector[0]) + 
						(vector[1]*vector[1]) +
						(vector[2]*vector[2]));

	// Keep the program from blowing up by providing an exceptable
	// value for vectors that may calculated too close to zero.
	if(length == 0.0f)
		length = 1.0f;

	// Dividing each element by the length will result in a
	// unit normal vector.
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
	}

void calcNormal(float v[3][3], float out[3])
	{
	float v1[3],v2[3];
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;

	// Calculate two vectors from the three points
	v1[x] = v[0][x] - v[1][x];
	v1[y] = v[0][y] - v[1][y];
	v1[z] = v[0][z] - v[1][z];

	v2[x] = v[1][x] - v[2][x];
	v2[y] = v[1][y] - v[2][y];
	v2[z] = v[1][z] - v[2][z];

	// Take the cross product of the two vectors to get
	// the normal vector which will be stored in out
	out[x] = v1[y]*v2[z] - v1[z]*v2[y];
	out[y] = v1[z]*v2[x] - v1[x]*v2[z];
	out[z] = v1[x]*v2[y] - v1[y]*v2[x];

	// Normalize the vector (shorten length to one)
	ReduceToUnit(out);
	}

void ChangeSize(GLsizei w, GLsizei h)
	{
	GLfloat nRange = 100.0f;
	GLfloat fAspect;
	// Prevent a divide by zero
	if(h == 0)
		h = 1;

	lastWidth = w;
	lastHeight = h;
		
	fAspect=(GLfloat)w/(GLfloat)h;
	// Set Viewport to window dimensions
    glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, fAspect, 10.0, 10000);
	gluLookAt(position.x + 100, position.y+100, position.z, position.x, position.y, position.z, 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	}

void SetupRC()
	{
	// Light values and coordinates
	//GLfloat  ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	//GLfloat  diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	//GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	//GLfloat	 lightPos[] = { 0.0f, 150.0f, 150.0f, 1.0f };
	//GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };


	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
	//glEnable(GL_CULL_FACE);		// Do not calculate inside of jet

	// Enable lighting
	//glEnable(GL_LIGHTING);

	// Setup and enable light 0
	//glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	//glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	//glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	//glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	//glEnable(GL_LIGHT0);

	// Enable color tracking
	//glEnable(GL_COLOR_MATERIAL);
	
	// Set Material properties to follow glColor values
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	//glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
	//glMateriali(GL_FRONT,GL_SHININESS,128);


	// White background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f );
	// Black brush
	glColor3f(0.0,0.0,0.0);
	}
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;							// wskaünik pozycji pliku
	BITMAPFILEHEADER	bitmapFileHeader;		// nag≥Ûwek pliku
	unsigned char		*bitmapImage;			// dane obrazu
	int					imageIdx = 0;		// licznik pikseli
	unsigned char		tempRGB;				// zmienna zamiany sk≥adowych

	// otwiera plik w trybie "read binary"
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// wczytuje nag≥Ûwek pliku
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	
	// sprawdza, czy jest to plik formatu BMP
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	// wczytuje nag≥Ûwek obrazu
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// ustawia wskaünik pozycji pliku na poczπtku danych obrazu
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// przydziela pamiÍÊ buforowi obrazu
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

	// sprawdza, czy uda≥o siÍ przydzieliÊ pamiÍÊ
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// wczytuje dane obrazu
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

	// sprawdza, czy dane zosta≥y wczytane
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	// zamienia miejscami sk≥adowe R i B 
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	// zamyka plik i zwraca wskaünik bufora zawierajπcego wczytany obraz
	fclose(filePtr);
	return bitmapImage;
}

void RenderScene(void)
	{

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Save the matrix state and do the rotations
	glPushMatrix();

	glRotatef(xRot, 0.0f, 0.0f, 1.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	
	gluLookAt(position.x - 30*sin(xMove), position.y+30*sin(yMove), position.z + 30*cos(xMove), position.x, position.y, position.z, 0, 1, 0);
	/////////////////////////////////////////////////////////////////
	// MIEJSCE NA KOD OPENGL DO TWORZENIA WLASNYCH SCEN:		   //
	/////////////////////////////////////////////////////////////////
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < ilosc_asteroid; i++)
	{
		glPushMatrix();
		glRotatef(scale.at(i)*21.3,0.5,1,0.5);
		RenderObject(asteroidy.at(i).x, asteroidy.at(i).y, asteroidy.at(i).z,scale.at(i), stone, 2);
		glPopMatrix();
	}


	position = ufo.getpos();
	
	glTranslatef(position.x,position.y,position.z);
	RenderObject(0, 0, 0, 1000, skybox, 3);
	glRotatef(-ufo.skret, 0, 1, 0);
	glRotatef(-ufo.ped*7, 0, 0, 1);
	ufo.Render();

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	// Flush drawing commands
	glFlush();
	}

void SetDCPixelFormat(HDC hDC)
	{
	int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // Size of this structure
		1,                                                              // Version of this structure    
		PFD_DRAW_TO_WINDOW |                    // Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |					// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,                       // Double buffered
		PFD_TYPE_RGBA,                          // RGBA Color mode
		24,                                     // Want 24bit color 
		0,0,0,0,0,0,                            // Not used to select mode
		0,0,                                    // Not used to select mode
		0,0,0,0,0,                              // Not used to select mode
		32,                                     // Size of depth buffer
		0,                                      // Not used to select mode
		0,                                      // Not used to select mode
		PFD_MAIN_PLANE,                         // Draw in main plane
		0,                                      // Not used to select mode
		0,0,0 };                                // Not used to select mode

	// Choose a pixel format that best matches that described in pfd
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	// Set the pixel format for the device context
	SetPixelFormat(hDC, nPixelFormat, &pfd);
	}

HPALETTE GetOpenGLPalette(HDC hDC)
	{
	HPALETTE hRetPal = NULL;	// Handle to palette to be created
	PIXELFORMATDESCRIPTOR pfd;	// Pixel Format Descriptor
	LOGPALETTE *pPal;			// Pointer to memory for logical palette
	int nPixelFormat;			// Pixel format index
	int nColors;				// Number of entries in palette
	int i;						// Counting variable
	BYTE RedRange,GreenRange,BlueRange;
								// Range for each color entry (7,7,and 3)


	// Get the pixel format index and retrieve the pixel format description
	nPixelFormat = GetPixelFormat(hDC);
	DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	// Does this pixel format require a palette?  If not, do not create a
	// palette and just return NULL
	if(!(pfd.dwFlags & PFD_NEED_PALETTE))
		return NULL;

	// Number of entries in palette.  8 bits yeilds 256 entries
	nColors = 1 << pfd.cColorBits;	

	// Allocate space for a logical palette structure plus all the palette entries
	pPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) +nColors*sizeof(PALETTEENTRY));

	// Fill in palette header 
	pPal->palVersion = 0x300;		// Windows 3.0
	pPal->palNumEntries = nColors; // table size

	// Build mask of all 1's.  This creates a number represented by having
	// the low order x bits set, where x = pfd.cRedBits, pfd.cGreenBits, and
	// pfd.cBlueBits.  
	RedRange = (1 << pfd.cRedBits) -1;
	GreenRange = (1 << pfd.cGreenBits) - 1;
	BlueRange = (1 << pfd.cBlueBits) -1;

	// Loop through all the palette entries
	for(i = 0; i < nColors; i++)
		{
		// Fill in the 8-bit equivalents for each component
		pPal->palPalEntry[i].peRed = (i >> pfd.cRedShift) & RedRange;
		pPal->palPalEntry[i].peRed = (unsigned char)(
			(double) pPal->palPalEntry[i].peRed * 255.0 / RedRange);

		pPal->palPalEntry[i].peGreen = (i >> pfd.cGreenShift) & GreenRange;
		pPal->palPalEntry[i].peGreen = (unsigned char)(
			(double)pPal->palPalEntry[i].peGreen * 255.0 / GreenRange);

		pPal->palPalEntry[i].peBlue = (i >> pfd.cBlueShift) & BlueRange;
		pPal->palPalEntry[i].peBlue = (unsigned char)(
			(double)pPal->palPalEntry[i].peBlue * 255.0 / BlueRange);

		pPal->palPalEntry[i].peFlags = (unsigned char) NULL;
		}
		

	// Create the palette
	hRetPal = CreatePalette(pPal);

	// Go ahead and select and realize the palette for this device context
	SelectPalette(hDC,hRetPal,FALSE);
	RealizePalette(hDC);

	// Free the memory used for the logical palette structure
	free(pPal);

	// Return the handle to the new palette
	return hRetPal;
	}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
	MSG                     msg;            // Windows message structure
	WNDCLASS        wc;                     // Windows class structure
	HWND            hWnd;           // Storeage for window handle

	hInstance = hInst;

	// Register Window style
	wc.style                        = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc          = (WNDPROC) WndProc;
	wc.cbClsExtra           = 0;
	wc.cbWndExtra           = 0;
	wc.hInstance            = hInstance;
	wc.hIcon                        = NULL;
	wc.hCursor                      = LoadCursor(NULL, IDC_ARROW);
	
	// No need for background brush for OpenGL window
	wc.hbrBackground        = NULL;         
	
	wc.lpszMenuName         = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName        = lpszAppName;

	// Register the window class
	if(RegisterClass(&wc) == 0)
		return FALSE;


	// Create the main application window
	hWnd = CreateWindow(
				lpszAppName,
				lpszAppName,
				
				// OpenGL requires WS_CLIPCHILDREN and WS_CLIPSIBLINGS
				WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
	
				// Window position and size
				50, 50,
				400, 400,
				NULL,
				NULL,
				hInstance,
				NULL);

	// If window was not created, quit
	if(hWnd == NULL)
		return FALSE;


	// Display the window
	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);

	const WORD ID_TIMER = 1;
	SetTimer(hWnd, ID_TIMER, 15, NULL);
	// Process application messages until the application closes
	while( GetMessage(&msg, NULL, 0, 0))
		{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}

	return msg.wParam;
	}

void LoadTexture(int i, char* filename)
{

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);

	glBindTexture(GL_TEXTURE_2D, texture[i]);       // aktywuje obiekt tekstury

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// tworzy obraz tekstury
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
		bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);

	if (bitmapData)
		free(bitmapData);

}

LRESULT CALLBACK WndProc(HWND hWnd,	UINT message, WPARAM wParam, LPARAM lParam)
	{
	static HGLRC hRC;               // Permenant Rendering context
	static HDC hDC;                 // Private GDI Device context

	switch (message)
		{
		// Window creation, setup for OpenGL
		case WM_CREATE:
			// Store the device context
			hDC = GetDC(hWnd);              

			// Select the pixel format
			SetDCPixelFormat(hDC);          

			// Create palette if needed
			hPalette = GetOpenGLPalette(hDC);

			// Create the rendering context and make it current
			hRC = wglCreateContext(hDC);
			wglMakeCurrent(hDC, hRC);
			SetupRC();

			//define number
			glGenTextures(4, &texture[0]);                  	
			//
			LoadTexture(0, "szyba.bmp");
			LoadTexture(1, "ring.bmp");
			LoadTexture(2, "stone.bmp");
			LoadTexture(3, "skybox.bmp");




			// ustalenie sposobu mieszania tekstury z t≥em
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
			///////////////////////////////////////////////////////
			///////////////PUT YOUR INIT HERE//////////////////////
			///////////////////////////////////////////////////////
			uforing = ImportObject("uforing.obj");
			ufocab = ImportObject("ufocab.obj");
			stone = ImportObject("stone.obj");
			skybox = ImportObject("skybox.obj");

			for (int i = 0; i < ilosc_asteroid; i++)
			{
				vek3 a;
				float s;
				a.x = (rand() % 2000) - 1000;
				a.y=(rand() % 1000) - 500;
				a.z=(rand() % 2000) - 1000;
				s=(rand() % 25) + 5;
				asteroidy.push_back(a);
				scale.push_back(s);
			}
			//////////////////////////////////////////////////////
			//////////////////////////////////////////////////////
			//////////////////////////////////////////////////////

			break;

		// Window is being destroyed, cleanup
		case WM_DESTROY:
			// Deselect the current rendering context and delete it
			wglMakeCurrent(hDC,NULL);
			wglDeleteContext(hRC);

			// Delete the palette if it was created
			if(hPalette != NULL)
				DeleteObject(hPalette);

			// Tell the application to terminate after the window
			// is gone.
			PostQuitMessage(0);
			break;

		case WM_TIMER:
		{
			Update();
			RenderScene();
			SwapBuffers(hDC);
			ValidateRect(hWnd, NULL);
			break;
		}
		// Window is resized.
		case WM_SIZE:
			// Call our function which modifies the clipping
			// volume and viewport
			ChangeSize(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_KEYUP:
		{
			if (wParam == VK_UP)
				keyboard.up = false;

			if (wParam == VK_DOWN)
				keyboard.down = false;

			if (wParam == VK_LEFT)
				keyboard.left = false;

			if (wParam == VK_RIGHT)
				keyboard.right = false;

			if (wParam == 0x53)//S
				keyboard.s = false;

			if (wParam == 0x57)//W
				keyboard.w = false;

			if (wParam == 0x41)//A
				keyboard.a = false;

			if (wParam == 0x44)//D
				keyboard.d = false;

			if (wParam == 0x51)//Q
				keyboard.q = false;

			if (wParam == 0x45)//E
				keyboard.e = false;
			Update();

		}
		break;
		
		// The painting function.  This message sent by Windows 
		// whenever the screen needs updating.
		case WM_PAINT:
			{
			// Call OpenGL drawing code
			RenderScene();

			SwapBuffers(hDC);

			// Validate the newly painted client area
			ValidateRect(hWnd,NULL);
			}

			break;

		// Windows is telling the application that it may modify
		// the system palette.  This message in essance asks the 
		// application for a new palette.
		case WM_QUERYNEWPALETTE:
			// If the palette was created.
			if(hPalette)
				{
				int nRet;

				// Selects the palette into the current device context
				SelectPalette(hDC, hPalette, FALSE);

				// Map entries from the currently selected palette to
				// the system palette.  The return value is the number 
				// of palette entries modified.
				nRet = RealizePalette(hDC);

				// Repaint, forces remap of palette in current window
				InvalidateRect(hWnd,NULL,FALSE);

				return nRet;
				}
			break;

	
		// This window may set the palette, even though it is not the 
		// currently active window.
		case WM_PALETTECHANGED:
			// Don't do anything if the palette does not exist, or if
			// this is the window that changed the palette.
			if((hPalette != NULL) && ((HWND)wParam != hWnd))
				{
				// Select the palette into the device context
				SelectPalette(hDC,hPalette,FALSE);

				// Map entries to system palette
				RealizePalette(hDC);
				
				// Remap the current colors to the newly realized palette
				UpdateColors(hDC);
				return 0;
				}
			break;

		// Key press, check for arrow keys to do cube rotation.
		case WM_KEYDOWN:
		{
			if (wParam == VK_UP)
				keyboard.up = true;
			
			if (wParam == VK_DOWN)
				keyboard.down = true;
		
			if (wParam == VK_LEFT)
				keyboard.left = true;

			if (wParam == VK_RIGHT)
				keyboard.right = true;

			if (wParam == 0x53)//S
				keyboard.s = true;

			if (wParam == 0x57)//W
				keyboard.w = true;

			if (wParam == 0x41)//A
				keyboard.a = true;

			if (wParam == 0x44)//D
				keyboard.d = true;

			if (wParam == 0x51)//Q
				keyboard.q = true;

			if (wParam == 0x45)//E
				keyboard.e = true;

			InvalidateRect(hWnd,NULL,FALSE);
			Update();

		}
		break;
		
	default:   // Passes it on if unproccessed
	    return (DefWindowProc(hWnd, message, wParam, lParam));

	}

    return (0L);
	}

