#ifndef _STONE_LAYER_H_
#define _STONE_LAYER_H_

#include "cocos2d.h"
#include "StoneSprite.h"
#include "enumHeader.h"

class CComputer;

class CStoneLayer : public cocos2d::Layer {
public:
	CStoneLayer();
	virtual ~CStoneLayer();
	virtual bool init();
	CREATE_FUNC(CStoneLayer);

	void initStoneLayer();
	void createStone(const Stone s);
	void createStone(const cocos2d::Vec2 &pos, const Stone s);
	void calculationPosition(const cocos2d::Vec2 &pos, const Stone s);
	void calculationBoardSize(const cocos2d::Size &basesize, const float scale);
	CStoneSprite* getSprite(const int x, const int y);
	CStoneSprite* searchAroundSprite(const float x, const float y, const int dir, const int depth);

public:
	enum Direction {
		up, down, left, right,
		upper_left, upper_right, lower_left, lower_right, dir_count
	};
	enum Min_MaxXY {
		max_x, min_x, max_y, min_y
	};

	std::vector<cocos2d::Label*> labelvec;
	int min_max_xy_position[4];							// Min_MaxXY 값으로 접근

private:
	GameState checkWinColor();
	void checkBoard(const Stone s);
	void checkStone(const int x, const int y);
	void checkWidth();
	void checkHeight();
	void checkSlash();
	void checkBackSlash();
	void activeAroundStone(const cocos2d::Vec2 &pos);

private:
	const int board_size_x;
	const int board_size_y;
	float margin_pixel;			// 바둑판 여백 픽셀
	float interval_pixel;			// 바둑판 점 간격 픽셀

	CComputer* computer;				// 컴퓨터 멤버변수
	float scale;						// 전체 이미지 스케일값
	int check_count;					// ********오목 체크, 멤버가 좋은가, 함수간 인자로 주는게 좋은가?
	Stone check_stone_color;			// 색상 저장 변수 ********** 상동**********
	bool check_win;						// 승리 체크
	cocos2d::Vec2 str_position;			// 바둑판 시작점
	cocos2d::Vec2 end_position;			// 파둑판 끝점
	
	int count;							// ************* 디버그용
};
#endif 