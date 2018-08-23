// GLWind.c
// Simple OpenGL Perspective view window to Build on

#include <windows.h>            // Window defines
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
#include "resource.h"           // About box resource identifiers.

// includes for superbible texture mapping
#include "texture.h"
//#include "tex2d.h"
#include <stdarg.h>

#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <iostream.h>

// Bob. Start 02282003 WCSC Code
#define MSWIN
#define MSWIN32
#define MSWINDLL
#include "comm.h"
// Bob. End 02282003

#include "defs.h"
#include "common.h"
#include "global.h"
// Bob. Start 06222002
#include "serial.h"
// Bob. End 06222002

GLuint		SkyTexture;		/* Sky texture image */

HPALETTE hPalette = NULL;

// Application name and instance storeage
static LPCTSTR lpszAppName = "OpenGL Window";
static HINSTANCE hInstance;

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 30.0f;
static GLfloat zRot = 0.0f;

// Translation amounts
static GLfloat xTran = 0.0f;
static GLfloat yTran = 0.0f;
static GLfloat zTran = -300.0f;

static float cubexsize = 50.0f;
static float cubeysize = 50.0f;
static float cubezsize = 50.0f;

// Rotation amounts
static GLfloat xRot2 = 30.0f;
static GLfloat yRot2 = 30.0f;
static GLfloat zRot2 = 0.0f;

// Translation amounts
static GLfloat xTran2 = 100.0f;
static GLfloat yTran2 = 0.0f;
static GLfloat zTran2 = -260.0f;

static float cubexsize2 = 25.0f;
static float cubeysize2 = 25.0f;
static float cubezsize2 = 25.0f;

OSVERSIONINFO	versionInformation;

#define VIDEO_STATUS		0x03DA	/* address of EGA board status register */
#define VERTICAL_RETRACE	0x08	/* bit is on while in retrace */

// Choi. Start
GLUquadricObj* QObj;
GLUquadricObj* CylObj;
GLUquadricObj* SphObj;
// Choi. End

// Choi. Start 07012000
HWND hWndMessage;
char* globalMessage = "Press space key to start experiment";
int msgcBackRed;
int msgcBackGreen;
int msgcBackBlue;
int msgcTextRed;
int msgcTextGreen;
int msgcTextBlue;
int count_ix;
int count_iy;
// Choi. End 07052000


//Adam. start 112117
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

LPFN_ISWOW64PROCESS fnIsWow64Process;

BOOL IsWow64()
{
    BOOL bIsWow64 = FALSE;

    //IsWow64Process is not available on all supported versions of Windows.
    //Use GetModuleHandle to get a handle to the DLL that contains the function
    //and GetProcAddress to get a pointer to the function if available.

    fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
        GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

    if(NULL != fnIsWow64Process)
    {
        if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
        {
            //handle error
        }
    }
    return bIsWow64;
}
//Adam. end 112117

// Change viewing volume and viewport.  Called when window is resized
void ChangeSize(GLsizei w, GLsizei h)
	{
	GLfloat fAspect;

	// Prevent a divide by zero
	if(h == 0)
		h = 1;

	// set hresval and vresval parameters

	hresval = w;
	vresval = h;

	// Set Viewport to window dimensions
    glViewport(0, 0, w, h);

	fAspect = (GLfloat)w/(GLfloat)h;

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Produce the perspective projection
	// gluPerspective(60.0f, fAspect, 1.0, 2.0);

// Choi. Start 12011999
	// gluPerspective(RAD2DEG*(float)((float)2.0*atan((float)((float)0.5*viewport_height/h_dist))), (float)(viewport_width/viewport_height), h_dist, 1000000.0);
	gluPerspective(RAD2DEG*(float)((float)2.0*atan((float)((float)0.5*viewport_height/h_dist))), (float)(viewport_width/viewport_height), h_dist, 1000000.0);


// Choi End 12011999

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	}



