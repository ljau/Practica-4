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
{// interrupcion detecta cada vez que presionas una tecla
   while(kbhit()){
     cadenaDeCaracteres[contadorVector]=getc();//iguala la tecla presionada a un vector
     contadorVector++;//contador de la posicion vector
     if(contadorVector >= tamanoDeVector){//si si el contador es menor que el tamaño
        contadorVector--;//regresa una posicion antes para sobreescribirla pues sera un error por desbordamiento del vector
     }
     banderaCaracterNuevo=1;//activa una bandera para saber que se han intriducido alguna tecla
   }
   
}//regresara al punto donde se quedo antes de la interru

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
//busca el enter
   if (banderaCaracterNuevo==1){
   contadorDeBusqueda=0;
   banderaCaracterNuevo=0;  
      //setea la bandera en cero para evitar que entra
      //el contador del vectorque buscara el enter
      while((signed)tamanoDeVector>=contadorDeBusqueda)//ciclo para buscar
      {
    
         if(cadenaDeCaracteres[contadorDeBusqueda]==59){
            banderaFinDeTrama=1;
         //si la posicion del vectoor que este buscando es igual a enter entonces
            return contadorDeBusqueda;}
            //termina la funcion y envia el valor que tenga el contador de busqueda para saber hasta donde termina la trama
         contadorDeBusqueda++;//suma mas uno al contador de busqueda si no ha encontrado el enter de lo contrario no alacanzara a llegar  aeste punta
      }
      }
      else
      return 0;
      }


int separacioTrama(void)//separa la trama buscando el signo de operacioon 
{
if(banderaFinDeTrama==1)
{
contadorDeSeparacion=0;
char auxilar=0;//declaramos un auxiliar el cual nos servira para guardar el valor del vector
while(contadorDeBusqueda > contadorDeSeparacion)//buscara desde la posicion cero hasta la posicion donde se encontro el enter
{auxilar= cadenaDeCaracteres[contadorDeSeparacion];//asignamos temporalmente el valor de cada espacio del vector uno a la vez
 if((auxilar>='*' && auxilar<='/')&&(auxilar!='.'&&auxilar!=','))// si alguno de los caracteres encontrados es +-*/ y no es ., entonces
 {
 banderaFinDeTrama=0;
 banderaEncontroSigno=1;

    return contadorDeSeparacion;//retornamos ek contador de operacion para saber en que posicion  esta el signo
 }
  contadorDeSeparacion++;//cada vez que no encuentre un signo sumara uno de lo contrario n llegara hasta este punto
}
}
else
return 0;
//regresara al main
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


