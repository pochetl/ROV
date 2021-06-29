/*
  Multiple Serial test

  Receives from the main serial port, sends to the others.
  Receives from serial port 1, sends to the main serial (Serial 0).

  This example works only with boards with more than one serial like Arduino Mega, Due, Zero etc.

  The circuit:
  - any serial device attached to Serial port 1
  - Serial Monitor open on Serial port 0

  created 30 Dec 2008
  modified 20 May 2012
  by Tom Igoe & Jed Roach
  modified 27 Nov 2015
  by Arturo Guadalupi

  This example code is in the public domain.
*/


#include <Servo_Hardware_PWM.h>

bool foundHeader = false; 
char inboundpacket[12];
int inboundpacketindex = 0;
int responseCounter = 0;
char response[4];
Servo myServo1;

void setup() {
  // initialize both serial ports:
  Serial.begin(115200);
  Serial3.begin(115200);
  myServo1.attach(8);
  Serial.write("Starting");

  response[0] = 0xFE;
  response[1] = 1;
  response[2] = 0;
  response[3] = 0;

            
}

void loop() {
 
  // read from port 1, send to port 0:
  if (Serial3.available()) {
    if (foundHeader == true){
         int inByte = Serial3.read();
      if(inboundpacketindex >8){
         
           
           inboundpacket[inboundpacketindex]=inByte;
            

            
            Serial.print(inboundpacket[0], DEC);
            Serial.write("            ");
            Serial.print(inboundpacket[1], DEC);
            Serial.write("\n");
            foundHeader = false;
            int motor1Speed = 1500+4.5*inboundpacket[1];
            myServo1.writeMicroseconds(motor1Speed);

            responseCounter++;
            //Respond
            if(responseCounter%10==0){
              Serial3.write(response, 4);
                           
              
            Serial.write("Happy\n");
              
              }
         
      }else{
         inboundpacket[inboundpacketindex]=inByte;
         inboundpacketindex +=1;
      }
        
    }
    else{
      
            
       int inByte = Serial3.read();
       if(inByte == 0xFE){
        inboundpacketindex=0;
        foundHeader = true;
       }
     }
 
  }






















  // read from port 0, send to port 1:
  if (Serial.available()) {
    int inByte = Serial.read();
    Serial3.write(inByte);
  }
}
