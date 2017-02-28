#define DEBUG_MODE 0;
#define MAXVAL(i, j) i = j + 2 < 18 ? j + 2 : 18
#define MINVAL(i, j) i = j - 2 > 0 ? j - 2 : 0

#include "Computer.h"
#include "StoneLayer.h"
#include <time.h>

using namespace cocos2d;
using namespace std;

CComputer::CComputer() : count1(0) {
	srand((unsigned)time(NULL));
}

CComputer::~CComputer() {}

Vec2 CComputer::computerTurn(Node* node) const {
	CStoneLayer* stonelayer = dynamic_cast<CStoneLayer*>(node);
	// 전체 바둑판 19*19에 대해서 점수를 계산하고
	// 가장 점수가 높은 자리에 착수를 한다.
	int maxX = MAXVAL(maxX, stonelayer->min_max_xy_position[CStoneLayer::Min_MaxXY::max_x]);
	int minX = MINVAL(minX, stonelayer->min_max_xy_position[CStoneLayer::Min_MaxXY::min_x]);
	int maxY = MAXVAL(maxY, stonelayer->min_max_xy_position[CStoneLayer::Min_MaxXY::max_y]);
	int minY = MINVAL(minY, stonelayer->min_max_xy_position[CStoneLayer::Min_MaxXY::min_y]);

	vector<Vec2> vector;
	int iBestPointScore = 0;
	for (int y = minY; y <= maxY; ++y) {
		for (int x = minX; x <= maxX; ++x) {
			CStoneSprite* sprite = stonelayer->getSprite(x, y);
			if (sprite->getActive()) {
				if (sprite->getScore() > iBestPointScore) {
					iBestPointScore = sprite->getScore();
					vector.clear();
					vector.push_back(Vec2(x, y));
				}
				else if (sprite->getScore() == iBestPointScore) {
					vector.push_back(Vec2(x, y));
				}
			}
		}
	}
	if (vector.size() == 0) vector.push_back(Vec2(9, 9));
	return vector[rand() % vector.size()];
}

void CComputer::calculationScore(Node* node) {
	CStoneLayer* stone_layer = dynamic_cast<CStoneLayer*>(node);
	
	int max_x = MAXVAL(max_x, stone_layer->min_max_xy_position[CStoneLayer::Min_MaxXY::max_x]);
	int min_x = MINVAL(min_x, stone_layer->min_max_xy_position[CStoneLayer::Min_MaxXY::min_x]);
	int max_y = MAXVAL(max_y, stone_layer->min_max_xy_position[CStoneLayer::Min_MaxXY::max_y]);
	int min_y = MINVAL(min_y, stone_layer->min_max_xy_position[CStoneLayer::Min_MaxXY::min_y]);

	
	int point_max_score = 0;
	CStoneSprite* sprite;
	for (int y = min_y; y <= max_y; ++y) {
		for (int x = min_x; x <= max_x; ++x) {
			sprite = stone_layer->getSprite(x, y);
			if (sprite->getActive()) {
				point_max_score = calculationPointMaxScore(stone_layer, x, y);
				sprite->setScore(point_max_score);
#ifdef DEBUG_MODE == 1
				vector<Label*> labelvector = stone_layer->labelvec;
				Label* label = labelvector.at(19 * y + x);
				char str[4];
				sprintf_s(str, sizeof(str), "%d", point_max_score);
				label->setString(str);
				label->setColor(Color3B(250, 50, 50));
				label->setVisible(true);
				++count1;
#endif
			}
		}
	}
#ifdef DEBUG_MODE == 1
	CCLOG("ComputerTurn count1 : %d", count1);
#endif
}

int CComputer::calculationPointMaxScore(CStoneLayer* stone_layer, int x, int y) {
	CStoneSprite* sprite;
	int max_score[2] = { 0, };
	int score[2] = { 0, };
	Stone previous_color[4] = { Stone::emptied, };
	for (int dir = 0; dir < CStoneLayer::Direction::dir_count; ++dir) {
		for (int depth = 1; depth <= 4; ++depth) {
			sprite = stone_layer->searchAroundSprite(x, y, dir, depth);
			if (sprite == nullptr) break;
			Stone color = sprite->getStoneType();
			previous_color[depth - 1] = color;

			// 돌이 없다.
			if (color == Stone::emptied) {
				if (depth != 1) {
					// 2번 연속 비었는가?
					if (color == previous_color[depth - 2]) break;
					score[previous_color[depth - 2]] += (depth - 2);
				}
			}
			// 돌이 있다. 1~4
			else if (depth == 1) {
				score[color] += 1;
			}
			else {
				if (previous_color[depth - 1] == previous_color[depth - 2]) {
					score[color] += (depth*1.8f);
				}
				else if (depth > 2 && previous_color[depth - 1] == previous_color[depth - 3]) {
					score[color] += 1;
				}
				else if (previous_color[depth - 1] != Stone::emptied) {
					score[color] -= depth;
					break;
				}
			}
		}
		max_score[Stone::black] += score[Stone::black];
		max_score[Stone::white] += score[Stone::white];
		for (int i = 0; i < 4; ++i) {
			previous_color[i] = Stone::emptied;
		}
		score[Stone::black] = 0;
		score[Stone::white] = 0;
	}
	return (max_score[Stone::black] > max_score[Stone::white] ? max_score[Stone::black] : max_score[Stone::white]);
}

