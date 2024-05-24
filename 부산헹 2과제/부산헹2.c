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

void M_movement1(void);
void M_movement2(void);
void M_action1(void);

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
		printf("YOU WIN!\nTHE citizen has escaped to the next train.\n");
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
void Percent(void) {
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
	scanf_s("%d", &Madongseok_Stamina);
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
		if (Random_NumC <= (100 - percentile_probability)) {
			PC--;
			if (Citizen_Aggro < AGGRO_MAX) {
				Citizen_Aggro++;
				Position_C = 1;
			}
			else {
				Position_C = 2;
			}
		}
		else {
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
		printf("\nCitizen : %d -> %d (aggro : %d -> %d)", Citizen, PC, Citizen_Aggro - 1, Citizen_Aggro);
		break;
	case 2:
		printf("\nCitizen : %d - > %d (aggro : %d)", Citizen, PC, Citizen_Aggro);
		break;
	case 3:
		printf("\nCitizen : %d (aggro : %d -> %d)", PC, Citizen_Aggro + 1, Citizen_Aggro);
		break;
	case 4:
		printf("\nCitizen : %d (aggro : %d)", PC, Citizen_Aggro);
		break;
	default:
		break;
	}
}

//좀비 이동s
void Z_movement(void) {
	if (Turnphase % 2 == 1 && Madongseok_Hold == 1) {
		Position_Z = 1;
	}
	else if (Turnphase % 2 == 1 && Madongseok_Hold == 0) {
		Position_Z = 2;
	}
	else if (Turnphase % 2 == 0 && Madongseok_Hold == 0) {
		Random_NumZ = rand() % 100 + 1;
		if (Random_NumZ <= percentile_probability) {
			if ((PM - PZ) > 1) {
				PZ++;
				Position_Z = 2;
			}
			else {
				Position_Z = 4;
			}
		}
		else {
			if ((PZ - PC) > 1) {
				PZ--;
				Position_Z = 3;
			}
			else {
				Position_Z = 4;
			}
		}
	}
}

