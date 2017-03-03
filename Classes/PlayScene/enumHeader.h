#ifndef _ENUM_HEADER_H_
#define _ENUM_HEADER_H_

enum GameState {
	black_win, white_win, time_over = 10, error
};

enum Stone { 
	black, white, emptied, block,
};

#endif
