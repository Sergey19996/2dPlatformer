#include "RPG_Dynamic.h"
#include "RPG_Engine.h"


RPG_Engine* cDynamic::g_engine = nullptr;

cDynamic::cDynamic(std::string n)
{

	sName = n;
	px = 0.0f;
	py = 0.0f;
	vx = 0.0f;
	vy = 0.0f;
	bSolidVsMap = true;
	

	
	m_layer = Neutral;
	bOnGround = false;
	Jumpcounter = 0;


	CollbordersX = 0.8f;
	CollbordersXF = 1.1f;

	CollbordersY = 0.6f;
	CollbordersYF = 1.5f;
	
}

Environment::Environment(std::string sname)
{
	this->sname = sname;
}

Environment::~Environment()
{

}
cDynamic::~cDynamic()
{
}

void cDynamic::SwitchLayer(int num)
{
	switch (num)
	{
	case 0:
		m_layer = Friend;
	case 1:
		m_layer = Enemy;
		break;
	case 2:
		m_layer = Neutral;
		break;
	case 3:
		m_layer = Particle;
		break;
	case 4:
		m_layer = Questable;
		break;
	}
	
	

}
///////////////////////////////////////////////////////////////



cDynamic_Creature::cDynamic_Creature(std::string name, olc::Decal* spriteRight, olc::Decal* spriteLeft) :cDynamic(name)     //here we take two sprites
{

	m_pSpriteRight = spriteRight;
	m_pSpriteLeft = spriteLeft;
	nHealth = 10;
	nHealthMax = 10;
	//M_nFacingDirection = SOUTH;
	
	m_fTimer = 0.0f;
	bIsAttackable = true;
	bOnGround =false;
	isAttack =false;
	IsOnePlay = false;

	gravity = true;
	 m_nGraphicCounterX = 0;
	 m_nGraphicCounterY = 0;
	 FXFrame = 0;
	 frameIndicator = 0;
	 framespeed = 0.12f;
	 m_nGraphicAmountFrames = 0;
	 m_nGraphicEndFrame = 0;
	 animspr = 0.0f;
	 enumCounter = 0;
}

cDynamic_ClipTrigger::cDynamic_ClipTrigger(std::string name) : cDynamic(name)
{
	
	gravity = true;
}


void cDynamic_Creature::Update(float fElapsedTime, cDynamic* player)
{
	

	
	

	if (m_fKnockBackTimer > 0.0f)
	{
		vx = m_fKnockBackDX * 10.0f;
		vy = m_fKnockBackDY * 10.0f;

		m_fKnockBackTimer -= fElapsedTime;

		if (m_fKnockBackTimer <= 0.0f )
		{
			//vx = 0;
			bControllable = true; // when we control in scripteditor
			bIsAttackable = true;
			bAnimAction = false;
			
		//	player->bSolidvsDyn = true;     //return  unavalibale to go through ppl
			m_fKnockBackTimer = 0;
			ReturnBaseLayer();
			//mass = 48;

			greenColor = 255;   //<--return colors
			blueColor = 255;
			redColor = 255;

		}
	}

	else
	{
		if (isAttack == false && isDirectionLock==false)
		{
		if (vx < 0) M_nFacingDirection = WEST;
		if (vx > 0) M_nFacingDirection = EAST;
		}

		if (nHealth <= 0)                                //Death char Tracker
		{
			std::cout << "Hello" << std::endl;
			bDraw = false;
			if (!quested)
			{
			this->DeathFun(); // droping fx object
			}
			bDead = true;
			g_engine->SetCurrentExp(calculateDeathExp()); // Get exp from dead enemy
			g_engine->AddVecDynamic(new cDynamic_Item(this->px, this->py,new cCoin()));

			if (!this->quested)
			g_engine->ReturnToPool(this);  // now for wolfs bandits

				//vecDyns.push_back(new cDynamic_Item(10, 10, RPG_Assets::get().GetItem("Small Health")));
		}
		//if (vy < 0) M_nFacingDirection = NORTH;
		//if (vy > 0) M_nFacingDirection = SOUTH;


		if (vx > 25.0f)
		{
			vx = 25.0f;

		}
		if (vx < -25.0f)
		{

			vx = -25.0f;
		}
		if (vy > 50.0f)
		{
			vy = 50.0f;
		}
		if (vy < -50.0f)
		{
			vy = -50.0f;
		}



		animspr += fElapsedTime;

		setEnum();

		Behaviour(fElapsedTime, player);

		if (gravityofftimer>0)
		{
		GravityControl(fElapsedTime);
		}

	}
	



}

void cDynamic_Creature::Behaviour(float fElapsedTime, cDynamic* player)
{
}

void cDynamic_Creature::calculateStats()
{
	switch (M_BaseClass)
	{
	case cDynamic_Creature::STRENGTH:

		break;
	case cDynamic_Creature::AGILITY:

		break;
	case cDynamic_Creature::INTELLIGENCE:

		break;
	}

	// Initialize variables
	float sup_intelligence = BasicIntelect - 1.5f;
	float sup_agility = BasicAgility - 2.5f;
	float averHelp = (19.0f * 1.35f) - 1.35f;
	float sup_strength = BasicStrength - 1.7f;
	float defsup = 2.25f - 0.18f;
	float MoveSup = BasicMovementSpeed;
	float HasteSup = 0;
	for (int i = 1; i <= LvL; i++)
	{
		// Calculate Intelligence
		sup_intelligence += 1.5f;
		if (i == LvL)
		{
			intelligence = sup_intelligence;
			std::cout << "IntL: " << intelligence << std::endl;
		}

		// Calculate Agility
		sup_agility += 2.5f;
		HasteSup = ((sup_agility/100)-0.165f)/10;

		averHelp += 1.35f + 2.5f;

		MoveSup += 0.5f;
		if (i == LvL)
		{
			Haste = HasteSup;
			std::cout << "Haste: " << Haste << std::endl;
			Movementspeed = MoveSup;
			std::cout << "Movementspd: " << Movementspeed << std::endl;
			agility = sup_agility;
			std::cout << "agility: " << agility << std::endl;
			AverageAttack = averHelp;
			std::cout << "AverAttack: " << AverageAttack << std::endl;
		}

		// Calculate Strength
		sup_strength += 1.7f;
		defsup += 0.18f;
		if (i == LvL)
		{
			strength = sup_strength;
			std::cout << "strength: " << strength << std::endl;
			nHealthMax = strength * 25;
			nHealth = nHealthMax;
			hpUpdate->setLevelHp(nHealthMax);
			Defence = defsup;
		}
	}


}

void cDynamic_Creature::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{
	
	const int spriteWidth = m_pSpriteRight->sprite->width / nSheetSizeX;

	                //0.12f   - 0.05f (20 lvl)
	if (animspr >= framespeed-Haste)                 //it's speed of frames
	{

			frameIndicator++;

		if (IsOnePlay || isAttack ||bOnLanded )                 // if we put left mouse key
		{
			//x stroke is getting growth


			if (frameIndicator >= m_nGraphicEndFrame)
			{
				IsOnePlay = false;
				isAttack = false;
				bOnLanded = false;
				IsThrow = false;
				//isprojfollow = false;
				sparedVx =0.0f;
			//	setEnum();
			}
			if (frameIndicator == FXFrame && isAttack)   //   <---- summon projectile when attack
			{
				PerformAttack();
			}
		}
		
		animspr =0;

	}
	else if (frameIndicator >= m_nGraphicEndFrame || frameIndicator <= m_nGraphicCounterX)   // <==
	{

		frameIndicator = m_nGraphicCounterX;
		setEnum();
	}


	                   //m_nGraphicCounterY offset for column     @frameIndicator /(int(m_pSpriteRight->sprite->width / nSheetSizeX@ lock in sprite width
	nSheetOffsetY = (m_nGraphicCounterY + (frameIndicator / spriteWidth))*nSheetSizeX;
	nSheetOffsetX = (frameIndicator % spriteWidth)*nSheetSizeY;


	//std::cout << nSheetOffsetY/ nSheetSizeY <<'\t'<< nSheetOffsetX / nSheetSizeY << std::endl;

	/// //////

	if (M_nFacingDirection == EAST)
	{

		gfx->DrawPartialDecal({ (px - ox) * 64.0f,(py - oy) * 64.0f }, m_pSpriteRight, { nSheetOffsetX,nSheetOffsetY }, { nSheetSizeX,nSheetSizeY }, {1,1},olc::Pixel(redColor,greenColor, blueColor));   //offset pulling player back into the screen
	}
	else
	{
		gfx->DrawPartialDecal({ (px - ox) * 64.0f,(py - oy) * 64.0f }, m_pSpriteLeft, { nSheetOffsetX,nSheetOffsetY }, { nSheetSizeX,nSheetSizeY }, { 1,1 }, olc::Pixel(redColor, greenColor, blueColor));   //offset pulling player back into the screen

	}


	if (hpUpdate !=nullptr )
	{

	hpUpdate->DrawSelf(gfx, ox, oy);

	}
	
	
	if (bOnTarget ==true)
	{
	//	gfx->DrawPartialDecal({ (px - ox) * 64.0f+38,(py - oy) * 64.0f+18 }, m_pSpriteRight, { nSheetOffsetX,nSheetOffsetY }, { nSheetSizeX,nSheetSizeY }, { 1,1 }, olc::Pixel(redColor, greenColor, blueColor));   //offset pulling player back into the screen
		
		g_engine->DrawBigText(std::to_string(LvL) + " " + sName, (px - ox + 0.36f) * 64.0f + 18, (py - oy) * 64.0f , 0.5, 0.5, olc::YELLOW);


		//gfx->DrawPartialDecal({ (px - ox) * 64.0f + 28,(py - oy) * 64.0f + 18 }, HpEmpty, { 0,0 }, { (float)HpEmpty->sprite->width,(float)HpEmpty->sprite->height });   //offset pulling player back into the screen
	}

}

int cDynamic_Creature::calculateDeathExp()
{
	int sup = 14;
	for (int i = 1; i <= LvL+1; i++)
	{
		sup = sup + 5 * i + 6; //200 for 2 lvl

		if (i == LvL)
		{
			return sup;
			break;
		}

	}

	return  0;

}

void cDynamic_Creature::GravityControl(float felapsedtime)
{
	gravity = false;
	gravityofftimer -= felapsedtime;

		if (gravityofftimer <=0 || m_fKnockBackTimer>0.0f)
		{
			gravity = true;
		}

	
}

void cDynamic_Creature::SetUpDamage ()   //standart damage for all chars
{

	switch (M_nGraphicState)
	{
	case cDynamic_Creature::Idle:
		break;
	case cDynamic_Creature::Walk:
		break;
	case cDynamic_Creature::Jump:
	
		break;
	case cDynamic_Creature::Attack:
		calculatedDamage = (AverageAttack + pEquipedWeapon->nDamage) * 2.65f;
		break;
	case cDynamic_Creature::AttackEasy:
		calculatedDamage = (AverageAttack - 20 + pEquipedWeapon->nDamage) * 5;
		break;
	case cDynamic_Creature::AttackGreat:
		calculatedDamage = (AverageAttack + 20 + pEquipedWeapon->nDamage) * 3;
		break;
	case cDynamic_Creature::DoubleJump:
		break;
	case cDynamic_Creature::AirAttack:
		calculatedDamage = (AverageAttack + pEquipedWeapon->nDamage) * 2.65f;
		break;
	case cDynamic_Creature::Eviscirate:
		calculatedDamage = (AverageAttack + 20 + pEquipedWeapon->nDamage) * 3.6f;
		break;
	case cDynamic_Creature::JumpDown:
		break;
	case cDynamic_Creature::JumpZenit:
		break;
	case cDynamic_Creature::Landing:
		break;
	case cDynamic_Creature::AirEviscirate:
		calculatedDamage = (AverageAttack + 20 + pEquipedWeapon->nDamage) * 3.6f;
		break;
	case cDynamic_Creature::Hit:
		
		break;
	case cDynamic_Creature::AttackBack:
		calculatedDamage = (AverageAttack + pEquipedWeapon->nDamage) * 4;
		break;
	case cDynamic_Creature::AirDownEviscirate:
		calculatedDamage = (AverageAttack + 20+ pEquipedWeapon->nDamage) * 3.6f;
		break;
	case cDynamic_Creature::LANDINGEviscirate:
		calculatedDamage = (AverageAttack  + pEquipedWeapon->nDamage) * 3.6f;
		break;


	default:
		break;
	}

}

bool cDynamic_Creature::IsLanded()
{

	if (bOnLanded)
	{
		return true;
	}
	else
	{
		bOnLanded = true;
		if (bAnimAction)   // <--Means when we action stage don't switch animation on onladed
		{
			return false;
		}
		enumCounter = 11;
		isAttack = false;
		IsOnePlay = false;
		frameIndicator = 0;
		setEnum();
		return false;
	}
	
}

