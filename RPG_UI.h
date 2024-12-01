#pragma once
#include "RPG_Dynamic.h"
#include "InventorySocket.h"
class RPG_Engine;
class cScriptProcessor;


struct  ButtonData
{
	olc::vi2d SourcePos;
	olc::vi2d SourceSize;
	olc::vi2d Pos;

	ButtonData(olc::vi2d Pos, olc::vi2d sourcePos, olc::vi2d size) :Pos(Pos), SourcePos(sourcePos), SourceSize(size) {};
	ButtonData() {};
};

class cUI
{
public:
	cUI(std::string name);


	
	virtual void Update(const float& fElapsedTime,olc::vi2d& mouse) {}
	virtual void  DrawSelf() {}
	virtual bool Onclick(olc::vi2d& mouse) { return true; }

	std::string sName;
	static RPG_Engine* g_engine;
	bool Redundant = false;


protected:

	olc::vi2d sourcePos;
	olc::vi2d sourceSize;

	int16_t offsetX;
	int16_t offsetY;

};


class UICounter : public cUI
{
public: 
	UICounter(std::string name) : cUI(name) {}






};


class UIFastSlot : public cUI

{

public :
	UIFastSlot(std::string name,uint16_t offsetX, uint16_t offsetY);
	bool OneClick(cDynamic* player);
	void DrawSelf() override;
	bool OnInteraction(const olc::vi2d& Mouse);   //на описани
	void ConnectSlot(const olc::vi2d& mouse, InventaryItem* grabitem);


private:

	InventaryItem* Item = nullptr;

};

class UIMainIndicator : public cUI
{
public:
	UIMainIndicator(std::string name) : cUI(name) { MAX = 0; MaxSoursePos = 0; proportion = 0;}

	void  Update(const float& fElapsedTime , olc::vi2d& mouse ) override;

	virtual uint16_t GetIndicator() { return 0; };
	virtual void DrawSelf();

protected:
	uint16_t MAX;
	int MaxSoursePos;
	float proportion;

private:

};

class UIEnergyIndicator : public UIMainIndicator
{
public:
	UIEnergyIndicator();

	//void Update(float fElapsedTime = 0.0f) override;

	uint16_t GetIndicator() override;
	

private:

};

class UIRageIndicator : public UIMainIndicator
{
public:
	UIRageIndicator();


	uint16_t GetIndicator() override;


private:

};

class UIExperienceIndicator : public UIMainIndicator
{
public:
	UIExperienceIndicator();

	void  Update(const float& fElapsedTime , olc::vi2d& mouse ) override;
	void DrawSelf() override;

	uint16_t GetIndicator() override;


private:
	
};



class UICurrentSpell : public cUI
{
public:
	UICurrentSpell(std::string name);

	bool OnInteraction(olc::vi2d& Mouse);   //на описани

	void  Update(const float& fElapsedTime, olc::vi2d& mouse) override;
	void DrawSelf()override;



protected:
	std::pair<olc::vi2d, olc::vi2d> CurrSPell;
};

class UICurrentEnergySpell : public UICurrentSpell
{
public:
	UICurrentEnergySpell();
	void  Update(const float& fElapsedTime, olc::vi2d& mouse) override;
};

class UICurrRageSpell : public UICurrentSpell
{

public:
	UICurrRageSpell();
	void  Update(const float& fElapsedTime , olc::vi2d& mouse ) override;
};




class UiRect : public cUI
{
public:
	UiRect(std::string name, float Fscale = 2.0f) : cUI(name) { fscale = Fscale; };
	virtual void DrawSelf();
	int16_t& GetRectPosX() { return offsetX; };
	int16_t& GetRectPosY() { return offsetY; };
protected:

	float fscale;
};


class UIMainRect : public UiRect
{
public:
	UIMainRect();
};

class UIOptionRect : public UiRect
{
public:
	UIOptionRect();
};

class UIGameRect : public UiRect
{
public:
	UIGameRect();
};

class UITalentRect : public UiRect
{
public:
	UITalentRect();
};






