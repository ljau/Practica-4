float sum(float A, float B);
float subtraction(float A, float B);
float multiplication(float A, float B);
float division(float A, float B);
float operation(float x, float y, char selector);
float opResult;

float operation(float x, float y, char selector)
{
    switch(selector)
    {
        case('+'):
           opResult=sum(x,y);
           return(opResult);           
        case('-'):
           opResult=subtraction(x,y);
           return(opResult);           
        case('*'):
           opResult=multiplication(x,y);
           return(opResult);           
        case('/'):
           opResult=division(x,y);
           return(opResult);           
    }
}

float sum(float A, float B)
{
    return (A+B);
}
float subtraction(float A, float B)
{
    return (A-B);
}
float multiplication(float A, float B)
{
    return (A*B);
}
float division(float A, float B)
{
    return (A/B);
}