bool cDynamic_Creature::setEnum()
{
	
	if (IsOnePlay || isAttack || bOnLanded )
	{
		switch (enumCounter)
		{
		case 11:
			M_nGraphicState = Landing;
			IndicateAnim();
			return true;
			break;
		case 3:
			vx = 0;
			M_nGraphicState = Attack;
			IndicateAnim();
			return true;
			break;
		case 8:
			vx = 0;
			M_nGraphicState = Eviscirate;
			IndicateAnim();
			return true;
			break;
		case 7:
			M_nGraphicState = AirAttack;
			IndicateAnim();
			return true;
			break;
		case 12:
			M_nGraphicState = AirEviscirate;
			IndicateAnim();
			return true;
			break;
		case 6:
			M_nGraphicState = DoubleJump;
			IndicateAnim();
			return true;

			break;
		case 13:
			M_nGraphicState = Hit;
			IndicateAnim();
			return true;
			break;
		case 5:
			M_nGraphicState = AttackGreat;
			IndicateAnim();
			return true;
			break;

		case 4:
			M_nGraphicState = AttackEasy;
			IndicateAnim();
			return true;
			break;

		case 1:
			M_nGraphicState = Walk;
			IndicateAnim();
			return true;
			break;

		

		case 16:
			M_nGraphicState = AttackBack;
			IndicateAnim();
			return true;
			break;


		case 17:
			M_nGraphicState = UpEviscirate;
			IndicateAnim();
			return true;
			break;


		case 18:
			M_nGraphicState = AirDownEviscirate;
			IndicateAnim();
			return true;
			break;

		case 19:
			M_nGraphicState = LANDINGEviscirate;
			IndicateAnim();
			return true;
			break;

		case 20:
			M_nGraphicState = BLINKDESAPEAR;
			IndicateAnim();
			return true;
			break;

		case 21:
			M_nGraphicState = GRAB;
			IndicateAnim();
			return true;
			break;

		case 22:
			M_nGraphicState =Swirl;
			IndicateAnim();
			return true;
			break;

		case 24:
			M_nGraphicState = IdleTwo;
			IndicateAnim();
			return true;
			break;

		case 25:
			M_nGraphicState = IdleReaction;
			IndicateAnim();
			return true;
			break;

		}


	}

	

	if (bOnGround && !bOnLanded)
	{

		if (vx > 0.5f ||vx<-0.5f )
		{

			M_nGraphicState = Walk;
			IndicateAnim();
			return true;
		}
		
	}

	/// ///////////////////

	if (!bOnGround && Jumpcounter <= 1 &&!bOnLanded)
	{

		
		if (vy > 0.1f  )
		{
			M_nGraphicState = JumpDown;
			bOnLanded = false;
			IndicateAnim();
			return true;
		}
		if (vy<2.8f && vy>-2.8f)
		{
			M_nGraphicState = JumpZenit;
			bOnLanded = false;
			IndicateAnim();
			return true;
		}

		if (vy < -0.1f)
		{
			M_nGraphicState = Jump;
			bOnLanded = false;
			IndicateAnim();
			return true;
		}
	/// ///////////////////
	}
	if (vx <= 0.5f && vx >= -0.5f && bOnGround && !bAnimAction )

	{
		M_nGraphicState = Idle;
		
		IndicateAnim();
		return true;
	}


	///////////////////////
	
	IndicateAnim();
	return false;
	//IndicateAnim();

}

void cDynamic_Creature::KnockBack(float dx, float dy, float dist)
{

	//mass = mass;
	if (bKnockBack)
	{

	vx = 0;
	m_fKnockBackDX = dx;
	m_fKnockBackDY = dy;
	m_fKnockBackTimer = dist;
	
	if (this->sName !="Pantir")
	{
		isDirectionLock = true;
	}


	bControllable = false; // when we control in scripteditor char
	bIsAttackable = false;
	isprojfollow = false;    // keep projectile follow at char 
	
	isAttack = false;
	 Btarget = false;
	blueColor = 0;
	greenColor = 0;
	IsOnePlay = true;
	enumCounter = 13;
	//frameIndicator = 13;
	setEnum();
	}




}



cDynamic_creature_Bandit::cDynamic_creature_Bandit() : cDynamic_creature_Enemy("Bandit", RPG_Assets::get().GetSprite("BanditRight"), RPG_Assets::get().GetSprite("BanditLeft"))
{
	                                       //Bandit constructor
	



	//collision borders
	CollbordersX = 0.8f;
	CollbordersXF = 1.1f;


	CollbordersY = 0.6f;
	CollbordersYF = 1.5f;
	//


	pEquipedWeapon = (cWeapon*)RPG_Assets::get().GetItem("Bandit Sword");
	hpUpdate = new cDynamic_HpBar(px, py, RPG_Assets::get().GetSprite("HPBarFull"), RPG_Assets::get().GetSprite("HPBarEmprty"), nHealth, this);
	
	LvL = 3;

	

	BasicMovementSpeed = 16;
	BasicAgility = 19;
	BasicStrength = 15;
	BasicIntelect = 13;

	calculateStats();
}
cDynamic_creature_DireWolf::cDynamic_creature_DireWolf() : cDynamic_creature_Enemy("DireWolf", RPG_Assets::get().GetSprite("DireWolfRight"), RPG_Assets::get().GetSprite("DireWolfLeft"))
{

	pEquipedWeapon = (cWeapon*)RPG_Assets::get().GetItem("Bandit Sword");
	hpUpdate = new cDynamic_HpBar(px, py, RPG_Assets::get().GetSprite("HPBarFull"), RPG_Assets::get().GetSprite("HPBarEmprty"), nHealth, this);

	LvL = 1;

	CollbordersX = 0.8f;
	CollbordersXF = 1.1f;

	CollbordersY = 0.6f;
	CollbordersYF = 1.5f;

	BasicMovementSpeed = 20;
	BasicAgility = 19;
	BasicStrength = 9;
	BasicIntelect = 13;

	calculateStats();
}



cDynamic_creature_NPCBandit::cDynamic_creature_NPCBandit(std::string sname) : cDynamic_Creature("Npc", RPG_Assets::get().GetSprite("BanditRight"), RPG_Assets::get().GetSprite("BanditLeft"))
{
	//Bandit constructor
	m_layer = Neutral;

	sName = sname;

	nHealth = 1;
	nHealthMax = 1;
	bDead = false;
	m_fStateTick = 0.2f;
	bOnGround = false;

//	bSolidVsMap = true;

		//collision borders
	CollbordersX = 0.8f;
	CollbordersXF = 1.1f;

	CollbordersY = 0.6f;
	CollbordersYF = 1.5f;
	//

}






















bool cDynamic_creature_Enemy::IsLanded()
{

	if (bOnLanded)
	{
		return true;
	}
	else
	{
		bOnLanded = true;
		if (bAnimAction)   // <--Means when we action stage don't switch animation on onladed
		{
			return false;
		}

		vx = 0;
		enumCounter = 11;
		isAttack = false;
		IsOnePlay = true;
		frameIndicator = 0;
		setEnum();
		return false;
	}

}



void cDynamic_creature_NPCBandit::IndicateAnim()
{
	switch (M_nGraphicState)
	{
	case Idle:
		enumCounter = 0;
		m_nGraphicCounterY = 0;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 8;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	}
}


///////////
void cDynamic_creature_DireWolf::IndicateAnim()
{
	
	
	switch (M_nGraphicState)
	{
	case Idle:
		enumCounter = 0;
		m_nGraphicCounterY = 0;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 7;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case Landing:
		enumCounter = 11;
		m_nGraphicCounterY = 4;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 4;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case Walk:
		enumCounter = 1;
		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 4;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

		break;
	case Jump:
		enumCounter = 2;
		m_nGraphicCounterY = 3;
		m_nGraphicCounterX = 4;
		m_nGraphicAmountFrames = 3;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case JumpZenit:
		enumCounter = 10;
		m_nGraphicCounterY = 0;
		m_nGraphicCounterX = 8;
		m_nGraphicAmountFrames = 2;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case JumpDown:
		enumCounter = 9;
		m_nGraphicCounterY = 3;
		m_nGraphicCounterX = 7;
		m_nGraphicAmountFrames = 3;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case Attack:        // <-- (Horizontal Attack)
		enumCounter = 3;
	

		break;
	case AttackEasy:// <-- (Throw attack)
		enumCounter = 4;
		
	
		
		if (frameIndicator == m_nGraphicEndFrame -1)
		{
			vy =- 10;

		}

		break;
	
	
	case Hit:
		enumCounter = 13;
		m_nGraphicCounterY = 3;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 4;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		
		break;


	}
}



void cDynamic_creature_DireWolf::SpecAttack(float targetX, float targetY, float Distance)
{

	isprojfollow = true;
	isAttack = true;
	enumCounter = 4;     //  <---Attack Throw Dagger
	vx = (targetX / Distance) * 5.0f;
	isDirectionLock = true;
	sparedVx = -vx;
	//vy = -10.0f;
	frameIndicator = 0;
	//setEnum();

	m_nGraphicCounterY = 4;
	m_nGraphicCounterX = 3;
	m_nGraphicAmountFrames = 6;
	m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

	//ProjSize
	ProjCollbordersX = 0.25f;
	ProjCollbordersXF = 1.75f;
	ProjCollbordersY = 0.65f;
	ProjCollbordersYF = 1.8f;
	//

	attackdirectionX = 0.3f;
	attackdirectionY = -0.5f;
	m_nShockTime = 0.1f;
	FxColumn = 3;    //<--  3 column      place eviscirate
	FXFrame = m_nGraphicEndFrame - 1;

	M_nFacingDirectionVertical = NORTH;
}



void cDynamic_creature_DireWolf::AttackOne()
{
	isAttack = true;
	enumCounter = 3;     //  <---Attack

	frameIndicator = 0;

	
	ProjCollbordersX = 0.25f;
	ProjCollbordersXF = 1.75f;
	ProjCollbordersY = 0.65f;
	ProjCollbordersYF = 1.8f;

	isprojfollow = false;
	vx = 0;
	m_nGraphicCounterY = 2;
	m_nGraphicCounterX = 0;
	m_nGraphicAmountFrames = 10;
	attackdirectionX = 0.3f;
	attackdirectionY = -0.5f;
	m_nShockTime = 0.1f;
	FxColumn = 1;
	FXFrame = 3;

	M_nFacingDirectionVertical = NOTLOOKING;
	m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

}

void cDynamic_creature_DireWolf::AttackTwo()
{
	AttackOne();

}

void cDynamic_creature_DireWolf::DeathFun()
{

	olc::vi2d fallFrame{ 1,5 };
	olc::vi2d landingframe{ 1,6 };
	
	std::pair<olc::vi2d, olc::vi2d> spritePos{ fallFrame, landingframe };

	if (vx<=0)
	{
		g_engine->AddParticle(new VfxDeath(this, m_pSpriteRight, spritePos, 0));
	}
	else
	{
		g_engine->AddParticle(new VfxDeath(this, m_pSpriteLeft, spritePos, 1));
	}


}

//////


void cDynamic_creature_Bandit::IndicateAnim()
{
	
	switch (M_nGraphicState)
	{
	case Idle:
		enumCounter = 0;
		m_nGraphicCounterY = 0;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 8;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case Landing:
		enumCounter = 11;
		m_nGraphicCounterY = 2;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 6;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

		

		break;
	case Walk:
		enumCounter = 1;
		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 9;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

		break;
	case Jump:
		enumCounter = 2;
		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 10;
		m_nGraphicAmountFrames = 2;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case JumpZenit:
		enumCounter = 10;
		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 10;
		m_nGraphicAmountFrames = 2;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case JumpDown:
		enumCounter = 9;
		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 10;
		m_nGraphicAmountFrames = 2;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case Attack:        // <-- (Horizontal Attack)
		enumCounter = 3;
		

		break;
	case AttackEasy:// <-- (Throw attack)
		enumCounter = 4;
		
		break;
	case AttackGreat:  




		break;
	case Eviscirate:
		
		break;
	case AirEviscirate:  //(Vertical Attack - becouse fx suit
		enumCounter = 12;
		
		break;

	case Hit:
		enumCounter = 13;
		m_nGraphicCounterY =0;
		m_nGraphicCounterX = 8;
		m_nGraphicAmountFrames = 4;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

		
		break;
		

	}

}

void cDynamic_creature_Bandit::SpecAttack(float targetX, float targetY, float Distance)
{
	
	isAttack = true;
	IsThrow = true;

	enumCounter = 4;     //  <---Attack Throw Dagger
	vx = (-targetX / Distance) * 5.0f;
	isDirectionLock = true;

	sparedVx = (vx < 0) ? 5 : -5;

	vy = -10.0f;
	frameIndicator = 0;
	
	
	

	ProjCollbordersX = 0.75f;
	ProjCollbordersXF = 1.25f;
	ProjCollbordersY = 0.95f;
	ProjCollbordersYF = 1.25f;

	m_nGraphicCounterY = 3;
	m_nGraphicCounterX = 0;
	m_nGraphicAmountFrames = 6;
	attackdirectionX = 0.2f;
	attackdirectionY = -0.5f;
	m_nShockTime = 0.1f;
	

	FxColumn = 0;
	FXFrame = 4;
	
	m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

}

void cDynamic_creature_Bandit::AttackOne()
{
	enumCounter = 3;     //  <---Attack

	frameIndicator = 0;

	

	ProjCollbordersX = 0.25f;
	ProjCollbordersXF = 1.75f;
	ProjCollbordersY = 0.65f;
	ProjCollbordersYF = 1.8f;


	isAttack = true;
	vx = 0;
	m_nGraphicCounterY = 5;
	m_nGraphicCounterX = 0;
	m_nGraphicAmountFrames = 9;
	attackdirectionX = 0.3f;
	attackdirectionY = -0.5f;
	m_nShockTime = 0.1f;   

	FXFrame = 5;
	FxColumn = 1;

	m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;


}

void cDynamic_creature_Bandit::AttackTwo()
{
	isAttack = true;
	enumCounter = 12;     //  <---Attack Vertical

	frameIndicator = 0;


	
	ProjCollbordersX = 0.25f;
	ProjCollbordersXF = 1.75f;
	ProjCollbordersY = 0.65f;
	ProjCollbordersYF = 1.8f;


	vx = 0;
	m_nGraphicCounterY = 4;
	m_nGraphicCounterX = 0;
	m_nGraphicAmountFrames = 9;
	attackdirectionX = 0.3f;
	attackdirectionY = -0.5f;
	m_nShockTime = 0.1f;
	FxColumn = 4;  // <--Second column
	FxColumn = 2;
	m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;


}

void cDynamic_creature_Bandit::DeathFun()
{

	olc::vi2d fallFrame{ 0,11 };
	olc::vi2d landingframe{ 2,7 };

	std::pair<olc::vi2d, olc::vi2d> spritePos{ fallFrame, landingframe };

	if (M_nFacingDirection == EAST)
	{
		g_engine->AddParticle(new VfxDeath(this, m_pSpriteRight, spritePos, 0));
	}
	else
	{
		g_engine->AddParticle(new VfxDeath(this, m_pSpriteLeft, spritePos, 1));
	}
}

