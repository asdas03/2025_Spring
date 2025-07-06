#pragma once

#include "ofMain.h"
#define MAIN 0
#define GAME 1
#define EXIT 3
#define EASY 4
#define NORMAL 5
#define HARD 6
#define ii pair<int,int>

class ofApp : public ofBaseApp{

	public:
		void setup(); //게임 화면 실행전 초기화 및 사진들 불러오기
		void update();
		void draw(); //게임 화면 그려주는 역할을 하는 함수
		void drawBoard(int y, int x, int starty, int startx, int size);
		// 난이도에 따른 게임판을 계속 그려주는 함수
		void timecounter();
		// 지뢰찾기 한판동안 걸린 시간을 측정하고, 지금까지 걸린 시간을 나타내주기 위한 함수
		void printflag();
		// 남은 깃발 사용 가능 횟수를 나타내주기 위한 함수
		void resetField(int row, int col, int starty, int startx, int size);
		// 게임 시작 전 필드를 초기화해주는 함수. 이때 무작위로 지뢰 설치하고, 각 자리마다 주변에 지뢰개수를 넣어줌
		void checkField(int row, int col, int starty, int startx, int size, int y, int x);
		// 게임 플레이 동안 좌클릭이 들어왔을 때 클릭한 영역을 받아오고 그 영역을 드러내주는 역할
		// 또한 지뢰를 클릭했거나, 게임을 클리어했는지를 계산해줌
		void dfs(ii pos);
		// 지뢰찾기 게임 규칙에 따라 클릭한 영역이 있을때, 얼마나 주변 영역을 밝힐지를 찾아주는 함수. DFS 알고리즘을 사용
		void drawGameover();
		// 게임오버 화면을 그려주는 함수
		void drawSuccess();
		// 게임을 클리어 했을시의 화면을 그려주는 함수
		void placeFlag(int row, int col, int starty, int startx, int size, int y, int x);
		// 우클릭으로 플레이어는 지뢰가 있을 것으로 예상되는 곳에 깃발을 설치할 수 있음. 이 기능을 구현한 함수
		// 클릭한 영역을 받아오는 방식은 checkField와 동일함.
		void revealMines(int row, int col);
		// 게임종료시 모든 지뢰 위치를 드러내주는 함수
		void exit() override;
		// 게임 종료시 갱신한 최고기록들을 저장하기 위해 기존의 ofExit을 override 했음

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		// 여기 함수들은 of 기본제공 함수들이다
		

		int menu;
		// 현재 게임 상태를 나타내는 변수
		int windowWidth, windowHeight;
		// 게임 실행시 프로그램 실행 위치를 찾기 위한 변수
		struct tile {
			bool hasMine;
			bool isRevealed;
			bool isFlagged;
			short int neighborCount;
			short int y, x;
		};
		// 지뢰찾기 게임판의 각 한칸의 자료형. 지뢰여부, 공개여부, 깃발여부, 주변 지뢰 개수, 좌상단 위치값을 하나의 구조체로 선언
		int tilerows[3] = { 8,10,12 };
		int tilecols[3] = { 8,10,12 };
		int mines[3] = { 8,10,12 };
		// 각 난이도에 따른 게임판의 행개수, 열개수, 지뢰개수
		int tilesize = 60;
		// 게임판에서 한칸의 크기
		int r, c, s;
		// 현재 난이도에 따른 행개수, 열개수, 한칸의 크기 값을 담고있는 변수
		int i, j, k, l;
		// 반복문용 변수들
		string flagLeft;
		// 남은 깃발개수를 string형태로 변환해서 출력해주기 위한 문자열
		// "Flags: #(남은깃발수)" 가 하나의 문자열이고, 이를 printFlag에서 출력함
		int flagUsed;
		// 게임판에 설치된 깃발 개수
		int currentFlag;
		// 각 난이도에 따른 총 깃발 수
		// 깃발 수는 지뢰 수와 같으니 난이도에 따라 mines[n] 값을 받아옴
		// 남은 깃발수 카운팅은 currentFlag - flagUsed
		int clickx, clicky, color;
		// 클릭 좌표를 clickx, clicky로 넣고 다른 함수에서 사용
		// color는 drawBoard()에서 위치에 따른 다른 색을 넣도록 설정해둠
		int randy, randx;
		// resetField()에서 지뢰를 설치하기 위한 무작위 row, col 값을 담기 위한 변수들
		bool loaded;
		// 처음 setup()때 게임 실행을 위한 파일이 제대로 로드되었는지를 판별하기 위한 함수
		// 이 값이 거짓이면 게임을 실행할 수 없다
		int isGameover, isSuccess, isNewRecord;
		// 현재 상태가 게임 오버인지, 클리어인지, 클리어면 최고기록을 갱식했는지를 판별해주는 변수
		bool found;
		// 클릭했을 때, 클릭위치에 해당하는 타일을 찾았을 시 바로 반복문을 중단하기 위한 변수
		int dy[8] = { 1,-1,0,0,1,1,-1,-1 };
		int dx[8] = { 0,0,1,-1,1,-1,-1,1 };
		// dfs나 주변 지뢰 위치를 찾을 때 사용하기 위한 값들을 모아놓은 배열
		int nearby;
		// 타일의 주변 지뢰 개수
		int tilesRevealed;
		// 드러난 타일의 개수. 이게 전체칸 - 지뢰수와 같으면 바로 게임 클리어
		stack<ii> stk;
		// DFS에서 사용하기 위한 스택 자료구조
		tile field[12][12];
		// 게임판
		ofTrueTypeFont font;
		// 출력을 위한 폰트 불러오는 자료형

		ofImage flag;
		ofImage digit[9];
		// 사용하는 숫자는 1부터 8까지 총 8개지만, 밑에서 사용하기 편할려고 1부터 사용. 그래서 길이가 9가 됨.
		ofImage mine;
		// 이미지 호출용 자료형들

		string path;
		// 파일 경로, 이름을 위한 문자열
		int startTime, endTime;
		// 시간 카운팅을 위한 변수
		string currentTime, Timer;
		// Timer는 게임 시작후 흐른 시간을 출력하기 위한 문자열
		// currentTime은 게임 클리어시 이를 파일에 쓰기 위한 문자열

		int ShortestTime[3];
		// 각 난이도에 따른 최단기록
		int currentShortest;
		// 현재 플레이중인 난이도의 최단기록
		ofFile scoreRead;
		// highscore를 읽기 위한 변수
		ofFile scoreWrite;
		// highscore에 기록한 최단기록들을 쓰기 위한 변수
};
