#include "config.h"
#include "trama.h"
#include "serial.c"
#include "matematicas.h"
#include "reseteoValores.h"


void main (void)
{
   configuracion();
   while(1)
   {
      if(contadorTimer0<=1800)
      {
         if(buscarFinTrama())
         {     
            if(separacioTrama())    
            {
               if(banderaEncontroSigno==1)
               {
                  if (errores==0)
                  {
                     seteoPirmerNumero();
                     seteoSegundoNumero();
                  }
               }
               else
               {
                  printf("\rno se encontro signo 0\r\rxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\r");
                  reseteoDeValoresIniciales();
               }
               if(numero1encontrado==1&&numero2encontrado==1 && errores==0)
                  realizarOperacion();
               else if(errorDivision0==1)
                  printf("\rNo se puede dividir entre 0\r\rxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\r");
               reseteoDeValoresIniciales();       
         
            }      
         }       
      } 
      else
      {
         if (contadorAlerta >=300)
         {
             reseteoDeValoresIniciales();
             printf("\rEl tiempo de espera ha terminado, presione alguna tecla para empezar\r>"); 
             contadorAlerta=0;
             contadorTimer0=0;
         }
      }
   } 
}