cDynamic_creature_Pantir::cDynamic_creature_Pantir() : cDynamic_Creature("Pantir", RPG_Assets::get().GetSprite("PantirRight"), RPG_Assets::get().GetSprite("PantirLeft"))
{

	m_pSpriteGrabLeft = RPG_Assets::get().GetSprite("PantirGrabLeft");
	m_pSpriteGrabRight = RPG_Assets::get().GetSprite("PantirGrabRight");
	M_BaseClass = AGILITY;
	M_nGraphicState = Idle;                                            //Pantir constructor
	m_fStateTick = 2.0f;
	Jumpcounter = 0;
	pEquipedWeapon = (cWeapon*)RPG_Assets::get().GetItem("Pantir's Dagger");
	
	
	bDead = false;
	//Borders for collision
	CollbordersX = 0.8f;
	CollbordersXF = 1.1f;

	CollbordersY = 0.6f;
	CollbordersYF = 1.5f;
	

	
	energeyAmount = 100;
	m_layer = Friend;
	
	
	fAttackcount = 0;


	BasicMovementSpeed = 12;
	 BasicAgility = 19;
	 BasicStrength = 15;
	 BasicIntelect = 13;


	 calculateStats();
	 hpUpdate = new cDynamic_HpBar(this->px, this->py, RPG_Assets::get().GetSprite("HPBarFull"), RPG_Assets::get().GetSprite("HPBarEmprty"), nHealth, this);
}





void cDynamic_creature_Pantir::IndicateAnim()
{

	switch (M_nGraphicState)
	{
	case Idle:
		enumCounter = 0;
		m_nGraphicCounterY = 8;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 13;

		
		FXFrame = 0;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case Landing:
		enumCounter = 11;
		m_nGraphicCounterY = 3;
		m_nGraphicCounterX = 10;
		m_nGraphicAmountFrames = 3;
		FXFrame = 0;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case Walk:
		enumCounter = 1;
		m_nGraphicCounterY = 0;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 9;
		FXFrame = 0;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

		break;
	case Jump:
		enumCounter = 2;
		m_nGraphicCounterY = 2;
		m_nGraphicCounterX = 9;
		m_nGraphicAmountFrames = 2;
		FXFrame = 0;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case JumpDown:
		enumCounter = 9;
		m_nGraphicCounterY = 2;
		m_nGraphicCounterX = 12;
		m_nGraphicAmountFrames = 2;
		FXFrame = 0;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case DoubleJump:
		enumCounter = 6;
		m_nGraphicCounterY = 3;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 7;
		FXFrame = 0;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

		break;
	case Attack:
		enumCounter = 3;
		
		break;

	case AttackGreat:
		enumCounter = 5;
	

		// fAttackcount = 0;
		break;

	case AttackEasy:
		enumCounter = 4;
		

		break;
	case JumpZenit:
		enumCounter = 10;
		m_nGraphicCounterY = 3;
		m_nGraphicCounterX = 8;
		m_nGraphicAmountFrames = 2;
		FXFrame = 0;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;

	case AirAttack:
		enumCounter = 7;
		
		break;
	case Eviscirate:
		enumCounter = 8;
		
		break;
	case AirEviscirate:
		enumCounter = 12;
		
		break;

	case Hit:
		enumCounter = 13;
		m_nGraphicCounterY = 5;
		m_nGraphicCounterX = 11;
		FXFrame = 0;
		m_nGraphicAmountFrames = 4;

		

		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;


	case AttackBack:
		enumCounter = 16;
		m_nGraphicCounterY = 5;
		m_nGraphicCounterX = 0;
		FXFrame = 3;
		m_nGraphicAmountFrames = 10;
		attackdirectionX = 1.0f;
		attackdirectionY = -0.9f;
		m_nShockTime = 0.15f;
		rageset = 25;
		

		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;

	case UpEviscirate:
		enumCounter = 17;
		m_nGraphicCounterY = 11;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 6;  //  <---Means how long our projectile should work
		attackdirectionX = 0.1f;
		attackdirectionY = -1.1f;
		m_nShockTime = 0.15f;
		IsThrow = true;
		


		FXFrame = 1;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

		break;
	case AirDownEviscirate:
		
		enumCounter = 18;

		switch (frameIndicator)
		{
		case 1:
			FXFrame = 2;
			
			isprojfollow = true;
			break;
		case 4:
			if (!bOnLanded)
			{
			//	FXFrame = 1;
				frameIndicator = 0;       // here technic droping indicator in 0
				m_nGraphicCounterX = 4;   // and first frame we set on four knowing that in drawing mepthod frame indicator will equal m_nGraphicCounter
			}

			break;

		case 6:		
			enumCounter = 19; // < -- Switch in LandingEviscirate
			RageMoveAttackLanding();
			break;
		}


		

		break;


	case LANDINGEviscirate :

		enumCounter = 19;
		
		

		if (frameIndicator == m_nGraphicAmountFrames)
		{
			bAnimAction = false;
			bIsAttackable = true;
		}

		break;


	case BLINKDESAPEAR:

		enumCounter =20;
		FXFrame = 0;
		isprojfollow = false;    // keep projectile follow at char 
		IsThrow = false;
		m_nGraphicCounterY =4;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 6;  //  <---Means how long our projectile should work
	
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		
	
	
		if (frameIndicator == m_nGraphicAmountFrames-1)
		{
			bDead = true;
		}

		break;



	case GRAB:

		enumCounter = 21;
		FXFrame = 0;
		
		//
		switch (Grabedenemy)  	//   Here on new spritesheet we must find exactly our target (Bandit, wolf, std)
		{
		case 0:   // Bandit
			m_nGraphicCounterY = 0;
			m_nGraphicCounterX = 0;
			break;

		}
		//
		m_nGraphicAmountFrames = 6;  //  <---Means how long our projectile should work

		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;



		if (frameIndicator == m_nGraphicAmountFrames - 1)    // in the end of turning we return sprite back
		{
			if (m_pSpriteReserve == RPG_Assets::get().GetSprite("PantirRight"))            // We check Reserve if there saved PantirRight
			{
				m_pSpriteRight = m_pSpriteReserve;         // we must in the end return it
				m_pSpriteReserve = nullptr;
			}
			else
			{
				m_pSpriteLeft = m_pSpriteReserve;
				m_pSpriteReserve = nullptr;
			}
			enumCounter = 22;
		}

		break;

	case Swirl:

		enumCounter = 22;
		//FXFrame = 0;
	
		m_nGraphicCounterY = 4;
		m_nGraphicCounterX = 7;
		m_nGraphicAmountFrames = 6;  //  <---Means how long our projectile should work

		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

		attackdirectionX = 0.1f;
		attackdirectionY = -0.3f;
		Hittimes = 4;  // <--How many times projectile should hit

		

		SetVerticalDirection(2);
		

		if (bOnLanded)
		{
			//ProjSize
			ProjCollbordersX = 0.0f;
			ProjCollbordersXF = 4.0f;
			ProjCollbordersY = 1.3f;
			ProjCollbordersYF = 2.7f;
			//


			
			bAnimAction = false;
			bIsAttackable = true;

			Target->bDraw = true;
			FxColumn = 8;
			PerformAttack();
			this->vy -=8;
			Jumpcounter++;
			//if (Target !=nullptr)
		//	Target = nullptr;
			bOnLanded = false;
			bControllable = true;
			enumCounter = 2;

			break;
		}
		else
		{

			if (frameIndicator == m_nGraphicEndFrame-1)
			{
				frameIndicator = 0;
			}
		}
		


		break;



	case Death:
		if (bOnLanded)
		{
			m_nGraphicCounterY = 5;
			m_nGraphicCounterX = 13;
			m_nGraphicAmountFrames = 1;  //  <---Means how long our projectile should work
			FXFrame = 0;

			m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		}
		else
		{
			m_nGraphicCounterY = 4;
			m_nGraphicCounterX = 13;
			m_nGraphicAmountFrames = 1;  //  <---Means how long our projectile should work
			FXFrame = 0;

			m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		}

		break;


	}

	




	}
	


bool cDynamic_creature_Pantir::checkFacingDirection(cDynamic* Enemu)
{
	cDynamic_Creature* Enemy = (cDynamic_Creature*)Enemu;

	int first = this->GetFacingDirection();
	int second = Enemy->GetFacingDirection();

	if(first == second)
	{
		//bBackstabActive = true;
		return true;  //1 means we looking at one side
	}


	return false;

}

int cDynamic_creature_Pantir::GetStats(int num)
{
	if(num==1)
	return agility;
	if (num == 2)
	return strength;
	if (num == 3)
	return intelligence;
	if (num == 4)
	return Defence;
	if (num == 5)
	return Movementspeed;
	if (num == 6)
	return AverageAttack;
	if (num == 7)
	return fHpRegeneration;
	if (num == 8)
	return versality;


	return 0;
}


void cDynamic_creature_Pantir::SwirlGrab( cDynamic* dyn)
{

	Target = dyn;

	isprojfollow = false;    // keep projectile follow at char 
	IsThrow = false;
	bAnimAction = true;  // <--blocking other animations while it's working
	bIsAttackable = false;
	
	bControllable = false;

	if (Target != nullptr)
	{
		if (Target->sName == "Bandit")
		{
			Grabedenemy = 0;
		}
	}

	if (this->GetFacingDirection() == 1)            // Take a sprite where we turn the enemy's head down towards the ground
	{
		m_pSpriteReserve = m_pSpriteLeft;              // in reserve we write changed sprite

		m_pSpriteLeft = m_pSpriteGrabLeft;         // we must in the end return it

		Target->px = this->px - (Target->CollbordersXF - Target->CollbordersX);

		
	}
	else
	{
		m_pSpriteReserve = m_pSpriteRight;

		m_pSpriteRight = m_pSpriteGrabRight;

		Target->px = this->px +(Target->CollbordersXF - Target->CollbordersX);
	}

	vy -= 7.0f;
	Target->vy = this->vy;

	Target->bDraw = false;

	enumCounter = 21;
		
}

void cDynamic_creature_Pantir::Behaviour(float fElapsedTime, cDynamic* player)
{

	

	if (bOnGround)
	{

	
		vx += -10.0f * vx * fElapsedTime;
		if (fabs(vx) < 0.01f)
		{
			vx = 0.0f;
		}
	}
	else
	{
		vx += -7.0f * vx * fElapsedTime;
		if (fabs(vx) < 0.01f)
		{
			vx = 0.0f;
		}
	}
	

	
	if (vx !=0)
	{
		energyCount += fElapsedTime * (25.0);
	}
	else
	{
		energyCount += fElapsedTime * 35.0f;
	}
	if (energyCount >=1)
	{
		energeyAmount += (int)energyCount;
		energyCount = 0.0f;		
	}


	if (energeyAmount >= 100 )
		energeyAmount = 100;
		
	
	if (energeyAmount <= 0 )
		energeyAmount = 0;	
	
	
	if ( rageAmount >= 100)
		rageAmount = 100;

	if ( rageAmount <= 0)
		rageAmount = 0;

	
	rageCount += fElapsedTime * 2.5f;

	if (rageCount >= 1)
	{
		rageAmount -= (int)rageCount;
		rageCount = 0.0f;
	}

	EnergyHide(fElapsedTime);



	if (targetTime>0)
	{
	targetTime -= fElapsedTime;
	}

	if (Target != nullptr&& Target->bDead == true)
	{
		targetTime = 0;
		Target = nullptr;

	}
	
}



void cDynamic_creature_Pantir::PerformAttack()
{
	if (pEquipedWeapon == nullptr)
		return;

	

			switch (M_nGraphicState)
			{
			case cDynamic_Creature::Idle:
				break;
			case cDynamic_Creature::Walk:
				break;
			case cDynamic_Creature::Jump:
				
				break;
			case cDynamic_Creature::Attack:

				this->energeyAmount -= 55;
				calculatedDamage = (AverageAttack + pEquipedWeapon->nDamage) * 2.65f;
				break;
			case cDynamic_Creature::AttackEasy:
				this->energeyAmount -=35;
				calculatedDamage = (AverageAttack - 20+pEquipedWeapon->nDamage) * 5;
				break;
			case cDynamic_Creature::AttackGreat:
				this->energeyAmount -= 100;
				calculatedDamage = (AverageAttack + 20 + pEquipedWeapon->nDamage) * 3;
				break;
			case cDynamic_Creature::DoubleJump:
				this->energeyAmount -= 20;
				break;
			case cDynamic_Creature::AirAttack:
				this->energeyAmount -= 35;
				calculatedDamage = (AverageAttack + pEquipedWeapon->nDamage) * 2.65f;
				break;
			case cDynamic_Creature::Eviscirate:
				this->energeyAmount += 15;
				calculatedDamage = (AverageAttack - 20 + pEquipedWeapon->nDamage) * 3.6f;
				this->rageAmount -= 35;
				break;
			case cDynamic_Creature::JumpDown:
				break;
			case cDynamic_Creature::JumpZenit:
				break;
			case cDynamic_Creature::Landing:
				break;
			case cDynamic_Creature::AirEviscirate:
				this->energeyAmount += 15;
				calculatedDamage = (AverageAttack - 20 + pEquipedWeapon->nDamage) * 3.6f;
				this->rageAmount -= 35;

				break;
			case cDynamic_Creature::Hit:
				this->energeyAmount -= 20;
				break;
			case cDynamic_Creature::AttackBack:
				this->energeyAmount -= 40;
				calculatedDamage = (AverageAttack+ pEquipedWeapon->nDamage) * 4;
				break;
			case cDynamic_Creature::UpEviscirate:
				this->energeyAmount += 15;
				calculatedDamage = (AverageAttack - 20 + pEquipedWeapon->nDamage) * 3.6f;
				this->rageAmount -= 35;
				break;
			case cDynamic_Creature::AirDownEviscirate:
				this->energeyAmount += 15;
				calculatedDamage = (AverageAttack -20 + pEquipedWeapon->nDamage) * 3.6f;
				this->rageAmount -= 35;
				break;
			case cDynamic_Creature::LANDINGEviscirate:
				this->energeyAmount += 15;
				calculatedDamage = (AverageAttack - 20 + pEquipedWeapon->nDamage) * 3.6f;
				this->rageAmount -= 35;

			case cDynamic_Creature::Swirl:
				this->energeyAmount -= 40;
				calculatedDamage = (AverageAttack - 20 + pEquipedWeapon->nDamage) * 3.6f;
				this->rageAmount -= 35;


				break;


			}

			pEquipedWeapon->OnUse(this);
}



cDynamic_Item::cDynamic_Item(float x, float y, cItem* i) : cDynamic("pickup")     //Constructror
{
	px = x;
	py = y;
	
	bSolidVsMap = true;
	
	m_layer = Neutral;
	bCollected = false;
	item = i;

	CollbordersX = 0.0f;
	CollbordersXF = 0.5f;


	CollbordersY = 0.0f;
	CollbordersYF = 0.5f;


}

