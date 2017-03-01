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
	void stopCountDown();
	void runActionTimer();
	void resetTimer();
	void stopTimer();
	void setPauseCheck(bool* b);
	void setRunActionrFuncs(std::function<void(void)> gameupdate, 
							std::function<void(void)> timeover);

private:
	void updateCountDown(float dt);
	void updateTimer(float dt);
	std::function<void(void)>runActionGameUpdate;
	std::function<void(void)>runActionTimeOver;

private:
	int count_down;
	int timer;
	bool* pause_check;
	cocos2d::Label* count_down_label;
	cocos2d::Label* timer_label;
};
#endif