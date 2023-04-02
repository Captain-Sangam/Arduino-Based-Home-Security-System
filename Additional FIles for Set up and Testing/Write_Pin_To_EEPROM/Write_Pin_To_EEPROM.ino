#include <EEPROM.h>

int pas = 1234; //Enter Required Pin here
void setup()
{
  pin_write();
}

void loop()
{
}

void pin_write( int pas)
{
  int s;
  int addr = 3;
  while (pas > 0)
  {
    s = pas % 10;
    EEPROM.write(addr, s);
    pas /= 10;
    addr--;
  }
}
