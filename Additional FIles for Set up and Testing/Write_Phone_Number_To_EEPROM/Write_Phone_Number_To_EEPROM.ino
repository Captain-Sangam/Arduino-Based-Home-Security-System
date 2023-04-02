#include <EEPROM.h>
char num[11] = "123456789"; // Enter your phone number here

void setup()
{
  phone_write(num);
}

void loop()
{
}

void phone_write( char num[11] )
{
  int s;
  int i = 0;
  int addr = 14;
  while (addr > 4)
  {
    s = num[i] - '0';
    EEPROM.write(addr, s);
    i++;
    addr--;
  }
  String st1(num);
  mobile = st1;
}
