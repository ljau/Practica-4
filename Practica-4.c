#define uC18f4620
#include <18F4620.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#fuses XT, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, MCLR, STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)
#define infoDebug
#define RX_232        PIN_C7
#define TX_232        PIN_C6
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
     cadenaDeCaracteres[contadorVector]=getc();//aqui guardamos la tecla sin importar cual sea en un la posicion del vector a la que corresponde pue contador de vector =0 al iniciar
     contadorVector++;//le sumamos uno al contador para asi cambiar de posicion y si se ejecuta denuevo la interrupcion el siguiente caracter se guardara en un espacio vacio
     if(contadorVector >= tamanoDeVector)//en esta seccion revisamos si ya se lleno el contador
        contadorVector--;//si el vector se encuentra lleno entonces regresamos una pocsicion para sobreescribirla si  se ejeuta denuevo la instrucion
     banderaCaracterNuevo=1;//activamos una bandera para decir que se ha encontrado un nuevo caracter
     }
   
}

void main (void){//1 esta es la funcion main aqui inicia el programa
   setup_oscillator(OSC_16MHZ | OSC_NORMAL   );//seteamos el oscilador
   set_tris_c(0x80);//seteamos el puesto serial en salidas y entradas
   clear_interrupt(INT_RDA);//limpiamos la interrupcion del puerto sserial
   enable_interrupts(INT_RDA);//habilitamos la interrupcion serial
   enable_interrupts(GLOBAL);// habilitamos la interrupcion global
   while(1)//2 este while se ejecutara perpetuamente
   {
   if(buscarFinTrama()){//3 hace una comparacion para saber si el valor que regresa el ejecutar la funcion buscar fin de trama es diferente de cerosi es asi ejecutara el codigo dentro de este if
      if(separacioTrama())//13 revismos si existe un separador de trama ejecutando la funcion separacion de trama
        if(banderaEncontroSigno==1){//25si la bandera dice que encontro un signo en la trama entonces ejecuta lo dentro del if
        seteoPirmerNumero();//26 separamos el primer numero de la trama
        seteoSegundoNumero();
        revisionDeErrores();
        if(numero1encontrado==1&&numero2encontrado==1 && errorEncontrado==0)
           realizarOperacion();
        reseteoDeValoresIniciales();       
        }       
      
  }
    
   
   
   }
}
signed int buscarFinTrama(void)//4 este codigo es evaluado por la funcion if  en el paso 3 buscara donde se encuentra el carcter que asignamos como fin de trama en este caso ;
{
   inicioDeBusqueda=1;
   if (banderaCaracterNuevo==1){//5 si la interrupcion detecto un nuevo caracterentonces ejecutara este codigo si no se saldra y retornara un false;
   contadorDeBusqueda=0;// 6 inicamos un contador para buscar enla siguiente posicion
   banderaCaracterNuevo=0;  //7seteamos la bandera de caracter nuevo en 0 pues significa que ya entro a revisar los caracteres acutuamente en el buffer

      while((signed)tamanoDeVector>=contadorDeBusqueda)//8ciclo para buscar el caracter;
      {
         int auxilar= cadenaDeCaracteres[contadorDeBusqueda];
         if ((auxilar>='*' && auxilar<='/')&&(auxilar!='.'&&auxilar!=','&&auxilar!=';'))
         {
            numeroDeSignos++;
         }
     
         
    
         if(cadenaDeCaracteres[contadorDeBusqueda]==59){//9si el vector en posicion del contador( cmenzando en 0 como en el paso 6) es igual
            banderaFinDeTrama=1;//10 activamos una bandera para decir que ya encontramos el caracter que finaliza nuestra trama

            return contadorDeBusqueda;}//11 retornamos el valor del contador haciendo de esta manera la condicion del if en el main true
                  //12 en este punto retornarra a el main para ejecutar lo que esta dentro del if en el paso 13
         contadorDeBusqueda++;//si no encuentrael caracter de fin de trama en la primer posicion cambia  ala siguiente   posicion
         
      }
          if(numeroDeSignos>3)
               errorExcedenteOperaciones=1;
         else
               numeroDeSignos=0;
      }
      else//si la bandera de caracter nuevo no esta acativa saldra de los sciclor retornando un valor 0 o false
      return 0;
      }


int separacioTrama(void)//14 en esta funcion revisaremos en que posicion se encuentra el signo de operacion 
{
   if(banderaFinDeTrama==1)//15 verfica si se encontro un fin de trame
   {
      contadorDeSeparacion=0;//16 iniciamos un contador que nos dira en que posicion de encuentra el signo
      char auxilar=0;//17 declaramos este auxiliar para evitar sobrepasar los carcateres or linea
      while(contadorDeBusqueda > contadorDeSeparacion)//18 si la posiccon en la que se encuenta el contador es menor que la posicion donde se encuentra el ;
      
      {
         auxilar= cadenaDeCaracteres[contadorDeSeparacion];//19 convertimos el valor de de el vector en cada posicion a ale euxilira
         if((auxilar>='*' && auxilar<='/')&&(auxilar!='.'&&auxilar!=',')&&contadorDeSeparacion!=0)//20 comparamossi la poscion es un + - / y *
         {
            banderaFinDeTrama=0;//21 quitamos la bandera de fin de trama para evitar entrar dennuevo a esta opcion
            banderaEncontroSigno=1;//22 si encuentra un signo entonces activara un bandera
            return 1;//23 retornamos un valor para el contador de separacion
         }
         contadorDeSeparacion++;//24 sumamos uno al contador
       }
   }
   else//si no encunetra la bandera activa retornara un 0 por lo tanto no ejecutara el if del paso 13
   return 0;
}

void seteoPirmerNumero(void)//27 separa el primer numero de la trama
{
   char vectorAuxiliar[9]={0};//28 egregamos un vector auxiliar para introduucior la secion de la trama que corresponde
   for(int i=0;i<contadorDeSeparacion;i++)//29 guardamos el primer numero en un vector auxiliar 
   {
      vectorAuxiliar[i]=cadenaDeCaracteres[i];
   }
   numero1=atof(vectorAuxiliar);
   printf("\rprimer numero");
   printf("%f",numero1);
   numero1encontrado=1;
   
}
void seteoSegundoNumero(void){
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


