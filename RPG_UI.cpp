#include "RPG_UI.h"
#include "RPG_Engine.h"

RPG_Engine* cUI::g_engine = nullptr;

cUI::cUI(std::string name, olc::Decal* Full )
{
	sName = name; pSpriteFull = Full;

}

cIndicator::cIndicator(std::string name, olc::Decal* pSpriteFull, olc::Decal* pSpriteEmpty) : cUI(name, pSpriteFull)
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

void cIndicator::DrawSelf(olc::PixelGameEngine* gfx, int px, int py)    // Draw indicators
{
}

void cIndicator::DrawSelfTalent(olc::PixelGameEngine* gfx, int px, int py)    // Draw indicators
{
}

cExperience::cExperience() : cIndicator("Experience Indicator", RPG_Assets::get().GetSprite("ExperienceFull"), RPG_Assets::get().GetSprite("ExperienceEmprty"))
{
}


void cExperience::DrawSelf(olc::PixelGameEngine* gfx, int px, int py)
{
	float procent = g_engine->GetRequredExp() / (float)pSpriteFull->sprite->width;

	float EXperienceiff = g_engine->GetCurrExp() / procent;

	float pix = 0;
	float piy = 0;

	gfx->DrawPartialDecal({ pix,piy }, pSpriteEmpty, { 0,0 }, { (float)pSpriteFull->sprite->width,(float)pSpriteFull->sprite->height });   //offset pulling player back into the screen

	gfx->DrawPartialDecal({ pix,piy }, pSpriteFull, { 0,0 }, { EXperienceiff,(float)pSpriteFull->sprite->height });   //offset pulling player back into the screen


	g_engine->DrawBigText("EXP:" + std::to_string(g_engine->GetCurrExp()) + "/" + std::to_string(g_engine->GetRequredExp()), pix+(pSpriteFull->sprite->width / 4), piy, 0.35, 0.35);

	
}



cAttackLow::cAttackLow() : cEnergyIndicators("Attack Low", RPG_Assets::get().GetSprite("AttackEasyFullUi"), RPG_Assets::get().GetSprite("AttackEasyEmptyUi"))
{

	this->offsetX =0;
	this->offsetY = 0;

	m_Classenum = LOWATTACK;

	TalentSave = 1;
	
}



cAttackMid::cAttackMid() : cEnergyIndicators("Attack Mid", RPG_Assets::get().GetSprite("AttackMidFullUi"), RPG_Assets::get().GetSprite("AttackMidEmptyUi"))
{

	this->offsetX = pSpriteFull->sprite->width- (0.2 * pSpriteFull->sprite->width);
	this->offsetY = 0;

	m_Classenum = MIDATTACK;

	
	TalentSave = 2;

}

cAttackHigh::cAttackHigh() : cEnergyIndicators("Attack High", RPG_Assets::get().GetSprite("AttackHighFullUi"), RPG_Assets::get().GetSprite("AttackHighEmptyUi"))
{

	this->offsetX = 2*(pSpriteFull->sprite->width-(0.2* pSpriteFull->sprite->width));
	this->offsetY = 0;

	m_Classenum = HIGHATTACK;

	
	TalentSave = 3;

}
cAttacBack::cAttacBack() : cEnergyIndicators("Attack Back", RPG_Assets::get().GetSprite("BackStubFullUi"), RPG_Assets::get().GetSprite("BackStubEmptyUi"))
{

	this->offsetX = 1 * pSpriteFull->sprite->width*0.5f;
	this->offsetY = -120;

	m_Classenum = BACKATTACK;

	
	TalentSave = 4;
}


cStepBack::cStepBack() : cEnergyIndicators("Appear Behind", RPG_Assets::get().GetSprite("ShadowStepFullUi"), RPG_Assets::get().GetSprite("ShadowStepEmptyUi"))
{
	this->offsetX = 1 * pSpriteFull->sprite->width * 0.5f;
	this->offsetY = -120;

	m_Classenum = STAPBACK;

	
	TalentSave = 5;
}


cSwirlAttack::cSwirlAttack() : cEnergyIndicators("Swirl Attack", RPG_Assets::get().GetSprite("GrabSwirlFullUi"), RPG_Assets::get().GetSprite("GrabSwirlEmptyUi"))
{
	this->offsetX = 1 * pSpriteFull->sprite->width * 0.5f;
	this->offsetY = -120;

	m_Classenum =SWIRLATTACK;

	
	TalentSave = 6;
}

