#pragma once
#include "RPG_Assets.h"
#include "olcPixelGameEngine.h"

class RPG_Engine;
class Environment;
class cWeapon;
class cItem;
class cEquip;

class cIndicator;
class cDynamic_HpBar;
class cDynamic_EnergyBar;
class cDynamic_RageBar;
class cComand;
class cDynamic
{
public:
	cDynamic(const std::string n);
	~cDynamic();

public:
	float px, py;
	float vx, vy;
	bool bSolidVsMap;

	enum LAYER
	{ Friend = 0, Enemy = 1, Neutral = 2, Particle = 3,Questable =4 } m_layer;


	// Объявляем флаги через enum
	enum DynamicFlagsEnum {
		BisProjectile = 1 << 0,  // 1-й бит
		bDraw = 1 << 1,  // 2-й бит
		bIsAttackable = 1 << 2,  // 3-й бит
		bRedundant = 1 << 3,  // 4-й бит
		bDead = 1 << 4,  // 5-й бит
		bOnGround = 1 << 5,  // 6-й бит
		bOnTarget = 1 << 6,  // 7-й бит
		binitialized = 1 << 7,  // 8-й бит
		bControllable = 1 << 8,  // 9-й бит
		quested = 1 << 9,  // 10-й бит  для обьектов, которые рендерятся без ограничений 
		isAttack = 1 << 10, // 11-й бит
		IsThrow = 1 << 11, // 12-й бит
		IsOnePlay = 1 << 12, // 13-й бит
		bOnLanded = 1 << 13, // 14-й бит
		bAnimAction = 1 << 14, // 15-й бит
		isprojfollow = 1 << 15, // 16-й бит
		Btarget = 1 << 16, // 17-й бит
		gravity = 1 << 17,  // 18-й бит
		isDirectionLock = 1 << 18 // 19-й бит
		
	};

	
	// Переменная для хранения всех флагов
	unsigned int DynamicFlags = 0;

	// Методы для установки и проверки флагов
	void setFlag(DynamicFlagsEnum flag) {
		DynamicFlags |= flag;  // Устанавливаем флаг
	}

	void clearFlag(DynamicFlagsEnum flag) {
		DynamicFlags &= ~flag;  // Сбрасываем флаг //revers number
	}

	bool checkFlag(DynamicFlagsEnum flag) const {
		return DynamicFlags & flag;  // Проверяем флаг
	}

	std::string sName;
	unsigned int Jumpcounter :4;




	int mass = 32;
	
	std::pair<olc::vf2d*, olc::vf2d*>* obsticlepoints = nullptr;

	float CollbordersX, CollbordersY, CollbordersXF, CollbordersYF;


public:
	virtual void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) {}
	virtual void Update(float fElapsedTime, cDynamic* player = nullptr) {}
	virtual void OnInteract(cDynamic* player = nullptr) {}
	virtual bool IsLanded() { return false; }
	virtual void SwitchLayer(int num) ;
	virtual  int GetHealth() { return 0; };
	virtual void SetHealth(int setHealth)  { };



	static RPG_Engine* g_engine;

protected :

};





class cDynamic_Creature : public cDynamic
{
public:
	cDynamic_Creature(const std::string n, olc::Decal* spriteRight, olc::Decal* spriteLeft);       // <--Sprite

protected:
	olc::Decal* m_pSpriteRight;
	olc::Decal* m_pSpriteLeft;


	float m_fTimer;
	enum { WEST = 1, EAST = 3 } M_nFacingDirection;
	enum { SOUTH = 0, NORTH = 1, NOTLOOKING = 3 } M_nFacingDirectionVertical;

	enum { STRENGTH = 0, AGILITY = 1, INTELLIGENCE = 2 }M_BaseClass;

	float fHpRegeneration = 0;



	int calculatedDamage = 0;



	//   Calculation thatnk's lvl oh character

	float LvlHaste = 0;  // how fast animations flow


	unsigned int Lvlattributes = 0;

