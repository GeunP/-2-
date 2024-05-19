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
int train_length, percentile_probability, PM, PC, PZ;
int turn = 1;
int Citizen, Zombie;
int Random_NumC, Random_NumZ;	


//��Ʈ��
void intro(void) {
	printf("\n==========GAME START==========\n\n");
	printf("   ���� ���ؼ� �������ÿ�.");
	printf("\n\n==============================\n\n");

	PM = train_length - 2;	//�ʱ�ȭ
	PZ = train_length - 3;	//�ʱ�ȭ
	PC = train_length - 6;	//�ʱ�ȭ
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
		printf("INVALID INPUT");
		scanf_s("%d", &train_length);
	}
}

//Ȯ��
void percent(void) {
	printf("percentile probability (%d ~ %d) >> ", PROB_MIN, PROB_MAX);
	scanf_s("%d", &percentile_probability);
	while (percentile_probability < PROB_MIN || percentile_probability > PROB_MAX) {
		printf("INVALID INPUT");
		scanf_s("%d", &percentile_probability);
	}
}

//��������
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

//�ù�, ���� �̵�
void movement(void) {
	//�ù� �̵�
	Random_NumC = rand() % 100 + 1;
	if (Random_NumC <= (100 - percentile_probability) && PC > 1) {
		PC--;
	}

	//���� �̵�
	if (turn % 2 == 1) {
		Random_NumZ = rand() % 100 + 1;
		if (Random_NumZ <= percentile_probability && PZ > 1) {
			PZ--;
		}
	}

	displayTrain();

}

//�ù�, ���� �̵� ���
void updatePosition(void) {
	//�ù� �̵� ���
	if (Citizen == PC) {
		printf("\ncitizen : stay %d\n", PC);
	}
	else {
		printf("\ncitizen : %d -> %d\n", Citizen, PC);
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


int main(void) {
	srand((unsigned int)time(NULL));
	intro();
	train();
	percent();
	displayTrain();
	while (1) {
		movement();
		updatePosition();
		if (PC == PZ || PC == 1) {
			outro();
			break;
		}
		turn++;
		Sleep(4000);
	}
	return 0;
}