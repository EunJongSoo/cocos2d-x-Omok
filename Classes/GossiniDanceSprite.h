#ifndef _GOSSINI_DANCE_SPRITE_
#define _GOSSINI_DANCE_SPRITE_

#include "cocos2d.h"


class CGossiniDanceSprite : public cocos2d::Sprite {
public:
	enum GossiniDanceState {
		front_dance, back_dance, idle,
	};

	CGossiniDanceSprite();
	virtual ~CGossiniDanceSprite();
	virtual bool init();
	CREATE_FUNC(CGossiniDanceSprite);

	void runActionAnimation(const GossiniDanceState state);
	void changeDanceAnimation();

private:
	cocos2d::Texture2D* idle_texture;
	cocos2d::Animation* front_animation;
	cocos2d::Animation* back_animation;

	GossiniDanceState dance_state;
};

#endif
