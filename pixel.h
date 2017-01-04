#ifndef PIXEL_H_INCLUDED
#define PIXEL_H_INCLUDED

typedef struct pixel
{
    char type;
    float vel;
} PIXEL;


char get_type_pixel(PIXEL* p);
void set_type_pixel(PIXEL* p, char type);


#endif // PIXEL_H_INCLUDED
