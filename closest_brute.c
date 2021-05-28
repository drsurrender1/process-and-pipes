#include "closest_brute.h"
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

double brute_force(struct Point P[], size_t n)
{
    double curr =-1;
    double final_d;

    int i =0;
    int j =0;
    if(n == 1){
        perror("write fail");
        exit(1);
    }

    while(i<n){

        while(j<i){
            final_d = dist(P[i],P[j]);
            if(curr ==-1){
                curr = final_d;
            }
            if(curr<final_d){
                final_d =curr;
            }
            j++;
        }
        i++;
    }
    return final_d;
}
