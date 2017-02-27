#include "PlayScene\TimerLayer.h"

using namespace cocos2d;

const float CountDown = 3.0f;
const float FallBackTime = 5.0f;
const float SystemFontSize = 50.0f;

CTimerLayer::CTimerLayer():m_CountDown(CountDown), m_Timer(FallBackTime), m_GameStart(false){}
CTimerLayer::~CTimerLayer() {}

bool CTimerLayer::init() {
	if (!Layer::init())
		return false;

	Size winSize = CCDirector::getInstance()->getWinSize();
	
	m_CountDownLabel = cocos2d::Label::create();
	m_CountDownLabel->setPosition(Vec2(winSize / 2));
	m_CountDownLabel->setSystemFontSize(SystemFontSize);
	m_CountDownLabel->setVisible(false);
	this->addChild(m_CountDownLabel);

	m_TimerLabel = cocos2d::Label::create();

	// ********* 타임레이블 위치 잡을 방법 구상필요
	// ********* 타임레이블 위치 잡을 방법 구상필요
	// ********* 타임레이블 위치 잡을 방법 구상필요
	// ********* 타임레이블 위치 잡을 방법 구상필요
	m_TimerLabel->setPosition(Vec2(winSize.width / 10 ,winSize.height / 1.1));
	m_TimerLabel->setSystemFontSize(SystemFontSize);
	m_TimerLabel->setVisible(false);
	this->addChild(m_TimerLabel);

	return true;
}

bool CTimerLayer::getGameStart() const {
	return m_GameStart;
}

void CTimerLayer::setGameStart(bool b) {
	m_GameStart = b;
}

void CTimerLayer::setCountDownVisible(bool b) {
	m_CountDownLabel->setVisible(b);
}

void CTimerLayer::setTimerVisible(bool b) {
	m_TimerLabel->setVisible(b);
}

bool CTimerLayer::UpdateCountDown(float time) {
	if (m_CountDown > 0) {
		m_CountDown -= time;
		char str[4];
		sprintf_s(str, sizeof(str), "%d", (int)roundf(m_CountDown));
		m_CountDownLabel->setString(str);
		if (m_CountDown < 0)
			return m_GameStart = true;
	}
	return false;
}

void CTimerLayer::UpdateTimer(float time) {
	// 제한시간 
	if (m_Timer > 0) {
		m_Timer -= time;
		char str[4];
		sprintf_s(str, sizeof(str), "%d", (int)roundf(m_Timer));
		m_TimerLabel->setString(str);
	}
	// 제한시간 초과시
	else if(m_Timer <= 0){
		throw (int)GameState::TimeOver;
	}
}

void CTimerLayer::resetTimerLayer() {
	resetTimer();
	m_CountDown = CountDown;
	m_GameStart = false;
	m_CountDownLabel->setVisible(false);
	m_TimerLabel->setVisible(false);
}

void CTimerLayer::resetTimer() {
	m_Timer = FallBackTime;
	//UpdateTimer(0.0);
}