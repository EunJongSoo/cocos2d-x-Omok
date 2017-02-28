#ifndef _COMPUTER_H_
#define _COMPUTER_H_

#include "cocos2d.h"

class CStoneLayer;

class CComputer {
public:
	CComputer();
	~CComputer();
	cocos2d::Vec2 computerTurn(cocos2d::Node* node) const;
	void calculationScore(cocos2d::Node* node);

private:
	int calculationPointMaxScore(CStoneLayer* stone_layer, 
									int x, int y);

private:
	int count1;						// ********** 디버그용 변수
};
#endif
