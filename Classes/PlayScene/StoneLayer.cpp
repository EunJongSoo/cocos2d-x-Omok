#include "StoneLayer.h"
#include "Computer.h"

using namespace cocos2d;

const float fMarginPixel = 11.0;
const float fIntervalPixel = 21.0;

const int sizeX = 19;
const int sizeY = 19;

CStoneLayer::CStoneLayer() : m_bCheckWin(0) {}
CStoneLayer::~CStoneLayer() {
	// ********** 안전삭제 이거 맞는지 확인 필요함
	// ********** 안전삭제 이거 맞는지 확인 필요함
	// ********** 안전삭제 이거 맞는지 확인 필요함
	CC_SAFE_DELETE(m_Computer);
}

bool CStoneLayer::init() {
	if (!Layer::init())
		return false;
	count = 0;
	// 컴퓨터 동적할당
	m_Computer = new CComputer;
	// 바둑판을 생성한다.
	initStone();
	return true;
}

void CStoneLayer::initStone() {
	m_bCheckWin = false;
	for (int i = 0; i < 2; ++i) {
		m_iXY[i] = 10;
		m_iXY[i+1] = 8;
	}
	this->removeAllChildren();
	labelvec.clear();
	CreateBoard();
	// 바둑돌을 만들어서 추가하며, 좌표값을 Tag로 붙여둔다.
	// 초기 생성시 바둑돌은 빈돌이다.
	for (int y = 0; y < sizeY; ++y) {
		for (int x = 0; x < sizeX; ++x) {
			CStoneSprite* sprite = CStoneSprite::create();
			this->addChild(sprite, 1, sizeY * y + x);

			// ****** 디버그용 점수를 보기위한 레이블	
			// ****** 디버그용 점수를 보기위한 레이블	
			// ****** 디버그용 점수를 보기위한 레이블	
			float posX = x * m_fIntervalPixel + m_fMarginPixel + m_StrPos.x;
			float posY = y * m_fIntervalPixel + m_fMarginPixel + m_StrPos.y;

			Label* label = Label::create();
			label->setPosition(posX, posY);
			label->setSystemFontSize(25);
			label->setString("0");
			label->setVisible(false);
			this->addChild(label, 10);
			labelvec.push_back(label);
		}
	}
}

void CStoneLayer::CreateBoard() {
	Sprite* base = Sprite::create("Base.jpg");

	// 화면 크기를 가져오서 저장
	Size winsize = Director::getInstance()->getWinSize();
	
	// 바둑판 이미지의 크기를 가저와서 저장
	Size basesize = base->getContentSize();

	// 게임 화면의 가로 또는 세로의 크기에 맞게 바둑판을 늘리기 위해서
	// 화면의 가로, 세로 값을 바둑판 크기로 나눠서 배율을 구함
	float widthscale = winsize.width / basesize.width;
	float heightscale = winsize.height / basesize.height;
	
	// 배율이 작은 값을 저장함
	m_fScale = widthscale < heightscale ? widthscale : heightscale;

	// 배율에 맞춰 바둑판 크기를 저장함
	basesize = basesize * m_fScale;

	// 바둑판 픽셀 값도 배율에 맞춰서 저장함
	m_fMarginPixel = fMarginPixel * m_fScale;
	m_fIntervalPixel = fIntervalPixel * m_fScale;

	// 엥커 위치와 스케일을 변경해주고 위치는 화면의 중앙으로 잡아서 추가한다.
	base->setAnchorPoint(Vec2(0.5, 0.5));
	base->setScale(m_fScale);
	base->setPosition(winsize / 2);
	addChild(base, 0);

	// 바둑판 시작점과, 끝점의 값을 저장해둔다.
	// 바둑돌을 둘때 사용한다.
	m_StrPos = Vec2(base->getPosition().x - basesize.width / 2,
		base->getPosition().y - basesize.height / 2);
	m_EndPos = Vec2(base->getPosition().x + basesize.width / 2,
		base->getPosition().y + basesize.height / 2);
}

