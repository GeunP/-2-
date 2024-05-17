#include<stdio.h>

#define LEN_MIN 15     //기차 길이
#define LEN_MAX 50
#define STM_MIN 0      //마동석 체력  
#define STM_MAX 5
#define PROB_MIN 10    //확률
#define PROB_MAX 90
#define AGGRO_MIN 0    //어그로 범위
#define AGGRO_MAX 5

//마동석 이동 방향
#define MOVE_LEFT 1
#define MOVE_STAY 0

//좀비의 공격 대상
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

//마동석 행동
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

//랜덤변수
srand((unsigned)time(NULL));

//변수 선언
int train_length, percentile_probability, PM, PC, PZ, turn;

//함수 선언
int updatePosition(int, int, int);
int displayMove(int, int, char);

//인트로
void intro(void) {
	printf("\n==========GAME START==========\n\n");
	printf("   좀비를 피해서 도망가시오.");
	printf("\n\n==============================\n\n");
}

//아웃트로
void outro(void) {
	if (PC == PZ - 1) {
		printf("GAME OVER!\nTHE citizen has been caught by the zombie.\n");
	}


	if (PC == 1) {
		printf("SUCCESS!\nTHE citizen has escaped to the next train.\n");
	}
}

//열차 길이
void train(void) {
	printf("train length (%d ~ %d) >> ", LEN_MIN, LEN_MAX);
	scanf_s("%d", &train_length);
	while (train_length < LEN_MIN || train_length > LEN_MAX) {
		printf("INVALID INPUT");
		scanf_s("%d", &train_length);
	}
}

//확률
void percent(void) {
	printf("percentile probability (%d ~ %d) >> ", PROB_MIN, PROB_MAX);
	scanf_s("%d", &percentile_probability);
	while (percentile_probability < PROB_MIN || percentile_probability > PROB_MAX) {
		printf("INVALID INPUT");
		scanf_s("%d", &percentile_probability);
	}
}

//열차상태
void displayTrain(void) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < train_length; j++) {
			if (i == 0 || i == 2) printf("#");
			else {
				if (j == 0 || j == train_length - 1) printf("#");
				else if (j == PC) printf("C");
				else if (j == PZ) printf("Z");
				else if (j == PM) printf("M");
				else printf(" ");
			}
		}
		printf("\n");
	}
	printf("\n");
}

//dd