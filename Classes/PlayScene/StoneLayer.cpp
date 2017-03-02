#define DEBUG_MODE 0
#define BOARD_SIZE_X 19
#define BOARD_SIZE_Y 19
#define MARGIN_PIXEL 11.0f
#define INTERVAL_PIXEL 21.0f

#include "StoneLayer.h"
#include "Computer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CStoneLayer::CStoneLayer() : board_size_x(BOARD_SIZE_X), board_size_y(BOARD_SIZE_Y), 
								margin_pixel(MARGIN_PIXEL), interval_pixel(INTERVAL_PIXEL), 
									min_max_xy_position{ 15, 4, 15, 4 }, count(0), scale(1.0f){}
CStoneLayer::~CStoneLayer() {
	CC_SAFE_DELETE(computer);
}

bool CStoneLayer::init() {
	if (!Layer::init())
		return false;
	computer = new CComputer();
	return true;
}

void CStoneLayer::calculationBoardSize(const Size &basesize, const float _scale) {
	scale = _scale;
	Size win_size = CCDirector::getInstance()->getWinSize();
	Size size = basesize * scale;
	str_position = Vec2(win_size.width / 2 - size.width / 2,
		win_size.height/2 - size.height / 2);
	end_position = Vec2(win_size.width /2 + size.width / 2,
		win_size.height/2 + size.height / 2);
	
	margin_pixel = margin_pixel * scale;
	interval_pixel = interval_pixel * scale;
	initStoneLayer();
}

void CStoneLayer::stopAllActionVector() {
	int size = my_struct_vector.size();
	for (int i = 0; i < size; ++i) {
		eraseMyStructVector(0);
	}
}

void CStoneLayer::initStoneLayer() {
	check_win = false;
	min_max_xy_position[0] = 10;
	min_max_xy_position[1] = 8;
	min_max_xy_position[2] = 10;
	min_max_xy_position[3] = 8;
	if(this->getChildrenCount() > 0)
		this->removeAllChildren();
#ifndef DEBUG_MODE == 1
	labelvec.clear();
#endif
	float pos_x = 0;
	float pos_y = 0;
	int pos = 0, y = 0, x = 0;
	// 바둑돌을 만들어 추가시킨다.
	for (y = 0; y < board_size_y; ++y) {
		for (x = 0; x < board_size_x; ++x) {
			CStoneSprite* sprite = CStoneSprite::create();
			pos_x = x * interval_pixel + margin_pixel + str_position.x;
			pos_y = y * interval_pixel + margin_pixel + str_position.y;
			sprite->setPosition(pos_x, pos_y);
			pos = board_size_y * y + x;
			this->addChild(sprite, 1, pos);
			sprite->setPos(pos);
#ifndef DEBUG_MODE == 1
			Label* label = Label::create();
			label->setPosition(pos_x, pos_y);
			label->setSystemFontSize(25);
			label->setString("0");
			label->setVisible(false);
			this->addChild(label, 10);
			labelvec.push_back(label);
#endif
		}
	}
}

void CStoneLayer::calculationPosition(const Vec2 &pos, const Stone s) {
	// 바둑돌을 착수한 위치가 바둑판 바깥쪽이면 착수 실패
	if (pos < str_position || pos > end_position) {
#ifndef DEBUG_MODE == 1
		CCLOG("************* positionCalculation, Fail");
#endif
		throw GameState::error;
	}
	// 좌표 계산
	int x = round((pos.x - margin_pixel - str_position.x) / (interval_pixel));
	int y = round((pos.y - margin_pixel - str_position.y) / (interval_pixel));
	this->createStone(Vec2(x, y), s);
}

void CStoneLayer::createStone(const Stone s) {
	computer->calculationScore(this);
	createStone(computer->computerTurn(this), s);
}

