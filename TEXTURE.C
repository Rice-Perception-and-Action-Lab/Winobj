/*
 * "$Id: texture.c,v 1.1 1995/12/31 21:33:34 mike Exp mike $"
 *
 *   Texture image routines for MS Window and OpenGL.
 *
 * Contents:
 *
 *   TextureLoadBitmap() - Load a bitmap file and define it as a 1D or 2D texture.
 *   TextureLoadMipmap() - Load a bitmap file and define it as a 1D or 2D mipmapped-texture.
 *
 * Revision History:
 *
 *   $Log: texture.c,v $
 *   Revision 1.1  1995/12/31  21:33:34  mike
 *   Initial revision
 */

/*
 * Include necessary headers.
 */

#include "texture.h"


/*
 * 'TextureLoadBitmap()' - Load a bitmap file and define it as a 1D or 2D texture.
 *
 * Returns 0 on success or -1 on error.
 */

int
TextureLoadBitmap(char *filename)		/* I - Bitmap file to load */
{
  BITMAPINFO	*info;				/* Bitmap information */
  void		*bits;				/* Bitmap pixel bits */
  GLubyte	*rgb;				/* Bitmap RGB pixels */


 /*
  * Try loading the bitmap and converting it to RGB...
  */

  bits = LoadDIBitmap(filename, &info);
  if (bits == NULL)
    return (-1);

  rgb = ConvertRGB(info, bits);
  if (rgb == NULL)
  {
    free(info);
    free(bits);

    return (-1);
  };

 /*
  * Define the texture image.  If the width or height is 1, then we
  * define a 1D texture.
  */

  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	/* Force 4-byte alignment */
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

  if (info->bmiHeader.biHeight == 1)
  {
    glTexImage1D(GL_TEXTURE_1D, 0, 3, info->bmiHeader.biWidth, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  }
  else if (info->bmiHeader.biWidth == 1)
  {
    glTexImage1D(GL_TEXTURE_1D, 0, 3, info->bmiHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  }
  else
  {
    glTexImage2D(GL_TEXTURE_2D, 0, 3, info->bmiHeader.biWidth, info->bmiHeader.biHeight, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, rgb);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  };

 /*
  * Free the bitmap and RGB images, then return 0 (no errors).
  */

  free(rgb);
  free(info);
  free(bits);

  return (0);
}


/*
 * 'TextureLoadMipmap()' - Load a bitmap file and define it as a 1D or 2D mipmapped-texture.
 *
 * Returns 0 on success or -1 on error.
 */

int
TextureLoadMipmap(char *filename)		/* I - Bitmap file to load */
{
  BITMAPINFO	*info;				/* Bitmap information */
  void		*bits;				/* Bitmap pixel bits */
  GLubyte	*rgb;				/* Bitmap RGB pixels */


 /*
  * Try loading the bitmap and converting it to RGB...
  */

  bits = LoadDIBitmap(filename, &info);
  if (bits == NULL)
    return (-1);

  rgb = ConvertRGB(info, bits);
  if (rgb == NULL)
  {
    free(info);
    free(bits);

    return (-1);
  };

 /*
  * Define the texture image.  If the width or height is 1, then we
  * define a 1D texture.
  */

  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	/* Force 4-byte alignment */
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

  if (info->bmiHeader.biHeight == 1)
  {
    gluBuild1DMipmaps(GL_TEXTURE_1D, 3, info->bmiHeader.biWidth, GL_RGB, GL_UNSIGNED_BYTE, rgb);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  }
  else if (info->bmiHeader.biWidth == 1)
  {
    gluBuild1DMipmaps(GL_TEXTURE_1D, 3, info->bmiHeader.biHeight, GL_RGB, GL_UNSIGNED_BYTE, rgb);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  }
  else
  {
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, info->bmiHeader.biWidth, info->bmiHeader.biHeight,
                      GL_RGB, GL_UNSIGNED_BYTE, rgb);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  };

 /*
  * Free the bitmap and RGB images, then return 0 (no errors).
  */

  free(rgb);
  free(info);
  free(bits);

  return (0);
}


/*
 * End of "$Id: texture.c,v 1.1 1995/12/31 21:33:34 mike Exp mike $".
 */
