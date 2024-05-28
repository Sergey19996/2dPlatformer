#pragma once
#include "RPG_Dynamic.h"
class RPG_Engine;

class cUI
{
public:
	cUI(std::string name,  olc::Decal* Full);

	virtual bool OnInteract(cDynamic* object) { return false; } 

	virtual bool OnUse(cDynamic* object) { return  false; }

	virtual void DrawSelf(olc::PixelGameEngine* gfx, int px, int py) {}

	virtual void DrawSelfTalent(olc::PixelGameEngine* gfx, int px, int py) {}


	virtual int SaveSlotTalent() { return 0; };

public:
	static RPG_Engine* g_engine;
	

	std::string sName;
	
	olc::Decal* pSpriteFull;
	

	

	int offsetX;
	int offsetY;

};


class cIndicator :public cUI
{
public:
	cIndicator(std::string name, olc::Decal* Full,olc::Decal* Empty);
	bool OnInteract(cDynamic* object) override;
	bool OnUse(cDynamic* object) override;

	void DrawSelf(olc::PixelGameEngine* gfx, int px, int py) override;

	void DrawSelfTalent(olc::PixelGameEngine* gfx, int px, int py) override;

	int SaveSlotTalent() override;


public:

	int indicator;
	olc::Decal* pSpriteEmpty;


	int TalentSave = 0;

};





class  cExperience : public cIndicator
{
public:
	cExperience();

public:
	void DrawSelf(olc::PixelGameEngine* gfx, int px, int py) override;


};



class  cEnergyIndicators : public cIndicator
{
public:
	cEnergyIndicators(std::string name, olc::Decal* Fill, olc::Decal* Empty);



public:
	void DrawSelf(olc::PixelGameEngine* gfx, int px, int py) override;
	void DrawSelfTalent(olc::PixelGameEngine* gfx, int px, int py) override;
	
	enum
	{
		EnergyIndicatorFirst = 0,
		EnergyIndicatorSecond = 1,
		EnergyIndicatorThird = 2,
		JUMP = 3,
		BACKATTACK = 4,
		LOWATTACK = 5,
		MIDATTACK =6,
		HIGHATTACK=7,
		STAPBACK =8,
		SWIRLATTACK =9,
		VANISH = 10

	} m_Classenum;

	olc::Decal* pSpriteReady;


	int sizeframe = 50;
	int bhide = 0;
};



class  cRageIndicators : public cIndicator
{
public:
	cRageIndicators(std::string name, olc::Decal* Full, olc::Decal* Empty);

public:
	void DrawSelf(olc::PixelGameEngine* gfx, int px, int py) override;
	void DrawSelfTalent(olc::PixelGameEngine* gfx, int px, int py) override;
	enum
	{
		RageIndicatorFirst = 0,
		RageIndicatorSecond = 1,
		RageIndicatorThird = 2,
		ATTACKEARTH = 3,
		ATTACKAIR = 4,
		ATTACKFLIGHTUP = 5,
		ATTACKEPICLANDING = 6,
		

	} m_Classenum;
	olc::Decal* pSpriteReady;
	int bhide = 0;
	int sizeframe = 50;
};



class  cNewEnergyIndicator : public cEnergyIndicators
{
public:
	cNewEnergyIndicator(int attackvariation, int fOffsetx, int foffsety, std::string nameadd);
	
private:
	
	

};


class  cNewRageIndicator : public cRageIndicators
{
public:
	cNewRageIndicator(int attackvariation, int fOffsetx, int foffsety, std::string nameadd);
	
private:



};



class  cAttackLow : public cEnergyIndicators
{
public:
	cAttackLow();

public:
	//void DrawSelf(olc::PixelGameEngine* gfx, int px, int py) override;

};


class  cVanish : public cEnergyIndicators
{
public:
	cVanish();

public:
	//void DrawSelf(olc::PixelGameEngine* gfx, int px, int py) override;

};


class  cAttackMid : public cEnergyIndicators
{
public:
	cAttackMid();

public:
//	void DrawSelf(olc::PixelGameEngine* gfx, int px, int py) override;

};

class  cAttackHigh : public cEnergyIndicators
{
public:
	cAttackHigh();

public:
	//void DrawSelf(olc::PixelGameEngine* gfx, int px, int py) override;

};
class  cAttacBack : public cEnergyIndicators
{
public:
	cAttacBack();

public:
	//void DrawSelf(olc::PixelGameEngine* gfx, int px, int py) override;

};
class  cRightAttack : public cRageIndicators
{
public:
	cRightAttack();

public:
	//void DrawSelf(olc::PixelGameEngine* gfx, int px, int py) override;

};


class  cRightEpicLanding : public cRageIndicators
{
public:
	cRightEpicLanding();

public:
	//void DrawSelf(olc::PixelGameEngine* gfx, int px, int py) override;

};

class  cRightAttackAir : public cRageIndicators
{
public:
	cRightAttackAir();

public:
//	void DrawSelf(olc::PixelGameEngine* gfx, int px, int py) override;

};

class  cRightFlightUp : public cRageIndicators
{
public:
	cRightFlightUp();

public:
	//	void DrawSelf(olc::PixelGameEngine* gfx, int px, int py) override;

};



class  cJump : public cEnergyIndicators
{
public:
	cJump();

public:
	//void DrawSelf(olc::PixelGameEngine* gfx, int px, int py) override;

};

class  cStepBack : public cEnergyIndicators
{
public:
	cStepBack();

public:
	//void DrawSelf(olc::PixelGameEngine* gfx, int px, int py) override;

};


class  cSwirlAttack : public cEnergyIndicators
{
public:
	cSwirlAttack();

public:
	//void DrawSelf(olc::PixelGameEngine* gfx, int px, int py) override;

};




class  cStaticUi : public cUI
{
public:
	cStaticUi(std::string name, olc::Decal* Full);

public:
	
	void DrawSelf(olc::PixelGameEngine* gfx, int px, int py) override;
};


class cLevel : public cStaticUi
{
public:
	cLevel();


};


class cSpellPlace : public cStaticUi
{
public:
	cSpellPlace();


};




class cNumber : public cStaticUi
{
public:
	cNumber();

	void DrawSelf(olc::PixelGameEngine* gfx, int px, int py) override;

};


