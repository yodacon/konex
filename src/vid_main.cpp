/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: vid_main.cpp,v 1.21 2005/08/14 06:56:45 ecco Exp $   *
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
// Implementation Private Includes
/////////////////////////////////////////////////////////////////////////////
#include "defs.h"

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Definitions, Enumeration and Typedefs
/////////////////////////////////////////////////////////////////////////////
#define MAX_IMAGES 1024
#define SUB_SYSTEM vidl

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Variables
/////////////////////////////////////////////////////////////////////////////
static vid_image_t vidImages[MAX_IMAGES];
static int vidFont;

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////

int vidWidth, vidHeight;
bool vidFullscreen;

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Functions
/////////////////////////////////////////////////////////////////////////////
static unsigned int getFreeImage()
{
	unsigned int i;
	for (i = 1; i < MAX_IMAGES; i++)
	{
		if (vidImages[i].used == false)
			return i;
	}

	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void vid_Init(int width, int height, bool fullscreen)
{
#if defined(MACOS)
	vidsdl_Init(width, height, fullscreen);
	vidFont = vidsdl_CreateFont();
#elif defined(LINUX)
	vidl_Init(width, height, fullscreen);
	vidFont = vidl_CreateFont();
#else
	vidw_Init(width, height, fullscreen);
	vidFont = vidw_CreateFont();
#endif

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Debugging
	console_Print("VID: Video system is operational.");
	console_Print(" - Vendor (%s)", glGetString(GL_VENDOR));
	console_Print(" - Version (%s)", glGetString(GL_VERSION));
	console_Print(" - Renderer (%s)", glGetString(GL_RENDERER));

	vidWidth = width;
	vidHeight = height;
	vidFullscreen = fullscreen;

	vid_Resize(width, height);
}

void vid_SetMode(int width, int height, bool fullscreen)
{
#if defined(MACOS)
	vidsdl_SetMode(width, height, fullscreen);
#elif defined(LINUX)
	vidl_SetMode(width, height, fullscreen);
#else
	//vidw_SetMode(fullscreen);
#endif

	vidWidth = width;
	vidHeight = height;

	vid_Resize(width, height);
}


void vid_DrawString(int x, int y, unsigned char alpha, char *s, ...)
{
	// Don't draw images outside viewable area
	if (x > vidWidth)
		return;
	if (y > vidHeight)
		return;
	if (x + 100 < 2)
		return;
	if (y + 20 < 0)
		return;

	static char textBuffer[2048];
	va_list args;
	va_start(args, s);
	vsprintf(textBuffer, s, args);
	va_end(args);

	//glDisable(GL_BLEND);
	glColor4f(1.0f,1.0f,1.0f,alpha / 255.0f);
    glBindTexture(GL_TEXTURE_2D, 0);
	glRasterPos2f(x + 2,y + 11);
	glPushAttrib (GL_LIST_BIT);
	glListBase(vidFont);
	glCallLists(strlen(textBuffer), GL_UNSIGNED_BYTE, (GLubyte *)textBuffer);
	glPopAttrib ();
	//glEnable(GL_BLEND);
}

void vid_Rect(int x, int y, int width, int height, vid_color_t color, byte a)
{
	// Don't draw images outside viewable area
	if (x > vidWidth)
		return;
	if (y > vidHeight)
		return;
	if (x + width < 0)
		return;
	if (y + height < 0)
		return;

	glColor4f(color.red / 255.0, color.green / 255.0, color.blue / 255.0, a / 255.0f);
    glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);
		glVertex3f(x, y, 1.0f);
		glVertex3f(x + width, y, 1.0f);
		glVertex3f(x + width, y + height, 1.0f);
		glVertex3f(x, y + height, 1.0f);
	glEnd();
}

