#include "RPG_UI.h"
#include "RPG_Engine.h"

RPG_Engine* cUI::g_engine = nullptr;

cUI::cUI(std::string name)
{
	sName = name; ;

}

cIndicator::cIndicator(std::string name) : cUI(name)
{
	
	this->pSpriteEmpty = pSpriteEmpty;  // empty we add in indicator
}




int  cIndicator::SaveSlotTalent()
{
	return TalentSave;
}




bool cIndicator::OnInteract(cDynamic* object)
{
	return false;
}
bool cIndicator::OnUse(cDynamic* object)
{
	return false;
}

void cIndicator::DrawSelf(olc::PixelGameEngine* gfx, float px, float py)    // Draw indicators
{
}

void cIndicator::DrawSelfTalent(olc::PixelGameEngine* gfx, float px, float py)    // Draw indicators
{
}

cExperience::cExperience() : cIndicator("Experience Indicator")
{
	/*sourcePosX = 960;
	sourcePosY = 142;
	sourceSizeX = 230;
	sourceSizeY = 17;*/
	sourcePos = { 960,142 };
	sourceSize = { 230,17 };
}


void cExperience::DrawSelf(olc::PixelGameEngine* gfx, float px, float py)
{
	float procent = g_engine->GetRequredExp() / (float)sourceSize.x;

	float EXperienceiff = g_engine->GetCurrExp() / procent;

	float pix = 0;
	float piy = 0;

	//g_engine->D_Ui

	gfx->DrawPartialDecal({ pix,piy }, g_engine->D_FullUi, { sourcePos }, { (float)sourceSize.x,(float)sourceSize.y },{g_engine->fscale,g_engine->fscale });   //offset pulling player back into the screen
	gfx->DrawPartialDecal({ pix,piy }, g_engine->D_FullUi, { (float)sourcePos.x,(float)sourcePos.y+ (float)sourceSize.y }, { EXperienceiff,(float)sourceSize.y }, { g_engine->fscale,g_engine->fscale });   //offset pulling player back into the screen


	g_engine->DrawBigText("EXP:" + std::to_string(g_engine->GetCurrExp()) + "/" + std::to_string(g_engine->GetRequredExp()), pix+(sourceSize.x / 3)*g_engine->fscale, piy+1, 0.35*g_engine->fscale, 0.35*g_engine->fscale);

	
}



cAttackLow::cAttackLow() : cEnergyIndicators("Attack Low")
{

	this->offsetX =0;
	this->offsetY = 0;

	m_Classenum = LOWATTACK;

	TalentSave = 1;

//	sourcePosX = 0;
//	sourcePosY = 0;
//	sourceSizeX = 64;
	//sourceSizeY = 64;
	sourcePos = { 0,0 };
	sourceSize = { 64,64 };
}




cAttackMid::cAttackMid() : cEnergyIndicators("Attack Mid")
{

	/*sourcePosX = 64;
	sourcePosY = 0;
	sourceSizeX = 64;
	sourceSizeY = 64;*/
	sourcePos = { 64,0 };
	sourceSize = { 64,64 };


	this->offsetX = sourceSize.x - (0.2 * sourceSize.x);
	this->offsetY = 0;

	m_Classenum = MIDATTACK;



	TalentSave = 2;

}



cAttackHigh::cAttackHigh() : cEnergyIndicators("Attack High")
{
	/*sourcePosX = 128;
	sourcePosY = 0;
	sourceSizeX = 64;
	sourceSizeY = 64;*/

	sourcePos = { 128,0 };
	sourceSize = { 64,64 };

	this->offsetX = 2*(sourceSize.x -(0.2* sourceSize.x));
	this->offsetY = 0;

	m_Classenum = HIGHATTACK;

	
	TalentSave = 3;
	

}

cAttacBack::cAttacBack() : cEnergyIndicators("Attack Back")
{
	/*sourcePosX = 192;
	sourcePosY = 0;
	sourceSizeX = 64;
	sourceSizeY = 64;*/

	sourcePos = { 192,0 };
	sourceSize = { 64,64 };

	this->offsetX = 1 * sourceSize.x*0.5f;
	this->offsetY = -120;

	m_Classenum = BACKATTACK;

	
	TalentSave = 4;

	
}