void CStoneLayer::PosCalculation(const Vec2 &pos, int s) {
	// 바둑돌을 착수한 위치가 바둑판 바깥쪽이면 착수 실패
	if (pos < m_StrPos || pos > m_EndPos) {
		CCLOG("*************StoneLayer.cpp PosCalculation, Fail");
		throw (int)GameState::error;
	}

	// 좌표 계산
	int x = round((pos.x - m_fMarginPixel - m_StrPos.x) / m_fIntervalPixel);
	int y = round((pos.y - m_fMarginPixel - m_StrPos.y) / m_fIntervalPixel);
	this->CreateStone(Vec2(x,y), s);
}

void CStoneLayer::CreateStone(int s) {
	m_Computer->CalculationScore(this);
	CreateStone(m_Computer->ComputerTurn(this), s);
}

void CStoneLayer::CreateStone(const Vec2 &pos, int s) {
	// 해당 좌표의 돌을 불러온다.
	CStoneSprite* pSprite = getSprite(pos.x, pos.y);

	// 불러온 돌의 종류를 확인해서 돌이 있으면 착수 실패
	if (pSprite->getStoneType() != Stone::Emptied) {
		CCLOG("******* StoneLayer.cpp CreateStone, Fail");
		throw (int)GameState::error;
	}

	// 비어있다면 좌표 값으로 착수 위치를 계산해서
	float posX = pos.x * m_fIntervalPixel + m_fMarginPixel + m_StrPos.x;
	float posY = pos.y * m_fIntervalPixel + m_fMarginPixel + m_StrPos.y;

	// 바둑돌을 추가한다.
	pSprite->initSprite(Vec2(posX, posY), m_fScale, s);

	// 착수된 지점은 점수계산이 필요없으므로 비활성화 시켜준다.
	pSprite->setActive(false);
	
	// 최대, 최소 좌표점 저장
	if (m_iXY[eMaxX] < pos.x) m_iXY[eMaxX] = pos.x;
	if (m_iXY[eMinX] > pos.x) m_iXY[eMinX] = pos.x;
	if (m_iXY[eMaxY] < pos.y) m_iXY[eMaxY] = pos.y;
	if (m_iXY[eMinY] > pos.y) m_iXY[eMinY] = pos.y;

	// 오목 여부를 검사한다.
	CheckBoard(s);

	// 주변 2칸을 활성화 시킨다.
	for (int dir = 0; dir < DirCount; ++dir){
		for (int depth = 1; depth <= 2; ++depth) {
			CStoneSprite* sprite = SearcharoundSprite(pos.x, pos.y, dir, depth);
			if (sprite != nullptr && sprite->getStoneType() == Stone::Emptied) {
				sprite->setActive(true);
			}
		}
	}
	
	// ****** 점수계산을 위해서 착수 후 함수 호출함
	// 컴퓨터 착수전에만 계산하면 효율적임
	// ****** 점수계산을 위해서 착수 후 함수 호출함
	// 컴퓨터 착수전에만 계산하면 효율적임
	m_Computer->CalculationScore(this);
}

CStoneSprite* CStoneLayer::getSprite(int x, int y) const {
	return dynamic_cast<CStoneSprite*>(this->getChildByTag(sizeY * y + x));
}

CStoneSprite* CStoneLayer::SearcharoundSprite(int x, int y, int dir, int depth) const {
	int ix = 0, iy = 0;

	switch (dir) {
	case Direction::Up: ix = 0, iy = 1; break;
	case Direction::Down: ix = 0, iy = -1; break;
	case Direction::Left: ix = -1, iy = 0; break;
	case Direction::Right: ix = 1, iy = 0; break;
	case Direction::UpperLeft: ix = -1, iy = 1; break;
	case Direction::UpperRight: ix = 1, iy = 1; break;
	case Direction::LowerLeft: ix = -1, iy = -1; break;
	case Direction::LowerRight: ix = 1, iy = -1; break;
	default: ix = 0, iy = 0; break;
	}

	int tx = ix * depth + x;
	int ty = iy * depth + y;
	// 좌표 값이 0보다 작거나 바둑판 보다 크면 검사하지 않는다.
	if (tx < 0 || tx > sizeX || ty < 0 || ty > sizeY) return nullptr;

	return this->getSprite(tx, ty);
}