// This function does any needed initialization on the rendering
// context.  Here it sets up and initializes the lighting for
// the scene.
void SetupRC()
	{
	// Light values and coordinates
//	GLfloat  whiteLight[] = { 0.45f, 0.45f, 0.45f, 1.0f };
//	GLfloat  sourceLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
//	GLfloat	 lightPos[] = { -50.f, 25.0f, 250.0f, 0.0f };

	GLfloat  whiteLight[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat  sourceLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat	 lightPos[] = { -50.0f, 50.0f, 100.0f, 0.0f };

	if(!runobj_compatibility_flag)
		glEnable(GL_DEPTH_TEST);	// Hidden surface removal

//	glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
	glFrontFace(GL_CW);			// Counter clock-wise polygons face out
	glEnable(GL_CULL_FACE);		// Do not calculate back of polygons

	if (!runobj_compatibility_flag)
	{
	// Enable lighting
	glEnable(GL_LIGHTING);

	// Setup and enable light 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,whiteLight);
	glLightfv(GL_LIGHT0,GL_AMBIENT,sourceLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,sourceLight);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	glEnable(GL_LIGHT0);

	// Enable color tracking
	glEnable(GL_COLOR_MATERIAL);
	
	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	}
	
	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	}

// Called to draw scene
void RenderScene(void)
	{

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// superbible texture mapping code
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	// Save the matrix state and do the rotations
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

//	glTranslatef(0.0f, 0.0f, -300.0f);
	glTranslatef(xTran, yTran, zTran);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	
	// superbible texture mapping code
    glEnable(GL_TEXTURE_2D);
    glCallList(TextureMap[0]);

	// Simple Cube (now with texture map

	glBegin(GL_QUADS);

		// Front Panel
		// Pointing straight out Z
		glRGB(255, 0, 0);
		glNormal3f(0.0f, 0.0f, 1.0f);
		// superbible texture mapping code
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-cubexsize, -cubeysize, cubezsize);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-cubexsize, cubeysize, cubezsize);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(cubexsize, cubeysize, cubezsize);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(cubexsize, -cubeysize, cubezsize);
	
	glEnd();

	// superbible texture mapping code
    glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);

		// Top Panel 
		// Normal points up Y axis
		glRGB(0, 255, 0);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-cubexsize, cubeysize, cubezsize);
		glVertex3f(-cubexsize, cubeysize, -cubezsize);
		glVertex3f(cubexsize, cubeysize, -cubezsize);
		glVertex3f(cubexsize, cubeysize, cubezsize);
		
		// Bottom Panel
		glRGB(0, 0, 255);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(-cubexsize, -cubeysize, -cubezsize);
		glVertex3f(-cubexsize, -cubeysize, cubezsize);
		glVertex3f(cubexsize, -cubeysize, cubezsize);
		glVertex3f(cubexsize, -cubeysize, -cubezsize);

		// Right Panel
		glRGB(255, 0, 255);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f(cubexsize, -cubeysize, cubezsize);
		glVertex3f(cubexsize, cubeysize, cubezsize);
		glVertex3f(cubexsize, cubeysize, -cubezsize);
		glVertex3f(cubexsize, -cubeysize, -cubezsize);

		// Left Panel
		glRGB(0, 255, 255);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(-cubexsize, -cubeysize, -cubezsize);
		glVertex3f(-cubexsize, cubeysize, -cubezsize);
		glVertex3f(-cubexsize, cubeysize, cubezsize);
		glVertex3f(-cubexsize, -cubeysize, cubezsize);


		// Back Panel
		// Pointing straight out Z
		glRGB(255, 255, 0);
		glNormal3f(0.0f, 0.0f, -1.0f);	
		glVertex3f(-cubexsize, cubeysize, -cubezsize);
		glVertex3f(-cubexsize, -cubeysize, -cubezsize);
		glVertex3f(cubexsize, -cubeysize, -cubezsize);
		glVertex3f(cubexsize, cubeysize, -cubezsize);
	
	glEnd();

	// Restore the matrix state
	glPopMatrix();	// Modelview matrix

	// now do a second cube

	glPushMatrix();

	glTranslatef(xTran2, yTran2, zTran2);
	glRotatef(xRot2, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot2, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot2, 0.0f, 0.0f, 1.0f);
	
	// superbible texture mapping code
    glEnable(GL_TEXTURE_2D);
    glCallList(SkyTexture);

	// Simple Cube (now with texture map

	glBegin(GL_QUADS);

		// Front Panel
		// Pointing straight out Z
		glRGB(255, 0, 0);
		glNormal3f(0.0f, 0.0f, 1.0f);
		// superbible texture mapping code
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-cubexsize2, -cubeysize2, cubezsize2);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-cubexsize2, cubeysize2, cubezsize2);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(cubexsize2, cubeysize2, cubezsize2);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(cubexsize2, -cubeysize2, cubezsize2);
	
	glEnd();

	// superbible texture mapping code
    glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);

		// Top Panel 
		// Normal points up Y axis
		glRGB(0, 255, 0);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-cubexsize2, cubeysize2, cubezsize2);
		glVertex3f(-cubexsize2, cubeysize2, -cubezsize2);
		glVertex3f(cubexsize2, cubeysize2, -cubezsize2);
		glVertex3f(cubexsize2, cubeysize2, cubezsize2);
		
		// Bottom Panel
		glRGB(0, 0, 255);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(-cubexsize2, -cubeysize2, -cubezsize2);
		glVertex3f(-cubexsize2, -cubeysize2, cubezsize2);
		glVertex3f(cubexsize2, -cubeysize2, cubezsize2);
		glVertex3f(cubexsize2, -cubeysize2, -cubezsize2);

		// Right Panel
		glRGB(255, 0, 255);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f(cubexsize2, -cubeysize2, cubezsize2);
		glVertex3f(cubexsize2, cubeysize2, cubezsize2);
		glVertex3f(cubexsize2, cubeysize2, -cubezsize2);
		glVertex3f(cubexsize2, -cubeysize2, -cubezsize2);

		// Left Panel
		glRGB(0, 255, 255);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(-cubexsize2, -cubeysize2, -cubezsize2);
		glVertex3f(-cubexsize2, cubeysize2, -cubezsize2);
		glVertex3f(-cubexsize2, cubeysize2, cubezsize2);
		glVertex3f(-cubexsize2, -cubeysize2, cubezsize2);


		// Back Panel
		// Pointing straight out Z
		glRGB(255, 255, 0);
		glNormal3f(0.0f, 0.0f, -1.0f);	
		glVertex3f(-cubexsize2, cubeysize2, -cubezsize2);
		glVertex3f(-cubexsize2, -cubeysize2, -cubezsize2);
		glVertex3f(cubexsize2, -cubeysize2, -cubezsize2);
		glVertex3f(cubexsize2, cubeysize2, -cubezsize2);
	
	glEnd();

	// Restore the matrix state
	glPopMatrix();	// Modelview matrix

	// Flush drawing commands
	glFlush();
	}


