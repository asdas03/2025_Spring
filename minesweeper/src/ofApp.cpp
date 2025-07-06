#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//���α׷� â ���� ����
	ofSetWindowTitle("MineSweeper");
	ofBackground(20);
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	ofSetWindowPosition((ofGetScreenWidth() - windowWidth) / 2, (ofGetScreenHeight() - windowHeight) / 2);
	ofSetWindowShape(1024, 1024);
	font.setLetterSpacing(1.1);
	// ���ӿ��� ����� ������ �ʱ�ȭ
	menu = 0;
	startTime = 0;
	endTime = 0;
	flagUsed = 0;
	isGameover = 0;
	// 1���� 8������ �̹��� �ҷ�����
	for (i = 1; i <= 8; i++) {
		path = "digit_" + to_string(i) + ".png";
		loaded = digit[i].load(path);
		if (loaded) cout << i << " loaded succesfully!\n";
		else {
			cout << i << " load fail!\n";
			ofExit();
		}
	}
	// ��� �̹��� �ҷ�����
	loaded = flag.load("flag.png");
	if (loaded) cout << "flag loaded successfully!\n";
	else {
		cout << "flag load fail\n";
		ofExit();
	}
	// ���� �̹��� �ҷ�����
	loaded = mine.load("mine.png");
	if (loaded) cout << "mine loaded successfully!\n";
	// �ִܱ�� �ҷ�����
	ofFile scoreRead("highscore.txt", ofFile::ReadOnly, true);
	if (scoreRead.exists()) {
		for (int i = 0; i < 3; i++) {
			scoreRead >> ShortestTime[i];
			cout << ShortestTime[i] << '\n';
		}
		cout << "highscore.txt loaded sucessfully\n";
	}
	else {
		cout << "highscore.txt load fail\n";
		ofExit();
	}
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	switch (menu) {
	case 0:
		// ���� ����� �ٷ� ���̴� ȭ��. PLAY, EXIT�� �� ���̵��� ���� �ִܱ���� ������
		ofSetColor(255);
		font.load("consola.ttf", 64, true, true);
		font.drawString("MINESWEEPER", 240, 200);
		ofDrawRectangle(362, 300, 300, 100);
		ofSetColor(20);
		font.load("consola.ttf", 32, true, true);
		font.drawString("PLAY", 455, 360);
		ofSetColor(255);
		ofDrawRectangle(362, 450, 300, 100);
		ofSetColor(20);
		font.drawString("EXIT", 455, 510);
		ofSetColor(255);
		font.drawString("Record:", 152, 700);
		font.drawString("EASY: " + to_string(ShortestTime[0]), 152, 750);
		font.drawString("NORMAL: " + to_string(ShortestTime[1]), 152, 800);
		font.drawString("HARD: " + to_string(ShortestTime[2]), 152, 850);
		break;
	case 1:
		// 0���� PLAY ���ý� �ٷ� ���̴� ���̵� ���� ȭ��
		ofSetColor(255);
		font.load("consola.ttf", 48, true, true);
		font.drawString("CHOOSE DIFFICULTY", 190, 200);
		ofDrawRectangle(362, 300, 300, 100);
		ofSetColor(20);
		font.load("consola.ttf", 32, true, true);
		font.drawString("EASY", 455, 360);
		ofSetColor(255);
		ofDrawRectangle(362, 450, 300, 100);
		ofSetColor(20);
		font.drawString("NORMAL", 430, 510);
		ofSetColor(255);
		ofDrawRectangle(362, 600, 300, 100);
		ofSetColor(20);
		font.drawString("HARD", 455, 660);
		break;
	case 3:
		// 0���� EXIT�� ������ ���, �ٷ� ���α׷� ����
		ofExit();
		break;
	case 4:
		// EASY ���̵� �÷��� ȭ��, �Ź� ���� �������� Ŭ�������� Ȯ��
		if (isGameover) {
			drawGameover();
		}
		else if (isSuccess) {
			drawSuccess();
		}
		drawBoard(r, c, 272, 272, s);
		timecounter();
		printflag();
		break;
	case 5:
		// NORMAL ���̵� �÷��� ȭ��, �Ź� ���� �������� Ŭ�������� Ȯ��
		if (isGameover) {
			drawGameover();
		}
		else if (isSuccess) {
			drawSuccess();
		}
		drawBoard(r, c, 272, 212, s);
		timecounter();
		printflag();
		break;
	case 6:
		// HARD ���̵� �÷��� ȭ��, �Ź� ���� �������� Ŭ�������� Ȯ��
		if (isGameover) {
			drawGameover();
		}
		else if (isSuccess) {
			drawSuccess();
		}
		drawBoard(r, c, 272, 152, s);
		timecounter();
		printflag();
		break;
	}
}

