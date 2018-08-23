/*
 * "$Id$"
 *
 *   OpenGL 2D texture demo.
 *
 * Contents:
 *
 *   WinMain()             - Main entry...
 *   DisplayErrorMessage() - Display an error message dialog.
 *   MakePalette()         - Make a color palette for RGB colors if
 *                           necessary.
 *   SceneProc()           - Handle window events in the viewing window.
 *   LoadAllTextures()     - Load texture images for the scene.
 *   RepaintWindow()       - Redraw the client area with our scene.
 *
 * Revision History:
 *
 *   $Log$
 */

/*
 * Include necessary headers.
 */

#include "texture.h"
#include "tex2d.h"
#include <stdarg.h>
#include <math.h>
#ifndef M_PI
#  define M_PI 3.141592649
#endif /* !M_PI */


/*
 * Globals...
 */

HWND		SceneWindow;		/* Scene window */
HPALETTE	ScenePalette;		/* Color palette (if necessary) */
HDC		SceneDC;		/* Drawing context */
HGLRC		SceneRC;		/* OpenGL rendering context */

GLuint		SkyTexture;		/* Sky texture image */


/*
 * Local functions...
 */

void			DisplayErrorMessage(char *, ...);
void			MakePalette(int);
LRESULT CALLBACK	SceneProc(HWND, UINT, WPARAM, LPARAM);
void			LoadAllTextures(void);
void			RepaintWindow(RECT *);
void			SaveBitmapFile(void);
void			PrintBitmap(void);


/*
 * 'WinMain()' - Main entry...
 */

int APIENTRY
WinMain(HINSTANCE hInst,		/* I - Current process instance */
	HINSTANCE hPrevInstance,	/* I - Parent process instance */
	LPSTR     lpCmdLine,		/* I - Command-line arguments */
	int       nCmdShow)		/* I - Show window at startup? */
{
  MSG		msg;			/* Window UI event */
  WNDCLASS	wc;                     /* Window class */


 /*
  * Register main window...
  */

  wc.style         = 0;
  wc.lpfnWndProc   = (WNDPROC)SceneProc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = hInst;
  wc.hIcon         = NULL;
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = 0;         
  wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1);
  wc.lpszClassName = "2D Texture Demo";

  if (RegisterClass(&wc) == 0)
  {
    DisplayErrorMessage("Unable to register window class!");
    return (FALSE);
  };

 /*
  * Then create it...
  */

  SceneWindow = CreateWindow("2D Texture Demo", "2D Texture Demo",
                            WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
  			    32, 32, 256, 256,
			    NULL, NULL, hInst, NULL);

  if (SceneWindow == NULL)
  {
    DisplayErrorMessage("Unable to create window!");
    return (FALSE);
  };

  ShowWindow(SceneWindow, nCmdShow);
  UpdateWindow(SceneWindow);

 /*
  * Loop on events until the user quits this application...
  */

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  };

  return (msg.wParam);
}


/*
 * 'DisplayErrorMessage()' - Display an error message dialog.
 */

void
DisplayErrorMessage(char *format,	/* I - printf() style format string */
                    ...)		/* I - Other arguments as necessary */
{
  va_list	ap;			/* Argument pointer */
  char		s[1024];		/* Output string */


  if (format == NULL)
    return;

  va_start(ap, format);
  vsprintf(s, format, ap);
  va_end(ap);

  MessageBeep(MB_ICONEXCLAMATION);
  MessageBox(NULL, s, "Error", MB_OK | MB_ICONEXCLAMATION);
}


/*
 * 'MakePalette()' - Make a color palette for RGB colors if necessary.
 */

