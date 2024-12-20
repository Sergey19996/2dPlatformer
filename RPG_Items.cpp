#include "RPG_Items.h"
#include "RPG_Engine.h"

RPG_Engine* cItem::g_engine = nullptr;


cItem::cItem(std::string name, olc::Decal* decal, std::string desc,  uint32_t sprtindex,bool keyitem)
{
	sName = name; pSprite = decal; sDescription = desc; bKeyItem = keyitem; spriteindex = sprtindex;
}

cItem::cItem(const cItem& p)
{
	sName = p.sName; pSprite = p.pSprite; sDescription = p.sDescription;
}

bool cItem::OnSell()    // Take gold for objects   sell them in shop  
{
//	g_engine->SaleItem(sTradePrice, this);
	

	return true;
}
bool cItem::OnBuy()    // Take gold for objects   sell them in shop  
{

	return true;
}

cWeapon::cWeapon(std::string name, olc::Decal* sprite,olc::Decal* PrjctLeft,olc::Decal* PrjctRight, std::string desc, uint8_t dmg, uint8_t aglty, uint8_t inteleg, uint8_t versality, uint8_t Strangth,uint32_t spriteindex,uint8_t equipIndex, uint8_t price ) : cEquip(name,sprite,desc,dmg,aglty,inteleg,versality,Strangth,spriteindex,equipIndex,price)
{
	
	ProjectileLeft = PrjctLeft;
	ProjectileRight = PrjctRight;
	sTradePrice = price;
	this->equipIndex = 3;  // - when equipIndex =0  , Means that we can't equip item


}

bool cWeapon::OnInteract(cDynamic* object)
{
	return true;
}

bool cWeapon::OnUse(cDynamic* object, bool toggle)
{
	// When weapons are used, they are used on the object that owns the weapon, i.e.
		// the attacker. However this does not imply the attacker attacks themselves

		// Get direction of attacker
	cDynamic_Creature* aggressor = (cDynamic_Creature*)object;



	// Determine attack origin
	float x, y, vx, vy, attackdir;



	



	if (aggressor->GetFacingDirection() == 1) // West
	{
		x = aggressor->px+1.0f;
		y = aggressor->py+1.0f;
		vx = aggressor->vx; vy = aggressor->vy;

		attackdir = -aggressor->attackdirectionX;
	}
	else
	{
		x = aggressor->px+1.0f;
		y = aggressor->py+1.0f;
		vx = aggressor->vx; vy = aggressor->vy;

		attackdir = aggressor->attackdirectionX;

	}

	if (toggle)   // for escape position  (mainchar escape it)
	{

	if (aggressor->GetFacingDirectionVertical() == 1)  // Up
	{
		x = aggressor->px;
		y = aggressor->py;
		vx = 0; vy = aggressor->vy;
		
		aggressor->setFlag(aggressor->isprojEqualX);  // px projectile be on the same position where owner 
	

	}
	if (aggressor->GetFacingDirectionVertical() == 0)  // Down
	{
		x = aggressor->px;
		y = aggressor->py;
		vx = 0; vy = aggressor->vy;
		aggressor->setFlag(aggressor->isprojEqualX);
	
	

	}

	}



	if (aggressor->checkFlag(aggressor->isprojfollow))  // fallendownEviscirate
	{
		
		olc::vf2d pos{ x, y };
		
	

		cDynamic_Projectile* p = (cDynamic_Projectile*)g_engine->SpawnProjectile(pos);
		p->SetSprites(ProjectileRight, ProjectileLeft);
		p->SetAgressorData(aggressor);
		p->bSolidVsMap = true;
		p->bOneHit = true;
		p->attackDirectionX = attackdir;
		p->rageamount = aggressor->rageset;

		aggressor->clearFlag(aggressor->isprojEqualX);
	
		return false;
	}
	if (aggressor->checkFlag(aggressor->IsThrow))  // upflighteviscirate
	{

		olc::vf2d pos{ x, y };
	
		cDynamic_Projectile* p = (cDynamic_Projectile*)g_engine->SpawnProjectile(pos);
		p->px = x;
		p->py = y;
		p->attackDirectionX = attackdir;
		p->SetSprites(ProjectileRight, ProjectileLeft);
		p->SetAgressorThowData(aggressor, 1);

		//cDynamic_Projectile* p = new cDynamic_Projectile(x, y, aggressor->m_layer, aggressor->sparedVx, aggressor->sparedVy, 1, ProjectileLeft, ProjectileRight, aggressor->FxColumn, aggressor->m_nShockTime, aggressor->rageset);
		
		
		aggressor->clearFlag(aggressor->isprojEqualX);
		
		return false;
	}
	
	aggressor->clearFlag(aggressor->isprojEqualX);
	
	return false;
}