void CStoneLayer::createStone(const Vec2 &pos, const Stone s) {
	// 해당 좌표의 돌을 불러온다.
	CStoneSprite* pSprite = getSprite(pos.x, pos.y);

	// 불러온 돌의 종류를 확인해서 돌이 있으면 착수 실패
	if (pSprite->getStoneType() != Stone::emptied) {
		throw GameState::error;
	}
	// 바둑돌 이미지를 변경해준다.
	pSprite->initSprite(scale, s);
	SimpleAudioEngine::getInstance()->playEffect("sound/effect/ui-sound8.wav");
	// 착수된 지점은 점수계산이 필요없으므로 비활성화 시켜준다.
	pSprite->setActive(false);
	// 최대, 최소 좌표점 저장
	if (min_max_xy_position[max_x] < pos.x) min_max_xy_position[max_x] = pos.x;
	if (min_max_xy_position[min_x] > pos.x) min_max_xy_position[min_x] = pos.x;
	if (min_max_xy_position[max_y] < pos.y) min_max_xy_position[max_y] = pos.y;
	if (min_max_xy_position[min_y] > pos.y) min_max_xy_position[min_y] = pos.y;
	// 오목 여부를 검사한다.
	checkBoard(s);
	// 3~4개의 돌은 깜박이게 한다.
	BlinkingStoneSprite();
	// 주변 2칸을 활성화 시킨다.
	activeAroundStone(pos);
#ifndef DEBUG_MODE == 1
	computer->calculationScore(this);
#endif
}

void CStoneLayer::activeAroundStone(const Vec2 &pos) {
	for (int dir = 0; dir < Direction::dir_count; ++dir) {
		for (int depth = 1; depth <= 2; ++depth) {
			CStoneSprite* sprite = searchAroundSprite(pos.x, pos.y, dir, depth);
			if (sprite != nullptr && sprite->getStoneType() == Stone::emptied)
				sprite->setActive(true);
		}
	}
}

CStoneSprite* CStoneLayer::getSprite(const int x, const int y) {
	return dynamic_cast<CStoneSprite*>(this->getChildren().at(board_size_y * y + x));
}

CStoneSprite* CStoneLayer::searchAroundSprite(const float x, const float y, const int dir, const int depth) {
	int ix = 0, iy = 0;

	switch (dir) {
	case Direction::up: ix = 0, iy = 1; break;
	case Direction::down: ix = 0, iy = -1; break;
	case Direction::left: ix = -1, iy = 0; break;
	case Direction::right: ix = 1, iy = 0; break;
	case Direction::upper_left: ix = -1, iy = 1; break;
	case Direction::upper_right: ix = 1, iy = 1; break;
	case Direction::lower_left: ix = -1, iy = -1; break;
	case Direction::lower_right: ix = 1, iy = -1; break;
	default: ix = 0, iy = 0; break;
	}

	int tx = ix * depth + x;
	int ty = iy * depth + y;
	// 좌표 값이 0보다 작거나 바둑판 보다 크면 검사하지 않는다.
	if (tx < 0 || tx > board_size_x || ty < 0 || ty > board_size_y) return nullptr;

	return this->getSprite(tx, ty);
}

void CStoneLayer::checkBoard(const Stone s) {
	check_count = 0;
	check_stone_color = s;
	bool check[2] = { 0, 0 };
	if (min_max_xy_position[max_x] - min_max_xy_position[min_x] >= 3) {
		checkWidth();
		check[0] = true;
	}
	if (min_max_xy_position[max_y] - min_max_xy_position[min_y] >= 3) {
		checkHeight();
		check[1] = true;
	}
	if (check[0] && check[1]) {
		checkSlash();
		checkBackSlash();
	}
#ifndef DEBUG_MODE == 1
	CCLOG("count : %d\n", count);
	if (check_stone_color == Stone::black)
		CCLOG("CheckBoard Complete Color : Black\n");
	else if (check_stone_color == Stone::white)
		CCLOG("CheckBoard Complete Color : White\n");
#endif
}

GameState CStoneLayer::checkWinColor() {
	return check_stone_color == Stone::black ? GameState::black_win : GameState::white_win;
}

