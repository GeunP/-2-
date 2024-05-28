// Coded by : 이근표
// Date     : 24/05/28

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
#define MOVE_STAY 0
#define MOVE_LEFT 1

//좀비의 공격 대상
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

//마동석 행동
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

// 변수 초기화를 위함 임의값
#define initialValue 10000

//변수 선언
int train_length, percentile_probability, PM, PC, PZ;
int Turnphase;
int Citizen, Zombie, Madongseok;
int Random_NumC, Random_NumZ;
int move, rest;
int Citizen_Aggro;  // Aggro 변수 추가
int Madongseok_Stamina, Madongseok_Aggro, Madongseok_Action, Madongseok_Hold;  // 마동석 변수
int Position_C, Position_Z, Position_M;     // Citizen, Zombie and Madongseok's Position Value
int Action_C, Action_Z, Action_M;            // Citizen, Zombie and Madongseok's Action Value
int GameOver;                               // Game Over Fag, '0':ing, '1':end


//인트로
void intro(void) {
	printf("\n==========GAME START==========\n\n");
	printf("   좀비를 피해서 도망가시오.");
	printf("\n\n==============================\n\n");
}

//아웃트로
void outro(void) {
	if (PC == PZ - 1) {
		printf("\nGAME OVER!\n THE citizen has been caught by the zombie.\n");
	}
	if (PC == 1) {
		printf("\nYOU WIN!\n THE citizen has escaped to the next train.\n");
	}
}

//열차 길이
void TrainLength(void) {
	train_length = initialValue; // 임의값 초기화
	while (train_length < LEN_MIN || train_length > LEN_MAX) {
		printf("train length (%d ~ %d) >> ", LEN_MIN, LEN_MAX);
		scanf_s("%d", &train_length);
	}
}

//확률 
void Percent(void) {
	percentile_probability = initialValue; // 임의값 초기화
	while (percentile_probability < PROB_MIN || percentile_probability > PROB_MAX) {
		printf("percentile probability 'p'(%d ~ %d) >> ", PROB_MIN, PROB_MAX);
		scanf_s("%d", &percentile_probability);
	}

}

//마동석 체력
void Stamina(void) {
	Madongseok_Stamina = initialValue; // 임의값 초기화
	while (Madongseok_Stamina < STM_MIN || Madongseok_Stamina > STM_MAX) {
		printf("madongseok stamina (%d ~ %d) >> ", STM_MIN, STM_MAX);
		scanf_s("%d", &Madongseok_Stamina);
	}
}

