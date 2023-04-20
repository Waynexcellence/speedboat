#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#define m2_3_ALL_POSSIBLE_SCORE 5355
#define m1_4_ALL_POSSIBLE_SCORE 2730

#define m2_3_ALL_POSSIBLE_TYPE 306
#define m1_4_ALL_POSSIBLE_TYPE 156

#define ALL_PARAMETER (2/3)

#define TOTAL_DICE 5

#define SUM(n) (1+n)*n/2
#define SUMSUM(n) (n)*(n+1)*(n+2)/6
#define SUMSUMSUM(n) (n)*(n+1)*(n+2)*(n+3)/24

int C_m_n( int m , int n ){
	int result = 1;
	if( n > m/2 ) n = m-n;
	for(int x=m-n+1;x<=m;x++) result *= x;
	for(int x=2;x<=n;x++)     result /= x;
	return result;
}

int power6( int n ){
	if( n == 0 ) return 1;
	if( n == 1 ) return 6;
	if( n == 2 ) return 36;
	if( n == 3 ) return 216;
	if( n == 4 ) return 1296;
	if( n == 5 ) return 7776;
	return -1;
}

int sum( int level , int n ){
	if( level == -1 )     return 1;
	else if( level == 0 ) return n;
	else if( level == 1 ) return SUM(n);
	else if( level == 2 ) return SUMSUM(n);
	else if( level == 3 ) return SUMSUMSUM(n);
	return -1;
}

void get_amount( int dice[5] , int amount[7] ){
	for(int x=0;x<5;x++){
		amount[ dice[x] ] ++;
	}
	return;
}

bool is_2_3( int amount[7] ){
	bool three = false , two = false;
	for(int x=1;x<=6;x++){
		if( amount[x] == 5 ) return true;
		if( amount[x] == 3 ) three = true;
		if( amount[x] == 2 ) two = true;
	}
	if( three && two ) return true;
	return false;
}

bool is_1_4( int amount[7] ){
	bool four = false , one = false;
	for(int x=1;x<=6;x++){
		if( amount[x] == 5 ) return true;
		if( amount[x] == 4 ) four = true;
		if( amount[x] == 1 ) one = true;
	}
	if( four && one ) return true;
	return false;
}

bool is_abcd( int amount[7] ){
	if( !amount[3] || !amount[4] ) return false;
	if( amount[1] && amount[2] ) return true;
	if( amount[2] && amount[5] ) return true;
	if( amount[5] && amount[6] ) return true;
	return false;
}

bool is_abcde( int amount[7] ){
	if( !amount[2] || !amount[3] || !amount[4] || !amount[5] ) return false;
	if( amount[1] || amount[6] ) return true;
	return false;
}

bool is_ooooo( int dice[5] ){
	for(int x=1;x<5;x++){
		if( dice[x] != dice[x-1] ) return false;
	}
	return false;
}

void max_score_can_get( int dice[5] , int score_sheet[13] , int ret[2] ){
	memset( ret , 0 , 2*sizeof(int) );
	int amount[7] = {};
	get_amount( dice , amount );
	int sum = 0;
	for(int y=0;y<5;y++){
		sum += dice[y];
	}
	int max_score = -1;
	for(int x=1;x<=6;x++){
		if( score_sheet[x] ) continue;
		if( amount[x]*x > max_score ){ max_score = amount[x]*x; ret[1] = x; }
	}
	for(int x=7;x<=12;x++){
		if( score_sheet[x] ) continue;
		if     ( sum > max_score && x == 7 && sum > 21*(ALL_PARAMETER) ){ max_score = sum; ret[1] = x; }
		else if( sum > max_score && x == 8 && is_2_3(amount) )  { max_score = sum; ret[1] = x; }
		else if( sum > max_score && x == 9 && is_1_4(amount) )  { max_score = sum; ret[1] = x; }
		else if( 15 > max_score && x == 10 && is_abcd(amount) ) { max_score = 15;  ret[1] = x; }
		else if( 30 > max_score && x == 11 && is_abcde(amount) ){ max_score = 30;  ret[1] = x; }
		else if( 50 > max_score && x == 12 && is_ooooo(dice) )  { max_score = 50;  ret[1] = x; }
	}
	ret[0] = max_score;
}

void show_dice( int mode , int dice[TOTAL_DICE] , bool strategy[TOTAL_DICE] ){
	char output[5][256] = {};
	int length = TOTAL_DICE*7;
	for(int x=0;x<TOTAL_DICE;x++){
		sprintf( &(output[1][x*7]) , "b     b" );
		sprintf( &(output[2][x*7]) , "b  %d  b" , dice[x] );
		sprintf( &(output[3][x*7]) , "b     b" );
	}
	memset( output[0] , 'b' , length );
	memset( output[4] , 'b' , length );
	if( mode == 1 ) printf("\e[1m[ Notice ]\e[0m\e[1;33mYellow means re throw again.\e[0m\n");
	for(int x=0;x<5;x++){
		for(int y=0;y<length;y++){
			if( strategy[y/7] ){
				if( output[x][y] == 'b' ) printf("\e[33;43m%c\e[0m" , output[x][y] );
				else                      printf("\e[33m%c\e[0m" , output[x][y] );
			}
			else{
				if( output[x][y] == 'b' ) printf("\e[37;47m%c\e[0m" , output[x][y] );
				else                      printf("%c" , output[x][y] );
			}
		}
		printf("\n");
	}
}