cStepBack::cStepBack() : cEnergyIndicators("Appear Behind")
{

	/*sourcePosX = 576;
	sourcePosY = 0;
	sourceSizeX = 64;
	sourceSizeY = 64;*/

	sourcePos = { 576,0 };
	sourceSize = { 64,64 };

	this->offsetX = 1 * sourceSize.x * 0.5f;
	this->offsetY = -120;

	m_Classenum = STAPBACK;

	
	TalentSave = 5;



	
}


cSwirlAttack::cSwirlAttack() : cEnergyIndicators("Swirl Attack")
{
	/*sourcePosX = 512;
	sourcePosY = 0;
	sourceSizeX = 64;
	sourceSizeY = 64;*/

	sourcePos = { 512,0 };
	sourceSize = { 64,64 };

	this->offsetX = 1 * sourceSize.x * 0.5f;
	this->offsetY = -120;

	m_Classenum =SWIRLATTACK;

	

	
	TalentSave = 6;
}

cVanish::cVanish() : cEnergyIndicators("Vanish")   // spells for talent
{
	/*sourcePosX = 640;
	sourcePosY = 0;
	sourceSizeX = 64;
	sourceSizeY = 64;*/

	sourcePos = { 640,0 };
	sourceSize = { 64,64 };

	this->offsetX = 1 * sourceSize.x * 0.5f;
	this->offsetY = -120;

	m_Classenum = VANISH;

	
	TalentSave = 7;


}







cRightAttack::cRightAttack() : cRageIndicators("Attack Right")   // spells for talent
{
	/*sourcePosX = 448;
	sourcePosY = 0;
	sourceSizeX = 64;
	sourceSizeY = 64;*/

	sourcePos = { 448,0 };
	sourceSize = { 64,64 };


	this->offsetX = 3 * (sourceSize.x - (0.2 * sourceSize.x))+50;
	this->offsetY = 50;

	m_Classenum = ATTACKEARTH;

	
	TalentSave = 11;

	

}





cRightAttackAir::cRightAttackAir() : cRageIndicators("Attack Right Air")   // spells for talen
{
	/*sourcePosX = 256;
	sourcePosY = 0;
	sourceSizeX = 64;
	sourceSizeY = 64;*/

	sourcePos = { 256,0 };
	sourceSize = { 64,64 };

	this->offsetX = 3 * (sourceSize.x - (0.2 * sourceSize.x)) + 50;
	this->offsetY = 50;

	m_Classenum = ATTACKAIR;
	
	TalentSave = 12;


}



cRightFlightUp::cRightFlightUp():cRageIndicators("Attack Right Flight Up")   // spells for talent
{
	/*sourcePosX = 384;
	sourcePosY = 0;
	sourceSizeX = 64;
	sourceSizeY = 64;*/

	sourcePos = { 384,0 };
	sourceSize = { 64,64 };


	this->offsetX = 3 * (sourceSize.x - (0.2 * sourceSize.x)) + 50;
	this->offsetY = 50;

	m_Classenum = ATTACKFLIGHTUP;

	
	TalentSave = 13;

	

}

cRightEpicLanding::cRightEpicLanding():cRageIndicators("Attack Right Epic Landing")   // spells for talent
{

	//sourcePosX = 320;
	//sourcePosY = 0;
	//sourceSizeX = 64;
	//sourceSizeY = 64;

	sourcePos = { 320,0 };
	sourceSize = { 64,64 };

	this->offsetX = 3 * (sourceSize.x - (0.2 * sourceSize.x)) + 50;
	this->offsetY = 50;

	m_Classenum = ATTACKEPICLANDING;

	
	TalentSave = 14;


}







cEnergyIndicators::cEnergyIndicators(std::string name) : cIndicator(name)
{

	this->offsetX = 0;
	this->offsetY = 0;
//	sizeframe = pSpriteFull->sprite->width;

}

cRageIndicators::cRageIndicators(std::string name) : cIndicator(name)
{
	this->offsetX = 0;
this->	offsetY = 0;
//	sizeframe = pSpriteFull->sprite->width;
}