//--------------------------------------------------------------
void ofApp::drawBoard(int row, int col, int starty, int startx, int size) {
	// field ��ü�� ���� �ش� ��ġ�� �ش��ϴ� �׸��� �׷��ش�.
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			color = ((i + j) % 2) ? 200 : 150;
			if (field[i][j].isFlagged) {
				ofSetColor(color);
				ofDrawRectangle(startx + j * 60, starty + i * 60, size, size);
				flag.draw(startx + j * 60, starty + i * 60, size, size);
			}
			else if (field[i][j].isRevealed) {
				if (!field[i][j].hasMine) {
					ofSetColor(255);
					ofDrawRectangle(startx + j * 60, starty + i * 60, size, size);
					digit[field[i][j].neighborCount].draw(startx + j * 60, starty + i * 60, size, size);
				}
				else {
					ofSetColor(255);
					ofDrawRectangle(startx + j * 60, starty + i * 60, size, size);
					mine.draw(startx + j * 60, starty + i * 60, size, size);
				}
			}
			else {
				ofSetColor(color);
				ofDrawRectangle(startx + j * 60, starty + i * 60, size, size);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::timecounter() {
	// ������ ������ ���� ���� �ð��� �귯������ if�� ���
	if (!isGameover && !isSuccess) {
		endTime = time(NULL);
		currentTime = to_string(endTime - startTime);
	}
	// ���� if���� ������ currentTime�� ���� ���� �����Ƿ� �ð� ������ �ȵ�
	Timer = "Time: " + currentTime;
	ofSetColor(255);
	font.load("consola.ttf", 48, true, true);
	font.drawString(Timer, 200, 200);
}

//--------------------------------------------------------------
void ofApp::printflag() {
	// ���� ��� ������ ������ִ� �Լ�
	flagLeft = "Flags: " + to_string(currentFlag - flagUsed);
	ofSetColor(255);
	font.load("consola.ttf", 48, true, true);
	font.drawString(flagLeft, 600, 200);
}

//--------------------------------------------------------------
void ofApp::resetField(int row, int col, int starty, int startx, int size) {
	// �� ���� ���۽� �ʵ带 �ʱ�ȭ �����ִ� �Լ�
	// �켱 �ʵ� ��ü ���� �ʱ�ȭ �����ش�
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			field[i][j].hasMine = 0;
			field[i][j].isFlagged = 0;
			field[i][j].isRevealed = 0;
			field[i][j].y = starty + 60 * i;
			field[i][j].x = startx + 60 * j;
		}
	}
	// ���� ������ ��ġ�� currentFlag ����ŭ ���ڵ��� ��ġ�Ѵ�
	for (i = 0; i < currentFlag; i++) {
		randy = (int)ofRandom(0, row);
		randx = (int)ofRandom(0, col);
		if (field[randy][randx].hasMine) i--;
		field[randy][randx].hasMine = 1;
	}
	// �ֺ� ���� �������� ������ Ÿ�Ͽ� �־��ش�
	int x, y;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			nearby = 0;
			for (k = 0; k < 8; k++) {
				y = i + dy[k]; x = j + dx[k];
				if (y < 0 || y >= row || x < 0 || x >= col) {
					continue;
				}
				else {
					if (field[y][x].hasMine) 
						nearby++;
				}
				field[i][j].neighborCount = nearby;
			}
		}
	}
}
//--------------------------------------------------------------
void ofApp::checkField(int row, int col, int starty, int startx, int size, int y, int x) {
	// Ŭ�� ��ġ�� ���� Ÿ�� ��ġ�� ã�� ���� �ݺ���
	found = 0;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (y > starty + i * 60 && y < starty + (i + 1) * 60 && x > startx + j * 60 && x < startx + (j + 1) * 60) {
				// Ŭ���� ��ġ�� Ÿ���� ����� if�� �� ��ȸ�ϸ鼭 ���� �ൿ�� ����
				if (field[i][j].hasMine) {
					// ���ڸ� �Ͷ߷����Ƿ� ��� ���ڵ��� �巯���� ���ӿ���
					revealMines(row, col);
					isGameover = 1;
					break;
				}
				else if (!field[i][j].isRevealed) {
					// ���ڸ� �� �Ͷ߷��� ��, ������ �巯���� ���� ���� ����� ������ �����, dfs �����ؼ� ���� �� �ִ� ���� ã��
					if (field[i][j].isFlagged) {
						field[i][j].isFlagged = 0;
						flagUsed--;
					}
					dfs({ i,j });
				}
				if (tilesRevealed == row * col - row) {
					// ���� ���� ���� �Ǵ�
					revealMines(row, col);
					isSuccess = 1;
					if (currentShortest > endTime - startTime) {
						isNewRecord = 1;
						currentShortest = endTime - startTime;
					}
				}
				found = 1;
				// �̹� ��ġ�� ã�����Ƿ� ������ �ݺ����� ������ �ʿ䰡 ����
				break;
			}
		}
		if (found) break;
	}
}

