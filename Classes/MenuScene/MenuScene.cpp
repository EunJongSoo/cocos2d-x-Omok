#include "MenuScene.h"
#include "PlayScene\PlayScene.h"

using namespace cocos2d;
using namespace ui;

Scene* CMenuScene::createScene() {
	Scene* scene = Scene::create();
	Layer* layer = CMenuScene::create();
	scene->addChild(layer);
	return scene;
}

bool CMenuScene::init() {
	if (!Layer::init())
		return false;
	// cocos studio에서 작업한 버튼을 불러온다.
	auto rootNode = CSLoader::createNode("MenuLayer.csb");
	// MenuScene Layer에 추가시킨다.
	addChild(rootNode, 0, 0);

	// Node에서 STARTBUTTON을 찾아서 변수에 저장한다.
	Button* start_button = (Button*)rootNode->getChildByName("STARTBUTTON");
	Button* exit_button = (Button*)rootNode->getChildByName("STARTBUTTON");
	// 버튼을 눌렀을때의 이벤트를 추가한다.
	start_button->addTouchEventListener(CC_CALLBACK_2(CMenuScene::onTouch, this));

	return true;
}


// 이벤트별 행동을 정의해준다.
void CMenuScene::onTouch(Ref* sender, Widget::TouchEventType type) {
	switch (type) {
	case Widget::TouchEventType::BEGAN:
		CCLOG("BEGAN");
		break;
	case Widget::TouchEventType::MOVED:
		CCLOG("MOVED");
		break;
	case Widget::TouchEventType::ENDED:
		CCLOG("ENDED");
		CallFunc* call = CallFunc::create(CC_CALLBACK_0(CMenuScene::replaceScene, this));
		FadeOut* fadeout = FadeOut::create(0.5f);
		Sequence* seq = Sequence::create(fadeout, call, NULL);
		Node* node = this->getChildByTag(0);
		node->runAction(seq);
		break;
	}
}

void CMenuScene::replaceScene() {
	CPlayScene* scene = CPlayScene::create();
	Director::getInstance()->replaceScene(scene);
}