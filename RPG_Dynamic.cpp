#include "RPG_Dynamic.h"
#include "RPG_Engine.h"


RPG_Engine* cDynamic::g_engine = nullptr;
RPG_Engine* Environment::g_engine = nullptr;

cDynamic::cDynamic(const std::string n)
{

	sName = n;
	px = 0.0f;
	py = 0.0f;
	vx = 0.0f;
	vy = 0.0f;
	bSolidVsMap = true;
	

	
	m_layer = Neutral;

	clearFlag(bOnGround);
	clearFlag(quested);
	setFlag(bDraw);
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



cDynamic_Creature::cDynamic_Creature(const std::string name, olc::Decal* spriteRight, olc::Decal* spriteLeft) :cDynamic(name)     //here we take two sprites
{

	m_pSpriteRight = spriteRight;
	m_pSpriteLeft = spriteLeft;
	nHealth = 10;
	nHealthMax = 10;
	//M_nFacingDirection = SOUTH;

	//m_fTimer = 0.0f;

	setFlag(bIsAttackable);
	setFlag(bControllable);
	clearFlag(bOnGround);
	clearFlag(isAttack);
	clearFlag(IsOnePlay);
	setFlag(gravity);

	CenterX = (nSheetSizeX / g_engine->CellSize) / 2;
	m_nGraphicCounterX = 0;
	m_nGraphicCounterY = 0;
	FXFrame = 0;
	frameIndicator = 0;
	framespeed = 0.12f;
	m_nGraphicAmountFrames = 0;
	
	animspr = 0.0f;
	enumCounter = 0;


	setColor(255, 255, 255, 255);

}

cDynamic_ClipTrigger::cDynamic_ClipTrigger(std::string name) : cDynamic(name)
{
	
	this->setFlag(gravity);
	
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
			setFlag(bControllable);// when we control in scripteditor
			setFlag(bIsAttackable);
			clearFlag(bAnimAction);
			
			
		//	player->bSolidvsDyn = true;     //return  unavalibale to go through ppl
			m_fKnockBackTimer = 0;
			ReturnBaseLayer();
			//mass = 48;


			setColor(255, 255, 255, 255);
		//	greenColor = 255;   //<--return colors
			//blueColor = 255;
			//redColor = 255;

		}
	}
	else
	{
		if (!checkFlag(isAttack) && !checkFlag(isDirectionLock))
		{
		if (vx < 0) M_nFacingDirection = WEST;
		if (vx > 0) M_nFacingDirection = EAST;
		}

		if (nHealth <= 0)                                //Death char Tracker
		{
			
			clearFlag(bDraw);
			setFlag(bDead);
			g_engine->SetCurrentExp(calculateDeathExp()); // Get exp from dead enemy
			g_engine->SpawnItem(olc::vf2d{ this->px,this->py }, RPG_Assets::get().GetItem("Coin"));
			if (!checkFlag(quested))
			{
			this->DeathFun(); // droping fx object
			}

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


		if (g_engine->getScriptActive())
		{
		Behaviour(fElapsedTime, player);
		}
		else
		{
			vx = 0;
		}

		if (gravityofftimer>0)
		{
		GravityControl(fElapsedTime);
		}

	}
	if(hpUpdate !=nullptr)
	hpUpdate->Update(fElapsedTime, this);


}

void cDynamic_Creature::Behaviour(float fElapsedTime, cDynamic* player)
{
}

void cDynamic_Creature::setGreenColor(uint8_t Green)
{
	color = (color & 0xFFFF00FF) | (Green << 8);
}

void cDynamic_Creature::setBlueColor(uint8_t Blue)
{
	color = (color & 0xFF00FFFF) | (Blue << 16);
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





	// Initialize variables  we subtract first wave of addition
	float sup_intelligence = getBasicInt() - 1.5f;  // Basic stats it's constants 
	float sup_agility = getBasicAgil() - 2.5f;
	float averHelp =BasicAveAtck -  2.5f;
	float sup_strength = getBasicStrength() - 1.7f;
	float defsup = 2.25f - 0.18f;
	float MoveSup = getBasicMovement();
	//float HasteSup = 0;
	for (int i = 1; i <= LvL; i++)
	{
		// Calculate Intelligence
		sup_intelligence += 1.5f;
		if (i == LvL)
		{
			SetLvlInt(sup_intelligence);
		//	std::cout << "IntL: " << LvlIntelligence << std::endl;
		}

		// Calculate Agility
		sup_agility += 2.5f;
	//	HasteSup = ((sup_agility/100)-0.165f)/10;

		averHelp += 2.5f;

		MoveSup += 0.1f;
		if (i == LvL)
		{
		//	LvlHaste = HasteSup;
			std::cout << "Haste: " << LvlHaste << std::endl;
			SetLvlMovement(MoveSup);
		//	LvlMovementspeed = MoveSup;
		//	std::cout << "Movementspd: " << LvlMovementspeed << std::endl;
			SetLvlAgility(sup_agility);
		//	LvlAgility = sup_agility;
		//	std::cout << "agility: " << LvlAgility << std::endl;
			LvlAverageAttack = averHelp;
			std::cout << "AverAttack: " << LvlAverageAttack << std::endl;
		}

		// Calculate Strength
		sup_strength += 1.7f;
		defsup += 0.18f;
		if (i == LvL)
		{
			SetLvlStrength(sup_strength);
			//LvlStrength = sup_strength;
			//std::cout << "strength: " << LvlStrength << std::endl;
			lvlHealthMax = gettLvlStrength() * 25;
			nHealth = lvlHealthMax;
			hpUpdate->setLevelHp(lvlHealthMax);
			LvlDefence = defsup;
		}
	}
	nHealthMax = nHealth;
}

void cDynamic_Creature::SetAlphaColor(uint8_t Alpha)
{
	color = (color & 0x00FFFFFF) | (Alpha >> 24);  // reading mask need form right to left
}

void cDynamic_Creature::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{
	float nSheetOffsetY, nSheetOffsetX;
	const int spriteWidth = m_pSpriteRight->sprite->width / nSheetSizeX;

	                //0.12f   - 0.05f (20 lvl)
	if (animspr >= framespeed-EndHaste)                 //it's speed of frames
	{



		frameIndicator++;

			

		if (checkFlag(IsOnePlay) || checkFlag(isAttack) || checkFlag(bOnLanded) )                 // if we put left mouse key
		{
			//x stroke is getting growth

			if (frameIndicator == FXFrame)   //   <---- summon projectile when attack
			{
				PerformAttack();
				//clearFlag(isAttack);
			}

			if (frameIndicator > m_nGraphicCounterX + m_nGraphicAmountFrames)
			{
				clearFlag(IsOnePlay);
				clearFlag(isAttack);
				clearFlag(bOnLanded);
				clearFlag(IsThrow);
				clearFlag(isprojfollow);
				sparedVx =0.0f;
			//	setEnum();
			}
			
		}
		
		animspr =0;

	}
	if (frameIndicator > m_nGraphicCounterX + m_nGraphicAmountFrames || frameIndicator <= m_nGraphicCounterX)   // <==
	{

		frameIndicator = m_nGraphicCounterX;
		setEnum();
	}

	int NextRow = frameIndicator / spriteWidth;
	                   //m_nGraphicCounterY offset for column     @frameIndicator /(int(m_pSpriteRight->sprite->width / nSheetSizeX@ lock in sprite width
	nSheetOffsetX = (frameIndicator % spriteWidth)*nSheetSizeX;
	nSheetOffsetY = (m_nGraphicCounterY+NextRow) *nSheetSizeX;


	//std::cout << nSheetOffsetY/ nSheetSizeY <<'\t'<< nSheetOffsetX / nSheetSizeY << std::endl;

	/// //////

	if (M_nFacingDirection == EAST)
	{

		gfx->DrawPartialDecal({ (px - ox) * g_engine->CellSize,(py - oy) * g_engine->CellSize }, m_pSpriteRight, { nSheetOffsetX,nSheetOffsetY }, { nSheetSizeX,nSheetSizeY }, {1*entityScale,1 * entityScale },olc::Pixel(getRed(), getGreen(), getBlue()));   //offset pulling player back into the screen
	}
	else
	{
		gfx->DrawPartialDecal({ (px - ox) * g_engine->CellSize,(py - oy) * g_engine->CellSize }, m_pSpriteLeft, { nSheetOffsetX,nSheetOffsetY }, { nSheetSizeX,nSheetSizeY }, { 1 * entityScale,1 * entityScale }, olc::Pixel(getRed(), getGreen(), getBlue()));   //offset pulling player back into the screen

	}


	if (hpUpdate !=nullptr )
	{

	hpUpdate->DrawSelf(gfx, ox, oy);

	}
	
	
	if (checkFlag(bOnTarget))
	{
	//	gfx->DrawPartialDecal({ (px - ox) * 64.0f+38,(py - oy) * 64.0f+18 }, m_pSpriteRight, { nSheetOffsetX,nSheetOffsetY }, { nSheetSizeX,nSheetSizeY }, { 1,1 }, olc::Pixel(redColor, greenColor, blueColor));   //offset pulling player back into the screen
		
		g_engine->DrawBigText(std::to_string(LvL) + " " + sName, (px - ox + 0.36f) * (g_engine->fscale * 64.0f) + 18, (py - oy) * (g_engine->fscale * 64.0f), 0.5, 0.5, olc::YELLOW);


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
	clearFlag(gravity);
	gravityofftimer -= felapsedtime;

		if (gravityofftimer <=0 || m_fKnockBackTimer>0.0f)
		{
			setFlag(gravity);
		}

	
}

void cDynamic_Creature::SetUpDamage ()   //standart damage for all chars
{
	

	float halfdmg = GetWeaponAttributes(pEquipedWeapon->Attributes) / 2; // Пример значения для halfdmg

	// Генерация случайных чисел
	uint16_t rand1 = rand() % static_cast<int>(halfdmg);
	uint16_t rand2 = rand() % static_cast<int>(halfdmg);

	int16_t rand3 = rand1 - rand2;

	switch (M_nGraphicState)
	{
	case cDynamic_Creature::Idle:
		break;
	case cDynamic_Creature::Walk:
		break;
	case cDynamic_Creature::Jump:
	
		break;
	case cDynamic_Creature::Attack:
		calculatedDamage = EndAverageAttack+ rand3 * 2.65f;
		break;
	case cDynamic_Creature::AttackEasy:
		calculatedDamage = EndAverageAttack + rand3 + 20 * 5;
		break;
	case cDynamic_Creature::AttackGreat:
		calculatedDamage = EndAverageAttack+ rand3 + 20 * 3;
		break;
	case cDynamic_Creature::DoubleJump:
		break;
	case cDynamic_Creature::AirAttack:
		calculatedDamage = EndAverageAttack+ rand3 * 2.65f;
		break;
	case cDynamic_Creature::EviscirateDown:
		calculatedDamage = EndAverageAttack+ rand3 + 20 * 3.6f;
		
		
		break;
	case cDynamic_Creature::JumpDown:
		break;
	case cDynamic_Creature::JumpZenit:
		break;
	case cDynamic_Creature::Landing:
		break;
	case cDynamic_Creature::AirEviscirate:
		calculatedDamage = EndAverageAttack+ rand3 + 20 * 3.6f;
		break;
	case cDynamic_Creature::Hit:
		
		break;
	case cDynamic_Creature::AttackBack:
		calculatedDamage = EndAverageAttack+ rand3 * 4;
		break;
	case cDynamic_Creature::AirDownEviscirate:
		calculatedDamage = (EndAverageAttack+ rand3 + 20) * 3.6f;
		break;
	case cDynamic_Creature::LANDINGEviscirate:
		calculatedDamage = EndAverageAttack+ rand3 * 3.6f;
		break;


	default:
		break;
	}

}

bool cDynamic_Creature::IsLanded()
{

	if (checkFlag(bOnLanded))
	{
		return true;
	}
	else
	{
		setFlag(bOnLanded);
		if (checkFlag(bAnimAction))   // <--Means when we action stage don't switch animation on onladed
		{
			return false;
		}
		enumCounter = 11;
		clearFlag(isAttack);
		clearFlag(IsOnePlay);
		frameIndicator = 0;
		setEnum();
		return false;
	}
	
}

bool cDynamic_Creature::setEnum()
{
	
	if (checkFlag(IsOnePlay) || checkFlag(isAttack) || checkFlag(bOnLanded) )
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
			M_nGraphicState = EviscirateDown;
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

		case 26:
			M_nGraphicState = Eviscirate;
			IndicateAnim();
			return true;
			break;

		}


	}

	

	if (checkFlag(bOnGround) && !checkFlag(bOnLanded))
	{

		if (vx > 0.5f ||vx<-0.5f )
		{

			M_nGraphicState = Walk;
			IndicateAnim();
			return true;
		}
		
	}

	/// ///////////////////

	if (!checkFlag(bOnGround) &&!checkFlag(bOnLanded))
	{

		
		if (vy > 0.1f  )
		{
			M_nGraphicState = JumpDown;
			clearFlag(bOnLanded);
			IndicateAnim();
			return true;
		}
		if (vy<2.8f && vy>-2.8f)
		{
			M_nGraphicState = JumpZenit;
			clearFlag(bOnLanded);
			IndicateAnim();
			return true;
		}

		if (vy < -0.1f)
		{
			M_nGraphicState = Jump;
			clearFlag(bOnLanded);
			IndicateAnim();
			return true;
		}
	/// ///////////////////
	}
	if (vx <= 0.5f && vx >= -0.5f && checkFlag(bOnGround) && !checkFlag(bAnimAction) )

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
		setFlag(isDirectionLock);
	}


	clearFlag(bControllable); // when we control in scripteditor char
	clearFlag(bIsAttackable);
	clearFlag(isprojfollow);    // keep projectile follow at char 
	
	clearFlag(isAttack);
	clearFlag(Btarget);
	setBlueColor(0);
	setGreenColor(0);

	//blueColor = 0;
	//greenColor = 0;
	setFlag(IsOnePlay);
	enumCounter = 13;
	//frameIndicator = 13;
	setEnum();
	}




}

void cDynamic_Creature::setColor(const uint8_t Red, const uint8_t Green, const uint8_t Blue, const uint8_t Alpha)
{
	color |= (Red << 0);
	color |= (Green << 8);
	color |= (Blue << 16);
	color |= (Alpha << 24);
}

const uint8_t cDynamic_Creature::getRed()
{

	
	return color & 0xFF;
}

const uint8_t cDynamic_Creature::getGreen()
{
	return (color >> 8) & 0xFF;
}

const uint8_t cDynamic_Creature::getBlue()
{
	return (color >> 16) & 0xFF;
}

void cDynamic_Creature::setRedColor(uint8_t Red)
{
	color = (color & 0xFFFFFF00) | Red;

}

const uint8_t cDynamic_Creature::getAlpha()
{
	return (color >> 24) & 0xFF;
}


cDynamic_creature_DireWolf::cDynamic_creature_DireWolf() : cDynamic_creature_Enemy("DireWolf", RPG_Assets::get().GetSprite("DireWolfRight"), RPG_Assets::get().GetSprite("DireWolfLeft"))
{

	pEquipedWeapon = (cWeapon*)RPG_Assets::get().GetItem("Bandit Sword");

	LvL = 1;

	CollbordersX = 0.75f;
	CollbordersXF = CollbordersX + 0.5f;

	CollbordersY = 0.6f;
	CollbordersYF = 1.5f;

	SetEndMovement(20);
	//EndMovementspeed = 20;
	setBasicAgility(19);
	setBasicInt(13);
	setBasicStrength(5);
	//BasicAgility = 19;
	//BasicStrength = 9;
	//BasicIntelect = 13;
	BasicAveAtck = 30;

	hpUpdate = new cDynamic_HpBar(px, py, nHealth, this);

	calculateStats();


	EndAverageAttack = LvlAverageAttack;
	SetEndStrength(gettLvlStrength());
	SetEndInt(getLvlInt());
	SetEndAgility(getLvlAgil());
//	EndStrength = gettLvlStrength();
//	EndIntelligence = getLvlInt();
//	EndAgility = getLvlAgil();
}



