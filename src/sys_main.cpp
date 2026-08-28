/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: sys_main.cpp,v 1.20 2005/08/09 10:34:12 ecco Exp $   *
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

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////
bool sysKeys[1024];
bool sysMouseButtons[5];
int sysMouseX, sysMouseY;
int sysRunning = 1;
int sysActive = 1;
char sysAppPath[1024];

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Functions
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void sys_Init()
{
	console_Print("**************************************************************************");
	console_Print("* Welcome to %s...                                                    *", SYS_APPNAME);
	console_Print("**************************************************************************");

	config_Init("config.xml");
	config_Load();

	timer_Init();
    vid_Init(1024, 768, true);
//	snd_Init();
	in_Init();
//	net_Init();

	console_Print("**************************************************************************");
	console_Print("* Engine initialization complete...                                      *");
	console_Print("**************************************************************************");

	konex_Init();

	// Lauch the main function
#if defined(WIN32)
	vidw_Main();
#elif defined(MACOS)
	vidsdl_Main();
#else
	vidl_Main();
#endif
}

void sys_Frame()
{
	timer_Frame();
	vid_Frame();
//	snd_Frame();
	in_Frame();
//	net_Frame();

	konex_Frame();
}

void sys_Shutdown()
{
//	net_Shutdown();
	in_Shutdown();
//	snd_Shutdown();
	vid_Shutdown();

	console_Print("**************************************************************************");
	console_Print("* Engine shutdown complete...                                            *");
	console_Print("**************************************************************************");

	exit(0);
}

int sys_Rand(int floor, int ceiling)
{
	int result = (rand() % ceiling) + floor;
	return result;
}