// Select the pixel format for a given device context
void SetDCPixelFormat(HDC hDC)
	{
	int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // Size of this structure
		1,                                      // Version of this structure    
		PFD_DRAW_TO_WINDOW |                    // Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |					// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,                       // Double buffered,
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

	// 052299 stereo code
	if ( stereo_on_flag)
		pfd.dwFlags |= PFD_STEREO;				// Support stereo

	// Choose a pixel format that best matches that described in pfd
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	// Set the pixel format for the device context
	SetPixelFormat(hDC, nPixelFormat, &pfd);
	}



// If necessary, creates a 3-3-2 palette for the device context listed.
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


// Entry point for all Windows programs
int APIENTRY WinMain(   HINSTANCE       hInst,
						HINSTANCE       hPrevInstance,
						LPSTR           lpCmdLine,
						int			    nCmdShow)
{
	MSG                     msg;            // Windows message structure
	WNDCLASS        wc;                     // Windows class structure
	HWND            hWnd;           // Storeage for window handle

	HANDLE h;

	int	status;

	// check to see what operating system is running

/*	versionInformation.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(GetVersionEx(&versionInformation))
	{
		if(versionInformation.dwPlatformId == VER_PLATFORM_WIN32_NT)
			sprintf(tMessage, "Windows NT, V%d.%d, %s", versionInformation.dwMajorVersion, versionInformation.dwMinorVersion, versionInformation.szCSDVersion );
		else if(versionInformation.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
			sprintf(tMessage, "Windows 95, V%d.%d, %s", versionInformation.dwMajorVersion, versionInformation.dwMinorVersion, versionInformation.szCSDVersion );
		else
			sprintf(tMessage, "Invalid OS, V%d.%d, %s", versionInformation.dwMajorVersion, versionInformation.dwMinorVersion, versionInformation.szCSDVersion );

		DisplayInfoMessage(tMessage);
	}
	else
	{
		DisplayErrorMessage("GetVersionEx failed!");
		return (FALSE);
	} */

	/*// ret code to use the NT giveio driver (from tstio example)
	
	if(versionInformation.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		h = CreateFile("\\\\.\\giveio", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(h == INVALID_HANDLE_VALUE)
		{
			DisplayErrorMessage("Couldn't access giveio device");
			return (FALSE);
		}
		CloseHandle(h);
	}*/

//Adam. start 112117
// ret code to load the runobj.cfg file
// check whether interface type is different than mouse
// If it is, try to get a handle to giveio driver
// If unable, prompt user to install giveio
if ( get_config_table ( "runobj.cfg" ) == ERROR )
{
	return ( ERROR );
}
if (config.control_1_interface_type != 2 || config.control_2_interface_type != 2)
{
	h = CreateFile("\\\\.\\giveio", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(h == INVALID_HANDLE_VALUE)
	{
		if(IsWow64())
		{
			DisplayErrorMessage("A control interface type is set to use a device other than the mouse. If this is correct, please install the 64-bit giveio driver! Otherwise, check your runobj settings.");
			return (FALSE);
		}
		else
		{
			DisplayErrorMessage("A control interface type is set to use a device other than the mouse. If this is correct, please install the 32-bit giveio driver! Otherwise, check your runobj settings.");
			return (FALSE);
		}
	
	}
	else
	{
		sprintf(tMessage, "Winobj 2017: Press OK to begin simulation");
		DisplayInfoMessage(tMessage);
	}
}
else
{
	sprintf(tMessage, "Winobj 2017: Press OK to begin simulation");
	DisplayInfoMessage(tMessage);
}
CloseHandle(h);
//Adam. end 112117



	// experiment initialization

	if ( experiment_init () == ERROR )
	{
// Bob. Start 02282003 WCSC Code
		// close port if open
		if ( VxdPortID_Flag )
		{
			if((status=WCSCVxDCleanup(VxDPortId))!=RS232ERR_NONE)
				sprintf ( message[0], "Error %d WCSCVxDPutPacket POLHEMUS_BINARY WCSCVxDCleanup", status );
		}
// Bob. End 02282003

		DisplayErrorMessage(message[0]);
		return (FALSE);
	}

// Bob. Start 06222002 code to quick test the Fastrak
	// Config serial port
//	s_set_interrupts ( COM1, DISABLE );
//	s_set_baud ( COM1, BAUD9600 );
//	s_set_parity ( COM1, NONE );
//	s_set_datalen ( COM1, DL8 );
//	s_set_stop ( COM1, STOP1 );
// code to quick test the Fastrak binary subroutine
//	init_polhemus_fastrak ();
// Bob. End 06222002

	// for RunObj compatibility with no lighting and depth testing
	// set to 1 (WinObj2/WinStep2), else 0 (WinObj/WinStep)

	// 052299 runobj.cfg changes for flags
//	runobj_compatibility_flag = 0;

	if ( depth_lighting_flag )
		runobj_compatibility_flag = 0;
	else
		runobj_compatibility_flag = 1;

	// For Step set to 1 (WinStep/WinStep2), else 0 (WinObj/WinObj2)

	// 052299 runobj.cfg changes for flags
//	frame_debug_flag = 1;
	frame_debug_flag = step_control_flag;

	frame_step_flag = 0;

	if(frame_debug_flag)
		frame_step_flag = 1;

	hInstance = hInst;

	// Register Window style
	wc.style                        = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc          = (WNDPROC) WndProc;
	wc.cbClsExtra           = 0;
	wc.cbWndExtra           = 0;
	wc.hInstance            = hInstance;
	wc.hIcon                        = NULL;
	wc.hCursor                      = LoadCursor(NULL, IDC_ARROW);
	
	// No need for background brush with OpenGL window
	wc.hbrBackground        = NULL;         
	
	wc.lpszMenuName         = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName        = lpszAppName;

	// Register the window class
	if(RegisterClass(&wc) == 0)
		return FALSE;

// Choi. Start. 07012000

	// Register message window
	wc.lpfnWndProc   = MessageWndProc;
	wc.lpszClassName = "MessageWindow";
	if (!RegisterClass(&wc))
	{
	    return FALSE;
    };

// Choi. End. 07052000

	// Create the main application window
	hWnd = CreateWindow(
				lpszAppName,
				lpszAppName,
				
				// OpenGL requires WS_CLIPCHILDREN and WS_CLIPSIBLINGS
//				WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
				// ret change to get window with no border
				// also edited resource.c to remove top menu bar completely
				// after adding ability to use the escape key to terminate the application
				WS_POPUP | WS_MAXIMIZE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,

				// Window position and size
				50, 50,
				500, 500,
				NULL,
				NULL,
				hInstance,
				NULL);

	// If window was not created, quit
	if(hWnd == NULL)
	{
		DisplayErrorMessage("CreateWindow failed!");
		return (FALSE);
	}

	// Display the window
	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);

	experiment_wait_flag = 1;

	// Process application messages until the application closes
	while( GetMessage(&msg, NULL, 0, 0))
		{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}

	return msg.wParam;
}

