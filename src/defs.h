/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: defs.h,v 1.30 2005/08/16 22:55:28 ecco Exp $   *
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
// Interface Definitions, Enumeration and Typedefs
/////////////////////////////////////////////////////////////////////////////

// Determine OS
#if defined(__APPLE__)
	#define MACOS
#elif defined(WIN32)
	/* windows */
#else
	#define LINUX
#endif

// OS Specific headers
#ifdef MACOS
	#include <sys/time.h>
	#include <SDL.h>
#elif defined(LINUX)
	#include <sys/time.h>
	#include <X11/keysym.h>
	#include <X11/Xlib.h>
	#include <GL/glx.h>
	#include <AL/al.h>
	#include <AL/alu.h>
	#include <AL/alut.h>
#else
	#include <windows.h>
#endif

// Generic headers
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#ifdef MACOS
	#define GL_SILENCE_DEPRECATION
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif
#include <math.h>

// OS Dependant defines
#if defined(LINUX) || defined(MACOS)
	#define SYS_APPNAME "Konex"
#else
	#define SYS_APPNAME "Conex"
#endif

// Generic defines
#define SYS_APPVERSION "V0.9.5"

#define byte unsigned char
#define dword unsigned long
#define sdword signed long
#define word unsigned int
#define sword signed int

#ifndef WIN32
	#define MAX_PATH 128
#endif

#define PI 3.1415926535

#define FALSE 0
#define TRUE 1

//////////////////////////
// Base engine functions
//////////////////////////

// XML Support
#include "tinyxml/tinyxml.h"
#include "xml.h"
// Coordinate System
#include "vector.h"

#include "list_c.h"

// Time System
#include "tm_main.h"
// Video System
#include "vid_main.h"
#include "vid_image.h"
#include "vid_model.h"
// Generic system
#include "sys_main.h"
// Input
#include "in_main.h"
// Sound
//#include "snd_main.h"
// Nework
#include "net_main.h"

//////////////////////////
// Game engine functions
//////////////////////////

#include "entity.h"

#include "spawn.h"
#include "player.h"
#include "planets.h"
#include "missile.h"
#include "explosion.h"
#include "item.h"

#include "interface.h"
#include "menu.h"
#include "config.h"
#include "stars.h"
#include "konex.h"
#include "map.h"
#include "console.h"
#include "ships.h"
#include "view.h"
#include "target.h"
#include "ai.h"
#include "game.h"

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interface Function
/////////////////////////////////////////////////////////////////////////////