class UIIcons : public cUI
{
public:
	UIIcons(std::string name) : cUI(name) {};
	void DrawSelf();
	void  Update(const float& fElapsedTime , olc::vi2d& mouse) override;
	virtual bool Activate() { return false; };
private:
	bool activator = false;
	enum Namse
	{
		BackLayer =0,
		Animlayer =1
	};
	float animsprite = 0.2f;
	int index = 0;
protected:
	std::array<ButtonData, 2> SpriteDataAr;

};
class UITalentIcon : public UIIcons
{
public:
	UITalentIcon();

	bool Activate() override;
	

private:
	
};
class UIInventoryIcon : public UIIcons
{
public:
	UIInventoryIcon();


	bool Activate() override;

};
class UIQuestIcon : public UIIcons
{
public:
	UIQuestIcon();

	bool Activate() override;


};






class  UIButton : public cUI
{
public:
	UIButton(std::string name);

	virtual bool OnInteraction(olc::vi2d& Mouse); // highlighted
	bool Onclick(olc::vi2d& mouse)override { return true; };
	 void Update(const float& fElapsedTime , olc::vi2d& mouse)override;

	 void DrawSelf()override;

	void SwitchChain(UiRect* Back);

	static cScriptProcessor* g_script;

protected:

	virtual void CalculatePosition() {};

	olc::vi2d Pos;
	bool Interacted;
	UiRect* Rect;
};





class UIScroller : public UIButton
{

public:
	UIScroller(std::string name);

	 void DrawSelf() override;
	 void Update(const float& felapsedtime,olc::vi2d& Mouse)override;
	bool Onclick(olc::vi2d& mouse) override;

	virtual void ScrollerAction(int parametr) {};

	bool OnInteraction(olc::vi2d& mouse) override;


	void checkBorder();
	void CalculateRelation();


protected:

	void CalculatePosition()override {};

	int ScrollLine;

	uint16_t MIN;
	uint16_t MAX;


	enum ScrollerEnmu
	{
		Back =0,
		Cursor =1
	};

	std::array<ButtonData, 2> spritesAr;

};

class UISoundScroller : public UIScroller
{
public:
	UISoundScroller();

	void ScrollerAction(int parametr)override;


protected:
	void CalculatePosition()override;

};

class UIMusicScroller : public UIScroller
{
public:
	UIMusicScroller();

	void ScrollerAction(int parametr)override;

protected:
	void CalculatePosition()override;


};


class UIStartButton : public UIButton
{
public:
	UIStartButton();
	bool  Onclick(olc::vi2d& mouse) override;


protected:
	void CalculatePosition()override;


private:

};

class UIContinueButton : public UIButton
{
public:
	UIContinueButton();
	bool  Onclick(olc::vi2d& mouse) override;
	void DrawSelf()override;


protected:
	void CalculatePosition()override;


private:

	 bool bSaveExist;

};
class UIQuitButton : public UIButton
{
public:
	UIQuitButton();
	bool Onclick(olc::vi2d& mouse) override;

protected:
	void CalculatePosition()override;



private:

};
class UIOptionButton : public UIButton
{
public:
	UIOptionButton();
	bool Onclick(olc::vi2d& mouse) override;


protected:
	void CalculatePosition()override;



private:
	//bool Activate = false;


};


class UIExitButton : public UIButton
{
public:
	UIExitButton();
	bool Onclick(olc::vi2d& mouse) override;

	void DrawSelf()override;


protected:
	void CalculatePosition()override;



private:
	//bool Activate = false;


};


class UIFullScreenButton : public UIButton
{
public:
	UIFullScreenButton();

	void DrawSelf() override;
	
	bool OnInteraction(olc::vi2d& Mouse) override;

	bool Onclick(olc::vi2d& mouse) override;

	bool SwitchScreen();


protected:
	void CalculatePosition()override;



private:

	

	std::array<ButtonData, 3> spritesAr;

	bool Toggle = true;

};

class Event : public cUI
{

public: 
	Event(std::string name,olc::vi2d sourcepos,olc::vi2d sourcesize, float Maxtime =0.3f); // Используем указатель);

	void DrawSelf();
	bool update(float fElapsedTime);
	void reset() { timer=MaxTimer ; Redundant = false; Alpha = 255; };

protected:

	float timer;
	float MaxTimer = 0.3f;

	uint8_t Alpha;
};