// 'DisplayErrorMessage()' - Display an error message dialog.

void
DisplayErrorMessage(char *format,	/* I - printf() style format string */
                    ...)			/* I - Other arguments as necessary */
{
  va_list	ap;				/* Argument pointer */
  char		s[1024];		/* Output string */

  if (format == NULL)
    return;

  va_start(ap, format);
  vsprintf(s, format, ap);
  va_end(ap);

  MessageBeep(MB_ICONEXCLAMATION);
  MessageBox(NULL, s, "Error", MB_OK | MB_ICONEXCLAMATION);
}

// 'DisplayInfoMessage()' - Display an information message dialog.

void
DisplayInfoMessage(char *format,	/* I - printf() style format string */
                    ...)			/* I - Other arguments as necessary */
{
  va_list	ap;				/* Argument pointer */
  char		s[1024];		/* Output string */

  if (format == NULL)
    return;

  va_start(ap, format);
  vsprintf(s, format, ap);
  va_end(ap);

  MessageBeep(MB_OK);
  MessageBox(NULL, s, "Information", MB_OK | MB_ICONINFORMATION);
}

// Choi Start 12031999
HGLRC hRC;               // Permenant Rendering context
HDC hDC;                 // Private GDI Device context
// Choi End 12031999

// Window procedure, handles all messages for this program
LRESULT CALLBACK WndProc(			 HWND    hWnd,
							UINT    imessage,
							WPARAM  wParam,
							LPARAM  lParam)
{
	int i;
	// static HGLRC hRC;               // Permenant Rendering context
	// static HDC hDC;                 // Private GDI Device context

	// Bob. Start 01292000 code to force lower frame rate sync
	float	temp_time1;
	float	temp_time2;
	long	temp_frame_ms;
	long	temp_half_frame_ms;
	long	temp_diff;
	// Bob. End 01292000

	int	status;

	switch (imessage)
		{
		// Window creation, setup for OpenGL
		case WM_CREATE:

			// Store the device context
			hDC = GetDC(hWnd);              

			// Select the pixel format
			SetDCPixelFormat(hDC);          

			hPalette = GetOpenGLPalette(hDC);

			// Create the rendering context and make it current
			hRC = wglCreateContext(hDC);
			wglMakeCurrent(hDC, hRC);

			// ret code from help to use fonts with opengl
			// make the system font the device context's selected font 
			SelectObject (hDC, GetStockObject (SYSTEM_FONT)); 
 
			// create the bitmap display lists 
			// we're making images of glyphs 0 thru 255 
			// the display list numbering starts at 1000, an arbitrary choice 
			wglUseFontBitmaps (hDC, 0, 255, 1000); 

			SetupRC();

			for(TextureMapIndex=0; TextureMapIndex<TextureFileIndex; TextureMapIndex++)
			{
				strcpy(TextureMapName, TextureFile[TextureMapIndex].Name);

				// superbible texture mapping code to load a texture map
			    LoadAllTextures();
			}

			ShowCursor(FALSE);

			// ret code from superbible to do real time animation
			// Create a timer that fires every millisecond
//			SetTimer(hWnd,101,1,NULL);
			// specify timer to trigger every 16 ms (approximately 60/sec)
//			SetTimer(hWnd,101,(int)((float)((float)1000.0*((float)header.frame_rate*(float)0.0167))),NULL);
//			SetTimer(hWnd,101,(int)((float)((float)1000.0*((float)1.0*(float)0.0167))),NULL);
//			SetTimer(hWnd,101,33,NULL);
			SetTimer(hWnd,101,10,NULL);
			set_timer_flag = 0;

// Start. Choi
			QObj = gluNewQuadric();
			CylObj = gluNewQuadric();
			SphObj = gluNewQuadric();
// End. Choi.

// Choi. Start 07012000
			{
				int x, y, width, height;
				char* str = "Press space key to start experiment";
				width = strlen(str)*8;
				height = 18;
				x = (GetSystemMetrics(SM_CXFULLSCREEN) - width)/2;
				y = (GetSystemMetrics(SM_CYFULLSCREEN) - height)/2;

				hWndMessage = CreateWindow("MessageWindow", "m",
					WS_CHILD | WS_BORDER | WS_VISIBLE,
					x, y, width, height,
					hWnd,
					0,
					((LPCREATESTRUCT)lParam)->hInstance,
					NULL);
			};
// Choi. End 07052000

			break;

		// Window is being trashed, cleanup
		case WM_DESTROY:

			ShowCursor(TRUE);

			// ret code from superbible to kill the timer we set up
			// Kill the timer that we created
			KillTimer(hWnd,101);

			// Deselect the current rendering context and delete it
			wglMakeCurrent(hDC,NULL);
			wglDeleteContext(hRC);

			if(hPalette != NULL)
				DeleteObject(hPalette);

			// Tell the application to terminate after the window
			// is gone.
			PostQuitMessage(0);
			break;

		// Window is resized.
		case WM_SIZE:
			// Call our function which modifies the clipping
			// volume and viewport
			ChangeSize(LOWORD(lParam), HIWORD(lParam));
			break;

		// ret code from superbible to handle timer messages
		// Timer simply calls the idlefunction, then invalidates the 
		// window so it will be redrawn.
		case WM_TIMER:
			{
				// get rid of any extra timer messages in the que
				MSG msg; // Windows message structure
				PeekMessage(&msg, hWnd, WM_TIMER, WM_TIMER, PM_REMOVE);

				// set timer if needed

				if ( !set_timer_flag )
				{
//					SetTimer(hWnd,101,(int)((float)((float)1000.0*((float)header.frame_rate*(float)0.0167))),NULL);
//					SetTimer(hWnd,101,10,NULL);
					set_timer_flag = 0;
				}

				// Bob. Start 01292000 code to force lower frame rate sync
				if ( swap_buffer_flag )
				{
					// see where we are in time and only call swapbuffers if within 1/2
					// frame time of the next desired sync
					temp_time1 = (long)( (float)( get_pclock () - before ) * (float)pclock_msecs_per_tick );
					temp_time2 = ((float)(frame_number-1) * (float)((float)1000.0/config_frame_rate));
					temp_frame_ms = (long)((float)500.0/config_frame_rate);
					temp_diff = temp_time1 - temp_time2;
					if ( !scene_run_flag || temp_diff > -temp_frame_ms )
					{
						SwapBuffers(hDC);
						swap_buffer_flag = 0;
						swap_ticks = get_pclock ();
	
					}
				}
//				if ( !frame_step_flag )
				else if ( !frame_step_flag )
				{
					InvalidateRect(hWnd,NULL,FALSE);
				}
				// Bob. End 01292000
			}
			break;

		// The painting function.  This message sent by Windows 
		// whenever the screen needs updating.
		case WM_PAINT:
				wglMakeCurrent(hDC, hRC);
			// Call OpenGL drawing code
//			RenderScene();

//		if ( experiment_done_flag )
//		{
//			// exit
//
//			sprintf(tMessage, "Experiment is done");
//			DisplayInfoMessage(tMessage);
//			
//			DestroyWindow(hWnd);
//		
//			break;
//		}

		if ( process_time () == ERROR )
		{
			// display error message
		
			DisplayErrorMessage(message[0]);
		
			// exit
			
			DestroyWindow(hWnd);
		
// Bob. Start 02282003 WCSC Code
			// close port if open
			if ( VxdPortID_Flag )
			{
				if((status=WCSCVxDCleanup(VxDPortId))!=RS232ERR_NONE)
					sprintf ( message[0], "Error %d WCSCVxDPutPacket POLHEMUS_BINARY WCSCVxDCleanup", status );
			}
// Bob. End 02282003

			break;
		}
		
		if ( process_button () == ERROR )
		{
// Bob. Start 02282003 WCSC Code
			// close port if open
			if ( VxdPortID_Flag )
			{
				if((status=WCSCVxDCleanup(VxDPortId))!=RS232ERR_NONE)
					sprintf ( message[0], "Error %d WCSCVxDPutPacket POLHEMUS_BINARY WCSCVxDCleanup", status );
			}
// Bob. End 02282003

			// display error message
		
			DisplayErrorMessage(message[0]);
		
			// exit
			
			DestroyWindow(hWnd);
		
			break;
		}

		swap_buffer_flag = 0;

		if ( process_trials () == ERROR )
		{
// Bob. Start 02282003 WCSC Code
			// close port if open
			if ( VxdPortID_Flag )
			{
				if((status=WCSCVxDCleanup(VxDPortId))!=RS232ERR_NONE)
					sprintf ( message[0], "Error %d WCSCVxDPutPacket POLHEMUS_BINARY WCSCVxDCleanup", status );
			}
// Bob. End 02282003

			// display error message
		
//			DisplayErrorMessage(message[0]);

// ***** debug
// Clear the window with current clearing color
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
DisplayGLMessage (message[0], (float)0.0, (float)0.0);
SwapBuffers(hDC);
reset_trial_flags();

			// exit
			
//			DestroyWindow(hWnd);
		
			break;
		}

		/* do timing */

		if ( swap_buffer_flag )
		{
//			sprintf ( message[0], "Video Time=%f", video_time );
//			DisplayGLMessage (message[0], (float)0.0, (float)(vp_height*(float)-0.2));
//			glFlush();

//			while ( !( _inp ( VIDEO_STATUS ) & VERTICAL_RETRACE ) );
//			while ( ( _inp ( VIDEO_STATUS ) & VERTICAL_RETRACE ) );
//			while ( !( _inp ( VIDEO_STATUS ) & VERTICAL_RETRACE ) );
//			while ( ( _inp ( VIDEO_STATUS ) & VERTICAL_RETRACE ) );
//			while ( !( _inp ( VIDEO_STATUS ) & VERTICAL_RETRACE ) );

			// Bob. Start 01292000 code to force lower frame rate sync
//			SwapBuffers(hDC);
//			swap_ticks = get_pclock ();
			// see where we are in time and only call swapbuffers if within 1/2
			// frame time of the next desired sync
			temp_time1 = (long)( (float)( get_pclock () - before ) * (float)pclock_msecs_per_tick );
			temp_time2 = ((float)(frame_number-1) * (float)((float)1000.0/config_frame_rate));
			temp_frame_ms = (long)((float)500.0/config_frame_rate);
			temp_diff = temp_time1 - temp_time2;
			// Bob. Start 02142000 code to force lower frame rate sync
			if ( !force_sync_flag || !scene_run_flag || ( temp_diff > -temp_frame_ms ) )
			{
//				SwapBuffers(hDC);
// Choi. Start 07012000
				SwapBuffers(hDC);
// Choi. End 07052000
				swap_buffer_flag = 0;
				swap_ticks = get_pclock ();
			}
			// Bob. End 02142000
			// Bob. End 01292000
		}
			// Validate the newly painted client area
			ValidateRect(hWnd,NULL);
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

			if(wParam == VK_ESCAPE)
			{
// Bob. Start 02282003 WCSC Code
				// close port if open
				if ( VxdPortID_Flag )
				{
					if((status=WCSCVxDCleanup(VxDPortId))!=RS232ERR_NONE)
						sprintf ( message[0], "Error %d WCSCVxDPutPacket POLHEMUS_BINARY WCSCVxDCleanup", status );
				}
// Bob. End 02282003

				DestroyWindow(hWnd);
					
				break;
			}

			if ( frame_debug_flag )
			{
				// step forward (normal)

				if(wParam == VK_DOWN)
				{
					if ( scene_run_flag )
					{
						frame_step_flag = 1;

						increment = 1;
					}

					InvalidateRect(hWnd,NULL,FALSE);
				}

				// step backward

				else if(wParam == VK_UP)
				{
					if ( scene_run_flag )
					{
						frame_step_flag = 1;

						if ( frame_number > 1 )
						{	
							frame_number -= 2;

							increment = -1;

							InvalidateRect(hWnd,NULL,FALSE);
						}
					}

				}

				// run forward (normal)

				if(wParam == VK_RIGHT)
				{
					if ( scene_run_flag )
					{
						frame_step_flag = 0;

						increment = 1;
					}
				}

				else
					process_keypress ( wParam );
			}

			else
				process_keypress ( wParam );

			break;

//		case WM_MOUSEMOVE:
//			{
//			if(wParam == MK_LBUTTON)
//				;
//
//			if(wParam == MK_RBUTTON)
//				;
//
//			LOWORD(lParam);  // horizontal position of cursor 
//			HIWORD(lParam);  // vertical position of cursor 
//			}
//
//			break;

		case WM_LBUTTONDOWN:
			{
// Bob. Start 06272004 Always check mouse buttons
//				if ( control_1_type == 2 )
//				if ( control_1_type == 2 || control_2_type == 2 )
//				{
//					if(wParam == MK_LBUTTON)
//					{
//						mouse_button_flag = BUTTON1;
//						mouse_button_value = mouse_button_flag;
//						mouse_button_time = get_pclock ();
//					}
//				}
// Bob. End 06272004
				if(wParam == MK_LBUTTON)
				{
					mouse_button_flag = BUTTON1;
					mouse_button_value = mouse_button_flag;
					mouse_button_time = get_pclock ();
				}
			}

			break;

		case WM_RBUTTONDOWN:
			{
// Bob. Start 06272004 Always check mouse buttons
//				if ( control_1_type == 2 )
//				if ( control_1_type == 2 || control_2_type == 2 )
//				{
//					if(wParam == MK_RBUTTON)
//					{
//						mouse_button_flag = BUTTON2;
//						mouse_button_value = mouse_button_flag;
//						mouse_button_time = get_pclock ();
//					}
//				}
				if(wParam == MK_RBUTTON)
				{
					mouse_button_flag = BUTTON2;
					mouse_button_value = mouse_button_flag;
					mouse_button_time = get_pclock ();
				}
// Bob. End 06272004
			}

			break;

		// A menu command
		case WM_COMMAND:
			{
			switch(LOWORD(wParam))
				{
				// Exit the program
				case ID_FILE_EXIT:
// Bob. Start 02282003 WCSC Code
					// close port if open
					if ( VxdPortID_Flag )
					{
						if((status=WCSCVxDCleanup(VxDPortId))!=RS232ERR_NONE)
							sprintf ( message[0], "Error %d WCSCVxDPutPacket POLHEMUS_BINARY WCSCVxDCleanup", status );
					}
// Bob. End 02282003

					DestroyWindow(hWnd);
					break;

				// Display the about box
				case ID_HELP_ABOUT:
					DialogBox (hInstance,
						MAKEINTRESOURCE(IDD_DIALOG_ABOUT),
						hWnd,
						AboutDlgProc);
					break;
				}
			}
			break;


	default:   // Passes it on if unproccessed
	    return (DefWindowProc(hWnd, imessage, wParam, lParam));

	}

    return (0L);
	}

