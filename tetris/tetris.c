#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_REC_PLAY: recommendedPlay(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	if(head != NULL){
	node_pointer temp;
}
	return 0;
}

void InitTetris(){
	int i,j;
	createRankList();
	rankcreated = 1;
	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	nextBlock[2]=rand()%7;
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;
	DrawOutline();
	DrawField();
	DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawNextBlock(nextBlock);
	PrintScore(score);
	child rt = initroot();
	modified_recommend(rt);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK 1");
	DrawBox(3,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(9,WIDTH+10);
	printw("NEXT BLOCK 2");
	DrawBox(10,WIDTH+10,4,8);
	
	move(16,WIDTH+10);
	printw("SCORE");
	DrawBox(17,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag){
		DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	}
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(18,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){ // 1이면 출력을 해라
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
	for( i = 0; i < 4; i++ ){
		move(11+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[2]][0][i][j] == 1 ){ // 1이면 출력을 해라
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID, int blockRotate,char tile){
	int i,j;
	
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}
	}
	move(HEIGHT,WIDTH+10);
}

void DrawEmpty(int y, int x, int blockID,int blockRotate){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				printw(".");
			}
		}
		while(CheckToMove(field,blockID,blockRotate,y+1,x)) y++;
		for(i=0;i<4;i++){
			for(j=0;j<4;j++){
				if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
					move(i+y+1,j+x+1);
					printw(".");
				}
			}
		}
	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			if(block[currentBlock][blockRotate][i][j] == 1)
				if(blockY+i<0 || blockY+i>=HEIGHT || blockX+j<0 || blockX+j>=WIDTH || (f[blockY+i][blockX+j] == 1))
					return 0;
		}
	}
	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	//3. 새로운 블록 정보를 그린다.
	switch(command){
		case KEY_UP:
		if(blockRotate == 0) DrawEmpty(blockY,blockX,currentBlock,3);
		else DrawEmpty(blockY,blockX,currentBlock,blockRotate-1);
			break;
		case KEY_DOWN:
			DrawEmpty(blockY-1,blockX,currentBlock,blockRotate);
			break;
		case KEY_RIGHT:
			DrawEmpty(blockY,blockX-1,currentBlock,blockRotate);
			break;
		case KEY_LEFT:
			DrawEmpty(blockY,blockX+1,currentBlock,blockRotate);
			break;
		default:
			break;
	}
	DrawBlockWithFeatures(blockY,blockX,currentBlock,blockRotate);
}

void BlockDown(int sig){
	// user code
	//강의자료 p26-27의 플로우차트를 참고한다.
	timed_out = 0;
	if(CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)){
		DrawEmpty(blockY,blockX,nextBlock[0],blockRotate);
		blockY++;
		DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
	}
	else{
		if(blockY == -1){
			gameOver = 1;
		}
		AddBlockToField(field,nextBlock[0],blockRotate,blockY,blockX);
		int temp = DeleteLine(field);
		score += temp * temp * 100;
		nextBlock[0] = nextBlock[1];
		nextBlock[1] = nextBlock[2];
		nextBlock[2] = rand()%7;
		blockY = -1; blockX = WIDTH/2-2; blockRotate = 0;
		DrawNextBlock(nextBlock);
		PrintScore(score);
		child rt = initroot();
		modified_recommend(rt);
		DrawField();
		DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
		DrawRecommend(recommendY,recommendX,nextBlock[0],recommendR);
	}
}

void AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	//Block이 추가된 영역의 필드값을 바꾼다.
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			if(block[currentBlock][blockRotate][i][j]){
				f[blockY+i][blockX+j] = 1;
				if(blockY+i == HEIGHT-1) score += 10;
				else if(blockY+i < HEIGHT-1){
					if(f[blockY+i+1][blockX+j] == 1) score += 10;
				}
			}
		}
	}
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code
	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
	int plus = 0;
	int check = 0;
	for(int j=0; j<HEIGHT; j++){
		check = 0;
		for(int i=0; i<WIDTH; i++){
			if(f[j][i] == 1) check++;
		}
		if(check == WIDTH){
			for(int i=0; i<WIDTH; i++){
				f[j][i] = 0;
				for(int k=j; k>0; k--){
					f[k][i] = f[k-1][i];
				}
			}
			plus++;
		}
	}
	return plus;
}

