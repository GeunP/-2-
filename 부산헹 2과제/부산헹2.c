#include <stdio.h>
#include <time.h>
#include <windows.h>

#define LEN_MIN 15     //���� ����
#define LEN_MAX 50
#define STM_MIN 0      //������ ü��  
#define STM_MAX 5
#define PROB_MIN 10    //Ȯ��
#define PROB_MAX 90
#define AGGRO_MIN 0    //��׷� ����
#define AGGRO_MAX 5

//������ �̵� ����
#define MOVE_LEFT 1
#define MOVE_STAY 0

//������ ���� ���
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

//������ �ൿ
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2


//���� ����
int train_length, percentile_probability, PM, PC, PZ;
int TurnPhase;
int CITIZEN, ZOMBIE, MADONGSEOK;
int Random_NumC;
int move, rest;
int Citizen_Aggro, Madongseok_Aggro;						//�ù�, ������ ��׷�
int Position_M, Position_C, Position_Z;						//������, �ù�, ���� ��ġ
int Action_M, Action_C, Action_Z;							//������, �ù�, ���� �ൿ
int Madongseok_Stamina, Madongseok_Action, Madongseok_Hold;	//������ �ΰ����� ���
int GameOver;

//�Լ�  ����
void Movement_Z1(void);
void Movement_M1(void);

//��Ʈ��
void intro(void) {
	printf("\n==========GAME START==========\n\n");
	printf("   ���� ���ؼ� �������ÿ�.");
	printf("\n\n==============================\n\n");
}

//�ƿ�Ʈ��
void outro(void) {
	if (PC == PZ - 1) {
		printf("\nGAME OVER!\nTHE CITIZEN HAS BEEN CAUGHT BY THE ZOMBIE.\n");
	}
	if (PC == 1) {
		printf("\nYOU WIN!\nTHE CITIZEN HAS ESCAPED TO THE NEST TRAIN.\n");
	}
}

//���� ����
void TrainLength(void) {
	printf("train length (%d ~ %d) >> ", LEN_MIN, LEN_MAX);
	scanf_s("%d", &train_length);
	while (train_length < LEN_MIN || train_length > LEN_MAX) {
		printf("train length (%d ~ %d) >> ", LEN_MIN, LEN_MAX);
		scanf_s("%d", &train_length);
	}
}

//������ ü��
void Stamina(void) {
	printf("madongseok stamina(%d ~ %d) >> ", STM_MIN, STM_MAX);
	scanf_s("%d", &Madongseok_Stamina);
	while (Madongseok_Stamina < STM_MIN || Madongseok_Stamina > STM_MAX) {
		printf("madongseok stamina (%d ~ %d) >> ", STM_MIN, STM_MAX);
		scanf_s("%d", &Madongseok_Stamina);
	}
}

//Ȯ��
void Percent(void) {
	printf("percentile probability 'p'(%d ~ %d) >> ", PROB_MIN, PROB_MAX);
	scanf_s("%d", &percentile_probability);
	while (percentile_probability < PROB_MIN || percentile_probability > PROB_MAX) {
		printf("percentile probability 'p'(%d ~ %d) >> ", PROB_MIN, PROB_MAX);
		scanf_s("%d", &percentile_probability);
	}
}

//���� ����
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

//�ù� �̵�
void Movement_C(void) {
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
}

//�ù� �̵� switch��
void displayPostion_C(void) {
	switch (Position_C) {
	case 1:
		printf("\nCitizen : %d -> %d (aggro : %d -> %d)\n", PC + 1, PC, Citizen_Aggro - 1, Citizen_Aggro); 
		break;
	case 2:
		printf("\nCitizen : %d - > %d (aggro : %d)\n", PC + 1, PC, Citizen_Aggro); 
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

//���� �̵�
void Movement_Z(void) {
	if (TurnPhase % 2 == 1 || Madongseok_Hold == 1) { 
		Position_Z = 1;
		Madongseok_Hold = 0;
	}
	else if (TurnPhase % 2 == 0 && Madongseok_Hold == 0) { 
		Movement_Z1();
	}
}

//���� �̵�+
void Movement_Z1(void) {
	if (Citizen_Aggro < Madongseok_Aggro) {
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

//���� �̵� switch��
void displayPosition_Z(void) {
	switch (Position_Z) {
	case 1:
		printf("\nZombie : stay %d (cannot move)\n", PZ); 
		break;
	case 2:
		printf("\nZombie : %d -> %d\n", PZ - 1, PZ); 
		break;
	case 3:
		printf("\nZombie : %d -> %d\n", PZ + 1, PZ); 
		break;
	case 4:
		printf("\nZombie : stay %d\n", PZ);
		break;
	default:
		break;
	}
}

//������ �̵�
void Movement_M(void) {
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
}

//������ �̵�+
void Movement_M1(void){
	if (move == MOVE_STAY) { 
		if (Madongseok_Aggro > AGGRO_MIN) {
			Madongseok_Aggro--;
			Position_M = 1;
		}
		else Position_M = 2;
	}
	if (move == MOVE_LEFT) { 
		PM--;
		if (Madongseok_Aggro < AGGRO_MAX) {
			Madongseok_Aggro++;
			Position_M = 3;
		}
		else Position_M = 4;
	}
}

//������ �̵� switch��
void displayPostion_M(void) {
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
}
