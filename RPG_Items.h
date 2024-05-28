#pragma once
#include "RPG_Dynamic.h"

class RPG_Engine;

class cItem
{
public:
	cItem(std::string name, olc::Decal* decal, std::string desc);

	cItem(const cItem &p);

	virtual bool OnInteract(cDynamic* object) { return false; }
	virtual bool OnUse(cDynamic* object) { return false; }
	virtual bool OnSell() ;
	virtual bool OnBuy();
	virtual bool OnCraft(bool click) { return false; };
public:
	std::string sName;
	std::string sDescription;
	std::string sCraftRequaries;
	int sTradePrice;   // <---Price for sell
	int sTradeBuyPrice; // <--Price for buy
	olc::Decal* pSprite;
	bool bKeyItem = false;
	bool bEquipable = false;

	bool Absorbed = false;

	static RPG_Engine* g_engine;
	

	float ipx = 0;    // Offset in inventory
	float ipy = 0;
	bool mouseGrabbed = false;
	bool objectselected = false;
	int invnumber = 0;  // Index in inventory
	bool bInWarehouse = false;

	int GoldCount = 0;  // Describe gold state in wallet and splited coins

};


class cWeapon : public cItem    //store is amount of damage  that particular weapon does
{
public:
	cWeapon(std::string name, olc::Decal* sprite, std::string desc, int dmg);
	bool OnInteract(cDynamic* object) override;
	bool OnUse(cDynamic* object) override;
	bool OnBuy()  override { return false; };
	bool OnCraft(bool click)  override { return false; };
public:
	int nDamage = 0;


};


class cGold : public cItem    //store is amount of damage  that particular weapon does
{
public:
	cGold(std::string name, olc::Decal* sprite, std::string desc, int dmg);
	bool OnInteract(cDynamic* object) override;
	bool OnUse(cDynamic* object) override;
	bool OnBuy() override { return false; };
	bool OnCraft(bool click)  override { return false; };
public:


};



class cCoin : public cGold
{
public:
	cCoin();


};

class cSmallWallet : public cGold
{
public:
	cSmallWallet();
	bool OnUse(cDynamic* object) override;
	bool OnBuy() override;
	bool OnCraft(bool click) override;
private:
	
	//CraftZone
	std::string FirstItem = "Broken Sword";
	std::string SecondItem = "Pantir's Dagger";
};




class cWeapon_PantirSword : public cWeapon
{
public:
	cWeapon_PantirSword();

public:
	bool OnUse(cDynamic* object) override;
};

class cWeapon_BanditSword : public cWeapon
{
public:
	cWeapon_BanditSword();

public:
	bool OnUse(cDynamic* object) override;
};


class cWeapon_BanditBossSword : public cWeapon
{
public:
	cWeapon_BanditBossSword();

public:
	bool OnUse(cDynamic* object) override;
};




class cWeapon_BrokenSword : public cWeapon
{
public:
	cWeapon_BrokenSword();

public:
	bool OnUse(cDynamic* object) override;


};

