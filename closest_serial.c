#include "closest_serial.h"
#include <stdlib.h>
#include "closest_parallel.h"
#include <unistd.h>
#include <stdio.h>
/*find smallest length between right and left sides*/
double find_small(double x,double y){
    if (x<y) {
        return x;
    }else{
        return y;
    }
}
/*
 * Find the distance between closest pair of points within a vertical
 * strip centered at the midpoint, where both points are within
 * distance d from the vertical line passing through the midpoint.
 */
double combine_lr(struct Point P[], size_t n, struct Point mid_point, double d)
{
    struct Point *combinerl_array = malloc(sizeof(struct Point) * n);
    int i =0;

    int length_lr = 0;
    //build array contian points p_i which abs(p_i1.x - p_mid.x)<d
    while(i<n){
        if(abs(P[i].x - mid_point.x)<d){
            combinerl_array[length_lr]= P[i];
            length_lr+=1;
        }
        i++;
    }
    //sort points with qsort by y
    qsort(combinerl_array,length_lr,sizeof(struct Point), compare_y);
    //
    int final_d;
    int dis;
    int li = 0;
    int ri;
    struct Point p_i;
    struct Point p_j;
    //for each point find the sequece point point p_j p_j.y-pi.y<d
    while(li<=length_lr){
        ri = li +1;
        while (ri<=li){
            p_i = combinerl_array[li];
            p_j = combinerl_array[ri];
            if (p_i.y - p_j.y< d) {
                dis = dist(combinerl_array[li], combinerl_array[ri]);
                final_d =dis;
                if(final_d<d){
                    d = final_d;
                }
            }
            ri++;
        }
        li++;
    }
    free(combinerl_array);
    return d;
}
/*
 * Single-process recursive divide-and-conquer implementation to find the
 * distance between the closest pair of n points in P[]. Assumes that the
 * array P[] is sorted by x-coordinate.
 */
double _closest_serial(struct Point P[], size_t n)
{
    int p_mid;
    double d;
    double left_side;
    double right_side;
    int l_p;
    int r_p;
    //find mid point
    p_mid=n/2;
    
    l_p = p_mid;
    r_p = n - p_mid;
    //one or both sides consist less than 3 points, brute
    if(n<=3){
        return brute_force(P,n);
    }else{
        //recursive left halves
        left_side = _closest_serial(P,l_p);
        //recrusive right halves
        right_side = _closest_serial(&P[l_p],r_p);
        //find smallest 1
        d = find_small(left_side,right_side);
        return combine_lr(P,n,P[l_p],d);
    }    
    
}

/*
 * Wrapper function that sorts P[] and then calls _closest_serial().
 * Use this function if P[] is not pre-sorted.
 */
double closest_serial(struct Point P[], size_t n)
{
    //first sort the points by x
    qsort(P,n,sizeof(struct Point), compare_x);
    return _closest_serial(P,n);
    
}
