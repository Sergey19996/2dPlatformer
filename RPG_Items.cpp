#include "RPG_Items.h"
#include "RPG_Engine.h"

RPG_Engine* cItem::g_engine = nullptr;


cItem::cItem(std::string name, olc::Decal* decal, std::string desc)
{
	sName = name; pSprite = decal; sDescription = desc;
}

cItem::cItem(const cItem& p)
{
	sName = p.sName; pSprite = p.pSprite; sDescription = p.sDescription;
}

bool cItem::OnSell()    // Take gold for objects   sell them in shop  
{

	g_engine->GiveItem(new cCoin);


	return true;
}
bool cItem::OnBuy()    // Take gold for objects   sell them in shop  
{
	return true;
}

cWeapon::cWeapon(std::string name, olc::Decal* sprite, std::string desc, int dmg) : cItem(name,sprite,desc)
{
	nDamage = dmg;
}

bool cWeapon::OnInteract(cDynamic* object)
{
	return true;
}

bool cWeapon::OnUse(cDynamic* object)
{
	return false;
}

cGold::cGold(std::string name, olc::Decal* sprite, std::string desc, int amount) : cItem(name, sprite, desc)
{
	this->GoldCount = amount;

	bKeyItem = true;
}

bool cGold::OnInteract(cDynamic* object)
{
	
	return true;
}

bool cGold::OnUse(cDynamic* object)
{
	
	return false;
}


cWeapon_PantirSword::cWeapon_PantirSword() : cWeapon("Pantir's Dagger", RPG_Assets::get().GetSprite("Pantir's Dagger"), "dagger, 5 dmg\n", 5)
{
	
}

