#define DEBUG_MODE 1
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
									min_max_xy_position{ 10, 8, 10, 8 }, count(0) {}
CStoneLayer::~CStoneLayer() {
	CC_SAFE_DELETE(computer);
}

bool CStoneLayer::init() {
	if (!Layer::init())
		return false;
	computer = new CComputer();

	base_sprite = Sprite::create("etc/Base.jpg");
	Size winsize = Director::getInstance()->getWinSize();		// 화면 크기
	Size basesize = base_sprite->getContentSize();						// 바둑판 이미지의 크기

	float widthscale = winsize.width / basesize.width;
	float heightscale = winsize.height / basesize.height;
	scale = widthscale > heightscale ? heightscale : widthscale;

	margin_pixel = margin_pixel * scale;
	interval_pixel = interval_pixel * scale;

	base_sprite->setAnchorPoint(Vec2(0.5, 0.5));
	base_sprite->setScale(scale);
	base_sprite->setPosition(winsize / 2);
	basesize = basesize * scale;

	// 바둑판 시작점과, 끝점의 값을 저장해둔다.
	// 바둑돌을 둘때 사용한다.
	str_position = Vec2(base_sprite->getPosition().x - basesize.width / 2,
		base_sprite->getPosition().y - basesize.height / 2);
	end_position = Vec2(base_sprite->getPosition().x + basesize.width / 2,
		base_sprite->getPosition().y + basesize.height / 2);

	initStoneLayer();
	return true;
}

void CStoneLayer::initStoneLayer() {
	check_win = false;
	min_max_xy_position[0] = 10;
	min_max_xy_position[1] = 8;
	min_max_xy_position[2] = 10;
	min_max_xy_position[3] = 8;
	if (this->getChildrenCount() > 0)
		this->removeAllChildren();
#ifdef DEBUG_MODE == 1
	labelvec.clear();
#endif
	float pos_x = 0;
	float pos_y = 0;
	// 바둑돌을 만들어 추가시킨다.
	for (int y = 0; y < board_size_y; ++y) {
		for (int x = 0; x < board_size_x; ++x) {
			CStoneSprite* sprite = CStoneSprite::create();
			pos_x = x * interval_pixel + margin_pixel + str_position.x;
			pos_y = y * interval_pixel + margin_pixel + str_position.y;
			sprite->setPosition(pos_x, pos_y);
			this->addChild(sprite, 1);
#ifdef DEBUG_MODE == 1
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
	addChild(base_sprite, 0);
}

void CStoneLayer::positionCalculation(const Vec2 &pos, const Stone s) {
	// 바둑돌을 착수한 위치가 바둑판 바깥쪽이면 착수 실패
	if (pos < str_position || pos > end_position) {
#ifdef DEBUG_MODE == 1
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
		CCLOG("******* StoneLayer.cpp CreateStone, Fail");
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
	// 주변 2칸을 활성화 시킨다.
	activeAroundStone(pos);
#ifdef DEBUG_MODE == 1
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
	if (min_max_xy_position[max_x] - min_max_xy_position[min_x] > 3) {
		checkWidth();
		check[0] = true;
	}
	if (min_max_xy_position[max_y] - min_max_xy_position[min_y] > 3) {
		checkHeight();
		check[1] = true;
	}
	if (check[0] && check[1]) {
		checkSlash();
		checkBackSlash();
	}
#ifdef DEBUG_MODE == 1
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

void CStoneLayer::checkStone(const int x, const int y) {
	CStoneSprite* sprite = this->getSprite(x, y);
	if (sprite->getStoneType() == check_stone_color) {
		if (++check_count == 5)	check_win = true;
		else if (check_count > 5) check_win = false;
	}
	else check_count = 0;
#ifdef DEBUG_MODE == 1
	++count;
#endif
}

void CStoneLayer::checkWidth() {
	// 가로 검사 좌에서 우로 검사, 끝나면 한줄 위로 이동
	for (int y = min_max_xy_position[min_y]; y <= min_max_xy_position[max_y]; ++y) {
		for (int x = min_max_xy_position[min_x]; x <= min_max_xy_position[max_x]; ++x) {
			checkStone(x, y);
#ifdef DEBUG_MODE == 1
			CCLOG("********** checkWidth x:%d, y:%d, color:%d, count : %d", x, y, check_stone_color, check_count);
#endif
		}
		check_count = 0;
#ifdef DEBUG_MODE == 1
		CCLOG("\n");
#endif
	}
	if (check_win) throw checkWinColor();
}

void CStoneLayer::checkHeight() {
	// 세로 검사 밑에서 위로 검사, 끝나면 한줄 옆으로 이동
	for (int x = min_max_xy_position[min_x]; x <= min_max_xy_position[max_x]; ++x) {
		for (int y = min_max_xy_position[min_y]; y <= min_max_xy_position[max_y]; ++y) {
			checkStone(x, y);
#ifdef DEBUG_MODE == 1
			CCLOG("********** checkHeight x:%d, y:%d, color:%d, count : %d", x, y, check_stone_color, check_count);
#endif
		}
		check_count = 0;
#ifdef DEBUG_MODE == 1
		CCLOG("\n");
#endif
	}
	if (check_win) throw checkWinColor();
}

void CStoneLayer::checkSlash() {
	// 대각선 검사 왼쪽위에서 오른쪽 아래 방향으로 검사 0, 19 ~ 19, 0으로
	for (int ix = min_max_xy_position[min_x], iy = min_max_xy_position[max_y];
		ix <= min_max_xy_position[max_x] && iy >= min_max_xy_position[min_y];
		ix == min_max_xy_position[max_x] ? --iy : ++ix) {
		for (int y = iy, x = ix; x >= min_max_xy_position[min_x] && y >= min_max_xy_position[min_y]; --y, --x) {
			checkStone(x, y);
#ifdef DEBUG_MODE == 1
			CCLOG("********** CheckSlash x:%d, y:%d, color:%d, count : %d", x, y, check_stone_color, check_count);
#endif
		}
		check_count = 0;
#ifdef DEBUG_MODE == 1
		CCLOG("\n");
#endif
	}
	if (check_win) throw checkWinColor();
}

void CStoneLayer::checkBackSlash() {
	for (int ix = min_max_xy_position[min_x], iy = min_max_xy_position[min_y];
		ix <= min_max_xy_position[max_x] && iy <= min_max_xy_position[max_y];
		ix == min_max_xy_position[max_x] ? ++iy : ++ix) {
		for (int y = iy, x = ix; x >= min_max_xy_position[min_x] && y <= min_max_xy_position[max_y]; ++y, --x) {
			checkStone(x, y);
#ifdef DEBUG_MODE == 1
			CCLOG("********** checkBackSlash x:%d, y:%d, color:%d, count : %d", x, y, check_stone_color, check_count);
#endif
		}
		check_count = 0;
#ifdef DEBUG_MODE == 1
		CCLOG("\n");
#endif
	}
	if (check_win) throw checkWinColor();
}