void CStoneLayer::checkStone(const int x, const int y, const Direction dir) {
	CStoneSprite* sprite = this->getSprite(x, y);
	// 0 : 현재 돌, 1 : 다음돌, 2 : 첫번째 이전돌
	Stone tmp_color[3];
	tmp_color[0] = sprite->getStoneType();
	if (tmp_color[0] == check_stone_color) {
		/* enum Direction {
		up, down, right, left, upper_left, lower_right, lower_left, upper_right, dir_count	};*/
		++check_count;
		if (check_count > 2 && check_count < 6) {
			tmp_color[1] = searchAroundSprite(x, y, dir, 1)->getStoneType();
			// 이번 돌과 다음 돌이 달라야한다.
			// 세번째 돌과 네번째 돌, 네번째 다섯번째 돌
			if (tmp_color[0] != tmp_color[1]) {
				tmp_color[2] = searchAroundSprite(x, y, dir + 1, check_count)->getStoneType();
				// 돌의 앞 뒤가 막히지 않아야 한다.
				if (tmp_color[1] == Stone::emptied || tmp_color[2] == Stone::emptied) {
					addVector(x, y, check_count + 2, dir);
				}
			}
		}
		if (check_count == 5)	check_win = true;
		else if (check_count > 5) check_win = false;
	}
	else {
		check_count = 0;
	}
#ifndef DEBUG_MODE == 1
	++count;
#endif
}

void CStoneLayer::addVector(const int x, const int y, const int count, const Direction dir) {
	// 돌의 앞뒤를 포함해서 모두 저장한다.
	MyStruct* my_struct = new MyStruct(count);
	CStoneSprite* sprite;
	my_struct->sprite[0] = searchAroundSprite(x, y, dir, 1);
	for (int i = 0; i < count-1; ++i) {
		sprite = searchAroundSprite(x, y, dir + 1, i);
		my_struct->sprite[i+1] = sprite;
		my_struct->sum_pos += sprite->getPos();
	}
	overlapCheckVector(my_struct);
}

void CStoneLayer::overlapCheckVector(MyStruct* mystr) {
	bool same_check = false;
	for (int i = 0; i < my_struct_vector.size(); ++i) {
		// 같은 점수가 있는지 검사한다.
		if (my_struct_vector.at(i)->sum_pos == mystr->sum_pos) {
			// 돌을 하나씩 비교한다. 같은 위치에 같은 돌이 없다면 다른 모임이므로 검사를 중단한다.
			for (int j = 0; j < my_struct_vector.at(i)->size; ++j) {
				if (my_struct_vector.at(i)->sprite[j]->getPos() == mystr->sprite[j]->getPos()) {
					same_check = true;
				}
				else {
					same_check = false;
					break;
				}
			}
			// 같은게 저장되 있어서 함수를 빠져나감
			if (same_check) return;
		}
	}
	my_struct_vector.push_back(mystr);
}

void CStoneLayer::BlinkingStoneSprite() {
	int forsize = my_struct_vector.size();
	int size = 0;
	int i = 0;
	for (int j = 0; j < forsize; ++j) {
		size = my_struct_vector.at(i)->size;
		// 첫번째 돌과 마지막 돌이 하나는 비어있을 것
		if (my_struct_vector.at(i)->sprite[0]->getStoneType() == Stone::emptied ||
			my_struct_vector.at(i)->sprite[size - 1]->getStoneType() == Stone::emptied) {
			size = my_struct_vector.at(i)->size;
			if (size == 5 || size == 6) {
				if (my_struct_vector.at(i)->sprite[1]->getStoneType()
					== my_struct_vector.at(i)->sprite[0]->getStoneType()
					|| my_struct_vector.at(i)->sprite[1]->getStoneType()
					== my_struct_vector.at(i)->sprite[size-1]->getStoneType()) {
					eraseMyStructVector(i);
				}
				else {
					++i;
				}
			}
			else {
				++i;
			}
		}
		else eraseMyStructVector(i);
	}
	for (int j = 0; j < my_struct_vector.size(); ++j) {
		runActionBlinkingSprite(j);
	}
}

