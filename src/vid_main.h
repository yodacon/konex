/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: vid_main.h,v 1.11 2005/08/08 22:06:26 ecco Exp $   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// Interface Includes
/////////////////////////////////////////////////////////////////////////////
#if defined(MACOS)
	#include "vid_sdl.h"
#elif defined(LINUX)
	#include "vid_linux.h"
	#include "vid_sdl.h"
#else
	#include "vid_win32.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// Interface Definitions, Enumeration and Typedefs
/////////////////////////////////////////////////////////////////////////////
struct vid_image_t
{
	int width;
	int height;
	int bpp;
	dword id;
	bool used;
	byte *data;
};

struct vid_color_t
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////
extern int vidWidth, vidHeight;
extern bool vidFullscreen;

/////////////////////////////////////////////////////////////////////////////
// Interface Function
/////////////////////////////////////////////////////////////////////////////
void			vid_Init(int width, int height, bool fullscreen);
void 			vid_SetMode(int width, int height, bool fullscreen);
void			vid_Frame(void);
void			vid_Shutdown(void);

unsigned int	vid_LoadImage(char* filename);
int 			vid_GetImageWidth(unsigned int imageid);
int 			vid_GetImageHeight(unsigned int imageid);

void			vid_DrawImage(int x, int y, unsigned int imageid, bool centered);
void 			vid_DrawImage(int x, int y, unsigned char alpha, unsigned int imageid, bool centered);
void 			vid_DrawImage(int x, int y, int width, int height, unsigned int imageid, bool centered);
void 			vid_DrawImage(int x, int y, int width, int height, byte alpha, unsigned int imageid, bool centered);


vid_color_t		vid_Color(unsigned char r, unsigned char g, unsigned char b);

void 			vid_Line(int x1, int y1, int x2, int y2, vid_color_t color);
void 			vid_Rect(int x, int y, int width, int height, vid_color_t color, byte a);
void			vid_PSet(int x, int y, vid_color_t color);
void 			vid_DrawString(int x, int y, unsigned char alpha, char *s, ...);

void 			vid_Resize(int width, int height);

void			vid_Bmp2tga(char *filename);