// Choi. Start 07012000
static LONG APIENTRY MessageWndProc(HWND hWndFrame, UINT wMsg, WPARAM wParam, LONG lParam)
{
	RECT rect;
	PAINTSTRUCT ps;
	HDC hDC;
	HBRUSH BackgroundBrush;
    switch (wMsg) 
	{
		case WM_PAINT:
			// draw message
			if (hDC = BeginPaint(hWndFrame, &ps))
			{
				BackgroundBrush = CreateSolidBrush(RGB(0x00, 0x00, 0x00));
				SelectObject(hDC, BackgroundBrush);
				GetClientRect(hWndFrame, &rect);
				FillRect(hDC, &rect, BackgroundBrush);

				SetBkMode(hDC, TRANSPARENT);
				SetTextColor(hDC, RGB(0xff, 0xff, 0xff));
				SetTextAlign(hDC, TA_CENTER | VTA_CENTER);
				TextOut(hDC, (rect.right-rect.left)/2, 0, globalMessage, strlen(globalMessage));

				DeleteObject(BackgroundBrush);
			};
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWndFrame, wMsg, wParam, lParam);
    }; // end of switch block
    return 0;
};
// Choi. End 07052000

// superbible texture mapping code
/*
 * 'LoadAllTextures()' - Load texture images for the scene.
 */