// 기존 계산 코드 주석처리
// 기존 계산 코드 주석처리
// 기존 계산 코드 주석처리
// 기존 계산 코드 주석처리

// 1.  첫번째 돌이 비었다.		* 검사 없음, 점수 없음
// 1.1 두번째 돌도 비었다.		* 검사 없음, 점수 없음
// 1.2 두번째 돌이 검은색이다.	* 추가 검사, 점수 가산
// 1.3 두번째 돌이 흰색이다.		* 추가 검사, 점수 가산

// 2.  첫번째 돌이 검은색이다.	* 점수 가산
// 2.1 두번째 돌이 비었다.		* 추가 검사, 점수 가산 없음
// 2.2 두번째 돌이 검은색이다.	* 추가 검사, 점수 가산
// 2.3 두번째 돌이 흰색이다.		* 검사 없음

// 3.  첫번째 돌이 흰색이다.		* 점수 가산
// 3.1 두번째 돌이 비었다.		* 추가 검사, 점수 가산 없음
// 3.2 두번째 돌이 검은색이다.	* 검사 없음
// 3.3 두번째 돌이 흰색이다.		* 추가 검사, 점수 가산

// 비었으면 1점을 더하고
// 돌이 있으면 2점을 더한다.
// 막혔으면 점수 변동이 없다.

//	else if (depth == 2) {
//		if (PreviousColor[depth - 2] == Stone::Emptied) ++score[color];
//		else if (PreviousColor[depth - 2] == PreviousColor[depth - 1]) ++score[color] *= 2;
//		else {
//			score[color] -= 2;
//			break;
//		}

//		

//	}
//	else if (depth == 3) {
//		// 돌이 같다
//		if (PreviousColor[1] == color) {
//			// 2개 또는 3개 연속이다.
//			iTmpScore += 2;
//			if (PreviousColor[0] == color) {
//				iTmpScore += 3;
//			}
//		}
//		// 돌이 다르다
//		else {
//			// 이전 돌이 비었다.
//			if (PreviousColor[1] == Stone::Emptied) {
//				// 이전 돌이 비었지만 첫돌이 나와 같다.
//				if (PreviousColor[0] == color) {
//					iTmpScore += 2;
//				}
//				// 이전돌이 비었고 첫돌이 나와 다르다. 검사중단
//				else {
//					// 다른 돌로 막혔기 때문에 점수를 깍는다.
//					iTmpScore -= 1;
//					break;
//				}
//			}
//			// 이전 돌과 다른데 비지 않았다는 나와 다른 돌 검사중단
//			else {
//				// 다른 색의 돌로 막혔기 때문에 점수를 깍는다.
//				iTmpScore -= 1;
//				break;
//			}
//		}
//	}
//}



//// 추가 검사
//// 세번째, 네번째 돌을 검사
//if (b) {
//	for (int depth = 3; depth <= 4; ++depth) {
//		sprite = stonelayer->SearcharoundSprite(x, y, dir, depth);
//		if (sprite == nullptr) break;
//		int color = sprite->getStoneType();

//		// 돌이 있다.
//		if (color != Stone::Emptied) {
//			if (depth == 3) {
//				// 돌이 같다
//				if (PreviousColor[1] == color) {
//					// 2개 또는 3개 연속이다.
//					iTmpScore += 2;
//					if (PreviousColor[0] == color) {
//						iTmpScore += 3;
//					}
//				}
//				// 돌이 다르다
//				else {
//					// 이전 돌이 비었다.
//					if (PreviousColor[1] == Stone::Emptied) {
//						// 이전 돌이 비었지만 첫돌이 나와 같다.
//						if (PreviousColor[0] == color) {
//							iTmpScore += 2;
//						}
//						// 이전돌이 비었고 첫돌이 나와 다르다. 검사중단
//						else {
//							// 다른 돌로 막혔기 때문에 점수를 깍는다.
//							iTmpScore -= 1;
//							break;
//						}
//					}
//					// 이전 돌과 다른데 비지 않았다는 나와 다른 돌 검사중단
//					else {
//						// 다른 색의 돌로 막혔기 때문에 점수를 깍는다.
//						iTmpScore -= 1;
//						break;
//					}
//				}
//			}
//			// 중간에 돌이 달랐던게 있다면 검사 중단되기 때문에
//			// 첫번째, 두번째 돌과 세번째 돌이 다른건 없다.
//			else if (depth == 4) {
//				// 돌이 같다
//				// 빈돌이 없으면 점수를 더해준다.
//				if (PreviousColor[2] == color) {
//					iTmpScore += 2;
//					if (PreviousColor[1] == color) {
//						iTmpScore += 3;
//					}
//					if (PreviousColor[0] == color) {
//						iTmpScore += 4;
//					}
//				}
//				// 돌이 다르다
//				// 막혔거나 비엇거나
//				else {
//					if (PreviousColor[2] == Stone::Emptied) {
//						// 퐁당이라서 점수를 더한다.
//						if (PreviousColor[1] == color) {
//							iTmpScore += 2;
//						}
//						else {
//							// 다른돌에 막혔기 때문에 점수를 깍는다.
//							iTmpScore -= 1;
//						}
//					}
//					// 막혔다면 점수를 깍는다.
//					else {
//						iTmpScore -= 1;
//					}
//				}
//			}
//		}
//	}
//}

// 최고점으로 판단
// iPointMaxScore = iTmpScore >= iPointMaxScore ? iTmpScore : iPointMaxScore;
// 모두 더한 점수로 판단
