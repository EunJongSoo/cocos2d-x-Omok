#include "UILayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace ui;
using namespace CocosDenshion;

CUILayer::CUILayer() : m_Pause(true), m_State(true), m_bReStart(false),
m_bBGMState(false),m_bEffectState(true) {}
CUILayer::~CUILayer() {}

bool CUILayer::init() {
	if (!Layer::init())
		return false;
	// UI를 만드는 함수 호출
	this->CreateUI();

	// 배경음악 출력, 배경 음악이 꺼져있으면 배경음악을 실행
	if (!m_bBGMState) {
		m_bBGMState = true;
		SimpleAudioEngine::getInstance()->playBackgroundMusic("01_01_trial.mp3", true);
	}

	return true;
}

int CUILayer::getPlayerColor() const {
	return m_Player;
}

bool CUILayer::getPause() const {
	return m_Pause;
}

bool CUILayer::getReStart() const {
	return m_bReStart;
}

bool CUILayer::getBGMState() const {
	return m_bBGMState;
}

bool CUILayer::getEffectState() const {
	return m_bEffectState;
}

void CUILayer::CreateUI() {
	cocos2d::ui::Button* bt1 = ui::Button::create("grey_panel.png","yellow_panel.png","Black.png");
	bt1->setPosition(Vec2(400, 400));
	bt1->addTouchEventListener(CC_CALLBACK_2(CUILayer::bt1OnTouch, this));
	bt1->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(bt1, 1, "black");

	Button* bt2 = Button::create("grey_panel.png", "yellow_panel.png", "White.png");
	bt2->setPosition(Vec2(550, 400));
	bt2->addTouchEventListener(CC_CALLBACK_2(CUILayer::bt2OnTouch, this));
	bt2->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(bt2, 1, "white");
	
	Button* bt3 = Button::create("grey_panel.png", "yellow_panel.png", "Black.png");
	bt3->setPosition(Vec2(475, 250));
	bt3->addTouchEventListener(CC_CALLBACK_2(CUILayer::bt3OnTouch, this));
	bt3->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(bt3, 1, "selete");

	Size winSize = CCDirector::getInstance()->getWinSize();

	cocos2d::ui::Button* optionbt = ui::Button::create("grey_panel.png", "yellow_panel.png");
	optionbt->setPosition(Vec2(winSize.width / 1.1f, winSize.height / 1.1f));
	optionbt->addTouchEventListener(CC_CALLBACK_2(CUILayer::btOptionOnTouch, this));
	optionbt->setVisible(false);
	this->addChild(optionbt, 1, "option");

	Label* label = Label::create();
	label->setPosition(500, 500);
	label->setSystemFontSize(50);
	label->setColor(Color3B(255, 255, 255));
	label->setVisible(false);
	this->addChild(label, 5, "resultstr");

	CreateMenu();
	CreateSoundMenu();
}

void CUILayer::ResetUILayer() {
	m_Pause = true;
	m_bReStart = false;

	Button* bt1 = (Button*)this->getChildByName("black");
	Button* bt2 = (Button*)this->getChildByName("white");
	Button* bt3 = (Button*)this->getChildByName("selete");
	Button* bt4 = (Button*)this->getChildByName("option");
	Menu* menu = (Menu*)this->getChildByName("menu");
	Menu* soundmenu = (Menu*)this->getChildByName("soundmenu");
	Label* label = (Label*)this->getChildByName("resultstr");
	bt1->setVisible(true);
	bt2->setVisible(true);
	bt1->setEnabled(true);
	bt2->setEnabled(true);
	bt3->setVisible(true);
	bt4->setEnabled(true);
	bt4->setVisible(false);
	menu->setVisible(false);
	soundmenu->setVisible(false);
	label->setVisible(false);
}

void CUILayer::CreateMenu() {
	Label* label1 = Label::create();
	label1->setString("restart");
	label1->setSystemFontSize(50);
	label1->setColor(Color3B(255, 100, 100));
	Label* label2 = Label::create("sound", "", 50);
	label2->setColor(Color3B(100, 255, 100));
	Label* label3 = Label::create("gameoff", "", 50);
	label3->setColor(Color3B(0, 0, 0));

	MenuItemLabel* itemlabel1 = MenuItemLabel::create(label1, CC_CALLBACK_1(CUILayer::btRestartGameOnTouch, this));
	auto itemlabel2 = MenuItemLabel::create(label2, CC_CALLBACK_1(CUILayer::btSoundOnTouch, this));
	auto itemlabel3 = MenuItemLabel::create(label3, CC_CALLBACK_1(CUILayer::btOffGameOnTouch, this));

	Menu* menu = Menu::create(itemlabel1, itemlabel2, itemlabel3, NULL);
	menu->alignItemsVertically();
	menu->setVisible(false);
	this->addChild(menu, 4, "menu");
}

