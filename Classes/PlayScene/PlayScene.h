#ifndef _PLAY_SCENE_H_
#define _PLAY_SCENE_H_

#include "cocos2d.h"
#include "enumHeader.h"

// 클래스 전방선언
// ******* effective 항목 31이 이게 맞나
// ******* effective 항목 31이 이게 맞나
// ******* effective 항목 31이 이게 맞나

class CStoneLayer;
class CTimerLayer;
class CUILayer;

class sound;


class CPlayScene : public cocos2d::Scene {
private:
	CStoneLayer* m_StoneLayer;
	CTimerLayer* m_TimerLayer;
	CUILayer* m_UiLayer;

	int m_PlayerColor;
	int m_ComputerColor;
	int m_Turn;

private:
	void EndGame();
	void EffectSound();
	void gameupdate(const float dt);
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	// 옵션 저장 및 불러오기
	void DataLoad();
	void DataSave();

public:
	CPlayScene();
	virtual ~CPlayScene();
	virtual bool init();
	CREATE_FUNC(CPlayScene);
};
#endif