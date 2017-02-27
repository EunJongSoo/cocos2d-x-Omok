#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class CMenuScene : public cocos2d::Layer {
private:
	void onTouch(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void replaceScene();
public:
	CMenuScene() {};
	virtual ~CMenuScene() {};
	virtual bool init();
	CREATE_FUNC(CMenuScene);
	static cocos2d::Scene* createScene();
};

#endif