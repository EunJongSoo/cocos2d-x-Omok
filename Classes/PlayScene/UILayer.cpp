#include "UILayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace ui;
using namespace CocosDenshion;

CUILayer::CUILayer() : pause_check(true), /*selete_color_check(false),*/state_check(true), /*restart_check(false),*/
bgm_state_check_check(false),effect_State_check(true) {
	RunActionrCountDown = NULL;
}
CUILayer::~CUILayer() {}

void CUILayer::setRunActionrCountDownFunc(std::function<void(void)> countdown) {
	RunActionrCountDown = countdown;
}

void CUILayer::setRunActionrPauseFunc(std::function<void(void)> pause) {
	RunActionrPause = pause;
}

void CUILayer::setRunActionrRestartFunc(std::function<void(void)> restart) {
	RunActionrRestart = restart;
}

bool CUILayer::init() {
	if (!Layer::init())
		return false;
	this->CreateUI();
	return true;
}

void CUILayer::initUILayer() {
	pause_check = true;
	//restart_check = false;
	//selete_color_check = false;

	Button* bt1 = (Button*)this->getChildByName("black");
	Button* bt2 = (Button*)this->getChildByName("white");
	Button* bt3 = (Button*)this->getChildByName("selete");
	Button* bt4 = (Button*)this->getChildByName("option");
	Menu* menu = (Menu*)this->getChildByName("menu");
	Menu* soundmenu = (Menu*)this->getChildByName("soundmenu");
	Label* label = (Label*)this->getChildByName("resultstr");
	bt1->setVisible(true);
	bt1->setEnabled(true);
	bt2->setVisible(true);
	bt2->setEnabled(true);
	bt3->setVisible(true);
	bt4->setEnabled(true);
	bt4->setVisible(false);
	menu->setVisible(false);
	soundmenu->setVisible(false);
	label->setVisible(false);
}

void CUILayer::createGaemResult(const GameState s) {
	// 승리 색상 표현
	std::string str;
	switch (s) {
	case GameState::black_win: str = "Black Win!!"; break;
	case GameState::white_win: str = "White Win!!"; break;
	case GameState::time_over: str = "Time Over"; break;
	}

	Label* label = (Label*)this->getChildByName("resultstr");
	label->setString(str);
	label->setVisible(true);
	createOptionMenu();
	Button* bt4 = (Button*)this->getChildByName("option");
	bt4->setEnabled(false);
}

void CUILayer::CreateUI() {
	cocos2d::ui::Button* bt1 = ui::Button::create("Button/grey_panel.png","Button/yellow_panel.png","etc/Black.png");
	bt1->setPosition(Vec2(400, 400));
	bt1->addTouchEventListener(CC_CALLBACK_2(CUILayer::onTouchBlackSeleteButton, this));
	bt1->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(bt1, 1, "black");

	Button* bt2 = Button::create("Button/grey_panel.png", "Button/yellow_panel.png", "etc/White.png");
	bt2->setPosition(Vec2(550, 400));
	bt2->addTouchEventListener(CC_CALLBACK_2(CUILayer::onTouchWhiteSeleteButton, this));
	bt2->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(bt2, 1, "white");
	
	Button* bt3 = Button::create("Button/grey_panel.png", "Button/yellow_panel.png");
	bt3->setPosition(Vec2(475, 250));
	bt3->addTouchEventListener(CC_CALLBACK_2(CUILayer::onTouchColorCheckButton, this));
	bt3->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(bt3, 1, "selete");

	Size winSize = CCDirector::getInstance()->getWinSize();

	Button* optionbt = ui::Button::create("Button/grey_panel.png", "Button/yellow_panel.png");
	optionbt->setPosition(Vec2(winSize.width / 1.1f, winSize.height / 1.1f));
	optionbt->addTouchEventListener(CC_CALLBACK_2(CUILayer::onTouchOptionMenuButton, this));
	optionbt->setVisible(false);
	this->addChild(optionbt, 1, "option");

	Label* label = Label::create();
	label->setPosition(500, 500);
	label->setSystemFontSize(50);
	label->setColor(Color3B(255, 255, 255));
	label->setVisible(false);
	this->addChild(label, 5, "resultstr");

	createMenu();
	createSoundMenu();
}

void CUILayer::createMenu() {
	Label* label1 = Label::create();
	label1->setString("restart");
	label1->setSystemFontSize(50);
	label1->setColor(Color3B(255, 100, 100));
	Label* label2 = Label::create("sound", "", 50);
	label2->setColor(Color3B(100, 255, 100));
	Label* label3 = Label::create("gameoff", "", 50);
	label3->setColor(Color3B(0, 0, 0));

	MenuItemLabel* itemlabel1 = MenuItemLabel::create(label1, CC_CALLBACK_1(CUILayer::onTouchRestartGameButton, this));
	auto itemlabel2 = MenuItemLabel::create(label2, CC_CALLBACK_1(CUILayer::onTouchSoundMenuButton, this));
	auto itemlabel3 = MenuItemLabel::create(label3, CC_CALLBACK_1(CUILayer::onTouchQuitButton, this));

	Menu* menu = Menu::create(itemlabel1, itemlabel2, itemlabel3, NULL);
	menu->alignItemsVertically();
	menu->setVisible(false);
	this->addChild(menu, 4, "menu");
}

