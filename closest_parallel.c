#include "closest_parallel.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
/*read check*/
void p_read(int fd, double *dl){
    if(read(fd,dl,sizeof(*dl)) == -1){
        perror("read fail");
        exit(1);
    }
}
/*write check*/
void p_write(int fd, double dl){
    if(write(fd,&dl,sizeof(dl)) == -1){
        perror("write fail");
        exit(1);
    }
}
/*close check*/
void p_close(int fd){
    if(close(fd) == -1){
        perror("close fail");
        exit(1);
    }
}
/*check pipe*/
void p_pipe(int *child){
    if(pipe(child) == -1){
        perror("pipe");
        exit(1);
    }
}
/*find the smallest distance between left and right sides*/
double find_smalld(double x,double y){
    if (x<y) {
        return x;
    }else{
        return y;
    }
}


int curr_depth = 0;
double _closest_parallel(struct Point P[], size_t n, int pdmax, int *pcount)
{
    static int num_forks = 0;//count
    int point_mi = n/2;
    int point_r = n-n/2;
    int left_fd[2];
    int right_fd[2];
    //basecase
    if(n<=3||pdmax ==0){
        return _closest_serial(P,n);
    }
    
    if(verbose){
        printf("depth is %d\n",curr_depth);
        printf("forknum is %d\n",num_forks);
    }
    
    p_pipe(left_fd);
    p_pipe(right_fd);

    
    int fork1 = fork();
    if (fork1 == 0) {//child 1
        if (close(left_fd[0])==-1) {
            perror("close failed");
            exit(1);
        }
        
        double left_side = _closest_parallel(P, point_mi, pdmax-1, pcount);//&num_forks

        p_write(left_fd[1],left_side);
        
        if (close(left_fd[1]) ==-1) {
            perror("close failed");
            exit(1);
        }
        
        curr_depth++;
        exit(num_forks);
        
    }else if(fork1 < 0){
        perror("fork1 failed");
        exit(1);
    }
    //
    close(left_fd[1]);
    
    int fork2 = fork();
    if (fork2 == 0) {//child 2
        curr_depth++;
        
        close(left_fd[0]);//
        close(left_fd[1]);
        
        if(verbose){
            printf("depth is %d\n",curr_depth);
            printf("forknum is %d\n",num_forks);
        }
        
        double right_side = _closest_parallel(&P[point_mi], point_r, pdmax-1, pcount);
        //-write result of pipe2
        p_close(right_fd[0]);
        p_write(right_fd[1],right_side);
        
        p_close(right_fd[1]);
        exit(num_forks);
        
    }else if(fork2 <0){
        perror("fork2");
        exit(1);
    }
    if (fork1 > 0 && fork2 >0) {
        
        
        int sta1e;
        int sta2e;
        int status1;
        int status2;
        
        (*pcount) +=2;

        
        if(waitpid(fork1,&status1,0) == -1){
            perror("waitpid1");
            exit(1);
        }
        if(waitpid(fork2,&status2,0) ==-1){
            perror("waitpid2");
            exit(1);
        };
        
        
        sta1e = WEXITSTATUS(status1);
        sta2e = WEXITSTATUS(status2);
        
        if (sta1e ==-1) {
            perror("status1");
            exit(1);
            
        }else if(sta2e == -1){
            perror("status2");
            exit(1);
        }else{
            num_forks += sta1e +sta2e +2;
            }
       
        *pcount += sta1e +sta2e;
        
    }else{
        perror("fork12");
        exit(1);
    }
    
    double d;
    double left_side;
    double right_side;

    p_read(left_fd[0],&left_side);
    p_read(right_fd[0],&right_side);
    
    p_close(left_fd[0]);
    p_close(right_fd[0]);
    
    if(verbose){
        printf("depth is %d\n", curr_depth);
        printf("forknum is %d\n", num_forks);
    }
    
    d = find_smalld(left_side, right_side);
    
    return combine_lr(P,n,P[point_mi],d);
}

double closest_parallel(struct Point P[], size_t n, int pdmax, int *pcount)
{
    
    qsort(P,n,sizeof(struct Point), compare_x);
    
    return _closest_parallel(P,n,pdmax,pcount);
    
}