///////////////////////////////////////////////////////////////////////////
void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
	while(CheckToMove(field,blockID,blockRotate,y+1,x)) y++;
	DrawBlock(y,x,blockID,blockRotate,'/');
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate){
	DrawRecommend(recommendY, recommendX, nextBlock[0], recommendR);
	DrawShadow(y,x,blockID,blockRotate);
	DrawBlock(y,x,blockID,blockRotate,' ');
}


void createRankList(){
	// 목적: Input파일인 "rank.txt"에서 랭킹 정보를 읽어들임, 읽어들인 정보로 랭킹 목록 생성
	// 1. "rank.txt"열기
	// 2. 파일에서 랭킹정보 읽어오기
	// 3. LinkedList로 저장
	// 4. 파일 닫기
	FILE *fp;
	int i, j;

	//1. 파일 열기
	fp = fopen("rank.txt", "r");
	head = (node_pointer)malloc(sizeof(node));
	tail = head;
	// 2. 정보읽어오기
	/* int fscanf(FILE* stream, const char* format, ...);
	stream:데이터를 읽어올 스트림의 FILE 객체를 가리키는 파일포인터
	format: 형식지정자 등등
	변수의 주소: 포인터
	return: 성공할 경우, fscanf 함수는 읽어들인 데이터의 수를 리턴, 실패하면 EOF리턴 */
	// EOF(End Of File): 실제로 이 값은 -1을 나타냄, EOF가 나타날때까지 입력받아오는 if문
	if (fscanf(fp,"%d",&score_num) != EOF) {
		node player;
		for(int i=0; i<score_num; i++){
			temp = (node_pointer)malloc(sizeof(node));
			fscanf(fp,"%s %d",temp->rank_name,&temp->rank_score);
			tail->next = temp;
			tail = tail->next;
		}
	}
	else {
		fprintf(stderr,"The Rank.txt is not available!\n");
		exit(1);
	}
	// 4. 파일닫기
	fclose(fp);
}

void rank(){
	//목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
	//1. 문자열 초기화
	createRankList();
	int X=1, Y=score_num, ch, i, j;
	clear();
	//2. printw()로 3개의 메뉴출력
	move(0,0);
	printw("This is rank page. Enter what to do");
	move(1,0);
	printw("1. list ranks from X to Y");
	move(2,0);
	printw("2. list ranks by a specific name");
	move(3,0);
	printw("3. delete a specific rank");
	//3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장
	echo();
	ch = wgetch(stdscr);
	clear();
	//4. 각 메뉴에 따라 입력받을 값을 변수에 저장
	//4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y사이의 rank 출력
	if (ch == '1') {
		move(0,0);
		printw("1. list ranks from X to Y");
		move(1,0);
		printw("Enter X and Y.");
		move(2,0);
		printw("X: ");
		move(3,0);
		printw("Y: ");
		move(5,7);
		printw("name     ||   score");
		move(6,0);
		printw("=====================================");
		move(2,3);
		scanw("%d",&X);
		move(2,3);
		printw("%d",X);
		move(3,3);
		scanw("%d",&Y);
		move(3,3);
		printw("%d",Y);
		if(X > Y || Y > score_num || X < 0){
			move(7,0);
			printw("Search Failure: No rank in the list");
		}
		else{
			temp = head;
			for(i=0; i<X; i++) temp = temp->next;
			for(i=X; i<=Y; i++){
				move(7+i-X,0);
				printw("%-16s|| %-12d",temp->rank_name,temp->rank_score);
				temp = temp->next;
			}
		}
	}
	//4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
	else if ( ch == '2') {
		char str[NAMELEN+1];
		int check = 0;
		clear();
		move(0,0);
		printw("2. list ranks by a specific name");
		move(1,0);
		printw("Enter the name to find");
		move(5,7);
		printw("name     ||   score");
		move(6,0);
		printw("=====================================");
		move(2,0);
		scanw("%s",str);
		temp = head;
		for(int i=0; i<score_num; i++){
			temp = temp->next;
			if(!strcmp(temp->rank_name,str)){
				move(7+check,0);
				printw("%-16s|| %-12d",temp->rank_name,temp->rank_score);
				check++;
			}
		}
		if(!check){
			move(7,0);
			printw("Search Failure: no name in the list");
		}
	}

	//4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
	else if ( ch == '3') {
		int num;
		clear();
		move(0,0);
		printw("3. delete a specific rank");
		move(1,0);
		printw("Enter the number to delete from rank");
		move(5,7);
		printw("name     ||   score");
		move(6,0);
		printw("=====================================");
		move(2,0);
		scanw("%d",&num);
		if(num == 1){
			temp = head->next;
			head->next = temp->next;
			free(temp);
		}
		else if(num > 1 && num <= score_num){
			temp = head;
			for(int i=0; i<num; i++){
				idx = temp;
				temp = temp->next;
			}
			idx->next = temp->next;
			free(temp);
			score_num--;
			move(7,0);
			printw("Success: Deleted %d from the list",num);
		}
		else{
			move(7,0);
			printw("Search Failure: Rank is not on the list");
		}
		writeRankFile();
	}
	getch();
	
}