unsigned int vid_LoadImage(char* filename)	
{
	unsigned int imageid = getFreeImage();
	vidImages[imageid].used = true;

	if (!image_LoadTarga(filename, &vidImages[imageid]))
	{
		console_Print("VID: Error loading %s", filename);
		return 0;
	}

	unsigned int id;  //= (unsigned int)vidImages[imageid].id;
	glGenTextures( 1, &id);

	vidImages[imageid].id = id;

	glBindTexture(GL_TEXTURE_2D, vidImages[imageid].id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, vidImages[imageid].width, vidImages[imageid].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, vidImages[imageid].data);

	return imageid;
}

int vid_GetImageWidth(unsigned int imageid)
{
	return vidImages[imageid].width;
}

int vid_GetImageHeight(unsigned int imageid)
{
	return vidImages[imageid].height;
}

void vid_DrawImage(int x, int y, unsigned int imageid, bool centered)
{
	vid_DrawImage(x, y, vidImages[imageid].width, vidImages[imageid].height, imageid, centered);
}

void vid_DrawImage(int x, int y, byte alpha, unsigned int imageid, bool centered)
{
	vid_DrawImage(x, y, vidImages[imageid].width, vidImages[imageid].height, alpha, imageid, centered);
}

void vid_DrawImage(int x, int y, int width, int height, unsigned int imageid, bool centered)
{
	vid_DrawImage(x, y, width, height, 255, imageid, centered);
}

void vid_DrawImage(int x, int y, int width, int height, byte alpha, unsigned int imageid, bool centered)
{
	if (centered == true)
	{
		x -= vid_GetImageWidth(imageid) / 2;
		y -= vid_GetImageHeight(imageid) / 2;
	}

	// Don't draw images outside viewable area
	if (x > vidWidth)
		return;
	if (y > vidHeight)
		return;
	if (x + width < 0)
		return;
	if (y + height < 0)
		return;

	// Set color to white with specified alpha
	glColor4f(1.0f, 1.0f, 1.0f, alpha / 255.0);

    glBindTexture(GL_TEXTURE_2D, vidImages[imageid].id);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x, y, 1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x + width, y, 1.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x + width, y + height, 1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x, y + height, 1.0f);
	glEnd();
}

vid_color_t	vid_Color(unsigned char r, unsigned char g, unsigned char b)
{
	vid_color_t color;
	color.red = r; color.green = g; color.blue = b;
	return color;
}

void vid_Line(int x1, int y1, int x2, int y2, vid_color_t color)
{
    glBindTexture(GL_TEXTURE_2D, 0);
	glColor4f(color.red / 255.0, color.green / 255.0, color.blue / 255.0, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(x1, y1, 1.0f);
	glVertex3f(x2, y2, 1.0f);
	glEnd();
}

void vid_PSet(int x, int y, vid_color_t color)
{
    glBindTexture(GL_TEXTURE_2D, 0);
	glColor4f(color.red / 255.0, color.green / 255.0, color.blue / 255.0, 1.0f);
	glBegin(GL_POINTS);
	glVertex3f(x, y, 1.0f);
	glEnd();
}

void vid_Frame(void)
{
#if defined(MACOS)
	vidsdl_Swap();
#elif defined(LINUX)
	vidl_Swap();
#else
	vidw_Swap();
#endif

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslatef( 0.0f, 0.0f, -5.0f );
}

void vid_Resize(int width, int height)
{
	glViewport( 0, 0, width, height );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, width, height, 0.0f, -1500.0, 1500.0);
}

void vid_Bmp2tga(char *filename)
{
	vid_image_t vid;
	char newfile[1024];

	image_LoadBitmap(filename, &vid);
	image_RGB2BGRA(&vid);

	/*int i,j;
	int num = 0;
	for (i = 5; i >= 0; i--)
	{
		for (j = 0; j < 6; j++)
		{*/
			sprintf(newfile, "%s.tga", filename);
			//num++;
			//sprintf(newfile, "00.tga");

			//image_Crop(&vid, &vidnew, j * 88, i * 88, 88, 88);
			image_Resize(&vid, 128, 128);

			//image_Crop(&vid, &vidnew, j * 72, i * 72, 72, 72);

			image_SaveTarga(newfile, &vid);	
		/*}
	}*/
}

void vid_Shutdown()
{
}