bool cWeapon_PantirSword::OnUse(cDynamic* object)
{
	// When weapons are used, they are used on the object that owns the weapon, i.e.
			// the attacker. However this does not imply the attacker attacks themselves

			// Get direction of attacker
	cDynamic_Creature* aggressor = (cDynamic_Creature*)object;


	// Determine attack origin
	float x, y, vx, vy, attackdir;
	

	if (aggressor->GetFacingDirection() == 1) // East
	{
		x = aggressor->px - 0.95f;
		y = aggressor->py;
		vx = aggressor->vx; vy = aggressor->vy;

		attackdir = -aggressor->attackdirectionX;
	}



	if (aggressor->GetFacingDirection() == 3) // West
	{
		x = aggressor->px + 0.85f;
		y = aggressor->py;
		vx = aggressor->vx; vy = aggressor->vy;

		attackdir = aggressor->attackdirectionX;
	}
	if (aggressor->GetFacingDirectionVertical() == 1)  // Up
	{
		x = aggressor->px;
		y = aggressor->py;
		vx = 0; vy = aggressor->vy;
		aggressor->sparedVx = 0;


	}
	if (aggressor->GetFacingDirectionVertical() == 0)  // Down
	{
		x = aggressor->px;
		y = aggressor->py;
		vx = 0; vy = aggressor->vy;
		aggressor->sparedVx = 0;


	}

	


	if (aggressor->isprojfollow)  // fallendownEviscirate
	{

		cDynamic_Projectile* p = new cDynamic_Projectile(RPG_Assets::get().GetSprite("Pantir's DaggerLeftFx"), RPG_Assets::get().GetSprite("Pantir's DaggerRightFx"), aggressor);
		p->px = x;
		p->py = y;
		p->bSolidVsMap = true;
		p->nDamage = aggressor->GetAttack();
		p->bOneHit = true;
		p->gettarget = aggressor->Btarget;
		//collision
		p->CollbordersX = aggressor->ProjCollbordersX;
		p->CollbordersXF = aggressor->ProjCollbordersXF;
		p->CollbordersY = aggressor->ProjCollbordersY;
		p->CollbordersYF = aggressor->ProjCollbordersYF;
		//which way aim fly
		p->attackDirectionX = attackdir;
		p->attackDirectionY = aggressor->attackdirectionY;
		p->m_FacingDirection = aggressor->GetFacingDirection();
		p->m_VerticalFacingDirection = aggressor->GetFacingDirectionVertical();
		p->HitAmount = aggressor->Hittimes;  //   < -- we add how many times our projectile should take damage before leave  default 1

		g_engine->AddProjectile(p);   // add in  Rpg_Engine class prijectile list this projectile
		return false;
	}
	if (aggressor->IsThrow == true)  // upflighteviscirate
	{



		cDynamic_Projectile* p = new cDynamic_Projectile(x, y, aggressor->m_layer, aggressor->sparedVx * 2, vy, aggressor->m_nGraphicAmountFrames * 0.1f, RPG_Assets::get().GetSprite("Pantir's DaggerLeftFx"), RPG_Assets::get().GetSprite("Pantir's DaggerRightFx"), aggressor->FxColumn, aggressor->m_nShockTime, aggressor->rageset);
		p->bSolidVsMap = true;
		p->nDamage = aggressor->GetAttack();
		p->bOneHit = true;
	
		p->HitAmount = aggressor->Hittimes;  //   < -- we add how many times our projectile should take damage before leave  default 1
		p->gettarget = aggressor->Btarget;
		p->CollbordersX = aggressor->ProjCollbordersX;
		p->CollbordersXF = aggressor->ProjCollbordersXF;
		p->CollbordersY = aggressor->ProjCollbordersY;
		p->CollbordersYF = aggressor->ProjCollbordersYF;

		p->attackDirectionX = attackdir;
		p->attackDirectionY = aggressor->attackdirectionY;

		//p->CollbordersY = 1;
		//p->CollbordersYF = 3;
		g_engine->AddProjectile(p);   // add in  Rpg_Engine class prijectile list this projectile
		return false;
	}
	else
	{




		cDynamic_Projectile* p = new cDynamic_Projectile(x, y, aggressor->m_layer, vx, vy, 0.0f, RPG_Assets::get().GetSprite("Pantir's DaggerLeftFx"), RPG_Assets::get().GetSprite("Pantir's DaggerRightFx"), aggressor->FxColumn, aggressor->m_nShockTime, aggressor->rageset);
		p->bSolidVsMap = false;
		p->nDamage = aggressor->GetAttack();
		p->bOneHit = true;
		p->attackDirectionX = attackdir;
		p->attackDirectionY = aggressor->attackdirectionY;

		p->gettarget = aggressor->Btarget;
		


		p->CollbordersX = aggressor->ProjCollbordersX;
		p->CollbordersXF = aggressor->ProjCollbordersXF;
		p->CollbordersY = aggressor->ProjCollbordersY;
		p->CollbordersYF = aggressor->ProjCollbordersYF;

		//p->attackDirectionX =attackdir;
		p->m_FacingDirection = aggressor->GetFacingDirection();
		p->m_VerticalFacingDirection = aggressor->GetFacingDirectionVertical();
		p->HitAmount = aggressor->Hittimes;  //   < -- we add how many times our projectile should take damage before leave  default 1




		g_engine->AddProjectile(p);   // add in  Rpg_Engine class prijectile list this projectile
		return false;
	}

	return false;

}

cWeapon_BrokenSword::cWeapon_BrokenSword() : cWeapon("Broken Sword", RPG_Assets::get().GetSprite("BrokenSword"), "it seems the sword was \nbroken quite a long time  \nago\n", 0)
{
	//bKeyItem = true;
	sTradePrice = 2;
}

bool cWeapon_BrokenSword::OnUse(cDynamic* object)
{
	return false;
}

cWeapon_BanditSword::cWeapon_BanditSword() : cWeapon("Bandit Sword", RPG_Assets::get().GetSprite("BrokenSword"), "usually these kind of swords  \nare used by bandits", 1)
{
//	bKeyItem = true;
	sTradePrice = 5;
}

