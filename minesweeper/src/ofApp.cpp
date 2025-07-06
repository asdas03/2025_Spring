#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//프로그램 창 사전 세팅
	ofSetWindowTitle("MineSweeper");
	ofBackground(20);
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	ofSetWindowPosition((ofGetScreenWidth() - windowWidth) / 2, (ofGetScreenHeight() - windowHeight) / 2);
	ofSetWindowShape(1024, 1024);
	font.setLetterSpacing(1.1);
	// 게임에서 사용할 변수들 초기화
	menu = 0;
	startTime = 0;
	endTime = 0;
	flagUsed = 0;
	isGameover = 0;
	// 1부터 8까지의 이미지 불러오기
	for (i = 1; i <= 8; i++) {
		path = "digit_" + to_string(i) + ".png";
		loaded = digit[i].load(path);
		if (loaded) cout << i << " loaded succesfully!\n";
		else {
			cout << i << " load fail!\n";
			ofExit();
		}
	}
	// 깃발 이미지 불러오기
	loaded = flag.load("flag.png");
	if (loaded) cout << "flag loaded successfully!\n";
	else {
		cout << "flag load fail\n";
		ofExit();
	}
	// 지뢰 이미지 불러오기
	loaded = mine.load("mine.png");
	if (loaded) cout << "mine loaded successfully!\n";
	// 최단기록 불러오기
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
		// 게임 실행시 바로 보이는 화면. PLAY, EXIT과 각 난이도에 따른 최단기록을 보여줌
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
		// 0에서 PLAY 선택시 바로 보이는 난이도 선택 화면
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
		// 0에서 EXIT을 선택한 경우, 바로 프로그램 종료
		ofExit();
		break;
	case 4:
		// EASY 난이도 플레이 화면, 매번 게임 오버인지 클리어인지 확인
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
		// NORMAL 난이도 플레이 화면, 매번 게임 오버인지 클리어인지 확인
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
		// HARD 난이도 플레이 화면, 매번 게임 오버인지 클리어인지 확인
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
	// field 전체를 돌며 해당 위치에 해당하는 그림을 그려준다.
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
	// 게임이 끝나지 않을 때만 시간이 흘러가도록 if문 사용
	if (!isGameover && !isSuccess) {
		endTime = time(NULL);
		currentTime = to_string(endTime - startTime);
	}
	// 위의 if문이 없으면 currentTime에 값을 넣지 않으므로 시간 갱신이 안됨
	Timer = "Time: " + currentTime;
	ofSetColor(255);
	font.load("consola.ttf", 48, true, true);
	font.drawString(Timer, 200, 200);
}

//--------------------------------------------------------------
void ofApp::printflag() {
	// 남은 깃발 개수를 출력해주는 함수
	flagLeft = "Flags: " + to_string(currentFlag - flagUsed);
	ofSetColor(255);
	font.load("consola.ttf", 48, true, true);
	font.drawString(flagLeft, 600, 200);
}

//--------------------------------------------------------------
void ofApp::resetField(int row, int col, int starty, int startx, int size) {
	// 새 게임 시작시 필드를 초기화 시켜주는 함수
	// 우선 필드 전체 값을 초기화 시켜준다
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			field[i][j].hasMine = 0;
			field[i][j].isFlagged = 0;
			field[i][j].isRevealed = 0;
			field[i][j].y = starty + 60 * i;
			field[i][j].x = startx + 60 * j;
		}
	}
	// 이후 무작위 위치에 currentFlag 값만큼 지뢰들을 설치한다
	for (i = 0; i < currentFlag; i++) {
		randy = (int)ofRandom(0, row);
		randx = (int)ofRandom(0, col);
		if (field[randy][randx].hasMine) i--;
		field[randy][randx].hasMine = 1;
	}
	// 주변 지뢰 개수들을 각각의 타일에 넣어준다
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
	// 클릭 위치에 따른 타일 위치를 찾기 위한 반복문
	found = 0;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (y > starty + i * 60 && y < starty + (i + 1) * 60 && x > startx + j * 60 && x < startx + (j + 1) * 60) {
				// 클릭한 위치에 타일이 존재시 if문 을 순회하면서 이후 행동을 정함
				if (field[i][j].hasMine) {
					// 지뢰를 터뜨렸으므로 모든 지뢰들을 드러내고 게임오버
					revealMines(row, col);
					isGameover = 1;
					break;
				}
				else if (!field[i][j].isRevealed) {
					// 지뢰를 안 터뜨렸을 때, 영역을 드러내기 위해 만약 깃발이 있으면 지우고, dfs 실행해서 지울 수 있는 영역 찾기
					if (field[i][j].isFlagged) {
						field[i][j].isFlagged = 0;
						flagUsed--;
					}
					dfs({ i,j });
				}
				if (tilesRevealed == row * col - row) {
					// 게임 성공 여부 판단
					revealMines(row, col);
					isSuccess = 1;
					if (currentShortest > endTime - startTime) {
						isNewRecord = 1;
						currentShortest = endTime - startTime;
					}
				}
				found = 1;
				// 이미 위치를 찾았으므로 나머지 반복문을 실행할 필요가 없음
				break;
			}
		}
		if (found) break;
	}
}

//--------------------------------------------------------------
void ofApp::dfs(ii pos) {
	// 드러낼 수 있는 영역을 찾아주는 dfs 함수
	// 만약 깃발이 깔려져 있었으면 지워줌
	// 영역이 드러나는 기준은 시작 영역에서 한칸 떨어져 있으며 시작 영역에 지뢰가 하나도 없을 경우이다
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
	// 게임오버 출력을 위한 함수
	ofSetColor(255, 0, 0);
	font.load("consola.ttf", 48, true, true);
	font.drawString("Game Over!", 300, 100);
}

//--------------------------------------------------------------
void ofApp::drawSuccess() {
	// 게임 클리어 출력을 위한 함수
	ofSetColor(0, 255, 0);
	font.load("consola.ttf", 48, true, true);
	font.drawString("Success!", 300, 100);
}

//--------------------------------------------------------------
void ofApp::placeFlag(int row, int col, int starty, int startx, int size, int y, int x) {
	// 필드 전체를 한 행씩 탐색하면서 우클릭시 깃발이 없으면 깃발을 설치하고, 깃발이 있으면 깃발을 지워주는 함수
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
	// 게임 오버시 지뢰들을 드러내주는 함수
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
	// 게임 종료시 호출되는 함수, 최단 기록들을 파일에 갱신해줌
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
			// 각 play, exit 버튼 위치에 해당시 해당 메뉴 번호로 변경
			if (clickx > 362 && clickx < 662 && clicky > 300 && clicky < 400) menu = 1;
			if (clickx > 362 && clickx < 662 && clicky > 450 && clicky < 550) menu = 3;
			break;
		case 1:
			// 게임 난이도 선택 단계
			// 먼저 공통적으로 사용하는 변수들 초기화, 이후 각 단계에 해당하는 변수들로 초기화
			isGameover = 0;
			isSuccess = 0;
			isNewRecord = 0;
			tilesRevealed = 0;
			if (clickx > 362 && clickx < 662 && clicky > 300 && clicky < 400) {
				// EASY 선택
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
				// NORMAL 선택
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
				// HARD 선택
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