//--------------------------------------------------------------
void ofApp::dfs(ii pos) {
	// �巯�� �� �ִ� ������ ã���ִ� dfs �Լ�
	// ���� ����� ����� �־����� ������
	// ������ �巯���� ������ ���� �������� ��ĭ ������ ������ ���� ������ ���ڰ� �ϳ��� ���� ����̴�
	int uy = pos.first, ux = pos.second;
	int ty, tx;
	bool pushed;
	field[uy][ux].isRevealed = 1;
	stk.push({ uy,ux });
	if (field[uy][ux].isFlagged) {
		field[uy][ux].isFlagged = 0;
		flagUsed--;
	}
	tilesRevealed++;
	while (!stk.empty()) {
		pushed = 0;
		uy = stk.top().first; ux = stk.top().second;
		for (i = 0; i < 8; i++) {
			ty = uy + dy[i]; tx = ux + dx[i];
			if (ty > -1 && ty < r && tx > -1 && tx < c)
				if (!field[ty][tx].isRevealed && !field[uy][ux].neighborCount) {
					field[ty][tx].isRevealed = 1;
					tilesRevealed++;
					if (field[ty][tx].isFlagged) {
						field[ty][tx].isFlagged = 0;
						flagUsed--;
					}
					pushed = 1;
					stk.push({ ty,tx });
					break;
				}
		}
		if (!pushed) stk.pop();
	}
}
//--------------------------------------------------------------
void ofApp::drawGameover() {
	// ���ӿ��� ����� ���� �Լ�
	ofSetColor(255, 0, 0);
	font.load("consola.ttf", 48, true, true);
	font.drawString("Game Over!", 300, 100);
}

//--------------------------------------------------------------
void ofApp::drawSuccess() {
	// ���� Ŭ���� ����� ���� �Լ�
	ofSetColor(0, 255, 0);
	font.load("consola.ttf", 48, true, true);
	font.drawString("Success!", 300, 100);
}

