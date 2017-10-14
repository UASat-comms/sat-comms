#include <stdio.h>
#include <wiringPi.h>

#define IN1_UD 21
#define IN2_UD 22
#define IN3_UD 23
#define IN4_UD 25

#define IN1_LR 26
#define IN2_LR 27
#define IN3_LR 28
#define IN4_LR 29

void setStepUD(int w1, int w2, int w3, int w4){
        digitalWrite(IN1_UD, w1);
        digitalWrite(IN2_UD, w2);
        digitalWrite(IN3_UD, w3);
        digitalWrite(IN4_UD, w4);
}

void setStepLR(int w1, int w2, int w3, int w4){
        digitalWrite(IN1_LR, w1);
        digitalWrite(IN2_LR, w2);
        digitalWrite(IN3_LR, w3);
        digitalWrite(IN4_LR, w4);
}

int drawMenu(){
	int choice;
	// print the formatted menu
	printf("\n\n\n");
	printf("|==================================|\n");
	printf("|WELCOME TO CUBESAT CONTROL CONSOLE|\n");
	printf("|==================================|\n");
	printf("| 1: Change beam direction         |\n");
	printf("| 2: Test RF connectivity          |\n");
	printf("| 3: Send a file over laser        |\n");
	printf("| 4: Quit                          |\n");
	printf("|==================================|\n\n");
	printf("Please make a selection: ");
	// make sure we get an appropriate value for the user choice
	while(1){
		scanf("%d", &choice);
		if (choice > 4 || choice < 1){
			char c;
			// flush stdin so char inputs dont get us stuck
			while ((c = getchar() != '\n' && c != EOF)) {}
			printf("Invalid choice; Choose again: ");
		} else break;
	}
	return choice;
}

void motorStepVert(int dir, int steps){
	int i = 0;
	switch (dir){
		case 0:
			for (i = 0; i < steps; i++){
                		setStepUD(1,0,0,0);
                		delay(10);
                		setStepUD(1,1,0,0);
                		delay(10);
                		setStepUD(0,1,0,0);
                		delay(10);
                		setStepUD(0,1,1,0);
                		delay(10);
				setStepUD(0,0,1,0);
				delay(10);
				setStepUD(0,0,1,1);
				delay(10);
				setStepUD(0,0,0,1);
				delay(10);
				setStepUD(1,0,0,1);
				delay(10);
        		}break;
		case 1:
			for (i = 0; i < steps; i++){
				setStepUD(0,0,0,1);
                		delay(10);
                		setStepUD(0,0,1,1);
                		delay(10);
                		setStepUD(0,0,1,0);
                		delay(10);
                		setStepUD(0,1,1,0);
                		delay(10);
				setStepUD(0,1,0,0);
				delay(10);
				setStepUD(1,1,0,0);
				delay(10);
				setStepUD(1,0,0,0);
				delay(10);
				setStepUD(1,0,0,1);
				delay(10);
			}break;
	}
}

void motorStepHoriz(int dir, int steps){
	int i = 0;
	switch (dir){
		case 0:
			for (i = 0; i < steps; i++){
                		setStepLR(1,0,0,0);
                		delay(10);
                		setStepLR(1,1,0,0);
                		delay(10);
                		setStepLR(0,1,0,0);
                		delay(10);
                		setStepLR(0,1,1,0);
                		delay(10);
				setStepLR(0,0,1,0);
				delay(10);
				setStepLR(0,0,1,1);
				delay(10);
				setStepLR(0,0,0,1);
				delay(10);
				setStepLR(1,0,0,1);
                		delay(10);
        		}break;
		case 1:
			for (i = 0; i < steps; i++){
				setStepLR(0,0,0,1);
                		delay(10);
                		setStepLR(0,0,1,1);
                		delay(10);
                		setStepLR(0,0,1,0);
                		delay(10);
                		setStepLR(0,1,1,0);
                		delay(10);
				setStepLR(0,1,0,0);
				delay(10);
				setStepLR(1,1,0,0);
				delay(10);
				setStepLR(1,0,0,0);
				delay(10);
				setStepLR(1,0,0,1);
                		delay(10);
			}break;
	}
}
	
void motorControl(){
	int choice, steps;
	printf("Please choose a direction to move:\n\t1: UP\n\t2: DOWN\n\t3: LEFT\n\t4: RIGHT\n");
	printf("Your choice: ");
	scanf("%d", &choice);
	printf("Please enter a number of steps to move: ");
	scanf("%d", &steps);
	switch(choice){
		case 1: motorStepVert(0,steps); break;
		case 2: motorStepVert(1,steps); break;
		case 3: motorStepHoriz(0,steps); break;
		case 4: motorStepHoriz(1,steps); break;
	}
}

void motorTest(){
	int i;
	for (i = 0; i < 1; i++){
		motorStepVert(0, 3000);
		motorStepVert(1, 3000);
	}
	//for (i = 0; i < 1; i++){
	//	motorStepHoriz(0, 3000);
	//	motorStepHoriz(1, 3000);
	//}
}

void sendLaser(){
	char filename[80];
	char command[90];
	printf("Enter filename to send: ");
	scanf("%s",filename);
	sprintf(command, "./st %s", filename);
	system(command);
}

int main(){
	if (wiringPiSetup() < 0){
                fprintf(stderr, "setup failed\n");
                return 1;
        }

        pinMode(IN1_UD, OUTPUT);
        pinMode(IN3_UD, OUTPUT);
        pinMode(IN2_UD, OUTPUT);
        pinMode(IN4_UD, OUTPUT);

	pinMode(IN1_LR, OUTPUT);
	pinMode(IN3_LR, OUTPUT);
	pinMode(IN2_LR, OUTPUT);
	pinMode(IN4_LR, OUTPUT);

	int choice = 0;
	while(choice != 4){
		choice = drawMenu();
		switch(choice){
			case 1: motorControl(); break;
			//case 2: radioTest(); break;
			case 3: sendLaser(); break;
			case 4: printf("Shutting down\n"); break;
		}
	}
}

