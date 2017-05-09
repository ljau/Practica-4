#define uC18f4620
#include <18F4620.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#fuses XT, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, MCLR, STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)
#define infoDebug
#define RX_232   PIN_C7
#define TX_232   PIN_C6
#use RS232(BAUD=9600, XMIT=TX_232, RCV=RX_232, BITS=8,PARITY=N, STOP=1)
#use fast_io(c)
#define tamanoDeVector 22
byte banderas=0;
#bit banderaCaracterNuevo =banderas.0
#bit banderaFinDeTrama=banderas.1
#bit banderaSeparacionDeTrama=banderas.2
#bit banderaEncontroSigno=banderas.3
#bit numero1encontrado=banderas.4
#bit numero2encontrado=banderas.5
#bit inicioDeBusqueda=banderas.6
byte errores=0;
#bit errorDivision0=errores.0
#bit errorFaltaDeDigito=errores.1
#bit errorFaltaOperador=errores.2
#bit errorExcedenteOperaciones=errores.3
#bit errorDeSintaxis=errores.4
#bit errorEncontrado=errores.5


char cadenaDeCaracteres[tamanoDeVector]={0};
int contadorVector=0;
signed int buscarFinTrama(void);
int separacioTrama(void);
void seteoPirmerNumero(void);
void seteoSegundoNumero(void);
void realizarOperacion(void);
void revisionDeErrores(void);
void reseteoDeValoresIniciales (void);
signed int contadorDeBusqueda=0;
int contadorDeSeparacion=0;
int numeroDeSignos=0;
float numero1=0;
float numero2=0;
float resultado=0;
#int_rda
void isr_rda()
{   
    while(kbhit())
    {
        cadenaDeCaracteres[contadorVector]=getc();
        contadorVector++;
           if(contadorVector >= tamanoDeVector)
              contadorVector--;
              banderaCaracterNuevo=1;
    }
   
}

void main (void)
{
   setup_oscillator(OSC_16MHZ|OSC_NORMAL);
   set_tris_c(0x80);
   clear_interrupt(INT_RDA);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   while(1)
   {
      if(buscarFinTrama())
      {
         if(separacioTrama())
            if(banderaEncontroSigno==1)
               {
                  seteoPirmerNumero();
                  seteoSegundoNumero();
                  revisionDeErrores();
                     if(numero1encontrado==1&&numero2encontrado==1 && errorEncontrado==0)
                        realizarOperacion();
                        reseteoDeValoresIniciales();       
               }       
      }
   } 
}
signed int buscarFinTrama(void)
{
   inicioDeBusqueda=1;
   if (banderaCaracterNuevo==1)
   {
      contadorDeBusqueda=0;
      banderaCaracterNuevo=0;
      while((signed)tamanoDeVector>=contadorDeBusqueda)
      {
         int auxilar= cadenaDeCaracteres[contadorDeBusqueda];
         if ((auxilar>='*' && auxilar<='/')&&(auxilar!='.'&&auxilar!=','&&auxilar!=';'))
         {
            numeroDeSignos++;
         }     
         if(cadenaDeCaracteres[contadorDeBusqueda]==59)
         {
            banderaFinDeTrama=1;
            return contadorDeBusqueda;
         }          
         contadorDeBusqueda++;  
      }
      if(numeroDeSignos>3)
         errorExcedenteOperaciones=1;
      else
         numeroDeSignos=0;
   }
   else
   return 0;
}
int separacioTrama(void)
{
   if(banderaFinDeTrama==1)
   {
      contadorDeSeparacion=0;
      char auxilar=0;
      while(contadorDeBusqueda > contadorDeSeparacion)      
      {
         auxilar= cadenaDeCaracteres[contadorDeSeparacion];
         if((auxilar>='*' && auxilar<='/')&&(auxilar!='.'&&auxilar!=',')&&contadorDeSeparacion!=0)
         {
            banderaFinDeTrama=0;
            banderaEncontroSigno=1;
            return 1;
         }
         contadorDeSeparacion++;
      }
   }
   else
   return 0;
}
void seteoPirmerNumero(void)
{
   char vectorAuxiliar[9]={0};
   for(int i=0;i<contadorDeSeparacion;i++)
   {
      vectorAuxiliar[i]=cadenaDeCaracteres[i];
   }
   numero1=atof(vectorAuxiliar);
   printf("\rprimer numero");
   printf("%f",numero1);
   numero1encontrado=1;   
}
void seteoSegundoNumero(void)
{
   char vector[10]={0};
   for(int i=(contadorDeSeparacion+1);i<contadorDeBusqueda;i++)
   {
      vector[i-(contadorDeSeparacion+1)]=cadenaDeCaracteres[i];
   }  
   numero2=atof(vector); 
   printf("\rsegundo numero");
   printf("%f",numero2);
   if (numero2==0 && cadenaDeCaracteres[contadorDeSeparacion] == '/')
   {
      errorDivision0=1;     
   }
   banderaEncontroSigno=0;
   numero2encontrado=1;
}
void revisionDeErrores (void)
{
   if (errores != 0)
   {
      errorEncontrado=1;
      if (errorDivision0==1)
      {
         printf("\rError, no se puede dividir entre 0!");
         errorDivision0=0;
      }
      else if (errorExcedenteOperaciones==1)
      {
         printf("\rError, exceso de operadores!");
         errorExcedenteOperaciones=0;
      }  
      /*  
      else if (errorFaltaOperador==1)
      else if (errorFaltaDeDigito==1)
      else if (errorDeSintaxis==1)*/
      printf("\r\r/////////////////////////////////////\r");
   }  
}
void reseteoDeValoresIniciales (void)
{
   errores=0;
   banderas=0;
   contadorDeBusqueda=0;
   contadorDeSeparacion=0;
   numeroDeSignos=0;
   //numero1=0;
   //numero2=0;
   resultado=0;
   contadorVector=0;
   for (int i=0; i<tamanoDeVector; i++)
   {
      cadenaDeCaracteres[i]=0;
   }
   /*for (int j=0; j<10; j++)
   {    
   }*/
   ///////////////////////////////xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
}
void realizarOperacion(void)
{
   switch(cadenaDeCaracteres[contadorDeSeparacion])
   {
      case('+'):
         resultado=numero1+numero2;
         break;
      case('-'):
         resultado=numero1-numero2;
         break;
      case('*'):
         resultado=numero1*numero2;
         break;
      case('/'):
         resultado=numero1/numero2;
         break;
   }
   printf("\rel resultado es ");
   printf("%f\r\r/////////////////////////////////////\r",resultado);
}