	void SetLvlAgility(uint8_t agil);
	void SetLvlStrength(uint8_t str);
	void SetLvlInt(uint8_t Inteleg);
	void SetLvlMovement(uint8_t movement);
	const uint8_t getLvlAgil();
	const uint8_t gettLvlStrength();
	const uint8_t getLvlInt();
	const uint8_t getLvlMovement();



	unsigned int LvlAverageAttack : 8;
	unsigned int LvlDefence : 8;
	unsigned int lvlHealthMax =0;
	//
	
	unsigned int Endattributes = 0;
	void SetEndAgility(uint8_t agil);
	void SetEndStrength(uint8_t str);
	void SetEndInt(uint8_t Inteleg);
	void SetEndMovement(uint8_t movement);
	const uint8_t getEndAgil();
	const uint8_t getEndStrength();
	const uint8_t getEndInt();
	const uint8_t getEndMovement();



	unsigned int EndAverageAttack : 8;   // here keeping calculation with addition from items
	unsigned int EndDefence : 8;

	float EndHaste = 0;

	unsigned int BasicAttributes = 0;


	void setBasicAgility(uint8_t agil);
	void setBasicStrength(uint8_t str);
	void setBasicInt(uint8_t Inteleg);
	void setBasicMovement(uint8_t movement);
	const uint8_t getBasicAgil();
	const uint8_t getBasicStrength();
	const uint8_t getBasicInt();
	const uint8_t getBasicMovement();


	const uint8_t GetWeaponAttributes(const unsigned int attributes)
	{
		uint8_t dmg = (attributes >> 16) & 0xf;
		return dmg;
	}

	enum
	{
		Idle = 0,
		Walk = 1,
		Jump = 2,
		Attack = 3,
		AttackEasy = 4,
		AttackGreat = 5,

		DoubleJump = 6,
		AirAttack = 7,
		Eviscirate = 8,       //  Right Botton

		JumpDown = 9,
		JumpZenit = 10,
		Landing = 11,

		AirEviscirate = 12,   //  Right Botton
		Hit = 13,

		AttackBack = 16,

		UpEviscirate = 17,      //  Right Botton
		AirDownEviscirate = 18,    //  Right Botton
		LANDINGEviscirate = 19,    //  Right Botton
		BLINKDESAPEAR = 20,
		GRAB = 21,
		Swirl = 22,
		Death = 23,
		IdleTwo = 24,
		IdleReaction = 25


	}  M_nGraphicState;

	float m_fStateTick;  //deplete every frame  check on agro in behaviour

	float m_fKnockBackTimer = 0.0f;
	float m_fKnockBackDX = 0.0f;
	float m_fKnockBackDY = 0.0f;



	uint32_t color = 0;

	uint8_t BeveOffsetpX = 0;   // offset for behave px
	uint8_t BeveOffsetpY = 0; // py

	// int8_t redColor = 255;      //<--colors
	// int8_t greenColor = 255;
	// int8_t blueColor = 255;

	
	

public:

	virtual uint32_t getIdleData() { return 0; };
	virtual uint32_t getWalkData() { return 0; };

	float ProjCollbordersX, ProjCollbordersY, ProjCollbordersXF, ProjCollbordersYF;

	//when need stop gravity for seconds use it
	float gravityofftimer = 0.0f;
	//

	int moveBonus = 0;   // increase it when we use shadow form. it add in A and D logic

	int FxColumn;

	int nHealth;
	
	int nHealthMax;


	
	unsigned int enumCounter :8;
	
	/// ///
	float attackdirectionX = 0.0f;       //in indicators equal enumirator of attack 
	float attackdirectionY = 0.0f;       // we can modify direction of victim in depend of attack
	

	// need put it in int






	// working with drawning sprites
	int m_nGraphicCounterX;
	int m_nGraphicCounterY;
	float m_nShockTime;


	int frameIndicator;
	float framespeed;
	float animspr ;
	int m_nGraphicAmountFrames;
//	int m_nGraphicEndFrame;

	//bool FxGo;
	int FXFrame;

