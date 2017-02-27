#include "StoneSprite.h"

CStoneSprite::CStoneSprite() : m_Stone(Emptied), m_iScore(0), m_bActive(false)
                               /*,ix(0), iy(0)*/ {}
CStoneSprite::~CStoneSprite() {}

bool CStoneSprite::init() {
	if (!Sprite::init()) return false;
	
	return true;
}

bool CStoneSprite::initSprite(cocos2d::Vec2 pos, const float scale, const int stone) {
	if (m_Stone == Emptied) {
		// 착수를 한 바둑돌의 이미지를 불러서
		switch (stone) {
		case Black:
			if (!initWithFile("Black.png"))	return false;
			break;
		case White:
			if (!initWithFile("White.png"))	return false;
			break;
		default:
			return false;
		}

		// 정해진 위치에 바둑돌을 띄워준다.
		m_Stone = stone;
		m_bActive = false;
		setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
		setPosition(pos);
		setScale(0.5 * scale);
		return true;
	}
	return false;
}

bool CStoneSprite::initSprite(const float x, const float y, const float scale, const int stone) {
	return initSprite(cocos2d::Vec2(x, y), scale, stone);
}

void CStoneSprite::setScore(const int score) {
	m_iScore = score;
}

void CStoneSprite::setActive(const bool b) {
	m_bActive = b;
}

bool CStoneSprite::getActive() const {
	return m_bActive;
}

int CStoneSprite::getScore() const {
	return m_iScore;
}

int CStoneSprite::getStoneType() const {
	return m_Stone;
}



//int CStoneSprite::getXpos() const {
//	return ix;
//}
//
//int CStoneSprite::getYpos() const {
//	return iy;
//}

//CStoneSprite* CStoneSprite::create(const int x, const int y) {
//	CStoneSprite* tmp = new CStoneSprite();
//	if (tmp && tmp->init(x, y))
//	{
//		tmp->autorelease();
//		return tmp;
//	}
//	else
//	{
//		if (tmp != nullptr) delete tmp;
//		tmp = nullptr;
//		return tmp;
//	}
//	return tmp;
//}
//
//bool CStoneSprite::init(const int x, const int y) {
//	if (!Sprite::init()) return false;
//	// 최초 바둑돌 위치 생성시 좌표값 입력
//	ix = x;
//	iy = y;
//	return true;
//}