void recur_dice_value( int ret[5] , int num_dice , int level ){
	memset( ret , 0 , 5*sizeof(int) );
	int num_power = num_dice;
	while( level > 0 ){
		int minus = power6(num_power);
		if( level < minus ){
			num_power -= 1;
			continue;
		}
		level -= minus;
		ret[num_power] ++;
	}
	for(int x=0;x<num_dice;x++){
		ret[x] ++;
	}
}

void recur_dice_index( int ret[5] , int num_dice , int level ){
	memset( ret , 0 , 5*sizeof(int) );
	int num_start = (TOTAL_DICE-1)-(num_dice-1)+1; 
	for(int x=0;x<num_dice;x++){ // ret[x]
		int diff = num_dice-2-x;
		for(int y=num_start;y>=1;y--){
			int num_skip = sum( diff , y );
			assert( num_skip > -1); // calculate error
			if( level >= num_skip ){
				level -= num_skip;
				num_start --; // magical
				assert( num_start != 0 );
				ret[x] ++;
				if( level == 0 ){ // speed up
					for(int z=x+1;z<num_dice;z++){
						ret[z] = ret[z-1]+1;
					}
					return ;
				}
			}
			else{
				if( x+1 < num_dice ) ret[x+1] = ret[x]+1;
				break;
			}
		}
	}
}

void get_strategy( int dice[5] , bool strategy[5] , int score_sheet[13] ){ // true -> throw again
	int temp_dice[5] = {};
	double **expected_score = calloc( 6 , sizeof(double*) ); // re throw 0~5 dices
	for(int x=0;x<6;x++){
		expected_score[x] = calloc( C_m_n(5,x) , sizeof(double) );
	}
	int re_throw_value[5] = {};
	int re_throw_index[5] = {};
	int ret[2] = {};
	for(int x=0;x<=TOTAL_DICE;x++){ // x means the amount of dices re thrown
		int choose = C_m_n( TOTAL_DICE , x ); // choose also equals to the length of expected_score[x]
		int num_dice_possible_result = power6(x);
		for(int y=0;y<choose;y++){
			recur_dice_index( re_throw_index , x , y );
			for(int z=0;z<num_dice_possible_result;z++){
				recur_dice_value( re_throw_value , x , z );
				memcpy( temp_dice , dice , sizeof(int)*5 ); // load original dice to temp_dice
				for(int m=0;m<x;m++){
					temp_dice[ re_throw_index[m] ] = re_throw_value[m];
				}
				max_score_can_get( temp_dice , score_sheet , ret );
				expected_score[x][y] += ret[0];
			}
			expected_score[x][y] /= (double) num_dice_possible_result;
		}
	}
	double max_expected_value = 0;
	int num_dice_rethrow = -1 , index_rethrow = -1;
	for(int x=0;x<=TOTAL_DICE;x++){
		int choose = C_m_n( TOTAL_DICE , x );
		for(int y=0;y<=choose;y++){
			if( max_expected_value < expected_score[x][y] ){
				max_expected_value = expected_score[x][y];
				num_dice_rethrow = x;
				index_rethrow = y;
			}
		}
	}
	assert( num_dice_rethrow != -1 );
	assert( index_rethrow != -1 );
	recur_dice_index( re_throw_index , num_dice_rethrow , index_rethrow );
	for(int m=0;m<num_dice_rethrow;m++){
		strategy[ re_throw_index[m] ] = true;
	}
	for(int x=0;x<6;x++){
		free( expected_score[x] );
	}
	free( expected_score );
}

int main(){
    srand( time(0) );
    // 35 1 2 3 4 5 6 all 2_3 1_4 abcd abcde ooooo
    int score_sheet[13] = {};
    int dice[5] = {};
	bool calculate = true;
    for(int x=0;x<1;x++){
        #ifdef USER_INPUT
		printf("\e[1m[ Notice ] Please enter the value of dice separated with a space, thank you.\e[0m\n");
        scanf("%d %d %d %d %d" , &dice[0] , &dice[1] , &dice[2] , &dice[3] , &dice[4] );
        #elif defined PROGRAM_INPUT
        for(int y=0;y<5;y++){
            dice[y] = rand()%6+1;
        }
        #endif

		bool strategy[5] = {};
		show_dice( 0 , dice , strategy );
		
		if( calculate ) get_strategy( dice , strategy , score_sheet );
		else{
			for(int y=0;y<TOTAL_DICE;y++){
				if( rand()%2 ) strategy[y] = true;
			}
		}

		show_dice( 1 , dice , strategy );
		#ifdef USER_INPUT
		printf("\e[1m[ Notice ] Please enter\e[0m \e[1;4mall\e[0m \e[1mthe value of dice separated with a space, thank you.\e[0m\n");
        scanf("%d %d %d %d %d" , &dice[0] , &dice[1] , &dice[2] , &dice[3] , &dice[4] );
        #elif defined PROGRAM_INPUT
        for(int y=0;y<5;y++){
            if( strategy[y] ) dice[y] = rand()%6+1;
        }
        #endif

		memset( strategy , 0 , TOTAL_DICE*sizeof(bool) );
		show_dice( 0 , dice , strategy );
    }
    return 0;
}