void cDynamic_Item::OnInteract(cDynamic* player)
{
	if (bCollected)
		return;

	if(g_engine->GetListItem().size() > 23)
		return;
		//m_listItems.size() + 1

	if (item->OnInteract(player) )
	{

		
		//Add item to invenotory
		if (g_engine->GiveItem(item)) //< -- if return true means that full	
			return;
		

	}
	
	this->bDead =true;
	bCollected = true;
}
void cDynamic_Item::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{

	if (bCollected)
		return;
	gfx->DrawPartialDecal({ (px - ox) * 64,(py - oy) *64 }, item->pSprite, {0,0}, {64,64 },{1*0.5,1*0.5});   //offset pulling player back into the screen
	//gfx->DrawPartialSprite((px - ox) * 16.0f, (py - oy) * 16.0f, item->pSprite, 0, 0, 6, 16);



}

cDynamic_Projectile::cDynamic_Projectile(float ox, float oy, LAYER mlayer, float velx, float vely,  float duration, olc::Decal* spriteLeft, olc::Decal* spriteRight,  int m_nGraphicState, float m_nGraphicTime, int rageSet ) : cDynamic("projectile")
{
	gravity = true;
	
	pSpriteRight = spriteRight;
	pSpriteLeft = spriteLeft;
	agrsr = nullptr;
	this->m_GraphicState = setEnum(m_nGraphicState);
	fDuration = duration;
	px = ox;
	py = oy;
	vx = velx;
	vy = vely;


	HitAmount = 1;
	
	bSolidVsMap = true;
	bIsProjectile = true;
	knockbacktime = m_nGraphicTime;
	m_layer = mlayer;
	mass = 5;
	rageamount = rageSet;

	Frameindicator=0;

	CollbordersX = 0.25f;
	CollbordersXF = 2.0f;

	if (duration ==0.0f)      //Setup timer auto, when not 0
	{
		SetTimer(m_nGraphicState);
	}
	CollbordersY = 0.65f;
	CollbordersYF = 1.50f;

	
}

cDynamic_Projectile::cDynamic_Projectile(olc::Decal* spriteLeft, olc::Decal* spriteRight, cDynamic_Creature* Aggressor) : cDynamic("projectile")
{ 
	gravity = true;

	pSpriteRight = spriteRight;
	pSpriteLeft = spriteLeft;
	this->m_GraphicState = setEnum(Aggressor->FxColumn);
	SetTimer(Aggressor->FxColumn);
	px = Aggressor->px;
	py = Aggressor->py;
	vx = 0;
	vy = 0;


	bSolidVsMap = true;
	bIsProjectile = true;
	knockbacktime = Aggressor->m_nShockTime;
	m_layer = Aggressor->m_layer;
	mass = 5;
	rageamount =10;

	Frameindicator = 0;

	CollbordersX = 0.5f;
	CollbordersXF = 4.0f;


	
	
	CollbordersY = 1.3f;
	CollbordersYF = 2.7f;


	agrsr = Aggressor;
}

void cDynamic_Projectile::SetTimer(int charstate)
{
	
	switch (charstate)
	{

	

	case 0:   //<--Strong Attack
		fDuration = 6 * 0.1f;
		break;

	case 2:   //<--Easy Attack
		fDuration = 4 * 0.1f;
		break;

	case 3:  //<--Eviscirate
		fDuration = 6 * 0.1f;
		break;
	case 1:      // <-- Mid attack
		fDuration =4 * 0.1f;
		break;

	case 4:      // <-- AirEviscirate
		fDuration = 6 * 0.1f;
		break;
	case 5:      // <-- Back attack
		fDuration = 6 * 0.1f;
	case 6:      // <-- up Rageattack
		fDuration = 6 * 0.1f;
		break;

	case 7:      // <-- AirDownEviscRage
		fDuration = 6 * 0.1f;
		//repitable = true;

		break;
	case 8:      // <-- LandingEvis
		fDuration = 6 * 0.1f;
		break;

	case 22:      // <--Swirl
		fDuration = 6 * 0.1f;
		break;

	default:

		fDuration = 6 * 0.1f;
		break;

	}
	
}



void cDynamic_Projectile::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{

	if (animspr >= framespeed)                 //it's speed of frames
	{

		Frameindicator++;

		animspr -= framespeed;
	}

	if (agrsr != nullptr && repitable == true)
	{
		Frameindicator = Frameindicator % 6;  // make a cycle
			//fDuration = Frameindicator *0.1f+1;
		
	}

if (m_FacingDirection == 3)
	{

	gfx->DrawPartialDecal({ (px - ox) * 64.0f,(py - oy) * 64.0f }, pSpriteRight, { (float)Frameindicator * nSheetSize,(float)m_GraphicState * nSheetSize }, { nSheetSize,nSheetSize });   //offset pulling player back into the screen
	}
else
	{
	gfx->DrawPartialDecal({ (px - ox) *64.0f,(py - oy) * 64.0f }, pSpriteLeft, { (float)Frameindicator * nSheetSize,(float)m_GraphicState * nSheetSize }, { nSheetSize,nSheetSize });   //offset pulling player back into the screen

	}
	/*float angle = 0.0f;
	if (m_FacingDirection ==3)
	{
		angle = 0;
	}
	else
	{
		angle = 3.14;
	}

gfx->DrawRotatedDecal({ (px - ox) * 64.0f,(py - oy) * 64.0f }, RPG_Assets::get().GetSprite("Bandit ThrowDaggerFx"), atan2(vy,vx), {128,128});
std::cout << angle + atan2(vy - py, vx - px) << '\t' << vy << '\t' << vx << '\t'<<px<<'\t' << py << std::endl;*/

}

void cDynamic_Projectile::Update(float fElapsedTime, cDynamic* player)
{


	fDuration -= fElapsedTime;
	if (fDuration <= 0.0f)
		bRedundant = true;

		animspr += fElapsedTime;


		if (bOnGround)
		{
			vx = vx / 2;
		}

		if (repitable)
		{
			fDuration = 0.3f;
		}

		
		
		if (agrsr !=nullptr)
		{

			
			if (agrsr->isprojfollow == true)
			{
				

				px = agrsr->px + agrsr->ProjOffsetX;
				py = agrsr->py + agrsr->ProjOffsetY;

			}
		}

}

void cDynamic_Projectile::SwitchLayer(int num)
{
	switch (num)
	{
	case 0:
		this->m_layer = Friend;
		break;

	case 1:
		this->m_layer = Enemy;
		break;
	case 2:
		this->m_layer = Neutral;
		break;

	case 3:
		this->m_layer = Particle;
		break;
	}

}

int cDynamic_Projectile::setEnum(int charstate)
{
	switch (charstate)
	{

	case 1:  // <-- air attack pantir
		SpriteStateFX = MIDATTACK;
		return SpriteStateFX;
		break;

	case 0:
		SpriteStateFX = STRONGATTACK;
		return SpriteStateFX;
		break;

	case 2:  
		SpriteStateFX =EASYATTACK;
		return SpriteStateFX;
		break;

	case 3:        
		SpriteStateFX = EVISCIRATE;
		return SpriteStateFX;
		break;
	

	case 4:      // <-- Mid attack
		SpriteStateFX = AIREVISCIRATE;
		
		return SpriteStateFX;
		break;

	case 5:      // <-- Back attack
		SpriteStateFX = ATTACKBACK;
		return SpriteStateFX;
		break;

	case 6:      // <-- Back attack
		SpriteStateFX = ATTACKVERTICAL;
		return SpriteStateFX;
		break;
	case 7:      // <-- Back attack
		SpriteStateFX = ATTACKFOLLOWDOWN;
		return SpriteStateFX;
	case 8:      // <-- Back attack
		SpriteStateFX = ATTACKLANDING;
		return SpriteStateFX;

	default:
		nSheetSize = 0;
		return SpriteStateFX = STRONGATTACK;
		break;

	}
	return SpriteStateFX;
}

void cDynamic_HpBar::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{
	float hpdiff = charHp / procent;

	charHp = Hpowner->nHealth;
	px = Hpowner->px;
	py = Hpowner->py;
	if (Hpowner->bDead == true)
	{
		bRedundant = true; 
	}
	

	
		gfx->DrawPartialDecal({ (px - ox) * 64.0f+28,(py - oy) * 64.0f+18 }, HpEmpty, { 0,0 }, { (float)HpEmpty->sprite->width,(float)HpEmpty->sprite->height });   //offset pulling player back into the screen

		gfx->DrawPartialDecal({ (px - ox) * 64.0f+28,(py - oy) * 64.0f+18 }, HpFull, {  0,0 }, { hpdiff,(float)HpFull->sprite->height });   //offset pulling player back into the screen
	


		std::string sTextCurHp = std::to_string(charHp);
		std::string StextFullHp = std::to_string(hpMax);
	


		g_engine->DrawBigText(sTextCurHp + "/" + StextFullHp, (px - ox + 0.36f) * 64.0f + 28, (py - oy) * 64.0f + 16, 0.25, 0.25, olc::WHITE);
		
		//g_engine->DrawBigText(StextFullHp, (px - ox+1.5) * 64.0f + 56, (py - oy) * 64.0f + 37, 0.5, 0.5, olc::WHITE);
}

cDynamic_HpBar::cDynamic_HpBar(float ox, float oy,  olc::Decal* HpFulls, olc::Decal* HpEmptys, int hp, cDynamic_Creature* hpown) : cDynamic("HpFirst")
{
	hpMax = hp;
	HpFull = HpFulls;
	HpEmpty = HpEmptys;
	px = ox;
	py = oy;
	Hpowner = hpown;


	g_engine->AddIndicators(this);
}

void cDynamic_HpBar::Update(float fElapsedTime, cDynamic* player)
{
	
}

void cDynamic_HpBar::setLevelHp(int par)
{
	hpMax = par;
	procent = (float)hpMax / HpFull->sprite->width;
}

cDynamic_Teleport::cDynamic_Teleport(float x, float y, std::string mapName, float tx, float ty) : cDynamic("Teleport")
{
	px = x;
	py = y;
	fMapPosX = tx;
	fMapPosY = ty;
	sMapName = mapName;
	
	bSolidVsMap = false;
	gravity = false;

}

void cDynamic_Teleport::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{
	//Does Nothing
	gfx->DrawCircle(((px + 0.5f) - ox) * 64.0f, ((py + 0.5f) - oy) * 64.0f, 0.5f * 64.0f);

}

void cDynamic_Teleport::Update(float fElapsedTime, cDynamic* player)
{

	//Does Nothing
}


cDynamic_TextDamage::cDynamic_TextDamage(float px, float py, std::string DamageText, olc::Pixel color) : cDynamic("Text")
{
	this->px = px;
	this->py = py;
	this->DamageText = DamageText;
	Color = color;
	gravity = false;


	m_layer = Neutral;


	

}

void cDynamic_TextDamage::Update(float fElapsedTime, cDynamic* player )
{
	
	time += fElapsedTime;
	if (DamageText !="Fuck YOU!!!")
	{
	vy = -2 ;
	py = py + vy*fElapsedTime;
	}

	if (time>1)
	{
		time = 0;
		bRedundant = true;
	}

}

void cDynamic_TextDamage::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{

	//g_engine->DrawBigText(DamageText, ((this->px - ox)*64.0f), ((this->py - oy)*64.0f), 1, 1);



	int i = 0;
	int z = 0;
	for (auto c : DamageText)
	{
		if (c == 10)
		{
			z++;
			i = 0;
			continue;
		}

		float sx = ((c - 32) % 16) * 32;
		float sy = ((c - 32) / 16) * 32;



		gfx->DrawPartialDecal({ (float)((this->px - ox) * 64.0f) + (i * (18* g_engine->fscale)), (float)((this->py - oy) * 64.0f) + (z * 32) }, RPG_Assets::get().GetSprite("font"), {sx, sy}, {32, 32}, {1 * g_engine->fscale,1*g_engine->fscale}, Color);
		i++;
	}


//	gfx->DrawPartialDecal({ (px - ox) * 64.0f + 50,(py - oy) * 64.0f + 29 }, RageEmpty, { 0,0 }, { 155,32 });   //offset pulling player back into the screen

}

void cDynamic_creature_Pantir::MoveJump()
{
	vy = -9.0f;
	bAnimAction = false;    // <--Now jump can deflect animation downjumpevis
	IsOnePlay = false;
	isAttack = false;
	bOnLanded = false;
	isprojfollow = false;
	Btarget = false;
	Jumpcounter++;
	frameIndicator = 0;     // if we change it on 3 - it will be a comboo whith eviscirate but isAttack must be true
	
	//setEnum();
}

void cDynamic_creature_Pantir::MoveDoubleJump()
{
	IsOnePlay = true;
	isAttack = false;
	bOnLanded = false;
	isprojfollow = false;
	Btarget = false;
	enumCounter = 6;
	vy = -9.0f;
	Jumpcounter++;
	
	//setEnum();
}


void cDynamic_creature_Pantir::EnergyMoveAttackAir()
{
	if (!bHideMode )
	{


		isAttack = true;
		IsOnePlay = false;
		isprojfollow = true;    // keep projectile follow at char 
		Btarget = false;
	enumCounter = 7;  // <-- Air attack
	frameIndicator = 0;
	sparedVx = vx;

	Hittimes = 1;

	m_nGraphicCounterY = 6;
	m_nGraphicCounterX = 5;
	m_nGraphicAmountFrames = 11 - 5;
	attackdirectionX = 0.2f;
	attackdirectionY = -0.2f;
	m_nShockTime = 0.1f;

	rageset = 10;

	FXFrame = 6;
	FxColumn = 2;
	m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;


	//ProjSize
	ProjCollbordersX = 0.25f;
	ProjCollbordersXF = 1.75f;
	ProjCollbordersY = 0.65f;
	ProjCollbordersYF = 1.5f;
	//

	switch (GetFacingDirection())
	{
	case 1:       // <--East
		ProjOffsetX = -0.95f;
		ProjOffsetY = 0;
		break;
	case 3:       // <--Weast
		ProjOffsetX = 0.85f;
		ProjOffsetY = 0;
		break;

	}

}
 }

