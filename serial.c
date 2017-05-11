#include "serial.h"
#ifndef RS485_DRIVER
#define RS485_DRIVER

#int_rda
void isr_rda(void)
{
   while(kbhit())
   {
      cadenaDeCaracteres[contadorVector]=getc();
      if(cadenaDeCaracteres[contadorVector]==0x08)
         contadorVector--;
      else
         contadorVector++;
      if(contadorVector >= tamanoDeVector)
         contadorVector--;
      banderaCaracterNuevo=1;
   }
}
#endif
