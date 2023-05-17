#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

#define TOTAL_DICE 5
#define HALF_DICE TOTAL_DICE/2+1

#define m2_3_ALL_POSSIBLE_SCORE 5355
#define m1_4_ALL_POSSIBLE_SCORE 2730

#define m2_3_ALL_POSSIBLE_TYPE 306
#define m1_4_ALL_POSSIBLE_TYPE 156

#define P_abcd (double)1200/7776
#define P_abcde (double)240/7776

#define m2_3_AVERAGE (double)(m2_3_ALL_POSSIBLE_SCORE)/(double)(m2_3_ALL_POSSIBLE_TYPE)
#define m1_4_AVERAGE (double)(m1_4_ALL_POSSIBLE_SCORE)/(double)(m1_4_ALL_POSSIBLE_TYPE)
#define mALL_AVERAGE (double)(TOTAL_DICE)*3.5

#define mALL_PARAMETER (1)
#define m2_3_PARAMETER (1)
#define m1_4_PARAMETER (1)

#define SUM(n) (1+n)*n/2
#define SUMSUM(n) (n)*(n+1)*(n+2)/6
#define SUMSUMSUM(n) (n)*(n+1)*(n+2)*(n+3)/24
#define BREAK_TIME 1

#define ERR(){                                                                  \
	printf("\e[41m[ Waring ] error happen in %s function\e[0m \n" , __func__ ); \
	exit(0);                                                                    \
}
#define NOTICE(string) printf("\e[1;46m[ NOTICE ] %s\e[0m \n" , string );

//                            Yah  1_4        abcde 2_3 abcd         all
int miss_order[12] = { 1 , 2 , 12 , 9 , 3 , 4 , 11 , 8 , 10 , 5 , 6 , 7 };// parameter can change
int miss_index = 0;

typedef struct Sheet{
	bool filled[13];
	int score[13];
}Sheet;

