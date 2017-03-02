#include "StoneSprite.h"

CStoneSprite::CStoneSprite() : stone(Stone::emptied), score(0), active(false) {}
CStoneSprite::~CStoneSprite() {}

using namespace cocos2d;

bool CStoneSprite::init() {
	if (!Sprite::init()) return false;
	return true;
}

bool CStoneSprite::initSprite(const float scale, const Stone s) {
	if (stone == Stone::emptied) {
		// 착수를 한 바둑돌의 이미지를 불러서
		switch (s) {
		case Stone::black:
			if (!initWithFile("etc/Black.png"))	return false;
			break;
		case Stone::white:
			if (!initWithFile("etc/White.png"))	return false;
			break;
		default:
			return false;
		}
		stone = s;
		active = false;
		setAnchorPoint(Vec2(0.5f, 0.5f));
		setScale(0.5 * scale);
		return true;
	}
	return false;
}

void CStoneSprite::runActionBlinkingSprite(){
	FadeIn* fade_in = FadeIn::create(0.3f);
	FadeOut* fade_out = FadeOut::create(0.3f);
	Sequence* seq = Sequence::create(fade_in, fade_out, NULL);
	RepeatForever* repeat_forever = RepeatForever::create(seq);
	this->runAction(repeat_forever);
}

void CStoneSprite::stopBlinkingSprite() {
	this->stopAllActions();
	FadeIn* fade_in = FadeIn::create(0.1f);
	this->runAction(fade_in);
}

int CStoneSprite::getScore() const {
	return score;
}
void CStoneSprite::setScore(const int i) {
	score = i;
}

int CStoneSprite::getPos() const {
	return pos;
}
void CStoneSprite::setPos(const int i) {
	pos = i;
}

bool CStoneSprite::getActive() const {
	return active;
}
void CStoneSprite::setActive(const bool b) {
	active = b;
}

Stone CStoneSprite::getStoneType() const {
	return stone;
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