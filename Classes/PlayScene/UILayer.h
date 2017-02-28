#ifndef _UI_LAYER_H_
#define _UI_LAYER_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "enumHeader.h"

class CUILayer : public cocos2d::Layer {
public:
	CUILayer();
	virtual ~CUILayer();
	virtual bool init();
	CREATE_FUNC(CUILayer);

	void initUILayer();
	void createGaemResult(const GameState s);
	Stone getPlayerColor() const;
	bool getPause() const;
	//bool getSeleteColor() const;
	//void setSeleteColor(bool b);
	bool getReStart() const;
	bool getBGMState() const;
	bool getEffectState() const;
	void setSoundOption(const bool bgm, const bool effect);
	void setRunActionrCountDownFunc(std::function<void(void)> countdown);
	void setRunActionrPauseFunc(std::function<void(void)> pause);
	void setRunActionrRestartFunc(std::function<void(void)> restart);

private:
	void CreateUI();
	void createMenu();
	void createSoundMenu();
	void createOptionMenu();

	void enabledColorSeleteButton(const Stone color);
	void onTouchBlackSeleteButton(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void onTouchWhiteSeleteButton(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void onTouchColorCheckButton(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void onTouchOptionMenuButton(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void onTouchRestartGameButton(cocos2d::Ref* sender);
	void onTouchSoundMenuButton(cocos2d::Ref* sender);
	void onTouchQuitButton(cocos2d::Ref* sender);
	void onTouchBGM_ON_OFF_Toggle(Ref* sender);
	void onToucheffect_ON_OFF_Toggle(Ref* sender);
	void onTouchSoundMenuBackButton(Ref* sender);

	std::function<void(void)>RunActionrCountDown;
	std::function<void(void)>RunActionrPause;
	std::function<void(void)>RunActionrRestart;

private:
	Stone player_color;
	bool pause_check;
	//bool selete_color_check;
	bool state_check;
	//bool restart_check;
	bool bgm_state_check_check;
	bool effect_State_check;
};

#endif