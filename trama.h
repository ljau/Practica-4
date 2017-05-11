#include "serial.h"

signed int buscarFinTrama(void)
{
   inicioDeBusqueda=1;
   if (banderaCaracterNuevo==1)
   {
      contadorTimer0=0;
      contadorAlerta=0;
      contadorDeBusqueda=0;
      banderaCaracterNuevo=0;
      while((signed)tamanoDeVector>=contadorDeBusqueda)
      {
         int auxilar= cadenaDeCaracteres[contadorDeBusqueda];
         if ((auxilar>='*' && auxilar<='/')&&(auxilar!='.'&&auxilar!=','&&auxilar!=';'))
            numeroDeSignos++;  
         if(cadenaDeCaracteres[contadorDeBusqueda]==59)
         {
            banderaFinDeTrama=1;
            return contadorDeBusqueda;
         }          
         contadorDeBusqueda++;  
      }
      if(numeroDeSignos>3)
      {
         printf("\rError, exceso de operadores!, Intente de nuevo\r\rxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\r");      
         reseteoDeValoresIniciales();
         break;
      }
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
   numero1encontrado=1;   
}
void seteoSegundoNumero(void)
{
   char vector[10]={0};
   int p=0;
   for(int i=(contadorDeSeparacion+1);i<contadorDeBusqueda;i++)
   {
      vector[i-(contadorDeSeparacion+1)]=cadenaDeCaracteres[i];
      p++;
      numero2encontrado=1;
   }
   numero2=atof(vector);
   if(numero2==0&&cadenaDeCaracteres[contadorDeSeparacion]=='/')
      errorDivision0=1;
   banderaEncontroSigno=0;
}
