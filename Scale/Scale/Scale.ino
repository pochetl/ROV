//
//    FILE: HX_performance.ino
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.0
// PURPOSE: HX711 demo
//     URL: https://github.com/RobTillaart/HX711
//
// HISTORY:
// 0.1.0    2020-06-15 initial version
//

#include "HX711.h"
#define DisplaySerial Serial
#include "Goldelox_Serial_4DLib.h"
#include "Goldelox_Const4D.h"

Goldelox_Serial_4DLib Display(&DisplaySerial);

HX711 scale;

uint8_t dataPin = 6;
uint8_t clockPin = 7;

uint32_t start, stop;
volatile float f;

void setup()
{
  DisplaySerial.begin(9600) ;

  delay (1000); 

  Display.gfx_ScreenMode(LANDSCAPE);
  Display.SSTimeout(0) ;
  Display.SSSpeed(0) ;
  Display.SSMode(0) ;
  
  Display.gfx_BGcolour(YELLOW) ; //change background color to white
  Display.gfx_Cls();            //clear the screen
/*
  Serial.println(__FILE__);
  Serial.print("LIBRARY VERSION: ");
  Serial.println(HX711_LIB_VERSION);
  Serial.println();
*/
  scale.begin(dataPin, clockPin);
 
  Display.gfx_BGcolour(BLUE);
  Display.gfx_Cls();  
  measure(10);
  delay(1000);
  Display.gfx_BGcolour(RED);  
  Display.gfx_Cls();  

  // TODO find a nice solution for this calibration..
  // loadcell factor 20 KG
  // scale.set_scale(127.15);
  // loadcell factor 5 KG
  scale.set_scale(420.0983);
    Display.gfx_BGcolour(YELLOW);
  Display.gfx_Cls();  

  // reset the scale to zero = 0
  scale.tare();
  Display.gfx_BGcolour(WHITE);
  Display.gfx_Cls();  

  delay(1000);
  measure(10);

    Display.gfx_BGcolour(PINK);
  Display.gfx_Cls();  

  scale.power_down();
  delay(2000);
  scale.power_up();
  Serial.println("\nWAKE UP");
  delay(1000);
  measure(10);

  Serial.println("\nPERFORMANCE");
  start = micros();
  f = 0;
  for (int i = 0; i < 100; i++)
  {
    f = scale.get_units(1);
  }
  stop = micros();
  Serial.print("100x get_units(1) = ");
  Serial.println(stop - start);
  Serial.print("  VAL: ");
  Serial.println(f, 2);

  Serial.println("\nPRECISSION");
  f = 0;
  for (int i = 0; i < 100; i++)
  {
    f += scale.get_units(1);
  }
  Serial.print("  VAL:");
  Serial.println(f * 0.01, 4);

}

void loop()
{
  //Display.gfx_Cls();
  
  // continuous scale 4x per second
  f = scale.get_units(5);
  Display.gfx_MoveTo (0,0);
  Display.print(f); Display.print(' ');
  // Serial.println(f);
  
  delay(250);
  
}

void measure(uint8_t cnt)
{
  /*Serial.print("  RAW: ");
  Serial.println(scale.read());
  Serial.print("  AVG: ");
  Serial.println(scale.read_average(cnt));
  Serial.print("VALUE: ");
  Serial.println(scale.get_value(cnt));
  Serial.print("UNITS: ");
  Serial.println(scale.get_units(cnt), 1);
  */
}


// -- END OF FILE --