void
LoadAllTextures(void)
{
  BITMAPINFO	*info;				/* Bitmap information */
  void		*bits;				/* Bitmap pixel bits */
  GLubyte	*rgb;				/* Bitmap RGB pixels */


 /*
  * Try loading the bitmap and converting it to RGB...
  */

  bits = LoadDIBitmap(TextureMapName, &info);
  if (bits == NULL)
    return;

  rgb = ConvertRGB(info, bits);
  if (rgb == NULL)
  {
    free(info);
    free(bits);

    return;
  };

  glNewList(TextureMap[TextureMapIndex] = glGenLists(1), GL_COMPILE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 052299 mipmapping code
	if ( mipmapping_flag )
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   /*
    * Define the 2D texture image.
    */

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	/* Force 4-byte alignment */
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

	// 052299 mipmapping code
	if ( mipmapping_flag )
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, info->bmiHeader.biWidth, info->bmiHeader.biHeight, GL_RGB, GL_UNSIGNED_BYTE, rgb);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, 3, info->bmiHeader.biWidth, info->bmiHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb);

  glEndList();

 /*
  * Free the bitmap and RGB images, then return 0 (no errors).
  */

  free(rgb);
  free(info);
  free(bits);
}


// Dialog procedure.
BOOL APIENTRY AboutDlgProc (HWND hDlg, UINT message, UINT wParam, LONG lParam)
	{
	
    switch (message)
	{
		// Initialize the dialog box
	    case WM_INITDIALOG:
			{
			int i;
			GLenum glError;

			// glGetString demo
			SetDlgItemText(hDlg,IDC_OPENGL_VENDOR,glGetString(GL_VENDOR));
			SetDlgItemText(hDlg,IDC_OPENGL_RENDERER,glGetString(GL_RENDERER));
			SetDlgItemText(hDlg,IDC_OPENGL_VERSION,glGetString(GL_VERSION));
			SetDlgItemText(hDlg,IDC_OPENGL_EXTENSIONS,glGetString(GL_EXTENSIONS));

			// gluGetString demo
			SetDlgItemText(hDlg,IDC_GLU_VERSION,gluGetString(GLU_VERSION));
			SetDlgItemText(hDlg,IDC_GLU_EXTENSIONS,gluGetString(GLU_EXTENSIONS));


			// Display any recent error messages
			i = 0;
			do {
				glError = glGetError();
				SetDlgItemText(hDlg,IDC_ERROR1+i,gluErrorString(glError));
				i++;
				}
			while(i < 6 && glError != GL_NO_ERROR);


			return (TRUE);
			}
			break;

		// Process command messages
	    case WM_COMMAND:      
			{
			// Validate and Make the changes
			if(LOWORD(wParam) == IDOK)
				EndDialog(hDlg,TRUE);
		    }
			break;

		// Closed from sysbox
		case WM_CLOSE:
			EndDialog(hDlg,TRUE);
			break;
		}

	return FALSE;
	}

