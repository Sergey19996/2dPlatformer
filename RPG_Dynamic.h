#pragma once
#include "RPG_Assets.h"
#include "olcPixelGameEngine.h"

class RPG_Engine;

class cWeapon;
class cItem;

class cIndicator;
class cDynamic_HpBar;
class cDynamic_EnergyBar;
class cDynamic_RageBar;
class cComand;

class cDynamic
{
public:
	cDynamic(std::string n);
	~cDynamic();

public:
	float px, py;
	float vx, vy;
	bool bSolidVsMap;

	enum LAYER
	{ Friend = 0, Enemy = 1, Neutral = 2, Particle = 3,Questable =4 } m_layer;

	bool bIsProjectile;
	bool bDraw = true;
	bool bIsAttackable;
	bool bRedundant;        //translate like unnecessary
	bool bDead = false;
	std::string sName;
	bool bOnGround;
	int Jumpcounter;
	bool bOnTarget = false;

	bool binitialized = false;


	bool bControllable = true;

	int mass = 32;
	bool gravity = true;
	
	std::pair<olc::vf2d*, olc::vf2d*>* obsticlepoints = nullptr;

	float CollbordersX, CollbordersY, CollbordersXF, CollbordersYF;

	bool quested = false;

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
	cDynamic_Creature(std::string n, olc::Decal* spriteRight, olc::Decal* spriteLeft);       // <--Sprite

protected:
	olc::Decal* m_pSpriteRight;
	olc::Decal* m_pSpriteLeft;

	
	float m_fTimer;
	enum {  WEST = 1, EAST = 3 } M_nFacingDirection;
	enum { SOUTH = 0,  NORTH = 1, NOTLOOKING =3 } M_nFacingDirectionVertical;

	enum{STRENGTH =0, AGILITY =1,INTELLIGENCE=2 }M_BaseClass;

	int Defence = 0;
	int AverageAttack = 0;
	int calculatedDamage = 0;
	float fHpRegeneration = 0;
	int Movementspeed = 0;
	



	int versality = 0;

	int agility = 0;
	float Haste = 0;
	int strength = 0;
	int intelligence = 0;


	bool isDirectionLock =false;
	
	enum
	{
		Idle = 0,
		Walk = 1,
		Jump = 2,
		Attack = 3,
		AttackEasy =4,
		AttackGreat =5,

		DoubleJump = 6,
		AirAttack = 7,
		Eviscirate = 8,       //  Right Botton

		JumpDown = 9,
		JumpZenit = 10,
		Landing = 11,

		AirEviscirate = 12,   //  Right Botton
		Hit =13,

		AttackBack = 16,

		UpEviscirate =17,      //  Right Botton
		AirDownEviscirate =18,    //  Right Botton
		LANDINGEviscirate = 19,    //  Right Botton
		BLINKDESAPEAR =20,
		GRAB =21,
		Swirl =22,
		Death =23,
		IdleTwo =24,
		IdleReaction =25


	}  M_nGraphicState;

	float m_fStateTick;  //deplete every frame  check on agro in behaviour

	float m_fKnockBackTimer = 0.0f;
	float m_fKnockBackDX = 0.0f;
	float m_fKnockBackDY = 0.0f;

	
	int redColor = 255;      //<--colors
	int greenColor = 255;
	int blueColor = 255;

	
public:
	float ProjCollbordersX, ProjCollbordersY, ProjCollbordersXF, ProjCollbordersYF;

	//when need stop gravity for seconds use it
	float gravityofftimer = 0.0f;
	//

	int moveBonus = 0;   // increase it when we use shadow form. it add in A and D logic

	int FxColumn;

	int nHealth;
	int nHealthMax;
	
	int enumCounter;
	
	/// ///
	float attackdirectionX = 0.0f;       //in indicators equal enumirator of attack 
	float attackdirectionY = 0.0f;       // we can modify direction of victim in depend of attack
	
	bool isAttack ;
	bool IsThrow;
	bool IsOnePlay;
	bool bOnLanded =false;
	bool bAnimAction = false;   // <--Situation, when we need act animation  while not ended  abuse onladinind idle and other
	bool isprojfollow =false;


	bool Btarget = false; 

	// working with drawning sprites
	int m_nGraphicCounterX;
	int m_nGraphicCounterY;
	float m_nShockTime;


	int frameIndicator;
	float framespeed;
	float animspr ;
	int m_nGraphicAmountFrames;
	int m_nGraphicEndFrame;

	//bool FxGo;
	int FXFrame;

	///////
	float nSheetOffsetY, nSheetOffsetX;
	float nSheetSizeX = 128.0f;
	float nSheetSizeY = 128.0f;
	float sparedVx = 0;

	int rageset = 0;   // <- parametr for calculate rage indicator
	

	int LvL = 0;
	int Hittimes = 0;
	 
public:
	
	virtual void Behaviour(float fElapsedTime, cDynamic* player = nullptr);
	virtual void ReturnBaseLayer() {};
	virtual void DeathFun() {};
	virtual void IndicateAnim() {}  // using after setenum// define sprites positions
	virtual void PerformAttack() {};
	virtual void KnockBack(float dx, float dy, float dist);
	virtual void Action() {};