void cDynamic_creature_Pantir::EnergyMoveAttackLow()
{
	if (! bHideMode)
	{
		
		

		sparedVx = vx;
		isAttack = true;
		IsOnePlay = false;
		isprojfollow = true;    // keep projectile follow at char 
		Btarget = false;
		enumCounter = 4; // <--Easy sinister strike
		frameIndicator = 0;
		fAttackcount = 0.5f;

		Hittimes = 1;

		//ProjSize
		ProjCollbordersX = 0.25f;
		ProjCollbordersXF = 1.75f;
		ProjCollbordersY = 0.65f;
		ProjCollbordersYF = 1.5f;
		//


		m_nGraphicCounterY = 0;
		m_nGraphicCounterX = 10;
		m_nGraphicAmountFrames = 4;
		attackdirectionX = 0.2f;
		attackdirectionY = -0.3f;
		m_nShockTime = 0.1f;
		FXFrame = 11;
		FxColumn = 2;
		rageset = 15;

		switch (GetFacingDirection())
		{
		case 1:       // <--East
			ProjOffsetX = -0.95f;
			ProjOffsetY = 0;
			break;
		case 3:       // <--Weast
			ProjOffsetX = 0.85f;
			ProjOffsetY = 0;
			break;

		}
		



		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
	}
}
void cDynamic_creature_Pantir::HideStage()
{
	if (energeyAmount>50 && bHideMode ==false)
	{
	moveBonus+= 10;
	m_layer = Neutral;
    energeyAmount -= 75;
	bHideMode = true;
	IsOnePlay = false;
	isAttack = false;
	Btarget = false;
	blueColor = 0;
	greenColor = 0;
	redColor = 0;
	}

}
void cDynamic_creature_Pantir::EnergyMoveAttackMid()
{
	if (!bHideMode)
	{

		isAttack = true;
		IsOnePlay = false;
		enumCounter = 3; // <-- Mid sinister strike
		isprojfollow = true;    // keep projectile follow at char 
		Btarget = false;
		frameIndicator = 0;
		fAttackcount = 0.5f;
		sparedVx = vx;

		Hittimes = 1;

		//ProjSize
		ProjCollbordersX = 0.25f;
		ProjCollbordersXF = 1.75f;
		ProjCollbordersY = 0.65f;
		ProjCollbordersYF = 1.5f;
		//

		m_nGraphicCounterY = 2;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 8;
		attackdirectionX = 0.5f;
		attackdirectionY = -0.1f;
		m_nShockTime = 0.15f;    // <--knockbacktime



		rageset = 20;

		FXFrame = 2;
		FxColumn = 1;

		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;


		switch (GetFacingDirection())
		{
		case 1:       // <--East
			ProjOffsetX = -0.95f;
			ProjOffsetY = 0;
			break;
		case 3:       // <--Weast
			ProjOffsetX = 0.85f;
			ProjOffsetY = 0;
			break;

		}

	}
}

void cDynamic_creature_Pantir::EnergyMoveAttackHigh()
{
	if (!bHideMode)
	{
		isAttack = true;
		IsOnePlay = false;
		enumCounter = 5; // <-- High sinister strike
		isprojfollow = true;    // keep projectile follow at char 
		Btarget = false;  // <--- Set projectile take target enemy for blink or other
		frameIndicator = 0;
		fAttackcount = 0.5f;
		sparedVx = vx;

		Hittimes =2;

		//ProjSize
		ProjCollbordersX = 0.25f;
		ProjCollbordersXF = 1.75f;
		ProjCollbordersY = 0.65f;
		ProjCollbordersYF = 1.5f;
		//

		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 14;
		attackdirectionX = 0.9f;
		attackdirectionY = -0.2f;
		m_nShockTime = 0.2f;

		FXFrame = 8;
		FxColumn = 0;
		rageset = 30;

		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;



		switch (GetFacingDirection())
		{
		case 1:       // <--East
			ProjOffsetX = -0.95f;
			ProjOffsetY = 0;
			break;
		case 3:       // <--Weast
			ProjOffsetX = 0.85f;
			ProjOffsetY = 0;
			break;

		}

	}
}

void cDynamic_creature_Pantir::RageMoveAttackUp()
{
	if (!bHideMode)
	{
		sparedVx = vx;
		isAttack = true;
		IsOnePlay = false;
		isprojfollow = true;    // keep projectile follow at char 
		enumCounter = 8;     //Eviscirate throw up
		frameIndicator = 0;

		rageset = 0;
		Hittimes = 2;
		//
		if ((g_engine->GetLearnedTalent(5)))	
		Btarget = true;// <--- Set projectile take target enemy for blink or other


		targetTime = 0.2f;
		//
		//ProjSize
		ProjCollbordersX = 0.25f;
		ProjCollbordersXF = 1.7f;
		ProjCollbordersY = 0.15f;
		ProjCollbordersYF = 1.8f;
		//
		m_nGraphicCounterY = 10;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 12;
		attackdirectionX = 0.0f;
		attackdirectionY = -1.1f;
		m_nShockTime = 0.1f;


		FXFrame = 4;
		FxColumn = 3;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;


		switch (GetFacingDirection())
		{
		case 1:       // <--East
			ProjOffsetX = -0.95f;
			ProjOffsetY = 0;
			break;
		case 3:       // <--Weast
			ProjOffsetX = 0.85f;
			ProjOffsetY = 0;
			break;

		}
	}


}

void cDynamic_creature_Pantir::RageMoveAttackAir()
{
	if (!bHideMode)
	{
		sparedVx = vx;

		isAttack = true;
		IsOnePlay = false;
		Btarget = false; // <--- Set projectile take target enemy for blink or other
		enumCounter = 12;  // air eviscirate
		isprojfollow = false;    // keep projectile follow at char 
		frameIndicator = 0;
		rageset = 0;
		Hittimes = 2;

		vy -= 5;

		//ProjSize
		ProjCollbordersX = 0.25f;
		ProjCollbordersXF = 1.7f;
		ProjCollbordersY = 0.6f;
		ProjCollbordersYF = 1.8f;
		//

		m_nGraphicCounterY = 9;
		m_nGraphicCounterX = 0;
		attackdirectionX = 0.0f;
		attackdirectionY = +2.5f;
		m_nShockTime = 0.1f;




		FXFrame = 5;
		FxColumn = 4;
		m_nGraphicAmountFrames = 11;


		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

	}
}

void cDynamic_creature_Pantir::EnergyMoveAttackBack()
{
	if (!bHideMode)
	{
		isAttack = true;
		IsOnePlay = false;
		Btarget = false; // <--- Set projectile take target enemy for blink or other
		enumCounter = 16; // <-- BackStab
		frameIndicator = 0;
		sparedVx = vx;
		isprojfollow = true;    // keep projectile follow at char 

		Hittimes = 1;


		FxColumn = 5;

		//ProjSize
		ProjCollbordersX = 0.25f;
		ProjCollbordersXF = 1.75f;
		ProjCollbordersY = 0.65f;
		ProjCollbordersYF = 1.8f;
		//



		switch (GetFacingDirection())
		{
		case 1:       // <--East
			ProjOffsetX = -0.95f;
			ProjOffsetY = 0;
			break;
		case 3:       // <--Weast
			ProjOffsetX = 0.85f;
			ProjOffsetY = 0;
			break;

		}
	}
}

void cDynamic_creature_Pantir::SetVerticalDirection(int Var)
{
	switch (Var)
	{
	case 1:
		M_nFacingDirectionVertical = NORTH;
		break;
	case 2:
		M_nFacingDirectionVertical = SOUTH;
		break;
	case 3:
		M_nFacingDirectionVertical = NOTLOOKING;
		break;
	}
}
void cDynamic_creature_Pantir::SetFacingDirection(int Var)
{
	switch (Var)
	{
	case 1:
		M_nFacingDirection = WEST;
		break;
	case 3:
		M_nFacingDirection = EAST;
		break;
	}
}
void cDynamic_creature_Pantir::EnergyHide(float fElapsedTime)
{
	if (bHideMode)
	{
		Hidetimer -= fElapsedTime * 2.0f;

		if (Hidetimer < 0.5f)
		{
			redColor = (0.5f - Hidetimer) * 510;
			greenColor = (0.5f - Hidetimer) * 510;
			blueColor = (0.5f - Hidetimer) * 510;


			if (Hidetimer <= 0)
			{
				m_layer = Friend;
				redColor = 255;
				greenColor = 255;
				blueColor = 255;
				moveBonus -= 10;
				bHideMode = false;
				Hidetimer = 2.0f;
			}
		}
	}
}
void cDynamic_creature_Pantir::RageMoveAttackJumpUp()
{
	if (!bHideMode)
	{
	

		isAttack = true;
		IsOnePlay = false;
		bAnimAction = false;
		Btarget = false; // <--- Set projectile take target enemy for blink or other
		enumCounter = 17;  //  eviscirate jump Up
		isprojfollow = false;    // keep projectile follow at char 
		frameIndicator = 0;

		vy -= 15;
		rageset = 0;
		Hittimes = 2;

		//ProjSize
		ProjCollbordersX = 0.2f;
		ProjCollbordersXF = 1.9f;
		ProjCollbordersY = 0.15f;
		ProjCollbordersYF = 1.8f;
		//

		FxColumn = 6;

	}
}
void cDynamic_creature_Pantir::RageMoveAttackAirDown()
{
	if (!bHideMode)
	{


		isAttack = true;
		IsOnePlay = false;

		bAnimAction = true;  // <--blocking other animations while it's working
		bIsAttackable = false;

		Btarget = false;  // <--- Set projectile take target enemy for blink or other

		enumCounter = 18;  //  eviscirate jump down
		isprojfollow = true;    // keep projectile follow at char 
		frameIndicator = 0;

		vy = 0;
		rageset = 0;
		Hittimes = 2;

		//ProjSize
		ProjCollbordersX = 0.2f;
		ProjCollbordersXF = 1.9f;
		ProjCollbordersY = 0.75f;
		ProjCollbordersYF = 1.8f;
		//

		m_nGraphicCounterY = 7;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 7;  //  <---Means how long our projectile should work
		attackdirectionX = 0.1f;
		attackdirectionY = 1.1f;
		m_nShockTime = 0.15f;

		
			ProjOffsetX = 0.0f;
		     // <--Weast
			ProjOffsetY = 0.25f;
		

			FxColumn = 7;


		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

	}
}

void cDynamic_creature_Pantir::RageMoveAttackLanding()
{
	FXFrame = 8;
	isprojfollow = true;    // keep projectile follow at char 
	IsThrow = false;
	m_nGraphicCounterY = 7;
	m_nGraphicCounterX = 5;
	m_nGraphicAmountFrames = 9;  //  <---Means how long our projectile should work
	attackdirectionX = 0.1f;
	attackdirectionY = -0.3f;

	SetVerticalDirection(2);
	m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

	Hittimes = 4;  // <--How many times projectile should hit
	//ProjSize

	ProjCollbordersX = 0.0f;
	ProjCollbordersXF = 2.1f;
	ProjCollbordersY = 0.75f;
	ProjCollbordersYF = 1.8f;
	//

	ProjOffsetX = 0.0f;
	// <--Weast
	ProjOffsetY = 0.0f;

	FxColumn = 8;

}

void cDynamic_creature_Pantir::BlinkBehind()
{
	if (Target !=nullptr)
	{



	cDynamic_Creature* TargetChild = (cDynamic_Creature*)Target;

	


	if (TargetChild->GetFacingDirection() ==1)    // Here we decide which position we should put our char it  ( offset px)
	{
		// just for simple reading code
		float px = TargetChild->px + (TargetChild->CollbordersXF - TargetChild->CollbordersX);  // <-- new position char
		float py = TargetChild->py;
		//

		if (g_engine->CheckPosition(px + this->CollbordersX, px + this->CollbordersXF, py + this->CollbordersY, py + this->CollbordersYF)) // checkPosition on solidblocks  if @true@ means that we can teleport
		{


			// Create shadow 
			cDynamic_creature_Pantir* shadow = new cDynamic_creature_Pantir;
			shadow->m_layer = Neutral;
			shadow->isAttack = true;
			shadow->px = this->px;
			shadow->py = this->py;
			shadow->enumCounter = 20;
			shadow->gravity = false;
			shadow->framespeed = 0.1f;
			shadow->SetFacingDirection(this->GetFacingDirection());
			g_engine->AddVecDynamic(shadow);



			this->SetFacingDirection(TargetChild->GetFacingDirection());  // here we grab  a way where looking our target 

			//gravityofftimer = 1.0f;
			//TargetChild->gravityofftimer = 1.0f;

			this->vx = Target->vx;  // copy velocity our target
			this->vy = Target->vy;


			//Copy from jump
			bAnimAction = false;    // <--Now jump can deflect animation downjumpevis
			IsOnePlay = false;
			isAttack = false;
			bOnLanded = false;
			isprojfollow = false;
			Btarget = false;
			Jumpcounter++;
			frameIndicator = 0;     // if we change it on 3 - it will be a comboo whith eviscirate but isAttack must be true
			//Copy from jump



		this->px = px;
		this->py = py;



		}
		
	}
	else
	{
		//
		float px = TargetChild->px - (TargetChild->CollbordersXF - TargetChild->CollbordersX);
		float py = TargetChild->py;
		//
		if (g_engine->CheckPosition(px + this->CollbordersX, px + this->CollbordersXF, py + this->CollbordersY, py + this->CollbordersYF)) // checkPosition on solidblocks
		{


			// Create shadow 
			cDynamic_creature_Pantir* shadow = new cDynamic_creature_Pantir;
			shadow->m_layer = Neutral;
			shadow->isAttack = true;
			shadow->px = this->px;
			shadow->py = this->py;
			shadow->enumCounter = 20;
			shadow->gravity = false;
			shadow->framespeed = 0.1f;
			shadow->SetFacingDirection(this->GetFacingDirection());
			g_engine->AddVecDynamic(shadow);



			this->SetFacingDirection(TargetChild->GetFacingDirection());  // here we grab  a way where looking our target 

			//gravityofftimer = 1.0f;
			//TargetChild->gravityofftimer = 1.0f;

			this->vx = Target->vx;  // copy velocity our target
			this->vy = Target->vy;


			//Copy from jump
			bAnimAction = false;    // <--Now jump can deflect animation downjumpevis
			IsOnePlay = false;
			isAttack = false;
			bOnLanded = false;
			isprojfollow = false;
			Btarget = false;
			Jumpcounter++;
			frameIndicator = 0;     // if we change it on 3 - it will be a comboo whith eviscirate but isAttack must be true
			//Copy from jump


			this->px = px;
			this->py = py;


		}
	}
	

	Target = nullptr;

	}
}

