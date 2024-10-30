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
	sourcePosX = 960;
	sourcePosY = 142;
	sourceSizeX = 230;
	sourceSizeY = 17;
}


void cExperience::DrawSelf(olc::PixelGameEngine* gfx, float px, float py)
{
	float procent = g_engine->GetRequredExp() / (float)sourceSizeX;

	float EXperienceiff = g_engine->GetCurrExp() / procent;

	float pix = 0;
	float piy = 0;

	//g_engine->D_Ui

	gfx->DrawPartialDecal({ pix,piy }, g_engine->D_FullUi, { (float)sourcePosX,(float)sourcePosY }, { (float)sourceSizeX,(float)sourceSizeY },{g_engine->fscale,g_engine->fscale });   //offset pulling player back into the screen
	gfx->DrawPartialDecal({ pix,piy }, g_engine->D_FullUi, { (float)sourcePosX,(float)sourcePosY+ (float)sourceSizeY }, { EXperienceiff,(float)sourceSizeY }, { g_engine->fscale,g_engine->fscale });   //offset pulling player back into the screen


	g_engine->DrawBigText("EXP:" + std::to_string(g_engine->GetCurrExp()) + "/" + std::to_string(g_engine->GetRequredExp()), pix+(sourceSizeX / 3)*g_engine->fscale, piy+1, 0.35*g_engine->fscale, 0.35*g_engine->fscale);

	
}



cAttackLow::cAttackLow() : cEnergyIndicators("Attack Low")
{

	this->offsetX =0;
	this->offsetY = 0;

	m_Classenum = LOWATTACK;

	TalentSave = 1;

	sourcePosX = 0;
	sourcePosY = 0;
	sourceSizeX = 64;
	sourceSizeY = 64;
	
}



cAttackMid::cAttackMid() : cEnergyIndicators("Attack Mid")
{

	sourcePosX = 64;
	sourcePosY = 0;
	sourceSizeX = 64;
	sourceSizeY = 64;


	this->offsetX = sourceSizeX - (0.2 * sourceSizeX);
	this->offsetY = 0;

	m_Classenum = MIDATTACK;



	TalentSave = 2;

}

cAttackHigh::cAttackHigh() : cEnergyIndicators("Attack High")
{
	sourcePosX = 128;
	sourcePosY = 0;
	sourceSizeX = 64;
	sourceSizeY = 64;

	this->offsetX = 2*(sourceSizeX -(0.2* sourceSizeX));
	this->offsetY = 0;

	m_Classenum = HIGHATTACK;

	
	TalentSave = 3;
	

}
cAttacBack::cAttacBack() : cEnergyIndicators("Attack Back")
{
	sourcePosX = 192;
	sourcePosY = 0;
	sourceSizeX = 64;
	sourceSizeY = 64;


	this->offsetX = 1 * sourceSizeX*0.5f;
	this->offsetY = -120;

	m_Classenum = BACKATTACK;

	
	TalentSave = 4;

	
}


cStepBack::cStepBack() : cEnergyIndicators("Appear Behind")
{

	sourcePosX = 576;
	sourcePosY = 0;
	sourceSizeX = 64;
	sourceSizeY = 64;

	this->offsetX = 1 * sourceSizeX * 0.5f;
	this->offsetY = -120;

	m_Classenum = STAPBACK;

	
	TalentSave = 5;



	
}


cSwirlAttack::cSwirlAttack() : cEnergyIndicators("Swirl Attack")
{
	sourcePosX = 512;
	sourcePosY = 0;
	sourceSizeX = 64;
	sourceSizeY = 64;

	this->offsetX = 1 * sourceSizeX * 0.5f;
	this->offsetY = -120;

	m_Classenum =SWIRLATTACK;

	

	
	TalentSave = 6;
}

cVanish::cVanish() : cEnergyIndicators("Vanish")   // spells for talent
{
	sourcePosX = 640;
	sourcePosY = 0;
	sourceSizeX = 64;
	sourceSizeY = 64;

	this->offsetX = 1 * sourceSizeX * 0.5f;
	this->offsetY = -120;

	m_Classenum = VANISH;

	
	TalentSave = 7;


}







cRightAttack::cRightAttack() : cRageIndicators("Attack Right")   // spells for talent
{
	sourcePosX = 448;
	sourcePosY = 0;
	sourceSizeX = 64;
	sourceSizeY = 64;


	this->offsetX = 3 * (sourceSizeX - (0.2 * sourceSizeX))+50;
	this->offsetY = 50;

	m_Classenum = ATTACKEARTH;

	
	TalentSave = 11;

	

}