void CUILayer::CreateSoundMenu() {
	auto itemimg1_1 = MenuItemImage::create("BGM_1.png", "BGM_2.png");
	auto itemimg1_2 = MenuItemImage::create("BGM_2.png", "BGM_1.png");
	auto itemimg2_1 = MenuItemImage::create("Effect_1.png", "Effect_2.png");
	auto itemimg2_2 = MenuItemImage::create("Effect_2.png", "Effect_1.png");
	MenuItemToggle* toggle1 = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(CUILayer::BGM_ON_OFF, this), itemimg1_1, itemimg1_2, NULL);

	auto* toggle2 = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(CUILayer::Effect_ON_OFF, this), itemimg2_1, itemimg2_2, NULL);

	auto itemimg3 = MenuItemImage::create("Back_1.png", "Back_2.png",
		CC_CALLBACK_1(CUILayer::btSoundMenuBackOnTouch, this));
	
	Menu* soundmenu = Menu::create(toggle1, toggle2, itemimg3, NULL);
	soundmenu->alignItemsHorizontally();
	soundmenu->setVisible(false);
	this->addChild(soundmenu, 5, "soundmenu");
}

void CUILayer::CreateOptionPanel() {
	m_Pause = !m_Pause;
	// 어떤걸 써야 하는가?
	Size winSize = CCDirector::getInstance()->getWinSize();
	Size visibelSize = CCDirector::getInstance()->getVisibleSize();
	Vec2 origin = CCDirector::getInstance()->getVisibleOrigin();

	// 생성, 버튼을 한번 더 누르면 삭제 해야됨
	Menu* menu = (Menu*)this->getChildByName("menu");
	menu->setVisible(m_Pause);

	if (!m_Pause) {
		Menu* soundmenu = (Menu*)this->getChildByName("soundmenu");
		soundmenu->setVisible(m_Pause);
	}
	
}

void CUILayer::GaemResult(const int s) {
	// 승리 색상 표현
	std::string str;
	switch (s) {
	case GameState::BlackWin: str = "Black Win!!"; break;
	case GameState::WhiteWin: str = "White Win!!"; break;
	case GameState::TimeOver: str = "Time Over"; break;
	}
	
	Label* label = (Label*)this->getChildByName("resultstr");
	label->setString(str);
	label->setVisible(true);
	CreateOptionPanel();
	Button* bt4 = (Button*)this->getChildByName("option");
	bt4->setEnabled(false);
}

void CUILayer::ButtonEnabled(int color) {
	// 색상을 받아서 저장하고
	m_Player = color;
	
	// 색상이 검은색이면 상태값을 flase, 아니면 true를 저장한다.
	m_Player == Black ? m_State = false : m_State = true;

	// 상태값으로 버튼1과 버튼2의 상태를 반대로 보이도록 한다.
	Button* bt1 = (Button*)this->getChildByName("black");
	bt1->setEnabled(m_State);
	Button* bt2 = (Button*)this->getChildByName("white");
	bt2->setEnabled(!m_State);
}

void CUILayer::bt1OnTouch(Ref* sender, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::ENDED) {
		ButtonEnabled(Stone::Black);
	}
}

void CUILayer::bt2OnTouch(Ref* sender, Widget::TouchEventType type) {
	switch (type) {
	case Widget::TouchEventType::ENDED:
		ButtonEnabled(Stone::White);
	}
}

void CUILayer::bt3OnTouch(Ref* sender, Widget::TouchEventType type) {
	Button* bt1 = (Button*)this->getChildByName("black");
	Button* bt2 = (Button*)this->getChildByName("white");
	// 검은돌이나 백색돌을 선택 후 버튼3을 누르면
	// 화면에 띄운 버튼을 모두 숨기고 정지를 풀어준다.
	if (!bt1->isEnabled() || !bt2->isEnabled()) {
		switch (type) {
		case Widget::TouchEventType::ENDED:
			Button* bt3 = (Button*)this->getChildByName("selete");
			bt1->setVisible(false);
			bt2->setVisible(false);
			bt3->setVisible(false);
			m_Pause = false;
			
			Button* bt4 = (Button*)this->getChildByName("option");
			bt4->setVisible(true);
		}
	}
}

void CUILayer::btRestartGameOnTouch(Ref* sender) {
	m_bReStart = true;
}

void CUILayer::btSoundOnTouch(Ref* sender) {
	Menu* menu = (Menu*)this->getChildByName("menu");
	menu->setVisible(false);
	Menu* soundmenu = (Menu*)this->getChildByName("soundmenu");
	soundmenu->setVisible(true);
}

void CUILayer::btSoundMenuBackOnTouch(Ref* sender) {
	Menu* menu = (Menu*)this->getChildByName("menu");
	menu->setVisible(true);
	Menu* soundmenu = (Menu*)this->getChildByName("soundmenu");
	soundmenu->setVisible(false);
}

void CUILayer::BGM_ON_OFF(Ref* sender) {
	if (m_bBGMState) {
		m_bBGMState = false;
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		return;
	}
	m_bBGMState = true;
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void CUILayer::Effect_ON_OFF(Ref* sender) {
	m_bEffectState = !m_bEffectState;
}

void CUILayer::btOffGameOnTouch(Ref* sender) {
	CCDirector::getInstance()->end();
}

void CUILayer::btOptionOnTouch(Ref* sender, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::ENDED) {
		CreateOptionPanel();
	}
}