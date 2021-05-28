#include "closest_helpers.h"
#include <math.h>
int verbose = 0;

int compare_x(const void* a, const void* b) 
{
    int ax = ((struct Point*)a)->x;
    int bx = ((struct Point*)b)->x;
    return ax - bx;
} 

int compare_y(const void* a, const void* b) 
{ 
    int ay = ((struct Point*)a)->y;
    int by = ((struct Point*)b)->y;
    return ay - by;
} 

double dist(struct Point p1, struct Point p2) 
{
    //compare_x(&p1,&p2)
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    double sq = pow(dx,2) + pow(dy,2);
    return pow(sq,0.5);
} 
