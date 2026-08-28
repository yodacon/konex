/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: vid_sdl.cpp,v 1.1 2005/07/25 18:37:16 ecco Exp $   *
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
#include <SDL.h>

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Variables
/////////////////////////////////////////////////////////////////////////////
static SDL_Window    *sdlWindow  = NULL;
static SDL_GLContext  sdlContext = NULL;

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void vidsdl_Init(int width, int height, bool fullscreen)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		console_Print("VID_SDL: Video initialization failed: (%s)", SDL_GetError());
		exit(1);
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,  24);
	// The engine is fixed-function GL 1.x, so ask for a compatibility context.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	if (fullscreen == true)
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

	sdlWindow = SDL_CreateWindow(SYS_APPNAME,
	                             SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                             width, height, flags);
	if (!sdlWindow)
	{
		console_Print("VID_SDL: Window creation failed: (%s)", SDL_GetError());
		exit(1);
	}

	sdlContext = SDL_GL_CreateContext(sdlWindow);
	if (!sdlContext)
	{
		console_Print("VID_SDL: GL context creation failed: (%s)", SDL_GetError());
		exit(1);
	}

	SDL_GL_MakeCurrent(sdlWindow, sdlContext);
	SDL_GL_SetSwapInterval(1);
}

void vidsdl_SetMode(int width, int height, bool fullscreen)
{
	if (!sdlWindow) return;

	SDL_SetWindowFullscreen(sdlWindow, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	if (fullscreen == false)
		SDL_SetWindowSize(sdlWindow, width, height);
}

// TODO(port): the X11 backend built these display lists with glXUseXFont, which
// has no macOS equivalent. The lists are allocated and left empty so the engine
// links and runs; text renders as nothing until a real bitmap font is uploaded.
int vidsdl_CreateFont()
{
	int font = glGenLists(256);

	for (int i = 0; i < 256; i++)
	{
		glNewList(font + i, GL_COMPILE);
		glBitmap(0, 0, 0, 0, 8, 0, NULL);   // advance the raster pos, draw nothing
		glEndList();
	}

	return font;
}

void vidsdl_Swap()
{
	SDL_GL_SwapWindow(sdlWindow);
}

void vidsdl_Main()
{
	SDL_Event event;
	bool done = false;

	while (!done)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_WINDOWEVENT:
					if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
						vid_Resize(event.window.data1, event.window.data2);
					break;
				case SDL_KEYDOWN:
					sysKeys[event.key.keysym.scancode & 0xFF] = true;
					break;
				case SDL_KEYUP:
					sysKeys[event.key.keysym.scancode & 0xFF] = false;
					break;
				case SDL_MOUSEMOTION:
					sysMouseX = event.motion.x;
					sysMouseY = event.motion.y;
					break;
				case SDL_MOUSEBUTTONDOWN:
					sysMouseButtons[event.button.button] = true;
					break;
				case SDL_MOUSEBUTTONUP:
					sysMouseButtons[event.button.button] = false;
					break;
				case SDL_QUIT:
					done = true;
					break;
				default:
					break;
			}
		}
		sys_Frame();
	}

	SDL_GL_DeleteContext(sdlContext);
	SDL_DestroyWindow(sdlWindow);
	SDL_Quit();
}
