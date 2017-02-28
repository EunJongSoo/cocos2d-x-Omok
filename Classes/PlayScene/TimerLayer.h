#ifndef _TIMER_LAYER_H_
#define _TIMER_LAYER_H_

#include "cocos2d.h"
#include "enumHeader.h"

class CTimerLayer : public cocos2d::Layer {
public:
	CTimerLayer();
	virtual ~CTimerLayer();
	virtual bool init();
	CREATE_FUNC(CTimerLayer);

	void initTimerLayer();
	void runActionCountDown();
	void runActionTimer();
	void unscheduleTimer();
	bool getGameStartCheck() const;
	void setGameStart(bool b);
	bool getTimeOverCheck() const;
	void setTimeOver(bool b);

	void setRunActionrFuncs(std::function<void(void)> gameupdate, 
							std::function<void(void)> timeover);

private:
	void updateCountDown(float dt);
	void updateTimer(float dt);
	std::function<void(void)>runActionGameUpdate;
	std::function<void(void)>runActionTimeOver;

private:
	float countDown;
	float timer;
	bool gameStartCheck;
	bool timeOverCheck;
	cocos2d::Label* countDownLabel;
	cocos2d::Label* timerLabel;
};
#endif