void CStoneLayer::CheckBoard(const int s) {
	m_iCheckCount = 0;
	m_iCheckColor = s;
	bool check[2] = { 0, 0 };
	if (m_iXY[eMaxX] - m_iXY[eMinX] > 3) {
		CheckWidth();
		check[0] = true;
	}
	if (m_iXY[eMaxY] - m_iXY[eMinY] > 3) {
		CheckHeight();
		check[1] = true;
	}
	if (check[0] && check[1]) {
		CheckSlash();
		CheckBackSlash();
	}

	// ************* 디버그용
	// ************* 디버그용
	// ************* 디버그용
	CCLOG("count : %d\n", count);
	if (m_iCheckColor == 0)
		CCLOG("CheckBoard Complete Color : Black\n");
	else if (m_iCheckColor == 1)
		CCLOG("CheckBoard Complete Color : White\n");
}

void CStoneLayer::CheckStone(const int x, const int y) {
	
	// ************ 디버그용 계산 횟수 헤아리기
	// ************ 디버그용 계산 횟수 헤아리기
	// ************ 디버그용 계산 횟수 헤아리기
	++count;


	CStoneSprite* sprite = this->getSprite(x, y);
	if (sprite->getStoneType() == m_iCheckColor) {
		if (++m_iCheckCount == 5) {		
			m_bCheckWin = true;
		}
		else if (m_iCheckCount > 5) {
			m_bCheckWin = false;
		}
	}
	else m_iCheckCount = 0;
}

void CStoneLayer::CheckWidth() {
	// 가로 검사 좌에서 우로 검사, 끝나면 한줄 위로 이동
	for (int y = m_iXY[eMinY]; y <= m_iXY[eMaxY]; ++y) {
		for (int x = m_iXY[eMinX]; x <= m_iXY[eMaxX]; ++x) {
			CheckStone(x, y);
			//CCLOG("*****CheckWidth***** color : %d, count : %d", m_iCheckColor, m_iCheckCount);
		}
		m_iCheckCount = 0;
		//CCLOG("\n");
	}
	if (m_bCheckWin) throw (int)GameState::Win;
}

void CStoneLayer::CheckHeight() {
	// 세로 검사 밑에서 위로 검사, 끝나면 한줄 옆으로 이동
	for (int x = m_iXY[eMinX]; x <= m_iXY[eMaxX]; ++x) {
		for (int y = m_iXY[eMinY]; y <= m_iXY[eMaxY]; ++y) {
			CheckStone(x, y);
			//CCLOG("*****CheckHeight***** color : %d, count : %d", m_iCheckColor, m_iCheckCount);
		}
		m_iCheckCount = 0;
		//CCLOG("\n");
	}
	if (m_bCheckWin) throw (int)GameState::Win;
}

void CStoneLayer::CheckSlash() {
	// 대각선 검사 왼쪽위에서 오른쪽 아래 방향으로 검사 0, 19 ~ 19, 0으로
	for (int ix = m_iXY[eMinX], iy = m_iXY[eMaxY];
		ix <= m_iXY[eMaxX] && iy >= m_iXY[eMinY];
		ix == m_iXY[eMaxX] ? --iy : ++ix) {
		for (int y = iy, x = ix; x >= m_iXY[eMinX] && y >= m_iXY[eMinY]; --y, --x) {
			CheckStone(x, y);
			CCLOG("CheckSlash x:%d, y:%d, color:%d, count : %d", x, y, m_iCheckColor, m_iCheckCount);
		}
		m_iCheckCount = 0;
		CCLOG("\n");
	}
	if (m_bCheckWin) throw (int)GameState::Win;
}

void CStoneLayer::CheckBackSlash() {
	// 오목검사가 제대로 안됨 6목이 되어야 승리함
	for (int ix = m_iXY[eMinX], iy = m_iXY[eMinY];
		ix <= m_iXY[eMaxX] && iy <= m_iXY[eMaxY];
		ix == m_iXY[eMaxX] ? ++iy : ++ix) {
		for (int y = iy, x = ix; x >= m_iXY[eMinX] && y <= m_iXY[eMaxY]; ++y, --x) {
			CheckStone(x, y);
			//CCLOG("CheckBackSlash x:%d, y:%d, color:%d, count : %d", x, y, m_iCheckColor, m_iCheckCount);
		}
		m_iCheckCount = 0;
		//CCLOG("\n");
	}
	if (m_bCheckWin) throw (int)GameState::Win;
}