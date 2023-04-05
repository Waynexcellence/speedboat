#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

bool is_3_2(){

}

int main(){
    srand( time(0) );
    // 35 1 2 3 4 5 6 全選 葫蘆 鐵支 小順 大順 快艇
    int score_sheet[13] = {};
    int dice[5] = {};
    for(int x=0;x<13;x++){
        #ifdef USER_INPUT
        scanf("%d %d %d %d %d" , &dice[0] , &dice[1] , &dice[2] , &dice[3] , &dice[4] );
        #endif
        #ifdef PROGRAM_INPUT
        for(int y=0;y<5;y++){
            dice[y] = rand()%6+1;
        }
        #endif
        for(int y=0;y<5;y++){
            printf("%d " , dice[y] );
        }
    }
    return 0;
}
