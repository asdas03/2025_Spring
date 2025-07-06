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
		void setup(); //���� ȭ�� ������ �ʱ�ȭ �� ������ �ҷ�����
		void update();
		void draw(); //���� ȭ�� �׷��ִ� ������ �ϴ� �Լ�
		void drawBoard(int y, int x, int starty, int startx, int size);
		// ���̵��� ���� �������� ��� �׷��ִ� �Լ�
		void timecounter();
		// ����ã�� ���ǵ��� �ɸ� �ð��� �����ϰ�, ���ݱ��� �ɸ� �ð��� ��Ÿ���ֱ� ���� �Լ�
		void printflag();
		// ���� ��� ��� ���� Ƚ���� ��Ÿ���ֱ� ���� �Լ�
		void resetField(int row, int col, int starty, int startx, int size);
		// ���� ���� �� �ʵ带 �ʱ�ȭ���ִ� �Լ�. �̶� �������� ���� ��ġ�ϰ�, �� �ڸ����� �ֺ��� ���ڰ����� �־���
		void checkField(int row, int col, int starty, int startx, int size, int y, int x);
		// ���� �÷��� ���� ��Ŭ���� ������ �� Ŭ���� ������ �޾ƿ��� �� ������ �巯���ִ� ����
		// ���� ���ڸ� Ŭ���߰ų�, ������ Ŭ�����ߴ����� �������
		void dfs(ii pos);
		// ����ã�� ���� ��Ģ�� ���� Ŭ���� ������ ������, �󸶳� �ֺ� ������ �������� ã���ִ� �Լ�. DFS �˰����� ���
		void drawGameover();
		// ���ӿ��� ȭ���� �׷��ִ� �Լ�
		void drawSuccess();
		// ������ Ŭ���� �������� ȭ���� �׷��ִ� �Լ�
		void placeFlag(int row, int col, int starty, int startx, int size, int y, int x);
		// ��Ŭ������ �÷��̾�� ���ڰ� ���� ������ ����Ǵ� ���� ����� ��ġ�� �� ����. �� ����� ������ �Լ�
		// Ŭ���� ������ �޾ƿ��� ����� checkField�� ������.
		void revealMines(int row, int col);
		// ��������� ��� ���� ��ġ�� �巯���ִ� �Լ�
		void exit() override;
		// ���� ����� ������ �ְ��ϵ��� �����ϱ� ���� ������ ofExit�� override ����

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
		// ���� �Լ����� of �⺻���� �Լ����̴�
		

		int menu;
		// ���� ���� ���¸� ��Ÿ���� ����
		int windowWidth, windowHeight;
		// ���� ����� ���α׷� ���� ��ġ�� ã�� ���� ����
		struct tile {
			bool hasMine;
			bool isRevealed;
			bool isFlagged;
			short int neighborCount;
			short int y, x;
		};
		// ����ã�� �������� �� ��ĭ�� �ڷ���. ���ڿ���, ��������, ��߿���, �ֺ� ���� ����, �»�� ��ġ���� �ϳ��� ����ü�� ����
		int tilerows[3] = { 8,10,12 };
		int tilecols[3] = { 8,10,12 };
		int mines[3] = { 8,10,12 };
		// �� ���̵��� ���� �������� �ళ��, ������, ���ڰ���
		int tilesize = 60;
		// �����ǿ��� ��ĭ�� ũ��
		int r, c, s;
		// ���� ���̵��� ���� �ళ��, ������, ��ĭ�� ũ�� ���� ����ִ� ����
		int i, j, k, l;
		// �ݺ����� ������
		string flagLeft;
		// ���� ��߰����� string���·� ��ȯ�ؼ� ������ֱ� ���� ���ڿ�
		// "Flags: #(������߼�)" �� �ϳ��� ���ڿ��̰�, �̸� printFlag���� �����
		int flagUsed;
		// �����ǿ� ��ġ�� ��� ����
		int currentFlag;
		// �� ���̵��� ���� �� ��� ��
		// ��� ���� ���� ���� ������ ���̵��� ���� mines[n] ���� �޾ƿ�
		// ���� ��߼� ī������ currentFlag - flagUsed
		int clickx, clicky, color;
		// Ŭ�� ��ǥ�� clickx, clicky�� �ְ� �ٸ� �Լ����� ���
		// color�� drawBoard()���� ��ġ�� ���� �ٸ� ���� �ֵ��� �����ص�
		int randy, randx;
		// resetField()���� ���ڸ� ��ġ�ϱ� ���� ������ row, col ���� ��� ���� ������
		bool loaded;
		// ó�� setup()�� ���� ������ ���� ������ ����� �ε�Ǿ������� �Ǻ��ϱ� ���� �Լ�
		// �� ���� �����̸� ������ ������ �� ����
		int isGameover, isSuccess, isNewRecord;
		// ���� ���°� ���� ��������, Ŭ��������, Ŭ����� �ְ����� �����ߴ����� �Ǻ����ִ� ����
		bool found;
		// Ŭ������ ��, Ŭ����ġ�� �ش��ϴ� Ÿ���� ã���� �� �ٷ� �ݺ����� �ߴ��ϱ� ���� ����
		int dy[8] = { 1,-1,0,0,1,1,-1,-1 };
		int dx[8] = { 0,0,1,-1,1,-1,-1,1 };
		// dfs�� �ֺ� ���� ��ġ�� ã�� �� ����ϱ� ���� ������ ��Ƴ��� �迭
		int nearby;
		// Ÿ���� �ֺ� ���� ����
		int tilesRevealed;
		// �巯�� Ÿ���� ����. �̰� ��üĭ - ���ڼ��� ������ �ٷ� ���� Ŭ����
		stack<ii> stk;
		// DFS���� ����ϱ� ���� ���� �ڷᱸ��
		tile field[12][12];
		// ������
		ofTrueTypeFont font;
		// ����� ���� ��Ʈ �ҷ����� �ڷ���

		ofImage flag;
		ofImage digit[9];
		// ����ϴ� ���ڴ� 1���� 8���� �� 8������, �ؿ��� ����ϱ� ���ҷ��� 1���� ���. �׷��� ���̰� 9�� ��.
		ofImage mine;
		// �̹��� ȣ��� �ڷ�����

		string path;
		// ���� ���, �̸��� ���� ���ڿ�
		int startTime, endTime;
		// �ð� ī������ ���� ����
		string currentTime, Timer;
		// Timer�� ���� ������ �帥 �ð��� ����ϱ� ���� ���ڿ�
		// currentTime�� ���� Ŭ����� �̸� ���Ͽ� ���� ���� ���ڿ�

		int ShortestTime[3];
		// �� ���̵��� ���� �ִܱ��
		int currentShortest;
		// ���� �÷������� ���̵��� �ִܱ��
		ofFile scoreRead;
		// highscore�� �б� ���� ����
		ofFile scoreWrite;
		// highscore�� ����� �ִܱ�ϵ��� ���� ���� ����
};