cDynamic_creature_NPC::cDynamic_creature_NPC(std::string sname, uint8_t version) : cDynamic_Creature("Npc", RPG_Assets::get().GetSprite("NPC"), RPG_Assets::get().GetSprite("NPC"))
{

	if (version <0)
	{
		clearFlag(bDraw);
	}
	
	m_nGraphicCounterY = static_cast<int>(version);
	//Bandit constructor
	m_layer = Neutral;

	sName = sname;

	nHealth = 1;
	nHealthMax = 1;
	clearFlag(bDead);
	//m_fStateTick = 0.2f;
	clearFlag(bOnGround);
	clearFlag(bIsAttackable);
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

	if (checkFlag(bOnLanded))
	{
		return true;
	}
	else
	{
		setFlag(bOnLanded);
		if (checkFlag(bAnimAction))   // <--Means when we action stage don't switch animation on onladed
		{
			return false;
		}
		m_fStateTick = 0.0f;
		vx = 0;
		enumCounter = 11;
		clearFlag(isAttack);
		setFlag(IsOnePlay);
		frameIndicator = 0;
		setEnum();
		return false;
	}

}



void cDynamic_creature_NPC::IndicateAnim()
{
	switch (M_nGraphicState)
	{
	case Idle:
		enumCounter = 0;
	//	m_nGraphicCounterY = 0;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 5;
		//std::cout << frameIndicator << std::endl;
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
	
		break;
	case Landing:
		enumCounter = 11;
		m_nGraphicCounterY = 4;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 4;
		
		break;
	case Walk:
		enumCounter = 1;
		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 4;
		

		break;
	case Jump:
		enumCounter = 2;
		m_nGraphicCounterY = 3;
		m_nGraphicCounterX = 4;
		m_nGraphicAmountFrames = 3;
		
		break;
	case JumpZenit:
		enumCounter = 10;
		m_nGraphicCounterY = 0;
		m_nGraphicCounterX = 8;
		m_nGraphicAmountFrames = 2;
	
		break;
	case JumpDown:
		enumCounter = 9;
		m_nGraphicCounterY = 3;
		m_nGraphicCounterX = 7;
		m_nGraphicAmountFrames = 3;
		
		break;
	case Attack:        // <-- (Horizontal Attack)
		enumCounter = 3;
	

		break;
	case AttackEasy:// <-- (Throw attack)
		enumCounter = 4;
		
	
		
		if (frameIndicator == m_nGraphicCounterX + m_nGraphicAmountFrames -1)
		{
			vy =- 10;

		}

		break;
	
	
	case Hit:
		enumCounter = 13;
		m_nGraphicCounterY = 3;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 4;
		
		
		break;


	}
}



void cDynamic_creature_DireWolf::SpecAttack(float targetX, float targetY, float Distance)
{

	setFlag(isprojfollow);
	setFlag(isAttack);
	enumCounter = 4;     //  <---Attack Throw Dagger
	vx = (targetX / Distance) * 5.0f;
	setFlag(isDirectionLock);
	sparedVx = -vx;
	sparedVy = 1.0f;
	//vy = -10.0f;
	frameIndicator = 0;
	//setEnum();

	m_nGraphicCounterY = 4;
	m_nGraphicCounterX = 3;
	m_nGraphicAmountFrames = 6;
	

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
	FXFrame = m_nGraphicCounterX + m_nGraphicAmountFrames - 1;

	M_nFacingDirectionVertical = NORTH;
}



