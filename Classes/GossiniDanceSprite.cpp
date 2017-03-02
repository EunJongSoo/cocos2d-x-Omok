#include "GossiniDanceSprite.h"

using namespace cocos2d;

CGossiniDanceSprite::CGossiniDanceSprite() : dance_state(GossiniDanceState::idle) {}

CGossiniDanceSprite::~CGossiniDanceSprite() {
	front_animation->release();
	back_animation->release();
}

bool CGossiniDanceSprite::init()
{
	if (!Sprite::init()) return false;

	TextureCache* pTexCache = Director::getInstance()->getTextureCache();
	idle_texture = pTexCache->addImage("Character/grossini.png");
	this->initWithTexture(idle_texture);
	
	front_animation = Animation::create();
	front_animation->setDelayPerUnit(0.3f);
	front_animation->setLoops(true);

	back_animation = Animation::create();
	back_animation->setDelayPerUnit(0.3f);
	back_animation->setLoops(true);

	int x = 85, y = 121;
	int j = 0, k = 0;
	for (int i = 0; i < 14; ++i) {
		SpriteFrame* sprite_frame = SpriteFrame::create("Character/grossini_dance_atlas.png", Rect(x * j, y * k, 85, 121));
		if (i > 3) {
			front_animation->addSpriteFrame(sprite_frame);
		}
		else {
			back_animation->addSpriteFrame(sprite_frame);
		}
		if (++j == 5) {
			j = 0;
			++k;
		}
	}
	front_animation->retain();
	back_animation->retain();
	return true;
}

void CGossiniDanceSprite::runActionAnimation(const GossiniDanceState state) {
	dance_state = state;
	this->stopAllActions();
	switch(dance_state){
	case GossiniDanceState::front_dance: {
		this->runAction(RepeatForever::create(Animate::create(front_animation)));
		return;
	}
	case GossiniDanceState::back_dance: {
		this->runAction(RepeatForever::create(Animate::create(back_animation)));
		return;
	}
	}
}

void CGossiniDanceSprite::changeDanceAnimation() {
	dance_state = dance_state == front_dance ? back_dance : front_dance;
	this->stopAllActions();
	switch (dance_state) {
	case GossiniDanceState::front_dance: {
		this->runAction(RepeatForever::create(Animate::create(front_animation)));
		return;
	}
	case GossiniDanceState::back_dance: {
		this->runAction(RepeatForever::create(Animate::create(back_animation)));
		return;
	}
	}
}