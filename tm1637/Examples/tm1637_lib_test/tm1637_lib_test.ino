#include "TM1637.h"
#include <Ticker.h>

tm1637 display(14, 13);
Ticker timer;
bool tock = false;
uint16_t count = 0;
void setup() {
  display.setBrightness(4);
  display.writeInteger(count++);
  timer.attach(1.0, tick);
}

void loop() {
  
  if (tock){
    tock = false;
    display.writeInteger(count++);
  }
}

void tick(void)
{
    tock = true;
}
