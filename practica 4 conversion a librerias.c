#include "config.h"
#include "trama.h"
#include "serial.c"
#include "matematicas.h"
#include "reseteoValores.h"


void main (void)
{/*
   setup_oscillator(OSC_16MHZ|OSC_NORMAL);
   set_tris_c(0x80);
   clear_interrupt(INT_RDA);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);*/
   configuracion();
   while(1)
   {
      if(buscarFinTrama())
      {     
      if(separacioTrama())    {  

         if(banderaEncontroSigno==1)
         if (errores==0)
         {
            seteoPirmerNumero();
            seteoSegundoNumero();
         }
         if(numero1encontrado==1&&numero2encontrado==1 && errores==0)
            realizarOperacion();
         else if(errorDivision0==1)
           printf("\rNo se puede dividir entre 0\r\rxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\r");
         reseteoDeValoresIniciales();       
         
      }

                   
      }       

   } 
}

