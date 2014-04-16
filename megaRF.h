/***************************************************
*	megaRF.cpp
*	author: heye everts
*	heyeeverts@gmx.de
* 	Copyright 2012 Heye Everts
*
*	This file is part of the megaRF library.
*
*   The megaRF library is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   The megaRF library is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with the megaRF library.  If not, see <http://www.gnu.org/licenses/>.
*    
*
*    
*	this library uses the rfm12lib by Hans-Gert Dahmen and Peter Fuhrmann. 
* 
***************************************************/

#ifndef megaRF_h
#define megaRF_h

#define BUFFER_SIZE 255

#include <inttypes.h>

#include "Arduino.h"

class megaRF 
{
	public:
		megaRF();
		void tick(void);
		void tickOnly();
		void warmUp();
		
		//uint8_t send(uint8_t*);
		uint8_t send(String&);
		uint8_t send(char*);
		uint8_t send(uint8_t);
		void waitAndSend(char*);
		void waitAndSend(uint8_t);
		
		uint8_t getByte(uint8_t);
		char* getBuffer(void);
		uint8_t numberOfBytes();
		void onReceive(void (*function)(void) );
	private:
		uint8_t packetLength;
		uint8_t intbuffer[BUFFER_SIZE];
		uint8_t *extbuffer;
    	static void (*user_onReceive)(void);
};

extern megaRF RF;

#endif