void cEnergyIndicators::DrawSelf(olc::PixelGameEngine* gfx, float px, float py)
{
	

		
		Update();

		if (bhide != 0)
		{



			if (IndicatorDiff <= 0)
			{
				IndicatorDiff = 0;
			}
			if (IndicatorDiff >= 50)
			{
				IndicatorDiff = 50;
			}
	


			if (bhide == 1)
			{
				gfx->DrawPartialDecal({ px + (offsetX * g_engine->fscale),py + (offsetY * g_engine->fscale) }, g_engine->D_FullUi, { (float)sourcePos.x- (float)sourceSize.x,(float)sourcePos.y+34 }, sourceSize, { (float)0.4*g_engine->fscale,(float)0.4 * g_engine->fscale });   //Empty

				gfx->DrawPartialDecal({ px + (offsetX * g_engine->fscale),py + (offsetY * g_engine->fscale) }, g_engine->D_FullUi,  sourcePos, { IndicatorDiff,(float)sourceSize.y }, { (float)0.4 * g_engine->fscale,(float)0.4 * g_engine->fscale });   //Fill layer
			}



			if (  bhide == 3)   //with energy bar and rage bar case. when they full need draww borders 
			{
				gfx->DrawPartialDecal({ px + (offsetX * g_engine->fscale),py + (offsetY * g_engine->fscale) }, g_engine->D_FullUi, { (float)sourcePos.x,(float)sourcePos.y+(float)sourceSize.y }, sourceSize, { (float)0.4 * g_engine->fscale,(float)0.4 * g_engine->fscale });   //FillED layer
			}
		}

	

}


void cEnergyIndicators::DrawSelfTalent(olc::PixelGameEngine* gfx, float px, float py)
{


	switch (m_Classenum)
	{

	case cEnergyIndicators::LOWATTACK:
		offsetX = 192;
		offsetY = 640;


		bhide = g_engine->GetLearnedTalent(TalentSave);    // when we return not returned backstub we can't see spell
		break;

	case cEnergyIndicators::MIDATTACK:
		offsetX = 192;
		offsetY = 512;

		bhide = g_engine->GetLearnedTalent(TalentSave);    // when we return not returned backstub we can't see spell
		break;

	case cEnergyIndicators::HIGHATTACK:
		offsetX = 64;
		offsetY = 512;


		bhide = g_engine->GetLearnedTalent(TalentSave);    // when we return not returned backstub we can't see spell
		break;
	case cEnergyIndicators::BACKATTACK:
		offsetX = 64;
		offsetY = 640;

		bhide = g_engine->GetLearnedTalent(TalentSave);    // when we return not returned backstub we can't see spell
		break;



	case cEnergyIndicators::STAPBACK:
		offsetX = 64;
		offsetY = 384;

		bhide = g_engine->GetLearnedTalent(TalentSave);
		break;

	case cEnergyIndicators::SWIRLATTACK:
		offsetX = 64;
		offsetY = 192;

		bhide = g_engine->GetLearnedTalent(TalentSave);    // when we return not returned backstub we can't see spell
		break;
	case cEnergyIndicators::VANISH:
		offsetX = 192;
		offsetY = 384;

		bhide = g_engine->GetLearnedTalent(TalentSave);    // when we return not returned backstub we can't see spell
		break;

	}




	float pix = px;
	float piy = py;
	float fixSize = 0.5f * g_engine->fscale;

	gfx->DrawPartialDecal({ pix + (offsetX * fixSize),piy + (offsetY * fixSize) }, g_engine->D_FullUi, sourcePos, sourceSize, { fixSize,fixSize });   //Empty

	if (bhide == true)
		gfx->DrawPartialDecal({ pix + (offsetX *fixSize),piy + (offsetY * fixSize) }, g_engine->D_FullUi, { (float)sourcePos.x,(float)sourcePos.y + 64 }, sourceSize, { fixSize, fixSize });   //Fill layer


}

cNewEnergyIndicator::cNewEnergyIndicator(int attackvariation, int foffsetX, int foffsetY,std::string name) :cEnergyIndicators("NewEnergyIndicator")
{
//	pSpriteReady = RPG_Assets::get().GetSprite("EnergyBarNewReady");

	sName += name;
	offsetX = foffsetX;
	offsetY = foffsetY;
	
	/*sourcePosX = 1010;
	sourcePosY = 192;
	sourceSizeX = 50;
	sourceSizeY = 15;*/

	sourcePos = { 1010,192 };
	sourceSize = { 50,15 };

	dampIndicator = 0;
	lowCoefficient = 33;
	switch (attackvariation)
	{
	case 0:
	m_Classenum = EnergyIndicatorFirst;
	highBorder = 33;
		break;
	case 1:
		
	m_Classenum = EnergyIndicatorSecond;
	dampIndicator = 33;
	highBorder = 66;
		break;
	case 2:
	m_Classenum = EnergyIndicatorThird;
	dampIndicator = 66;
	highBorder = 99;
		break;

	}
	

	//sizeframe = 50;

}