	///////
	float nSheetOffsetY, nSheetOffsetX;
	float nSheetSizeX = 64.0f;
	float nSheetSizeY = 64.0f;
	float sparedVx = 0.0f;
	float sparedVy = 0.0f;

	int rageset = 0;   // <- parametr for calculate rage indicator
	

	int LvL = 0;
	int Hittimes = 0;
	 
public:
	
		void setColor(const uint8_t Red, const uint8_t Green, const uint8_t Blue, const uint8_t Alpha);
		const uint8_t getRed();
		const uint8_t getGreen();
		const uint8_t getBlue();
		const uint8_t getAlpha();
		void setRedColor(uint8_t Red);
		void setGreenColor(uint8_t Green);
		void setBlueColor(uint8_t Blue);
		void SetAlphaColor(uint8_t Alpha);


	virtual void Behaviour(float fElapsedTime, cDynamic* player = nullptr);
	virtual void ReturnBaseLayer() {};
	virtual void DeathFun() {};
	virtual void IndicateAnim() {}  // using after setenum// define sprites positions
	virtual void PerformAttack() {};
	virtual void KnockBack(float dx, float dy, float dist);
	virtual void Action() {};


	void Update(float fElapsedTime, cDynamic* player = nullptr) override;
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) override;
	void SetAnimation(int num) { enumCounter = num;  setFlag(IsOnePlay); };
	//void SetHealth(int setHealth) override { nHealth -= setHealth; };
	void calculateStats();
	void GravityControl(float felapsedtiem);
	void SetUpDamage();

	


	bool IsLanded()  override;
	bool setEnum();     // <-keep the parametrs which need for use animtions


	olc::Decal* GetLeftSprite() { return m_pSpriteLeft; };
	olc::Decal* GetRightSprite() { return m_pSpriteRight; };
	int GetFacingDirection() { return M_nFacingDirection; };
	int GetFacingDirectionVertical() { return M_nFacingDirectionVertical; };
	int calculateDeathExp();
	int GetAttack() { return  calculatedDamage; };
	int GetDefence() { return EndDefence; };
	int GetHealth() override { return nHealth; };
	
	cEquip* pEquipedWeapon = nullptr;
	


	cDynamic_HpBar* hpUpdate = nullptr;
	cDynamic_EnergyBar* EnergyUpdate = nullptr;
	cDynamic_RageBar* RageUpdate = nullptr;
	

	

	//float ProjOffsetX = 0;
//	float ProjOffsetY =0;
protected:
	bool bKnockBack = true;




	 unsigned int BasicAveAtck : 8;
};



class cDynamic_Teleport : public cDynamic
{
public:
	cDynamic_Teleport(float x, float y, std::string sMapName, float tx, float ty);
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) override;
	void Update(float fElapsedTime, cDynamic* player = nullptr) override;

public:
	std::string sMapName;
	float fMapPosX;
	float fMapPosY;
};


class  cDynamic_creature_Enemy : public cDynamic_Creature
{
public:
	cDynamic_creature_Enemy(std::string n, olc::Decal* spriteRight, olc::Decal* spriteLeft);
	virtual void Behaviour(float fElapsedTime, cDynamic* player = nullptr);
	virtual void  SpecAttack(float targetX, float targetY, float Distance) {};
	//void KnockBack(float dx, float dy, float dist)override;
	virtual void AttackOne() {};
	virtual void AttackTwo() {};
	//virtual void Action() {};

	void PerformAttack() override;
	bool IsLanded()  override;

protected:
	float fSpecAtckdist = 2.9f;
	float fAttackDist = 1.6f;
	float vxBorder = 2.0f;
public:
	uint32_t getIdleData() override { return 0; };
	
	uint32_t getWalkData() override { return 0; };
};







class  cDynamic_creature_BossBandit : public  cDynamic_creature_Enemy
{

public:
	cDynamic_creature_BossBandit();

	void Behaviour(float fElapsedTime, cDynamic* player = nullptr) override;

	void KnockBack(float dx, float dy, float dist) override
	{};

