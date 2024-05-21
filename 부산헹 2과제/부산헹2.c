#include<stdio.h>
#include<time.h>
#include<windows.h>

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


//변수 선언
int train_length, percentile_probability, madongseok_stamina, PM, PC, PZ;
int turn = 1;
int Citizen, Zombie, Madongseok;
int Random_NumC, Random_NumZ;
int move, rest;
int Citizen_Aggro, Madongseok_Aggro;
int action_Madongseok;



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
		printf("train length (%d ~ %d) >> ", LEN_MIN, LEN_MAX);
		scanf_s("%d", &train_length);
	}
}

//확률
void percent(void) {
	printf("percentile probability 'p'(%d ~ %d) >> ", PROB_MIN, PROB_MAX);
	scanf_s("%d", &percentile_probability);
	while (percentile_probability < PROB_MIN || percentile_probability > PROB_MAX) {
		printf("percentile probability 'p'(%d ~ %d) >> ", PROB_MIN, PROB_MAX);
		scanf_s("%d", &percentile_probability);
	}
}

//마동석 체력
void Stamina(void) {
	printf("madongseok stamina(%d ~ %d) >> ", STM_MIN, STM_MAX);
	scanf_s("%d", &madongseok_stamina);
	while (madongseok_stamina < STM_MIN || madongseok_stamina > STM_MAX) {
		printf("madongseok stamina (%d ~ %d) >> ", STM_MIN, STM_MAX);
		scanf_s("%d", &madongseok_stamina);
	}
}

//마동석 조건
void Condition(void) {
	printf("madongseok move (0:stay, 1:left) >> ");
	scanf_s("%d", &move);
	while (move != 1 && move != 0) {
		printf("madongseok move (0:stay, 1:left) >> ");
		scanf_s("%d", &move);
	}
}

//열차 초기 상태
void displayTrain(void) {
	PM = train_length - 2;	//초기화
	PZ = train_length - 3;	//초기화
	PC = train_length - 6;	//초기화

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

//열차 상태
void basicTrain(void) {
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


void C_movement(void) {
	//시민 이동
	Random_NumC = rand() % 100 + 1;
	if (Random_NumC <= (100 - percentile_probability) && PC > 1) {
		PC--;
		if (Citizen_Aggro < AGGRO_MAX) {
			Citizen_Aggro++;
		}
	}
}
void Z_movement(void) {
		//좀비 이동
	if (turn % 2 == 1) {
		Random_NumZ = rand() % 100 + 1;
		if (Random_NumZ <= percentile_probability && PZ > 1) {
			PZ--;
		}
	}
}
void M_movement(void) {
	//마동석 이동
	if (move == 1) {
		PM--;
	}
}
	
//이동
void movement(void) {
	C_movement();
	Z_movement();
	M_movement();
	basicTrain();
}

//시민, 좀비 이동 출력
void updatePosition(void) {
	//시민 이동 출력
	if (Citizen == PC) {
		printf("\ncitizen : stay %d (aggro : %d)\n", PC, Citizen_Aggro);
	}
	else {
		printf("\ncitizen : %d -> %d (aggro : %d)\n", Citizen, PC, Citizen_Aggro);
	}

	//좀비 이동 출력
	if (turn % 2 == 1) {
		if (Zombie == PZ) {
			printf("zombie : stay %d\n\n", PZ);
		}
		else {
			printf("zombie : %d -> %d\n\n", Zombie, PZ);
		}
	}
	else {
		printf("zombie : stay %d(cannot move)\n\n", PZ);
	}
}

//마동석 대기
void Rest(void) {
	printf("madongseok action (%d.rest, %d.provoke) >> ", ACTION_REST, ACTION_PROVOKE);
	scanf_s("%d", &rest);

	if (rest == ACTION_REST) {
		printf("madongseok rest...\n");
		printf("madongseok: %d (aggro : %d, stamina : %d)\n", PM, AGGRO_MIN, madongseok_stamina);
	}
}


int main(void) {
	srand((unsigned int)time(NULL));
	intro(); // 시작
	train(); // 열차 길이
	Stamina(); // 마동석 체력
	percent(); // 확률
	displayTrain(); //처음 열차
	turn = 1;
	Citizen = 0;
	Zombie = 0;
	Madongseok = 0;
	while (1) {
		Citizen = PC;
		Zombie = PZ;
		Madongseok = PM;
		movement(); // 시민, 좀비 이동
		updatePosition(); // 시민, 좀비 이동 출력
		Condition(); // 마동석 조건
		movement(); // 열차 다시 출력
		printf("madongseok : stay %d (aggro : %d, stamina : %d)\n", PM, AGGRO_MIN, madongseok_stamina);
		printf("Citizen does nothing.\n");
		printf("zombie attaked nobody.\n");
		Rest(); // 마동석 대기
		if (PC == PZ - 1 || PC == 1) {
			outro(); // 끝
			break;
		}
		turn++;
		Sleep(4000);
	}
}