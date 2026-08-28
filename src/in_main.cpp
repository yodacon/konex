/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker   *
 *   jbussdieker@gmail.com   *
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
#include "defs.h"

#define MAX_HANDLERS 16

struct in_handler_t
{
	void (*callback)(in_event_t*);
	bool used;
};

in_handler_t inHandlers[MAX_HANDLERS];

static bool lastkeys[256];
static int lastmousex, lastmousey;
static bool lastmousebuttons[5];

static dword getFreeHandler()
{
	dword i;
	for (i = 1; i < MAX_HANDLERS; i++)
		if (inHandlers[i].used == false)
			return i;

	return 0;
}

static void copyState()
{
	memcpy(lastkeys, sysKeys, sizeof(lastkeys));
	memcpy(lastmousebuttons, sysMouseButtons, sizeof(lastmousebuttons));
	lastmousex = sysMouseX;
	lastmousey = sysMouseY;
}

static void dispatchEvents()
{
	in_event_t event;
	dword i, j;

	// Send keyboard events
	// for (keys)
	for (i = 0; i < 256; i++)
	{
		//if (sysKeys[i] != lastkeys[i])
		if (sysKeys[i] == true)
		{
			if (sysKeys[i] == true)
				event.type = IN_KEYPRESS;
			else
				event.type = IN_KEYRELEASE;

			event.key = i;
			event.handled = false;

			for (j = 1; j < MAX_HANDLERS; j++)
			{
				if (inHandlers[j].used == true)
				{
						inHandlers[j].callback(&event);
						if (event.remove == true)
							sysKeys[i] = false;
						if (event.handled == true)
							break;
				}
			}
		}
	}
}

void in_Init()
{
	memset(&inHandlers, 0, sizeof(inHandlers));
	console_Print("IN: Input system is operational.");
	copyState();
}

dword in_CreateHandler(void (*callback)(in_event_t*))
{
	dword tmpid;
	tmpid = getFreeHandler();

	inHandlers[tmpid].callback = callback;
	inHandlers[tmpid].used = true;

	return tmpid;
}

