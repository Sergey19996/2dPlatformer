#pragma once
#include "RPG_Dynamic.h"

class RPG_Engine;
class InventaryItem;
class cItem
{
public:
	cItem(std::string name, olc::Decal* decal, std::string desc);

	cItem(const cItem &p);

	virtual bool OnInteract(cDynamic* object) { return false; }
	virtual bool OnUse(cDynamic* object, std::vector<InventaryItem*>& m_listItems, InventaryItem* socket =nullptr) { return false; }
	virtual bool OnUse(cDynamic* object) { return false; }
	virtual bool OnSell() ;
	virtual bool OnBuy();
	virtual bool OnCraft(bool click) { return false; };
public:
	std::string sName;
	std::string sDescription;
	std::string sCraftRequaries;
	uint8_t sTradePrice;   // <---Price for sell
	uint8_t sTradeBuyPrice; // <--Price for buy
	olc::Decal* pSprite;
	bool bKeyItem = false;

	unsigned int Attributes = 0;  // 15 - 1111   255 - 1111 1111


	static RPG_Engine* g_engine;
	

	uint32_t spriteindex = 0;
	int UiIndex = 0;

	uint8_t equipIndex = 0; // Index in equipment
	uint8_t MaxStack = 0;  // maxamount of stack in inventory
	uint8_t Gold = 0;

};




class  cEquip : public cItem
{
public:
	cEquip(std::string name, olc::Decal* sprite, std::string desc, uint8_t dmg, uint8_t aglty, uint8_t inteleg, uint8_t versality, uint8_t Strangth, uint32_t spriteindex, uint8_t equipindex,uint8_t tradeprice);
	bool OnInteract(cDynamic* object) override;
	bool OnUse(cDynamic* object) override;
	bool OnBuy()  override { return false; };
	bool OnCraft(bool click)  override { return false; };


	//int nDamage = 0;

	

	

private:

};






class cWeapon : public cEquip    //store is amount of damage  that particular weapon does
{
public:
	cWeapon(std::string name, olc::Decal* sprite,olc::Decal*ProjectileLeft,olc::Decal* ProjectileRight, std::string desc, uint8_t dmg, uint8_t aglty, uint8_t inteleg, uint8_t versality, uint8_t Strangth, uint32_t spriteindex, uint8_t equipIndex,uint8_t price);
	bool OnUse(cDynamic* object) override;
	bool OnInteract(cDynamic* object) override;

private:
	olc::Decal* ProjectileLeft;
	olc::Decal* ProjectileRight;



};


class cGold : public cItem    //store is amount of damage  that particular weapon does
{
public:
	cGold(std::string name, olc::Decal* sprite, std::string desc, int dmg);
	bool OnInteract(cDynamic* object) override;
	bool OnUse(cDynamic* object) override;
	bool OnBuy() override { return false; };
	bool OnCraft(bool click)  override { return false; };
	int GoldCount = 0;  // Describe gold state in wallet and splited coins
public:
	uint8_t MaxGold = 60;

protected:

};



class cCoin : public cGold
{
public:
	cCoin();

	bool OnInteract(cDynamic* object) override;
};

class cSmallWallet : public cGold
{
public:
	cSmallWallet();
	bool OnUse(cDynamic* object, std::vector<InventaryItem*>& m_listItems, InventaryItem* Socket =nullptr) override;
	bool OnBuy() override;
	bool OnCraft(bool click) override;
	bool OnSell() override;
private:
	
	//CraftZone
	std::string FirstItem = "Broken Sword";
	std::string SecondItem = "Pantir's Dagger";

	uint8_t MaxGold = 60;
};


class cFullSmallWallet : public cGold
{
public:
	cFullSmallWallet();
	//bool OnUse(cDynamic* object, std::vector<InventaryItem*>& m_listItems, InventaryItem* Socket = nullptr) override;
	//bool OnBuy() override;
	//bool OnCraft(bool click) override;
private:

	//CraftZone
//	std::string FirstItem = "Broken Sword";
//	std::string SecondItem = "Pantir's Dagger";
};



class cEmptySlot : public cItem
{
public:
	cEmptySlot();


	bool OnUse(cDynamic* object) override;
private:
};


class cHealthElixir : public cItem
{
public:
	cHealthElixir();

 bool OnUse(cDynamic* object, std::vector<InventaryItem*>& m_listItems, InventaryItem* socket =nullptr) override;
private:

	
};


class cEnergyElixir : public cItem
{
public:
	cEnergyElixir();

	bool OnUse(cDynamic* object, std::vector<InventaryItem*>& m_listItems, InventaryItem* socket =nullptr) override;
private:

	

};

class cRageElixir : public cItem
{
public:
	cRageElixir();

	bool OnUse(cDynamic* object, std::vector<InventaryItem*>& m_listItems, InventaryItem* socket =nullptr) override;
private:

	

};


