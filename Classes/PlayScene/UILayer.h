#ifndef _UI_LAYER_H_
#define _UI_LAYER_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "enumHeader.h"

class CUILayer : public cocos2d::Layer {
private:
	enum UiNum {
		imgview,
	};

	int m_Player;
	bool m_Pause;
	bool m_State;
	bool m_bReStart;
	bool m_bBGMState;
	bool m_bEffectState;

private:
	void CreateUI();
	
	void ButtonEnabled(int color);
	void bt1OnTouch(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void bt2OnTouch(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void bt3OnTouch(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void btRestartGameOnTouch(cocos2d::Ref* sender);
	void btSoundOnTouch(cocos2d::Ref* sender);
	void btOffGameOnTouch(cocos2d::Ref* sender);
	void btOptionOnTouch(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void CreateOptionPanel();

	void CreateMenu();
	void CreateSoundMenu();

	void BGM_ON_OFF(Ref* sender);
	void Effect_ON_OFF(Ref* sender);
	void btSoundMenuBackOnTouch(Ref* sender);

public:
	CUILayer();
	virtual ~CUILayer();
	virtual bool init();
	CREATE_FUNC(CUILayer);
	
	void GaemResult(const int s);
	void ResetUILayer();
	int getPlayerColor() const;
	bool getPause() const;
	bool getReStart() const;
	bool getBGMState() const;
	bool getEffectState() const;
};

#endif