cRightAttackAir::cRightAttackAir() : cRageIndicators("Attack Right Air")   // spells for talen
{
	sourcePosX = 256;
	sourcePosY = 0;
	sourceSizeX = 64;
	sourceSizeY = 64;

	this->offsetX = 3 * (sourceSizeX - (0.2 * sourceSizeX)) + 50;
	this->offsetY = 50;

	m_Classenum = ATTACKAIR;
	
	TalentSave = 12;


}



cRightFlightUp::cRightFlightUp():cRageIndicators("Attack Right Flight Up")   // spells for talent
{
	sourcePosX = 384;
	sourcePosY = 0;
	sourceSizeX = 64;
	sourceSizeY = 64;

	this->offsetX = 3 * (sourceSizeX - (0.2 * sourceSizeX)) + 50;
	this->offsetY = 50;

	m_Classenum = ATTACKFLIGHTUP;

	
	TalentSave = 13;

	

}

cRightEpicLanding::cRightEpicLanding():cRageIndicators("Attack Right Epic Landing")   // spells for talent
{

	sourcePosX = 320;
	sourcePosY = 0;
	sourceSizeX = 64;
	sourceSizeY = 64;

	this->offsetX = 3 * (sourceSizeX - (0.2 * sourceSizeX)) + 50;
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
	

		float procent;
		float EnergyDiff;

		switch (m_Classenum)
		{
		case cEnergyIndicators::EnergyIndicatorFirst:

			procent = 33 / (float)sourceSizeX;   // calculate procent from width
			EnergyDiff = g_engine->GetEnergy() / procent;


			
			if (g_engine->GetEnergy() > 33)
			{
				this->bhide = 3;
				// bhide = !g_engine->GetBackStab();   // 1 backstub is reday
				break;
			}
			this->bhide = 1; // see filling

			break;
		case cEnergyIndicators::EnergyIndicatorSecond:

			procent = 33 / (float)sourceSizeX;
			EnergyDiff = (g_engine->GetEnergy() - 33) / procent;

		


			if (g_engine->GetEnergy() > 66)
			{

				this->bhide = 3;
				break;
			}
			this->bhide = 1;

			break;
		case cEnergyIndicators::EnergyIndicatorThird:
			procent = 33 / (float)sourceSizeX;
			EnergyDiff = (g_engine->GetEnergy() - 66) / procent;

			


			if (g_engine->GetEnergy() > 99)
			{
				this->bhide = 3;
				break;
			}

			this->bhide = 1;

			break;
		case cEnergyIndicators::JUMP:

			procent = 15 / (float)sourceSizeX;
			EnergyDiff = g_engine->GetEnergy() / procent;

			if (g_engine->GetEnergy() > 15)
			{
				this->bhide = 3;
				break;
			}
			this->bhide = 0;
			break;
		case cEnergyIndicators::BACKATTACK:

			procent = 25 / (float)sourceSizeX;
			EnergyDiff = g_engine->GetEnergy() / procent;

			if (g_engine->GetLearnedTalent(4) && g_engine->GetbOnGraund()&& g_engine->GetBackStab())
			{
				this->offsetX = 1 * sourceSizeX * 0.5f;
				this->offsetY = -120;
			bhide =1;    // when we return not returned backstub we can't see spell
			break;

			}
			bhide = 0;

			break;
		case cEnergyIndicators::LOWATTACK:
			procent = 0;
			EnergyDiff = 0;
			this->bhide = 0;
			
			break;
		case cEnergyIndicators::MIDATTACK:
			procent = 0;
			EnergyDiff = 0;
			this->bhide = 0;
		
			break;
		case cEnergyIndicators::HIGHATTACK:
			procent = 0;
			EnergyDiff = 0;
			this->bhide = 0;
			
			break;

		case cEnergyIndicators::STAPBACK:
			procent = 0;
			EnergyDiff = 64;
			if (g_engine->GetLearnedTalent(5))
			{
				this->offsetX = 2 * sourceSizeX * 0.5f;
				this->offsetY = -120;

				this->bhide = g_engine->GetTarget();    // when we return not returned backstub we can't see spell
				break;

			}
			this->bhide = 0;

			break;

		case cEnergyIndicators::SWIRLATTACK:
			procent = 0;
			EnergyDiff = 64;
			if (g_engine->GetLearnedTalent(6)&& !g_engine->GetbOnGraund())
			{

				this->offsetX = 3 * sourceSizeX * 0.5f;
				this->offsetY = -120;

				this->bhide = g_engine->GetBackStab();    // when we return not returned backstub we can't see spell
				break;

			}
			this->bhide = 0;

			break;

		case cEnergyIndicators::VANISH:
			procent = 0;
			EnergyDiff = 0;
			this->bhide = 0;

			break;

		}


		if (bhide != 0)
		{



		if (EnergyDiff <= 0)
		{
			EnergyDiff = 0;
		}
		if (EnergyDiff >= sourceSizeX)
		{
			EnergyDiff = sourceSizeX;
		}



			if (bhide == 1)
			{
				gfx->DrawPartialDecal({ px + (offsetX * g_engine->fscale),py + (offsetY * g_engine->fscale) }, g_engine->D_FullUi, { (float)sourcePosX- (float)sourceSizeX,(float)sourcePosY+34 }, { (float)sourceSizeX,(float)sourceSizeY }, { (float)0.4*g_engine->fscale,(float)0.4 * g_engine->fscale });   //Empty

				gfx->DrawPartialDecal({ px + (offsetX * g_engine->fscale),py + (offsetY * g_engine->fscale) }, g_engine->D_FullUi, { (float)sourcePosX,(float)sourcePosY }, { EnergyDiff,(float)sourceSizeY }, { (float)0.4 * g_engine->fscale,(float)0.4 * g_engine->fscale });   //Fill layer
			}



			if (  bhide == 3)   //with energy bar and rage bar case. when they full need draww borders 
			{
				gfx->DrawPartialDecal({ px + (offsetX * g_engine->fscale),py + (offsetY * g_engine->fscale) }, g_engine->D_FullUi, { (float)sourcePosX,(float)sourcePosY+(float)sourceSizeY }, { (float)sourceSizeX,(float)sourceSizeY }, { (float)0.4 * g_engine->fscale,(float)0.4 * g_engine->fscale });   //FillED layer
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

	gfx->DrawPartialDecal({ pix + (offsetX * fixSize),piy + (offsetY * fixSize) }, g_engine->D_FullUi, { (float)sourcePosX,(float)sourcePosY }, { (float)sourceSizeX,(float)sourceSizeY }, { fixSize,fixSize });   //Empty

	if (bhide == true)
		gfx->DrawPartialDecal({ pix + (offsetX *fixSize),piy + (offsetY * fixSize) }, g_engine->D_FullUi, { (float)sourcePosX,(float)sourcePosY + 64 }, { (float)sourceSizeX,(float)sourceSizeY }, { fixSize, fixSize });   //Fill layer


}

cNewEnergyIndicator::cNewEnergyIndicator(int attackvariation, int foffsetX, int foffsetY,std::string name) :cEnergyIndicators("NewEnergyIndicator")
{
//	pSpriteReady = RPG_Assets::get().GetSprite("EnergyBarNewReady");

	sName += name;
	offsetX = foffsetX;
	offsetY = foffsetY;
	
	sourcePosX = 1010;
	sourcePosY = 192;
	sourceSizeX = 50;
	sourceSizeY = 15;


	switch (attackvariation)
	{
	case 0:
	m_Classenum = EnergyIndicatorFirst;

		break;
	case 1:
		
	m_Classenum = EnergyIndicatorSecond;
		break;
	case 2:
	m_Classenum = EnergyIndicatorThird;

		break;

	}
	

	//sizeframe = 50;

}

cNewRageIndicator::cNewRageIndicator(int attackvariation, int foffsetX, int foffsetY, std::string name) :cRageIndicators("NewRageIndicator")
{
	//sizeframe = 50;

	//pSpriteReady = RPG_Assets::get().GetSprite("AngryBarNewReady");

	sName += name;
	offsetX = foffsetX;
	offsetY = foffsetY;


	switch (attackvariation)
	{
	case 0:
		m_Classenum = RageIndicatorFirst;

		break;
	case 1:
		m_Classenum = RageIndicatorSecond;
		break;

	case 2:
		m_Classenum =RageIndicatorThird;
		break;
	
	}

	sourcePosX = 960;
	sourcePosY = 192;
	sourceSizeX = 50;
	sourceSizeY = 16;


}

void cRageIndicators::DrawSelfTalent(olc::PixelGameEngine* gfx, float px, float py)
{


	switch (m_Classenum)
	{

	case cRageIndicators::ATTACKEARTH:
		offsetX = 320;
		offsetY = 640;

		bhide = g_engine->GetLearnedTalent(TalentSave);    // when we return not returned backstub we can't see spell
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

	gfx->DrawPartialDecal({ pix + (offsetX * fixSize),piy + (offsetY * fixSize) }, g_engine->D_FullUi, { (float)sourcePosX,(float)sourcePosY }, { (float)sourceSizeX,(float)sourceSizeY }, { fixSize,fixSize });   //Empty

	if (bhide == true)
		gfx->DrawPartialDecal({ pix + (offsetX * fixSize),piy + (offsetY * fixSize) }, g_engine->D_FullUi, { (float)sourcePosX,(float)sourcePosY+64 }, { (float)sourceSizeX,(float)sourceSizeY }, { fixSize,fixSize });   //Fill layer



}

void cRageIndicators::DrawSelf(olc::PixelGameEngine* gfx, float px, float py)
{


		float procent;
		float EnergyDiff;


		switch (m_Classenum)
		{
		case cRageIndicators::RageIndicatorFirst:     // suit for attack in air and on ground 
			procent = 35 / (float)sourceSizeX;
			EnergyDiff = g_engine->GetRage() / procent;


			if (g_engine->GetRage() > 35)
			{
				bhide = 3;

				break;
			}
			bhide = 1; // see filling

			break;
		case cRageIndicators::RageIndicatorSecond:
			procent = 35 / (float)sourceSizeX;
			EnergyDiff = (g_engine->GetRage() - 35) / procent;

			if (g_engine->GetRage() > 70)
			{
				bhide = 3;

				break;
			}
			bhide = 1; // see filling
			break;

		case cRageIndicators::RageIndicatorThird:
			procent = 35 / (float)sourceSizeX;
			EnergyDiff = (g_engine->GetRage() - 65) / procent;

			if (g_engine->GetRage() > 99)
			{
				bhide = 3;

				break;
			}
			bhide = 1; // see filling
			break;
		}



		float pix = px;
		float piy = py - (2 * sourceSizeY + (py / 64));


		if (EnergyDiff <= 0)
		{
			EnergyDiff = 0;
		}
		if (EnergyDiff >= 50)
		{
			EnergyDiff = 50;
		}


		if (bhide != 0)
		{

			if (bhide == 1)
			{
				gfx->DrawPartialDecal({ pix + (offsetX*g_engine->fscale),piy + (offsetY * g_engine->fscale) }, g_engine->D_FullUi, { (float)sourcePosX,(float)sourcePosY+34 }, { (float)sourceSizeX,(float)sourceSizeY }, { (float)0.4 * g_engine->fscale,(float)0.4 * g_engine->fscale });   //Empty

				gfx->DrawPartialDecal({ pix + (offsetX*g_engine->fscale),piy + (offsetY*g_engine->fscale) }, g_engine->D_FullUi, { (float)sourcePosX,(float)sourcePosY+18 }, { EnergyDiff,(float)sourceSizeY }, { (float)0.4 * g_engine->fscale,(float)0.4 * g_engine->fscale });   //Fill layer
			}



			if (bhide == 3)
			{
				gfx->DrawPartialDecal({ pix + (offsetX*g_engine->fscale),piy + (offsetY*g_engine->fscale) }, g_engine->D_FullUi, { (float)sourcePosX,(float)sourcePosY }, { (float)sourceSizeX,(float)sourceSizeY }, { (float)0.4*g_engine->fscale,(float)0.4 * g_engine->fscale });   //FillED layer
			}
		}
	



}




cJump::cJump() :cEnergyIndicators("Jump")
{
	offsetX = 0;
	offsetY = +4* sourceSizeX;
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



	gfx->DrawPartialDecal({ (pix + (offsetX * g_engine->fscale)),(piy + (offsetY * g_engine->fscale)) }, g_engine->D_FullUi, { (float)sourcePosX,(float)sourcePosY }, { (float)sourceSizeX,(float)sourceSizeY }, { 0.5f * g_engine->fscale,0.5f * g_engine->fscale });   //offset pulling player back into the screen
	
}

cLevel::cLevel() : cStaticUi("Level Ui")
{
	//RPG_Assets::get().GetSprite("UiLevelPlace")->sprite->width
	offsetX = 0;
	offsetY = 0;

	sourcePosX = 960;
	sourcePosY = 261;
	sourceSizeX = 62;
	sourceSizeY = 31;
}

cSpellPlace::cSpellPlace() : cStaticUi("Spell Place")
{
	//RPG_Assets::get().GetSprite("SpellUi")->sprite->height
	offsetX = 0;
	offsetY = 0;
	 
	sourcePosX = 832;
	sourcePosY = 0;
	sourceSizeX = 335;
	sourceSizeY = 142;
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
