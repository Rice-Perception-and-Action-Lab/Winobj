/*
 * "$Id: texture.h,v 1.1 1995/12/31 21:33:34 mike Exp mike $"
 *
 *   Texture image routines for MS Window and OpenGL.
 *
 * Revision History:
 *
 *   $Log: texture.h,v $
 *   Revision 1.1  1995/12/31  21:33:34  mike
 *   Initial revision
 */

/*
 * Include necessary headers.
 */

#ifndef _TEXTURE_H_
#  define _TEXTURE_H_

#  include <stdio.h>
#  include <stdlib.h>
#  include <errno.h>
#  include <windows.h>

#  include <GL/gl.h>
#  include <GL/glu.h>
#  include "bitmap.h"


/*
 * Make this header file work with C and C++ source code...
 */

#  ifdef __cplusplus
extern "C" {
#  endif /* __cplusplus */

extern int	TextureLoadBitmap(char *filename);
extern int	TextureLoadMipmap(char *filename);

#  ifdef __cplusplus
}
#  endif /* __cplusplus */
#endif /* !_TEXTURE_H_ */

/*
 * End of "$Id: texture.h,v 1.1 1995/12/31 21:33:34 mike Exp mike $".
 */