//Position_Z switch문
void displayPosition_Z(void) {
	switch (Position_Z) {
	case 1:
		printf("\nZombie : stay %d (cannot move)", PZ);
		break;
	case 2:
		printf("\nZombie : %d -> %d", PZ - 1, PZ);
		break;
	case 3:
		printf("\nZombie : %d -> %d", PZ + 1, PZ);
		break;
	case 4:
		printf("\nZombie : stay %d", PZ);
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
	M_movement1();
	M_movement2();
}
//마동석 이동 1
void M_movement1(void) {
	if (move == MOVE_STAY) {
		if (Madongseok_Aggro > AGGRO_MIN) {
			Madongseok_Aggro--;
			Position_M = 1;
		}
		else Position_M = 2;
	}
}
//마동석 이동 2
void M_movement2(void) {
	if (move == MOVE_LEFT) {
		PM--;
		if (Madongseok_Aggro < AGGRO_MAX) {
			Madongseok_Aggro++;
			Position_M = 3;
		}
		else {
			Position_M = 4;
		}
	}
}

//마동석 이동 1, 2 switch문
void displayPosition_M(void) {
	switch (Position_M) {
	case 1:
		printf("\nmadongseok : stay %d (aggro : %d -> %d, stamina : %d)\n", PM, Madongseok_Aggro + 1, Madongseok_Aggro, Madongseok_Stamina);
		break;
	case 2:
		printf("\nmadongseok : stay %d (aggro : %d, stamina : %d)\n", PM, Madongseok_Aggro, Madongseok_Stamina);
		break;
	case 3:
		printf("\nmadongseok : stay %d -> %d (aggro : %d, stamina : %d)\n", PM + 1, PM, Madongseok_Aggro, Madongseok_Stamina);
		break;
	case 4:
		printf("\nmadongseok : stay %d -> %d (aggro :%d -> %d, stamina : %d)\n", PM + 1, PM, Madongseok_Aggro - 1, Madongseok_Aggro, Madongseok_Stamina);
		break;
	default:
		break;
	}
}

//시민 행동
void C_action(void) {
	if (PC == 1) {
		GameOver = 1;
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
		printf("\nCitizen does nothing");
		break;
	default:
		break;
	}
}

//좀비 행동
void Z_action(void){
	if (((PZ - PC) > 1) && ((PM - PZ) > 1)) {
		Action_Z = ATK_CITIZEN;
	}
	else if ((PZ - PC) != 1 && (PM - PZ) != 1) {
		Action_Z = ATK_DONGSEOK;
	}
	else if ((PZ - PC) == 1 && (PM - PC) == 1) {
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

//좀비 행동 switch문
void displayAction_Z(void) {
	switch (Action_Z) {
	case ATK_NONE:
		printf("\nzombie attack nobody.\n");
		break;
	case ATK_CITIZEN:
		printf("GAME OVER! citizen dead.....\n");
		break;
	case ATK_DONGSEOK:
		Madongseok_Stamina++;
		if (Madongseok_Stamina == STM_MIN) {
			printf("GAME OVER! madongseok dead.....\n");
			GameOver = 1;
		}
		printf("\nZombie attacked madongsoke (aggro : %d vs %d, madongsoek stamina : %d -> %d", Citizen_Aggro, Madongseok_Aggro, Madongseok_Stamina + 1, Madongseok_Stamina);
	default:
		break;
	}
}

//마동석 행동
void M_action(void) {
	if ((PM - PZ) != 1) {
		while (Madongseok_Action != 0 && Madongseok_Action != 1) {
			printf("\nmadongseok action (%d : rest, %d : provoke) >> ", ACTION_REST, ACTION_PROVOKE);
			scanf_s("%d", &Madongseok_Action);
		}
	}
	else {
		while (Madongseok_Action < 0 && Madongseok_Action > 2) {
			printf("\nmadongseok action (%d :rest, %d : provoke, %d : pull) >> ", ACTION_REST, ACTION_PROVOKE, ACTION_PULL);
		}
	}

	M_action1();
}

//마동석 행동 1
void M_action1(void) {
	Madongseok_Action = -1;
	if (Madongseok_Action == ACTION_REST) {
		Madongseok_Aggro--;
		Madongseok_Stamina++;
		Action_M = ACTION_REST;
	}
	else if (Madongseok_Action == ACTION_PROVOKE) {
		Action_M = ACTION_PROVOKE;
	}
	else if (Madongseok_Action == ACTION_PULL) {
		Madongseok_Aggro += 2;
		if (Madongseok_Action > STM_MIN) {
			Madongseok_Stamina--;
		}
	Action_M = ACTION_PULL;
	}
}

//마동석 행동 swtich문
void displayAction_M(void) {
	switch (Action_M) {
	case ACTION_REST:
		printf("\nmadongseok rest.....\n");
		if (Madongseok_Stamina == STM_MIN) {
			printf("\nmadongseok : %d (aggro : %d -> %d, stamina : %d)\n", PM, Madongseok_Aggro + 1, Madongseok_Aggro, Madongseok_Stamina);
		}
		else {
			printf("\nmadongseok : %d (aggro : %d -> %d, stamina : %d -> %d)\n", PM, Madongseok_Aggro + 1, Madongseok_Aggro, Madongseok_Stamina - 1, Madongseok_Stamina);
		}
		break;
	case ACTION_PROVOKE:
		printf("\nmadongseok : %d (aggro : %d -> %d, stamina : %d)\n", PM, Madongseok_Aggro, AGGRO_MAX, Madongseok_Stamina);
		Madongseok_Aggro = AGGRO_MAX;
		break;
	case ACTION_PULL:
		Random_NumC = rand() % 100 + 1;
		if (Random_NumC <= (100 - percentile_probability)) {
			Madongseok_Hold = 1;
			printf("\nmadongseok pulled zombie... Next turn, it can't move\n");
		}
		else {
			Madongseok_Hold = 0; 
			printf("\nmadongseok failed to pull zombie\n");
		}
		printf("\nmadongseok: %d (aggro : %d -> %d, stamina: %d -> %d)\n", PM, Madongseok_Aggro - 2, Madongseok_Aggro, Madongseok_Stamina + 1, Madongseok_Stamina);
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
	Turnphase = 1;          


	intro();                
	train();           
	Stamina();         
	Percent();         

	PM = train_length - 2;  
	PZ = train_length - 3;  
	PC = train_length - 6;  
	Train();         

	while (1) {
		Citizen = PC;
		Zombie = PZ;
		Madongseok = PM;

		C_movement();           
		Z_movement();           

		Train();           
		displayPosition_Z();    
		printf("\n");
		M_movement();           
		Train();         

		displayPosition_M();    
		
		C_action();         
		Z_action();         
		displayAction_C();  
		if (GameOver == 1) break;
		displayAction_Z();  
		M_action();         
		displayAction_M();    
		
		Turnphase++;
		if (GameOver == 1) break;
	}
}