bool cWeapon_BanditSword::OnUse(cDynamic* object)
{
	// When weapons are used, they are used on the object that owns the weapon, i.e.
			// the attacker. However this does not imply the attacker attacks themselves

			// Get direction of attacker
	cDynamic_creature_Bandit* aggressor = (cDynamic_creature_Bandit*)object;


	// Determine attack origin
	float x, y, vx, vy, collx, collxf, attackdir;


	if (aggressor->GetFacingDirection() == 1) // East
	{
		x = aggressor->px - 0.95f;
		y = aggressor->py;
		vx = aggressor->vx; vy = aggressor->vy;
		collx = 0.0f;
		collxf = 1.75f;
		attackdir = -aggressor->attackdirectionX;
	}



	if (aggressor->GetFacingDirection() == 3) // West
	{
		x = aggressor->px + 0.85f;
		y = aggressor->py;
		vx = aggressor->vx; vy = aggressor->vy;
		collx = 0.25f;
		collxf = 2.0f;
		attackdir = aggressor->attackdirectionX;
	}
	if (aggressor->GetFacingDirectionVertical() == 1)  // Up
	{
		x = aggressor->px;
		y = aggressor->py;
		vx = 0; vy = aggressor->vy;



	}
	if (aggressor->GetFacingDirectionVertical() == 2)  // Down
	{
		x = aggressor->px;
		y = aggressor->py;
		vx = 0; vy = aggressor->vy;


	}





	if (aggressor->isprojfollow)  // fallendownEviscirate
	{

		cDynamic_Projectile* p = new cDynamic_Projectile(RPG_Assets::get().GetSprite("Bandit SwordLeftFx"), RPG_Assets::get().GetSprite("Bandit SwordLeftFx"), aggressor);
		p->px = x;
		p->py = y;
		p->bSolidVsMap = true;
		p->nDamage = aggressor->GetAttack();
		p->bOneHit = true;

		p->m_GraphicState = p->setEnum(aggressor->FxColumn);
		//collision
		p->CollbordersX = aggressor->ProjCollbordersX;
		p->CollbordersXF = aggressor->ProjCollbordersXF;
		p->CollbordersY = aggressor->ProjCollbordersY;
		p->CollbordersYF = aggressor->ProjCollbordersYF;
		//which way aim fly
		p->attackDirectionX = attackdir;
		p->attackDirectionY = aggressor->attackdirectionY;
		p->m_FacingDirection = aggressor->GetFacingDirection();
		p->m_VerticalFacingDirection = aggressor->GetFacingDirectionVertical();
		p->HitAmount = aggressor->Hittimes;  //   < -- we add how many times our projectile should take damage before leave  default 1

		
	

		g_engine->AddProjectile(p);   // add in  Rpg_Engine class prijectile list this projectile
		return false;
	}







	if (aggressor->IsThrow == true)
	{

		cDynamic_Projectile* p = new cDynamic_Projectile(x, y, aggressor->m_layer, aggressor->sparedVx * 2, vy,  aggressor->m_nGraphicAmountFrames * 0.1f, RPG_Assets::get().GetSprite("Bandit SwordLeftFx"), RPG_Assets::get().GetSprite("Bandit SwordRightFx"),  aggressor->FxColumn, aggressor->m_nShockTime, aggressor->rageset);
		p->bSolidVsMap = true;
		p->nDamage = aggressor->GetAttack();
		p->m_FacingDirection = aggressor->GetFacingDirection();
		p->m_VerticalFacingDirection = aggressor->GetFacingDirectionVertical();
		p->bOneHit = true;

		p->CollbordersX = aggressor->ProjCollbordersX;
		p->CollbordersXF = aggressor->ProjCollbordersXF;
		p->CollbordersY = aggressor->ProjCollbordersY;
		p->CollbordersYF = aggressor->ProjCollbordersYF;

		g_engine->AddProjectile(p);   // add in  Rpg_Engine class prijectile list this projectile
		return false;
	}
	else
	{
		cDynamic_Projectile* p = new cDynamic_Projectile(x, y, aggressor->m_layer, vx, vy,  0.0f, RPG_Assets::get().GetSprite("Bandit SwordLeftFx"), RPG_Assets::get().GetSprite("Bandit SwordRightFx"), aggressor->FxColumn, aggressor->m_nShockTime, aggressor->rageset);
		p->bSolidVsMap = false;
		p->nDamage = aggressor->GetAttack();
		p->bOneHit = true;
	
		p->attackDirectionY = aggressor->attackdirectionY;
		p->CollbordersX = collx;
		p->CollbordersXF = collxf;
		p->attackDirectionX = attackdir;
		p->m_FacingDirection = aggressor->GetFacingDirection();
		p->m_VerticalFacingDirection = aggressor->GetFacingDirectionVertical();

		p->CollbordersX = aggressor->ProjCollbordersX;
		p->CollbordersXF = aggressor->ProjCollbordersXF;
		p->CollbordersY = aggressor->ProjCollbordersY;
		p->CollbordersYF = aggressor->ProjCollbordersYF;


	


		g_engine->AddProjectile(p);   // add in  Rpg_Engine class prijectile list this projectile
		return false;
	}

	return false;
}

