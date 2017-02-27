#ifndef _TIMER_LAYER_H_
#define _TIMER_LAYER_H_

#include "cocos2d.h"
#include "enumHeader.h"

class CTimerLayer : public cocos2d::Layer {
private:
	float m_CountDown;
	float m_Timer;
	bool m_GameStart;

	cocos2d::Label* m_CountDownLabel;
	cocos2d::Label* m_TimerLabel;

public:
	CTimerLayer();
	virtual ~CTimerLayer();
	virtual bool init();
	CREATE_FUNC(CTimerLayer);

	bool getGameStart() const;
	void setGameStart(bool b = true);
	void setCountDownVisible(bool b = true);
	void setTimerVisible(bool b = true);
	
	bool UpdateCountDown(float time);
	void UpdateTimer(float time);
	void resetTimer();
	void resetTimerLayer();
};
#endif