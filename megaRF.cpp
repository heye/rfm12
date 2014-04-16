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

#include <avr/io.h>        
#include <avr/interrupt.h> 
#include <inttypes.h>

#include "megaRF.h"

#include "rfm12lib/rfm12.h"
#include "rfm12lib/rfm12.c"

 #include "Arduino.h"


void (*megaRF::user_onReceive)(void);



/***
*
*	constructor
*	initialise the module
*
***/
megaRF::megaRF(){
	rfm12_init();
	//rfm12_rx_clear();
	rfm12_tick();
}




/***
*
*	void tick() 
*	tick
*
***/
void megaRF::tickOnly()
{
	rfm12_tick();
}




/***
*
*	void tick() 
*	checks if there is a complete transmission and calls the users callback function
*
***/
void megaRF::tick()
{
	rfm12_tick();
	uint8_t i=0;
	if(rfm12_rx_status()==STATUS_COMPLETE){
		extbuffer = rfm12_rx_buffer();		
		for(i=0;i<rfm12_rx_len();i++)
		{
			intbuffer[i] = *extbuffer;
			extbuffer++;
		}
		intbuffer[i]='\0';
		packetLength=i;
		
		rfm12_rx_clear();
		  
		 
		// don't bother if user hasn't registered a callback
 		if(!user_onReceive){
    		
  		}
  		else{
			user_onReceive();
		}
	}
	
	
	rfm12_tick();
	return;
}



/***
*
*	void send(uint8_t*) 
*	function to enqueue a packet for transmission
*	returns: 0x02 (error) 0x03 (occupied) 0x80 (enqueued)	
*
***/
/*uint8_t megaRF::send(uint8_t *s)
{
	uint8_t b;
	uint8_t a =0;
    while (*s)
    {   // so lange *s != '\0' also ungleich dem "String-Endezeichen(Terminator)" 
        //UART_send_byte(*s);
        s++;
        a++;
    }
    b = rfm12_tx(1, 0xEE, &a);
    rfm12_tick();
    return b;
    //return rfm12_tx(sizeof(s), 0xEE, s);
}*/

void megaRF::warmUp(void)
{
  uint8_t i;
  for(i=0; i<8; i++){
    RF.send("");
    RF.tick();
  }
}


/***
*
*	void send(uint8_t) : function to enqueue a packet for transmission
*	returns: 0x02 (error) 0x03 (occupied) 0x80 (enqueued)	
*
***/
uint8_t megaRF::send(uint8_t s)
{
	/*
    while (*s)
    {   // so lange *s != '\0' also ungleich dem "String-Endezeichen(Terminator)" 
        UART_send_byte(*s);
        s++;
    }*/
    return rfm12_tx(1, 0xEE, &s);
}



/***
*
*	void send(char*) : function to enqueue a packet for transmission
*	returns: 0x02 (error) 0x03 (occupied) 0x80 (enqueued)	
*
***/
uint8_t megaRF::send(char* s)
{
	uint8_t* ptr=(uint8_t*)s;	
	uint8_t i=0;	
	
	//get the length of char s 
	while (*ptr)
    {   // so lange *s != '\0' also ungleich dem "String-Endezeichen(Terminator)" 
        ptr++;
        i++;
    }
    
    return rfm12_tx(i, 0xEE, (uint8_t*)s);
}


/***
*
*	void send(String&) : function to enqueue a packet for transmission
*	returns: 0x02 (error) 0x03 (occupied) 0x80 (enqueued)	
*
***/
uint8_t megaRF::send(String& s)
{
	uint8_t buf[255];
	s.getBytes(buf, s.length());
	
	uint8_t* ptr=(uint8_t*)buf;	
	uint8_t i=0;	
	
	//get the length of char s 
	while (*ptr)
    {   // so lange *s != '\0' also ungleich dem "String-Endezeichen(Terminator)" 
        ptr++;
        i++;
    }
    
    return rfm12_tx(i, 0xEE, (uint8_t*)buf);
}




/***
*
*	void waitAndSend(char*) : function to enqueue a packet for transmission
*	blocks the programm until packet is successfully enqued!
*	returns: 0x02 (error) 0x03 (occupied) 0x80 (enqueued)	
*
***/
void megaRF::waitAndSend(char* s)
{
	while(send(s) != 0x80){
		tick();
	}
	return;
}



/***
*
*	void waitAndSend(uint8_t) : function to enqueue a packet for transmission
*	blocks the programm until packet is successfully enqued!
*	returns: 0x02 (error) 0x03 (occupied) 0x80 (enqueued)	
*
***/
void megaRF::waitAndSend(uint8_t s)
{
	while(send(s) != 0x80){
		tick();
	}
	return;
}


/***
*
*	uint8_t getByte(uint8_t)
*	returns the specified byte from the package buffer
*
***/
uint8_t megaRF::getByte(uint8_t byte){
	return intbuffer[byte];
}




/***
*
*	uint8_t* getBuffer(void)
*	return pointer to the first element of the buffer
*
***/
char* megaRF::getBuffer(void){
	return (char*)intbuffer;
}




/***
*
*	uint8_t numberOfBytes(void)
*	returns the length of the received package
*
***/
uint8_t megaRF::numberOfBytes(){
	return packetLength;
}



/***
*
*	
*	sets function called when receive transmission complete
*
***/
void megaRF::onReceive( void (*function)(void) )
{
  user_onReceive = function;
}



// preinstantiate object

megaRF RF = megaRF();
