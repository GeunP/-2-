#include<stdio.h>
#include<time.h>
#include<windows.h>

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
int trian_length, percentile_probability, PM, PC, PZ;
int TurnPhase;
int CITIZEN, ZOMBIE, MADONGSEOK;
int Random_NumC;
int move, rest;
int Citizen_Aggro, Madongseok_Aggro;
int Postion_M, Postion_C, Position_Z;
int Action_M, Action_C, Action_Z;
int GameOver;

//��Ʈ��
void intro(void) {
	printf("\n==========GAME START==========\n\n");
	printf("   ���� ���ؼ� �������ÿ�.");
	printf("\n\n==============================\n\n");
}

//�ƿ�Ʈ��
void outro(void) {
	if (PC == PZ - 1) {
		printf("\nGAME OVER!\nTHE citizen has been caught by the zombie.\n");
	}
	if (PC == 1) {
		printf("\nYOU WIN!\nTHE citizen has escaped to the next train.\n");
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