char in_ASCII()
{
	int i;

	// Keep track of uppercase and lower case
	bool ucase = false;
	if (sysKeys[IN_LSHIFT] == true || sysKeys[IN_RSHIFT] == true)
		ucase = true;

	// Process text character input
	for (i = 0; i <= 255; i++)
	{
		if (sysKeys[i] == true && i != IN_TILDA && i != 37 && i != IN_LSHIFT && i != 64  && i != 62  && i != 63)
		{
			//console_Print("%d", i);
			sysKeys[i] = false;
			#ifdef LINUX
			switch (i)
			{
				case 10:
					if (ucase == true) return '!'; else return '1';
				case 11:
					if (ucase == true) return '@'; else return '2';
				case 12:
					if (ucase == true) return '#'; else return '3';
				case 13:
					if (ucase == true) return '$'; else return '4';
				case 14:
					if (ucase == true) return '%'; else return '5';
				case 15:
					if (ucase == true) return '^'; else return '6';
				case 16:
					if (ucase == true) return '&'; else return '7';
				case 17:
					if (ucase == true) return '*'; else return '8';
				case 18:
					if (ucase == true) return '('; else return '9';
				case 19:
					if (ucase == true) return ')'; else return '0';
				case 20:
					if (ucase == true) return '_'; else return '-';
				case 21:
					if (ucase == true) return '+'; else return '=';

				case 24:
					if (ucase == true) return 'Q'; else return 'q';
				case 25:
					if (ucase == true) return 'W'; else return 'w';
				case 26:
					if (ucase == true) return 'E'; else return 'e';
				case 27:
					if (ucase == true) return 'R'; else return 'r';
				case 28:
					if (ucase == true) return 'T'; else return 't';
				case 29:
					if (ucase == true) return 'Y'; else return 'y';
				case 30:
					if (ucase == true) return 'U'; else return 'u';
				case 31:
					if (ucase == true) return 'I'; else return 'i';
				case 32:
					if (ucase == true) return 'O'; else return 'o';
				case 33:
					if (ucase == true) return 'P'; else return 'p';
				case 34:
					if (ucase == true) return '{'; else return '[';
				case 35:
					if (ucase == true) return '}'; else return ']';


				case 38:
					if (ucase == true) return 'A'; else return 'a';
				case 39:
					if (ucase == true) return 'S'; else return 's';
				case 40:
					if (ucase == true) return 'D'; else return 'd';
				case 41:
					if (ucase == true) return 'F'; else return 'f';
				case 42:
					if (ucase == true) return 'G'; else return 'g';
				case 43:
					if (ucase == true) return 'H'; else return 'h';
				case 44:
					if (ucase == true) return 'J'; else return 'j';
				case 45:
					if (ucase == true) return 'K'; else return 'k';
				case 46:
					if (ucase == true) return 'L'; else return 'l';
				case 47:
					if (ucase == true) return ':'; else return ';';
				case 48:
					if (ucase == true) return '"'; else return '\'';

				case 51:
					if (ucase == true) return '|'; else return '\\';
				case 52:
					if (ucase == true) return 'Z'; else return 'z';
				case 53:
					if (ucase == true) return 'X'; else return 'x';
				case 54:
					if (ucase == true) return 'C'; else return 'c';
				case 55:
					if (ucase == true) return 'V'; else return 'v';
				case 56:
					if (ucase == true) return 'B'; else return 'b';
				case 57:
					if (ucase == true) return 'N'; else return 'n';
				case 58:
					if (ucase == true) return 'M'; else return 'm';
				case 59:
					if (ucase == true) return '<'; else return ',';
				case 60:
					if (ucase == true) return '>'; else return '.';
				case 61:
					if (ucase == true) return '?'; else return '/';

				case 79:
					return '7';
				case 80:
					return '8';
				case 81:
					return '9';
				case 82:
					return '-';
				case 83:
					return '4';
				case 84:
					return '5';
				case 85:
					return '6';
				case 86:
					return '+';
				case 87:
					return '1';
				case 88:
					return '2';
				case 89:
					return '3';
				case 90:
					return '0';
				case 91:
					return '.';

				case 112:
					return '/';
				case IN_BACKSPACE:
					return IN_BACKSPACE;
				case IN_SPACEBAR:
					return ' ';
				case IN_ENTER:
					return IN_ENTER;
			}
			#else
			switch (i)
			{
				case 48:
					if (ucase == true) return ')'; else return '0';
				case 49:
					if (ucase == true) return '!'; else return '1';
				case 50:
					if (ucase == true) return '@'; else return '2';
				case 51:
					if (ucase == true) return '#'; else return '3';
				case 52:
					if (ucase == true) return '$'; else return '4';
				case 53:
					if (ucase == true) return '%'; else return '5';
				case 54:
					if (ucase == true) return '^'; else return '6';
				case 55:
					if (ucase == true) return '&'; else return '7';
				case 56:
					if (ucase == true) return '*'; else return '8';
				case 57:
					if (ucase == true) return '('; else return '9';

				case 65:
					if (ucase == true) return 'A'; else return 'a';
				case 83:
					if (ucase == true) return 'S'; else return 's';
				case 68:
					if (ucase == true) return 'D'; else return 'd';
				case 70:
					if (ucase == true) return 'F'; else return 'f';
				case 71:
					if (ucase == true) return 'G'; else return 'g';
				case 72:
					if (ucase == true) return 'H'; else return 'h';
				case 74:
					if (ucase == true) return 'J'; else return 'j';
				case 75:
					if (ucase == true) return 'K'; else return 'k';
				case 76:
					if (ucase == true) return 'L'; else return 'l';

				case 81:
					if (ucase == true) return 'Q'; else return 'q';
				case 87:
					if (ucase == true) return 'W'; else return 'w';
				case 69:
					if (ucase == true) return 'E'; else return 'e';
				case 82:
					if (ucase == true) return 'R'; else return 'r';

				case 84:
					if (ucase == true) return 'T'; else return 't';
				case 89:
					if (ucase == true) return 'Y'; else return 'y';
				case 85:
					if (ucase == true) return 'U'; else return 'u';
				case 73:
					if (ucase == true) return 'I'; else return 'i';

				case 79:
					if (ucase == true) return 'O'; else return 'o';
				case 80:
					if (ucase == true) return 'P'; else return 'p';

				case 90:
					if (ucase == true) return 'Z'; else return 'z';
				case 88:
					if (ucase == true) return 'X'; else return 'x';
				case 67:
					if (ucase == true) return 'C'; else return 'c';
				case 86:
					if (ucase == true) return 'V'; else return 'v';
				

				case 66:
					if (ucase == true) return 'B'; else return 'b';
				case 78:
					if (ucase == true) return 'N'; else return 'n';
				case 77:
					if (ucase == true) return 'M'; else return 'm';

				case 188:
					if (ucase == true) return '<'; else return ',';
				case 190:
					if (ucase == true) return '>'; else return '.';

				case 222:
					if (ucase == true) return '"'; else return '\'';

				case 186:
					if (ucase == true) return ':'; else return ';';
				case 191:
					if (ucase == true) return '?'; else return '/';

				case 219:
					if (ucase == true) return '{'; else return '[';
				case 221:
					if (ucase == true) return '}'; else return ']';

				case 189:
					if (ucase == true) return '_'; else return '-';

				case 187:
					if (ucase == true) return '+'; else return '=';
				case 220:
					if (ucase == true) return '|'; else return '\\';

				case 96:
					return '0';
				case 97:
					return '1';
				case 98:
					return '2';
				case 99:
					return '3';
				case 100:
					return '4';
				case 101:
					return '5';
				case 102:
					return '6';
				case 103:
					return '7';
				case 104:
					return '8';
				case 105:
					return '9';

				case 106:
					return '*';
				case 107:
					return '+';
				case 109:
					return '-';
				case 110:
					return '.';
				case 111:
					return '/';

				case IN_BACKSPACE:
					return IN_BACKSPACE;
				case IN_SPACEBAR:
					return ' ';
				case IN_ENTER:
					return IN_ENTER;
			}
			#endif
		}
	}

	return 0;
}

void in_Frame()
{
	dispatchEvents();
	copyState();
}

void in_Shutdown()
{
}
