// Function prototypes for file glwind.c

extern void ChangeSize(GLsizei w, GLsizei h);
extern void SetupRC(void);
extern void RenderScene(void);
extern void SetDCPixelFormat(HDC hDC);
extern HPALETTE GetOpenGLPalette(HDC hDC);
extern int APIENTRY WinMain( HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
extern void DisplayErrorMessage(char *format, ...);
extern void DisplayInfoMessage(char *format, ...);
extern LRESULT CALLBACK WndProc( HWND hWnd, UINT imessage, WPARAM wParam, LPARAM lParam);
extern void LoadAllTextures(void);
extern BOOL APIENTRY AboutDlgProc (HWND hDlg, UINT message, UINT wParam, LONG lParam);
extern void DisplayGLMessage(char *message, float x, float y);

// Choi. Start 12081999
extern void OutputGLMessage(char*, float, float, RGB*);
DisplayGLMessageColor(char *, float, float, RGB*);
extern void OutputGLDefaultMessage(char*, float, float, RGB*);
void OutputGLMessageXY(char*);
// Choi. End 12081999

// Choi. Start 07012000
extern LONG APIENTRY MessageWndProc(HWND, UINT, WPARAM, LONG);
// Choi. End 07052000
