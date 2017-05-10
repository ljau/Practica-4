float operation(float x, float y, char selector);

float operation(float x, float y, char selector)
{
    switch(selector)
    {
        case('+'):
            return(x+y);
            break;
        case('-'):
            return(x-y);
            break;
        case('*'):
            return(x*y);
            break;
        case('/'):
            return(x/y);
            break;
    }
}


