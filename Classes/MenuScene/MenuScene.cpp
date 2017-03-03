#include "MenuScene.h"
#include "PlayScene\PlayScene.h"
#include "SimpleAudioEngine.h"
#include "GossiniDanceSprite.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace ui;

Scene* CMenuScene::createScene() {
	Scene* scene = Scene::create();
	CMenuScene* layer = CMenuScene::create();
	scene->addChild(layer);
	return scene;
}

bool CMenuScene::init() {
	if (!Layer::init())
		return false;
	createBackGround();
	createMenu();
	createTitle();
	return true;
}

void CMenuScene::createBackGround() {
	Size winsize = CCDirector::getInstance()->getWinSize();
	Sprite* back_ground = Sprite::create("etc/bg_1.jpg");
	back_ground->setPosition(winsize / 2);
	back_ground->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(back_ground, 1);
	for (int i = 1; i < 5; ++i) {
		CGossiniDanceSprite* sprite = CGossiniDanceSprite::create();
		sprite->setPosition(winsize.width / 5 * i, winsize.height / 5 * 4);
		this->addChild(sprite, 2);
		sprite->runActionAnimation(CGossiniDanceSprite::GossiniDanceState::front_dance);
	}
}

void CMenuScene::createTitle() {
	Size winsize = CCDirector::getInstance()->getWinSize();
	float x = winsize.width / 5;
	float y = winsize.height / 5;

	Label* single = Label::create("Single", "font/Marker Felt.ttf", 50);
	single->setScale(0.1f);
	single->setPosition(Vec2(-100, winsize.height / 5 * 2));
	this->addChild(single, 2);
	MoveTo* moveto = MoveTo::create(0.2f, Vec2(x * 2, y * 3));
	ScaleTo* scaleto1 = ScaleTo::create(0.2f, 1.0f);
	RotateTo* rotateto = RotateTo::create(0.2f, 360 * 2);
	Spawn* spawn1 = Spawn::create(moveto, scaleto1, rotateto, NULL);
	ScaleTo* scaleto2 = ScaleTo::create(0.1f, 1.5f);
	CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(CMenuScene::effectSound, this));
	Sequence* sequence1 = Sequence::create(spawn1, scaleto2, callfunc, NULL);
	single->runAction(sequence1);
	
	Label* omok = Label::create("Omok", "font/Marker Felt.ttf", 50);
	omok->setPosition(Vec2(x * 3, y * 3));
	omok->setScale(0.1f);
	omok->setOpacity(0);
	this->addChild(omok, 2);
	FadeIn* fadein = FadeIn::create(0.6f);
	ScaleTo* scaleto3 = ScaleTo::create(0.6f, 1.5f);
	DelayTime* delay = DelayTime::create(0.6f);
	Sequence* sequence2 = Sequence::create(delay, callfunc, NULL);
	Spawn* spawn2 = Spawn::create(fadein, scaleto3, sequence2, NULL);
	
	omok->runAction(spawn2);
}

void CMenuScene::effectSound() {
	SimpleAudioEngine::getInstance()->playEffect("sound/Effect/effect_1.wav");
}

void CMenuScene::createMenu() {
	Size winsize = CCDirector::getInstance()->getWinSize();

	Label* item_label_1 = Label::create("PlayGame", "font/Marker Felt.ttf", 50);
	Label* item_label_2 = Label::create("Exit", "font/Marker Felt.ttf", 50);
	MenuItemLabel* item_1 = MenuItemLabel::create(item_label_1, CC_CALLBACK_1(CMenuScene::onTouchPlayGameButton, this));
	MenuItemLabel* item_2 = MenuItemLabel::create(item_label_2, CC_CALLBACK_1(CMenuScene::onTouchExitButton, this));

	Menu* menu = Menu::create(item_1, item_2, NULL);
	menu->alignItemsVertically();
	menu->setPosition(winsize.width / 10 * 8, winsize.height / 10 * 2);
	this->addChild(menu, 2);
}

// 이벤트별 행동을 정의해준다.
void CMenuScene::onTouchPlayGameButton(Ref* sender) {
	CallFunc* call = CallFunc::create(CC_CALLBACK_0(CMenuScene::replaceScene, this));
	FadeOut* fadeout = FadeOut::create(0.5f);
	Sequence* seq = Sequence::create(fadeout, call, NULL);
	this->runAction(seq);
}

void CMenuScene::onTouchExitButton(cocos2d::Ref* sender) {
	CCDirector::getInstance()->end();
}

void CMenuScene::replaceScene() {
	CPlayScene* scene = CPlayScene::create();
	Director::getInstance()->replaceScene(scene);
}