#define uC18f4620
#include <18F4620.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef RS485_DRIVER
#define RS485_DRIVER
#include <string.h>
#fuses XT, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, MCLR, STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)
#define infoDebug
#define RX_232        PIN_C7
#define TX_232        PIN_C6
#use RS232(BAUD=9600, XMIT=TX_232, RCV=RX_232, BITS=8,PARITY=N, STOP=1)
#use fast_io(c)
#define tamanoDeVector 20
char cadenaDeCaracteres[tamanoDeVector]={0};
int contadorVector=0;
byte banderas=0;
#bit banderaCaracterNuevo =banderas.0
#bit banderaFinDeTrama=banderas.1
#bit banderaSeparacionDeTrama=banderas.2
#bit banderaEncontroSigno=banderas.3
#bit numero1encontrado=banderas.4
#bit numero2encontrado=banderas.5




signed int buscarFinTrama(void);
int separacioTrama(void);
void seteoPirmerNumero(void);
void seteoSegundoNumero(void);
void realizarOperacion(void);
signed int contadorDeBusqueda=0;
int contadorDeSeparacion=0;
float numero1=0;
float numero2=0;
float resultado=0;
#int_rda
void isr_rda()
{
   while(kbhit()){
     cadenaDeCaracteres[contadorVector]=getc();
     contadorVector++;
     if(contadorVector >= tamanoDeVector){
        contadorVector--;
     }
     banderaCaracterNuevo=1;
   }
   
}

void main (void){
   setup_oscillator(OSC_16MHZ | OSC_NORMAL   );
   set_tris_c(0x80);
   clear_interrupt(INT_RDA);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   while(1)
   {
   if(buscarFinTrama()){
      if(separacioTrama())
        if(banderaEncontroSigno==1){
        seteoPirmerNumero();
        seteoSegundoNumero();
        if(numero1encontrado==1&&numero2encontrado==1)
           realizarOperacion();
        }       
      
  }
    
   
   
   }
}
signed int buscarFinTrama(void){

   if (banderaCaracterNuevo==1){
   contadorDeBusqueda=0;
   banderaCaracterNuevo=0;  

      while((signed)tamanoDeVector>=contadorDeBusqueda)//ciclo para buscar
      {
    
         if(cadenaDeCaracteres[contadorDeBusqueda]==59){
            banderaFinDeTrama=1;

            return contadorDeBusqueda;}

         contadorDeBusqueda++;
      }
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
{auxilar= cadenaDeCaracteres[contadorDeSeparacion];
 if((auxilar>='*' && auxilar<='/')&&(auxilar!='.'&&auxilar!=','))
 {
 banderaFinDeTrama=0;
 banderaEncontroSigno=1;

    return contadorDeSeparacion;
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
   printf("primer numero");
   printf("%f",numero1);
   numero1encontrado=1;
   
}
void seteoSegundoNumero(void){
    char vector[10]={0};
    int p=0;
 for(int i=(contadorDeSeparacion+1);i<contadorDeBusqueda;i++)
 {
 
 vector[i-(contadorDeSeparacion+1)]=cadenaDeCaracteres[i];
  printf(" %c \n",vector[i]) ;
  p++;
 numero2encontrado=1;
  }
  
numero2=atof(vector);
      printf("segundo numero");
   printf("%f",numero2);
banderaEncontroSigno=0;
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
    printf("el resultado es");
    printf("%f",resultado);
}