	void SpecAttack(float targetX, float targetY, float Distance) override;
	void SpecAttack2(float targetX, float targetY, float Distance);
	void Mocking();
	void AttackTwo() override;


	void IndicateAnim() override;




private:
	unsigned int looptimes : 4;
	

	int attackdif = 0;

public:
	uint32_t getIdleData() override 
	{                               
		uint32_t frameData = 0;    // 0000 0000 0000 0000
		                           //      Amnt   Y    X 
		frameData = (frameData & 0xFFFFFF00) | 0;   //Data take from idle indicate state
		frameData = (frameData & 0xFFFF00FF) | (0 << 8);
		frameData = (frameData & 0xFF00FFFF) | (8 <<16);
		
		return frameData;
	
	};
	uint32_t getWalkData() override
	{
		uint32_t frameData = 0;    // 0000 0000 0000 0000
								   //      Amnt   Y    X 
		frameData = (frameData & 0xFFFFFF00) | 2;   //Data take from idle indicate state
		frameData = (frameData & 0xFFFF00FF) | (1 << 8);
		frameData = (frameData & 0xFF00FFFF) | (7 << 16);

		return frameData;

	};
};



class  cDynamic_creature_Bandit : public cDynamic_creature_Enemy
{

public:
//	cDynamic_creature_Bandit(std::string n, olc::Decal* spriteRight, olc::Decal* spriteLeft);
	cDynamic_creature_Bandit();
	void IndicateAnim() override;
	
	void SpecAttack(float targetX, float targetY, float Distance) override;
	

	void ReturnBaseLayer()override { m_layer = Enemy; };
	void AttackOne() override;
	void AttackTwo() override;

	
	void DeathFun() override;


public:
	uint32_t getIdleData() override
	{
		uint32_t frameData = 0;    // 0000 0000 0000 0000
		//      Amnt   Y    X 
		frameData = (frameData & 0xFFFFFF00) | 0; // X  //Data take from idle indicate state
		frameData = (frameData & 0xFFFF00FF) | (0 << 8); // Y
		frameData = (frameData & 0xFF00FFFF) | (8 <<16); // Amount frames

		return frameData;

	};
	uint32_t getWalkData() override
	{
		uint32_t frameData = 0;    // 0000 0000 0000 0000
		//      Amnt   Y    X 
		frameData = (frameData & 0xFFFFFF00) | 0;   //Data take from idle indicate state
		frameData = (frameData & 0xFFFF00FF) | (1 << 8);
		frameData = (frameData & 0xFF00FFFF) | (9 << 16);

		return frameData;

	};
};



class  cDynamic_creature_BanditArcher : public cDynamic_creature_Bandit
{

public:
	
	cDynamic_creature_BanditArcher();
	//void IndicateAnim() override;

	void SpecAttack(float targetX, float targetY, float Distance) override;


//	void ReturnBaseLayer()override { m_layer = Enemy; };

	void AttackOne() override;
	void AttackTwo() override;


	//void DeathFun() override;
//	void Behaviour(float fElapsedTime, cDynamic* player = nullptr) override;

public:
	uint32_t getIdleData() override
	{
		uint32_t frameData = 0;    // 0000 0000 0000 0000
		//      Amnt   Y    X 
		frameData = (frameData & 0xFFFFFF00) | 0; // posX  //Data take from idle indicate state sprite
		frameData = (frameData & 0xFFFF00FF) | (0 << 8); // posY
		frameData = (frameData & 0xFF00FFFF) | (8 << 16); // Amount frames

		return frameData;

	};
	uint32_t getWalkData() override
	{
		uint32_t frameData = 0;    // 0000 0000 0000 0000
		//      Amnt   Y    X 
		frameData = (frameData & 0xFFFFFF00) | 0;   //Data take from idle indicate state
		frameData = (frameData & 0xFFFF00FF) | (1 << 8);
		frameData = (frameData & 0xFF00FFFF) | (9 <<16);

		return frameData;

	};
};



class  cDynamic_creature_DireWolf : public  cDynamic_creature_Enemy
{

public:
	cDynamic_creature_DireWolf();

