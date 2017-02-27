#ifndef _STONE_LAYER_H_
#define _STONE_LAYER_H_

#include "cocos2d.h"
#include "StoneSprite.h"
#include "enumHeader.h"

class CComputer;

class CStoneLayer : public cocos2d::Layer {
private:
	CComputer* m_Computer;		// 컴퓨터 멤버변수

	float m_fScale;
	float m_fMarginPixel;		// 바둑판 여백 픽셀
	float m_fIntervalPixel;		// 바둑판 점 간격 픽셀
	int m_iCheckCount;			// ********오목 체크, 멤버가 좋은가, 함수간 인자로 주는게 좋은가?
	int m_iCheckColor;			// 색상 저장 변수 ********** 상동**********
	bool m_bCheckWin;			// 승리 체크
	cocos2d::Vec2 m_StrPos;
	cocos2d::Vec2 m_EndPos;

	// ************* 디버그용
	// ************* 디버그용
	// ************* 디버그용
	int count;

private:
	void CreateBoard();
	void CheckBoard(const int s);
	void CheckWidth();
	void CheckHeight();
	void CheckSlash();
	void CheckBackSlash();
	void CheckStone(const int x, const int y);
public:
	enum Direction {
		Up, Down, Left, Right,
		UpperLeft, UpperRight, LowerLeft, LowerRight, DirCount
	};
	enum Min_MaxXY {
		eMaxX, eMinX, eMaxY, eMinY
	};
	
	std::vector<cocos2d::Label*> labelvec;
	int m_iXY[4] = { 10,8,10,8 };

public:
	CStoneLayer();
	virtual ~CStoneLayer();
	virtual bool init();
	CREATE_FUNC(CStoneLayer);

	void initStone();
	void CreateStone(int s);
	void CreateStone(const cocos2d::Vec2 &pos, int s);
	void PosCalculation(const cocos2d::Vec2 &pos, int s);
	CStoneSprite* getSprite(int x, int y) const;
	CStoneSprite* SearcharoundSprite(int x, int y, int dir, int depth) const;
};
#endif 