cVanish::cVanish() : cEnergyIndicators("Vanish", RPG_Assets::get().GetSprite("VanishFullUi"), RPG_Assets::get().GetSprite("VanishEmptyUi"))   // spells for talent
{

	this->offsetX = 1 * pSpriteFull->sprite->width * 0.5f;
	this->offsetY = -120;

	m_Classenum = VANISH;

	
	TalentSave = 7;

}







cRightAttack::cRightAttack() : cRageIndicators("Attack Right", RPG_Assets::get().GetSprite("EviscirateUpFullUi"), RPG_Assets::get().GetSprite("EviscirateUpEmptyUi"))   // spells for talent
{

	this->offsetX = 3 * (pSpriteFull->sprite->width - (0.2 * pSpriteFull->sprite->width))+50;
	this->offsetY = 50;

	m_Classenum = ATTACKEARTH;

	
	TalentSave = 11;

}





cRightAttackAir::cRightAttackAir() : cRageIndicators("Attack Right Air", RPG_Assets::get().GetSprite("EviscirateDownFull"), RPG_Assets::get().GetSprite("EviscirateDownEmptyUi"))   // spells for talen
{

	this->offsetX = 3 * (pSpriteFull->sprite->width - (0.2 * pSpriteFull->sprite->width)) + 50;
	this->offsetY = 50;

	m_Classenum = ATTACKAIR;
	
	TalentSave = 12;
}



cRightFlightUp::cRightFlightUp():cRageIndicators("Attack Right Flight Up", RPG_Assets::get().GetSprite("EviscirateFlightFullUi"), RPG_Assets::get().GetSprite("EviscirateFlightEmptyUi"))   // spells for talent
{

	this->offsetX = 3 * (pSpriteFull->sprite->width - (0.2 * pSpriteFull->sprite->width)) + 50;
	this->offsetY = 50;

	m_Classenum = ATTACKFLIGHTUP;

	
	TalentSave = 13;

}

cRightEpicLanding::cRightEpicLanding():cRageIndicators("Attack Right Epic Landing", RPG_Assets::get().GetSprite("EviscirateFallLandingFullUi"), RPG_Assets::get().GetSprite("EviscirateFallLandingEmptyUi"))   // spells for talent
{

	this->offsetX = 3 * (pSpriteFull->sprite->width - (0.2 * pSpriteFull->sprite->width)) + 50;
	this->offsetY = 50;

	m_Classenum = ATTACKEPICLANDING;

	
	TalentSave = 14;

}







cEnergyIndicators::cEnergyIndicators(std::string name, olc::Decal* Full, olc::Decal* Empty) : cIndicator(name, Full,Empty)
{

	this->offsetX = 0;
	this->offsetY = 0;
	sizeframe = pSpriteFull->sprite->width;

}

cRageIndicators::cRageIndicators(std::string name, olc::Decal* full, olc::Decal* Empty) : cIndicator(name, full, Empty)
{
	this->offsetX = 0;
this->	offsetY = 0;
	sizeframe = pSpriteFull->sprite->width;
}