// Choi. Start 12101999
void
DisplayGLMessage(char *message, float x, float y)
{
// ***** debug
//char	t_buff[1024];
//sprintf(t_buff, "(%ld)", pclock_ticks);
//strcat(message, t_buff);
//	glRGB(255, 255, 255);

// Choi. Start 07012000
//	glRGB(200, 200, 200);
//	glRasterPos3f(x+(float)strlen(message)*((float)vp_width*(float)-0.0042),y,-h_dist);
//	glListBase (1000); 
//	glCallLists (strlen(message), GL_UNSIGNED_BYTE, message);
	
	globalMessage = message;
	ShowWindow(hWndMessage, SW_SHOW);
	InvalidateRect(hWndMessage, NULL, TRUE);
// Choi. End 07052000
};

DisplayGLMessageColor(char *message, float x, float y, RGB* color)
{
	glRGB(color->redvalue, color->greenvalue, color->bluevalue);
	glRasterPos3f(x+(float)strlen(message)*((float)vp_width*(float)-0.0042),y,-h_dist);
	glListBase (1000); 
	glCallLists (strlen(message), GL_UNSIGNED_BYTE, message);
};

void OutputGLMessage(char *message, float x, float y, RGB* color)
{
	GLfloat coord[4];
	GLfloat origin[4];
	GLfloat ratioX;
	GLfloat ratioY;
	GLfloat offsetx;
	GLfloat offsety;
	GLfloat sx, sy;

// Choi. Start 07012000
	// ShowWindow(hWndMessage, SW_HIDE);
// Choi. End 07052000
	// get the windows coordinates to map the OpenGL Cartesian coordinates
	glRasterPos3f(0, 0, -100);
	glGetFloatv(GL_CURRENT_RASTER_POSITION, origin);

	// get the windows coordinates
	glRasterPos3f(x, y, -100);
	glGetFloatv(GL_CURRENT_RASTER_POSITION, coord);

	x = coord[0] - origin[0];
	y = coord[1] - origin[1];
	x -= (((float)strlen(message))*6.5/2.0);

	// get the 10, 10 position to get the ratio
	glRasterPos3f(10, 10, -100);
	glGetFloatv(GL_CURRENT_RASTER_POSITION, coord);

	// find the center position of the string in the windows coordinate system
	offsetx = (float)(coord[0] - origin[0]);
	offsety = (float)(coord[1] - origin[1]);

	ratioX = 1/offsetx;
	ratioY = 1/offsety;

	// convert the position of text into the Cartesian coordinates
	sx = (float)(10.0*x*ratioX);
	sy = (float)(10.0*y*ratioY);

	glRGB(header.text_color.redvalue,
		  header.text_color.greenvalue,
		  header.text_color.bluevalue);
	glRasterPos3f(sx, sy, -100);
	glListBase (1000); 
	glCallLists (strlen(message), GL_UNSIGNED_BYTE, message);
};