void cNewEnergyIndicator::Update()
{

	float procent;
	IndicatorDiff;

	procent = lowCoefficient / (float)sourceSize.x;   // calculate procent from width

	// ѕровер€ем, что procent не равен нулю перед делением
	if (procent != 0) {
		IndicatorDiff = (g_engine->GetEnergy() - dampIndicator) / procent;
	}
	else {
		// ќбрабатываем случай, когда procent равен нулю
		IndicatorDiff = 0;  // или установите значение, подход€щее под вашу логику
	}




	this->bhide = 1; // see filling
	if (g_engine->GetEnergy() > highBorder)
	{
		this->bhide = 3;
	}
}

cNewRageIndicator::cNewRageIndicator(int attackvariation, int foffsetX, int foffsetY, std::string name) :cRageIndicators("NewRageIndicator")
{
	//sizeframe = 50;

	//pSpriteReady = RPG_Assets::get().GetSprite("AngryBarNewReady");

	sName += name;
	offsetX = foffsetX;
	offsetY = foffsetY;

	lowCoefficient = 35;
	dampIndicator = 0;
	switch (attackvariation)
	{
	case 0:
		m_Classenum = RageIndicatorFirst;

		highBorder = 35;
		break;
	case 1:
		m_Classenum = RageIndicatorSecond;
		dampIndicator = 35;
		highBorder = 70;
		break;

	case 2:
		m_Classenum =RageIndicatorThird;
		dampIndicator = 65;
		highBorder = 99;
		break;
	
	}

	//sourcePosX = 960;
	//sourcePosY = 192;
	//sourceSizeX = 50;
	//sourceSizeY = 16;

	sourcePos = { 960,192 };
	sourceSize = { 50,16 };
}

void cNewRageIndicator::Update()
{

	float procent;
	IndicatorDiff;

	procent = lowCoefficient / (float)sourceSize.x;   // calculate procent from width

	// ѕровер€ем, что procent не равен нулю перед делением
	if (procent != 0) {
		IndicatorDiff = (g_engine->GetRage() - dampIndicator) / procent;
	}
	else {
		// ќбрабатываем случай, когда procent равен нулю
		IndicatorDiff = 0;  // или установите значение, подход€щее под вашу логику
	}

	//	EnergyDiff = g_engine->GetEnergy() / procent - dampIndicator;

//	std::cout << static_cast<int>(g_engine->GetEnergy()) << std::endl;

	this->bhide = 1; // see filling
	if (g_engine->GetRage() > highBorder)
	{
		this->bhide = 3;
	}
}

void cRageIndicators::DrawSelfTalent(olc::PixelGameEngine* gfx, float px, float py)
{


	switch (m_Classenum)
	{

	case cRageIndicators::ATTACKEARTH:
		offsetX = 320;
		offsetY = 640;

		bhide = g_engine->GetLearnedTalent(TalentSave);    // when we return @not@ returned backstub, we can't see spell
		break;

	case cRageIndicators::ATTACKAIR:
		offsetX = 320;
		offsetY = 512;

		bhide = g_engine->GetLearnedTalent(TalentSave);    // when we return not returned backstub we can't see spell
		break;


	case cRageIndicators::ATTACKFLIGHTUP:
		offsetX = 320;
		offsetY = 256;

		bhide = g_engine->GetLearnedTalent(TalentSave);    // when we return not returned backstub we can't see spell
		break;

	case cRageIndicators::ATTACKEPICLANDING:
		offsetX = 320;
		offsetY = 64;

		bhide = g_engine->GetLearnedTalent(TalentSave);    // when we return not returned backstub we can't see spell
		break;


	}




	float pix = px;
	float piy = py;
	float fixSize = 0.5f * g_engine->fscale;

	gfx->DrawPartialDecal({ pix + (offsetX * fixSize),piy + (offsetY * fixSize) }, g_engine->D_FullUi, sourcePos,sourceSize, { fixSize,fixSize });   //Empty

	if (bhide == true)
		gfx->DrawPartialDecal({ pix + (offsetX * fixSize),piy + (offsetY * fixSize) }, g_engine->D_FullUi, { (float)sourcePos.x,(float)sourcePos.y+64 }, sourceSize, { fixSize,fixSize });   //Fill layer



}