void cEnergyIndicators::DrawSelf(olc::PixelGameEngine* gfx, int px, int py)
{
	

		float procent;
		float EnergyDiff;

		switch (m_Classenum)
		{
		case cEnergyIndicators::EnergyIndicatorFirst:

			procent = 35 / (float)pSpriteFull->sprite->width;   // calcilate procent from width
			EnergyDiff = g_engine->GetEnergy() / procent;


			
			if (g_engine->GetEnergy() > 35)
			{
				this->bhide = 3;
				// bhide = !g_engine->GetBackStab();   // 1 backstub is reday
				break;
			}
			this->bhide = 1; // see filling

			break;
		case cEnergyIndicators::EnergyIndicatorSecond:

			procent = 20 / (float)pSpriteFull->sprite->width;
			EnergyDiff = (g_engine->GetEnergy() - 35) / procent;

		


			if (g_engine->GetEnergy() > 55)
			{

				this->bhide = 3;
				break;
			}
			this->bhide = 1;

			break;
		case cEnergyIndicators::EnergyIndicatorThird:
			procent = 45 / (float)pSpriteFull->sprite->width;
			EnergyDiff = (g_engine->GetEnergy() - 55) / procent;

			


			if (g_engine->GetEnergy() > 99)
			{
				this->bhide = 3;
				break;
			}

			this->bhide = 1;

			break;
		case cEnergyIndicators::JUMP:

			procent = 15 / (float)pSpriteFull->sprite->width;
			EnergyDiff = g_engine->GetEnergy() / procent;

			if (g_engine->GetEnergy() > 15)
			{
				this->bhide = 3;
				break;
			}
			this->bhide = 0;
			break;
		case cEnergyIndicators::BACKATTACK:

			procent = 25 / (float)pSpriteFull->sprite->width;
			EnergyDiff = g_engine->GetEnergy() / procent;

			if (g_engine->GetLearnedTalent(4) && g_engine->GetbOnGraund()&& g_engine->GetBackStab())
			{
				this->offsetX = 1 * pSpriteFull->sprite->width * 0.5f;
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
				this->offsetX = 2 * pSpriteFull->sprite->width * 0.5f;
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

				this->offsetX = 3 * pSpriteFull->sprite->width * 0.5f;
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



		float pix = px;
		float piy = py ;


		if (EnergyDiff <= 0)
		{
			EnergyDiff = 0;
		}
		if (EnergyDiff >=sizeframe)
		{
			EnergyDiff = sizeframe;
		}


		if (bhide != 0)
		{

			if (bhide == 1)
			{
				gfx->DrawPartialDecal({ pix + offsetX,piy + offsetY }, pSpriteEmpty, { 0,0 }, { (float)pSpriteFull->sprite->width,(float)pSpriteFull->sprite->height }, { 0.4,0.4 });   //Empty

				gfx->DrawPartialDecal({ pix + offsetX,piy + offsetY }, pSpriteFull, { 0,0 }, { EnergyDiff,(float)pSpriteFull->sprite->height }, { 0.4,0.4 });   //Fill layer
			}



			if (pSpriteReady != nullptr && bhide == 3)   //with energy bar and rage bar case. when they full need draww borders 
			{
				gfx->DrawPartialDecal({ pix + offsetX,piy + offsetY }, pSpriteReady, { 0,0 }, { (float)pSpriteReady->sprite->width,(float)pSpriteReady->sprite->height }, { 0.4,0.4 });   //FillED layer
			}
		}

	

}


void cEnergyIndicators::DrawSelfTalent(olc::PixelGameEngine* gfx, int px, int py)
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


	gfx->DrawPartialDecal({ pix + (offsetX * g_engine->fscale),piy + (offsetY * g_engine->fscale) }, pSpriteEmpty, { 0,0 }, { (float)pSpriteFull->sprite->width,(float)pSpriteFull->sprite->height }, { 0.5,0.5  });   //Empty

	if (bhide == true)
		gfx->DrawPartialDecal({ pix + (offsetX *g_engine->fscale),piy + (offsetY *g_engine->fscale) }, pSpriteFull, { 0,0 }, { (float)pSpriteFull->sprite->width,(float)pSpriteFull->sprite->height }, { 0.5 ,0.5  });   //Fill layer


}

cNewEnergyIndicator::cNewEnergyIndicator(int attackvariation, int foffsetX, int foffsetY,std::string name) :cEnergyIndicators("NewEnergyIndicator", RPG_Assets::get().GetSprite("EnergyBarNewFill"), RPG_Assets::get().GetSprite("AngryBarNewEmpty"))
{
	pSpriteReady = RPG_Assets::get().GetSprite("EnergyBarNewReady");

	sName += name;
	offsetX = foffsetX;
	offsetY = foffsetY;
	

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
	

	sizeframe = 50;

}

cNewRageIndicator::cNewRageIndicator(int attackvariation, int foffsetX, int foffsetY, std::string name) :cRageIndicators("NewRageIndicator", RPG_Assets::get().GetSprite("AngryBarNewFill"), RPG_Assets::get().GetSprite("AngryBarNewEmpty"))
{
	sizeframe = 50;

	pSpriteReady = RPG_Assets::get().GetSprite("AngryBarNewReady");

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

}

void cRageIndicators::DrawSelfTalent(olc::PixelGameEngine* gfx, int px, int py)
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


	gfx->DrawPartialDecal({ pix + (offsetX * g_engine->fscale),piy + (offsetY * g_engine->fscale) }, pSpriteEmpty, { 0,0 }, { (float)pSpriteFull->sprite->width,(float)pSpriteFull->sprite->height }, { 1 * g_engine->fscale,1 * g_engine->fscale });   //Empty

	if (bhide == true)
		gfx->DrawPartialDecal({ pix + (offsetX * g_engine->fscale),piy + (offsetY * g_engine->fscale) }, pSpriteFull, { 0,0 }, { (float)pSpriteFull->sprite->width,(float)pSpriteFull->sprite->height }, { 1 * g_engine->fscale,1 * g_engine->fscale });   //Fill layer



}

