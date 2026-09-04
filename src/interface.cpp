/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: interface.cpp,v 1.36 2005/08/16 22:55:28 ecco Exp $           *
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
//#define MAX_WINDOWS 64
#define WINDOW_TITLE_HEIGHT 15
#define WINDOW_PADDING 5

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Variables
/////////////////////////////////////////////////////////////////////////////
//window windows[MAX_WINDOWS];
list_c<window>windows;

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////
item_c<window>* interfaceMiniMap;
item_c<window>* interfaceTargeting;
item_c<window>* interfaceFullMap;
item_c<window>* interfaceSelectShip;
item_c<window>* interfaceHUD;
item_c<window>* interfacePlanet;
item_c<window>* interfaceMenu;
item_c<window>* interfaceFPS;
item_c<window>* interfaceViewSprite;
item_c<window>* interfaceViewSpriteID;

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Functions
/////////////////////////////////////////////////////////////////////////////
static void drawWindows()
{
	dword i;
	vid_color_t framecolor = vid_Color(100, 100, 100);
	vid_color_t backcolor = vid_Color(0, 0, 0);
	item_c<window>* win;
	window* w;

	win = windows.firstitem;

	while (win != 0)
	{
		w = &win->item;
		if (w->used == true && w->visible == true)
		{
			// Update windows coordinates
			w->x = w->sx + WINDOW_PADDING;
			w->y = w->sy + WINDOW_PADDING + WINDOW_TITLE_HEIGHT;
			w->height = w->sheight - WINDOW_PADDING * 2 - WINDOW_TITLE_HEIGHT;
			w->width = w->swidth - WINDOW_PADDING * 2;

			// Draw the window frame
			vid_Line(w->sx, w->sy, 
					w->sx, w->sy + w->sheight, framecolor);
			vid_Line(w->sx, w->sy, 
					w->sx + w->swidth, w->sy, framecolor);
			vid_Line(w->sx + w->swidth, w->sy + w->sheight, 
					w->sx + w->swidth, w->sy, framecolor);
			vid_Line(w->sx + w->swidth, w->sy + w->sheight, 
					w->sx, w->sy + w->sheight, framecolor);

			// Draw the background
			vid_Rect(w->sx + 1, w->sy, w->swidth - 1, w->sheight - 1, backcolor, 192);

			// Call draw command
			if (w->draw2)			
				w->draw2(w);

			// Draw the window title
			vid_DrawString(w->sx, w->sy, 192, w->caption);

			vid_Line(w->sx, w->sy + WINDOW_TITLE_HEIGHT, 
					w->sx + w->swidth, w->sy + WINDOW_TITLE_HEIGHT, framecolor);

			// Close button
			int closealpha;

			if (w->mousex >= w->swidth - 12 &&
				w->mousex <= w->swidth - 2 &&
				w->mousey >= 2 &&
				w->mousey <= 12)
				closealpha = 255;
			else
				closealpha = 128;

			vid_Rect(w->sx + w->swidth - 12, w->sy + 2, 10, 10, framecolor, closealpha);
		}

		win = win->nextitem;
	}
}

static void drawHUD(window *win)
{
	vid_DrawString(win->x, win->y, 255, "Red: %d", gameRedTeam);
	vid_DrawString(win->x, win->y + 20, 255, "Green: %d", gameGreenTeam);
	vid_DrawString(win->x, win->y + 40, 255, "Blue: %d", gameBlueTeam);

	vid_DrawString(win->x, win->y + 80, 255, "Frags: %d", gameMainPlayer->frags);
	vid_DrawString(win->x, win->y + 100, 255, "Deaths: %d", gameMainPlayer->deaths);
	vid_DrawString(win->x, win->y + 120, 255, "Health: %d", gameMainPlayer->health);
	vid_DrawString(win->x, win->y + 140, 255, "Money: %d", gameMainPlayer->money);
}