void CUILayer::createSoundMenu() {
	auto itemimg1_1 = MenuItemImage::create("Button/BGM_1.png", "Button/BGM_2.png");
	auto itemimg1_2 = MenuItemImage::create("Button/BGM_2.png", "Button/BGM_1.png");
	auto itemimg2_1 = MenuItemImage::create("Button/Effect_1.png", "Button/Effect_2.png");
	auto itemimg2_2 = MenuItemImage::create("Button/Effect_2.png", "Button/Effect_1.png");
	MenuItemToggle* toggle1 = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(CUILayer::onTouchBGM_ON_OFF_Toggle, this), itemimg1_1, itemimg1_2, NULL);

	auto* toggle2 = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(CUILayer::onToucheffect_ON_OFF_Toggle, this), itemimg2_1, itemimg2_2, NULL);

	auto itemimg3 = MenuItemImage::create("Button/Back_1.png", "Button/Back_2.png",
		CC_CALLBACK_1(CUILayer::onTouchSoundMenuBackButton, this));

	Menu* soundmenu = Menu::create(toggle1, toggle2, itemimg3, NULL);
	soundmenu->alignItemsHorizontally();
	soundmenu->setVisible(false);
	this->addChild(soundmenu, 5, "soundmenu");
}

void CUILayer::createOptionMenu() {
	pause_check = !pause_check;
	Menu* menu = (Menu*)this->getChildByName("menu");
	menu->setVisible(pause_check);
	if (!pause_check) {
		Menu* soundmenu = (Menu*)this->getChildByName("soundmenu");
		soundmenu->setVisible(pause_check);
	}
}

void CUILayer::enabledColorSeleteButton(const Stone color) {
	// 색상을 받아서 저장하고
	player_color = color;
	
	// 색상이 검은색이면 상태값을 flase, 아니면 true를 저장한다.
	player_color == Stone::black ? state_check = false : state_check = true;

	// 상태값으로 버튼1과 버튼2의 상태를 반대로 보이도록 한다.
	Button* bt1 = (Button*)this->getChildByName("black");
	bt1->setEnabled(state_check);
	Button* bt2 = (Button*)this->getChildByName("white");
	bt2->setEnabled(!state_check);
}

void CUILayer::onTouchBlackSeleteButton(Ref* sender, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::ENDED) {
		enabledColorSeleteButton(Stone::black);
	}
}

void CUILayer::onTouchWhiteSeleteButton(Ref* sender, Widget::TouchEventType type) {
	switch (type) {
	case Widget::TouchEventType::ENDED:
		enabledColorSeleteButton(Stone::white);
	}
}

void CUILayer::onTouchColorCheckButton(Ref* sender, Widget::TouchEventType type) {
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
			pause_check = false;
			Button* bt4 = (Button*)this->getChildByName("option");
			bt4->setVisible(true);
			RunActionrCountDown();
		}
	}
}

void CUILayer::onTouchOptionMenuButton(Ref* sender, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::ENDED) {
		createOptionMenu();
	}
}

void CUILayer::onTouchRestartGameButton(Ref* sender) {
	//restart_check = true;
	RunActionrRestart();
}

void CUILayer::onTouchSoundMenuButton(Ref* sender) {
	Menu* menu = (Menu*)this->getChildByName("menu");
	menu->setVisible(false);
	Menu* soundmenu = (Menu*)this->getChildByName("soundmenu");
	soundmenu->setVisible(true);
}

void CUILayer::onTouchQuitButton(Ref* sender) {
	CCDirector::getInstance()->end();
}

void CUILayer::onTouchBGM_ON_OFF_Toggle(Ref* sender) {
	if (bgm_state_check_check) {
		bgm_state_check_check = false;
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		return;
	}
	bgm_state_check_check = true;
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void CUILayer::onToucheffect_ON_OFF_Toggle(Ref* sender) {
	if (effect_State_check) {
		effect_State_check = false;
		SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
	}
	else {
		effect_State_check = true;
		SimpleAudioEngine::getInstance()->setEffectsVolume(0);
	}
}

void CUILayer::onTouchSoundMenuBackButton(Ref* sender) {
	Menu* menu = (Menu*)this->getChildByName("menu");
	menu->setVisible(true);
	Menu* soundmenu = (Menu*)this->getChildByName("soundmenu");
	soundmenu->setVisible(false);
}

Stone CUILayer::getPlayerColor() const {
	return player_color;
}

bool CUILayer::getPause() const {
	return pause_check;
}

//bool CUILayer::getSeleteColor() const {
//	return selete_color_check;
//}
//
//void CUILayer::setSeleteColor(bool b) {
//	selete_color_check = b;
//}

//bool CUILayer::getReStart() const {
//	return restart_check;
//}

bool CUILayer::getBGMState() const {
	return bgm_state_check_check;
}

bool CUILayer::getEffectState() const {
	return effect_State_check;
}

void CUILayer::setSoundOption(const bool bgm, const bool effect) {
	bgm_state_check_check = bgm;
	effect_State_check = effect;
	if (bgm && !SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/bgm/01_01_trial.mp3", true);
	}
	if (!effect) {
		SimpleAudioEngine::getInstance()->setEffectsVolume(0);
	}
}