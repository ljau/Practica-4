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
    printf("\rEl resultado es %f\r",resultado);
    printf("\rxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\r");  
} 