void cRageIndicators::DrawSelf(olc::PixelGameEngine* gfx, int px, int py)
{


		float procent;
		float EnergyDiff;


		switch (m_Classenum)
		{
		case cRageIndicators::RageIndicatorFirst:     // suit for attack in air and on ground 
			procent = 35 / (float)pSpriteFull->sprite->width;
			EnergyDiff = g_engine->GetRage() / procent;


			if (g_engine->GetRage() > 35)
			{
				bhide = 3;

				break;
			}
			bhide = 1; // see filling

			break;
		case cRageIndicators::RageIndicatorSecond:
			procent = 35 / (float)pSpriteFull->sprite->width;
			EnergyDiff = (g_engine->GetRage() - 35) / procent;

			if (g_engine->GetRage() > 70)
			{
				bhide = 3;

				break;
			}
			bhide = 1; // see filling
			break;

		case cRageIndicators::RageIndicatorThird:
			procent = 35 / (float)pSpriteFull->sprite->width;
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
		float piy = py - (2 * pSpriteFull->sprite->height + (py / 64));


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
				gfx->DrawPartialDecal({ pix + offsetX,piy + offsetY }, pSpriteEmpty, { 0,0 }, { (float)pSpriteFull->sprite->width,(float)pSpriteFull->sprite->height }, { 0.4,0.4 });   //Empty

				gfx->DrawPartialDecal({ pix + offsetX,piy + offsetY }, pSpriteFull, { 0,0 }, { EnergyDiff,(float)pSpriteFull->sprite->height }, { 0.4,0.4 });   //Fill layer
			}



			if (pSpriteReady != nullptr && bhide == 3)
			{
				gfx->DrawPartialDecal({ pix + offsetX,piy + offsetY }, pSpriteReady, { 0,0 }, { (float)pSpriteReady->sprite->width,(float)pSpriteReady->sprite->height }, { 0.4,0.4 });   //FillED layer
			}
		}
	



}




cJump::cJump() :cEnergyIndicators("Jump", RPG_Assets::get().GetSprite("JumpFullUi"), RPG_Assets::get().GetSprite("JumpEmptyUi"))
{
	offsetX = 0;
	offsetY = +4*pSpriteFull->sprite->height;
	m_Classenum = JUMP;
	sizeframe = 64;
}






cStaticUi::cStaticUi(std::string name, olc::Decal* Full) : cUI(name, Full)
{
	offsetX = 0;
	offsetY = 0;
}


void  cStaticUi::DrawSelf(olc::PixelGameEngine* gfx, int px, int py)
{


	float pix =	20;
	float piy = 20;



	gfx->DrawPartialDecal({ (pix +offsetX),(piy+offsetY) }, pSpriteFull, { 0,0 }, { (float)pSpriteFull->sprite->width,(float)pSpriteFull->sprite->height });   //offset pulling player back into the screen
	
}

cLevel::cLevel() : cStaticUi("Level Ui", RPG_Assets::get().GetSprite("UiLevelPlace"))
{
	//RPG_Assets::get().GetSprite("UiLevelPlace")->sprite->width
	offsetX = 0;
	offsetY = 0;
}

cSpellPlace::cSpellPlace() : cStaticUi("Spell Place", RPG_Assets::get().GetSprite("SpellUi"))
{
	//RPG_Assets::get().GetSprite("SpellUi")->sprite->height
	offsetX = 0;
	offsetY = 0;
}

cNumber::cNumber() :cStaticUi("Level Number", RPG_Assets::get().GetSprite("font"))
{
	offsetX = 18;
	offsetY = 0;
}

void  cNumber::DrawSelf(olc::PixelGameEngine* gfx, int px, int py)
{
	int i = 0;
	int x = g_engine->GetLvl();
	std::string sText =  std::to_string(x);

	float pix = 20;
	float piy = 20;


	g_engine->DrawBigText(sText, (pix +offsetX), (piy +offsetY), 1,1, olc::YELLOW);

	//gfx->DrawPartialDecal({ ((float)px + offsetX),((float)py + offsetY) }, pSpriteFull, { 0,0 }, { (float)pSpriteFull->sprite->width,(float)pSpriteFull->sprite->height });   //offset pulling player back into the screen

}