static void drawPlanet(window *win)
{
//	vid_DrawImage(win->x + 32, win->y + 32, planets[gameMainPlayer->planetid].viewimageid, false);
	vid_DrawString(256 + win->x + 64, win->y + 32, 255, "Planet: %d", gameMainPlayer->planetid);
	vid_DrawString(256 + win->x + 64, win->y + 50, 255, "Population: 100");
	vid_DrawString(256 + win->x + 64, win->y + 72, 255, "Government: SAA");
}

static void drawSelectShip(window *win)
{
	#define ITEM_SIZE 128
	#define ITEM_PADDINGX 50
	#define ITEM_PADDINGY 50
	#define BACKGROUND_ALPHA 160
	#define INACTIVE_ALPHA 128
	#define ACTIVE_ALPHA 255

	static int selecteditem;

	int panewidth = win->width;
	int paneheight = win->height;

	// Clear the selected item
	selecteditem = -1;

	// Draw all ships
	int curx = win->x + ITEM_PADDINGX;
	int cury = win->y;
	int drawx, drawy;
	int i;

	for (i = 1; i < 13; i++)
	{
		// Check for mouse over
		if (sysMouseX > curx && sysMouseX < curx + ITEM_SIZE &&
			sysMouseY > cury && sysMouseY < cury + ITEM_SIZE)
		{
			// Draw opaqe and set selection
			vid_DrawImage(curx, cury, ACTIVE_ALPHA, ships[i].yard, false);
			selecteditem = i;
		}
		else
		{
			// Draw inactive icon
			vid_DrawImage(curx, cury, INACTIVE_ALPHA, ships[i].yard, false);
		}

		drawx = curx;
		drawy = cury + ITEM_SIZE;

		vid_DrawString(drawx, drawy - 8, 255, ships[i].name);

		drawx += 8;

		vid_DrawString(drawx, drawy + 16, 255, "Speed: %0.1f km/sec", ships[i].maxvelocity / 10.0);
		vid_DrawString(drawx, drawy + 32, 255, "Accel: %0.1f km/sec%c", ships[i].acceleration / 10.0, 178);
		vid_DrawString(drawx, drawy + 48, 255, "Mass: %0.1f kg", ships[i].mass);

		// Move to the next item slot
		curx += ITEM_SIZE + ITEM_PADDINGX;
		
		// If the end slot has been reached then wrap to next row
		if (curx > win->x + panewidth - ITEM_SIZE - ITEM_PADDINGX)
		{
			curx = win->x + ITEM_PADDINGX;
			cury += ITEM_SIZE + ITEM_PADDINGY;
		}
	}
	
	if (sysMouseButtons[1] == true && selecteditem > -1)
	{
		gameMainPlayer->shipid = selecteditem;
		config.PlayerShipID = selecteditem;
		interface_ToggleSelectShip();
	}
}

static void drawFPS(window *win)
{
	static const int resolution = 128;
	static float fpshistory[resolution];

	static float lasttime = 0.0f;
	static float lastfps = 0.0f;

	static float frametime = 0.0f;
	static float framecount = 0;

	int i;
	
	if ((timer_FloatTime() - lasttime) > 0.05f)
	{
		lastfps = 1.0f / ((timer_FloatTime() - lasttime) / framecount);
		lasttime = timer_FloatTime();
		framecount = 0;

		for (i = 0; i < resolution - 1; i++)
			fpshistory[i] = fpshistory[i+1];

		fpshistory[resolution - 1] = tm_FrameTime;
	}
	else
		framecount += 1;

	vid_DrawString(win->x, win->y + win->height - 16, 255, "%0.2f frames/sec", lastfps);

	float cur;
	int barwidth = 1;

	for (i = 0; i < resolution; i++)
	{
		cur = (1.0f / fpshistory[i]) / 5;
		vid_Rect(win->x + i * barwidth, win->y + win->height - cur - 20, barwidth, cur, vid_Color(0,255,0), 100);
	}
}

