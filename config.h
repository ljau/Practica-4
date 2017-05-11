#define uC18f4620
#include <18F4620.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#fuses XT, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, MCLR, STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)
#use fast_io(c)
byte errores=0;
#bit errorDivision0=errores.0
#bit errorFaltaDeDigito=errores.1
#bit errorFaltaOperador=errores.2
#bit errorExcedenteOperaciones=errores.3
#bit errorDeSintaxis=errores.4
byte banderas=0;
#bit banderaCaracterNuevo =banderas.0
#bit banderaFinDeTrama=banderas.1
#bit banderaSeparacionDeTrama=banderas.2
#bit banderaEncontroSigno=banderas.3
#bit numero1encontrado=banderas.4
#bit numero2encontrado=banderas.5
#bit inicioDeBusqueda=banderas.6
#bit errorEncontrado=banderas.7
#define tamanoDeVector 22
void isr_rda(void);
int separacioTrama(void);
signed int buscarFinTrama(void);
void seteoPirmerNumero(void);
void seteoSegundoNumero(void);
void realizarOperacion(void);
void revisionDeErrores(void);
signed int buscarFinTrama(void);
int revisiontrama(int auxiliar);
void reseteoDeValoresIniciales (void);
char cadenaDeCaracteres[tamanoDeVector]={0};
int contadorDeSeparacion=0;
int contadorVector=0;
int numeroDeSignos=0;
float resultado=0;
float numero1=0;
float numero2=0;
int16 contadorTimer0=0;
int16 contadorAlerta=0;
signed int contadorDeBusqueda=0;

#int_timer0
void timer_0()      
{
   contadorTimer0++;
   contadorAlerta++;
   set_timer0(0);
}
void configuracion(void)
{
   setup_oscillator(OSC_16MHZ | OSC_NORMAL   );
   set_tris_c(0x80);
   clear_interrupt(INT_RDA);
   enable_interrupts(int_timer0);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256|T0_8_BIT);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   
}