//열차 상태
void Train(void) {
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

//시민 이동
void C_movement(void) {
	if (PC > 1) {
		Random_NumC = rand() % 100 + 1;
		if (Random_NumC <= (100 - percentile_probability)) { // 시민 좌로 1 이동
			PC--;
			if (Citizen_Aggro < AGGRO_MAX) {
				Citizen_Aggro++;
				Position_C = 1;
			}
			else {
				Position_C = 2;
			}
		}
		else { // 시민 제자리
			if (Citizen_Aggro > AGGRO_MIN) {
				Citizen_Aggro--;
				Position_C = 3;
			}
			else {
				Position_C = 4;
			}
		}
	}
	else {
		GameOver = 1;
	}
}

//Position_C의 switch문
void displayPosition_C(void) {
	switch (Position_C) {
	case 1:
		printf("\nCitizen : %d -> %d (aggro : %d -> %d)\n", Citizen, PC, Citizen_Aggro - 1, Citizen_Aggro); // Citizen은 PC값이 바뀌기 전 값을 가짐
		break;
	case 2:
		printf("\nCitizen : %d - > %d (aggro : %d)\n", Citizen, PC, Citizen_Aggro); // Citizen은 PC값이 바뀌기 전 값을 가짐
		break;
	case 3:
		printf("\nCitizen : %d (aggro : %d -> %d)\n", PC, Citizen_Aggro + 1, Citizen_Aggro);
		break;
	case 4:
		printf("\nCitizen : %d (aggro : %d)\n", PC, Citizen_Aggro);
		break;
	default:
		break;
	}
}

//좀비 이동
void Z_movement(void) {
	if (Turnphase % 2 == 1 || Madongseok_Hold == 1) { // 마동석 '붙들기' 성공
		Position_Z = 1;
		Madongseok_Hold = 0;
	}
	else if (Turnphase % 2 == 0 && Madongseok_Hold == 0) { // 2턴마다 이동 가능, 마동석 '붙들기' 실패
		if (Citizen_Aggro < Madongseok_Aggro) {
			if ((PM - PZ) > 1) {
				PZ++; // 마동석 어그로가 높으면, 마동석 쪽으로 이동
				Position_Z = 2;
			}
			else {
				Position_Z = 4; // 이동하지 않음
			}
		}
		else {
			if ((PZ - PC) > 1) {
				PZ--; // 시민 어그로가 높거나 같으면, 시민 쪽으로 이동
				Position_Z = 3;
			}
			else {
				Position_Z = 4; // 이동하지 않음
			}
		}
	}
}

//Position_Z switch문
void displayPosition_Z(void) {
	switch (Position_Z) {
	case 1:
		printf("\nZombie : stay %d (cannot move)\n", PZ); // 이동할 수 없음
		break;
	case 2:
		printf("\nZombie : %d -> %d\n", Zombie, PZ); // 좀비, 마동석 쪽으로 이동
		break;
	case 3:
		printf("\nZombie : %d -> %d\n", Zombie, PZ); // 좀비, 시민 쪽으로 이동
		break;
	case 4:
		printf("\nZombie : stay %d\n", PZ); // 이동하지 않음
		break;
	default:
		break;
	}
}

//마동석 이동
void M_movement(void) {
	move = -1;
	if ((PM - PZ) == 1) {
		while (move != 0) {
			printf("\nmadongseok move (%d : stay)", MOVE_STAY);
			scanf_s("%d", &move);
		}
	}
	else {
		while (move != 0 && move != 1) {
			printf("\nmadongseok move (%d : stay, %d : left)", MOVE_STAY, MOVE_LEFT);
			scanf_s("%d", &move);
		}
	}
	if (move == MOVE_STAY) { // 제자리 대기
		if (Madongseok_Aggro > AGGRO_MIN) {
			Madongseok_Aggro--; // 마동석 어그로 감소
			Position_M = 1;
		}
		else Position_M = 2;
	}
	if (move == MOVE_LEFT) { // 왼쪽으로 이동
		PM--;
		if (Madongseok_Aggro < AGGRO_MAX) {
			Madongseok_Aggro++; // 마동석 어그로 증가
			Position_M = 3;
		}
		else Position_M = 4;
	}
}

//마동석 이동 switch문
void displayPosition_M(void) {
	switch (Position_M) {
	case 1:
		printf("\nmadongseok : stay %d (aggro : %d -> %d, stamina : %d)\n", PM, Madongseok_Aggro + 1, Madongseok_Aggro, Madongseok_Stamina);
		break;
	case 2:
		printf("\nmadongseok : stay %d (aggro : %d, stamina : %d)\n", PM, Madongseok_Aggro, Madongseok_Stamina);
		break;
	case 3:
		printf("\nmadongseok : stay %d -> %d (aggro : %d, stamina : %d)\n", Madongseok, PM, Madongseok_Aggro, Madongseok_Stamina);
		break;
	case 4:
		printf("\nmadongseok : stay %d -> %d (aggro :%d -> %d, stamina : %d)\n", Madongseok, PM, Madongseok_Aggro - 1, Madongseok_Aggro, Madongseok_Stamina);
		break;
	default:
		break;
	}
}

//시민 행동
void C_action(void) {
	if (PC == 1) {
		GameOver = 1; // Game Over
		Action_C = 1;
	}
	else {
		Action_C = 2;
	}
}

//시민 행동 switch문
void displayAction_C(void) {
	switch (Action_C) {
	case 1:
		printf("\nSUCCES!\n The citizen hase escaped to the next train.\n");
		break;
	case 2:
		printf("\nCitizen does nothing\n");
		break;
	default:
		break;
	}
}

//좀비 행동
void Z_action(void) {
	if (((PZ - PC) > 1) && ((PM - PZ) > 1)) {
		Action_Z = ATK_NONE;
	}
	else {
		if ((PZ - PC) == 1 && (PM - PZ) != 1) {
			Action_Z = ATK_CITIZEN;
		}
		else if ((PZ - PC) != 1 && (PM - PC) == 1) {
			Action_Z = ATK_DONGSEOK;
		}
		else if ((PZ - PC) == 1 && (PM - PZ) == 1) {
			if (Citizen_Aggro > Madongseok_Aggro) {
				Action_Z = ATK_CITIZEN;
			}
			else if (Citizen_Aggro < Madongseok_Aggro) {
				Action_Z = ATK_DONGSEOK;
			}
			else {
				Action_Z = ATK_NONE;
			}
		}
	}
}

//좀비 행동 switch문
void displayAction_Z(void) {
	switch (Action_Z) {
	case ATK_NONE:
		printf("\nzombie attack nobody.\n");
		break;
	case ATK_CITIZEN:
		printf("GAME OVER! citizen dead.....\n");
		GameOver = 1;
		break;
	case ATK_DONGSEOK:
		Madongseok_Stamina--;
		if (Madongseok_Stamina == STM_MIN) {
			printf("GAME OVER! madongseok dead..... (stamina : %d)\n", STM_MIN);
			GameOver = 1;
		}
		printf("\nZombie attacked madongsoke (aggro : %d vs %d, madongsoek stamina : %d -> %d", Citizen_Aggro, Madongseok_Aggro, Madongseok_Stamina + 1, Madongseok_Stamina);
	default:
		break;
	}
}

//마동석 행동
void M_action(void) {
	Madongseok_Action = initialValue; // 임의값 초기화
	if ((PM - PZ) != 1) {
		while (Madongseok_Action != ACTION_REST && Madongseok_Action != ACTION_PROVOKE) {
			printf("\nmadongseok action (%d.rest, %d.provoke) >> ", ACTION_REST, ACTION_PROVOKE);
			scanf_s("%d", &Madongseok_Action);
		}
	}
	else {
		while (Madongseok_Action < ACTION_REST || Madongseok_Action > ACTION_PULL) {
			printf("\nmadongseok action (%d.rest, %d.provoke, %d.pull) >> ", ACTION_REST, ACTION_PROVOKE, ACTION_PULL);
			scanf_s("%d", &Madongseok_Action);
		}
	}
	if (Madongseok_Action == ACTION_REST) {
		if (Madongseok_Aggro > AGGRO_MIN) {
			if (Madongseok_Stamina < STM_MAX) { // 마동석 체력이 체력 최고값 보다 작을 때
				Madongseok_Aggro--;		// 마동석 어그로 1 감소
				Madongseok_Stamina++;	// 마동석 체력 1 증가
				Action_M = 1;
			}
			else { // 마동석 체력이 체력 최고값 보다 같거나 클 때
				Madongseok_Aggro--;		// 마동석 어그로 1 감소
				Action_M = 2;
			}
		}
		else if (Madongseok_Stamina == AGGRO_MIN && Madongseok_Stamina < STM_MAX) {
			Madongseok_Stamina++;	// 마동석 체력 1 증가
			Action_M = 3;
		}
		else {
			Action_M = 4;
		}
	}
	else if (Madongseok_Action == ACTION_PROVOKE) {
		Action_M = 5;
	}
	else if (Madongseok_Action == ACTION_PULL) {
		if (Madongseok_Aggro + 1 <= AGGRO_MAX) {  // 마동석 어그로가 어그로 최고값 보다 1 작을 때
			Madongseok_Aggro = Madongseok_Aggro + 1;
		}
		else {
			Madongseok_Aggro = Madongseok_Aggro + 2;  // 마동석 어그로 2 증가, 마동석 어그로가 어그로 최고값 보다 2 이상 작을 때
		}
		Madongseok_Stamina--;						// 마동석 체력 1 감소
		Action_M = 6;
	}
}

//마동석 행동 swtich문
void displayAction_M(void) {
	switch (Action_M) {
	case 1:
		printf("\nmadongseok rest...\n");
		printf("\nmadongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", PM, Madongseok_Aggro + 1, Madongseok_Aggro, Madongseok_Stamina - 1, Madongseok_Stamina);
		break;
	case 2:
		printf("\nmadongseok rest...\n");
		printf("\nmadongseok: %d (aggro: %d -> %d, stamina: %d)\n", PM, Madongseok_Aggro + 1, Madongseok_Aggro, Madongseok_Stamina);
		break;
	case 3:
		printf("\nmadongseok rest...\n");
		printf("\nmadongseok: %d (aggro: %d, stamina: %d -> %d)\n", PM, Madongseok_Aggro, Madongseok_Stamina - 1, Madongseok_Stamina);
		break;
	case 4:
		printf("\nmadongseok rest...\n");
		printf("\nmadongseok: %d (aggro: %d, stamina: %d)\n", PM, Madongseok_Aggro, Madongseok_Stamina);
		break;
	case 5:
		printf("\nmadongseok provoked zombie...\n");
		printf("\nmadongseok: %d (aggro: %d -> %d, stamina: %d)\n", PM, Madongseok_Aggro, AGGRO_MAX, Madongseok_Stamina);
		Madongseok_Aggro = AGGRO_MAX; // 어그로 최대로 증가 (AGGRO_MAX)
		break;
	case 6:
		Random_NumC = rand() % 100 + 1;
		if (Random_NumC <= (100 - percentile_probability)) {
			Madongseok_Hold = 1; // 마동석 좀비 '붙들기' 성공
			printf("\nmadongseok pulled zombie... Next turn, it can't move\n");
		}
		else {
			Madongseok_Hold = 0; // 마동석 좀비 '붙들기' 실패
			printf("\nmadongseok failed to pull zombie\n");
		}
		if ((Madongseok_Aggro <= AGGRO_MAX) && (Madongseok_Stamina >= STM_MIN)) {
			printf("\nmadongseok: %d (aggro : %d -> %d, stamina: %d -> %d)\n", PM, Madongseok_Aggro - 2, Madongseok_Aggro, Madongseok_Stamina + 1, Madongseok_Stamina);
		}
		else {
			printf("\nmadongseok: %d (aggro : %d, stamina: %d)\n", PM, Madongseok_Aggro, Madongseok_Stamina);
		}
		break;
	default:
		break;
	}
}

//main 함수
int main(void) {
	srand((unsigned int)time(NULL));

	Position_C = 0;
	Position_Z = 0;
	Position_M = 0;
	GameOver = 0;
	Citizen = 0;
	Zombie = 0;
	Madongseok = 0;
	Madongseok_Hold = 0;
	Citizen_Aggro = 1;
	Madongseok_Aggro = 1;
	Madongseok_Action = -1;
	Turnphase = 0;
	Citizen = PC;
	Zombie = PZ;
	Madongseok = PM;


	intro();
	TrainLength();
	Stamina();
	Percent();

	PM = train_length - 2;
	PZ = train_length - 3;
	PC = train_length - 6;
	Train();

	while (1) {
		GameOver = 0;		// 초기화
		Citizen = PC;		// 초기화
		Zombie = PZ;		// 초기화
		Madongseok = PM;	// 초기화

		// 1턴 <이동> 페이즈 (상태출력-시민이동출력-좀비이동출력-마동석이동입력-상태출력-마동석이동출력)
		C_movement();
		Z_movement();

		Train();
		displayPosition_C();
		displayPosition_Z();
		printf("\n");

		M_movement();
		Train();

		displayPosition_M();
		// 1턴 <이동> 페이즈 (상태출력-시민이동출력-좀비이동출력-마동석이동입력-상태출력-마동석이동출력)

		// 1턴 <행동> 페이즈 (시민-좀비-마동석)
		C_action();
		Z_action();
		displayAction_C();
		if (GameOver == 1) break;
		displayAction_Z();
		if (GameOver == 1) break;
		M_action();
		Madongseok_Hold = 0;  // 초기화
		displayAction_M();
		// 1턴 <행동> 페이즈 (시민-좀비-마동석)

		Turnphase++;
		if (GameOver == 1) break;
	}
}