cGold::cGold(std::string name, olc::Decal* sprite, std::string desc, int amount) : cItem(name, sprite, desc,3)
{
	this->GoldCount = amount;

	//spriteindex = 3;

	//bKeyItem = true;

	Gold = 0;   // when we take it in inventary we add it
	
}

bool cGold::OnInteract(cDynamic* object)
{
	
	return true;
}

bool cGold::OnUse(cDynamic* object)
{
	
	return false;
}


cCoin::cCoin() : cGold("Coin", RPG_Assets::get().GetSprite("Items"), "currency\n", 1)
{
	
	spriteindex = 3	;
	MaxStack = 10;
	Gold = 1;   // when we take it in inventary we add it
	MaxGold = 10;
}

bool cCoin::OnInteract(cDynamic* object)
{

//	g_engine->goldcount++;

	return true;
}

cSmallWallet::cSmallWallet() : cGold("SmallWallet", RPG_Assets::get().GetSprite("Items"), "\nKeep 60 golds.\n",0)
{
	MaxStack = 0;
	bKeyItem = false;
	sTradePrice = 2;       // �� ������� ������� � �����
	sTradeBuyPrice = 5;    //������� ����� � ������
	
	sCraftRequaries = std::to_string(1)+FirstItem + "\n"  + std::to_string(1)+SecondItem + "\n";
	spriteindex = 65;
	MaxGold = 60;
}

bool cSmallWallet::OnUse(cDynamic* object, std::vector<InventaryItem*>& m_listItems, InventaryItem* socket)
{
	
	
	




	for (auto& item : m_listItems)  // go in inv
	{
		

		if (socket->Gold >=MaxGold)   //when sin wallet socket gold more or equal than max we out
		{
			break;
		}

		if (item->Item !=nullptr)    // when socket is not empty 
		{  


			if (item->Item->Gold > 0 && socket->Gold < MaxGold)   // item has gold number higher than zero and socket gold less than max
		{

			int8_t balance = socket->Gold + item->Gold - MaxGold;    // we find  reminder
			 
			if (balance > 0)    // when reminder  more  than zero
			{
				socket->Gold = MaxGold;
				item->Gold = balance;

				socket->Item = RPG_Assets::get().GetItem("FullSmallWallet");
			}
			else if (balance == 0)   // when  we get to max wallet space and coins is over
			{
				socket->Gold = MaxGold;
				item->Gold = 0;
				item->Item = nullptr;
				item->clearFlag(item->Breserved);

				socket->Item = RPG_Assets::get().GetItem("FullSmallWallet");
			}
			else{     // when coins and wallet less than max  


				
				socket->Gold += (item->Gold);
				item->Gold = 0;
				item->Item = nullptr;
				item->clearFlag(item->Breserved);
			}
				
			
				
				
			
			//item->Item = nullptr;
			//item->clearFlag(item->Breserved);
			//item->Absorbed = true;  // absorb coin
			//g_engine->absorbCounter++; // COunter for cyle when we need to find cells for delete
			//this->GoldCount++;
			//sDescription = "Keep 60 golds:  " + std::to_string(socket->Gold)+ "/60" + "\n" + "Shalalala dont be scared"+'\n'+"you got to mood prepare\n";
			//spriteindex = 66;
			continue;
			


		}
		
		}
	}


	



	return true;
}

bool cSmallWallet::OnBuy()
{
	if (g_engine->CheckMoney() >= sTradeBuyPrice)
	{
		g_engine->TakeItem(this->sTradeBuyPrice); // first take item froms inventory for get free the space 

	g_engine->GiveItem(this->sName,1,false);  // then we get bought object
	return true;
	}
	
}
bool cSmallWallet::OnCraft(bool click)
{
	


	if (g_engine->HasItem(FirstItem) && g_engine->HasItem(SecondItem))
	{
		if (click==1)
		g_engine->GiveItem(this->sName,1,false);
	return true;
	}
	
	return false;
}

bool cSmallWallet::OnSell()
{
	return false;
}

