#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <termios.h>
#include <sys/time.h>
#include <unistd.h>

/* game over flag & board state */
int game_over, b[4][4];
FILE *fp;

void init(){
	int i, j;
	game_over = 0;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
			b[i][j] = 0;
}

void rotate(int b[4][4]){
	/* user code */ 
	int tmp[4][4];
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			tmp[j][3-i] = b[i][j];
		}
	}
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			b[i][j] = tmp[i][j];
		}
	}
}

int set_board(int dir, int b[4][4]){

	/* user code init */
	for(int i=0; i<dir; i++){
		rotate(b);
	}
	int ismoved = 0;
	for(int i=0; i<4; i++){
		int ptr = 3;
		while(ptr > 0){
			if(b[i][ptr] == b[i][ptr-1]){
				b[i][ptr] *= 2;
				b[i][ptr-1] = 0;
				ismoved = 1;
			}
			ptr--;
		}
	}
	for(int i=0; i<4; i++){
		int ptr = 3;
		while(ptr > 0){
			if(b[i][ptr] == 0 && b[i][ptr-1] != 0){
				b[i][ptr] = b[i][ptr-1];
				b[i][ptr-1] = 0;
				ismoved = 1;
			}
			ptr--;
		}
	}
	for(int i=0; i<4-dir; i++){
		rotate(b);
	}
	if(ismoved) return 1; //valid move
	else return 0; // unvalid move
}

int is_game_over(){
	/* user code */
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			if(b[i][j] == 0) return 0;
		}
	}
	for(int i=0; i<4; i++){
		for(int j=0; j<3; j++){
			if(b[i][j] == b[i][j+1]) return 0;
		}
	}
	for(int i=0; i<3; i++){
		for(int j=0; j<4; j++){
			if(b[i][j] == b[i+1][j]) return 0;
		}
	}
	/* if game over return 0, else then return 1 */
	return 1;
}

void draw_board(int tot, int command){
	int i, j, k, c[8][8], score;
	/* console clear */
	system("clear");

	score = 0;
	/* user code */
	
	/* calculate score & check sum of all block equasls variable tot */
	int sum = 0;
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			sum += b[i][j];
			if(b[i][j] > score) score = b[i][j];
		}
	}
	

	printf("    Score : %d\n", score);
	fprintf(fp, "%d %d\n", score, command);

	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			c[i][j] = 32;
			for(k=0; k<50; k++){
				if( b[i][j] == (1<<k) ){
					c[i][j] = 32 + (k%6);
					break;
				}
			}

		}
	}
	for(i=0; i<4; i++){
		for(j=0; j<4; j++)
			fprintf(fp, "%d ", b[i][j]);
		fprintf(fp, "\n");
	}

	for(i=0; i<4; i++){
		puts("");
		puts("");
		for(j=0; j<4; j++)
			printf("\033[%dm%5d\033[0m", c[i][j], b[i][j]);
		puts("");
		puts("");
	}
}

int make_two_or_four(){
	/* user code */

	/* return 2 or 4 that makes in this times */
	int arr[16];
	int size = 0;
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			if(b[i][j] == 0){
				arr[size] = i*4 + j;
				size++;
			}
		}
	}
	if(size == 0) return 0;
	else{
		int num = rand() % size;
		int pos = arr[num];
		int row = pos / 4;
		int col = pos % 4;
		int two_or_four = (rand() % 3 == 0)? 4 : 2;
		b[row][col] = two_or_four;
		return two_or_four;
	}
	/* if can not make two or four, then return 0 */
}


int getch(void)
{
	char   ch;
	int   error;
	static struct termios Otty, Ntty;

	fflush(stdout);
	tcgetattr(0, &Otty);
	Ntty = Otty;
	Ntty.c_iflag = 0;
	Ntty.c_oflag = 0;
	Ntty.c_lflag &= ~ICANON;
#if 1
	Ntty.c_lflag &= ~ECHO;
#else
	Ntty.c_lflag |= ECHO;
#endif
	Ntty.c_cc[VMIN] = 0;
	Ntty.c_cc[VTIME] = 1;

#if 1
#define FLAG TCSAFLUSH
#else
#define FLAG TCSANOW
#endif

	if (0 == (error = tcsetattr(0, FLAG, &Ntty)))
	{
		error = read(0, &ch, 1);
		error += tcsetattr(0, FLAG, &Otty);
	}

	return (error == 1 ? (int)ch : -1);
}


int GetCommand() {
	int ch = getch();
	switch (ch)
	{
	case 'd':
	case 'D': return 0;
	case 'w':
	case 'W': return 1;
	case 'a':
	case 'A': return 2;
	case 's':
	case 'S': return 3;
	default: return -1;
	}

	return -1;
}


int main() {
	int command = -1;
	fp = fopen("output.txt", "w");
	init();
	srand(time(NULL));

	int tot = 0;

	/* make init board */
	/* user code */
	make_two_or_four();
	draw_board(tot, -1);

	
	do {
		command = GetCommand();
		
		if (command != -1) {
			if( set_board(command, b) ){

				/* user code */
				make_two_or_four();
				draw_board(tot, command);
			}
			game_over = is_game_over();
		}
	} while (!game_over);

	system("clear");
	printf("game over!\n");
	fclose( fp );
}