bool cDynamic_creature_Pantir::IsLanded()
{

	if (bOnLanded)
	{
		return true;
	}
	else
	{
		bOnLanded = true;
		
		
		if (bAnimAction)   // <--Means when we action stage don't switch animation on onladed
		{
			return false;
		}
		enumCounter = 11;
		isAttack = false;
		IsOnePlay = false;
		frameIndicator = 0;
		setEnum();
		return false;
	}
}

void cDynamic_creature_Pantir::DeathFun()
{

	olc::vi2d fallFrame{ 5,13 };
	olc::vi2d landingframe{4,13 };
	
	std::pair<olc::vi2d, olc::vi2d> spritePos{ fallFrame, landingframe };

	if (M_nFacingDirection == EAST)
	{
	g_engine->AddParticle(new VfxDeath(this, m_pSpriteRight, spritePos, 0));
	}
	else
	{
		g_engine->AddParticle(new VfxDeath(this, m_pSpriteLeft, spritePos,1));
	}


	
}



cDynamic_creature_Enemy::cDynamic_creature_Enemy(std::string n, olc::Decal* spriteRight, olc::Decal* spriteLeft) : cDynamic_Creature(n, spriteRight, spriteLeft)
{
	m_layer = Enemy;
	nHealth = 50;
	nHealthMax = 50;
	bDead = false;
	m_fStateTick = 0.3f;
	bOnGround = false;
	Jumpcounter = 0;
}

void cDynamic_creature_Enemy::Behaviour(float fElapsedTime, cDynamic* player)
{
	if (bControllable) // when we control in scripteditor
	{
		float fTargetX = player->px - px;
		float fTargetY = player->py - py;
		float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

		

		m_fStateTick -= fElapsedTime;

		if (m_fStateTick <= 0.0f)          //only if stateTick less than zero we gonna do anything. For optimization
		{
			if (nHealth > 0)
			{
				if (fDistance < 9.0f) // before target
				{

				


					if (bOnGround)  // if stay on ground
					{

						if (player->m_layer == Friend)
						{



						if (!isAttack && !IsOnePlay)   // if no attack b noplay (hit)
						{



							vx = (fTargetX / fDistance) *(Movementspeed/4);

							
							//vy = (fTargetY / fDistance) * 1.0f;


							if (vx>0)
							{
								M_nFacingDirection = EAST;
								
							}
							else
							{
								M_nFacingDirection = WEST;
								
								

								
								
							}

							if (fDistance < 2.9f && rand() % 5 == 4)  // && rand() % 25 == 5
							{

								SpecAttack(fTargetX, fTargetY,fDistance);


							}
							else if (fDistance < 1.6f)
							{
								if (rand() % 10 >= 5 && !isAttack)
								{

									AttackOne();

								}
								else 
								{
									
									AttackTwo();
									}
								}
							}
						}
						else
						{
							vx = 0;
						}

					}
					else
					{



					}
				}
				else
				{
					float vxhelp1, vxhelp2;








					//Randomizer walking bandit 
					vxhelp1 = rand()%2;
					vxhelp2 = rand() % 2;


					vx = vxhelp2 - vxhelp1;



					if (vx > 0)
					{
						M_nFacingDirection = EAST;

					}
					else
					{
						M_nFacingDirection = WEST;
					}



					//vy = 0;
					cDynamic_TextDamage* Text;
					switch (rand() % 50)
					{
					case 1:
						Text = new cDynamic_TextDamage(px + CollbordersX + 0.25f, py, "We shut all them.", olc::RED);
						g_engine->GiveNewText(Text);
						break;
					case 2:

						Text = new cDynamic_TextDamage(px + CollbordersX + 0.25f, py, "Boss Protect Us...", olc::RED);
						g_engine->GiveNewText(Text);
						break;
					case 3:
						if (abs(fTargetX) < 7.0f)
						{

							Text = new cDynamic_TextDamage(px + CollbordersX + 0.25f, py, "Fuck you!!!", olc::RED);
							g_engine->GiveNewText(Text);

						}
						break;

					}


				}
			}
			else
			{

				bDead = true;
			}

			m_fStateTick += 0.35f;

		}

		if (bOnGround)    
		{

		if (M_nFacingDirection == EAST)
		{
			
			if (g_engine->CheckPosition(px + CollbordersXF + (vx * fElapsedTime), py + CollbordersYF + 0.4))
				vx = 0;
		}
		else
		{
			
			if (g_engine->CheckPosition(px + CollbordersX + (vx * fElapsedTime),  py + CollbordersYF + 0.4))
				vx = 0;

		}

		}

	}


}

void cDynamic_creature_Enemy::PerformAttack()
{
	if (pEquipedWeapon == nullptr)
		return;

	SetUpDamage();
	pEquipedWeapon->OnUse(this);
}

cDynamic_creature_BossBandit::cDynamic_creature_BossBandit():  cDynamic_creature_Enemy("BanditBoss", RPG_Assets::get().GetSprite("BanditBossRight"), RPG_Assets::get().GetSprite("BanditBossLeft"))
{
	nSheetSizeX = 256.0f;
	nSheetSizeY = 256.0f;


	//collision borders
	CollbordersX = 0.75f*2;
	CollbordersXF = 1.18f*2;


	CollbordersY = 0.7f*2;
	CollbordersYF = 1.5f*2;
	//

	pEquipedWeapon = (cWeapon*)RPG_Assets::get().GetItem("Bandit Boss Sword");
	hpUpdate = new cDynamic_HpBar(px, py, RPG_Assets::get().GetSprite("HPBarFull"), RPG_Assets::get().GetSprite("HPBarEmprty"), nHealth, this);

	LvL = 1;



	BasicMovementSpeed = 20;
	BasicAgility = 7;
	BasicStrength = 50;
	BasicIntelect = 13;

	calculateStats();

}

void cDynamic_creature_BossBandit::Behaviour(float fElapsedTime, cDynamic* player)
{
	if (bControllable == 1) // when we control in scripteditor
	{
		float fTargetX = player->px - px;
		float fTargetY = player->py - py;
		float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);


		m_fStateTick -= fElapsedTime;

		if (m_fStateTick <= 0.0f)          //only if stateTick less than zero we gonna do anything. For optimization
		{
			if (nHealth > 0)
			{
				if (fDistance < 9.0f && player->m_layer == Friend) // before target
				{




					if (bOnGround)  // if stay on ground
					{
						if (isAttack==false && IsOnePlay==false)   // if isAttack false and isOneplay false
						{



							vx = (fTargetX / fDistance) * (Movementspeed / 4);


							//vy = (fTargetY / fDistance) * 1.0f;


							if (player->px > this->px+0.5f)
							{
								M_nFacingDirection = EAST;

							}
							else
							{
								M_nFacingDirection = WEST;





							}

						
							 if (fDistance < 2.2f)
							{
								 int randv = rand() % 12;
								if (randv >= 7 )
								{

									
									//Mocking();

									SpecAttack2(fTargetX, fTargetY, fDistance); // vertushka

								}
								else if (randv <=4)
								{
									
									AttackTwo();
								}
								else if (randv>=5 && randv < 7)
								{
								 SpecAttack(fTargetX, fTargetY, fDistance); // nunchaki

								}
								else
								{
									Mocking();

								}

								
							 }
							
						}

					}
					else
					{



					}
				}
				else
				{
					float vxhelp1, vxhelp2;


					//Randomizer walking bandit 
					vxhelp1 = rand() % 2;
					vxhelp2 = rand() % 2;


					vx = vxhelp2 - vxhelp1;

					//vy = 0;
					cDynamic_TextDamage* Text;
					switch (rand() % 50)
					{
					case 1:
						Text = new cDynamic_TextDamage(px + CollbordersX + 0.25f, py, "We shut all them.", olc::RED);
						g_engine->GiveNewText(Text);
						break;
					case 2:

						Text = new cDynamic_TextDamage(px + CollbordersX + 0.25f, py, "Boss Protect Us...", olc::RED);
						g_engine->GiveNewText(Text);
						break;
					case 3:
						if (abs(fTargetX) < 7.0f)
						{

							Text = new cDynamic_TextDamage(px + CollbordersX + 0.25f, py, "Fuck you!!!", olc::RED);
							g_engine->GiveNewText(Text);

						}
						break;

					}


				}
			}
			else
			{

				bDead = true;
			}

			m_fStateTick += 0.3f;

		}

		if (bOnGround)
		{



			if (M_nFacingDirection == EAST)
			{

				if (g_engine->CheckPosition(px + CollbordersXF + (vx * fElapsedTime), py + CollbordersYF + 0.4))
					vx = 0;
			}
			else
			{

				if (g_engine->CheckPosition(px + CollbordersX + (vx * fElapsedTime), py + CollbordersYF + 0.4))
					vx = 0;

			}

		}

	}



}

void cDynamic_creature_BossBandit::SpecAttack(float targetX, float targetY, float Distance)
{

	isAttack = true;
	isprojfollow = true;
	enumCounter = 4;     //  <---Attack Huricane
	looptimes = 2;
	frameIndicator = 0;
	FxColumn = 8;    //<--  3 column      place eviscirate
	vx = 0;

	m_nShockTime = 0.3f;
	FXFrame = 7;
	//IsThrow = true;
	//isprojfollow = true;

	//ProjSize
	ProjCollbordersX = 0.6f;
	ProjCollbordersXF = 3.4f;
	ProjCollbordersY = 0.8f;
	ProjCollbordersYF = 3.5f;
	//
	M_nFacingDirectionVertical = NORTH;
	attackdirectionX = 0.3f;
	attackdirectionY = -0.5f;

	m_nGraphicCounterY = 3;
	m_nGraphicCounterX = 0;
	m_nGraphicAmountFrames = 12;

	ProjOffsetX = 0.0f;
	ProjOffsetY = 0.0f;


	attackdif = 1;

	m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
}


void cDynamic_creature_BossBandit::SpecAttack2(float targetX, float targetY, float Distance)
{


	isAttack = true;
	isprojfollow = true;
	enumCounter = 4;     //  <---Attack Huricane
	looptimes = 2;
	frameIndicator = 0;
	FxColumn = -1;    //<--  3 column      place eviscirate
	vx = 0;
	sparedVx = (targetX / Distance > 0) ? 5 : -5; //vx storrage
	
	m_nShockTime = 0.3f;
	//IsThrow = true;
	//isprojfollow = true;

	//ProjSize
	ProjCollbordersX = 0.5f;
	ProjCollbordersXF = 3.36f;
	ProjCollbordersY = 0.8f;
	ProjCollbordersYF = 3.25f;
	//
	M_nFacingDirectionVertical = NORTH;
	attackdirectionX = 0.3f;
	attackdirectionY = -0.5f;

	m_nGraphicCounterY = 5;
	m_nGraphicCounterX = 5;
	m_nGraphicAmountFrames = 12;

	ProjOffsetX = 0.0f;
	ProjOffsetY = 0.0f;


	attackdif = 2; // in indicate state for difference from nunchaki stage

	m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
	FXFrame = m_nGraphicEndFrame -3;
}


void cDynamic_creature_BossBandit::Mocking()
{


	IsOnePlay = true;
	enumCounter = 11;     //  <---Attack Monkey
	vx = 0;
	isDirectionLock = true;
	looptimes = 3;

}

void cDynamic_creature_BossBandit::AttackTwo()
{

	isAttack = true;
	//IsThrow = true;
	isprojfollow = true;
	enumCounter = 3;     //  <---Attack   pierce

	frameIndicator = 0;

	m_nGraphicCounterY = 4;
	m_nGraphicCounterX = 4;
	m_nGraphicAmountFrames = 9;
	FXFrame = 9;
	vx = 0;

	FxColumn = 0;    //<--  3 column      
	//ProjSize
	ProjCollbordersX = 0.25f;
	ProjCollbordersXF = 1.75f;
	ProjCollbordersY = 1.2f;
	ProjCollbordersYF = 2.5f;
	//

	M_nFacingDirectionVertical = NOTLOOKING;

	attackdirectionX = 0.3f;
	attackdirectionY = -0.5f;
	m_nShockTime = 0.1f;

	m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
	//sparedVx = -vx;
	//vy = -10.0f;
	//frameIndicator = 0;
	//setEnum();

	switch (GetFacingDirection())
	{
	case 1:       // <--East
		ProjOffsetX = -0.25f;
		ProjOffsetY = 0.5f;
		break;
	case 3:       // <--Weast
		ProjOffsetX = +2.1f;
		ProjOffsetY = 0.5f;
		break;

	}



}

void cDynamic_creature_BossBandit::IndicateAnim()
{

	switch (M_nGraphicState)
	{
	case Idle:
		enumCounter = 0;
		m_nGraphicCounterY = 0;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 8;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;

	case Walk:
		enumCounter = 1;
		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 2;
		m_nGraphicAmountFrames = 7;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

		break;
	


	case Landing:   // <--Mokicng

		enumCounter = 11;
		m_nGraphicCounterY = 2;
		m_nGraphicCounterX = 2;
		m_nGraphicAmountFrames = 6;


		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		if (frameIndicator == m_nGraphicEndFrame - 1 && looptimes != 0)
		{
			frameIndicator = 2;
			looptimes -= 1;
		}
		break;
	case Attack:        // <-- (Horizontal Attack)
		enumCounter = 3;

		



		break;
	case AttackEasy:// <-- nunchaki and huricane
		enumCounter = 4;

	
		switch (attackdif)
		{
		case 1:

			if (frameIndicator == m_nGraphicEndFrame - 1 && looptimes != 0)
			{
				frameIndicator = 6;
				looptimes -= 1;
			}

			break;
		case 2:


			if (frameIndicator == m_nGraphicEndFrame - 1 && looptimes != 0)
			{
				frameIndicator = m_nGraphicEndFrame-4;
				vx = sparedVx;
				looptimes -= 1;
			}


			break;
		}




		break;
	case AttackGreat:




		break;
	case Eviscirate:

		break;
	case AirEviscirate: 
		


		break;

	


	}
}

VfxParticles::VfxParticles(float px, float py)  : cDynamic("Particles")
{
	this->px = px;
	this->py = py;
}