cEquip::cEquip(std::string name, olc::Decal* sprite, std::string desc, uint8_t dmg, uint8_t aglty, uint8_t inteleg, uint8_t versality, uint8_t strength, uint32_t sprtindex, uint8_t equipindex, uint8_t price ) : cItem(name, sprite, desc, sprtindex,false)
{

	// ��������� �������� 4 ������ (�������� �� 0 �� 15)
	Attributes |= (aglty & 0xF) << 0;       // ���������� agility � ������ 4 ���� (0-3)
	Attributes |= (inteleg & 0xF) << 4;     // ���������� intelligence � ��������� 4 ���� (4-7)
	Attributes |= (versality & 0xF) << 8;   // ���������� versatility � ���� 8-11
	Attributes |= (strength & 0xF) << 12;   // ���������� strength � ���� 12-15
	Attributes |= (dmg & 0xF) << 16;        // ���������� DMg � ���� 16-20


	equipIndex = equipindex;
	sTradePrice = price;


	switch (spriteindex)
	{
	case 35:                        // Neck
		FirstItem = "Cooper";
		SecondItem = "White Nuggets";
		sCraftRequaries = std::to_string(1) + FirstItem + "\n" + std::to_string(1) + SecondItem + "\n";;
		break;

	case 32:                        //Helment
		FirstItem = "Piece Of Leather";
		SecondItem = "Cooper";
		sCraftRequaries = std::to_string(10) + FirstItem + "\n" + std::to_string(3) + SecondItem + "\n";;
		break;
	case 33:                       // Chest 
		FirstItem = "Piece Of Leather"; 
		SecondItem = "Cooper";
		ThirdItem = "Red Sample";
		sCraftRequaries = std::to_string(10) + FirstItem + "\n" + std::to_string(1) + SecondItem + "\n" + std::to_string(1) + ThirdItem + "\n";
		break;

	case 34:
		sTradeBuyPrice = 100;
		break;
	

	}
	//sCraftRequaries = std::to_string(1) + FirstItem + "\n" + std::to_string(1) + SecondItem + "\n"+ std::to_string(1) + ThirdItem + "\n"+ std::to_string(1) + FourItem;
}

bool cEquip::OnInteract(cDynamic* object)
{
	return false;
}

bool cEquip::OnUse(cDynamic* object)
{
	return false;
}

bool cEquip::OnCraft(bool click)
{


			if (g_engine->HasItem(FirstItem) && g_engine->HasItem(SecondItem)&& g_engine->HasItem(ThirdItem)&& g_engine->HasItem(FourItem))
			{
				if (click == 1)
					g_engine->GiveItem(this->sName, 1);
				return true;
			}

	return false;
}



cEmptySlot::cEmptySlot() : cItem("Empty", nullptr, "Empty SLot\n",0)
{
	Attributes = 0;

}

bool cEmptySlot::OnUse(cDynamic* object)
{
	return false;
}

cHealthElixir::cHealthElixir() : cItem("Health Elixir", RPG_Assets::get().GetSprite("Items"), "Good Smelling Elixir,\nRecover 100 Helth\n",1)
{
	
	MaxStack = 10;
	sTradePrice = 2;       // �� ������� ������� � �����
	sTradeBuyPrice = sTradePrice * 10;    //������� ����� � ������

	consumable = true;
}

bool cHealthElixir::OnUse(cDynamic* object, InventaryItem* socket)
{
	int currHealth = object->GetHealth();
	currHealth += 100;

	if (socket->currStacks > 1)
	{
		socket->currStacks--;
		g_engine->AddText(object->px + object->CollbordersX + 0.25f, object->py + object->CollbordersY + 0.6f, "+ 100 Health");
		object->SetHealth(currHealth);
		return false;  // <--Return true  when object will die
	}
	else
	{
		socket->Item = nullptr;
		socket->clearFlag(socket->Breserved);
		g_engine->AddText(object->px + object->CollbordersX + 0.25f, object->py + object->CollbordersY + 0.6f, "+ 100 Health");
		object->SetHealth(currHealth);
		return true;  // <--Return true  when object will die
	}


	return true;  // <--Return true  when object will die
}

cEnergyElixir::cEnergyElixir() : cItem("Energy Elixir", RPG_Assets::get().GetSprite("Items"), "Rare Elixir,\nRecover 60 Energy\n",2)
{
	
	MaxStack = 10;
	sTradePrice = 3;       // �� ������� ������� � �����
	sTradeBuyPrice = sTradePrice * 10;    //������� ����� � ������
	consumable = true;
}