void cRageIndicators::DrawSelf(olc::PixelGameEngine* gfx, float px, float py)
{


	

	Update();
		



		float pix = px;
		float piy = py - (2 * sourceSize.y + (py / 64));


		if (IndicatorDiff <= 0)
		{
			IndicatorDiff = 0;
		}
		if (IndicatorDiff >= 50)
		{
			IndicatorDiff = 50;
		}


		if (bhide != 0)
		{

			if (bhide == 1)
			{
				gfx->DrawPartialDecal({ pix + (offsetX*g_engine->fscale),piy + (offsetY * g_engine->fscale) }, g_engine->D_FullUi, { (float)sourcePos.x,(float)sourcePos.y+34 }, sourceSize, { (float)0.4 * g_engine->fscale,(float)0.4 * g_engine->fscale });   //Empty

				gfx->DrawPartialDecal({ pix + (offsetX*g_engine->fscale),piy + (offsetY*g_engine->fscale) }, g_engine->D_FullUi, { (float)sourcePos.x,(float)sourcePos.y+18 }, { IndicatorDiff,(float)sourceSize.y }, { (float)0.4 * g_engine->fscale,(float)0.4 * g_engine->fscale });   //Fill layer
			}



			if (bhide == 3)
			{
				gfx->DrawPartialDecal({ pix + (offsetX*g_engine->fscale),piy + (offsetY*g_engine->fscale) }, g_engine->D_FullUi, sourcePos, sourceSize, { (float)0.4*g_engine->fscale,(float)0.4 * g_engine->fscale });   //FillED layer
			}
		}
	



}




cJump::cJump() :cEnergyIndicators("Jump")
{
	offsetX = 0;
	offsetY = +4* sourceSize.x;
	m_Classenum = JUMP;
	
}






cStaticUi::cStaticUi(std::string name) : cUI(name)
{
	offsetX = 0;
	offsetY = 0;
}


void  cStaticUi::DrawSelf(olc::PixelGameEngine* gfx, float px, float py)
{


	float pix = 10;
	float piy = 10;



	gfx->DrawPartialDecal({ (pix + (offsetX * g_engine->fscale)),(piy + (offsetY * g_engine->fscale)) }, g_engine->D_FullUi, sourcePos, sourceSize, { 0.5f * g_engine->fscale,0.5f * g_engine->fscale });   //offset pulling player back into the screen
	
}

cLevel::cLevel() : cStaticUi("Level Ui")
{
	//RPG_Assets::get().GetSprite("UiLevelPlace")->sprite->width
	offsetX = 0;
	offsetY = 0;

	/*sourcePosX = 960;
	sourcePosY = 261;
	sourceSizeX = 62;
	sourceSizeY = 31;*/

	sourcePos = { 960,261 };
	sourceSize = { 62,31 };
}

cSpellPlace::cSpellPlace() : cStaticUi("Spell Place")
{
	//RPG_Assets::get().GetSprite("SpellUi")->sprite->height
	offsetX = 0;
	offsetY = 0;
	 
	/*sourcePosX = 832;
	sourcePosY = 0;
	sourceSizeX = 335;
	sourceSizeY = 142;*/

	sourcePos = { 832,0 };
	sourceSize = { 335,142 };
}

cNumber::cNumber() :cStaticUi("Level Number")
{
	offsetX = 8;
	offsetY = 0;
}

void  cNumber::DrawSelf(olc::PixelGameEngine* gfx, float px, float py)
{
	int i = 0;
	int x = g_engine->GetLvl();
	std::string sText =  std::to_string(x);

	float pix = 10;
	float piy = 10;


	g_engine->DrawBigText(sText, (pix + (offsetX * g_engine->fscale)), (piy + (offsetY * g_engine->fscale)), 0.5f * g_engine->fscale, 0.5f * g_engine->fscale, olc::YELLOW);

	//gfx->DrawPartialDecal({ ((float)px + offsetX),((float)py + offsetY) }, pSpriteFull, { 0,0 }, { (float)pSpriteFull->sprite->width,(float)pSpriteFull->sprite->height });   //offset pulling player back into the screen

}