	void SpecAttack(float targetX, float targetY, float Distance) override;

	void IndicateAnim() override;
	
	void AttackOne() override;
	void AttackTwo() override;
	

	void DeathFun() override;


public:
	uint32_t getIdleData() override
	{
		uint32_t frameData = 0;    // 0000 0000 0000 0000
		//      Amnt   Y    X 
		frameData = (frameData & 0xFFFFFF00) | 0; // X  //Data take from idle indicate state
		frameData = (frameData & 0xFFFF00FF) | (0 << 8); // Y
		frameData = (frameData & 0xFF00FFFF) | (7 << 16); // Amount frames

		return frameData;

	};
	uint32_t getWalkData() override
	{
		uint32_t frameData = 0;    // 0000 0000 0000 0000
		//      Amnt   Y    X 
		frameData = (frameData & 0xFFFFFF00) | 0;   //Data take from idle indicate state
		frameData = (frameData & 0xFFFF00FF) | (4 << 8);
		frameData = (frameData & 0xFF00FFFF) | (4 << 16);

		return frameData;

	};

};

class  cDynamic_creature_WereWolf : public  cDynamic_creature_Enemy
{

public:
	cDynamic_creature_WereWolf();

	void SpecAttack(float targetX, float targetY, float Distance) override;

	void IndicateAnim() override;

	void AttackOne() override;
	void AttackTwo() override;

	void Action() override;

	void CallWolfs();
	void DeathFun() override;

	bool bTransformed;
private:
	std::vector<cDynamic*> wolfPack;




public:
	uint32_t getIdleData() override
	{
		uint32_t frameData = 0;    // 0000 0000 0000 0000
		//      Amnt   Y    X 
		frameData = (frameData & 0xFFFFFF00) | 8; // X  //Data take from idle indicate state
		frameData = (frameData & 0xFFFF00FF) | (0 << 8); // Y
		frameData = (frameData & 0xFF00FFFF) | (7 << 16); // Amount frames

		return frameData;

	};
	uint32_t getWalkData() override
	{
		uint32_t frameData = 0;    // 0000 0000 0000 0000
		//      Amnt   Y    X 
		frameData = (frameData & 0xFFFFFF00) | 3;   //Data take from idle indicate state
		frameData = (frameData & 0xFFFF00FF) | (2 << 8);
		frameData = (frameData & 0xFF00FFFF) | (7 << 16);

		return frameData;

	};
};

class  cDynamic_creature_Rider : public  cDynamic_creature_Enemy
{

public:
	cDynamic_creature_Rider(olc::Decal*pspriteRight, olc::Decal* pspriteLeft);

//	void SpecAttack(float targetX, float targetY, float Distance) override;

	void IndicateAnim() override;


	
	void TakeComands(std::list<cComand*> comands);
	void Behaviour(float fElapsedTime, cDynamic* player = nullptr) override;



//	void DeathFun() override;

private:
	std::list<cComand*> Rider_comands;
	

	

};




class  cDynamic_creature_Boar : public  cDynamic_creature_Enemy
{

public:
	cDynamic_creature_Boar();

	void SpecAttack(float targetX, float targetY, float Distance) override;

	void Behaviour(float fElapsedTime, cDynamic* player = nullptr) override;

	void obsticleReaction();

	void DeathFun() override;

	void IndicateAnim() override;

	bool IsLanded()  override;


private:
	bool Run =false;
	
	float savedFx = 0.0f;

	bool BRunready = false;
	

public:
	uint32_t getIdleData() override
	{
		uint32_t frameData = 0;    // 0000 0000 0000 0000
		//      Amnt   Y    X 
		frameData = (frameData & 0xFFFFFF00) | 8; // X  //Data take from idle indicate state
		frameData = (frameData & 0xFFFF00FF) | (0 << 8); // Y
		frameData = (frameData & 0xFF00FFFF) | (9 <<16); // Amount frames

		return frameData;

	};
	uint32_t getWalkData() override
	{
		uint32_t frameData = 0;    // 0000 0000 0000 0000
		//      Amnt   Y    X 
		frameData = (frameData & 0xFFFFFF00) | 10;   //Data take from idle indicate state
		frameData = (frameData & 0xFFFF00FF) | (1 << 8);
		frameData = (frameData & 0xFF00FFFF) | (16 << 16);

		return frameData;

	};
};