void writeRankFile(){
	// 목적: 추가된 랭킹 정보가 있으면 새로운 정보를 "rank.txt"에 쓰고 없으면 종료
	int sn, i;
	//1. "rank.txt" 연다
	FILE *fp = fopen("rank.txt", "w");

	//2. 랭킹 정보들의 수를 "rank.txt"에 기록
	fprintf(fp,"%d\n",score_num);
	//3. 탐색할 노드가 더 있는지 체크하고 있으면 다음 노드로 이동, 없으면 종료
	if(score_num){
		temp = head;
		for(int i=0; i<score_num; i++){
			temp = temp->next;
			fprintf(fp,"%s %d\n",temp->rank_name,temp->rank_score);
		}
	}
	fclose(fp);
}

void newRank(int score){
	// 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
	char str[NAMELEN+1];
	int i, j;
	clear();
	//1. 사용자 이름을 입력받음
	move(1,1);
	echo();
	printw("your name: ");
	scanw("%s", str);
	temp = (node_pointer)malloc(sizeof(node));
	strcpy(temp->rank_name,str); temp->rank_score = score;
	//2. 새로운 노드를 생성해 이름과 점수를 저장, score_number가
	if(score_num) {
		idx = head;
		while(idx->next->rank_score > score) idx = idx->next;
		temp->next = idx->next;
		idx->next = temp;
	}
	else {
		head = (node_pointer)malloc(sizeof(node));
		tail = head;
		tail->next = temp;
		tail = tail->next;
	}
	score_num++;
	writeRankFile();
}


void DrawRecommend(int y, int x, int blockID,int blockRotate){
	int i,j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("R");
				attroff(A_REVERSE);
			}
		}
	}
	move(HEIGHT,WIDTH+10);
	
}