cCoin::cCoin() : cGold("Coin", RPG_Assets::get().GetSprite("Coin"), "currency", 1)
{
	
}

cSmallWallet::cSmallWallet() : cGold("SmallWallet", RPG_Assets::get().GetSprite("PocketLowEmpty"), "Keep 6 golds:  " + std::to_string(GoldCount) + "/6",0)
{
	bKeyItem = false;
	sTradePrice = 2;
	sTradeBuyPrice = 5;

	sCraftRequaries = std::to_string(1)+FirstItem + "\n"  + std::to_string(1)+SecondItem + "\n";
}

bool cSmallWallet::OnUse(cDynamic* object)
{
	
	int counter = 0;

	for (auto& item : g_engine->GetListItem())  // go in warehouse list check cross with mouse 
	{
		

		

		if (item->sName =="Coin"&& GoldCount < 6)
		{

				
			counter++;
			item->Absorbed = true;  // absorb coin
			this->GoldCount++;
			sDescription = "Keep 6 golds:  " + std::to_string(GoldCount)+ "/6" + "\n" + "Shalalala dont be scared"+'\n'+"you got to mood prepare";
			pSprite = RPG_Assets::get().GetSprite("PocketLowFull");
		
			
			


		}
		else
		{
			item->ipx = ((item->invnumber-1 ) % 8 + item->ipx) - ((item->invnumber - counter-1) % 8);
			item->ipy = ((item->invnumber-1 ) / 8 + item->ipy) - ((item->invnumber - counter-1) / 8);
		}

	}



	return false;
}

bool cSmallWallet::OnBuy()
{
	if (g_engine->CheckMoney() >= sTradeBuyPrice)
	{
	g_engine->GiveItem(new cSmallWallet);
	return true;
	}
	return false;
}
bool cSmallWallet::OnCraft(bool click)
{
	


	if (g_engine->HasItem(RPG_Assets::get().GetItem(FirstItem)) && g_engine->HasItem(RPG_Assets::get().GetItem(SecondItem)))
	{
		if (click==1)
		g_engine->GiveItem(new cSmallWallet);
	return true;
	}
	
	return false;
}

cWeapon_BanditBossSword::cWeapon_BanditBossSword() : cWeapon("Bandit Boss Sword", RPG_Assets::get().GetSprite("BrokenSword"), "usually these kind of swords  \nare used by bandits", 10)
{
}