void
MakePalette(int pf)			/* I - Pixel format ID */
{
  PIXELFORMATDESCRIPTOR pfd;		/* Pixel format information */
  LOGPALETTE		*pPal;		/* Pointer to logical palette */
  int			nColors;	/* Number of entries in palette */
  int			i,		/* Color index */
			rmax,		/* Maximum red value */
			gmax,		/* Maximum green value */
			bmax;		/* Maximum blue value */


 /*
  * Find out if we need to define a color palette...
  */

  DescribePixelFormat(SceneDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

  if (!(pfd.dwFlags & PFD_NEED_PALETTE))
  {
    ScenePalette = NULL;
    return;
  };

 /*
  * Allocate memory for a color palette...
  */

  nColors = 1 << pfd.cColorBits;	

  pPal = (LOGPALETTE *)malloc(sizeof(LOGPALETTE) +
                              nColors * sizeof(PALETTEENTRY));

  pPal->palVersion    = 0x300;
  pPal->palNumEntries = nColors;

 /*
  * Get the maximum values for red, green, and blue.  Then build 'nColors'
  * colors...
  */

  rmax = (1 << pfd.cRedBits) - 1;
  gmax = (1 << pfd.cGreenBits) - 1;
  bmax = (1 << pfd.cBlueBits) - 1;

  for (i = 0; i < nColors; i ++)
  {
    pPal->palPalEntry[i].peRed   = 255 * ((i >> pfd.cRedShift) & rmax) / rmax;
    pPal->palPalEntry[i].peGreen = 255 * ((i >> pfd.cGreenShift) & gmax) / gmax;
    pPal->palPalEntry[i].peBlue  = 255 * ((i >> pfd.cBlueShift) & bmax) / bmax;

    pPal->palPalEntry[i].peFlags = 0;
  };
		
 /*
  * Create, select, and realize the palette...
  */

  ScenePalette = CreatePalette(pPal);
  SelectPalette(SceneDC, ScenePalette, FALSE);
  RealizePalette(SceneDC);

  free(pPal);
}


/*
 * 'SceneProc()' - Handle window events in the viewing window.
 */

LRESULT CALLBACK
SceneProc(HWND   hWnd,		/* I - Window triggering this event */
	 UINT   uMsg,		/* I - Message type */
	 WPARAM wParam,		/* I - 'word' parameter value */
	 LPARAM lParam)		/* I - 'long' parameter value */
{
  int			pf;	/* Pixel format ID */
  PIXELFORMATDESCRIPTOR pfd;	/* Pixel format information */
  PAINTSTRUCT		ps;	/* WM_PAINT message info */
  RECT			rect;	/* Current client area rectangle */


  switch (uMsg)
  {
    case WM_CREATE :
       /*
        * 'Create' message.  Get device and rendering contexts, and
	* setup the client area for OpenGL drawing...
	*/

	SceneDC = GetDC(hWnd);              

        pfd.nSize        = sizeof(pfd);
        pfd.nVersion     = 1;
        pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
						/* Do OpenGL drawing */
        pfd.dwLayerMask  = PFD_MAIN_PLANE;	/* Main drawing plane */
        pfd.iPixelType   = PFD_TYPE_RGBA;	/* RGB color buffer */
        pfd.cColorBits   = 0;			/* Best color buffer please */
        pfd.cDepthBits   = 16;			/* Need a depth buffer */
        pfd.cStencilBits = 0;			/* No stencil buffer */
        pfd.cAccumBits   = 0;			/* No accumulation buffer */

        pf = ChoosePixelFormat(SceneDC, &pfd);
	if (pf == 0)
          DisplayErrorMessage("texscene was unable to choose a suitable pixel format!");
        else if (!SetPixelFormat(SceneDC, pf, &pfd))
          DisplayErrorMessage("texscene was unable to set the pixel format!");

        MakePalette(pf);

        SceneRC = wglCreateContext(SceneDC);
        wglMakeCurrent(SceneDC, SceneRC);

        LoadAllTextures();
	break;

    case WM_SIZE :
    case WM_PAINT :
       /*
        * Repaint the client area with our bitmap...
	*/

	BeginPaint(hWnd, &ps);

        GetClientRect(hWnd, &rect);
	RepaintWindow(&rect);

	EndPaint(hWnd, &ps);
	break;

    case WM_COMMAND :
       /*
        * Handle menu selections...
	*/

	switch (LOWORD(wParam))
	{
	  case IDM_FILE_EXIT :
// Bob. Start 02282003 WCSC Code
		// close port if open
		if ( VxdPortID_Flag )
		{
			if((status=WCSCVxDCleanup(VxDPortId))!=RS232ERR_NONE)
				sprintf ( message[0], "Error %d WCSCVxDPutPacket POLHEMUS_BINARY WCSCVxDCleanup", status );
		}
// Bob. End 02282003
	      DestroyWindow(SceneWindow);
	      break;
	};
        break;

    case WM_QUIT :
    case WM_CLOSE :
       /*
        * Destroy the viewing window and exit...
        */
        
// Bob. Start 02282003 WCSC Code
		// close port if open
		if ( VxdPortID_Flag )
		{
			if((status=WCSCVxDCleanup(VxDPortId))!=RS232ERR_NONE)
				sprintf ( message[0], "Error %d WCSCVxDPutPacket POLHEMUS_BINARY WCSCVxDCleanup", status );
		}
// Bob. End 02282003
        DestroyWindow(SceneWindow);
	exit(0);
        break;

    case WM_DESTROY :
       /*
        * Release and free the device context and rendering context
	*/
  	if (SceneRC)
    	  wglDeleteContext(SceneRC);

    	if (SceneDC)
    	  ReleaseDC(SceneWindow, SceneDC);

	if (ScenePalette)
	  DeleteObject(ScenePalette);

        PostQuitMessage(0);
        break;

    case WM_QUERYNEWPALETTE :
       /*
        * Realize the color palette if necessary...
	*/

        if (ScenePalette)
	{
          SelectPalette(SceneDC, ScenePalette, FALSE);
          RealizePalette(SceneDC);

          InvalidateRect(hWnd, NULL, FALSE);
	  return (TRUE);
	};
        break;
	
    case WM_PALETTECHANGED:
       /*
        * Reselect our color palette if necessary...
	*/

	if (ScenePalette && (HWND)wParam != hWnd)
	{
          SelectPalette(SceneDC, ScenePalette, FALSE);
          RealizePalette(SceneDC);

          UpdateColors(SceneDC);
	};
	break;

    default :
       /*
        * Pass all other messages through the default window
        * procedure...
        */

        return (DefWindowProc(hWnd, uMsg, wParam, lParam));
  };

  return (FALSE);
}


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

  bits = LoadDIBitmap("sky.bmp", &info);
  if (bits == NULL)
    return;

  rgb = ConvertRGB(info, bits);
  if (rgb == NULL)
  {
    free(info);
    free(bits);

    return;
  };

  glNewList(SkyTexture = glGenLists(1), GL_COMPILE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   /*
    * Define the 2D texture image.
    */

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	/* Force 4-byte alignment */
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, info->bmiHeader.biWidth, info->bmiHeader.biHeight, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, rgb);
  glEndList();

 /*
  * Free the bitmap and RGB images, then return 0 (no errors).
  */

  free(rgb);
  free(info);
  free(bits);
}


