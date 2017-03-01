#ifndef _STONE_SPRITE_H_
#define _STONE_SPRITE_H_

#include "cocos2d.h"
#include "enumHeader.h"

class CStoneSprite : public cocos2d::Sprite {
public:
	CStoneSprite();
	virtual ~CStoneSprite();
	virtual bool init();
	CREATE_FUNC(CStoneSprite);
	
	bool CStoneSprite::initSprite(const float scale, const Stone s);

	void runActionBlinkingSprite();
	void stopBlinkingSprite();
	int getScore() const;
	void setScore(const int s);
	int getPos() const;
	void setPos(const int s);
	bool getActive() const;
	void setActive(const bool b);
	Stone getStoneType() const;

	int pos;
private:
	int score;
	bool active;
	Stone stone;
};
#endif