class  cDynamic_creature_NPCBandit : public  cDynamic_Creature
{

public:

	cDynamic_creature_NPCBandit(std::string sname);

	void IndicateAnim() override;

};


class  cDynamic_creature_Crowd : public  cDynamic_Creature
{

public:

	cDynamic_creature_Crowd(std::string sname,olc::Decal* Right, olc::Decal* Left, const int Count, uint32_t frameData);
	void Update(float fElapsedTime, cDynamic* player = nullptr) override 
	{
		for (size_t i = 0; i < count; i++)
		{
			vec_animspr[i] += fElapsedTime;
		}
	};
	void IndicateAnim()  override {};
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)override;

private:
	int count;
	std::vector<uint8_t> vec_Framecounters;   // for chaotic crowd animation
	std::vector<float> vec_Xoffset;
	std::vector<float> vec_animspr;
	int spriteWidth;
};



class  cDynamic_ClipTrigger : public  cDynamic
{

public:

	cDynamic_ClipTrigger(std::string sname);
	


};




class cDynamic_creature_Pantir : public cDynamic_Creature    //represent player
{

public:


	cEquip* pEquipedChest = nullptr;
	cEquip* pEquipedHelmet = nullptr;
	cEquip* pEquipedBoots = nullptr;
	cEquip* pEquipedBack = nullptr;
	cEquip* pEquipedNeck = nullptr;

	cEquip* pEmptySlot = nullptr;
	
	cDynamic_creature_Pantir();
	

	
	void MergeItemLvl();
	void  SwirlGrab(cDynamic* dyn);

	void Behaviour(float fElapsedTime, cDynamic* player = nullptr);
	void PerformAttack() override;

	bool checkFacingDirection(cDynamic* Enemy = nullptr);
	void IndicateAnim() override;
	int GetStats(int num);


	 int GetHealth() override { return nHealth; };
	 void SetHealth(int setHealth) override;
	
	 int GetEnergy()  { return energyAmount; };
	 void SetEnergy(int setEnergy) ;

	 int GetRage() { return rageAmount; };
	 void SetRage(int setEnergy);


	bool ConnectTarget(cDynamic* takenTarget) 
	
	{
		if (takenTarget !=nullptr)
		{


	
		Target = takenTarget;
		Target->setFlag(Btarget);
		return true;

		}
		Target = takenTarget;
		return false; 
	};
	cDynamic* Gettarget()
	{if (Target != nullptr) 
	return Target;
	return nullptr;}


	void TakeAttributes(unsigned int attributes);
	void SubstracAttributes(unsigned int attributes);

	
	int GetMovement() { return (this->getEndMovement() + this->moveBonus) / 10; };

	void MoveJump();
	void MoveDoubleJump();

	void SetFacingDirection(int dir);
	void SetVerticalDirection(int Var);

	void HideStage();
	void EnergyMoveAttackAir();
	void EnergyMoveAttackLow();
	void EnergyMoveAttackMid();
	void EnergyMoveAttackHigh();
	void EnergyMoveAttackBack();
	void EnergyHide(float felapsedTime);

	void RageMoveAttackUp();
	void RageMoveAttackAir();
	void RageMoveAttackJumpUp();
	void RageMoveAttackAirDown();
	void RageMoveAttackLanding();
	void BlinkBehind();


	void StatsImpact();
	
	bool IsLanded()  override;
	void DeathFun() override;

	void ReturnBaseLayer()override 
	{ m_layer = Friend; };


	float energyCount = 0;   // <counter of energe. thanks felapsed time it's gain 1 and than add it in energy amount
	float fAttackcount = 0;    // <-- Counter for sinister strices;  help count logic when we dragged lef mouse and released 
	int8_t energyAmount = 100;
	uint8_t MaxEnergy = 100;
	uint8_t MaxRage = 100;

