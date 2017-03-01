#define DEBUG_MODE 1

#include "PlayScene\PlayScene.h"
#include "PlayScene\StoneLayer.h"
#include "PlayScene\UILayer.h"
#include "PlayScene\TimerLayer.h"
#include "DataManager.h"

using namespace cocos2d;

CPlayScene::CPlayScene() : player_stone_color(Stone::emptied), now_turn(Stone::black),
pause_check(true), game_start_check(false) {}
CPlayScene::~CPlayScene() {
	this->saveData();
	CC_SAFE_DELETE(data_manager);
}

bool CPlayScene::init() {
	if (!Scene::init())
		return false;

	// 화면 전환 효과
	LayerColor* layer = LayerColor::create(Color4B(0, 0, 0, 0));
	this->addChild(layer, 10);
	FadeOut* fadeout = FadeOut::create(0.5f);
	layer->runAction(fadeout);

	// 마우스 이벤트 추가
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true); // 클릭 정확도 향상
	listener->onTouchBegan = CC_CALLBACK_2(CPlayScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CPlayScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(CPlayScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	float scale = createBoard();
	Size size = this->getChildByName("base")->getContentSize();
	// 레이어 추가
	stone_layer = CStoneLayer::create();
	stone_layer->calculationBoardSize(size, scale);
	this->addChild(stone_layer, 2);
	
	timer_layer = CTimerLayer::create();
	timer_layer->setRunActionrFuncs(std::bind(&CPlayScene::runActionGameUpdate, this),
									std::bind(&CPlayScene::runActionTimeOver, this));
	timer_layer->setPauseCheck(&pause_check);
	this->addChild(timer_layer, 3);
	// UI는 가장 위에 보인다.
	ui_layer = CUILayer::create();
	ui_layer->setRunActionrCountDownFunc(std::bind(&CPlayScene::runActionCountDown, this));
	ui_layer->setRunActionrRestartFunc(std::bind(&CPlayScene::runActionRestart, this));
	ui_layer->setPauseCheck(&pause_check);
	this->addChild(ui_layer, 4);

	data_manager = new CDataManager();
	Document d = data_manager->loadData();
	if (!d["open"].GetBool()) {
		ui_layer->setSoundOption(true, true);
		return true;
	}
	// 효과음 조절
	// 버튼 표현방법 변경
	ui_layer->setSoundOption(d["bgm"].GetBool(), d["effect"].GetBool());

	return true;
}

float CPlayScene::createBoard() {
	Sprite* base_sprite = Sprite::create("etc/Base.jpg");
	Size winsize = Director::getInstance()->getWinSize();
	Size basesize = base_sprite->getContentSize();

	float widthscale = winsize.width / basesize.width;
	float heightscale = winsize.height / basesize.height;
	float scale = widthscale > heightscale ? heightscale : widthscale;

	base_sprite->setAnchorPoint(Vec2(0.5, 0.5));
	base_sprite->setScale(scale);
	base_sprite->setPosition(winsize / 2);
	this->addChild(base_sprite, 1, "base");
	return scale;
}

void CPlayScene::runActionGameUpdate() {
	game_start_check = true;
	this->schedule(schedule_selector(CPlayScene::gameUpdate), 1.0f);
}

void CPlayScene::unscheduleGameUpdate() {
	this->unschedule(schedule_selector(CPlayScene::gameUpdate));
}

void CPlayScene::runActionTimeOver() {
	this->endGame(GameState::time_over);
}

void CPlayScene::runActionCountDown() {
	pause_check = false;
	computer_stone_color = oppositionColor(player_stone_color = ui_layer->getPlayerColor());
	timer_layer->runActionCountDown();
}

void CPlayScene::runActionRestart() {
	game_start_check = false;
	ui_layer->initUILayer();
	timer_layer->initTimerLayer();
	stone_layer->initStoneLayer();
	now_turn = Stone::black;
}

void CPlayScene::runActionComputer() {
	try {
		// AI 행동 함수 호출
		stone_layer->createStone(computer_stone_color);
		now_turn = oppositionColor(computer_stone_color);
	}
	// 승리처리
	catch (GameState e) {
		if (e == GameState::black_win || e == GameState::white_win)
			endGame(e);
#ifdef DEBUG_MODE == 1
		else if (e == GameState::error) {
			CCLOG("************ Computer Fail");
#endif
		}
	}
}

void CPlayScene::gameUpdate(const float dt) {
	if (!pause_check && now_turn == computer_stone_color && game_start_check) {
		runActionComputer();	// 컴퓨터 턴
		timer_layer->resetTimer();
	}
}

Stone CPlayScene::oppositionColor(const Stone &s) const {
	return s == Stone::black ? Stone::white : Stone::black;
}

void CPlayScene::saveData() const {
	data_manager->saveData("/bgm", ui_layer->getBGMState());
	data_manager->saveData("/effect", ui_layer->getEffectState());
	data_manager->outputData();
}

void CPlayScene::endGame(const GameState &s) {
	pause_check = true;
	game_start_check = false;
	this->unscheduleGameUpdate();
	timer_layer->stopTimer();
	ui_layer->createGaemResult(s);
}

bool CPlayScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	return true;
}
void CPlayScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {}
void CPlayScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
	try {
		// 게임이 정지했으면, 유저 색상과 현재 진행턴이 틀리면 착수 실패	
		if (!pause_check && now_turn == player_stone_color && game_start_check) {
			// 클릭한 좌표와 유저의 색상으로 바둑돌 생성 함수를 호출함
			stone_layer->calculationPosition(touch->getLocation(), player_stone_color);
			// 착수 성공하면 유저의 반대색으로 턴을 바꿈
			now_turn = oppositionColor(player_stone_color);
			timer_layer->resetTimer();
		}
	}
	catch (GameState e) {
		// 정지, 상대방 턴, 게임 시작 전, 게임 승리
		if (e == GameState::black_win || e == GameState::white_win)
			endGame(e);
	}
}