//--------------------------------------------------------------
void ofApp::placeFlag(int row, int col, int starty, int startx, int size, int y, int x) {
	// �ʵ� ��ü�� �� �྿ Ž���ϸ鼭 ��Ŭ���� ����� ������ ����� ��ġ�ϰ�, ����� ������ ����� �����ִ� �Լ�
	found = 0;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (y > starty + i * 60 && y < starty + (i + 1) * 60 && x > startx + j * 60 && x < startx + (j + 1) * 60) {
				if (!field[i][j].isFlagged && !field[i][j].isRevealed && currentFlag - flagUsed > 0) {
					field[i][j].isFlagged = 1;
					flagUsed++;
				}
				else if (field[i][j].isFlagged) {
					field[i][j].isFlagged = 0;
					flagUsed--;
				}
				found = 1;
				break;
			}
		}
		if (found) break;
	}
}
//--------------------------------------------------------------
void ofApp::revealMines(int row, int col) {
	// ���� ������ ���ڵ��� �巯���ִ� �Լ�
	for (k = 0; k < row; k++) {
		for (l = 0; l < col; l++) {
			if (field[k][l].hasMine) {
				field[k][l].isRevealed = 1;
				field[k][l].isFlagged = 0;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
	// ���� ����� ȣ��Ǵ� �Լ�, �ִ� ��ϵ��� ���Ͽ� ��������
	ofFile scoreWrite("highscore.txt", ofFile::WriteOnly, true);
	for (i = 0; i < 3; i++) {
		scoreWrite << ShortestTime[i] << '\n';
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	clickx = ofGetMouseX();
	clicky = ofGetMouseY();
	cout << clickx << ' ' << clicky << '\n';
	if (button == 0) // left click
		switch (menu) {
		case 0:
			// �� play, exit ��ư ��ġ�� �ش�� �ش� �޴� ��ȣ�� ����
			if (clickx > 362 && clickx < 662 && clicky > 300 && clicky < 400) menu = 1;
			if (clickx > 362 && clickx < 662 && clicky > 450 && clicky < 550) menu = 3;
			break;
		case 1:
			// ���� ���̵� ���� �ܰ�
			// ���� ���������� ����ϴ� ������ �ʱ�ȭ, ���� �� �ܰ迡 �ش��ϴ� ������� �ʱ�ȭ
			isGameover = 0;
			isSuccess = 0;
			isNewRecord = 0;
			tilesRevealed = 0;
			if (clickx > 362 && clickx < 662 && clicky > 300 && clicky < 400) {
				// EASY ����
				menu = 4;
				startTime = time(NULL);
				r = tilerows[0];
				c = tilecols[0];
				s = tilesize;
				currentFlag = mines[0];
				currentShortest = ShortestTime[0];
				flagUsed = 0;
				resetField(r, c, 272, 272, s);
			}
			else if (clickx > 362 && clickx < 662 && clicky > 450 && clicky < 550) {
				// NORMAL ����
				menu = 5;
				startTime = time(NULL);
				r = tilerows[1];
				c = tilecols[1];
				s = tilesize;
				currentFlag = mines[1];
				currentShortest = ShortestTime[1];
				flagUsed = 0;
				resetField(r, c, 272, 212, s);
			}
			else if (clickx > 362 && clickx < 662 && clicky > 600 && clicky < 700) {
				// HARD ����
				menu = 6;
				startTime = time(NULL);
				r = tilerows[2];
				c = tilecols[2];
				s = tilesize;
				currentFlag = mines[2];
				currentShortest = ShortestTime[2];
				flagUsed = 0;
				resetField(r, c, 272, 152, s);
			}
			break;
		case 4:
			// EASY PLAY
			if (isGameover || isSuccess)
				menu = 0;
			else if (clickx > 272 && clickx < 752 && clicky > 272 && clicky < 752)
				checkField(r, c, 272, 272, s, clicky, clickx);
			if (isNewRecord)
				ShortestTime[0] = currentShortest;
			break;
		case 5:
			// NORMAL PLAY
			if (isGameover || isSuccess)
				menu = 0;
			else if (clickx > 212 && clickx < 812 && clicky > 272 && clicky < 852)
				checkField(r, c, 272, 212, s, clicky, clickx);
			if (isNewRecord)
				ShortestTime[1] = currentShortest;
			break;
		case 6:
			// HARD PLAY
			if (isGameover || isSuccess)
				menu = 0;
			else if (clickx > 152 && clickx < 872 && clicky > 272 && clicky < 972)
				checkField(r, c, 272, 152, s, clicky, clickx);
			if (isNewRecord)
				ShortestTime[2] = currentShortest;
			break;
		}
	else // right click
		// only for placing flags
		switch (menu) {
		case 4:
			placeFlag(r, c, 272, 272, s, clicky, clickx);
			break;
		case 5:
			placeFlag(r, c, 272, 212, s, clicky, clickx);
			break;
		case 6:
			placeFlag(r, c, 272, 152, s, clicky, clickx);
			break;
		}
	cout << menu << ' ' << currentFlag - flagUsed << '\n';
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