	float rageCount = 0;      // <counter of Rage. thanks felapsed time it's gain 1 and than substruct it in  rage amount
	
	int8_t rageAmount = 0;
	bool bHideMode = false;

private:
	
	unsigned int ItemAttributes = 0;

	void setItemAgil(uint8_t Agil);
	void setItemStr(uint8_t Str);
	void setItemInt(uint8_t Int);
	void setItemMove(uint8_t Move);

	uint8_t getItemAgil();
	uint8_t getItemStr();
	uint8_t getItemInt();
	uint8_t getItemMove();

	


	float ItemHaste = 0;


	unsigned int ItemDef : 8;
	unsigned int ItemAvAtk = 0;

	float rawDmg = 0.0f;
	float rawHaste = 0.0f;
	float rawDef = 0.0f;
	float rawHealth = 0.0f;

	float targetTime = 0.5f;
	olc::Decal* m_pSpriteGrabRight;
	olc::Decal* m_pSpriteGrabLeft;
	olc::Decal* m_pSpriteReserve;

	int Grabedenemy = 0;

	cDynamic* Target = nullptr;
	float Hidetimer = 2.0f;
	

};




class cDynamic_Item : public cDynamic
{
public:
	
	cDynamic_Item();
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) override;
	void OnInteract(cDynamic* player = nullptr) override;
	void Spawn(const olc::vf2d pos, cItem* Item)
	{
		px = pos.x;
		py = pos.y;
		item = Item;

		bSolidVsMap = true;
		bCollected = false;
	
	}

	~cDynamic_Item()
	{
		std::cout << "Hello";
	};

public:
	cItem* item;
	bool bCollected = false;

	
	


};


class cDynamic_Projectile : public cDynamic
{
public:
	cDynamic_Projectile();

	//cDynamic_Projectile(float ox, float oy,  LAYER mlayer, float velx, float vely, float duration, olc::Decal* pSpriteRight, olc::Decal* pSpriteLeft ,  int m_GraphicState, float m_nGraphicTime, int rageset);
	//cDynamic_Projectile(olc::Decal* pSpriteRight, olc::Decal* pSpriteLeft, cDynamic_Creature* Aggressor) ; // when projectile follw for char
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) override;
	void Update(float fElapsedTime, cDynamic* player = nullptr) override;
	void SwitchLayer(int num);
	void SetTimer(int charstate);
	int setEnum(int charstate);
	
	void SetDeafult();
	void SetAgressorData(cDynamic_Creature* Aggresor);
	void SetAgressorThowData(cDynamic_Creature*Aggresor, uint8_t Time);
	void SetSprites(olc::Decal* pSpriteRight, olc::Decal* pSpriteLeft);

public:
	olc::Decal* pSpriteRight = nullptr;
	olc::Decal* pSpriteLeft = nullptr;
	

	float nSheetSize = 64.0f;
	uint8_t m_FacingDirection;
	uint8_t m_VerticalFacingDirection;
	int m_GraphicState;
	uint8_t Frameindicator;

	float attackDirectionX;
	float attackDirectionY;
	float fDuration;
	float knockbacktime;

	float rageamount;

	int nDamage = 5;
	bool bOneHit = true;
	bool gettarget = false;

	uint8_t HitCount = 0;     // we will gather this in rpg_engine 
	uint8_t HitAmount = 1;   // how many times projectile should attack befroe switch in neutral



private:
	cDynamic_Creature* agrsr;

private:
	bool repitable = false;
	float angle =0.0f;
	
protected:
	enum
	{
		STRONGATTACK=0,
	       MIDATTACK=1,
		  EASYATTACK=2,
		  EVISCIRATE=3,
		  AIREVISCIRATE=4,
		  ATTACKBACK =5,
		  ATTACKVERTICAL =6,
		  ATTACKFOLLOWDOWN =7,
		  ATTACKLANDING =8
	}  SpriteStateFX;
	float animspr = 0.0f;
	float framespeed = 0.1f;