void OutputGLMessageXY(char *message)
{
	GLfloat coord[4];
	GLfloat origin[4];
	GLfloat ratioX;
	GLfloat ratioY;
	GLfloat offsetx;
	GLfloat offsety;

	GLfloat x = header.text_posx-(((float)strlen(message))*6.5/2.0);
	GLfloat y = header.text_posy;
	GLfloat sx;
	GLfloat sy;

// Choi. Start 07012000
	ShowWindow(hWndMessage, SW_HIDE);
// Choi. End 07052000

	// get the windows coordinates to map the OpenGL Cartesian coordinates
	glRasterPos3f(0, 0, -100);
	glGetFloatv(GL_CURRENT_RASTER_POSITION, origin);

	glRasterPos3f(10, 10, -100);
	glGetFloatv(GL_CURRENT_RASTER_POSITION, coord);

	// find the center position of the string in the windows coordinate system
	offsetx = (float)(coord[0] - origin[0]);
	offsety = (float)(coord[1] - origin[1]);

	ratioX = 1/offsetx;
	ratioY = 1/offsety;

	// convert the position of text into the Cartesian coordinates
	sx = (float)(10.0*x*ratioX);
	sy = (float)(10.0*y*ratioY);

	glRGB(header.text_color.redvalue,
		  header.text_color.greenvalue,
		  header.text_color.bluevalue);
	glRasterPos3f(sx, sy, -100);
	glListBase (1000); 
	glCallLists (strlen(message), GL_UNSIGNED_BYTE, message);
};

// Choi. End 12091999
