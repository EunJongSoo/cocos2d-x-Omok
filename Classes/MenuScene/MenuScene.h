#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class CMenuScene : public cocos2d::Layer {
public:
	CMenuScene() {};
	virtual ~CMenuScene() {};
	virtual bool init();
	CREATE_FUNC(CMenuScene);
	static cocos2d::Scene* createScene();

private:
	void createMenu();
	void createTitle();
	void createBackGround();
	void effectSound();
	void onTouchPlayGameButton(cocos2d::Ref* sender);
	void onTouchExitButton(cocos2d::Ref* sender);
	void replaceScene();
};

#endif