void CStoneLayer::runActionBlinkingSprite(int i) {
	for (int k = 1; k < my_struct_vector.at(i)->size - 1; ++k) {
		my_struct_vector.at(i)->sprite[k]->runActionBlinkingSprite();
	}
}

void CStoneLayer::eraseMyStructVector(int i) {
	int size = my_struct_vector.at(i)->size;
	for (int k = 1; k < size - 1; ++k) {
		my_struct_vector.at(i)->sprite[k]->stopBlinkingSprite();
	}
	my_struct_vector.erase(my_struct_vector.begin() + i);
}

void CStoneLayer::checkWidth() {
	// 가로 검사 좌에서 우로 검사, 끝나면 한줄 위로 이동
	for (int y = min_max_xy_position[min_y]; y <= min_max_xy_position[max_y]; ++y) {
		for (int x = min_max_xy_position[min_x]; x <= min_max_xy_position[max_x]; ++x) {
			// width : left, height : down, slash : lower_left, back_slash : upper_left
			checkStone(x, y, Direction::right);
#ifndef DEBUG_MODE == 1
			CCLOG("********** checkWidth x:%d, y:%d, color:%d, count : %d", x, y, check_stone_color, check_count);
#endif
		}
		check_count = 0;
	}
	if (check_win) throw checkWinColor();
}

void CStoneLayer::checkHeight() {
	// 세로 검사 밑에서 위로 검사, 끝나면 한줄 옆으로 이동
	for (int x = min_max_xy_position[min_x]; x <= min_max_xy_position[max_x]; ++x) {
		for (int y = min_max_xy_position[min_y]; y <= min_max_xy_position[max_y]; ++y) {
			// width : left, height : down, slash : lower_left, back_slash : upper_left
			checkStone(x, y, Direction::up);
#ifndef DEBUG_MODE == 1
			CCLOG("********** checkHeight x:%d, y:%d, color:%d, count : %d", x, y, check_stone_color, check_count);
#endif
		}
		check_count = 0;
	}
	if (check_win) throw checkWinColor();
}

void CStoneLayer::checkSlash() {
	// 대각선 검사 오른쪽 위에서 왼쪽 아래 방향으로 검사
	for (int ix = min_max_xy_position[min_x], iy = min_max_xy_position[max_y];
		ix <= min_max_xy_position[max_x] && iy >= min_max_xy_position[min_y];
		ix == min_max_xy_position[max_x] ? --iy : ++ix) {
		for (int y = iy, x = ix; x >= min_max_xy_position[min_x] && y >= min_max_xy_position[min_y]; --y, --x) {
			// width : left, height : down, slash : upper_right, back_slash : upper_left
			checkStone(x, y, Direction::lower_left);
#ifndef DEBUG_MODE == 1
			CCLOG("********** CheckSlash x:%d, y:%d, color:%d, count : %d", x, y, check_stone_color, check_count);
#endif
		}
		check_count = 0;
	}
	if (check_win) throw checkWinColor();
}

void CStoneLayer::checkBackSlash() {
	// 역대각선 검사 오른쪽 아래에서 왼쪽 위 방향으로 검사
	for (int ix = min_max_xy_position[min_x], iy = min_max_xy_position[min_y];
		ix <= min_max_xy_position[max_x] && iy <= min_max_xy_position[max_y];
		ix == min_max_xy_position[max_x] ? ++iy : ++ix) {
		for (int y = iy, x = ix; x >= min_max_xy_position[min_x] && y <= min_max_xy_position[max_y]; ++y, --x) {
			// width : left, height : down, slash : lower_left, back_slash : upper_left
			checkStone(x, y, Direction::upper_left);
#ifndef DEBUG_MODE == 1
			CCLOG("********** checkBackSlash x:%d, y:%d, color:%d, count : %d", x, y, check_stone_color, check_count);
#endif
		}
		check_count = 0;
	}
	if (check_win) throw checkWinColor();
}