// megaRF receive1
// by Heye Everts <heyeeverts@.de>
//
// Demonstrates use of the megaRF library
// Receive bytes from another megaRF, and print with serial
// 
// Refer to the "megaRF send" or "megaRF waitAndSend" example for use with this
//
// Created 7 september 2012
// Copyright 2012 Heye Everts


//include the library
#include <megaRF.h>



// the setup routine runs once when you press reset:
void setup(){
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.print("####reset####");
  
  //register callback function
  RF.onReceive(receiveEvent);
}

// the loop routine runs over and over again forever:
void loop() {  
  delay(10);        // delay in between reads for stability
  
  //call this tick function periodically, it will take care of collision avoidance, start transferring packets and check if there are any received packets
  RF.tick();
}


//callback function that is called when a byte-word is received
void receiveEvent(void){
  Serial.println(RF.getBuffer());
  return;
}

