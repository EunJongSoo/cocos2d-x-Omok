#define COUNT_DOWN 4.0f
#define FALL_BACK_TIME 6.0f
#define SYSTEM_FONT_SIZE 50.0f

#include "PlayScene\TimerLayer.h"

using namespace cocos2d;

class CPlayScene;

CTimerLayer::CTimerLayer():countDown(COUNT_DOWN), timer(FALL_BACK_TIME), 
							gameStartCheck(false), timeOverCheck(false) {
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
	
	countDownLabel = cocos2d::Label::create();
	countDownLabel->setPosition(Vec2(winSize / 2));
	countDownLabel->setSystemFontSize(SYSTEM_FONT_SIZE);
	countDownLabel->setVisible(false);
	this->addChild(countDownLabel);

	timerLabel = cocos2d::Label::create();
	timerLabel->setPosition(Vec2(winSize.width / 10 ,winSize.height / 1.1));
	timerLabel->setSystemFontSize(SYSTEM_FONT_SIZE);
	timerLabel->setVisible(false);
	this->addChild(timerLabel);

	return true;
}

void CTimerLayer::initTimerLayer() {
	timer = FALL_BACK_TIME;
	countDown = COUNT_DOWN;
	gameStartCheck = false;
	countDownLabel->setVisible(false);
	timerLabel->setVisible(false);
}

void CTimerLayer::runActionCountDown() {
	countDownLabel->setVisible(true);
	this->schedule(schedule_selector(CTimerLayer::updateCountDown), 1.0f);
}

void CTimerLayer::runActionTimer() {
	timer = FALL_BACK_TIME;
	timerLabel->setVisible(true);
	this->schedule(schedule_selector(CTimerLayer::updateTimer), 1.0f);
}

void CTimerLayer::updateCountDown(float dt) {
	countDown -= dt;
	char str[4];
	sprintf_s(str, sizeof(str), "%d", (int)countDown);
	countDownLabel->setString(str);
	if (countDown < 0) {
		countDownLabel->setVisible(false);
		runActionGameUpdate();
	}
}

void CTimerLayer::updateTimer(float dt) {
	timer -= dt;
	char str[4];
	sprintf_s(str, sizeof(str), "%d", (int)timer);
	timerLabel->setString(str);
	if (timer < 0) {
		runActionTimeOver();
	}
}

void CTimerLayer::unscheduleTimer() {
	this->unschedule(schedule_selector(CTimerLayer::updateTimer));
}

bool CTimerLayer::getGameStartCheck() const {
	return gameStartCheck;
}

void CTimerLayer::setGameStart(bool b) {
	gameStartCheck = b;
}

bool CTimerLayer::getTimeOverCheck() const {
	return timeOverCheck;
}

void CTimerLayer::setTimeOver(bool b) {
	timeOverCheck = b;
}