int modified_recommend(child root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	if(root->lv == MAX_TREE_LV) return root->accumulated_score;

	for(int r=0; r<NUM_OF_ROTATE; r++){
		for(int x=-2; x<WIDTH; x++){
			int y = -1;
			if(!CheckToMove(root->recfield,nextBlock[root->lv],r,y+1,x)) continue;

			while(CheckToMove(root->recfield,nextBlock[root->lv],r,y+1,x)) y++;

			child childnode = (child)malloc(sizeof(tree));
			childnode->lv = root->lv + 1;
			childnode->childnum = 0;
			childnode->accumulated_score = root->accumulated_score;
			for(int i=0; i<HEIGHT; i++){
				for(int j=0; j<WIDTH; j++){
					childnode->recfield[i][j] = root->recfield[i][j];
				}
			}
			for(int i=0; i<4; i++){
				for(int j=0; j<4; j++){
					if(block[nextBlock[root->lv]][r][i][j]){
						childnode->recfield[y+i][x+j] = 1;
						if(y+i == HEIGHT-1){
							childnode->accumulated_score += 10;
						}
						else if(y+i < HEIGHT-1){
							if(childnode->recfield[y+i+1][x+j] == 1){
								childnode->accumulated_score += 10;
							}
						}
					}
				}
			}
			int line = DeleteLine(childnode->recfield);
			childnode->accumulated_score += line * line * 100 + y*10;
			root->treeptr[root->childnum] = childnode;
			int tmp = recommend(root->treeptr[root->childnum]);
			if(tmp >= max){
				max = tmp;
				if(root->lv == 0){
					recommendX = x;	
					recommendY = y;
					recommendR = r;
				}
			}
			root->childnum++;
		}
	}

	for(int i=0; i<root->childnum; i++){
		free(root->treeptr[i]);
	}
	// user code
	return max;
}

int recommend(child root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수
	if(root->lv == MAX_TREE_LV) return root->accumulated_score;
	for(int r=0; r<4; r++){
		for(int l=-2; l<WIDTH; l++){
			int temp_y = -1;
			if(CheckToMove(root->recfield,nextBlock[root->lv],r,temp_y+1,l)){
				while(CheckToMove(root->recfield,nextBlock[root->lv],r,temp_y+1,l)) temp_y++;
				int n = root->childnum;
				root->treeptr[n] = (child)malloc(sizeof(tree));
				root->treeptr[n]->accumulated_score = root->accumulated_score;
				for(int i=0; i<HEIGHT; i++){
					for(int j=0; j<WIDTH; j++){
						root->treeptr[n]->recfield[i][j] = root->recfield[i][j];
					}
				}
				for(int i=0; i<4; i++){
					for(int j=0; j<4; j++){
						if(block[nextBlock[root->lv]][r][i][j]){
							root->treeptr[n]->recfield[temp_y+i][l+j] = 1;
							if(temp_y+i == HEIGHT-1){
								root->treeptr[n]->accumulated_score += 10;
							}
							else if(temp_y+i < HEIGHT-1){
								if(root->treeptr[n]->recfield[temp_y+i+1][l+j] == 1){
									root->treeptr[n]->accumulated_score += 10;
								}
							}
						}
					}
				}
				int line = DeleteLine(root->treeptr[n]->recfield);
				root->treeptr[n]->accumulated_score += line * line * 100;
				root->treeptr[n]->lv = root->lv + 1;
				root->childnum++;
				root->treeptr[n]->childnum = 0;
				int tmp = recommend(root->treeptr[n]);
				if(tmp >= max){
					max = tmp;
					if(root->lv == 0){
						recommendX = l;	
						recommendY = temp_y;
						recommendR = r;
					}
				}
			}
		}
	}
	for(int i=0; i<root->childnum; i++){
		free(root->treeptr[i]);
	}
	// user code
	return max;
}

child initroot(){ /*root 생성만을 위한 함수*/
	child x = (child)malloc(sizeof(tree));
	x->accumulated_score = 0;
	x->childnum = 0;
	x->lv = 0;
	for(int i=0; i<HEIGHT; i++){
		for(int j=0; j<WIDTH; j++){
			x->recfield[i][j] = field[i][j];
		}
	}
	return x;
}

void recommendedPlay() {
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}
		command = GetCommand();
		blockX = recommendX;
		blockY = recommendY;
		blockRotate = recommendR;
		for(int i=WIDTH/2-2; i<WIDTH/2+2; i++){
			for(int j=0; j<3; j++){
				if(field[j][i]) gameOver = 1;
				continue;
			}
		}
		if(command == QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();
			return;
		}
		
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}