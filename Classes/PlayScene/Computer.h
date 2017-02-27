#ifndef _COMPUTER_H_
#define _COMPUTER_H_

#include "cocos2d.h"

class CComputer {
private:
	// 이전 색상 저장하는 변수 2는 비었다는 의미
	int PreviousColor[4] = { 2, 2, 2, 2 };
	
	// ********** 디버그용 변수
	// ********** 디버그용 변수
	// ********** 디버그용 변수
	// ********** 디버그용 변수
	int count1;

public:
	CComputer();
	~CComputer();
	cocos2d::Vec2 ComputerTurn(cocos2d::Node* node);
	void CalculationScore(cocos2d::Node* node);
};
#endif
