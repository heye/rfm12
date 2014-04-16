// megaRF send
// by Heye Everts <heyeeverts@.de>
//
// Demonstrates use of the megaRF library
// Send Bytes to another device
// 
// Refer to the "megaRF receive1" or "megaRF receive2" example for use with this
//
// Created 7 september 2012
// Copyright 2012 Heye Everts


//include the library
#include <megaRF.h>


// the setup routine runs once when you press reset:
void setup(){
}

void loop(){  
  //try to enque new packet for transmit
  RF.send("Hi");
  
  //call this tick function periodically, it will take care of collision avoidance, start transferring packets and check if there are any received packets
  RF.tick();  
 
  delay(500);
  
  //same as the above with an unsigned 1-byte integer
  RF.send((uint8_t) 42);
  RF.tick();
  
  delay(500);
}