VfxLevel::VfxLevel(float px, float py) : VfxParticles(px,py)
{
	
	VfxParticle = RPG_Assets::get().GetSprite("LevelVfx");

	time = 1.0f;
	timecount = 0.0f;
	AlphaColor = 255.0f;
	for (int i = 0; i < 36; ++i)
	{
		// Randomize starting positions, speeds, and angles for the raindrops
	
		float speed = static_cast<float>(rand() %3)+5 ; // Random speed between 50 and 150 pixels per second
		float angle = (i*10)* 3.14159f / 180.0f; // Random angle between 0 and pi radians

		olc::vf2d vSize{ std::cos(angle),std::sin(angle)};
		
		m_vecVfxLevel.push_back(vSize);
		m_speed.push_back(speed);
	}

}
void VfxLevel::DrawSelf(olc::PixelGameEngine* gfx, float offsetx, float offsety)
{


	for (auto& i : m_vecVfxLevel )
	{

		float angle = std::atan2(i.y, i.x);

		gfx->DrawRotatedDecal({ (this->px + (i.x*2) - offsetx) * 64.0f ,(this->py + (i.y*2) - offsety) * 64.0f }, VfxParticle, angle, { 0,0 }, { 1,1 }, olc::Pixel(255, 255, 255, AlphaColor));
	//gfx->DrawPartialDecal({ (px+(i.x*10)) ,(py+(i.y*20)) }, VfxParticle, { 0,0 }, { 8,8 }, { 1,1 }, olc::WHITE);   //offset pulling player back into the screen
	}

}

void VfxLevel::Update(float fElapsedTimeá, cDynamic* player)
{

	timecount += fElapsedTimeá;

	AlphaColor = 255-(timecount * 100 * 2.55f);


	

	px = player->px+0.95f;
	py = player->py+1.0f;

	if (timecount>=time)
	{
		bRedundant =true;
		AlphaColor = 0;
	}

	
	int index = 0;

	for (auto& i : m_vecVfxLevel)
	{
		int speed = m_speed[index];

		//i.xSpeed

		i.x -= i.x * speed * fElapsedTimeá;
		i.y -= i.y * speed * fElapsedTimeá;
		index++;

	}

}

//VfxParticles::VfxParticles(float px, float py) : cDynamic("Particles")
VfxShot::VfxShot(float px, float py) : VfxParticles(px, py)
{
	this->VfxParticle = RPG_Assets::get().GetSprite("SoulShotVfx");
	bSolidVsMap = true;
	time = 1.0f;
	timecount = 0.0f;
	AlphaColor = 255.0f;

	CollbordersX = 0;
	CollbordersXF = 0;
	CollbordersY = 0;
	CollbordersYF = 0;

	for (int i = 0; i < 18; ++i)
	{
		// Randomize starting positions, speeds, and angles for the raindrops

		float speed = static_cast<float>(rand() % 5) + 1; // Random speed between 50 and 150 pixels per second
		float angle = (i * 10+180) * 3.14159f / 180.0f; // Random angle between 0 and pi radians

		olc::vf2d vSize{ std::cos(angle),std::sin(angle) };

		m_vecVfxLevel.push_back(vSize);
		m_speed.push_back(speed);
	}


}

void VfxShot::Update(float fElapsedTimeá, cDynamic* player)
{

	timecount += fElapsedTimeá;

	AlphaColor = 255 - (timecount * 100 * 2.55f);

	if (timecount >= time)
	{
		bRedundant = true;
		AlphaColor = 0;
	}


	int index = 0;

	for (auto& i : m_vecVfxLevel)
	{
		int speed = m_speed[index];

		//i.xSpeed


		if (g_engine->CheckParticlePosition(this->px + (i.x * 0.3f), this->py + (i.y * 0.3f))) // checkPosition on solidblocks  if @true@ means that we can teleport
		{
			

			i.x += i.x * speed * fElapsedTimeá;
			i.y += i.y * speed * fElapsedTimeá + 0.09f;
		}
			index++;
	}

}



void VfxShot::DrawSelf(olc::PixelGameEngine* gfx, float  offsetx, float offsety)

{

	for (auto& i : m_vecVfxLevel)
	{

		//gfx->DrawRect((this->px + (i.x * 0.3f) - offsetx) * 64.0f, (this->py + (i.y * 0.3f) - offsety) * 64.0f, (this->px + (i.x * 0.3f)+0.1 - offsetx) * 64.0f, (this->py + (i.y * 0.3f)+0.1 - offsety) * 64.0f);


		float angle = std::atan2(i.y, i.x);

		gfx->DrawRotatedDecal({ (this->px + (i.x * 0.3f) - offsetx) * 64.0f ,(this->py + (i.y * 0.3f) - offsety) * 64.0f }, VfxParticle, angle, { 0,0 }, { 1,1 }, olc::Pixel(255, 255, 255, AlphaColor));
		//gfx->DrawPartialDecal({ (px+(i.x*10)) ,(py+(i.y*20)) }, VfxParticle, { 0,0 }, { 8,8 }, { 1,1 }, olc::WHITE);   //offset pulling player back into the screen
	}
}

cDynamic_creature_Boar::cDynamic_creature_Boar() : cDynamic_creature_Enemy("ForestBoar", RPG_Assets::get().GetSprite("BoarRight"), RPG_Assets::get().GetSprite("BoarLeft"))
{

	pEquipedWeapon = (cWeapon*)RPG_Assets::get().GetItem("Bandit Sword");
	hpUpdate = new cDynamic_HpBar(px, py, RPG_Assets::get().GetSprite("HPBarFull"), RPG_Assets::get().GetSprite("HPBarEmprty"), nHealth, this);

	LvL = 3;

	CollbordersX = 0.8f;
	CollbordersXF = 1.1f;

	CollbordersY = 0.6f;
	CollbordersYF = 1.5f;

	BasicMovementSpeed = 15;
	BasicAgility = 19;
	BasicStrength = 30;
	BasicIntelect = 13;

	calculateStats();
}

void cDynamic_creature_Boar::SpecAttack(float targetX, float targetY, float Distance)
{
	isAttack = true;
	enumCounter = 4;     //  <---Attack
	framespeed = 0.12f;


	bAnimAction = true;

	
	isprojfollow = true;
	 
	
	
	
	m_nShockTime = 0.3f;
	
	calculatedDamage = (AverageAttack + pEquipedWeapon->nDamage) * 2.65f;   //set damage for projectile

	
	attackdirectionX = 0.3f;
	attackdirectionY = -0.5f;

	

	ProjOffsetX = 0.0f;
	ProjOffsetY = 0.0f;


	m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
	



	savedFx = targetX / Distance;

	if (savedFx<0)  // when we run lef side
	{
		savedFx = -1;
	
		ProjCollbordersX = 0.0f;
		ProjCollbordersXF = 1.1f;

	}
	else   // when we run right
	{
		savedFx = 1;
		ProjCollbordersX = 0.8f;
		ProjCollbordersXF = 1.9f;
	}

	//ProjSize

	ProjCollbordersY = 0.6f;
	ProjCollbordersYF = 1.5f;
	//
	frameIndicator = 0;

	bKnockBack = false;
	FxColumn = -1;
	
	vx = 0;
	vy = 0;

	m_nGraphicCounterY = 0;
	m_nGraphicCounterX = 9;

	m_nGraphicAmountFrames = 16;
	attackdirectionX = 0.3f;
	attackdirectionY = -0.5f;
	m_nShockTime = 0.1f;
	M_nFacingDirectionVertical = NORTH;
	m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
	
}

void cDynamic_creature_Boar::Behaviour(float fElapsedTime, cDynamic* player)
{

	if (bControllable == 1) // when we control in scripteditor
	{
		float fTargetX = player->px - px;
		float fTargetY = player->py - py;
		float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

	
		m_fStateTick -= fElapsedTime;

		if (m_fStateTick <= 0.0f)          //only if stateTick less than zero we gonna do anything. For optimization
		{
			if (nHealth > 0)
			{
				if (fDistance < 9.0f && player->m_layer == Friend) // before target
				{
					



					if (bOnGround)  // if stay on ground
					{
						if (!isAttack && !IsOnePlay ) // only when is attack =false and isoneplay = false
						{


							if (BRunready)
							{
								vx = savedFx * (Movementspeed / 2);

							}
							else
							{


								vx = (fTargetX / fDistance) * (Movementspeed / 4); // normalize 


								//vy = (fTargetY / fDistance) * 1.0f;


								if (player->px > this->px)
								{
									M_nFacingDirection = EAST;

								}
								else
								{
									M_nFacingDirection = WEST;


								}
								if (fTargetX < 4)  // && rand() % 25 == 5
								{

									SpecAttack(fTargetX, fTargetY, fDistance);


								}

							}
							

							
						}
					
					
					
						

					}
					else
					{



					}
				}
				else
				{



				

					Run = false;

					

					
						float vxhelp1, vxhelp2;
						//Randomizer walking bandit 
						vxhelp1 = rand() % 2;
						vxhelp2 = rand() % 2;
					vx = vxhelp2 - vxhelp1;

					//vy = 0;
					cDynamic_TextDamage* Text;
					switch (rand() % 50)
					{
					case 1:
						Text = new cDynamic_TextDamage(px + CollbordersX + 0.25f, py, "Boar!", olc::RED);
						g_engine->GiveNewText(Text);
						break;
					case 2:

						Text = new cDynamic_TextDamage(px + CollbordersX + 0.25f, py, "Hrue...Hrue..Hrue", olc::RED);
						g_engine->GiveNewText(Text);
						break;
					case 3:
						if (abs(fTargetX) < 7.0f)
						{

							Text = new cDynamic_TextDamage(px + CollbordersX + 0.25f, py, "Diii!!!", olc::RED);
							g_engine->GiveNewText(Text);

						}
						break;

					}


					
				}
			}
			else
			{

				bDead = true;
			}

			m_fStateTick += 0.5f;

		}

		if (bOnGround)
		{

			if (M_nFacingDirection == EAST)
			{

				if (!g_engine->CheckPosition(px + CollbordersXF + (vx * fElapsedTime), py + CollbordersY))
				{

					vx = 0;

					if (BRunready)
					{
						obsticleReaction();
					}

				}
			}
			else
			{

				if (!g_engine->CheckPosition(px + CollbordersX + (vx * fElapsedTime), py + CollbordersY))
				{

					vx = 0;

					if (BRunready)
					{
						obsticleReaction();
					}
				}

			}

		}

		
	}


}

void cDynamic_creature_Boar::obsticleReaction()
{
	IsOnePlay = true;
	framespeed = 0.12f;


	BRunready = false;

	frameIndicator = 0;

	bKnockBack = true;


	vy = 0.0f;
	savedFx = 0.0f;
	enumCounter = 12;

	m_nGraphicCounterX = 11;
	m_nGraphicCounterY = 2;
	m_nGraphicAmountFrames = 35;

	m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

	M_nFacingDirectionVertical = NOTLOOKING;

	g_engine->AddParticle(px + CollbordersX + 0.25f, py + CollbordersY + 0.3f);
	
}

void cDynamic_creature_Boar::DeathFun()
{
	olc::vi2d fallFrame{ 2,15};
	olc::vi2d landingframe{ 3,0 };

	std::pair<olc::vi2d, olc::vi2d> spritePos{ fallFrame, landingframe };

	if (M_nFacingDirection == EAST)
	{
		g_engine->AddParticle(new VfxDeath(this, m_pSpriteRight, spritePos, 0));
	}
	else
	{
		g_engine->AddParticle(new VfxDeath(this, m_pSpriteLeft, spritePos, 1));
	}
}

void cDynamic_creature_Boar::IndicateAnim()
{
	


	switch (M_nGraphicState)
	{
	case Idle:
		framespeed = 0.12f;
		enumCounter = 0;
		m_nGraphicCounterY = 0;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 9;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case Landing:
		enumCounter = 11;
		m_nGraphicCounterY = 4;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 4;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case Walk:
		enumCounter = 1;

		if (Run)
		{
			framespeed = 0.12f*0.5f;
			m_nGraphicCounterY = 1;
			m_nGraphicCounterX = 10;
			m_nGraphicAmountFrames = 16;
			m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;



			if (animspr >= framespeed - Haste)                 //it's speed of frames
			{
			//std::cout << frameIndicator << std::endl;
			PerformAttack();

			}
		}
		else
		{
			framespeed = 0.12f;
			m_nGraphicCounterY = 5;
			m_nGraphicCounterX = 0;
			m_nGraphicAmountFrames = 9;
			m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		}
		

	



		break;
	case Jump:
		framespeed = 0.12f;
		enumCounter = 2;
		m_nGraphicCounterY = 3;
		m_nGraphicCounterX = 4;
		m_nGraphicAmountFrames = 3;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case JumpZenit:
		framespeed = 0.12f;
		enumCounter = 10;
		m_nGraphicCounterY = 0;
		m_nGraphicCounterX = 8;
		m_nGraphicAmountFrames = 2;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;


	case AirEviscirate:  // when boar react with obsticle 
		enumCounter = 12;

		if (frameIndicator == m_nGraphicEndFrame - 1)
		{
			Run = false;
			bAnimAction = false;
		}

		break;

	case JumpDown:   // we can't walk here from setenum function

		enumCounter = 9;

		
		
		break;
	case Attack:        // <-- (Horizontal Attack)
		framespeed = 0.12f;
		enumCounter = 3;


		break;
	case AttackEasy:// <-- PREPARATION FOR RUN
		

		

		enumCounter = 4;
		
		
		

		if (frameIndicator == m_nGraphicEndFrame-1)
		{

			BRunready = true;
			Run = true;
			
			

		}


		

		break;


	case Hit:
		framespeed = 0.12f;
		enumCounter = 0;
		m_nGraphicCounterY = 3;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 4;
		Run = false;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

		break;






	}
}

bool cDynamic_creature_Boar::IsLanded()
{
	if (bOnLanded)
	{
		return true;
	}
	else
	{
		bOnLanded = true;
		if (bAnimAction)   // <--Means when we action stage don't switch animation on onladed
		{
			return false;
		}

		vx = 0;
		enumCounter = 11;
		isAttack = false;
		IsOnePlay = true;
		frameIndicator = 0;
		setEnum();
		return false;
	}
}

//void cDynamic_creature_Boar::AttackOne()
//{
//	
//
//}

//void cDynamic_creature_Boar::AttackTwo()
//{
//}

