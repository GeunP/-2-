// Coded by : �̱�ǥ
// Date     : 24/05/28

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
#define MOVE_STAY 0
#define MOVE_LEFT 1

//������ ���� ���
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

//������ �ൿ
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

// ���� �ʱ�ȭ�� ���� ���ǰ�
#define initialValue 10000

//���� ����
int train_length, percentile_probability, PM, PC, PZ;
int Turnphase;
int Citizen, Zombie, Madongseok;
int Random_NumC, Random_NumZ;
int move, rest;
int Citizen_Aggro;  // Aggro ���� �߰�
int Madongseok_Stamina, Madongseok_Aggro, Madongseok_Action, Madongseok_Hold;  // ������ ����
int Position_C, Position_Z, Position_M;     // Citizen, Zombie and Madongseok's Position Value
int Action_C, Action_Z, Action_M;            // Citizen, Zombie and Madongseok's Action Value
int GameOver;                               // Game Over Fag, '0':ing, '1':end


//��Ʈ��
void intro(void) {
	printf("\n==========GAME START==========\n\n");
	printf("   ���� ���ؼ� �������ÿ�.");
	printf("\n\n==============================\n\n");
}

//�ƿ�Ʈ��
void outro(void) {
	if (PC == PZ - 1) {
		printf("\nGAME OVER!\n THE citizen has been caught by the zombie.\n");
	}
	if (PC == 1) {
		printf("\nYOU WIN!\n THE citizen has escaped to the next train.\n");
	}
}

//���� ����
void TrainLength(void) {
	train_length = initialValue; // ���ǰ� �ʱ�ȭ
	while (train_length < LEN_MIN || train_length > LEN_MAX) {
		printf("train length (%d ~ %d) >> ", LEN_MIN, LEN_MAX);
		scanf_s("%d", &train_length);
	}
}

//Ȯ�� 
void Percent(void) {
	percentile_probability = initialValue; // ���ǰ� �ʱ�ȭ
	while (percentile_probability < PROB_MIN || percentile_probability > PROB_MAX) {
		printf("percentile probability 'p'(%d ~ %d) >> ", PROB_MIN, PROB_MAX);
		scanf_s("%d", &percentile_probability);
	}

}

