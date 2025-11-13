#include <stdlib.h>
#include <stdio.h>

int main(){

    int a1[2][3] = {{1,2,3},{4,5,6}};
    int a2[3][2] = {{1,2},{3,4},{5,6}};
    int a3[2][2] = {{0,0},{0,0}};

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 2; j++){
            for(int k = 0; k < 3; k++){
                a3[i][j] += a1[i][k] * a2[k][j];
            }
        }
    }

    for(int i = 0; i < 2; i++){
        for(int j = 0 ; j < 2; j++){
            printf("%d, ",a3[i][j]);
        }
        printf("\n");
    }

}