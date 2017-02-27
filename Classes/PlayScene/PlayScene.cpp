#include "PlayScene\PlayScene.h"
#include "PlayScene\StoneLayer.h"
#include "PlayScene\UILayer.h"
#include "PlayScene\TimerLayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;


// Json 사용을 위한 참조
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"
#include "rapidjson/pointer.h"

using namespace rapidjson;
using namespace std;


CPlayScene::CPlayScene() : m_PlayerColor(Stone::Emptied), 
						   m_Turn(Stone::Black)  {}
CPlayScene::~CPlayScene() {
	DataSave();
}


void CPlayScene::DataLoad() {
	ifstream ifs("data/optiondata.json");
	IStreamWrapper isw(ifs);
	Document d;
	d.ParseStream(isw);
	assert(d.HasMember("BGM"));
	assert(d.HasMember("Effect"));
	CCLOG("DataLoad, d[BGM].IsBool() : %s", d["BGM"].IsBool() ? "true" : "false");
}
void CPlayScene::DataSave() {
	Document doc;
	Pointer("/BGM").Set(doc, m_UiLayer->getBGMState());
	Pointer("/Effect").Set(doc, m_UiLayer->getEffectState());
	
	// 출력하는 부분
	ofstream ofs("data/optiondata.json");
	OStreamWrapper osw(ofs);
	Writer<OStreamWrapper> writer(osw);
	doc.Accept(writer);
	// rapidjson 튜토리얼
	// https://www.slideshare.net/PaulKim81/rapid-json-tutorial?ref=http://jacking.tistory.com/1431
	// http://rapidjson.org/md_doc_pointer.html Json Pointer로 추가 하는 부분
	// http://rapidjson.org/md_doc_stream.html  입 출력 부분

//#include "rapidjson\stringbuffer.h"
//#include "rapidjson\prettywriter.h"
	//StringBuffer buffer;
	//PrettyWriter<StringBuffer> writer(buffer);		// 줄 바꿈 있음
	//doc.Accept(writer);
	//Writer<StringBuffer> w(buffer);					// 줄 바꿈 없음
	//doc.Accept(w);
	//auto os = std::ofstream("data/test1", std::ofstream::out);
	//os << buffer.GetString() << std::endl;
}

bool CPlayScene::init() {
	if (!Scene::init())
		return false;

	DataLoad();

	// 화면 전환 효과
	LayerColor* layer = LayerColor::create(Color4B(0, 0, 0, 0));
	this->addChild(layer, 10);
	FadeOut* fout = FadeOut::create(0.5f);
	layer->runAction(fout);

	// 마우스 이벤트 추가
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true); // 클릭 정확도 향상
	listener->onTouchBegan = CC_CALLBACK_2(CPlayScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CPlayScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(CPlayScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// 레이어 추가
	m_StoneLayer = CStoneLayer::create();
	this->addChild(m_StoneLayer, 1);
	m_TimerLayer = CTimerLayer::create();
	this->addChild(m_TimerLayer, 2);
	// UI는 가장 위에 보인다.
	m_UiLayer = CUILayer::create();
	this->addChild(m_UiLayer, 3);
	
	// 0.1초에 한번씩 게임 업데이트를 동작시킨다.
	this->schedule(schedule_selector(CPlayScene::gameupdate), 0.1f);
	
	return true;
}

void CPlayScene::gameupdate(const float dt) {
	// 게임 재시작 검사
	if (m_UiLayer->getReStart()) {
		m_UiLayer->ResetUILayer();
		m_TimerLayer->resetTimerLayer();
		m_StoneLayer->initStone();
		m_Turn = Stone::Black;
	}

	if (!m_UiLayer->getPause()) {
		// 게임이 시작했는지 검사한다.
		if (m_TimerLayer->getGameStart()) {
			m_PlayerColor = static_cast<Stone>(m_UiLayer->getPlayerColor());
			m_ComputerColor = m_PlayerColor == Stone::Black ? Stone::White : Stone::Black;

			// 플레이어 턴
			if (m_PlayerColor == m_Turn) {
				// 제한시간 표현
				try {
					m_TimerLayer->setTimerVisible(true);
					m_TimerLayer->UpdateTimer(dt);
				}
				catch (int e) {
					if (e == GameState::TimeOver) {
						m_UiLayer->GaemResult(GameState::TimeOver);
						CCLOG("************ Time Over *********");
					}
				}
			}
			// 컴퓨터 턴
			else if (m_Turn == m_ComputerColor) {
				try {
					// AI 행동 함수 호출
					m_StoneLayer->CreateStone(m_ComputerColor);
					EffectSound();
					m_Turn = m_ComputerColor == Stone::Black ? Stone::White : Stone::Black;
					// 타이머 초기화
					m_TimerLayer->resetTimer();
				}
				// 승리처리
				catch (int e) {
					if (e == GameState::Win)
						EndGame();
					else if (e == 1) {
						CCLOG("***************** Computer Fail");
					}
				}
			}
		}
		// 게임 시작 안 했으면
		else if (!m_TimerLayer->getGameStart()) {
			// 만들어둔 카운트 다운을 보이게 하고
			m_TimerLayer->setCountDownVisible(true);

			// 카운트 다운을 업데이트해서 시간이 흐르게 한다.
			if (m_TimerLayer->UpdateCountDown(dt)) {
				// 카운트 다운이 끝나면 오목을 두는 제한시간을 보이게 하고
				// 카운트 다운을 감춘다.
				m_TimerLayer->setTimerVisible(true);
				m_TimerLayer->setCountDownVisible(false);
			}
		}
	}
}

void CPlayScene::EffectSound() {
	if (m_UiLayer->getEffectState()) {
		SimpleAudioEngine::getInstance()->playEffect("ui-sound8.wav");
	}
}

void CPlayScene::EndGame() {
	m_UiLayer->GaemResult(m_Turn);
}

bool CPlayScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	return true;
}
void CPlayScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {}
void CPlayScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
	try {
		// 게임이 정지했거나, 시작하지 않았으면 착수 실패
		if (m_UiLayer->getPause() || !m_TimerLayer->getGameStart())
			throw (int)GameState::error;
		// 유저 색상과 현재 진행턴이 틀리면 착수 실패
		if (m_PlayerColor != m_Turn)
			throw (int)GameState::error;

		// 클릭한 좌표와 유저의 색상으로 바둑돌 생성 함수를 호출함
		m_StoneLayer->PosCalculation(touch->getLocation(), m_PlayerColor);
		EffectSound();
		// 착수 성공하면 유저의 반대색으로 턴을 바꿈
		m_Turn = m_PlayerColor == Stone::Black ? Stone::White : Stone::Black;
	}
	catch (int e) {
		// 정지, 상대방 턴, 게임 시작 전, 게임 승리
		if (e == GameState::Win)
			EndGame();
		else if (e == 1){
			CCLOG("*****************onTouchEnded Fail");
		}
	}
}