typedef struct Consideration{
	int score[13];
	bool need[13];
}Consideration;

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
	ERR();
	return -1;
}
int sum( int level , int n ){
	if( level == -1 )     return 1;
	else if( level == 0 ) return n;
	else if( level == 1 ) return SUM(n);
	else if( level == 2 ) return SUMSUM(n);
	else if( level == 3 ) return SUMSUMSUM(n);
	ERR();
}
void get_amount( int dice[TOTAL_DICE] , int amount[7] ){
	memset( amount , 0 , 7*sizeof(int) );
	for(int x=0;x<5;x++){
		amount[ dice[x] ] ++;
		amount[0] += dice[x];
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
bool is_ooooo( int dice[TOTAL_DICE] ){
	for(int x=1;x<5;x++){
		if( dice[x] != dice[x-1] ) return false;
	}
	return true;
}

void add_score_sheet( int dice[TOTAL_DICE] , Sheet *score_sheet , int index ){
	int amount[7] = {};
	get_amount( dice , amount );
	if( index > 12 || index < 1 || score_sheet->filled[index] ) ERR();
	if( 1 <= index && index <= 6 ){
		score_sheet->score[0]    += amount[index] * index; // [0] stores the current sum of number 1~6
		score_sheet->score[index] = amount[index] * index;
	}
	if( index == 7 )               score_sheet->score[index] = amount[0];
	if( index == 8 )               score_sheet->score[index] = (is_2_3(amount))?  amount[0] : 0;
	if( index == 9 )               score_sheet->score[index] = (is_1_4(amount))?  amount[0] : 0;
	if( index == 10 )              score_sheet->score[index] = (is_abcd(amount))?        15 : 0;
	if( index == 11 )              score_sheet->score[index] = (is_abcde(amount))?       30 : 0;
	if( index == 12 )              score_sheet->score[index] = (is_ooooo(dice))?         50 : 0;
	score_sheet->filled[index] = true;
	return ;
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
	if( mode == 1 ) NOTICE("[ Notice ] Yellow means re throw again.");
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
		printf(" \n");
	}
}
void show_score_sheet( Sheet score_sheet ){
	char info[256] = {};
	char line[256] = {};
	printf("\e[1;46m[ Notice ] Your score sheet is ...\e[0m \n");
	sprintf( info , "    Type   | index | Score ");
	//                    12        7
	memset( line , '-' , strlen(info) );
	printf("%s\n" , info );
	printf("%s\n" , line );
	for(int x=1;x<=6;x++){
		if( score_sheet.filled[x] ) printf("    %d      |   %d   |  %2d   \n" , x , x , score_sheet.score[x] );
		else                        printf("    %d      |   %d   |       \n" , x , x );
	}
	if( score_sheet.filled[7] )     printf("all select |   7   |  %2d   \n" , score_sheet.score[7] );
	else                            printf("all select |   7   |       \n");
	if( score_sheet.filled[8] )     printf("  oooxx    |   8   |  %2d   \n" , score_sheet.score[8] );
	else                            printf("  oooxx    |   8   |       \n");
	if( score_sheet.filled[9] )     printf("  oooox    |   9   |  %2d   \n" , score_sheet.score[9] );
	else                            printf("  oooox    |   9   |       \n");
	if( score_sheet.filled[10] )    printf("   1234    |  10   |  %2d   \n" , score_sheet.score[10] );
	else                            printf("   1234    |  10   |       \n");
	if( score_sheet.filled[11] )    printf("  12345    |  11   |  %2d   \n" , score_sheet.score[11] );
	else                            printf("  12345    |  11   |       \n");
	if( score_sheet.filled[12] )    printf("  Yahtzee  |  12   |  %2d   \n" , score_sheet.score[12] );
	else                            printf("  Yahtzee  |  12   |       \n");
}

void recur_dice_value( int ret[TOTAL_DICE] , int num_dice , int level ){
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
void recur_dice_index( int ret[TOTAL_DICE] , int num_dice , int level ){
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

int score_index_get( int amount[7] , int index ){
	if( index <= 6 ) return amount[index]*index;
	if( index <= 9 ) return amount[0];
	if( index <= 11 ) return 15*(index-9);
	if( index == 12 ) return 50;
	ERR();
}
void score_want_get( int dice[TOTAL_DICE] , Sheet score_sheet , int ret[2] ){ // ret[0] is the score , ret[1] is the index
	ret[0] = -1;
	ret[1] = -1;
	int amount[7] = {};
	get_amount( dice , amount );
	while( score_sheet.filled[miss_order[miss_index]] ){
		miss_index ++;
	}
	int miss_score = score_index_get( amount , miss_order[miss_index] );
	for(int x=1;x<=12;x++){
		int index_get = score_index_get(amount,x);

		if( score_sheet.filled[x] ) continue;
		if( x <= 6 && amount[x] < HALF_DICE ) continue;
		if( index_get < ret[0] ) continue;

		if( x == 7 && (index_get<=mALL_AVERAGE*mALL_PARAMETER) ) continue;
		if( x == 8 && (index_get<=m2_3_AVERAGE*m2_3_PARAMETER || !is_2_3(amount)) ) continue;
		if( x == 9 && (index_get<=m1_4_AVERAGE*m1_4_PARAMETER || !is_1_4(amount)) ) continue;
		if( x == 10 && (!is_abcd(amount)) ) continue;
		if( x == 11 && (!is_abcde(amount)) ) continue;
		if( x == 12 && (!is_ooooo(dice)) ) continue;
		ret[0] = index_get;
		ret[1] = x;
	}
	if( ret[1] == -1 ){
		ret[0] = miss_score;
		ret[1] = miss_order[miss_index];
	}
}

void get_strategy( int dice[5] , bool strategy[5] , Sheet score_sheet ){ // true -> throw again
	int temp_dice[5] = {};
	double **expected_score = calloc( 6 , sizeof(double*) ); // re throw 0~5 dices
	for(int x=0;x<=TOTAL_DICE;x++){
		expected_score[x] = calloc( C_m_n(TOTAL_DICE,x) , sizeof(double) );
	}
	int re_throw_value[5] = {};
	int re_throw_index[5] = {};
	int ret[2] = {};
	score_want_get( dice , score_sheet , ret );
	expected_score[0][0] = ret[0];
	for(int x=1;x<=TOTAL_DICE;x++){ // x means the amount of dices re thrown
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
				score_want_get( temp_dice , score_sheet , ret );
				expected_score[x][y] += ret[0];
			}
			expected_score[x][y] /= (double) num_dice_possible_result;
		}
	}
	double max_expected_value = 0;
	int num_dice_rethrow = -1 , index_rethrow = -1;
	for(int x=0;x<=TOTAL_DICE;x++){
		int choose = C_m_n( TOTAL_DICE , x );
		for(int y=0;y<choose;y++){
			printf("%lf " , expected_score[x][y] );
			if( max_expected_value < expected_score[x][y] ){
				max_expected_value = expected_score[x][y];
				num_dice_rethrow = x;
				index_rethrow = y;
			}
		}
		printf("\n");
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
    Sheet score_sheet = {};
    int dice[TOTAL_DICE] = {};

    for(int x=0;x<12;x++){
		printf("\e[1;46m[ NOTICE ] %d round\e[0m \n" , x+1 );
		bool strategy[TOTAL_DICE] = {};
		int result[2] = {};
		// ************************************************** first input and show
        #ifdef USER_INPUT
			NOTICE("Please enter the value you throw.");
			scanf("%d %d %d %d %d" , &dice[0] , &dice[1] , &dice[2] , &dice[3] , &dice[4] );
        #elif defined PROGRAM_INPUT
			for(int y=0;y<TOTAL_DICE;y++){
				dice[y] = rand()%6+1;
			}
        #endif
		show_dice( 0 , dice , strategy );
		sleep(BREAK_TIME);

		// ************************************************** first strategy and show
		get_strategy( dice , strategy , score_sheet );
		show_dice( 1 , dice , strategy );

		// **************************************************
		NOTICE("2 chance remain to throw.");
		sleep(BREAK_TIME);

		// ************************************************** second input and show
		#ifdef USER_INPUT
			NOTICE("Please enter the value you throw.");
			scanf("%d %d %d %d %d" , &dice[0] , &dice[1] , &dice[2] , &dice[3] , &dice[4] );
        #elif defined PROGRAM_INPUT
			for(int y=0;y<TOTAL_DICE;y++){
				if( strategy[y] ) dice[y] = rand()%6+1;
			}
        #endif
		memset( strategy , 0 , TOTAL_DICE*sizeof(bool) );
		show_dice( 0 , dice , strategy );
		sleep(BREAK_TIME);

		// ************************************************** second strategy and show
		get_strategy( dice , strategy , score_sheet );
		show_dice( 1 , dice , strategy );

		// **************************************************
		NOTICE("1 chance remain to throw.");
		sleep(BREAK_TIME);

		// ************************************************** third input and show
		#ifdef USER_INPUT
			NOTICE("Please enter the value you throw.");
			scanf("%d %d %d %d %d" , &dice[0] , &dice[1] , &dice[2] , &dice[3] , &dice[4] );
        #elif defined PROGRAM_INPUT
			for(int y=0;y<TOTAL_DICE;y++){
				if( strategy[y] ) dice[y] = rand()%6+1;
			}
        #endif
		memset( strategy , 0 , TOTAL_DICE*sizeof(bool) );
		show_dice( 0 , dice , strategy );
		sleep(BREAK_TIME);
		// **************************************************
		score_want_get( dice , score_sheet , result );
		printf("\e[1;46mindex: %d is the program wants to select.\e[0m \n" , result[1] );
		printf("\e[1;46mAnd it should get %d score.\e[0m \n" , result[0] );

		// ************************************************** only calculate which index should select
		#ifdef USER_INPUT
			int index = 0;
			show_score_sheet( score_sheet );
			NOTICE("Please enter the index you want to select.");
			scanf("%d" , &index );
			add_score_sheet( dice , &score_sheet , index );
        #elif defined PROGRAM_INPUT
			add_score_sheet( dice , &score_sheet , result[1] );
        #endif
		show_score_sheet( score_sheet );

		// **************************************************
		#ifdef PROGRAM_INPUT
			sleep(5);
		#endif
    }
	if( score_sheet.score[0] >= 63 ) score_sheet.score[0] += 35;                   // Bonus
	for(int x=7;x<=12;x++)           score_sheet.score[0] += score_sheet.score[x]; // Sum the total score to [0]
	NOTICE("Your total score is ");
	printf("\e[1;46m %d \e[0m \n" , score_sheet.score[0] );
	memset( &score_sheet , 0 , sizeof(Sheet) );


    return 0;
}
