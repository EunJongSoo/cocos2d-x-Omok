#ifndef _COMPUTER_H_
#define _COMPUTER_H_

#include "cocos2d.h"

class CComputer {
public:
	CComputer();
	~CComputer();
	cocos2d::Vec2 ComputerTurn(cocos2d::Node* node);
	void CalculationScore(cocos2d::Node* node);

private:
	int previous_color[4];			// 이전 색상 저장하는 변수 2는 비었다는 의미
	int count1;						// ********** 디버그용 변수
};
#endif