VfxDeath::VfxDeath(cDynamic* DeathObject,olc::Decal* sprite, std::pair<olc::vi2d, olc::vi2d> particlesPos,bool west) : VfxParticles(px, py)
{

	px = DeathObject->px;
	py = DeathObject->py;

	this->sprite = sprite;
	framepositions = particlesPos;

	west ? vxMult= vxMult * 1 : vxMult =vxMult * -1;

	this->vx = DeathObject->vx;
	this->vy = DeathObject->vy;


	bSolidVsMap = true;
	time = 5.0f;
	timecount = 0.0f;
	AlphaColor = 255.0f;
}

void VfxDeath::Update(float fElapsedTime, cDynamic* player)
{

	timecount += fElapsedTime;

	if (time-timecount<0.8)
	AlphaColor = 255 - (timecount * 100 * 2.55f);
	

	if (timecount >= time)
	{
		bRedundant = true;
		AlphaColor = 0;
	}

	this->vx += vxMult * fElapsedTime;
	this->vy += 32 * fElapsedTime;
	
	float fNewObjectPosX = this->px + this->vx * fElapsedTime;
	float fNewObjectPosY = this->py + this->vy * fElapsedTime;
	float fResponseX = 0;
	float fResponseY = 0;

		if (g_engine->CheckParticlePosition(fNewObjectPosX+(CollbordersXF-CollbordersX), fNewObjectPosY+CollbordersYF)) // checkPosition on solidblocks  if @true@ means that we can teleport
		{
		//	object->vy += object->mass * fElapsedTime;

			
			this->px = fNewObjectPosX;
			this->py = fNewObjectPosY;
		
			currpicturePos = framepositions.first;
		}
		else
		{
			//float fAngle = atan2f(this->vy, this->vx);

			//
		
			//	fResponseX = cosf(fAngle);
			//	fResponseY = sinf(fAngle);

			//float fMagResponse = sqrtf(fResponseX * fResponseX + fResponseY * fResponseY);

			//float dot = this->vx * (fResponseX / fMagResponse) + this->vy * (fResponseY / fMagResponse);

			//// Use friction coefficient to dampen response (approximating energy loss)
			//this->vx = 0.3*(-2.0f * dot * (fResponseX / fMagResponse) + this->vx);
			//this->vy = 0.3*(-2.0f * dot * (fResponseY / fMagResponse) + this->vy);
			//

			this->vy = 0;
			this->vx = 0;
			currpicturePos = framepositions.second;
		}
	
	
}

void VfxDeath::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{

	float nSheetSizeX = currpicturePos.y*128.0f;
	float nSheetSizeY = currpicturePos.x*128.0f;

	gfx->DrawPartialDecal({ (px - ox) * 64.0f,(py - oy) * 64.0f }, sprite, { nSheetSizeX,nSheetSizeY }, { 128,128 }, { 1,1 }, olc::Pixel(255, 255, 255, AlphaColor));   //offset pulling player back into the screen
	
}

ERaindrop::ERaindrop(float startX, float startY, float startSpeed, float startAngle) :Environment ("RainDrops")
{
	px = startX;
	py = startY;
	this->speed = startSpeed;
	this->angle = startAngle;


	psprite = RPG_Assets::get().GetSprite("RainVfx");

	
}

void ERaindrop::Update(float fElapsedTime, float screenwidth,float screenheigh)
{
	// Update the position of the raindrop based on its speed and angle
	px += speed * fElapsedTime *angle*0.01f;

	//px = screenwidth+initpx;
	//py = screenheigh + initpy;


	py += speed * fElapsedTime*0.03f;   // 0.01 - is snow

	// If raindrop goes off screen, wrap it around to the top
	if (py > screenheigh+8)
		py = screenheigh-1;
	if (py<screenheigh-1)
		py = screenheigh + 8;
	if (px > screenwidth+15)
		px = screenwidth;
	if (px < screenwidth)
		px = screenwidth + 15;

//
}
void ERaindrop::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{
	// Draw the raindrop using DrawPartialDecal

	olc::vf2d vDecal(px, py); // Destination position (where to draw the raindrop)

	//DrawRotatedDecal({ vDecal }, RPG_Assets::get().GetSprite("RainVfx"),atan2(vDecal.y-(vDecal.y+drop.speed*std::sin(drop.angle)), vDecal.x-(vDecal.x+drop.speed*std::cos(drop.angle))));
	gfx->DrawPartialDecal({(px-ox)*64, (py-oy)*64 }, psprite, { 0.0f,0.0f }, { 8.0f,8.0f }, { 1,1.5 }, olc::WHITE);// Draw raindrop image with transparency
}


Edynamic_Cloud::Edynamic_Cloud(float StartX, float startY, olc::Decal* psprite, float speed) :Environment ("Cloud")
{
	this->px = StartX;
	this->py = startY;
	this->speed = speed;
	this->psprite = psprite;
	widthsprite = psprite->sprite->width;
	heightsprite = psprite->sprite->height;
}

void Edynamic_Cloud::Update(float fElapsedTime, float screenWidth, float screenHeight)
{

	this->px += speed * fElapsedTime;

	if (this->px >screenWidth)
	{
		this->px = -widthsprite;
	}
	if (this->px < -widthsprite-1)
	{
		this->px = screenWidth + widthsprite;
	}

}
void Edynamic_Cloud::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{

	gfx->DrawPartialDecal({ (this->px-ox *10),(this->py-oy) }, psprite, { 0,0 }, { (float)widthsprite,(float)heightsprite }, { 1*0.5f,1*0.5f });   //offset pulling player back into the screen

}

cDynamic_creature_Rider::cDynamic_creature_Rider(olc::Decal* pspriteRight, olc::Decal* pspriteLeft) : cDynamic_creature_Enemy("Rider", pspriteRight, pspriteLeft)
{

	bIsAttackable = false;

	LvL = 1;
	bSolidVsMap = false;
	CollbordersX = 0.1f;
	CollbordersXF = 1.8f;

	CollbordersY = 0.6f;
	CollbordersYF = 1.5f;
}

void cDynamic_creature_Rider::IndicateAnim()
{


	switch (M_nGraphicState)
	{
	case Idle:
		enumCounter = 0;
		m_nGraphicCounterY = 0;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 7;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case Landing:
		enumCounter = 11;
		m_nGraphicCounterY = 4;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 4;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case Walk:
		enumCounter = 1;
		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 4;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

		break;
	case Jump:
		enumCounter = 2;
		m_nGraphicCounterY = 3;
		m_nGraphicCounterX = 4;
		m_nGraphicAmountFrames = 2;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case JumpZenit:
		enumCounter = 10;
		m_nGraphicCounterY = 0;
		m_nGraphicCounterX = 8;
		m_nGraphicAmountFrames = 2;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case JumpDown:
		enumCounter = 9;
		m_nGraphicCounterY = 3;
		m_nGraphicCounterX = 7;
		m_nGraphicAmountFrames = 2;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case Attack:        // <-- (Horizontal Attack)
		enumCounter = 3;


		break;
	case AttackEasy:// <-- (Throw attack)
		enumCounter = 4;



		if (frameIndicator == m_nGraphicEndFrame - 1)
		{
			vy = -10;

		}

		break;


	case Hit:
		enumCounter = 0;
		m_nGraphicCounterY = 3;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 4;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

		break;


	}
}


void cDynamic_creature_Rider::TakeComands(std::list<cComand*> comands)
{
	Rider_comands = comands;
	
}

void cDynamic_creature_Rider::Behaviour(float fElapsedTime, cDynamic* player)
{
	//if command are available, halt user control
	//bUserControlEnabled = m_listCommands.empty();
	//std::cout << px << '\t' << py << '\t' << Rider_comands.size() << std::endl;


	if (!Rider_comands.empty())
	{
		//A command is available
		if (!Rider_comands.front()->bCompleted)
		{
			//Command Has not been started
			if (!Rider_comands.front()->bStarted)
			{
				Rider_comands.front()->Start();              //record previous position in two temp 
				Rider_comands.front()->bStarted = true;      //return in high if
			}

			else //Command has been started so process it
				Rider_comands.front()->Update(fElapsedTime);   //will work before return bcompleted
		}

		else
		{
			//Command has been completed
			delete Rider_comands.front();
			Rider_comands.pop_front();
		}


	}
	else
	{

		px = 132;
		py = 24, 5;
		Rider_comands.push_back(new cComand_moveTo(this, 140.5, 24.5, 3));
		Rider_comands.push_back(new cComand_JumpTo(this, 144, 23.5, 3, 1.5));
		Rider_comands.push_back(new cComand_moveTo(this, 146, 23.5, 1));
		Rider_comands.push_back(new cComand_JumpTo(this, 149.5, 22.5, 3, 1.5));
		Rider_comands.push_back(new cComand_moveTo(this, 152.5, 22.5, 1));
		Rider_comands.push_back(new cComand_JumpTo(this, 156, 24.5, 3, 1.5));
		Rider_comands.push_back(new cComand_moveTo(this, 165, 24.5, 3));
		
	}
}

cDynamic_creature_WereWolf::cDynamic_creature_WereWolf() : cDynamic_creature_Enemy("WereWolf", RPG_Assets::get().GetSprite("WereWolfRight"), RPG_Assets::get().GetSprite("WereWolfLeft"))
{
	pEquipedWeapon = (cWeapon*)RPG_Assets::get().GetItem("Bandit Sword");
	hpUpdate = new cDynamic_HpBar(px, py, RPG_Assets::get().GetSprite("HPBarFull"), RPG_Assets::get().GetSprite("HPBarEmprty"), nHealth, this);
	 bTransformed = true;
	LvL = 3;

	CollbordersX = 0.8f;
	CollbordersXF = 1.1f;

	CollbordersY = 0.6f;
	CollbordersYF = 1.5f;

	BasicMovementSpeed = 12;
	BasicAgility = 19;
	BasicStrength = 22;
	BasicIntelect = 13;

	calculateStats();
	
	

}

void cDynamic_creature_WereWolf::SpecAttack(float targetX, float targetY, float Distance)
{
	Action();
}

void cDynamic_creature_WereWolf::IndicateAnim()
{

	switch (M_nGraphicState)
	{
	case Idle:    // Human Idle
		if (!bTransformed)
		{

		enumCounter = 0;
		m_nGraphicCounterY = 0;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 7;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
		}
		else
		{
			enumCounter = 0;
			m_nGraphicCounterY = 0;
			m_nGraphicCounterX = 8;
			m_nGraphicAmountFrames = 7;
			m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
			break;
		}
	case IdleTwo:   // WereWolf Idle



		
		break;

	case IdleReaction:   // Transform From Human to WEreWOlf
		enumCounter = 25;
		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 11;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		bTransformed = true;
		break;

	case Landing:
		enumCounter = 11;
		m_nGraphicCounterY = 2;
		m_nGraphicCounterX = 11;
		m_nGraphicAmountFrames = 2;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case Walk:
		enumCounter = 1;
		m_nGraphicCounterY = 2;
		m_nGraphicCounterX = 3;
		m_nGraphicAmountFrames = 7;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

		break;
	case Jump:
		enumCounter = 2;
		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 12;
		m_nGraphicAmountFrames = 3;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case JumpZenit:
		enumCounter = 10;
		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 15;
		m_nGraphicAmountFrames = 2;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case JumpDown:
		enumCounter = 9;
		m_nGraphicCounterY = 2;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 3;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
		break;
	case Attack:        // <-- (Horizontal Attack)
		enumCounter = 3;


		break;
	case AttackEasy:// <-- (Throw attack)
		enumCounter = 4;



	

		break;


	case Hit:
		enumCounter = 13;
		m_nGraphicCounterY = 2;
		m_nGraphicCounterX = 13;
		m_nGraphicAmountFrames = 2;
		m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

		break;


	}
}

void cDynamic_creature_WereWolf::AttackOne()
{
	isAttack = true;
	enumCounter = 3;     //  <---Attack

	frameIndicator = 0;


	ProjCollbordersX = 0.25f;
	ProjCollbordersXF = 1.75f;
	ProjCollbordersY = 0.65f;
	ProjCollbordersYF = 1.8f;

	isprojfollow = false;
	vx = 0;
	m_nGraphicCounterY = 3;
	m_nGraphicCounterX = 0;
	m_nGraphicAmountFrames = 8;
	attackdirectionX = 0.3f;
	attackdirectionY = -0.5f;
	m_nShockTime = 0.1f;
	FxColumn = 1;
	FXFrame = 5;

	M_nFacingDirectionVertical = NOTLOOKING;
	m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
}

void cDynamic_creature_WereWolf::AttackTwo()
{
	AttackOne();
}

void cDynamic_creature_WereWolf::Action()
{
	CallWolfs();
}

void cDynamic_creature_WereWolf::CallWolfs()
{
	



	if (wolfPack.size() < 2) //work once
	{
		olc::vf2d* WolfPos = new olc::vf2d{ this->px,this->py };
		float x = -1;
		for (int i = 0; i < 2; i++)
		{
			if (wolfPack.size()>=2)			
				break;
			
			WolfPos->x += x;
			cDynamic* wolf = g_engine->SpawnDireWolf(WolfPos);

			if (wolf ==nullptr)
			{
				break;
			}else
			wolfPack.push_back(wolf);
			x += 2;
		}
	}
	else {
		std::cout << wolfPack.size() << std::endl;
	

	for (auto it = wolfPack.begin(); it != wolfPack.end(); ++it) {
		cDynamic* entity = *it;


		if (this->px-entity->px > 15 && this->px-entity->px<15)
		{
			wolfPack.erase(it); // Remove the Bandit entity from the pool
			delete entity;
			break;
		}


		if (entity->binitialized == false)
		{	
		
			wolfPack.erase(it); // Remove the Bandit entity from the pool
			delete entity;
			break;
		}
	
	}

	
	}
}
	



void cDynamic_creature_WereWolf::DeathFun()
{

	olc::vi2d fallFrame{ 2,15 };    
	olc::vi2d landingframe{ 3,9 };

	std::pair<olc::vi2d, olc::vi2d> spritePos{ fallFrame, landingframe };

	if (vx <= 0)
	{
		g_engine->AddParticle(new VfxDeath(this, m_pSpriteRight, spritePos, 0));
	}
	else
	{
		g_engine->AddParticle(new VfxDeath(this, m_pSpriteLeft, spritePos, 1));
	}

}
