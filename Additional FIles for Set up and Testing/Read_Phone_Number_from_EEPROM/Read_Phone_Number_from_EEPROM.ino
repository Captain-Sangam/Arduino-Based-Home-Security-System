#include <EEPROM.h>

char num[11];
void setup()
{
  Serial.begin(9600);
  phone_read();
}

void loop()
{
    Serial.println(st);
}

void phone_read()
{
  int addr = 14;
  int s;
  int i = 0;
  while (addr > 4 )
  {
    num[i] = EEPROM.read(addr) + '0' ;
    i++;
    addr--;
  }
  String st(num);
  mobile = st;
}