static bool processWindowInput(window *win)
{
	win->mousex = sysMouseX - win->sx;
	win->mousey = sysMouseY - win->sy;
	win->lmouse = sysMouseButtons[1];
	
	if (win->lmouse == false)
	{
		win->moving = false;
	}

	if (win->moving == true)
	{
		win->sx = sysMouseX - win->moveoffsetx;
		win->sy = sysMouseY - win->moveoffsety;

		if (win->sx < 1)
			win->sx = 1;
		if (win->sy < 1)
			win->sy = 1;
		if (win->sx > vidWidth - win->swidth - 1)
			win->sx = vidWidth - win->swidth - 1;
		if (win->sy > vidHeight - win->sheight - 1)
			win->sy = vidHeight - win->sheight - 1;
	}

	if (win->mousex > 0 && win->mousex < win->swidth &&
		win->mousey > 0 && win->mousey < win->sheight)
	{
		// On click move to front
		if (win->lmouse == true)
		{
			//item_c<window>* w;
			//w = windows.find(win);
			//windows.moveback(w);
		}

		// Check other input
		if (win->lmouse == true && win->moving == false)
		{
			// Close button
			if (win->mousex >= win->swidth - 12 &&
				win->mousex <= win->swidth - 2 &&
				win->mousey >= 2 &&
				win->mousey <= 12)
			{
				win->close();
			}
			else
			{
				win->moving = true;
				win->moveoffsetx = sysMouseX - win->sx;
				win->moveoffsety = sysMouseY - win->sy;
				return true;
			}
		}
	}

	return false;
}

static void drawViewSprite(window *win)
{
	//vid_DrawImage(win->x, win->y, win->width, win->height, 255, interfaceViewSpriteID, false);
}

/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
void interface_HideMenu()
{
	interfaceMenu->item.visible = false;
	gamePaused = false;
}

void interface_ToggleMenu()
{
	interfaceMenu->item.visible = !interfaceMenu->item.visible;

	if (interfaceMenu->item.visible == true)
		gamePaused = true;
	else
		gamePaused = false;
}

void interface_ToggleFPS()
{
	config.ShowFPS = !config.ShowFPS;

	if (config.ShowFPS == 1)
		interfaceFPS->item.visible = true;
	else
		interfaceFPS->item.visible = false;
}

void interface_ToggleHUD()
{
	config.ShowHUD = !config.ShowHUD;

	if (config.ShowHUD == 1)
		interfaceHUD->item.visible = true;
	else
		interfaceHUD->item.visible = false;
}

void interface_ToggleTarget()
{
	config.ShowTarget = !config.ShowTarget;

	if (config.ShowTarget == 1)
		interfaceTargeting->item.visible = true;
	else
		interfaceTargeting->item.visible = false;
}

void interface_ToggleMiniMap()
{
	config.ShowMiniMap = !config.ShowMiniMap;

	if (config.ShowMiniMap == 1)
		interfaceMiniMap->item.visible = true;
	else
		interfaceMiniMap->item.visible = false;
}

void interface_ToggleMap()
{
	interfaceFullMap->item.visible = !interfaceFullMap->item.visible;

	if (interfaceFullMap->item.visible == true &&
		interfaceMenu->item.visible == true)
	{
		interface_ToggleMenu();
	}
}

void interface_ToggleSelectShip()
{
	interfaceSelectShip->item.visible = !interfaceSelectShip->item.visible;

	if (interfaceSelectShip->item.visible == true &&
		interfaceMenu->item.visible == true)
	{
		interface_ToggleMenu();
	}
}

void interface_ToggleViewSprite()
{
	interfaceViewSprite->item.visible = !interfaceViewSprite->item.visible;
}

void interface_SetPlanetStatus(bool landed)
{
	interfacePlanet->item.visible = landed;
}

void interface_SetGameStatus(bool ingame)
{
	if (ingame == true)
	{
		interfaceMiniMap->item.visible = config.ShowMiniMap;
		interfaceHUD->item.visible = config.ShowHUD;
		interfaceTargeting->item.visible = config.ShowTarget;
		interfaceMenu->item.visible = false;
	}
	else
	{
		interfaceSelectShip->item.visible = false;
		interfaceMiniMap->item.visible = false;
		interfaceHUD->item.visible = false;
		interfaceFullMap->item.visible = false;
		interfaceTargeting->item.visible = false;
		interfacePlanet->item.visible = false;
		interfaceMenu->item.visible = true;
	}

	menu_Init(ingame);
}