bool cWeapon_BanditBossSword::OnUse(cDynamic* object)
{
	// When weapons are used, they are used on the object that owns the weapon, i.e.
		// the attacker. However this does not imply the attacker attacks themselves

		// Get direction of attacker
	cDynamic_creature_Bandit* aggressor = (cDynamic_creature_Bandit*)object;


	// Determine attack origin
	float x, y, vx, vy, collx, collxf, attackdir;


	if (aggressor->GetFacingDirection() == 1) // East
	{
		x = aggressor->px - 0.5f;
		y = aggressor->py+0.5f;
		vx = aggressor->vx; vy = aggressor->vy;
		collx = 0.0f;
		collxf = 1.75f;
		attackdir = -aggressor->attackdirectionX;
	}



	if (aggressor->GetFacingDirection() == 3) // West
	{
		x = aggressor->px + 2.1f;
		y = aggressor->py + 0.5f;
		vx = aggressor->vx; vy = aggressor->vy;
		collx = 0.25f;
		collxf = 2.0f;
		attackdir = aggressor->attackdirectionX;
	}
	if (aggressor->GetFacingDirectionVertical() == 1)  // Up
	{
		x = aggressor->px;
		y = aggressor->py;
		vx = 0; vy = aggressor->vy;



	}
	if (aggressor->GetFacingDirectionVertical() == 2)  // Down
	{
		x = aggressor->px;
		y = aggressor->py;
		vx = aggressor->vx; vy = aggressor->vy;


	}





	if (aggressor->isprojfollow)  // fallendownEviscirate
	{

		cDynamic_Projectile* p = new cDynamic_Projectile(RPG_Assets::get().GetSprite("Bandit SwordLeftFx"), RPG_Assets::get().GetSprite("Bandit SwordLeftFx"), aggressor);
		p->px = x;
		p->py = y;
		p->bSolidVsMap = true;
		p->nDamage = aggressor->GetAttack();
		p->bOneHit = true;
		//collision
		p->CollbordersX = aggressor->ProjCollbordersX;
		p->CollbordersXF = aggressor->ProjCollbordersXF;
		p->CollbordersY = aggressor->ProjCollbordersY;
		p->CollbordersYF = aggressor->ProjCollbordersYF;
		//which way aim fly
		p->attackDirectionX = attackdir;
		p->attackDirectionY = aggressor->attackdirectionY;
		p->m_FacingDirection = aggressor->GetFacingDirection();
		p->m_VerticalFacingDirection = aggressor->GetFacingDirectionVertical();
		p->nSheetSize = 0;
		p->HitAmount = aggressor->Hittimes;  //   < -- we add how many times our projectile should take damage before leave  default 1



		g_engine->AddProjectile(p);   // add in  Rpg_Engine class prijectile list this projectile
		return false;
	}







	if (aggressor->IsThrow == true)
	{

		cDynamic_Projectile* p = new cDynamic_Projectile(x, y, aggressor->m_layer, aggressor->sparedVx * 2, vy, aggressor->m_nGraphicAmountFrames * 0.1f, RPG_Assets::get().GetSprite("Bandit SwordLeftFx"), RPG_Assets::get().GetSprite("Bandit SwordRightFx"), aggressor->FxColumn, aggressor->m_nShockTime, aggressor->rageset);
		p->bSolidVsMap = true;
		p->nDamage = aggressor->GetAttack();
		p->m_FacingDirection = aggressor->GetFacingDirection();
		p->m_VerticalFacingDirection = aggressor->GetFacingDirectionVertical();
		p->bOneHit = true;

		p->CollbordersX = aggressor->ProjCollbordersX;
		p->CollbordersXF = aggressor->ProjCollbordersXF;
		p->CollbordersY = aggressor->ProjCollbordersY;
		p->CollbordersYF = aggressor->ProjCollbordersYF;
		p->nSheetSize = 0;
		g_engine->AddProjectile(p);   // add in  Rpg_Engine class prijectile list this projectile
		return false;
	}
	else
	{
		cDynamic_Projectile* p = new cDynamic_Projectile(x, y, aggressor->m_layer, vx, vy, 0.0f, RPG_Assets::get().GetSprite("Bandit SwordLeftFx"), RPG_Assets::get().GetSprite("Bandit SwordRightFx"), aggressor->FxColumn, aggressor->m_nShockTime, aggressor->rageset);
		p->bSolidVsMap = false;
		p->nDamage = aggressor->GetAttack();
		p->bOneHit = true;

		p->attackDirectionY = aggressor->attackdirectionY;
		p->CollbordersX = collx;
		p->CollbordersXF = collxf;
		p->attackDirectionX = attackdir;
		p->m_FacingDirection = aggressor->GetFacingDirection();
		p->m_VerticalFacingDirection = aggressor->GetFacingDirectionVertical();

		p->CollbordersX = aggressor->ProjCollbordersX;
		p->CollbordersXF = aggressor->ProjCollbordersXF;
		p->CollbordersY = aggressor->ProjCollbordersY;
		p->CollbordersYF = aggressor->ProjCollbordersYF;
		p->nSheetSize = 0;

		


		g_engine->AddProjectile(p);   // add in  Rpg_Engine class prijectile list this projectile
		return false;
	}

	return false;
}