//������ ü��
void Stamina(void) {
	Madongseok_Stamina = initialValue; // ���ǰ� �ʱ�ȭ
	while (Madongseok_Stamina < STM_MIN || Madongseok_Stamina > STM_MAX) {
		printf("madongseok stamina (%d ~ %d) >> ", STM_MIN, STM_MAX);
		scanf_s("%d", &Madongseok_Stamina);
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
void C_movement(void) {
	if (PC > 1) {
		Random_NumC = rand() % 100 + 1;
		if (Random_NumC <= (100 - percentile_probability)) { // �ù� �·� 1 �̵�
			PC--;
			if (Citizen_Aggro < AGGRO_MAX) {
				Citizen_Aggro++;
				Position_C = 1;
			}
			else {
				Position_C = 2;
			}
		}
		else { // �ù� ���ڸ�
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

//Position_C�� switch��
void displayPosition_C(void) {
	switch (Position_C) {
	case 1:
		printf("\nCitizen : %d -> %d (aggro : %d -> %d)\n", Citizen, PC, Citizen_Aggro - 1, Citizen_Aggro); // Citizen�� PC���� �ٲ�� �� ���� ����
		break;
	case 2:
		printf("\nCitizen : %d - > %d (aggro : %d)\n", Citizen, PC, Citizen_Aggro); // Citizen�� PC���� �ٲ�� �� ���� ����
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
void Z_movement(void) {
	if (Turnphase % 2 == 1 || Madongseok_Hold == 1) { // ������ '�ٵ��' ����
		Position_Z = 1;
		Madongseok_Hold = 0;
	}
	else if (Turnphase % 2 == 0 && Madongseok_Hold == 0) { // 2�ϸ��� �̵� ����, ������ '�ٵ��' ����
		if (Citizen_Aggro < Madongseok_Aggro) {
			if ((PM - PZ) > 1) {
				PZ++; // ������ ��׷ΰ� ������, ������ ������ �̵�
				Position_Z = 2;
			}
			else {
				Position_Z = 4; // �̵����� ����
			}
		}
		else {
			if ((PZ - PC) > 1) {
				PZ--; // �ù� ��׷ΰ� ���ų� ������, �ù� ������ �̵�
				Position_Z = 3;
			}
			else {
				Position_Z = 4; // �̵����� ����
			}
		}
	}
}

//Position_Z switch��
void displayPosition_Z(void) {
	switch (Position_Z) {
	case 1:
		printf("\nZombie : stay %d (cannot move)\n", PZ); // �̵��� �� ����
		break;
	case 2:
		printf("\nZombie : %d -> %d\n", Zombie, PZ); // ����, ������ ������ �̵�
		break;
	case 3:
		printf("\nZombie : %d -> %d\n", Zombie, PZ); // ����, �ù� ������ �̵�
		break;
	case 4:
		printf("\nZombie : stay %d\n", PZ); // �̵����� ����
		break;
	default:
		break;
	}
}

//������ �̵�
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
	if (move == MOVE_STAY) { // ���ڸ� ���
		if (Madongseok_Aggro > AGGRO_MIN) {
			Madongseok_Aggro--; // ������ ��׷� ����
			Position_M = 1;
		}
		else Position_M = 2;
	}
	if (move == MOVE_LEFT) { // �������� �̵�
		PM--;
		if (Madongseok_Aggro < AGGRO_MAX) {
			Madongseok_Aggro++; // ������ ��׷� ����
			Position_M = 3;
		}
		else Position_M = 4;
	}
}

//������ �̵� switch��
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

//�ù� �ൿ
void C_action(void) {
	if (PC == 1) {
		GameOver = 1; // Game Over
		Action_C = 1;
	}
	else {
		Action_C = 2;
	}
}

//�ù� �ൿ switch��
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

//���� �ൿ
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

//���� �ൿ switch��
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

//������ �ൿ
void M_action(void) {
	Madongseok_Action = initialValue; // ���ǰ� �ʱ�ȭ
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
			if (Madongseok_Stamina < STM_MAX) { // ������ ü���� ü�� �ְ� ���� ���� ��
				Madongseok_Aggro--;		// ������ ��׷� 1 ����
				Madongseok_Stamina++;	// ������ ü�� 1 ����
				Action_M = 1;
			}
			else { // ������ ü���� ü�� �ְ� ���� ���ų� Ŭ ��
				Madongseok_Aggro--;		// ������ ��׷� 1 ����
				Action_M = 2;
			}
		}
		else if (Madongseok_Stamina == AGGRO_MIN && Madongseok_Stamina < STM_MAX) {
			Madongseok_Stamina++;	// ������ ü�� 1 ����
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
		if (Madongseok_Aggro + 1 <= AGGRO_MAX) {  // ������ ��׷ΰ� ��׷� �ְ� ���� 1 ���� ��
			Madongseok_Aggro = Madongseok_Aggro + 1;
		}
		else {
			Madongseok_Aggro = Madongseok_Aggro + 2;  // ������ ��׷� 2 ����, ������ ��׷ΰ� ��׷� �ְ� ���� 2 �̻� ���� ��
		}
		Madongseok_Stamina--;						// ������ ü�� 1 ����
		Action_M = 6;
	}
}

//������ �ൿ swtich��
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
		Madongseok_Aggro = AGGRO_MAX; // ��׷� �ִ�� ���� (AGGRO_MAX)
		break;
	case 6:
		Random_NumC = rand() % 100 + 1;
		if (Random_NumC <= (100 - percentile_probability)) {
			Madongseok_Hold = 1; // ������ ���� '�ٵ��' ����
			printf("\nmadongseok pulled zombie... Next turn, it can't move\n");
		}
		else {
			Madongseok_Hold = 0; // ������ ���� '�ٵ��' ����
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

//main �Լ�
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
		GameOver = 0;		// �ʱ�ȭ
		Citizen = PC;		// �ʱ�ȭ
		Zombie = PZ;		// �ʱ�ȭ
		Madongseok = PM;	// �ʱ�ȭ

		// 1�� <�̵�> ������ (�������-�ù��̵����-�����̵����-�������̵��Է�-�������-�������̵����)
		C_movement();
		Z_movement();

		Train();
		displayPosition_C();
		displayPosition_Z();
		printf("\n");

		M_movement();
		Train();

		displayPosition_M();
		// 1�� <�̵�> ������ (�������-�ù��̵����-�����̵����-�������̵��Է�-�������-�������̵����)

		// 1�� <�ൿ> ������ (�ù�-����-������)
		C_action();
		Z_action();
		displayAction_C();
		if (GameOver == 1) break;
		displayAction_Z();
		if (GameOver == 1) break;
		M_action();
		Madongseok_Hold = 0;  // �ʱ�ȭ
		displayAction_M();
		// 1�� <�ൿ> ������ (�ù�-����-������)

		Turnphase++;
		if (GameOver == 1) break;
	}
}