/*
 * 'RepaintWindow()' - Redraw the client area with our scene.
 */

void
RepaintWindow(RECT *rect)	/* I - Client area rectangle */
{
  GLfloat	x, y, s, t, th;


 /*
  * Reset the viewport and clear the window to black...
  */

  glViewport(0, 0, rect->right, rect->bottom);

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, (float)rect->right / (float)rect->bottom, 0.1, 1000.0);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glRotatef(10.0, 0.0, 1.0, 0.0);
    glTranslatef(0.0, -40.0, -100.0);

   /*
    * First draw the ground...
    */

    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0, 0.8, 0.0);
    glBegin(GL_POLYGON);
      for (th = 0.0; th < (2.0 * M_PI); th += (0.03125 * M_PI))
      {
        x = cos(th) * 200.0;
	y = sin(th) * 200.0;
	glTexCoord2f(x * 0.5, y * 0.5);
	glVertex3f(x, 0.0, y);
      };
    glEnd();

   /*
    * Then the sky...
    */

    glEnable(GL_TEXTURE_2D);
    glCallList(SkyTexture);
    glBegin(GL_QUAD_STRIP);
      for (th = 0.0; th < (2.0 * M_PI); th += (0.03125 * M_PI))
      {
        x = cos(th) * 200.0;
	y = sin(th) * 200.0;

	if (th > M_PI)
	  s = 2.0  - th / M_PI;
	else
	  s = th / M_PI;

	glTexCoord2f(s, 0.0);
	glVertex3f(x, 0.0, y);

	glTexCoord2f(s, 0.8);
	glVertex3f(x * 0.75, 150.0, y * 0.75);
      };

      glTexCoord2f(0.0, 0.0);
      glVertex3f(200.0, 0.0, 0.0);

      glTexCoord2f(0.0, 0.8);
      glVertex3f(150.0, 150.0, 0.0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5, 1.0);
      glVertex3f(0.0, 200.0, 0.0);

      for (th = 0.0; th < (2.0 * M_PI); th += (0.03125 * M_PI))
      {
        x = cos(th) * 150.0;
	y = sin(th) * 150.0;

	if (th > M_PI)
	  s = 2.0  - th / M_PI;
	else
	  s = th / M_PI;

	glTexCoord2f(s, 0.8);
	glVertex3f(x, 150.0, y);
      };
    glEnd();
  glPopMatrix();

  glFinish();
}


/*
 * End of "$Id$".
 */
