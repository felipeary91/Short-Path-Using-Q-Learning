#include <stdio.h>
#include <stdlib.h>
#define RAN_LIM 500000	//Size of random samples

//Global variables
double qMatrix[10][10], rMatrix[10][10], gammaLR = 0.8;	//declaring Q-Matrix, R-Matrix, and Learning Rate
int max_index[10], available_acts[10];
int ran_top=0, ran[RAN_LIM];

//Helper functions

//Returning a random number from list
int getRandom() {
	return ran[ran_top++];
}


//Returns the size of available actions based on the columns with
//values >= 0.0. Also, updates available_acts with those nodes available
//as a next step.
int available_actions(int state, int available_acts[], double rMatrix[][10]) {
	int k = 0, j = 0;
	while (j < 10) {
		if (rMatrix[state][j] >= 0.0) {
			available_acts[k] = j;
			k++;
		}
		j++;
	}
	printf("\n");
	return k;
}


//Selects a random index (with a value maximum of 'k' returned by
//available_actions) from the available actions (available_acts).
int sample_next_action (int size, int available_acts[]) {
	int a = getRandom();
	int next_action = available_acts[a % size];
	return next_action;
}


//Based on the value returned by available_actions, non-zero values from
//Q-Matrix are stored in max_index, from this a random index is obtained
//and the max value is gotten from Q-Matrix.
double update(int current_state, int action, double rMatrix[][10], double qMatrix[][10]) {
	int i = 0, j = 0, k = 0, index_of_max;
	double temp_max = 0.0, max_value = 0.0, sumA = 0.0;

	//Finding the best next node
	for (i = 0; i < 10; i++) {
		max_index[i] = 0;	//Setting max_index to 0.0 for temp_max > qMatrix[action][i]
		
		if (temp_max == qMatrix[action][i]) {
			max_index[j] = i;
			j++;
		}

		else if (temp_max < qMatrix[action][i]) {
			j = 0;
			temp_max = qMatrix[action][i];
			max_index[j] = i;
			j++;
		}
	}

	//Select a random index from max_index
	int a = getRandom() % j;
	index_of_max = max_index[a];
	max_value = qMatrix[action][index_of_max];

	//Main update
	qMatrix[current_state][action] = rMatrix[current_state][action] + (gammaLR * max_value);
	temp_max = 0.0;
	
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			if (qMatrix[i][j] > temp_max) {
				temp_max = qMatrix[i][j];
			}
		}
	}

	if (temp_max > 0) {
		for (i = 0; i < 10; i++) {
			for (j = 0; j < 10; j++) {
				sumA = sumA + (qMatrix[i][j] / temp_max);
			}
		}
		sumA = sumA * 100;
		return sumA;
	}
	else {
		return 0.0;
	}
}



int main() {
	//Variable initiation
	int i, j;
	int initial_state, final_state;	//Initial and final node
	int current_state, size_av_actions, action;
	double final_max = 0.0, scores[100000], score = 0.0;

	//Asking user input for initial state
	printf("Enter initial state (node): ");
	scanf_s("%d", &initial_state);

	//Asking user input for final state
	printf("Enter final state (node): ");
	scanf_s("%d", &final_state);

	//Filling random vector with random numbers from 0 to 7
	for (i = 0; i < RAN_LIM; i++) {
		ran[i] = rand() % 10;
	}

	//Filling Q-Matrix and R-MAtrix
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {

			rMatrix[i][j] = -1.0;
			qMatrix[i][j] = 0.0;	//Q-Matrix starts with all values in 0.0

			//Defining the nodes direct connection in the direction i -> j (state -> action)
			if ((i == 0 && j == 1) || (i == 1 && j == 5) || (i == 5 && j == 6) || (i == 5 && j == 4)
				|| (i == 1 && j == 2) || (i == 2 && j == 3) || (i == 2 && j == 7) || (i == 4 && j == 7) || (i == 1 && j == 4)
				|| (i == 6 && j == 8) || (i == 5 && j == 8) || (i == 7 && j == 9) || (i == 3 && j == 9)) {
				rMatrix[i][j] = 0.0;
			}

			//Defining the nodes direct connection in the direction j -> i (action -> state)
			if ((j == 0 && i == 1) || (j == 1 && i == 5) || (j == 5 && i == 6) || (j == 5 && i == 4) || (j == 1 && i == 2)
				|| (j == 2 && i == 3) || (j == 2 && i == 7) || (j == 4 && i == 7) || (j == 1 && i == 4)
				|| (j == 6 && i == 8) || (j == 5 && i == 8) || (j == 7 && i == 9) || (j == 3 && i == 9)) {
				rMatrix[i][j] = 0.0;
			}

			//Checking the nodes that are directly connected to the destination
			//node, and assigning them a value of 100.0
			if (rMatrix[i][final_state] == 0.0) {
				rMatrix[i][final_state] = 100.0;
			}

		}
	}

	//Printing R-Matrix
	printf("R-Matrix: \n");
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			printf("%f\t", rMatrix[i][j]);
		}
		printf("\n");
	}
	printf("\n\n\n");
	printf("%f", rMatrix[9][9]);

	
	//Training the Q-Matrix for 500 iterations
	for (i = 0; i < 500; i++) {
		current_state = getRandom();
		size_av_actions = available_actions(current_state, available_acts, rMatrix);
		action = sample_next_action(size_av_actions, available_acts);
		score = update(current_state, action, rMatrix, qMatrix);
		scores[i] = score;
		printf("\nScore: %f", score);
	}

	//Finding max
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (final_max < qMatrix[i][j]) {
				final_max = qMatrix[i][j];
			}
		}
	}

	printf("\n\nTrained Q-Matrix: \n");
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			printf("%f\t", (qMatrix[i][j] / (final_max * 100.0)));
		}
		printf("\n");
	}

	int curr_state = initial_state;
	int visited[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int no_way = 0;
	int row_max = 0, max_ind = 0;

	printf("Path: \n");
	while (visited[final_state] != 1) {

		printf("%d-> ", curr_state);
		row_max = 0;
		max_ind = 0;

		for (i = 0; i < 10; i++) {
			if (visited[i] == 0) {
				if (qMatrix[curr_state][i] > row_max) {
					max_ind = i;
					row_max = qMatrix[curr_state][i];
				}
			}
		}
		curr_state = max_ind;
		visited[max_ind] = 1;
		
		if (row_max == 0) {
			no_way = 1;
			break;
		}

		if (curr_state == final_state) {
			break;
		}
	}

	if (no_way == 1) {
		printf("%d | There's no way after this\n");
	}
	else {
		printf("%d is the shortest path\n", curr_state);
	}
}