item_c<window>* interface_CreateWindow(char *caption, int x, int y, int width, int height)
{
	window *w;
	w = new window;

	strcpy(w->caption, caption);
	w->x = x + WINDOW_PADDING;
	w->y = y + WINDOW_TITLE_HEIGHT + WINDOW_PADDING;
	w->width = width;
	w->height = height;

	w->sx = x;
	w->sy = y;
	w->swidth = width;
	w->sheight = height;


	w->used = true;
	w->visible = true;

	return windows.insert(*w);
}

void interface_Init()
{
	interfaceMiniMap = interface_CreateWindow("Mini Map", vidWidth - 200, 1, 199, 199);
	interfaceMiniMap->item.visible = false;
	interfaceMiniMap->item.draw2 = map_DrawMap;
	interfaceMiniMap->item.close = interface_ToggleMiniMap;

	interfaceTargeting = interface_CreateWindow("Target", vidWidth - 150, vidHeight - 321, 149, 320);
	interfaceTargeting->item.visible = false;
	interfaceTargeting->item.draw2 = target_DrawWindow;
	interfaceTargeting->item.close = interface_ToggleTarget;

	interfaceFullMap = interface_CreateWindow("Full Map", 50, 50, vidWidth - 148 - 100, vidHeight - 100);
	interfaceFullMap->item.visible = false;
	interfaceFullMap->item.draw2 = map_DrawMap;
	interfaceFullMap->item.close = interface_ToggleMap;
	
	interfacePlanet = interface_CreateWindow("Planet", 64, 64, vidWidth - 256, vidHeight - 128);
	interfacePlanet->item.visible = false;
	interfacePlanet->item.draw2 = drawPlanet;

	interfaceHUD = interface_CreateWindow("HUD", vidWidth - 150, 210, 149, 180);
	interfaceHUD->item.visible = false;
	interfaceHUD->item.draw2 = drawHUD;
	interfaceHUD->item.close = interface_ToggleHUD;

	interfaceSelectShip = interface_CreateWindow("Select Ship", 120, 80, vidWidth - 240, vidHeight - 160);
	interfaceSelectShip->item.visible = false;
	interfaceSelectShip->item.draw2 = drawSelectShip;
	interfaceSelectShip->item.close = interface_ToggleSelectShip;

	char buffer[128];
	sprintf(buffer, "Welcome to %s %s", SYS_APPNAME, SYS_APPVERSION);
	interfaceMenu = interface_CreateWindow(buffer, 300, 192, vidWidth - 600, vidHeight - 384);
	interfaceMenu->item.visible = true;
	interfaceMenu->item.draw2 = menu_Draw;
	interfaceMenu->item.close = interface_ToggleMenu;

	interfaceFPS = interface_CreateWindow("", 1, vidHeight - 101, 138, 100);
	interfaceFPS->item.visible = (config.ShowFPS == 1);
	interfaceFPS->item.draw2 = drawFPS;
	interfaceFPS->item.close = interface_ToggleFPS;

	interfaceViewSprite = interface_CreateWindow("View Sprite", 100, 100, 155, 155);
	interfaceViewSprite->item.visible = false;
	interfaceViewSprite->item.draw2 = drawViewSprite;
	interfaceViewSprite->item.close = interface_ToggleViewSprite;
}

void interface_Frame()
{
	console_DrawNotify();
	drawWindows();
	console_DrawConsole();
}

void interface_Input()
{
	if (gameRunning == true)
	{
		if (sysKeys[IN_TAB] == true)
		{
			interface_ToggleMap();
			sysKeys[IN_TAB] = false;
		}
	}

	if (sysKeys[IN_ESCAPE] == true)
	{
		interface_ToggleMenu();
		sysKeys[IN_ESCAPE] = false;
	}

	item_c<window>* win;
	window* w;
	win = windows.lastitem;
	while (win != 0)
	{
		w = &win->item;
		if (processWindowInput(w) == true)
			break;
		win = win->previtem;
	}

	console_Input();
}