bool cEnergyElixir::OnUse(cDynamic* object, InventaryItem* socket)
{
	cDynamic_creature_Pantir* user = (cDynamic_creature_Pantir*)object;

	int currHealth = user->GetEnergy();
	currHealth += 60;
	if (socket->currStacks > 1)
	{
		socket->currStacks--;

		g_engine->AddText(object->px + object->CollbordersX + 0.25f, object->py + object->CollbordersY + 0.6f, "+ 60 Energy");

		user->SetEnergy(currHealth);
		return false;
	}
	else
	{
		socket->Item = nullptr;
		socket->clearFlag(socket->Breserved);

		g_engine->AddText(object->px + object->CollbordersX + 0.25f, object->py + object->CollbordersY + 0.6f, "+ 60 Energy");

		user->SetEnergy(currHealth);
		return true;
	}




	
}

cRageElixir::cRageElixir() : cItem("Rage Elixir", RPG_Assets::get().GetSprite("Items"), "Very Rare Elixir,\nRecover 30 Rage\n",0)
{
	
	MaxStack = 10;
	sTradePrice = 5;       // �� ������� ������� � �����
	sTradeBuyPrice = sTradePrice * 10;    //������� ����� � ������
	consumable = true;
}

bool cRageElixir::OnUse(cDynamic* object, InventaryItem* socket)
{
	cDynamic_creature_Pantir* user = (cDynamic_creature_Pantir*)object;

	int currHealth = user->GetRage();
	currHealth += 30;
	if (socket->currStacks > 1)
	{
		socket->currStacks--;
		g_engine->AddText(object->px + object->CollbordersX + 0.25f, object->py + object->CollbordersY + 0.6f, "+ 30 Rage");
		user->SetRage(currHealth);
		return false;
	}
	else
	{
		socket->Item = nullptr;
		socket->clearFlag(socket->Breserved);
		g_engine->AddText(object->px + object->CollbordersX + 0.25f, object->py + object->CollbordersY + 0.6f, "+ 30 Rage");
		user->SetRage(currHealth);
	}

	//Absorbed = true;  // absorb Elixir
	//g_engine->absorbCounter++;



	return true;
}

cFullSmallWallet::cFullSmallWallet() : cGold("FullSmallWallet", RPG_Assets::get().GetSprite("Items"), "Keep 6 golds:  " + std::to_string(60) + "/60\n", 0)
{
	spriteindex = 66;
}

cWeaponBanditBoss::cWeaponBanditBoss() : cWeapon("Bandit Boss Sword", RPG_Assets::get().GetSprite("Items"), RPG_Assets::get().GetSprite("Bandit Boss SwordLeftFx"), RPG_Assets::get().GetSprite("Bandit Boss SwordRightFx"), "usually these kind of swords \nare used by bandits\n", 10, 0, 0, 0, 0, 67, 3, 3)
{

}

PieceOfLeather::PieceOfLeather() : cItem("Piece Of Leather", RPG_Assets::get().GetSprite("Items"), "ingredient for,\nCraft\n", 32 * 4)
{
	//spriteindex = 32 * 4;
	MaxStack = 10;
	sTradePrice = 3;
	sTradeBuyPrice = sTradePrice * 3;    //������� ����� � ������
}

WhiteNuggets::WhiteNuggets() : cItem("White Nuggets", RPG_Assets::get().GetSprite("Items"), "ingredient for,\nCraft\n", 32 * 3 + 1)
{
	//spriteindex = 32 * 3 + 1;
	MaxStack = 10;
	sTradePrice = 10;
	sTradeBuyPrice = sTradePrice * 3;    //������� ����� � ������
}

RedSample::RedSample() : cItem("Red Sample", RPG_Assets::get().GetSprite("Items"), "ingredient for,\nCraft\n", 32 * 3 + 2)
{
	//spriteindex = 32 * 3 + 2;
	MaxStack = 10;
	sTradePrice = 13;
	sTradeBuyPrice = sTradePrice *3;    //������� ����� � ������
}

copper::copper() : cItem("Cooper", RPG_Assets::get().GetSprite("Items"), "ingredient for,\nCraft\n",5)
{
	//spriteindex = 5;
	MaxStack = 1;
	sTradePrice = 20;
	sTradeBuyPrice = 60;    //������� ����� � ������
}
