#include <EEPROM.h>

int pas = 0;
void setup()
{
  Serial.begin(9600);
  pin_read();
}

void loop()
{
  Serial.println(pas);
}

void pin_read()
{
  int s;
  int addr = 0;
  while (addr < 4)
  {
    s = EEPROM.read(addr);
    pas = pas * 10 + s;
    addr++;
  }
  pass = pas;
}