	void Update(float fElapsedTime, cDynamic* player = nullptr) override;
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) override;
	void SetAnimation(int num) { enumCounter = num; IsOnePlay = true; };
	void SetHealth(int setHealth) override { nHealth -= setHealth; };
	void calculateStats();
	void GravityControl(float felapsedtiem);
	void SetUpDamage();

	


	bool IsLanded()  override;
	bool setEnum();     // <-keep the parametrs which need for use animtions



	int GetFacingDirection() { return M_nFacingDirection; };
	int GetFacingDirectionVertical() { return M_nFacingDirectionVertical; };
	int calculateDeathExp();
	int GetAttack() { return  calculatedDamage; };
	int GetDefence() { return Defence; };
	int GetHealth() override { return nHealth; };

	cWeapon* pEquipedWeapon = nullptr;
	cDynamic_HpBar* hpUpdate = nullptr;
	cDynamic_EnergyBar* EnergyUpdate = nullptr;
	cDynamic_RageBar* RageUpdate = nullptr;
	

	

	float ProjOffsetX = 0;
	float ProjOffsetY =0;
protected:
	bool bKnockBack = true;
	float BasicAgility = 0;
	float BasicStrength = 0;
	float BasicIntelect = 0;
	float BasicMovementSpeed = 0;


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
	int looptimes = 3;

	int attackdif = 0;


};



class  cDynamic_creature_Bandit : public cDynamic_creature_Enemy
{

public:
	cDynamic_creature_Bandit();
	void IndicateAnim() override;
	
	void SpecAttack(float targetX, float targetY, float Distance) override;
	

	void ReturnBaseLayer()override { m_layer = Enemy; };
	void AttackOne() override;
	void AttackTwo() override;

	
	void DeathFun() override;

	
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
	

};


class  cDynamic_creature_NPCBandit : public  cDynamic_Creature
{

public:

	cDynamic_creature_NPCBandit(std::string sname);

	void IndicateAnim() override;

};


class  cDynamic_ClipTrigger : public  cDynamic
{

public:

	cDynamic_ClipTrigger(std::string sname);
	


};




class cDynamic_creature_Pantir : public cDynamic_Creature    //represent player
{

public:
	cDynamic_creature_Pantir();
	
	//void PerformAttack() override;
	
	
	void  SwirlGrab(cDynamic* dyn);

	void Behaviour(float fElapsedTime, cDynamic* player = nullptr);
	void PerformAttack() override;

	bool checkFacingDirection(cDynamic* Enemy = nullptr);
	void IndicateAnim() override;
	
	int GetStats(int num);

	bool ConnectTarget(cDynamic* takenTarget) 
	
	{
		if (takenTarget !=nullptr)
		{



		Target = takenTarget;
		Target->bOnTarget = true;
		return true;

		}
		Target = takenTarget;
		return false; 
	};
	cDynamic* Gettarget()
	{if (Target != nullptr) 
	return Target;
	return nullptr;}

	
	int GetMovement() { return (this->Movementspeed+this->moveBonus)/10; };

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

	
	bool IsLanded()  override;
	void DeathFun() override;

	void ReturnBaseLayer()override 
	{ m_layer = Friend; };


	float energyCount = 0;   // <counter of energe. thanks felapsed time it's gain 1 and than add it in energy amount
	float fAttackcount = 0;    // <-- Counter for sinister strices;  help count logic when we dragged lef mouse and released 
	int energeyAmount = 100;


	float rageCount = 0;      // <counter of Rage. thanks felapsed time it's gain 1 and than substruct it in  rage amount
	
	int rageAmount = 100;
	bool bHideMode = false;

private:

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
	cDynamic_Item(float x, float y, cItem* item);
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) override;
	void OnInteract(cDynamic* player = nullptr) override;

public:
	cItem* item;
	bool bCollected = false;

	
	


};


class cDynamic_Projectile : public cDynamic
{
public:
	cDynamic_Projectile(float ox, float oy,  LAYER mlayer, float velx, float vely, float duration, olc::Decal* pSpriteRight, olc::Decal* pSpriteLeft ,  int m_GraphicState, float m_nGraphicTime, int rageset);
	cDynamic_Projectile(olc::Decal* pSpriteRight, olc::Decal* pSpriteLeft, cDynamic_Creature* Aggressor) ;
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) override;
	void Update(float fElapsedTime, cDynamic* player = nullptr) override;
	void SwitchLayer(int num);
	void SetTimer(int charstate);
	int setEnum(int charstate);
	

public:
	olc::Decal* pSpriteRight = nullptr;
	olc::Decal* pSpriteLeft = nullptr;
	

	float nSheetSize = 128.0f;
	int m_FacingDirection;
	int m_VerticalFacingDirection;
	int m_GraphicState;
	int Frameindicator;

	float attackDirectionX;
	float attackDirectionY;
	float fDuration;
	float knockbacktime;

	float rageamount;

	int nDamage = 5;
	bool bOneHit = true;
	bool gettarget = false;

	int HitCount = 0;     // we will gather this in rpg_engine 
	int HitAmount = 1;   // how many times projectile should attack befroe switch in neutral



private:
	cDynamic_Creature* agrsr;

private:
	bool repitable = false;
	
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
	cDynamic_HpBar( float ox, float oy, olc::Decal* HpFull, olc::Decal* HpEmpty,int HP, cDynamic_Creature* hpmember);
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) override;
	void Update(float fElapsedTime, cDynamic* player  = nullptr)override;
	void setLevelHp(int pam);

public:
	int charHp;
	int hpMax;
	olc::Decal* HpFull = nullptr;
	olc::Decal * HpEmpty = nullptr;
	cDynamic_Creature* Hpowner = nullptr;
	float procent;
	

};



class cDynamic_TextDamage : public cDynamic
{
public:
	cDynamic_TextDamage(float px, float py, std::string DamageText, olc::Pixel COlor =olc::WHITE);
	void Update(float fElapsedTimeá, cDynamic* player = nullptr)override;
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)override;

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