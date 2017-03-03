#include "StoneSprite.h"

CStoneSprite::CStoneSprite() : stone(Stone::emptied), score(0), active(false), rotate(false), blinking(false){}
CStoneSprite::~CStoneSprite() {
	blinking_sprite->release();
	this->release();
}

using namespace cocos2d;

CStoneSprite* CStoneSprite::create(const Stone s) {
	CStoneSprite* tmp = new CStoneSprite();
	if (tmp && tmp->init(s))
	{
		tmp->autorelease();
		return tmp;
	}
	else
	{
		if (tmp != nullptr) delete tmp;
		tmp = nullptr;
		return tmp;
	}
	return tmp;
}

bool CStoneSprite::init(const Stone s) {
	if (!Sprite::init()) return false;
	stone = Stone::block;
	this->retain();
	return true;
}

bool CStoneSprite::init() {
	if (!Sprite::init()) return false;
	this->retain();
	FadeIn* fade_in = FadeIn::create(0.3f);
	FadeOut* fade_out = FadeOut::create(0.3f);
	Sequence* seq = Sequence::create(fade_in, fade_out, NULL);
	blinking_sprite = RepeatForever::create(seq);
	blinking_sprite->retain();
	blinking_sprite->setTag(1);
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

void CStoneSprite::runActionRotateSprite() {
	if (!rotate) {
		rotate = true;
		RotateBy* rotate = RotateBy::create(0.4f, Vec3(360, 360, 360));
		ScaleBy* scaleby = ScaleBy::create(0.2f, 1.2f);
		MoveBy* moveby = MoveBy::create(0.2f, Vec3(0.0f, 20.0f, 0.0f));
		CallFunc* call = CallFunc::create(CC_CALLBACK_0(CStoneSprite::falseRotateCheck, this));
		Sequence* seq1 = Sequence::create(scaleby, scaleby->reverse(), NULL);
		Sequence* seq2 = Sequence::create(moveby, moveby->reverse(), call, NULL);
		Spawn* spawn = Spawn::create(seq1, seq2, rotate, NULL);
		this->runAction(spawn);
	}
}

void CStoneSprite::falseRotateCheck() {
	rotate = false;
}

void CStoneSprite::runActionBlinkingSprite(){
	if (!blinking) {
		blinking = true;
		this->runAction(blinking_sprite);
	}
}

void CStoneSprite::stopBlinkingSprite() {
	blinking = false;
	this->stopActionByTag(1);
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