void cDynamic_creature_DireWolf::AttackOne()
{
	setFlag(isAttack);
	enumCounter = 3;     //  <---Attack

	frameIndicator = 0;

	
	ProjCollbordersX = 0.25f;
	ProjCollbordersXF = 1.75f;
	ProjCollbordersY = 0.65f;
	ProjCollbordersYF = 1.8f;

	setFlag(isprojfollow);
	vx = 0;
	m_nGraphicCounterY = 2;
	m_nGraphicCounterX = 0;
	m_nGraphicAmountFrames = 10;
	attackdirectionX = 0.3f;
	attackdirectionY = -0.5f;
	m_nShockTime = 0.1f;
	FxColumn = 1;
	FXFrame = 3;

	//switch (GetFacingDirection())
	//{
	//case 1:       // <--East
	//	ProjOffsetX = -0.95f;
	//	ProjOffsetY = 0;
	//	break;
	//case 3:       // <--Weast
	//	ProjOffsetX = 0.85f;
	//	ProjOffsetY = 0;
	//	break;

	//}


	M_nFacingDirectionVertical = NOTLOOKING;
	

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

void cDynamic_creature_DireWolf::ReturnToPool()
{
	{
		cDynamic_Creature::ReturnToPool();
		this->sName = "DireWolf";
		this->nHealth = this->nHealthMax;
		g_engine->PoolsController(g_engine->getDireWolfsPool(), this);
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
		
		break;
	case Landing:
		enumCounter = 11;
		m_nGraphicCounterY = 2;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 6;
		

		

		break;
	case Walk:
		enumCounter = 1;
		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 9;
		

		break;
	case Jump:
		enumCounter = 2;
		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 10;
		m_nGraphicAmountFrames = 2;
		
		break;
	case JumpZenit:
		enumCounter = 10;
		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 10;
		m_nGraphicAmountFrames = 2;
		
		break;
	case JumpDown:
		enumCounter = 9;
		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 10;
		m_nGraphicAmountFrames = 2;
		
		break;
	case Attack:        // <-- (Horizontal Attack)
		enumCounter = 3;
		

		break;
	case AttackEasy:// <-- (Throw attack)
		enumCounter = 4;
		
		break;
	case AttackGreat:  




		break;
	case EviscirateDown:
		
		break;
	case AirEviscirate:  //(Vertical Attack - becouse fx suit
		enumCounter = 12;
		
		break;

	case Hit:
		enumCounter = 13;
		m_nGraphicCounterY =0;
		m_nGraphicCounterX = 8;
		m_nGraphicAmountFrames = 4;
		

		
		break;
		

	}

}

void cDynamic_creature_Bandit::SpecAttack(float targetX, float targetY, float Distance)
{
	
	setFlag(isAttack);
	setFlag(IsThrow);
	clearFlag(isprojfollow);

	enumCounter = 4;     //  <---Attack Throw Dagger
	vx = (-targetX / Distance) * 5.0f;
	setFlag(isDirectionLock);


	sparedVx = (M_nFacingDirection ==WEST) ? -5 : 5;
	sparedVy = 1.0f;

	//sparedVx = (vx <= 0) ? 5 : -5;

	vy = -10.0f;
	frameIndicator = 0;
	
	
	
	ProjCollbordersX = -0.25f;
	ProjCollbordersXF = 0.25f;
	ProjCollbordersY = 0.0f;
	ProjCollbordersYF = 0.25f;

	m_nGraphicCounterY = 3;
	m_nGraphicCounterX = 0;
	m_nGraphicAmountFrames = 6;
	attackdirectionX = 0.2f;
	attackdirectionY = -0.5f;
	m_nShockTime = 0.1f;
	

	FxColumn = 0;
	FXFrame = 4;
	
	
	M_nFacingDirectionVertical = NOTLOOKING;

}

cDynamic_creature_Bandit::cDynamic_creature_Bandit() : cDynamic_creature_Enemy("Bandit", RPG_Assets::get().GetSprite("BanditRight"), RPG_Assets::get().GetSprite("BanditLeft"))
{
	//collision borders
	CollbordersX = 0.8f;
	CollbordersXF = 1.1f;
	CollbordersY = 0.6f;
	CollbordersYF = 1.5f;
	//
	pEquipedWeapon = (cWeapon*)RPG_Assets::get().GetItem("Bandit Sword");
	hpUpdate = new cDynamic_HpBar(px, py, nHealth, this);
	LvL = 2;
	setBasicAgility(19);
	setBasicInt(13);
	setBasicStrength(9);

	BasicAveAtck = 25;
	calculateStats();

	EndAverageAttack = LvlAverageAttack;
	
	SetEndMovement(17);
	SetEndStrength(gettLvlStrength());
	SetEndInt(getLvlInt());
	SetEndAgility(getLvlAgil());

}



void cDynamic_creature_Bandit::AttackOne()
{
	enumCounter = 3;     //  <---Attack

	frameIndicator = 0;

	




	ProjCollbordersX = 0.25f;
	ProjCollbordersXF = 1.75f;
	ProjCollbordersY = 0.65f;
	ProjCollbordersYF = 1.8f;


	setFlag(isAttack);
	setFlag(isprojfollow);
	vx = 0;
	m_nGraphicCounterY = 5;
	m_nGraphicCounterX = 0;
	m_nGraphicAmountFrames = 9;
	attackdirectionX = 0.3f;
	attackdirectionY = -0.5f;
	m_nShockTime = 0.1f;   

	FXFrame = 5;
	FxColumn = 1;
	M_nFacingDirectionVertical = NOTLOOKING;
	


}

void cDynamic_creature_Bandit::AttackTwo()
{
	setFlag(isAttack);
	enumCounter = 12;     //  <---Attack Vertical

	frameIndicator = 0;


	
	ProjCollbordersX = 0.25f;
	ProjCollbordersXF = 1.75f;
	ProjCollbordersY = 0.65f;
	ProjCollbordersYF = 1.8f;

	setFlag(isprojfollow);
	vx = 0;
	m_nGraphicCounterY = 4;
	m_nGraphicCounterX = 0;
	m_nGraphicAmountFrames = 9;
	attackdirectionX = 0.3f;
	attackdirectionY = -0.5f;
	m_nShockTime = 0.1f;
	FXFrame = 5;  // <--Second column
	FxColumn = 2;
	M_nFacingDirectionVertical = NOTLOOKING;
	


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

void cDynamic_creature_Bandit::ReturnToPool()
{
	{
		cDynamic_Creature::ReturnToPool();
		this->sName = "Bandit";
		this->nHealth = this->nHealthMax;
		g_engine->PoolsController(g_engine->getBanditsPool(), this);
	};
}

cDynamic_creature_Pantir::cDynamic_creature_Pantir() : cDynamic_Creature("Pantir", RPG_Assets::get().GetSprite("PantirRight"), RPG_Assets::get().GetSprite("PantirLeft"))
{
	LvL = 1;
	m_pSpriteGrabLeft = RPG_Assets::get().GetSprite("PantirGrabLeft");
	m_pSpriteGrabRight = RPG_Assets::get().GetSprite("PantirGrabRight");
	M_BaseClass = AGILITY;
	M_nGraphicState = Idle;                                            //Pantir constructor
	//m_fStateTick = 2.0f;
	Jumpcounter = 0;
	pEquipedWeapon = (cWeapon*)RPG_Assets::get().GetItem("Pantir's Dagger");
	
	pEquipedBoots = (cEquip*)RPG_Assets::get().GetItem("Basic Boots");
	pEquipedHelmet = (cEquip*)RPG_Assets::get().GetItem("Basic Helment");
	pEquipedBack = (cEquip*)RPG_Assets::get().GetItem("Basic Back");
	pEquipedChest = (cEquip*)RPG_Assets::get().GetItem("Basic Chest");
	pEquipedNeck = (cEquip*)RPG_Assets::get().GetItem("Basic Neck");
	pEmptySlot = (cEquip*)RPG_Assets::get().GetItem("Empty");


	//pEquipedBoots = pEmptySlot;
	//pEquipedHelmet = pEmptySlot;
	//pEquipedBack = pEmptySlot;
	//pEquipedChest = pEmptySlot;
	//pEquipedNeck = pEmptySlot;
	//pEquipedWeapon = pEmptySlot;
	//

	clearFlag(bDead);
	//Borders for collision
	CollbordersX = 0.75f;
	CollbordersXF =CollbordersX+0.5f;

	CollbordersY = 0.5f ;
	CollbordersYF = 1.5f ;
	

	

	m_layer = Friend;
	
	
	fAttackcount = 0;

	 BasicAveAtck = 19.5;

	 setBasicMovement(12);
	 setBasicAgility(19);
	 setBasicStrength(15);
	 setBasicInt(13);
	// BasicMovementSpeed = 12;
    // BasicAgility = 19;
	// BasicStrength = 15;
	// BasicIntelect = 13;
	

	 uint8_t movement = getBasicMovement();

	 std::cout << movement << std::endl;


	 hpUpdate = new cDynamic_HpBar(this->px, this->py, nHealth, this);


	  TakeAttributes(pEquipedBack->Attributes);
	  TakeAttributes(pEquipedBoots->Attributes);
	  TakeAttributes(pEquipedHelmet->Attributes);
	  TakeAttributes(pEquipedChest->Attributes);
	  TakeAttributes(pEquipedNeck->Attributes);
	  TakeAttributes(pEquipedWeapon->Attributes);

	 calculateStats();


	 MergeItemLvl();
	

	/* for (int i = 0; i <5; i++)
	 {
	 g_engine->AttachEq(g_engine->Getequip(), i);
	 }*/

	

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
		
		break;
	case Landing:
		enumCounter = 11;
		m_nGraphicCounterY = 3;
		m_nGraphicCounterX = 10;
		m_nGraphicAmountFrames = 3;
		FXFrame = 0;
	
		break;
	case Walk:
		enumCounter = 1;
		m_nGraphicCounterY = 0;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 9;
		FXFrame = 0;
		
		//RPG_Assets::get().playSound("Run");
		break;
	case Jump:
		enumCounter = 2;
		m_nGraphicCounterY = 2;
		m_nGraphicCounterX = 9;
		m_nGraphicAmountFrames = 2;
		FXFrame = 0;
		
		break;
	case JumpDown:
		enumCounter = 9;
		m_nGraphicCounterY = 2;
		m_nGraphicCounterX = 12;
		m_nGraphicAmountFrames = 2;
		FXFrame = 0;
		
		break;
	case DoubleJump:
		enumCounter = 6;
		m_nGraphicCounterY = 3;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 7;
		FXFrame = 0;
		

		break;
	case Attack:
		enumCounter = 3;

		if (frameIndicator  == FXFrame && checkFlag(isAttack))   //   <---- summon projectile when attack
		{

			clearFlag(isAttack);
		}
		
		break;

	case AttackGreat:
		enumCounter = 5;
	
		if (frameIndicator == FXFrame && checkFlag(isAttack))   //   <---- summon projectile when attack
		{
			
			//clearFlag(isAttack);
		}
		
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
		
		break;

	case AirAttack:
		enumCounter = 7;
		
		break;
	case EviscirateDown:
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
		

	
		break;

	case UpEviscirate:
		enumCounter = 17;
		m_nGraphicCounterY = 11;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 6;  //  <---Means how long our projectile should work
		attackdirectionX = 0.1f;
		attackdirectionY = -1.1f;
		m_nShockTime = 0.15f;
		setFlag(IsThrow);
		


		FXFrame = 1;
		

		break;
	case AirDownEviscirate:
		
		enumCounter = 18;

		switch (frameIndicator)
		{
		case 1:
			FXFrame = 2;
			
			setFlag(isprojfollow);
			break;
		case 4:
			if (!checkFlag(bOnLanded))
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
			clearFlag(bAnimAction);
			setFlag(bIsAttackable);
		}

		break;


	case BLINKDESAPEAR:

		enumCounter =20;
		FXFrame = 0;
		clearFlag(isprojfollow);    // keep projectile follow at char 
		clearFlag(IsThrow);
		m_nGraphicCounterY =4;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 6;  //  <---Means how long our projectile should work
	
		
		
	
	
		if (frameIndicator == m_nGraphicAmountFrames-1)
		{
			setFlag(bDead);
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

		

		attackdirectionX = 0.1f;
		attackdirectionY = -0.3f;
		Hittimes = 4;  // <--How many times projectile should hit

		

		SetVerticalDirection(2);
		

		if (checkFlag(bOnLanded))
		{
			//ProjSize
			ProjCollbordersX = 0.0f;
			ProjCollbordersXF = 4.0f;
			ProjCollbordersY = 1.3f;
			ProjCollbordersYF = 2.7f;
			//


			
			clearFlag(bAnimAction);
			setFlag(bIsAttackable);

			setFlag(bDraw);
			FxColumn = 8;
			PerformAttack();
			this->vy -=8;
			Jumpcounter++;
			//if (Target !=nullptr)
		//	Target = nullptr;
			clearFlag(bOnLanded);
			setFlag(bControllable);
			enumCounter = 2;

			break;
		}
		else
		{

			if (frameIndicator == m_nGraphicCounterX + m_nGraphicAmountFrames -1)
			{
				frameIndicator = 0;
			}
		}
		


		break;



	case Death:
		if (checkFlag(bOnLanded))
		{
			m_nGraphicCounterY = 5;
			m_nGraphicCounterX = 13;
			m_nGraphicAmountFrames = 1;  //  <---Means how long our projectile should work
			FXFrame = 0;

			
		}
		else
		{
			m_nGraphicCounterY = 4;
			m_nGraphicCounterX = 13;
			m_nGraphicAmountFrames = 1;  //  <---Means how long our projectile should work
			FXFrame = 0;

			
		}

		break;
	
	


	case Eviscirate:
		enumCounter = 26;

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
	return getEndAgil();
	if (num == 2)
	return getEndStrength();
	if (num == 3)
	return getEndInt();
	if (num == 4)
	return EndDefence;
	if (num == 5)
	return getEndMovement();
	if (num == 6)
	return EndAverageAttack;
	if (num == 7)
	return fHpRegeneration;
	if (num == 8)
	


	return 0;
}


void cDynamic_creature_Pantir::SwirlGrab( cDynamic* dyn)
{

	Target = dyn;

	clearFlag(isprojfollow);    // keep projectile follow at char 
	clearFlag(IsThrow);
	setFlag(bAnimAction);  // <--blocking other animations while it's working
	clearFlag(bIsAttackable);
	
	clearFlag(bControllable);

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

	clearFlag(bDraw);

	enumCounter = 21;
		
}

void cDynamic_creature_Pantir::Behaviour(float fElapsedTime, cDynamic* player)
{
//	std::cout << "//////////" << std::endl;
//	std::cout << checkFlag(BisProjectile) <<"\t BisProjectile " << std::endl;
//	std::cout << checkFlag(bDraw) << "\tbDraw "<<  std::endl;
//	std::cout << checkFlag(bIsAttackable) << "\tbIsAttackable " << std::endl;
//	std::cout << checkFlag(bRedundant) << "\tbRedundant " << std::endl;
//	std::cout << checkFlag(bDead) << "\tbDead " << std::endl;
//	std::cout << checkFlag(bOnGround) << "\tbOnGround " << std::endl;
//	std::cout << checkFlag(Btarget) << "\tBtarget " << std::endl;
//	std::cout << checkFlag(binitialized) << "\tbinitialized " << std::endl;
//	std::cout << checkFlag(bControllable) << "\tbControllable " << std::endl;
//	std::cout << checkFlag(quested) << "\tquested " << std::endl;
//	std::cout << checkFlag(isAttack) << "\tisAttack " << std::endl;
//	std::cout << checkFlag(IsThrow) << "\tIsThrow " << std::endl;
//	std::cout << checkFlag(IsOnePlay) << "\tIsOnePlay " << std::endl;
//	std::cout << checkFlag(bOnLanded) << "\tbOnLanded " << std::endl;
//	std::cout << checkFlag(bAnimAction) << "\tbAnimAction " << std::endl;
//	std::cout << checkFlag(isprojfollow) << "\tisprojfollow " << std::endl;
//	std::cout << checkFlag(Btarget) << "\tBtarget " << std::endl;
	//std::cout << checkFlag(gravity) << "\tgravity " << std::endl;
	//std::cout << checkFlag(isDirectionLock) << "\tisDirectionLock " << std::endl;
	//std::cout << "//////////" << std::endl;
	


	if (checkFlag(bOnGround))
	{

	
		vx += -10.0f * vx * fElapsedTime;
		if (fabs(vx) < 0.01f)
		{
			vx = 0.0f;
		
			
				g_engine->WalkSound->stop();

			
		}




	}
	else
	{
		vx += -7.0f * vx * fElapsedTime;
		if (fabs(vx) < 0.01f)
		{
			vx = 0.0f;
			
			g_engine->WalkSound->stop();

		}
	}
	

	if (checkFlag(IsOnePlay) || !checkFlag(bOnGround))
	{
		g_engine->WalkSound->stop();
	}
	
	if (vx !=0)
	{
		energyCount += fElapsedTime * (25.0);

		if (checkFlag(bOnGround))
		{
			
			

			if (g_engine->WalkSound->getStatus() == sf::Sound::Playing)
			{

			}
			else
			{
		
			g_engine->WalkSound->play();

			//g_engine->PlaySounds(g_engine->WalkSound->);
		//	g_engine->sounds.push_back(g_engine->WalkSound);
		//	g_engine->currsound.setLoop(true);

			}
			
		}

	}
	else
	{
	energyCount += fElapsedTime * 35.0f;
	rageCount += fElapsedTime * 2.5f;

	if (Combocounter >0)    // logic for combos
	{
	fAttackcount += fElapsedTime;
		
	if (fAttackcount >1.0f)
	{
		fAttackcount = 0.0f;
		Combocounter = 0;
	} // 
	}

	}
	if (energyCount >=1)
	{
		energyAmount += (int)energyCount;
		energyCount = 0.0f;		
	}


	if (rageCount >= 1)
	{
		rageAmount -= (int)rageCount;
		rageCount = 0.0f;
	}

	if (energyAmount >= MaxEnergy )
		energyAmount = MaxEnergy;
		
	
	if (energyAmount <= 0 )
		energyAmount = 0;	
	
	
	if ( rageAmount >= MaxRage)
		rageAmount = MaxRage;

	if ( rageAmount <= 0)
		rageAmount = 0;

	


	EnergyHide(fElapsedTime);



	if (targetTime>0)
	{
	targetTime -= fElapsedTime;
	}

	if (Target != nullptr&& Target->checkFlag(Target->bDead))
	{
		targetTime = 0;
		Target = nullptr;

	}
	
}



cDynamic_creature_BanditArcher::cDynamic_creature_BanditArcher() : cDynamic_creature_Bandit()
{
	 sName = "BanditArcher";
	 fSpecAtckdist = 5.0f;
	 fAttackDist = 5.0f;
	// vxBorder = 5.0f;

	  BeveOffsetpX = 5;   // offset for behave px
	  BeveOffsetpY = 0; // py
	  SetEndMovement(5);
	  EndAverageAttack = EndAverageAttack / 2;
	
}


void cDynamic_creature_BanditArcher::AttackOne()
{
	//setFlag(isAttack);
	//setFlag(IsThrow);
	//clearFlag(isprojfollow);


	//setFlag(isDirectionLock);


	//sparedVx = (M_nFacingDirection == WEST) ? -10 : 10;
	//sparedVy = -2.0f;

	//enumCounter = 3;     //  <---Attack

	//frameIndicator = 0;


	//ProjCollbordersX = 0.75f;
	//ProjCollbordersXF = 1.25f;
	//ProjCollbordersY = 0.95f;
	//ProjCollbordersYF = 1.25f;


	//setFlag(isAttack);

	//vx = 0;
	//m_nGraphicCounterY = 5;
	//m_nGraphicCounterX = 0;
	//m_nGraphicAmountFrames = 9;
	//attackdirectionX = 0.3f;
	//attackdirectionY = -0.5f;
	//m_nShockTime = 0.1f;

	//FxColumn = 0;
	//FXFrame = 4;
	//M_nFacingDirectionVertical = NOTLOOKING;
	//m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;
}

void cDynamic_creature_BanditArcher::AttackTwo()
{
//	setFlag(isAttack);
//	setFlag(IsThrow);
//	clearFlag(isprojfollow);
//
//
//	setFlag(isDirectionLock);
//
//
//	sparedVx = (M_nFacingDirection == WEST) ? -2 : 2;
//	sparedVy = -5.0f;
////	sparedVy = 1.0f;
//
//	enumCounter = 12;     //  <---Attack Vertical
//
//	frameIndicator = 0;
//
//
//
//
//	ProjCollbordersX = 0.75f;
//	ProjCollbordersXF = 1.25f;
//	ProjCollbordersY = 0.95f;
//	ProjCollbordersYF = 1.25f;
//
//
//	vx = 0;
//	m_nGraphicCounterY = 4;
//	m_nGraphicCounterX = 0;
//	m_nGraphicAmountFrames = 9;
//	attackdirectionX = 0.3f;
//	attackdirectionY = -0.5f;
//	m_nShockTime = 0.1f;
//	FxColumn = 0;
//	FXFrame = 4;
//	M_nFacingDirectionVertical = NOTLOOKING;
//	m_nGraphicEndFrame = m_nGraphicCounterX + m_nGraphicAmountFrames;

}

void cDynamic_creature_BanditArcher::Behaviour(float fElapsedTime, cDynamic* player)
{
	if (checkFlag(bControllable)) // when we control in scripteditor
	{
		float Bevpx, Bevpy;
		float bvhelper = -1;

		float fTargetX = player->px - px;  // offset for archer 
		float fTargetY = player->py - py;

		float ftargetoffset;

		//	std::cout << fTargetX << std::endl;

		if (fTargetX > 0)  // means that mob in right  position
		{
			bvhelper = -1;
			Bevpx = CollbordersXF + BeveOffsetpX * bvhelper;


		}
		else
		{

			Bevpx = BeveOffsetpX - CollbordersXF;
			//Bevpy = BeveOffsetpY;

		}
		//	std::cout << Bevpx  << std::endl;
		ftargetoffset = fTargetX + Bevpx;


		
		float fDistanceOffset = sqrtf(ftargetoffset * ftargetoffset + fTargetY * fTargetY);
		float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);

		m_fStateTick -= fElapsedTime;

		if (m_fStateTick <= 0.0f)          //only if stateTick less than zero we gonna do anything. For optimization
		{
			if (nHealth > 0)
			{
				if (fDistanceOffset < 9.0f) // before target
				{


					vx = 0;





					if (checkFlag(bOnGround))  // if stay on ground
					{

						if (player->m_layer == Friend)
						{



							if (!checkFlag(isAttack) && !checkFlag(IsOnePlay))   // if no attack b noplay (hit)
							{


								if (fDistanceOffset > 0.3f)
								{
									vx = (ftargetoffset / fDistanceOffset) * (getEndMovement() / 4);
								}


								//vy = (fTargetY / fDistance) * 1.0f;

								if (vx > 0)
								{
									M_nFacingDirection = EAST;

								}
								else
								{
									M_nFacingDirection = WEST;


								}

								if (fDistanceOffset < fSpecAtckdist && rand() % 5 == 4)  // && rand() % 25 == 5
								{

									SpecAttack(fTargetX, fTargetY, fDistance);


								}
								else if (fDistanceOffset < fAttackDist)
								{
									if (rand() % 10 >= 5 && !checkFlag(isAttack))
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
					vxhelp1 = rand() % 2;
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
					cDynamic_TextDamage* T;
					olc::vf2d textdamagecoord;
					switch (rand() % 50)
					{
					case 1:

						textdamagecoord = { (float)px + CollbordersX + 0.25f, (float)py };
						T = (cDynamic_TextDamage*)g_engine->SpawnBattleText(textdamagecoord, "We shut all them.", olc::RED);


						break;
					case 2:
						textdamagecoord = { (float)px + CollbordersX + 0.25f, (float)py };
						T = (cDynamic_TextDamage*)g_engine->SpawnBattleText(textdamagecoord, "Boss Protect Us...", olc::RED);

						break;
					case 3:
						if (abs(fTargetX) < 7.0f)
						{


							textdamagecoord = { (float)px + CollbordersX + 0.25f, (float)py };
							T = (cDynamic_TextDamage*)g_engine->SpawnBattleText(textdamagecoord, "Fuck you!!!", olc::RED);

						}
						break;

					}


				}
			}
			else
			{

				setFlag(bDead);
			}

			m_fStateTick += 0.35f;

		}

		if (checkFlag(bOnGround))
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

void cDynamic_creature_BanditArcher::ReturnToPool()
{
	{
		cDynamic_Creature::ReturnToPool();
		this->sName = "BanditArcher";
		this->nHealth = this->nHealthMax;
		g_engine->PoolsController(g_engine->getBanditsArcherPool(), this);
	}
}


void cDynamic_creature_BanditArcher::SpecAttack(float targetX, float targetY, float Distance)
{

	setFlag(isAttack);
	setFlag(IsThrow);
	clearFlag(isprojfollow);

	enumCounter = 12;     //  <---Attack Throw Dagger
	vx = 0;
	//setFlag(isDirectionLock);

	float time =1.5f;
	float vxtest = targetX/time ;

	sparedVx = vxtest;
					//дистанция по y +половину от гравитации, в нашем случае 5, домножая на время 5 в квадрате
	float vytest = (targetY + 0.5f * 5 * (time * time)) / time;
	
	sparedVy =-vytest;

	if (targetX > 0)
	{
		M_nFacingDirection = EAST;

	}
	else
	{
		M_nFacingDirection = WEST;


	}

	
	frameIndicator = 0;




	ProjCollbordersX = -0.25f;
	ProjCollbordersXF = 0.25f;
	ProjCollbordersY = 0.0f;
	ProjCollbordersYF = 0.25f;

	m_nGraphicCounterY = 3;
	m_nGraphicCounterX = 0;
	m_nGraphicAmountFrames = 6;
	attackdirectionX = 0.2f;
	attackdirectionY = -0.5f;
	m_nShockTime = 0.1f;


	FxColumn = 0;
	FXFrame = 4;
	M_nFacingDirectionVertical = NORTH;  // Need for  correctly spawn prjectile
	
}

void cDynamic_creature_Pantir::PerformAttack()
{
	if (pEquipedWeapon == nullptr)
		return;

	

	float halfdmg = GetWeaponAttributes(pEquipedWeapon->Attributes) / 2; // Пример значения для halfdmg

	// Генерация случайных чисел
	uint16_t rand1 = rand() % static_cast<int>(halfdmg+halfdmg);
	uint16_t rand2 = rand() % static_cast<int>(halfdmg+halfdmg);

	int16_t rand3 = rand1 - rand2;

	

			switch (M_nGraphicState)
			{
			case cDynamic_Creature::Idle:
				break;
			case cDynamic_Creature::Walk:
				break;
			case cDynamic_Creature::Jump:
				
				break;
			case cDynamic_Creature::Attack:

				this->energyAmount -= 55;
				calculatedDamage = (EndAverageAttack + rand3 - 20) * 5;
				break;
			case cDynamic_Creature::AttackEasy:
				this->energyAmount -=35;
				calculatedDamage = (EndAverageAttack + rand3 - 20) * 5;
				break;
			case cDynamic_Creature::AttackGreat:
				this->energyAmount = 0;
				calculatedDamage = (EndAverageAttack + rand3 - 20) * 5;
				break;
			case cDynamic_Creature::DoubleJump:
				this->energyAmount -= 20;
				break;
			case cDynamic_Creature::AirAttack:
				this->energyAmount -= 35;
				calculatedDamage = (EndAverageAttack + rand3) * 2.65f;
				break;
			case cDynamic_Creature::EviscirateDown:
				this->energyAmount += 15;
				calculatedDamage = (EndAverageAttack + rand3 - 20 ) * 3.6f;
				this->rageAmount -= 35;
				break;
			case cDynamic_Creature::JumpDown:
				break;
			case cDynamic_Creature::JumpZenit:
				break;
			case cDynamic_Creature::Landing:
				break;
			case cDynamic_Creature::AirEviscirate:
				this->energyAmount += 15;
				calculatedDamage = (EndAverageAttack + rand3 - 20) * 3.6f;
				this->rageAmount -= 35;

				break;
			case cDynamic_Creature::Hit:
				this->energyAmount -= 20;
				break;
			case cDynamic_Creature::AttackBack:
				this->energyAmount -= 40;
				calculatedDamage = (EndAverageAttack + rand3) * 4;
				break;
			case cDynamic_Creature::UpEviscirate:
				this->energyAmount += 15;
				calculatedDamage = (EndAverageAttack + rand3 - 20 ) * 3.0f;
				this->rageAmount -= 35;
				break;
			case cDynamic_Creature::AirDownEviscirate:
				this->energyAmount += 15;
				calculatedDamage = (EndAverageAttack + rand3 -20 ) * 3.6f;
				this->rageAmount -= 35;
				break;
			case cDynamic_Creature::LANDINGEviscirate:
				this->energyAmount += 15;
				calculatedDamage = (EndAverageAttack + rand3 - 20 ) * 3.6f;
				this->rageAmount -= 35;

			case cDynamic_Creature::Swirl:
				this->energyAmount -= 40;
				calculatedDamage = (EndAverageAttack + rand3 - 20 ) * 3.6f;
				this->rageAmount -= 35;


				break;
			case cDynamic_Creature::Eviscirate:
				this->energyAmount += 15;
				calculatedDamage = (EndAverageAttack + rand3) * 2.0f;
				this->rageAmount -= 35;
				break;
			}

			pEquipedWeapon->OnUse(this);
}


void cDynamic_Item::OnInteract(cDynamic* player)
{
	if (bCollected)
		return;
	if(g_engine->GetFreespaceInventory() == 0)
		return;
	
		//m_listItems.size() + 1

	if (item->OnInteract(player) )
	{

		
		//Add item to invenotory
		if (g_engine->GiveItem(item->sName,1)) //< -- if return true means that full	
			return;
		

	}
	
	setFlag(bDead);
	bCollected = true;
}
void cDynamic_Item::ReturnToPool()
{
	{
		setFlag(bControllable);
		clearFlag(binitialized);
		setFlag(bDraw);
		clearFlag(bDead);
		clearFlag(quested);
		this->sName = "pickup";
		this->item = nullptr;
		g_engine->PoolsController(g_engine->getItemPool(), this);
	};
}
cDynamic_Item::cDynamic_Item() : cDynamic("pickup")     //Constructror
{
	px = 0;
	py = 0;
	m_layer = Neutral;
	CollbordersX = 0.0f;
	CollbordersXF = 0.5f;
	CollbordersY = 0.0f;
	CollbordersYF = 0.5f;
	setFlag(gravity);
}
void cDynamic_Item::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{
	if (bCollected)
		return;

	float sprX = item->spriteindex % 32;
	float sprY = item->spriteindex / 32;
	gfx->DrawPartialDecal({ (px - ox) * g_engine->CellSize,(py - oy) * g_engine->CellSize }, item->pSprite, {sprX*64,sprY*64}, {64,64 },{(float)0.5*g_engine->fscale,(float)0.5 * g_engine->fscale });   //offset pulling player back into the screen
	//gfx->DrawPartialSprite((px - ox) * 16.0f, (py - oy) * 16.0f, item->pSprite, 0, 0, 6, 16);



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
	

	if (checkFlag(IsThrow))  // when projectile on air we calculate angle 
	{
	
	

		gfx->DrawPartialRotatedDecal({ (px - ox) * (64.0f*g_engine->fscale),(py - oy) * (64.0f*g_engine->fscale) }, pSpriteRight, angle, { nSheetSize / 2 ,nSheetSize / 2 }, { (float)Frameindicator * nSheetSize,(float)m_GraphicState * nSheetSize }, { nSheetSize,nSheetSize });

		//gfx->DrawPartialRotatedDecal({ (px - ox) * 64.0f,(py - oy) * 64.0f }, pSpriteLeft, angle, { 64 / 2,18 / 2 }, { (float)Frameindicator * nSheetSize,(float)m_GraphicState * nSheetSize }, { g_engine->fscale,g_engine->fscale });


	}
	else
	{
		if (m_FacingDirection == 3)

		{

			gfx->DrawPartialDecal({ (px - ox) * (64.0f*g_engine->fscale),(py - oy) * (64.0f * g_engine->fscale) }, pSpriteRight, { (float)Frameindicator * nSheetSize,(float)m_GraphicState * nSheetSize }, { nSheetSize,nSheetSize });   //offset pulling player back into the screen
		}
		else
		{
			gfx->DrawPartialDecal({ (px - ox) * (64.0f * g_engine->fscale),(py - oy) * (64.0f * g_engine->fscale) }, pSpriteLeft, { (float)Frameindicator * nSheetSize,(float)m_GraphicState * nSheetSize }, { nSheetSize,nSheetSize });   //offset pulling player back into the screen

		}
	}


//std::cout  << angle << std::endl;
//gfx->DrawRotatedDecal({ (px - ox) * 64.0f,(py - oy) * 64.0f }, pSpriteLeft, 0.0f, {0,0}); // <--Arrow

}

void cDynamic_Projectile::Update(float fElapsedTime, cDynamic* player)
{

		animspr += fElapsedTime;
		fDuration -= fElapsedTime;


	if (fDuration <= 0.0f)

		setFlag(bRedundant);


	if (checkFlag(IsThrow))  // when projectile in air we add to him time for life 
	{

		fDuration = 0.3f;
		float  preangle = atan2(vy, vx);   // получаем угол в радианах от -1 до 1 ( не корректно)
		if (preangle < 0)
		{
			preangle += 2 * 3.14;          //преобразуем их в радины от 0 до 3.14*2
		}
		preangle = floor(preangle * 100) / 100;



		if (!checkFlag(bOnGround))   // when we o the ground we stop calculating angle and frame rate
		{
			angle = preangle;
			Frameindicator = Frameindicator % 6;


		}
		else
		{
			setFlag(bRedundant);
		}

	}


		if (repitable)
		{
			fDuration = 0.3f;
		}

		if (checkFlag(bOnGround))  
		{
			vx =0;
		}
		else      // when  bOngraund = false 
		{
			

		}

		
		
		if (agrsr !=nullptr)
		{

			
			SetFollowProjPos();
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

cDynamic_Projectile::cDynamic_Projectile() : cDynamic("projectile")
{
	setFlag(BisProjectile);
	
}


void cDynamic_Projectile::SetDeafult()
{
	pSpriteRight = nullptr;
	pSpriteLeft = nullptr;
	clearFlag(binitialized);
	clearFlag(isprojEqualX);
	clearFlag(bRedundant);
	clearFlag(gravity);
	clearFlag(IsThrow); // help switch draw method in this class
	clearFlag(isprojfollow);
	clearFlag(isReflected);
	this->m_GraphicState = 0;
	SetTimer(0);
	knockbacktime = 0;
	m_layer = Friend;
	nDamage = 0;
	gettarget = 0;
	px = 0;
	py = 0;
	vx = 0;
	vy = 0;
	CollbordersX = 0;
	CollbordersXF = 0;
	CollbordersY = 0;
	CollbordersYF = 0;
	attackDirectionX = 0;
	attackDirectionY = 0;
	m_FacingDirection = 0;
	m_VerticalFacingDirection = 0;
	HitAmount = 0;  //   < -- we add how many times our projectile should take damage before leave  default 1
	HitCount = 0;
	fDuration = 0;
	animspr = 0;
	Frameindicator = 0;
}

void cDynamic_Projectile::SetSprites(olc::Decal* IpSpriteRight, olc::Decal* IpSpriteLeft)
{
	pSpriteRight = IpSpriteRight;
	pSpriteLeft = IpSpriteLeft;

	nSheetSize = pSpriteLeft->sprite->width / 6;
}

void cDynamic_Projectile::ReturnToPool()
{
	{
		SetDeafult();
		g_engine->PoolsController(g_engine->getProjectilePool(), this);
	}
}

void cDynamic_Projectile::SetFollowProjPos()
{

	if (checkFlag(isprojfollow))
	{


		py = agrsr->py;
		if (checkFlag(isprojEqualX)==false)
		{
			if (m_FacingDirection == 3)  // west

			{

				px = agrsr->px + agrsr->CollbordersXF - CollbordersX;

			}
			else
			{
				px = agrsr->px - 2.0f + agrsr->CollbordersX + (2.0f - CollbordersXF);

			}

		}
		else
		{
			px = agrsr->px;

		}

	}
}

void cDynamic_Projectile::SetAgressorThowData(cDynamic_Creature* aggresor, uint8_t Time)
{
	setFlag(gravity);
	setFlag(IsThrow); // help switch draw method in this class
	vx = aggresor->sparedVx;
	vy = aggresor->sparedVy;
	angle = atan2(vy, vx);
	bSolidVsMap = false;
	agrsr = nullptr;
	nDamage = aggresor->GetAttack();

	mass = 5;
	HitAmount = aggresor->Hittimes;  //   < -- we add how many times our projectile should take damage before leave  default 1
	gettarget = aggresor->checkFlag(aggresor->Btarget);
	m_VerticalFacingDirection = aggresor->GetFacingDirectionVertical();
	m_FacingDirection = aggresor->GetFacingDirection();
	attackDirectionY = aggresor->attackdirectionY;

	CollbordersX = aggresor->ProjCollbordersX;
	CollbordersXF = aggresor->ProjCollbordersXF;
	CollbordersY = aggresor->ProjCollbordersY;
	CollbordersYF = aggresor->ProjCollbordersYF;
	
	//p->CollbordersY = 1;
	bOneHit = true;

	this->m_GraphicState = setEnum(aggresor->FxColumn);
	fDuration = Time;
//	HitAmount = 1;
	knockbacktime = aggresor->m_nShockTime;
	m_layer = aggresor->m_layer;
	rageamount = aggresor->rageset;
	Frameindicator = 0;

	if (Time == 0.0f)      //Setup timer auto, when not 0
	{
		SetTimer(aggresor->FxColumn);
	}


}

void cDynamic_Projectile::SetAgressorData(cDynamic_Creature* Aggresor)
{

	
	agrsr = Aggresor;
	this->m_GraphicState = setEnum(Aggresor->FxColumn);
	SetTimer(Aggresor->FxColumn);
	knockbacktime = Aggresor->m_nShockTime;
	m_layer = Aggresor->m_layer;
	nDamage = Aggresor->GetAttack();
	gettarget = Aggresor->checkFlag(Aggresor->Btarget);
	CollbordersX = Aggresor->ProjCollbordersX;
	CollbordersXF = Aggresor->ProjCollbordersXF;
	CollbordersY = Aggresor->ProjCollbordersY;
	CollbordersYF = Aggresor->ProjCollbordersYF;
	bOneHit = true;
	bSolidVsMap = false;
	attackDirectionY = Aggresor->attackdirectionY;
	m_FacingDirection = Aggresor->GetFacingDirection();
	m_VerticalFacingDirection = Aggresor->GetFacingDirectionVertical();
	HitAmount = Aggresor->Hittimes;  //   < -- we add how many times our projectile should take damage before leave  default 1
	
	setFlag(isprojfollow);
	if (Aggresor->checkFlag(Aggresor->isprojEqualX))
	{
		this->setFlag(this->isprojEqualX);
	
	}


	SetFollowProjPos();

}

void cDynamic_HpBar::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{
	float hpdiff;;
	if (charHp<0)
	{
		hpdiff = 0;
		charHp = 0;
	}
	else
	{
		hpdiff = charHp / procent;
		charHp = Hpowner->nHealth;
	}

	

	if (Hpowner->checkFlag(bDead))
	{
		setFlag(bRedundant);  // пометка, что объект мёртв и память нужно освободить
	}

	// Координаты отрисовки с учетом смещения и масштаба
	float drawPosX = (px - ox) * (64.0f * g_engine->fscale) + (28 * g_engine->fscale);
	float drawPosY = (py - oy) * (64.0f * g_engine->fscale) + (18 * g_engine->fscale);

	// Отрисовка общей рамки с использованием декали
	gfx->DrawPartialDecal({ drawPosX, drawPosY }, g_engine->D_FullUi,
		{ (float)soursceposX, (float)soursceposY + soursceSizeY },
		{ (float)soursceSizeX, (float)soursceSizeY },
		{ g_engine->fscale, g_engine->fscale });

	// Отрисовка полоски здоровья (с учётом оставшегося HP)
	gfx->DrawPartialDecal({ drawPosX, drawPosY }, g_engine->D_FullUi,
		{ (float)soursceposX, (float)soursceposY },
		{ hpdiff, (float)soursceSizeY },
		{ g_engine->fscale, g_engine->fscale });

	// Формирование строки с текущим и максимальным здоровьем
	std::string sTextCurHp = std::to_string(charHp);
	std::string sTextFullHp = std::to_string(hpMax);

	// Отрисовка текста здоровья
	g_engine->DrawBigText(sTextCurHp + "/" + sTextFullHp,
		(px - ox + 0.36f) * (64.0f * g_engine->fscale) + (28 * g_engine->fscale),
		(py - oy) * (64.0f * g_engine->fscale) + (16 * g_engine->fscale),
		0.25f * g_engine->fscale, 0.25f * g_engine->fscale, olc::WHITE);
}

cDynamic_HpBar::cDynamic_HpBar(float ox, float oy,  int hp, cDynamic_Creature* hpown) : cDynamic("HpFirst")
{
	hpMax = hp;
	//HpFull = HpFulls;
	//HpEmpty = HpEmptys;
	px = ox;
	py = oy;
	Hpowner = hpown;
	soursceposX = 960;
	soursceposY = 242;
	soursceSizeX = 72;
	soursceSizeY = 9;

	g_engine->AddIndicators(this);
}

void cDynamic_HpBar::Update(float fElapsedTime, cDynamic* player)
{
	px = player->px;
	py = player->py;
}

void cDynamic_HpBar::setLevelHp(int par)
{
	hpMax = par;
	procent = (float)hpMax / soursceSizeX;
}

cDynamic_Teleport::cDynamic_Teleport(float x, float y, std::string mapName, float tx, float ty) : cDynamic("Teleport")
{
	px = x;
	py = y;
	fMapPosX = tx;
	fMapPosY = ty;
	sMapName = mapName;
	
	bSolidVsMap = false;
	clearFlag(gravity);

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
	clearFlag(gravity);


	


	

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
		setFlag(bRedundant);
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



		gfx->DrawPartialDecal({ (float)((this->px - ox) * g_engine->CellSize) + (i * (18* g_engine->fscale)), (float)((this->py - oy) * g_engine->CellSize) + (z * 32) }, RPG_Assets::get().GetSprite("font"), {sx, sy}, {32, 32}, {1 * g_engine->fscale,1*g_engine->fscale}, Color);
		i++;
	}


//	gfx->DrawPartialDecal({ (px - ox) * 64.0f + 50,(py - oy) * 64.0f + 29 }, RageEmpty, { 0,0 }, { 155,32 });   //offset pulling player back into the screen

}

cDynamic_TextDamage::cDynamic_TextDamage() :cDynamic("Text")
{
	clearFlag(gravity);
	m_layer = Neutral;
}

void cDynamic_TextDamage::SetDeafult()
{
	this->px = 0;
	this->py = 0;
	this->DamageText.clear();
	Color = olc::WHITE;
	clearFlag(bRedundant);
	time =0;

}

void cDynamic_TextDamage::ReturnToPool()
{
	{
		SetDeafult();
		g_engine->PoolsController(g_engine->getTextPool(), this);

	};
}

void cDynamic_creature_Pantir::MergeItemLvl()
{


	SetEndAgility(getItemAgil() + getLvlAgil());
	SetEndInt(getItemInt() + getLvlInt());
	SetEndStrength(getItemStr() + gettLvlStrength());

	
	//EndAgility = getItemAgil() + getLvlAgil();
//	EndStrength = getItemStr() + gettLvlStrength();
	//EndIntelligence = getItemInt() + getLvlInt();

	StatsImpact();
}

void cDynamic_creature_Pantir::MoveJump()
{
	vy = -9.0f;
	clearFlag(bAnimAction);    // <--Now jump can deflect animation downjumpevis
	clearFlag(IsOnePlay);
	clearFlag(isAttack);
	clearFlag(bOnLanded);
	clearFlag(isprojfollow);
	clearFlag(Btarget);
	Jumpcounter++;
	frameIndicator = 0;     // if we change it on 3 - it will be a comboo whith eviscirate but isAttack must be true
	

	g_engine->PlaySounds("Jump");
	

	//setEnum();
}

void cDynamic_creature_Pantir::MoveDoubleJump()
{
	setFlag(IsOnePlay);
	clearFlag(isAttack);
	clearFlag(bOnLanded);
	clearFlag(isprojfollow);
	clearFlag(Btarget);
	enumCounter = 6;
	vy = -9.0f;
	Jumpcounter++;
	
	//setEnum();
}


void cDynamic_creature_Pantir::EnergyMoveAttackAir()
{
	if (!bHideMode )
	{

		
		setFlag(isAttack);
		clearFlag(IsOnePlay);
		setFlag(isprojfollow);    // keep projectile follow at char 
		clearFlag(Btarget);
	enumCounter = 7;  // <-- Air attack
	frameIndicator = 0;
	sparedVx = vx;

	Hittimes = 1;

	m_nGraphicCounterY = 6;
	m_nGraphicCounterX = 5;
	m_nGraphicAmountFrames = 6;
	attackdirectionX = 0.2f;
	attackdirectionY = -0.2f;
	m_nShockTime = 0.1f;

	rageset = 10;

	FXFrame = 6;
	FxColumn = 2;
	


	//ProjSize
	ProjCollbordersX = 0.25f;
	ProjCollbordersXF = 1.75f;
	ProjCollbordersY = 0.65f;
	ProjCollbordersYF = 1.5f;
	
	
}
 }

void cDynamic_creature_Pantir::comboManager()
{
	switch (Combocounter)
	{
	case 0:
		EnergyMoveAttackLow();
		Combocounter = 1;
		fAttackcount = 0.0f;
		break;
	case 1:
	//	if (energyAmount > 55)
	//	{

		EnergyMoveAttackMid();
		fAttackcount = 0.0f;
		Combocounter = 2;
	//	}
	//	else
	//	{
	//		EnergyMoveAttackLow();
	//		Combocounter = 1;
	//		fAttackcount = 0.0f;
	//	}
		break;
	case 2:
		EnergyMoveAttackHigh();
		fAttackcount = 0.0f;
		Combocounter = 0;
		break;
	}

}

void cDynamic_creature_Pantir::EnergyMoveAttackLow()
{
	if (! bHideMode)
	{
		
		
		SetVerticalDirection(3); //for escape coflicts with held botton up or down 
		sparedVx = vx;
		sparedVy = 1.0f;
		setFlag(isAttack);
		setFlag(IsOnePlay);
		setFlag(isprojfollow);    // keep projectile follow at char 
		clearFlag(Btarget);
		enumCounter = 4; // <--Easy sinister strike
		frameIndicator = 0;
		

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

		//switch (GetFacingDirection())
		//{
		//case 1:       // <--East
		//	ProjOffsetX = -0.95f;
		//	ProjOffsetY = 0;
		//	break;
		//case 3:       // <--Weast
		//	ProjOffsetX = 0.85f;
		//	ProjOffsetY = 0;
		//	break;

		//}
		



		g_engine->PlaySounds("SwordSwingTwo");

		
	}
}
void cDynamic_Creature::SetHealth(int setHealth)
{
	if (setHealth>=nHealthMax)
	{
		nHealth = nHealthMax;
	}
	else {

	nHealth = setHealth;
	}
	

}
void cDynamic_creature_Pantir::SetEnergy(int setHealth)
{
	if (setHealth >= MaxEnergy)
	{
		energyAmount= MaxEnergy;
	}
	else {

		energyAmount = setHealth;
	}

	
}
void cDynamic_creature_Pantir::SetRage(int setRage)
{
	if (setRage >= MaxRage)
	{
		
		rageAmount = MaxRage;
	}
	else {

		rageAmount = setRage;
	}


}
void cDynamic_creature_Pantir::HideStage()
{
	if (energyAmount>50 && bHideMode ==false)
	{
	moveBonus+= 10;
	m_layer = Neutral;
    energyAmount -= 75;
	bHideMode = true;
	clearFlag(IsOnePlay);
	clearFlag(isAttack);
	clearFlag(Btarget);

	setBlueColor(0);
	setGreenColor(0);
	setRedColor(0);
	//blueColor = 0;
//	greenColor = 0;
	//redColor = 0;
	}

}
void cDynamic_creature_Pantir::EnergyMoveAttackMid()
{
	if (!bHideMode)
	{
		SetVerticalDirection(3); //for escape coflicts with held botton up or down 
		setFlag(isAttack);
		setFlag(IsOnePlay);
		setFlag(isprojfollow);    // keep projectile follow at char 
		clearFlag(Btarget);
		enumCounter = 3; // <-- Mid sinister strike
		frameIndicator = 0;
		fAttackcount = 0.5f;
		sparedVx = vx;
		sparedVy = 1.0f;
		Hittimes = 1;

		//ProjSize
		ProjCollbordersX = 0.25f;
		ProjCollbordersXF = 1.75f;
		ProjCollbordersY = 0.65f;
		ProjCollbordersYF = 1.5f;
		//

		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 4;
		attackdirectionX = 0.2f;
		attackdirectionY = -0.3f;
		m_nShockTime = 0.1f;



		rageset = 15;

		FXFrame = 1;
		FxColumn = 1;

		


		//switch (GetFacingDirection())
		//{
		//case 1:       // <--East
		//	ProjOffsetX = -0.95f;
		//	ProjOffsetY = 0;
		//	break;
		//case 3:       // <--Weast
		//	ProjOffsetX = 0.85f;
		//	ProjOffsetY = 0;
		//	break;

		//}
		g_engine->PlaySounds("SwordSwingTwo");
	}
}

void cDynamic_creature_Pantir::EnergyMoveAttackHigh()
{
	if (!bHideMode)
	{
		SetVerticalDirection(3); //for escape coflicts with held botton up or down 
		setFlag(isAttack);
		setFlag(IsOnePlay);
		setFlag(isprojfollow);    // keep projectile follow at char 
		clearFlag(Btarget);  // <--- Set projectile take target enemy for blink or other
		enumCounter = 5; // <-- High sinister strike
		frameIndicator = 0;
		fAttackcount = 0.5f;
		sparedVx = vx;
		sparedVy = 1.0f;
		Hittimes =2;

		//ProjSize
		ProjCollbordersX = 0.25f;
		ProjCollbordersXF = 1.75f;
		ProjCollbordersY = 0.65f;
		ProjCollbordersYF = 1.5f;
		//

		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 5;
		m_nGraphicAmountFrames = 4;
		attackdirectionX = 0.2f;
		attackdirectionY = -0.3f;
		m_nShockTime = 0.1f;

		FXFrame = 7;
		FxColumn = 0;
		rageset = 15;

		


		//switch (GetFacingDirection())
		//{
		//case 1:       // <--East
		//	ProjOffsetX = -0.95f;
		//	ProjOffsetY = 0;
		//	break;
		//case 3:       // <--Weast
		//	ProjOffsetX = 0.85f;
		//	ProjOffsetY = 0;
		//	break;

		//}
		g_engine->PlaySounds("SwordSwingTwo");
	}
}

void cDynamic_creature_Pantir::RageMoveAttck()
{
	if (!bHideMode)
	{
		SetVerticalDirection(3); //for escape coflicts with held botton up or down 
		FXFrame =12;
		sparedVy = 1.0f;
		setFlag(isAttack);
		clearFlag(IsOnePlay);
		setFlag(isprojfollow);    // keep projectile follow at char 

		enumCounter = 26;  //  eviscirate jump down
	
		frameIndicator = 0;

	//	vy = -0.5f;
		if (GetFacingDirection() == 3)
			vx = 5.0f;		
		else
		vx = -5.0f;
		
		rageset = 0;
		Hittimes = 2;

		//ProjSize
		ProjCollbordersX = 0.2f;
		ProjCollbordersXF = 1.9f;
		ProjCollbordersY = 0.75f;
		ProjCollbordersYF = 1.8f;
		//

		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 9;
		m_nGraphicAmountFrames = 7;  //  <---Means how long our projectile should work
		attackdirectionX = 0.1f;
		attackdirectionY = 1.1f;
		m_nShockTime = 0.15f;


		//ProjOffsetX = 0.0f;
	 //    // <--Weast
		//ProjOffsetY = 0.25f;


		FxColumn =4;




	}
}

void cDynamic_creature_Pantir::RageMoveAttackUp()
{
	if (!bHideMode)
	{
		SetVerticalDirection(3); //for escape coflicts with held botton up or down 
		sparedVx = vx;
		sparedVy = 1.0f;
		setFlag(isAttack);
		clearFlag(IsOnePlay);
		setFlag(isprojfollow);    // keep projectile follow at char 
		enumCounter = 8;     //Eviscirate throw up
		frameIndicator = 0;

		rageset = 0;
		Hittimes = 2;
		//
		if ((g_engine->GetLearnedTalent(5)))	
		setFlag(Btarget);// <--- Set projectile take target enemy for blink or other


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
		


		//switch (GetFacingDirection())
		//{
		//case 1:       // <--East
		//	ProjOffsetX = -0.95f;
		//	ProjOffsetY = 0;
		//	break;
		//case 3:       // <--Weast
		//	ProjOffsetX = 0.85f;
		//	ProjOffsetY = 0;
		//	break;

		//}
	}


}

void cDynamic_creature_Pantir::RageMoveAttackAir()
{
	if (!bHideMode)
	{
		SetVerticalDirection(3); //for escape coflicts with held botton up or down 
		sparedVx = vx;
		sparedVy = 1.0f;
		setFlag(isAttack);
		clearFlag(IsOnePlay);
		clearFlag(Btarget); // <--- Set projectile take target enemy for blink or other
		enumCounter = 12;  // air eviscirate
		setFlag(isprojfollow);    // keep projectile follow at char 
		//setFlag(IsThrow);
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


		

	}
}

void cDynamic_creature_Pantir::EnergyMoveAttackBack()
{
	if (!bHideMode)
	{
		SetVerticalDirection(3); //for escape coflicts with held botton up or down 
		setFlag(isAttack);
		clearFlag(IsOnePlay);
		clearFlag(Btarget); // <--- Set projectile take target enemy for blink or other
		enumCounter = 16; // <-- BackStab
		frameIndicator = 0;
		sparedVx = vx;
		sparedVy = 1.0f;
		setFlag(isprojfollow);    // keep projectile follow at char 

		Hittimes = 1;


		FxColumn = 5;

		//ProjSize
		ProjCollbordersX = 0.25f;
		ProjCollbordersXF = 1.75f;
		ProjCollbordersY = 0.65f;
		ProjCollbordersYF = 1.8f;
		//



		//switch (GetFacingDirection())
		//{
		//case 1:       // <--East
		//	ProjOffsetX = -0.95f;
		//	ProjOffsetY = 0;
		//	break;
		//case 3:       // <--Weast
		//	ProjOffsetX = 0.85f;
		//	ProjOffsetY = 0;
		//	break;

		//}
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

			uint8_t Red = getRed();
			uint8_t Green = getGreen();
			uint8_t blue = getBlue();

			Red = (0.5f - Hidetimer) * 510;
			Green = (0.5f - Hidetimer) * 510;
			blue = (0.5f - Hidetimer) * 510;

			setGreenColor(Green);
			setRedColor(Red);
			setBlueColor(blue);


			if (Hidetimer <= 0)
			{
				m_layer = Friend;

				setGreenColor(255);
				setRedColor(255);
				setBlueColor(255);
				//redColor = 255;
				//greenColor = 255;
				//blueColor = 255;
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
	

		setFlag(isAttack);
		clearFlag(IsOnePlay);
		clearFlag(bAnimAction);
		clearFlag(Btarget); // <--- Set projectile take target enemy for blink or other
		enumCounter = 17;  //  eviscirate jump Up
		clearFlag(isprojfollow);    // keep projectile follow at char 
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


		setFlag(isAttack);
		setFlag(IsOnePlay);

		setFlag(bAnimAction);  // <--blocking other animations while it's working
		clearFlag(bIsAttackable);

		clearFlag(Btarget);  // <--- Set projectile take target enemy for blink or other

		enumCounter = 18;  //  eviscirate jump down
		setFlag(isprojfollow);    // keep projectile follow at char 
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

		
			//ProjOffsetX = 0.0f;
		 //    // <--Weast
			//ProjOffsetY = 0.25f;
		

			FxColumn = 7;


		

	}
}

void cDynamic_creature_Pantir::RageMoveAttackLanding()
{
	FXFrame = 8;
	setFlag(isprojfollow);    // keep projectile follow at char 
	clearFlag(IsThrow);
	m_nGraphicCounterY = 7;
	m_nGraphicCounterX = 5;
	m_nGraphicAmountFrames = 9;  //  <---Means how long our projectile should work
	attackdirectionX = 0.1f;
	attackdirectionY = -0.3f;

	SetVerticalDirection(2);
	

	Hittimes = 4;  // <--How many times projectile should hit
	//ProjSize

	ProjCollbordersX = 0.0f;
	ProjCollbordersXF = 2.1f;
	ProjCollbordersY = 0.75f;
	ProjCollbordersYF = 1.8f;
	//

	//ProjOffsetX = 0.0f;
	// <--Weast
//	ProjOffsetY = 0.0f;

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
			shadow->setFlag(isAttack);
			shadow->px = this->px;
			shadow->py = this->py;
			shadow->enumCounter = 20;
			shadow->clearFlag(gravity);
			shadow->framespeed = 0.1f;
			shadow->SetFacingDirection(this->GetFacingDirection());
			g_engine->AddVecDynamic(shadow);



			this->SetFacingDirection(TargetChild->GetFacingDirection());  // here we grab  a way where looking our target 

			//gravityofftimer = 1.0f;
			//TargetChild->gravityofftimer = 1.0f;

			this->vx = Target->vx;  // copy velocity our target
			this->vy = Target->vy;


			//Copy from jump
			clearFlag(bAnimAction);    // <--Now jump can deflect animation downjumpevis
			clearFlag(IsOnePlay);
			clearFlag(isAttack);
			clearFlag(bOnLanded);
			clearFlag(isprojfollow);
			clearFlag(Btarget);
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
			shadow->setFlag(isAttack);
			shadow->px = this->px;
			shadow->py = this->py;
			shadow->enumCounter = 20;
			shadow->clearFlag(gravity);
			shadow->framespeed = 0.1f;
			shadow->SetFacingDirection(this->GetFacingDirection());
			g_engine->AddVecDynamic(shadow);



			this->SetFacingDirection(TargetChild->GetFacingDirection());  // here we grab  a way where looking our target 

			//gravityofftimer = 1.0f;
			//TargetChild->gravityofftimer = 1.0f;

			this->vx = Target->vx;  // copy velocity our target
			this->vy = Target->vy;


			//Copy from jump
			clearFlag(bAnimAction);    // <--Now jump can deflect animation downjumpevis
			clearFlag(IsOnePlay);
			clearFlag(isAttack);
			clearFlag(bOnLanded);
			clearFlag(isprojfollow);
			clearFlag(Btarget);
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

	if (checkFlag(bOnLanded))
	{
		return true;
	}
	else
	{
		setFlag(bOnLanded);
		
		g_engine->PlaySounds("Landed");
	
		
		if (checkFlag(bAnimAction))   // <--Means when we action stage don't switch animation on onladed
		{
			return false;
		}
		enumCounter = 11;
		clearFlag(isAttack);
		clearFlag(IsOnePlay);
		frameIndicator = 0;
		Combocounter = 0;
		fAttackcount = 0.0f;
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

void cDynamic_creature_Pantir::TakeAttributes(unsigned int attributes)
{

	// pEquipedBack->Attributes
	uint8_t objAggil = (attributes >> 0) & 0xF;
	uint8_t objInteleg = (attributes >> 4) & 0xF;
	//uint8_t objVersality = (pEquipedBack->Attributes >> 8) & 0xF;
	uint8_t objStrength = (attributes >> 12) & 0xF;
	uint8_t Dmg = (attributes >> 16) & 0xF;


	setItemAgil(getItemAgil() + objAggil);
	setItemStr(getItemStr() + objStrength);
	setItemInt(getItemInt() + objInteleg);
	/*ItemsAgil = ItemsAgil + objAggil;
	ItemsStr = ItemsStr + objStrength;
	ItemsInt = ItemsInt + objInteleg;*/
	ItemAvAtk = ItemAvAtk + Dmg;
	


}

void cDynamic_creature_Pantir::SubstracAttributes(unsigned int attributes)
{

	// pEquipedBack->Attributes
	uint8_t objAggil = (attributes >> 0) & 0xF;
	uint8_t objInteleg = (attributes >> 4) & 0xF;
	//uint8_t objVersality = (pEquipedBack->Attributes >> 8) & 0xF;
	uint8_t objStrength = (attributes >> 12) & 0xF;
	uint8_t Dmg = (attributes >> 16) & 0xF;

	setItemAgil(getItemAgil() - objAggil);
	setItemStr(getItemStr() - objStrength);
	setItemInt(getItemInt() - objInteleg);
//	ItemsAgil = ItemsAgil - objAggil;
//	ItemsStr = ItemsStr - objStrength;
//	ItemsInt = ItemsInt - objInteleg;
	ItemAvAtk = ItemAvAtk - Dmg;
	
}

void cDynamic_creature_Pantir::StatsImpact()
{
	uint8_t agi = getEndAgil();
	uint8_t bAgi = getBasicAgil();

	int ImpactAgil = getEndAgil() - getBasicAgil();  // imapct from Items and LvlIncrease  //  we calculate only that part what we have from base stats (From items + Bonus from lvl)

	float addaverage = 0.0f;
	float addmove = 0.0f;
	float addHaste = 0.0f;
	for (size_t i = 0; i < ImpactAgil; i++)  //for agility
	{
		addaverage += 0.5f;
		addmove += 0.5f;
		addHaste += 0.0025f;
	}
	rawDmg = addaverage;
	rawHaste = addHaste;
	setItemMove(addmove);

	unsigned int ds = getEndStrength();
	unsigned int dw = getEndInt();
	unsigned int da = getEndAgil();

	int ImpactStr = getEndStrength() - getBasicStrength();  // imapct from Items and LvlIncrease


	float HelpHealth = 0.0f;
	float helpDef = 0.0f;
	for (size_t i = 0; i < ImpactStr; i++)
	{
		HelpHealth += 0.6f;
		helpDef += 0.2f;

	}
	rawHealth = HelpHealth*25.f;
	rawDef = helpDef;

	EndAverageAttack = LvlAverageAttack + ItemAvAtk + rawDmg;
	EndDefence = LvlDefence + rawDef;
	EndHaste = LvlHaste + rawHaste;
	SetEndMovement(getLvlMovement() + getItemMove());
	//EndMovementspeed = getLvlMovement() + getItemMove();
	nHealthMax = lvlHealthMax + rawHealth;
	hpUpdate->setLevelHp(nHealthMax);

	if (nHealth > nHealthMax)
	{
		nHealth = nHealthMax;
	}
	
	
}



cDynamic_creature_Enemy::cDynamic_creature_Enemy(std::string n, olc::Decal* spriteRight, olc::Decal* spriteLeft) : cDynamic_Creature(n, spriteRight, spriteLeft)
{
	m_layer = Enemy;
	nHealth = 50;
	nHealthMax = 50;
	clearFlag(bDead);
	m_fStateTick = 0.3f;
	clearFlag(bOnGround);
	Jumpcounter = 0;
}

void cDynamic_creature_Enemy::Behaviour(float fElapsedTime, cDynamic* player)
{
	if (checkFlag(bControllable)) // when we control in scripteditor
	{
	

		float fTargetX = player->px - px ;  // offset for archer 
		float fTargetY = player->py - py ;


	


		float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);
	
		

		m_fStateTick -= fElapsedTime;

		if (m_fStateTick <= 0.0f)          //only if stateTick less than zero we gonna do anything. For optimization
		{
			if (nHealth > 0)
			{
				if (fDistance < 9.0f) // before target
				{

				
					vx = 0;

					



					if (checkFlag(bOnGround))  // if stay on ground
					{

						if (player->m_layer == Friend)
						{



						if (!checkFlag(isAttack) && !checkFlag(IsOnePlay))   // if no attack b noplay (hit)
						{


							if (fDistance >0.3f)
							{
							vx = (fTargetX / fDistance) *(getEndMovement()/4);    // скорость расстояние делить на время 
							}

							
							//vy = (fTargetY / fDistance) * 1.0f;

							if (vx > 0)
							{
								M_nFacingDirection = EAST;

							}
							else
							{
								M_nFacingDirection = WEST;


							}

							if (fDistance < fSpecAtckdist && rand() % 5 == 4)  // && rand() % 25 == 5
							{

								SpecAttack(fTargetX, fTargetY,fDistance);


							}
							else if (fDistance < fAttackDist)
							{
								if (rand() % 10 >= 5 && !checkFlag(isAttack))
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
					cDynamic_TextDamage* T;
					olc::vf2d textdamagecoord;
					switch (rand() % 50)
					{
					case 1:
						
						textdamagecoord = { (float)px + CollbordersX + 0.25f, (float)py };
						 T = (cDynamic_TextDamage*)g_engine->SpawnBattleText(textdamagecoord, "We shut all them.", olc::RED);

					
						break;
					case 2:
						textdamagecoord = { (float)px + CollbordersX + 0.25f, (float)py };
						 T = (cDynamic_TextDamage*)g_engine->SpawnBattleText(textdamagecoord, "Boss Protect Us...", olc::RED);
				
						break;
					case 3:
						if (abs(fTargetX) < 7.0f)
						{

						
							textdamagecoord = { (float)px + CollbordersX + 0.25f, (float)py };
							 T = (cDynamic_TextDamage*)g_engine->SpawnBattleText(textdamagecoord, "Fuck you!!!", olc::RED);

						}
						break;

					}


				}
			}
			else
			{

				setFlag(bDead);
			}

			m_fStateTick += 0.3f;

		}

		if (checkFlag(bOnGround))    
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
	//nSheetSizeX = 256.0f;
//	nSheetSizeY = 256.0f;
	looptimes = 3;
//	entityScale = 2.0f;
	//collision borders
	CollbordersX =1.0f+0.7f;
	CollbordersXF = 3.0f-0.7f;


	CollbordersY = 1.0f+0.5f;
	CollbordersYF = 3.0f;
	//

	pEquipedWeapon = (cWeapon*)RPG_Assets::get().GetItem("Bandit Boss Sword");

	hpUpdate = new Boss_HpBar(px, py, nHealth, this);

	LvL = 1;

	nSheetSizeX = 128.0f;
	nSheetSizeY = 128.0f;
	CenterX = (nSheetSizeX / g_engine->CellSize) / 2;

	setBasicMovement(20);
	setBasicAgility(7);
	setBasicStrength(50);
	setBasicInt(13);

	//BasicMovementSpeed = 20;
	//BasicAgility = 7;
	//BasicStrength = 50;
	//BasicIntelect = 13;
	BasicAveAtck = 75;

	calculateStats();

	//EndMovementspeed = getLvlMovement();
	//EndAgility = getLvlAgil();
	//EndIntelligence = getLvlInt();
	SetEndMovement(getLvlMovement());
	SetEndAgility(getLvlAgil());
	SetEndInt(getLvlInt());
	EndAverageAttack = LvlAverageAttack;

}

void cDynamic_creature_BossBandit::Behaviour(float fElapsedTime, cDynamic* player)
{
	if (checkFlag(bControllable)) // when we control in scripteditor
	{
	
		float fTargetX = (player->px+player->CenterX) - (px+CenterX);
		float fTargetY = player->py - py;
		float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);


		m_fStateTick -= fElapsedTime;

		if (m_fStateTick <= 0.0f)          //only if stateTick less than zero we gonna do anything. For optimization
		{
			if (nHealth > 0)
			{
				if (fDistance < 9.0f) // before target
				{




					if (checkFlag(bOnGround))  // if stay on ground
					{
						if (checkFlag(isAttack)==false && checkFlag(IsOnePlay)==false)   // if isAttack false and isOneplay false
						{



							vx = (fTargetX / fDistance) * (getEndMovement() / 4);


							//vy = (fTargetY / fDistance) * 1.0f;


							if (player->px > this->px+0.5f)
							{
								M_nFacingDirection = EAST;

							}
							else
							{
								M_nFacingDirection = WEST;





							}


							if (attckCount >= 3)
							{
								Mocking();
							}
							else
							{



								if (fDistance < 2.2f)
								{





									int randv = rand() % 12;
									if (randv >= 7)
									{


										//Mocking();

										SpecAttack2(fTargetX, fTargetY, fDistance); // vertushka

									}
									else if (randv <= 4)
									{

										AttackTwo();
									}
									else
									{
										SpecAttack(fTargetX, fTargetY, fDistance); // nunchaki

									}



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

				setFlag(bDead);
			}

			m_fStateTick += 0.1f;

		}

		if (checkFlag(bOnGround))
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

	M_nFacingDirectionVertical = NORTH;
	setFlag(isAttack);
	setFlag(isprojfollow);
	
	enumCounter = 4;     //  <---Attack nuncnahki
	looptimes = 2;
	frameIndicator = 0;
	FxColumn = 2;    //<--  3 column      place eviscirate
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
	m_nGraphicAmountFrames = 11;

//	ProjOffsetX = 0.0f;
//	ProjOffsetY = 0.0f;


	

	attckCount++;
}


void cDynamic_creature_BossBandit::SpecAttack2(float targetX, float targetY, float Distance)
{
	M_nFacingDirectionVertical = NORTH;

	setFlag(isAttack);
	setFlag(isprojfollow);
	
	enumCounter = 5;     //  <---Attack Huricane
	looptimes = 2;
	frameIndicator = 0;
	FxColumn = 0;    //<--  3 column      place eviscirate
	vx = 0;
	sparedVx = (targetX / Distance > 0) ? 5 : -5; //vx storrage
	sparedVy = 1.0f;
	m_nShockTime = 0.3f;
	//IsThrow = true;
	//isprojfollow = true;

	//ProjSize
	ProjCollbordersX = 0.5f;
	ProjCollbordersXF = 3.36f;
	ProjCollbordersY = CollbordersY;
	ProjCollbordersYF = CollbordersYF;
	//
	attackdirectionX = 0.3f;
	attackdirectionY = -0.5f;

	m_nGraphicCounterY = 6;
	m_nGraphicCounterX = 0;
	m_nGraphicAmountFrames = 8;

//	ProjOffsetX = 0.0f;
//	ProjOffsetY = 0.0f;


	

	
	FXFrame = m_nGraphicCounterX + m_nGraphicAmountFrames -3;


	attckCount++;
}


void cDynamic_creature_BossBandit::Mocking()
{


	setFlag(IsOnePlay);
	enumCounter = 11;     //  <---Attack Monkey
	vx = 0;
	setFlag(isDirectionLock);
	looptimes = 3;

	attckCount = 0;

}

void cDynamic_creature_BossBandit::AttackTwo()
{

	setFlag(isAttack);
	//IsThrow = true;
	setFlag(isprojfollow);
	enumCounter = 3;     //  <---Attack   pierce

	frameIndicator = 0;

	m_nGraphicCounterY = 4;
	m_nGraphicCounterX = 4;
	m_nGraphicAmountFrames = 8;
	FXFrame = 9;
	vx = 0;

	FxColumn = 1;    //<--  3 column      
	//ProjSize
	ProjCollbordersX = 1.0f;
	ProjCollbordersXF = 3.0f;
	ProjCollbordersY = CollbordersY;
	ProjCollbordersYF =CollbordersYF;
	//

	M_nFacingDirectionVertical = NOTLOOKING;

	attackdirectionX = 0.3f;
	attackdirectionY = -0.5f;
	m_nShockTime = 0.1f;

	

	attckCount++;

}


void cDynamic_creature_BossBandit::IndicateAnim()
{
	
	switch (M_nGraphicState)
	{
	case Idle:
		enumCounter = 0;
		m_nGraphicCounterY = 0;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 7;
	
		break;

	case Walk:
		enumCounter = 1;
		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 2;
		m_nGraphicAmountFrames = 7;
	

		break;
	


	case Landing:   // <--Mokicng

		enumCounter = 11;
		m_nGraphicCounterY = 2;
		m_nGraphicCounterX = 2;
		m_nGraphicAmountFrames = 5;

		
		
		if (frameIndicator == m_nGraphicCounterX + m_nGraphicAmountFrames  && looptimes != 0)
		{
			frameIndicator = m_nGraphicCounterX;
			looptimes -= 1;
		}
		break;
	case Attack:        // <-- (Horizontal Attack)
		enumCounter = 3;
		
		



		break;
	case AttackEasy:// <-- nunchaki
		enumCounter = 4;

		
	

			if (frameIndicator == m_nGraphicCounterX + m_nGraphicAmountFrames  && looptimes != 0)
			{
				frameIndicator = m_nGraphicCounterX + m_nGraphicAmountFrames-5;
				looptimes -= 1;
			}

	




		break;
	case AttackGreat:   // huricane

		if (frameIndicator == m_nGraphicCounterX + m_nGraphicAmountFrames && looptimes != 0)
		{
			frameIndicator = m_nGraphicCounterX + m_nGraphicAmountFrames - 3;
			vx = sparedVx;
			sparedVy = 1.0f;
			looptimes -= 1;
		}


		break;
	case EviscirateDown:

		break;
	case AirEviscirate: 
		


		break;

	


	}
}

void cDynamic_creature_BossBandit::ReturnToPool()
{
	{
		this->sName = "BanditBoss";
		this->nHealth = this->nHealthMax;
		g_engine->PoolsController(g_engine->getBanditBossPool(), this);
	};
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

		gfx->DrawRotatedDecal({ (this->px + (i.x * 2) - offsetx) * g_engine->CellSize ,(this->py + (i.y * 2) - offsety) * g_engine->CellSize }, VfxParticle, angle, { 0,0 }, { 1 * g_engine->fscale,1 * g_engine->fscale }, olc::Pixel(255, 255, 255, AlphaColor));
	//gfx->DrawPartialDecal({ (px+(i.x*10)) ,(py+(i.y*20)) }, VfxParticle, { 0,0 }, { 8,8 }, { 1,1 }, olc::WHITE);   //offset pulling player back into the screen
	}

}

void VfxLevel::Update(float fElapsedTimeб, cDynamic* player)
{

	timecount += fElapsedTimeб;

	AlphaColor = 255-(timecount * 100 * 2.55f);


	

	px = player->px+0.95f;
	py = player->py+1.0f;

	if (timecount>=time)
	{
		setFlag(bRedundant);
		AlphaColor = 0;
	}

	
	int index = 0;

	for (auto& i : m_vecVfxLevel)
	{
		int speed = m_speed[index];

		//i.xSpeed

		i.x -= i.x * speed * fElapsedTimeб;
		i.y -= i.y * speed * fElapsedTimeб;
		index++;

	}

}

//VfxParticles::VfxParticles(float px, float py) : cDynamic("Particles")
VfxShot::VfxShot(float px, float py) : VfxParticles(px, py)
{
}

void VfxShot::Update(float fElapsedTimeб, cDynamic* player)
{

	timecount += fElapsedTimeб;

	AlphaColor = 255 - (timecount * 100 * 2.55f);

	if (timecount >= time)
	{
		setFlag(bRedundant);
		AlphaColor = 0;
	}


	int index = 0;

	for (auto& i : m_vecVfxLevel)
	{
		int speed = m_speed[index];

		//i.xSpeed


		if (g_engine->CheckParticlePosition(this->px + (i.x * 0.3f), this->py + (i.y * 0.3f))) // checkPosition on solidblocks  if @true@ means that we can teleport
		{
			

			i.x += i.x * speed * fElapsedTimeб;
			i.y += 9 *fElapsedTimeб;
			i.y += i.y * speed * fElapsedTimeб ;
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

		gfx->DrawRotatedDecal({ (this->px + (i.x * 0.3f) - offsetx) * g_engine->CellSize ,(this->py + (i.y * 0.3f) - offsety) * g_engine->CellSize }, VfxParticle, angle, { 0,0 }, { 1*g_engine->fscale,1 * g_engine->fscale }, olc::Pixel(255, 255, 255, AlphaColor));
		//gfx->DrawPartialDecal({ (px+(i.x*10)) ,(py+(i.y*20)) }, VfxParticle, { 0,0 }, { 8,8 }, { 1,1 }, olc::WHITE);   //offset pulling player back into the screen
	}
}

void VfxShot::Spawn()
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
		float angle = (i * 10 + 180) * 3.14159f / 180.0f; // Random angle between 0 and pi radians

		olc::vf2d vSize{ std::cos(angle),std::sin(angle) };

		m_vecVfxLevel.push_back(vSize);
		m_speed.push_back(speed);
	}

}

void VfxShot::ReturnToPool()
{
	{
		SetDefault();
		g_engine->PoolsController(g_engine->getVfxShotPool(), this);

	};
}

cDynamic_creature_Boar::cDynamic_creature_Boar() : cDynamic_creature_Enemy("ForestBoar", RPG_Assets::get().GetSprite("BoarRight"), RPG_Assets::get().GetSprite("BoarLeft"))
{

	pEquipedWeapon = (cWeapon*)RPG_Assets::get().GetItem("Bandit Sword");
	hpUpdate = new cDynamic_HpBar(px, py, nHealth, this);

	LvL = 3;

	CollbordersX = 0.8f;
	CollbordersXF = 1.1f;

	CollbordersY = 0.6f;
	CollbordersYF = 1.5f;

	setBasicMovement(15);
	setBasicAgility(19);
	setBasicStrength(30);
	setBasicInt(13);
	
	SetEndMovement(20);
	calculateStats();
	EndAverageAttack = LvlAverageAttack;

	SetEndStrength(gettLvlStrength());
	SetEndInt(getLvlInt());
	SetEndAgility(getLvlAgil());

}

void cDynamic_creature_Boar::SpecAttack(float targetX, float targetY, float Distance)
{
	setFlag(isAttack) ;
	enumCounter = 4;     //  <---Attack
	framespeed = 0.12f;


	setFlag(bAnimAction);

	
	setFlag(isprojfollow);
	 
	
	
	
	m_nShockTime = 0.3f;
	
	calculatedDamage = (EndAverageAttack) * 20.65f;   //set damage for projectile

	
	attackdirectionX = 0.3f;
	attackdirectionY = -0.5f;

	

	//ProjOffsetX = 0.0f;
//	ProjOffsetY = 0.0f;


	
	



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
	
	
}

void cDynamic_creature_Boar::Behaviour(float fElapsedTime, cDynamic* player)
{

	if (checkFlag(bControllable)) // when we control in scripteditor
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
					



					if (checkFlag(bOnGround))  // if stay on ground
					{
						if (!checkFlag(isAttack) && !checkFlag(IsOnePlay) ) // only when is attack =false and isoneplay = false
						{


							if (BRunready)
							{
								vx = savedFx * (getEndMovement() / 2);

							}
							else
							{


								vx = (fTargetX / fDistance) * (getEndMovement() / 4); // normalize 


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

				setFlag(bDead);
			}

			m_fStateTick += 0.5f;

		}

		if (checkFlag(bOnGround))
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
	setFlag(IsOnePlay);
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
	
		break;
	case Landing:
		enumCounter = 11;
		m_nGraphicCounterY = 4;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 4;
		
		break;
	case Walk:
		enumCounter = 1;

		if (Run)
		{
			framespeed = 0.12f*0.5f;
			m_nGraphicCounterY = 1;
			m_nGraphicCounterX = 10;
			m_nGraphicAmountFrames = 16;
		



			if (animspr >= framespeed - EndHaste)                 //it's speed of frames
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
		
		}
		

	



		break;
	case Jump:
		framespeed = 0.12f;
		enumCounter = 2;
		m_nGraphicCounterY = 3;
		m_nGraphicCounterX = 4;
		m_nGraphicAmountFrames = 3;
	
		break;
	case JumpZenit:
		framespeed = 0.12f;
		enumCounter = 10;
		m_nGraphicCounterY = 0;
		m_nGraphicCounterX = 8;
		m_nGraphicAmountFrames = 2;
	
		break;


	case AirEviscirate:  // when boar react with obsticle 
		enumCounter = 12;

		if (frameIndicator == m_nGraphicCounterX + m_nGraphicAmountFrames - 1)
		{
			Run = false;
			clearFlag(bAnimAction);
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
		
		
		

		if (frameIndicator == m_nGraphicCounterX + m_nGraphicAmountFrames -1)
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
		

		break;






	}
}

bool cDynamic_creature_Boar::IsLanded()
{
	if (checkFlag(bOnLanded))
	{
		return true;
	}
	else
	{
		setFlag(bOnLanded);
		if (checkFlag(bAnimAction))   // <--Means when we action stage don't switch animation on onladed
		{
			return false;
		}

		vx = 0;
		enumCounter = 11;
		clearFlag(isAttack);
		setFlag(IsOnePlay);
		frameIndicator = 0;
		setEnum();
		return false;
	}
}

void cDynamic_creature_Boar::ReturnToPool()
{
	{
		cDynamic_Creature::ReturnToPool();
		this->sName = "ForestBoar";
		this->nHealth = this->nHealthMax;
		Run = false;
		BRunready = false;
		savedFx = 0.0f;
		g_engine->PoolsController(g_engine->getBoarPool(), this);
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

	if (time-timecount<0.8f)
	AlphaColor = 255 - (timecount * 100 * 2.55f);
	

	if (timecount >= time)
	{
		setFlag(bRedundant);
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

	float nSheetSizeX = currpicturePos.y*64.0f;
	float nSheetSizeY = currpicturePos.x*64.0f;

	gfx->DrawPartialDecal({ (px - ox) * g_engine->CellSize,(py - oy) * g_engine->CellSize }, sprite, { nSheetSizeX,nSheetSizeY }, { 64,64 }, { 1 ,1 }, olc::Pixel(255, 255, 255, AlphaColor));   //offset pulling player back into the screen
	
}

ERaindrop::ERaindrop(uint16_t index) :Environment("RainDrops")
{
	//px = startX;
	//py = startY;
	//this->speed = startSpeed;
	//this->angle = startAngle;
	arrIndex = index;

	psprite = RPG_Assets::get().GetSprite("RainVfx");


	cachedPositions = RPG_Assets::get().GetCache("RainFx");
	//MakeCahce();

}

void ERaindrop::Update(float fElapsedTime, float fOffsetx,float fOffsety)
{
	// Update the position of the raindrop based on its speed and angle

	//px = screenwidth+initpx;
	//py = screenheigh + initpy;

	
	
	
	// If raindrop goes off screen, wrap it around to the top
	//if (py > g_engine->ScreenHeight())
	//	py = 0;
	//if (py< 0)
	//	py = g_engine->ScreenHeight();

	//if (px > g_engine->ScreenWidth())
	//	px = 0;
	//if (px <0)
	//	px = g_engine->ScreenWidth() ;

	ftargettime += fElapsedTime;

//	float vely = speed * fElapsedTime;   // 0.01 - is snow
//	float velx = angle * fElapsedTime;   // 0.01 - is snow

	if (ftargettime>=1.0f/90.0f)
	{
		ftargettime = 0.0f;

	for (int i = 0; i < cachedPositions->size(); i++)  // бежим по всему вектору 
	{
	//	for (int ii = 0; ii <counter[i]; ii++)
	//	{
		counter[i] = counter[i] % ((*cachedPositions)[i].size()-1); // если каунтер больше чем кадров в ряду, прыгаем на 0 


		

		if (i==0)
		{

		 checkX = (*cachedPositions)[i][counter[i]].x;
		 checkY = (*cachedPositions)[i][counter[i]].y;

		}



	float worldPx = (*cachedPositions)[i][counter[i]].x / g_engine->CellSize + fOffsetx;   //переводим в мировые координаты игры 
	float worldPy = (*cachedPositions)[i][counter[i]].y / g_engine->CellSize + fOffsety;

	if (g_engine->CheckParticlePosition(worldPx, worldPy)) // checkPosition on solidblocks  if @true@ means that we can teleport
	{

		counter[i]++;  //обнавляем счётчики для всех линий

	}
	else
	{
		
		g_engine->SpawnRainSpray(worldPx, worldPy, i + 100 );
		counter[i] = 0;

	
		//	uint16_t test =arrIndex + 100 + i;   // 0-100 + 100+i;


	}

	//counter[i]++;  //обнавляем счётчики для всех линий


	//	}
	}

	}
	
}
void ERaindrop::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{
	//cachedPositions[]

	for (int i = 0; i < cachedPositions->size(); i++)
	{
		
		if (counter[i] >= (*cachedPositions)[i].size()) {
			std::cout << "Counter[" << i << "] exceeded cache size. Counter: " << counter[i]
				<< ", Cache Size: " << std::endl;
		}


			olc::vf2d vDecal((*cachedPositions)[i][counter[i]].x, (*cachedPositions)[i][counter[i]].y); // Destination position (where to draw the raindrop)

			//DrawRotatedDecal({ vDecal }, RPG_Assets::get().GetSprite("RainVfx"),atan2(vDecal.y-(vDecal.y+drop.speed*std::sin(drop.angle)), vDecal.x-(vDecal.x+drop.speed*std::cos(drop.angle))));
			gfx->DrawPartialDecal({ vDecal.x, vDecal.y }, psprite, { 0.0f,0.0f }, { 8.0f,8.0f }, { (float)1 * g_engine->fscale,(float)1.5 * g_engine->fscale }, olc::WHITE);// Draw raindrop image with transparency
	
	}
}

void ERaindrop::MakeCahce()
{
	// Массив для хранения движений капель (100 капель, каждая имеет, например, 60 позиций)
	std::vector<std::vector<CachedPosition>> cachedPositions(100, std::vector<CachedPosition>(0));

	std::array < olc::vf2d, 3> vel_ar;   // we use for make dynamic lower 
//	std::array < float, 3> angle_ar;     // and write it in file
	std::array < float, 100> speed_ar;
	std::array<float, 100> XPos_ar;


	std::ofstream outFile("cache/fxcache/rainsDrop.txt", std::ios::binary);
	if (!outFile.is_open())
	{
		std::cerr << "Unable to open file for writing: " << "cache/fxcache/rainspray.txt" << std::endl;
		return;
	}

	float time = 1.0f;            
	float cahceframe = 1.0f / 90.0f;
	//float startcount = 0.0f;

	SetPos(0.0f, 0.0f);
	
	py = 0.0f;
	
	

	for (int i = 0; i < 100; i++)        //заполняем массив на позиции X 
	{

	
		float startX = static_cast<float>(rand() % g_engine->ScreenWidth());
		bool isDuplicate = false;

		// Проверяем, существует ли уже такая позиция в массиве
		for (int ii = 0; ii < i; ii++) // Сравниваем только с уже добавленными элементами
		{
			if (XPos_ar[ii] == startX)
			{
				isDuplicate = true;
				break;
			}
		}

		// Если найдено совпадение, пропускаем текущую итерацию и пытаемся еще раз
		if (isDuplicate)
		{
			i--; // Уменьшаем индекс для повторной попытки
		}
		else
		{
			XPos_ar[i] = startX; // Если нет совпадений, сохраняем позицию
		}

	}
	
	

	for (int i = 0; i < 100; i++)
	{
		py = 0;
		float speed = static_cast<float>(rand() % 500) + 450; // Random speed between 50 and 150 pixels per second
		float startX = static_cast<float>(rand() % g_engine->ScreenWidth());

		speed_ar[i] = speed;
	do
	{
	//	startcount += cahceframe;




			float vely = speed_ar[i] * cahceframe;   // 0.01 - is snow


			py += vely;


			// Закешировать текущую позицию
			cachedPositions[i].push_back({ XPos_ar[i], py });

		

			 




	} while (py < g_engine->ScreenHeight());    // выполняем пока меньше таргета 
	}





	// Сначала записываем количество объектов (векторов позиций)
	uint32_t numObjects = cachedPositions.size(); // Получаем количество векторов
	outFile.write(reinterpret_cast<const char*>(&numObjects), sizeof(numObjects)); // Записываем количество векторов в файл

	// Записываем данные для каждого объекта
	for (const auto& positions : cachedPositions) // Итерируемся по каждому вектору позиций
	{
		uint32_t numPositions = positions.size(); // Получаем количество позиций в текущем векторе
		outFile.write(reinterpret_cast<const char*>(&numPositions), sizeof(numPositions)); // Записываем количество позиций в файл

		// Записываем каждую позицию (массив) в файл
		outFile.write(reinterpret_cast<const char*>(positions.data()), numPositions * sizeof(CachedPosition)); // Записываем все позиции текущего вектора
	}

	outFile.close(); // Закрываем файл после завершения записи
}



Edynamic_Cloud::Edynamic_Cloud(float StartX, float startY, olc::Decal* psprite, float speed, uint16_t arrindex) :Environment ("Cloud")
{
	this->px = StartX;
	this->py = startY;
	this->speed = speed;
	this->psprite = psprite;
	widthsprite = psprite->sprite->width;
	heightsprite = psprite->sprite->height;
	
	arrIndex = arrindex;
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

	gfx->DrawPartialDecal({ (this->px - ox * 10),(this->py - oy) }, psprite, { 0,0 }, { (float)widthsprite,(float)heightsprite }, { 1 * g_engine->fscale,1 * g_engine->fscale });   //offset pulling player back into the screen

}

cDynamic_creature_Rider::cDynamic_creature_Rider(olc::Decal* pspriteRight, olc::Decal* pspriteLeft) : cDynamic_creature_Enemy("Rider", pspriteRight, pspriteLeft)
{

	clearFlag(bIsAttackable);

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
		
		break;
	case Landing:
		enumCounter = 11;
		m_nGraphicCounterY = 4;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 4;
		
		break;
	case Walk:
		enumCounter = 1;
		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 4;
	

		break;
	case Jump:
		enumCounter = 2;
		m_nGraphicCounterY = 3;
		m_nGraphicCounterX = 4;
		m_nGraphicAmountFrames = 2;
		
		break;
	case JumpZenit:
		enumCounter = 10;
		m_nGraphicCounterY = 0;
		m_nGraphicCounterX = 8;
		m_nGraphicAmountFrames = 2;
		
		break;
	case JumpDown:
		enumCounter = 9;
		m_nGraphicCounterY = 3;
		m_nGraphicCounterX = 7;
		m_nGraphicAmountFrames = 2;
		
		break;
	case Attack:        // <-- (Horizontal Attack)
		enumCounter = 3;


		break;
	case AttackEasy:// <-- (Throw attack)
		enumCounter = 4;



		if (frameIndicator == m_nGraphicCounterX + m_nGraphicAmountFrames - 1)
		{
			vy = -10;

		}

		break;


	case Hit:
		enumCounter = 0;
		m_nGraphicCounterY = 3;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 4;
		

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
	hpUpdate = new cDynamic_HpBar(px, py, nHealth, this);
	 bTransformed = true;
	LvL = 3;

	CollbordersX = 0.8f;
	CollbordersXF = 1.1f;

	CollbordersY = 0.6f;
	CollbordersYF = 1.5f;

	//EndMovementspeed = 12;

	setBasicAgility(19);
	setBasicInt(13);
	setBasicStrength(10);
	//BasicAgility = 19;
	//BasicIntelect = 13;
	EndAverageAttack = 50;
	

	calculateStats();
	
	EndAverageAttack = LvlAverageAttack;
	SetEndStrength(gettLvlStrength());
	SetEndAgility(getLvlAgil());
	SetEndInt(getLvlInt());
	SetEndMovement(12);
//	EndStrength = gettLvlStrength();
//	EndIntelligence = getLvlInt();
//	EndAgility = getLvlAgil();

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
		
		break;
		}
		else
		{
			enumCounter = 0;
			m_nGraphicCounterY = 0;
			m_nGraphicCounterX = 8;
			m_nGraphicAmountFrames = 7;
			
			break;
		}
	case IdleTwo:   // WereWolf Idle



		
		break;

	case IdleReaction:   // Transform From Human to WEreWOlf
		enumCounter = 25;
		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 11;
		
		bTransformed = true;
		break;

	case Landing:
		enumCounter = 11;
		m_nGraphicCounterY = 2;
		m_nGraphicCounterX = 11;
		m_nGraphicAmountFrames = 2;
		
		break;
	case Walk:
		enumCounter = 1;
		m_nGraphicCounterY = 2;
		m_nGraphicCounterX = 3;
		m_nGraphicAmountFrames = 7;
		

		break;
	case Jump:
		enumCounter = 2;
		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 12;
		m_nGraphicAmountFrames = 3;
		
		break;
	case JumpZenit:
		enumCounter = 10;
		m_nGraphicCounterY = 1;
		m_nGraphicCounterX = 15;
		m_nGraphicAmountFrames = 2;
		
		break;
	case JumpDown:
		enumCounter = 9;
		m_nGraphicCounterY = 2;
		m_nGraphicCounterX = 0;
		m_nGraphicAmountFrames = 3;
		
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
		

		break;


	}
}

void cDynamic_creature_WereWolf::AttackOne()
{
	setFlag(isAttack);
	enumCounter = 3;     //  <---Attack

	frameIndicator = 0;


	ProjCollbordersX = 0.25f;
	ProjCollbordersXF = 1.75f;
	ProjCollbordersY = 0.65f;
	ProjCollbordersYF = 1.8f;

	setFlag(isprojfollow);
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
	
}

void cDynamic_creature_WereWolf::AttackTwo()
{
	AttackOne();
}

void cDynamic_creature_WereWolf::Action()
{
	CallWolfs();
}

void cDynamic_creature_WereWolf::DeathWolfs()
{
	for (auto it : wolfPack)
	{
		it->SetHealth(0);
	}
}

void cDynamic_creature_WereWolf::CallWolfs()
{
	



	if (wolfPack.size() < 2) //work once
	{
		olc::vf2d WolfPos =  olc::vf2d{ this->px,this->py };
		float x = -1;
		for (int i = 0; i < 2; i++)
		{
			if (wolfPack.size()>=2)			
				break;
			
			WolfPos.x += x;
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

	DeathWolfs();

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

void cDynamic_creature_WereWolf::ReturnToPool()
{
	{
		cDynamic_Creature::ReturnToPool();
		this->sName = "WereWolf";
		this->nHealth = this->nHealthMax;
		wolfPack.clear();
		g_engine->PoolsController(g_engine->getWereWolfsPool(), this);
	}
}

void cDynamic_creature_Pantir::setItemAgil(uint8_t Agil)
{
//	color = (color & 0xFFFFFF00) | Red;
	ItemAttributes = (ItemAttributes & 0xFFFFFF00) | Agil;
}

void cDynamic_creature_Pantir::setItemStr(uint8_t Str)
{
	ItemAttributes = (ItemAttributes & 0xFFFF00FF) | (Str << 8);
}

void cDynamic_creature_Pantir::setItemInt(uint8_t Int)
{
	ItemAttributes = (ItemAttributes & 0xFF00FFFF) | (Int << 16);
}

void cDynamic_creature_Pantir::setItemMove(uint8_t Move)
{
	ItemAttributes = (ItemAttributes & 0x00FFFFFF) | (Move <<24);
}

uint8_t cDynamic_creature_Pantir::getItemAgil()
{
	return ItemAttributes & 0xFF;
}
uint8_t cDynamic_creature_Pantir::getItemStr()
{
	return (ItemAttributes >> 8) & 0xFF;
}
uint8_t cDynamic_creature_Pantir::getItemInt()
{
	return (ItemAttributes >> 16) & 0xFF;
}

uint8_t cDynamic_creature_Pantir::getItemMove()
{
	return (ItemAttributes >> 24) & 0xFF;
}

void cDynamic_Creature::SetLvlAgility(uint8_t agil)
{
	Lvlattributes = (Lvlattributes & 0xFFFFFF00) | agil;    // F means mask 0 measn set 0
}

void cDynamic_Creature::SetLvlStrength(uint8_t str)
{
	Lvlattributes = (Lvlattributes & 0xFFFF00FF) | (str << 8);
}

void cDynamic_Creature::SetLvlInt(uint8_t Inteleg)
{
	Lvlattributes = (Lvlattributes & 0xFF00FFFF) | (Inteleg << 16);
}

void cDynamic_Creature::SetLvlMovement(uint8_t movement)
{
	Lvlattributes = (Lvlattributes & 0x00FFFFFF) | (movement << 24);
}

const uint8_t cDynamic_Creature::getLvlAgil()
{
	return Lvlattributes & 0xFF;
}

const uint8_t cDynamic_Creature::gettLvlStrength()
{
	return (Lvlattributes >> 8) & 0xFF;
}

const uint8_t cDynamic_Creature::getLvlInt()
{
	return (Lvlattributes >> 16) & 0xFF;
}

const uint8_t cDynamic_Creature::getLvlMovement()
{
	return (Lvlattributes >> 24) & 0xFF;
}
void cDynamic_Creature::SetEndAgility(uint8_t agil)
{
	Endattributes = (Endattributes & 0xFFFFFF00) | agil;
}
void cDynamic_Creature::SetEndStrength(uint8_t Str)
{
	Endattributes = (Endattributes & 0xFFFF00FF) | (Str << 8);
}
void cDynamic_Creature::SetEndInt(uint8_t Inteleg)
{
	Endattributes = (Endattributes & 0xFF00FFFF) | (Inteleg << 16);
}
void cDynamic_Creature::SetEndMovement(uint8_t move)
{
	Endattributes = (Endattributes & 0x00FFFFFF) | (move << 24);
}

const uint8_t cDynamic_Creature::getEndAgil()
{
	return Endattributes & 0xFF;
}

const uint8_t cDynamic_Creature::getEndStrength()
{
	return (Endattributes >> 8) & 0xFF;
}

const uint8_t cDynamic_Creature::getEndInt()
{
	return (Endattributes >> 16) & 0xFF;
}

const uint8_t cDynamic_Creature::getEndMovement()
{
	return (Endattributes >> 24) & 0xFF;
}
void cDynamic_Creature::setBasicAgility(uint8_t agility)
{
	BasicAttributes = (BasicAttributes & 0xFFFFFF00) | agility ;
}
void cDynamic_Creature::setBasicStrength(uint8_t Strength)
{
	BasicAttributes = (BasicAttributes & 0xFFFF00FF) | (Strength << 8);
}
void cDynamic_Creature::setBasicInt(uint8_t Int)
{
	BasicAttributes = (BasicAttributes & 0xFF00FFFF) | (Int << 16);
}

void cDynamic_Creature::setBasicMovement(uint8_t move)
{
	BasicAttributes = (BasicAttributes & 0x00FFFFFF) | (move << 24);
}
const uint8_t cDynamic_Creature::getBasicAgil()
{
	return BasicAttributes & 0xFF;
}

const uint8_t cDynamic_Creature::getBasicStrength()
{
	return (BasicAttributes >> 8) & 0xFF;
}

const uint8_t cDynamic_Creature::getBasicInt()
{
	return (BasicAttributes >> 16) & 0xFF;
}

const uint8_t cDynamic_Creature::getBasicMovement()
{
	return (BasicAttributes >> 24) & 0xFF;
}

cDynamic_creature_Crowd::cDynamic_creature_Crowd(std::string sname, olc::Decal* Right, olc::Decal* Left, const int Count, uint32_t Data) : cDynamic_Creature(sname, Right, Left)
{
	count = Count;

	vec_Framecounters.resize(count);
	vec_Xoffset.resize(count);
	vec_animspr.resize(count);
	m_nGraphicCounterX = Data & 0xFF;
	m_nGraphicCounterY = (Data >> 8) & 0xFF;
	m_nGraphicAmountFrames = (Data >> 16) & 0xFF;

	for (size_t i = 0; i < count; i++)
	{
		uint8_t rand1 =rand() % m_nGraphicAmountFrames;
		rand1 += m_nGraphicCounterX;
		vec_animspr[i] = 0.01f;     // timers for every copy
		vec_Framecounters[i] = rand1;  // for chaotic animation
	}
	for (int i = 0; i < count; i++)
	{
		bool record = true;
		uint8_t rand2 = rand2 = rand() % count;
		for (size_t j = 0; j <i; j++)
		{
			

			if (vec_Xoffset[j] == rand2)
			{
				record = false;
				i--;
				break;
			}

		}
		if (record)
			vec_Xoffset[i] = (float)rand2 / 3.5f;      // offest from real object for make a crowd

		
	}

	
	
	spriteWidth = m_pSpriteRight->sprite->width / nSheetSizeX;
}


void cDynamic_creature_Crowd::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{
	float nSheetOffsetY, nSheetOffsetX;
	
	for (size_t i = 0; i < count; i++)
	{
		


	//0.12f   - 0.05f (20 lvl)
	if (vec_animspr[i] >= framespeed - EndHaste)                 //it's speed of frames
	{

		vec_Framecounters[i]++;

		if (checkFlag(IsOnePlay) || checkFlag(isAttack) || checkFlag(bOnLanded))                 // if we put left mouse key
		{
			//x stroke is getting growth


			if (vec_Framecounters[i] >= m_nGraphicCounterX + m_nGraphicAmountFrames)
			{
				clearFlag(IsOnePlay);
				clearFlag(isAttack);
				clearFlag(bOnLanded);
				clearFlag(IsThrow);
				//isprojfollow = false;
				sparedVx = 0.0f;
				//	setEnum();
			}
			if (vec_Framecounters[i] == FXFrame && checkFlag(isAttack))   //   <---- summon projectile when attack
			{
				PerformAttack();
			}
		}

		vec_animspr[i] = 0;

	}
	else if (vec_Framecounters[i] >= m_nGraphicCounterX + m_nGraphicAmountFrames || vec_Framecounters[i] <= m_nGraphicCounterX)   // <==
	{

		vec_Framecounters[i] = m_nGraphicCounterX;
		setEnum();
	}


	//m_nGraphicCounterY offset for column     @frameIndicator /(int(m_pSpriteRight->sprite->width / nSheetSizeX@ lock in sprite width
	nSheetOffsetX = (vec_Framecounters[i] % spriteWidth) * nSheetSizeY;
	nSheetOffsetY = (m_nGraphicCounterY + (vec_Framecounters[i] / spriteWidth)) * nSheetSizeX;


	//std::cout << nSheetOffsetY/ nSheetSizeY <<'\t'<< nSheetOffsetX / nSheetSizeY << std::endl;

	/// //////

	if (vx >= 0.1f)
	{

		gfx->DrawPartialDecal({ (px - ox+ vec_Xoffset[i]) * (g_engine->fscale * 64.0f),(py - oy) * (g_engine->fscale * 64.0f) }, m_pSpriteRight, { nSheetOffsetX,nSheetOffsetY }, { nSheetSizeX,nSheetSizeY }, { 1,1 }, olc::Pixel(getRed(), getGreen(), getBlue()));   //offset pulling player back into the screen
	}
	else
	{
		gfx->DrawPartialDecal({ (px - ox+vec_Xoffset[i]) * (g_engine->fscale * 64.0f),(py - oy) * (g_engine->fscale * 64.0f) }, m_pSpriteLeft, { nSheetOffsetX,nSheetOffsetY }, { nSheetSizeX,nSheetSizeY }, { 1,1 }, olc::Pixel(getRed(), getGreen(), getBlue()));   //offset pulling player back into the screen

	}

	}
}

Boss_HpBar::Boss_HpBar(float ox, float oy, int HP, cDynamic_Creature* hpmember): cDynamic_HpBar (g_engine->ScreenWidth()/2,g_engine->ScreenHeight()- (g_engine->ScreenHeight()/4), HP, hpmember)
{
}

void Boss_HpBar::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{
	float hpdiff = charHp / procent;
	charHp = Hpowner->nHealth;

	if (Hpowner->checkFlag(bDead))
	{
		setFlag(bRedundant);  // пометка, что объект мёртв и память нужно освободить
	}

	float ftrueSX = (soursceSizeX*2.5)/2;


	// Отрисовка общей рамки с использованием декали
	gfx->DrawPartialDecal({ px- ftrueSX, py }, g_engine->D_FullUi,
		{ (float)soursceposX, (float)soursceposY + soursceSizeY },
		{ (float)soursceSizeX, (float)soursceSizeY },
		{ g_engine->fscale*5, g_engine->fscale*3 });

	// Отрисовка полоски здоровья (с учётом оставшегося HP)
	gfx->DrawPartialDecal({ px- ftrueSX, py }, g_engine->D_FullUi,
		{ (float)soursceposX, (float)soursceposY },
		{ hpdiff, (float)soursceSizeY },
		{ g_engine->fscale*5, g_engine->fscale*3 });

	// Формирование строки с текущим и максимальным здоровьем
	std::string sTextCurHp = std::to_string(charHp);
	std::string sTextFullHp = std::to_string(hpMax);

	// Отрисовка текста здоровья
	g_engine->DrawBigText(sTextCurHp + "/" + sTextFullHp,
		(px  + 0.36f) * (64.0f * g_engine->fscale) + (28 * g_engine->fscale),
		(py ) * (64.0f * g_engine->fscale) + (16 * g_engine->fscale),
		 g_engine->fscale*3,  g_engine->fscale*3, olc::WHITE);
}

ERainSpray::ERainSpray(float startX, float startY, uint16_t arrayindex): Environment("RainSpray") // Инициализация вектора с 3 элементами
{
	px = startX;
	py = startY;
	
	arrIndex = arrayindex;

	psprite = RPG_Assets::get().GetSprite("RainVfx");

	
	cachedPositions = RPG_Assets::get().GetCache("SprayFx");
	
	ScaleCache();

	//MakeCahce();

}

void ERainSpray::Update(float fElapsedTime, float screenwidth, float screenHeight)
{


	counter += fElapsedTime;

	datacounter++;

	datacounter = datacounter % 10;   // cached 10 frames

	for (int i = 0; i < 3; i++)
	{

		// Проверяем, чтобы индекс не выходил за границы массива cachedPositions[i]
		if (!cachedPositions[i].empty()) // Если массив не пустой
		{
			// Записываем первую позицию из кэша в Pos_ar
			Pos_ar[i].x += (*cachedPositions)[i][datacounter].x;
			Pos_ar[i].y += (*cachedPositions)[i][datacounter].y;

		}

	}

	if (counter >0.1f) // life more than one sec
	{
		
		for (int i = 0; i < 3; i++)
		{
			counter = 0;
			datacounter = 0;


		}

		redundant = true;
		
	}
}

void ERainSpray::SetPos(float px, float py)
{
	for (int i=0; i < 3; i++)
	{
		Pos_ar[i] = { px,py };
	}
}

void ERainSpray::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{
	for (int i = 0; i < 3; i++)
	{

		/*if (i==0)
		{
			std::cout << Pos_ar[i].y << std::endl;
		}*/

	gfx->DrawPartialDecal({ (Pos_ar[i].x-ox ) * g_engine->CellSize, (Pos_ar[i].y - oy) * g_engine->CellSize }, psprite, { 3.0f,4.0f }, { 1.0f,1.0f }, { (float)1 * g_engine->fscale,(float)1.5 * g_engine->fscale }, olc::WHITE);// Draw raindrop image with transparency

	}

}

void ERainSpray::MakeCahce()
{

	//std::array < olc::vf2d, 3> vel_ar;   // we use for make dynamic lower 
	//std::array < float, 3> angle_ar;     // and write it in file
	//std::array < float, 3> speed_ar;

	//std::ofstream outFile("cache/fxcache/rainspray.txt", std::ios::binary);
	//if (!outFile.is_open())
	//{
	//	std::cerr << "Unable to open file for writing: " << "cache/fxcache/rainspray.txt" << std::endl;
	//	return;
	//}


	//float time = 1.0f;            
	//float cahceframe = 1.0f / 90.0f;
	//float startcount = 0.0f;

	//SetPos(0.0f, 0.0f);
	//

	//
	//
	//for (int i = 0; i < 3; i++)
	//{
	//	float speed = static_cast<float>(rand() % 10) + 200; // Random speed between 50 and 150 pixels per second
	//	float angle = static_cast<float>(rand() % 180) * 3.14159f / 180.0f; // Random angle between 0 and pi radians

	//	speed_ar[i] = speed;
	//	angle_ar[i] = angle;
	//}

	//do
	//{
	//	startcount += cahceframe;

	//	for (int i = 0; i < 3; i++)
	//	{


	//		vel_ar[i].x = speed_ar[i] * cahceframe * std::cos(angle_ar[i]);
	//		vel_ar[i].y = -speed_ar[i] * cahceframe * std::sin(angle_ar[i]);   // 0.01 - is snow



	//		Pos_ar[i].x += vel_ar[i].x * cahceframe;
	//		Pos_ar[i].y += vel_ar[i].y * cahceframe;

	//		// Закешировать текущую позицию
	//		cachedPositions[i].push_back({ Pos_ar[i].x, Pos_ar[i].y });

	//	}






	//} while (startcount <= 0.1f);    // выполняем пока меньше таргета 






	//// Сначала записываем количество объектов (векторов позиций)
	//uint32_t numObjects = cachedPositions.size(); // Получаем количество векторов
	//outFile.write(reinterpret_cast<const char*>(&numObjects), sizeof(numObjects)); // Записываем количество векторов в файл

	//// Записываем данные для каждого объекта
	//for (const auto& positions : cachedPositions) // Итерируемся по каждому вектору позиций
	//{
	//	uint32_t numPositions = positions.size(); // Получаем количество позиций в текущем векторе
	//	outFile.write(reinterpret_cast<const char*>(&numPositions), sizeof(numPositions)); // Записываем количество позиций в файл

	//	// Записываем каждую позицию (массив) в файл
	//	outFile.write(reinterpret_cast<const char*>(positions.data()), numPositions * sizeof(CachedPosition)); // Записываем все позиции текущего вектора
	//}

	//outFile.close(); // Закрываем файл после завершения записи


}

void ERainSpray::ScaleCache()
{

	//float scaleFactor = 1.0f /25.0f;                               //fElapsedTime / (1.0f / 60.0f);


	//for (int i = 0; i < 3; i++)
	//{

	//	for (int counter = 0; counter < 10; counter++)
	//	{


	//	if (!cachedPositions[i].empty()) // Проверяем, чтобы массив не был пуст
	//	{
	//		cachedPositions[i][counter].x = cachedPositions[i][counter].x * scaleFactor;
	//		cachedPositions[i][counter].y = cachedPositions[i][counter].y * scaleFactor;
	//	}
	//	}
	//}

}

std::vector<std::vector<CachedPosition>> ERainSpray::loadCache(const std::string& filename)
{
	
	// Объявляем вектор векторов CachedPosition для хранения загруженных данных
	std::vector<std::vector<CachedPosition>> cachedPositions;

	// Открываем бинарный файл для чтения
	std::ifstream inFile(filename, std::ios::binary);
	if (!inFile.is_open())  // Проверяем, успешно ли открыт файл
	{
		std::cerr << "Unable to open file for reading: " << filename << std::endl;  // Выводим сообщение об ошибке
		return cachedPositions;  // Возвращаем пустой вектор в случае ошибки
	}

	// Читаем количество объектов (векторов) из файла
	uint32_t numObjects;
	inFile.read(reinterpret_cast<char*>(&numObjects), sizeof(numObjects));  // Читаем данные в переменную numObjects
	cachedPositions.resize(numObjects);  // Устанавливаем размер вектора cachedPositions на основе прочитанного количества объектов

	// Читаем данные для каждого объекта (вектора)
	for (auto& positions : cachedPositions)  // Итерируем по каждому вектору в cachedPositions
	{
		uint32_t numPositions;  // Объявляем переменную для хранения количества позиций в текущем векторе
		inFile.read(reinterpret_cast<char*>(&numPositions), sizeof(numPositions));  // Читаем количество позиций из файла
		positions.resize(numPositions);  // Устанавливаем размер текущего вектора на основе прочитанного количества позиций

		// Читаем каждую позицию для текущего вектора
		inFile.read(reinterpret_cast<char*>(positions.data()), numPositions * sizeof(CachedPosition));  // Считываем данные позиций в текущий вектор
	}

	// Закрываем файл после завершения чтения
	inFile.close();
	// Возвращаем загруженные данные в виде вектора векторов
	return cachedPositions;
}
