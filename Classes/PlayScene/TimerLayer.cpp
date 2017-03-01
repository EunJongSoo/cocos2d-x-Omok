#define COUNT_DOWN 3
#define FALL_BACK_TIME 5
#define SYSTEM_FONT_SIZE 50.0f

#include "PlayScene\TimerLayer.h"

using namespace cocos2d;

CTimerLayer::CTimerLayer():count_down(COUNT_DOWN), timer(FALL_BACK_TIME) {
	runActionGameUpdate = NULL;

}
CTimerLayer::~CTimerLayer() {}

void CTimerLayer::setRunActionrFuncs(std::function<void(void)> gameupdate,
										std::function<void(void)> timeover){
	runActionGameUpdate = gameupdate;
	runActionTimeOver = timeover;
}

bool CTimerLayer::init() {
	if (!Layer::init())
		return false;

	Size winSize = CCDirector::getInstance()->getWinSize();
	
	count_down_label = Label::create();
	count_down_label->setPosition(Vec2(winSize / 2));
	count_down_label->setSystemFontSize(SYSTEM_FONT_SIZE);
	count_down_label->setVisible(false);
	this->addChild(count_down_label);

	timer_label = Label::create();
	timer_label->setPosition(Vec2(winSize.width / 10 ,winSize.height / 1.1));
	timer_label->setSystemFontSize(SYSTEM_FONT_SIZE);
	timer_label->setVisible(false);
	this->addChild(timer_label);

	return true;
}

void CTimerLayer::initTimerLayer() {
	timer = FALL_BACK_TIME;
	count_down = COUNT_DOWN;
	count_down_label->setVisible(false);
	timer_label->setVisible(false);
}

void CTimerLayer::runActionCountDown() {
	count_down_label->setString("3");
	count_down_label->setVisible(true);
	this->schedule(schedule_selector(CTimerLayer::updateCountDown), 1);
}

void CTimerLayer::stopCountDown(){
	this->unschedule(schedule_selector(CTimerLayer::updateCountDown));
}

void CTimerLayer::runActionTimer() {
	timer = FALL_BACK_TIME;
	timer_label->setString("5");
	timer_label->setVisible(true);
	this->schedule(schedule_selector(CTimerLayer::updateTimer), 1);
}

void CTimerLayer::stopTimer() {
	this->unschedule(schedule_selector(CTimerLayer::updateTimer));
}

void CTimerLayer::resetTimer() {
	timer = FALL_BACK_TIME;
	timer_label->setString("5");
}

void CTimerLayer::updateCountDown(float dt) {
	if (!*pause_check) {
		--count_down;
		char str[4];
		sprintf_s(str, sizeof(str), "%d", count_down);
		count_down_label->setString(str);
		if (count_down < 0) {
			count_down_label->setVisible(false);
			runActionGameUpdate();
			runActionTimer();
			stopCountDown();
		}
	}
}

void CTimerLayer::updateTimer(float dt) {
	if (!*pause_check) {
		--timer;
		char str[4];
		sprintf_s(str, sizeof(str), "%d", timer);
		timer_label->setString(str);
		if (timer == 0) {
			stopTimer();
			runActionTimeOver();
		}
	}
}

void CTimerLayer::setPauseCheck(bool* b) {
	pause_check = b;
}