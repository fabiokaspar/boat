#include "pixel.h"
#include <stdio.h>


char get_type_pixel(PIXEL* p)
{
    return p->type;
}

void set_type_pixel(PIXEL* p, char type)
{
    p->type = type;
}
