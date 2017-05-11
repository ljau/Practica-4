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
      {
         if(contadorVector!=0)
            contadorVector--;
         else
         {
            cadenaDeCaracteres[contadorVector]=0;
            break;
         }
      }
      else
         contadorVector++;
      if(contadorVector >= tamanoDeVector)
         contadorVector--;
      banderaCaracterNuevo=1;
   }
}
#endif


