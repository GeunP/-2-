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
int train_length, percentile_probability, madongseok_stamina, PM, PC, PZ;
int turn = 1;
int Citizen, Zombie, Madongseok;
int Random_NumC, Random_NumZ;
int move, rest;
int Citizen_Aggro, Madongseok_Aggro;
int action_Madongseok;



//��Ʈ��
void intro(void) {
	printf("\n==========GAME START==========\n\n");
	printf("   ���� ���ؼ� �������ÿ�.");
	printf("\n\n==============================\n\n");


}

//�ƿ�Ʈ��
void outro(void) {
	if (PC == PZ - 1) {
		printf("GAME OVER!\nTHE citizen has been caught by the zombie.\n");
	}


	if (PC == 1) {
		printf("SUCCESS!\nTHE citizen has escaped to the next train.\n");
	}
}

//���� ����
void train(void) {
	printf("train length (%d ~ %d) >> ", LEN_MIN, LEN_MAX);
	scanf_s("%d", &train_length);
	while (train_length < LEN_MIN || train_length > LEN_MAX) {
		printf("train length (%d ~ %d) >> ", LEN_MIN, LEN_MAX);
		scanf_s("%d", &train_length);
	}
}

//Ȯ��
void percent(void) {
	printf("percentile probability 'p'(%d ~ %d) >> ", PROB_MIN, PROB_MAX);
	scanf_s("%d", &percentile_probability);
	while (percentile_probability < PROB_MIN || percentile_probability > PROB_MAX) {
		printf("percentile probability 'p'(%d ~ %d) >> ", PROB_MIN, PROB_MAX);
		scanf_s("%d", &percentile_probability);
	}
}

//������ ü��
void Stamina(void) {
	printf("madongseok stamina(%d ~ %d) >> ", STM_MIN, STM_MAX);
	scanf_s("%d", &madongseok_stamina);
	while (madongseok_stamina < STM_MIN || madongseok_stamina > STM_MAX) {
		printf("madongseok stamina (%d ~ %d) >> ", STM_MIN, STM_MAX);
		scanf_s("%d", &madongseok_stamina);
	}
}

//������ ����
void Condition(void) {
	printf("madongseok move (0:stay, 1:left) >> ");
	scanf_s("%d", &move);
	while (move != 1 && move != 0) {
		printf("madongseok move (0:stay, 1:left) >> ");
		scanf_s("%d", &move);
	}
}

//���� �ʱ� ����
void displayTrain(void) {
	PM = train_length - 2;	//�ʱ�ȭ
	PZ = train_length - 3;	//�ʱ�ȭ
	PC = train_length - 6;	//�ʱ�ȭ

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

//���� ����
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
	//�ù� �̵�
	Random_NumC = rand() % 100 + 1;
	if (Random_NumC <= (100 - percentile_probability) && PC > 1) {
		PC--;
		if (Citizen_Aggro < AGGRO_MAX) {
			Citizen_Aggro++;
		}
	}
}
void Z_movement(void) {
		//���� �̵�
	if (turn % 2 == 1) {
		Random_NumZ = rand() % 100 + 1;
		if (Random_NumZ <= percentile_probability && PZ > 1) {
			PZ--;
		}
	}
}
void M_movement(void) {
	//������ �̵�
	if (move == 1) {
		PM--;
	}
}
	
//�̵�
void movement(void) {
	C_movement();
	Z_movement();
	M_movement();
	basicTrain();
}

//�ù�, ���� �̵� ���
void updatePosition(void) {
	//�ù� �̵� ���
	if (Citizen == PC) {
		printf("\ncitizen : stay %d (aggro : %d)\n", PC, Citizen_Aggro);
	}
	else {
		printf("\ncitizen : %d -> %d (aggro : %d)\n", Citizen, PC, Citizen_Aggro);
	}

	//���� �̵� ���
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

//������ ���
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
	intro(); // ����
	train(); // ���� ����
	Stamina(); // ������ ü��
	percent(); // Ȯ��
	displayTrain(); //ó�� ����
	turn = 1;
	Citizen = 0;
	Zombie = 0;
	Madongseok = 0;
	while (1) {
		Citizen = PC;
		Zombie = PZ;
		Madongseok = PM;
		movement(); // �ù�, ���� �̵�
		updatePosition(); // �ù�, ���� �̵� ���
		Condition(); // ������ ����
		movement(); // ���� �ٽ� ���
		printf("madongseok : stay %d (aggro : %d, stamina : %d)\n", PM, AGGRO_MIN, madongseok_stamina);
		printf("Citizen does nothing.\n");
		printf("zombie attaked nobody.\n");
		Rest(); // ������ ���
		if (PC == PZ - 1 || PC == 1) {
			outro(); // ��
			break;
		}
		turn++;
		Sleep(4000);
	}
}