protected:
	
};

class cDynamic_HpBar :public cDynamic
{
public:
	cDynamic_HpBar( float ox, float oy, int HP, cDynamic_Creature* hpmember);
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) override;
	void Update(float fElapsedTime, cDynamic* player  = nullptr)override;
	void setLevelHp(int pam);

public:
	int16_t charHp;
	int16_t hpMax;
	//olc::Decal* HpFull = nullptr;
	//olc::Decal * HpEmpty = nullptr;
	cDynamic_Creature* Hpowner = nullptr;
	float procent;
	
private:
	int16_t soursceposX, soursceposY, soursceSizeX, soursceSizeY;


};



class cDynamic_TextDamage : public cDynamic
{
public:

	cDynamic_TextDamage();
	cDynamic_TextDamage(float px, float py, std::string DamageText, olc::Pixel COlor =olc::WHITE);
	void Update(float fElapsedTimeб, cDynamic* player = nullptr)override;
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)override;
	
	void SetText(std::string Text, olc::Pixel Color = olc::WHITE)
	{
		DamageText = Text;
		this->Color = Color;
		this->clearFlag(bRedundant);
	};
	void SetDeafult();

private:
	
	std::string DamageText;
	olc::Pixel Color;
	float time;
};


class Environment
{
public:
	Environment(std::string sname);
	~Environment();

public:
	float px, py;
	float speed;
	std::string sname;
	virtual void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) {}
	virtual void Update(float fElapsedTime, float screenwidth, float screenheight) {}

	static RPG_Engine* g_engine;
private:

};


class ERaindrop : public Environment
{

public:
	
	float angle; // Angle of descent for the raindrop
	
	olc::Decal* psprite;
	ERaindrop(float startX, float startY, float startSpeed, float startAngle);

	void Update(float fElapsedTime, float screenwidth, float screenheight) override;
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)override;


private:
	float initpx,initpy;
};


class Edynamic_Cloud : public Environment
{
public:
	Edynamic_Cloud(float px, float py,olc::Decal* psprite, float speed);
	void Update(float fElapsedTime, float screenwidth, float screenheight)override;
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)override;


private:
	
	
	int widthsprite;
	int heightsprite;
	olc::Decal* psprite;

};




class VfxParticles :public cDynamic
{
public:
	VfxParticles(float px, float py);
	void Update(float fElapsedTime, cDynamic* player = nullptr)override {};
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)override {};

	olc::Decal* VfxParticle = nullptr;
private:
	
};



class VfxLevel : public VfxParticles
{
public:
	VfxLevel(float px, float py);
	~VfxLevel()
	{
		m_vecVfxLevel.clear();
		m_speed.clear();
	};

	void Update(float fElapsedTime, cDynamic* player = nullptr)override;
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)override;


private:
	float AlphaColor;
	float time;
	float timecount;

	std::vector< olc::vf2d> m_vecVfxLevel;  //Hp Rage Energy and text
	std::vector<float >m_speed;

};

class VfxShot : public VfxParticles
{
public:
	VfxShot(float px, float py);

	~VfxShot()
	{
		m_vecVfxLevel.clear();
		m_speed.clear();
	};

	void Update(float fElapsedTime, cDynamic* player = nullptr)override;
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)override;


private:
	float AlphaColor;
	float time;
	float timecount;

	std::vector< olc::vf2d> m_vecVfxLevel;  //Hp Rage Energy and text
	std::vector<float >m_speed;

};
class VfxDeath : public VfxParticles
{

public:
	VfxDeath(cDynamic* DeathObject, olc::Decal* sprite, std::pair<olc::vi2d, olc::vi2d> particlesPos, bool west);
	

	void Update(float fElapsedTime, cDynamic* player = nullptr)override;
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)override;


private:
	float AlphaColor;
	float time;
	float timecount;
	float vxMult =5.0f;
	std::pair<olc::vi2d, olc::vi2d> framepositions;
	olc::vi2d currpicturePos;
	olc::Decal* sprite;
};