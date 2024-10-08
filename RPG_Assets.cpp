#include "RPG_Assets.h"
#include "RPG_Maps.h"
#include "RPG_Items.h"
#include "RPG_UI.h"
#include "RPG_Quests.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

RPG_Assets::RPG_Assets()
{
}


RPG_Assets::~RPG_Assets()
{
}

void RPG_Assets::loadSound(const std::string& name, const std::string& filename)
{
	sf::SoundBuffer* buffer = new sf::SoundBuffer;
	if (buffer->loadFromFile(filename)) {
		soundBuffers[name] = buffer;
	}
	else {
		std::cerr << "Error loading sound: " << filename << std::endl;
	}
}


void RPG_Assets::playMusic(const std::string& filename, bool loop)
{
	if (music.openFromFile(filename)) {
		music.setLoop(loop);
		music.setVolume(10);
		music.play();
	}
	else {
		std::cerr << "Error loading music: " << filename << std::endl;
	}
}

void RPG_Assets::SetMusicVolume(float number)
{
	music.setVolume(number);
}



//void RPG_Assets::UpdateSounds()
//{
//	for (auto s: sounds )
//	{
//		if (s.getStatus() == sf::Sound::Stopped) {
//			s.play();  // ���� ���� �� �������������, �������� ���
//		}
//	}
//
//}



void RPG_Assets::LoadSprites()
{
	auto load = [&](std::string sName, std::string sFileName)
	{
		
		olc::Sprite* s = new olc::Sprite(sFileName);
		olc::Decal* d = new olc::Decal(s);
		m_mapSprites[sName] = d;     //map array <key,value>

	
	};

	
	//Font
	load("font", "graphs/UI/Font/font_1.png");

	//Pantir 
	load("PantirRight", "graphs/Pantir/Pantir.png");
	load("PantirLeft", "graphs/Pantir/Pantir_flip.png");

	load("PantirGrabLeft", "graphs/Pantir/PantirGrabLeft.png");
	load("PantirGrabRight", "graphs/Pantir/PantirGrabRight.png");


	load("Pantir's DaggerLeftFx", "graphs/PantirFX/PantirFXLeft.png");
	load("Pantir's DaggerRightFx", "graphs/PantirFX/PantirFXRight.png");

	//Bandit sword
	load("Bandit SwordLeftFx", "graphs/PantirFX/BanditFXLeft.png");
	load("Bandit SwordRightFx", "graphs/PantirFX/BanditFXRight.png");

	load("Bandit ThrowDaggerFx", "graphs/PantirFX/BanditFXRightTest.png");

	//inventory

	load("Items", "graphs/inventory/Items.png");
	load("DescriptionPattern", "graphs/inventory/DescriptionPattern.png");
	load("inventory", "graphs/inventory/InventoryUiBack.png");

	

	//StreGoods

	load("FullUi", "graphs/UI/UiPack.png");

	
	//BanditBoss
	load("BanditBossLeft", "graphs/BanditBoss/BanditBossLeft.png");
	load("BanditBossRight", "graphs/BanditBoss/BanditBossRight.png");    //Must change



	//1 Bandit
	load("BanditLeft", "graphs/Bandit/BanditLeft.png");
	load("BanditRight", "graphs/Bandit/BanditRight.png");    //Must change


	//2 Wolf
	load("DireWolfLeft", "graphs/Wolf/direwolf_Left2.png");
	load("DireWolfRight", "graphs/Wolf/direwolf_Right2.png");    //Must change

	//2 WereWOlf
	load("WereWolfLeft", "graphs/Werewolf/WereWolfLeft.png");
	load("WereWolfRight", "graphs/Werewolf/WereWolfRight.png");    //Must change


	// 3 Boar
	load("BoarLeft", "graphs/Boar/BoarLeft.png");
	load("BoarRight", "graphs/Boar/BoarRight.png");    //Must change


	load("ForestCloud", "graphs/Enviroment/Cloud.png");


	//Forest lvl  spritesheet location
	load("BlockFirstLvl", "graphs/FirstLvl/ForestLvl1Constructor.png");


	//Forest lvl  spritesheet location Part2 2
	load("ForesttLvlPartTwo", "graphs/FirstLvl/ForestLvl1ConstructorPart2.png");

	load("ForestDynamicLayer", "graphs/FirstLvl/ForestDynamicLayer.png");
	//Village lvl  spritesheet location
	load("BlockVillageLvl", "graphs/FirstLvl/VillageLvl1Constructor.png");
	//Tavern lvl  spritesheet location
	load("BlockTavernLvl", "graphs/FirstLvl/TavernLvl1Constructor.png");



//	load("CutItem", "graphs/cutscenes/Cutitem.png");


	//Parralax
	//Forest
	load("ParralaxForestClose","graphs/ParalaxLvl/LvLone/Forest/ForestClose.png");
	load("ParralaxForestMid", "graphs/ParalaxLvl/LvLone/Forest/ForestMid.png");
	load("ParralaxForestFar", "graphs/ParalaxLvl/LvLone/Forest/ForestFar.png");


	//VillageinFire
	load("ParralaxVillageInFireClose", "graphs/ParalaxLvl/LvLone/Village/VillageInFireClose.png");
	load("ParralaxVillageInFireMid", "graphs/ParalaxLvl/LvLone/Village/VillageInFireMid.png");
	load("ParralaxVillageInFireFar", "graphs/ParalaxLvl/LvLone/Village/VillageInFireFar.png");

	//Villagei
	load("ParralaxVillageClose", "graphs/ParalaxLvl/LvLone/Village/VillageClose.png");
	load("ParralaxVillageMid", "graphs/ParalaxLvl/LvLone/Village/VillageMid.png");
	load("ParralaxVillageFar", "graphs/ParalaxLvl/LvLone/Village/VillageFar.png");


	//Mountains
	load("ParralaxMountainsClose", "graphs/ParalaxLvl/LvLone/Mountains/MountainsClose.png");
	load("ParralaxMountainsMid", "graphs/ParalaxLvl/LvLone/Mountains/MountainsMid.png");
	load("ParralaxMountainsFar", "graphs/ParalaxLvl/LvLone/Mountains/MountainsFar.png");


	//MainMenu
	load("MainMenuBardChar", "graphs/UI/MainMenu/MainMenuBardChar.png");
	load("MainMenuCampFire", "graphs/UI/MainMenu/MainMenuCampFire.png");
	load("MainMenuCloseLayer", "graphs/UI/MainMenu/MainMenuCloseLayer.png");

	load("MainMenuCloud", "graphs/UI/MainMenu/MainMenuCloud.png");
	load("MainMenuFarLayer", "graphs/UI/MainMenu/MainMenuFarLayer.png");
	load("MainMenuFirstChar", "graphs/UI/MainMenu/MainMenuFirstChar.png");

	load("MainMenuMidLayer", "graphs/UI/MainMenu/MainMenuMidLayer.png");
	load("MainMenuSecondChar", "graphs/UI/MainMenu/MainMenuSecondChar.png");
	load("MainMenuWhisps", "graphs/UI/MainMenu/MainMenuWhisps.png");

	//Map stuff
	load("MapLayer", "graphs/UI/Map/MapLayer0.png");

	//MapVillage   
	load("VillageForestScetch", "graphs/UI/Map/VillageForest.png");
	load("VillageForestPaint", "graphs/UI/Map/VillageForestPaint.png");

	//FVX

	load("RainVfx", "graphs/Fvx/RaindDropFx.png");
	load("LevelVfx", "graphs/Fvx/LevelParticleFx.png");
	load("SoulShotVfx", "graphs/Fvx/SoulShotFx.png");

	//MapDesert
	load("DesertValeScetch", "graphs/UI/Map/DesertValeScetch.png");
	load("DesertValePaint", "graphs/UI/Map/DesertValePaint.png");
	//MapGateofHorseman
	load("HorseManGateScetch", "graphs/UI/Map/HorseManGateScetch.png");
	load("HorseManGatePaint", "graphs/UI/Map/HorseManGatePaint.png");

	//MapMountains
	load("MountainsScetch", "graphs/UI/Map/MountainsScetch.png");
	load("MountainsPaint", "graphs/UI/Map/MountainsPaint.png");

	//MapMonastery
	load("MonasteryScetch", "graphs/UI/Map/MonasteryScetch.png");
	load("MonasteryPaint", "graphs/UI/Map/MonasteryPaint.png");



	//MapLock
	load("Lock", "graphs/UI/Map/Lock.png");
	load("SkullCaveScetch", "graphs/UI/Map/SkullCaveScetch.png");
	load("ElvishGraveyardScetch", "graphs/UI/Map/ElvishGraveyardScetch.png");
	load("SwampOfWitchScetch", "graphs/UI/Map/SwampOfWitchScetch.png");



	//	load("Test", "graphs/Test.png");
	
}

void RPG_Assets::LoadMaps()
{
	auto load = [&](cMap* m)
	{
		m_mapMaps[m->sName] = m;
	};

	load(new cMap_Forest());
	load(new cMap_VillageInFire());
	load(new cMap_Village());
	load(new cMap_VillageTavern());
	load(new cMap_Mountains());
	load(new cMap_ForestPartTwo());

}

void RPG_Assets::LoadItems()
{
	auto load = [&](cItem* i)
	{
		m_mapItems[i->sName] = i;
	};
//	load(new  cWeapon_PantirSword());

	load(new cEmptySlot());

	load(new cEquip("Basic Neck", RPG_Assets::get().GetSprite("Items"), "General Neck\n", 0, 0, 0, 0, 0, 35, 1, 2));
	load(new cEquip("Basic Helment", RPG_Assets::get().GetSprite("Items"), "General Helment\n", 0, 0, 0, 0, 0, 32, 2, 2));
	load(new cEquip("Basic Chest", RPG_Assets::get().GetSprite("Items"), "General Chest\n", 0, 0, 0, 0, 0, 33, 4, 3));
	load(new cEquip("Basic Back", RPG_Assets::get().GetSprite("Items"), "General Back\n", 0, 0, 0, 0, 0, 32, 5, 1));
	load(new cEquip("Basic Boots", RPG_Assets::get().GetSprite("Items"), "General Boots\n +15 Strength\n", 0, 0, 0, 0, 0, 34, 6, 1));

//	load(new  cBaseBack());
//	load(new  cBaseChest());
//	load(new  cBaseHelmet());
//	load(new  cBaseNeck());
//	load(new  cBaseBoots());


	load(new cWeapon("Pantir's Dagger", RPG_Assets::get().GetSprite("Items"), RPG_Assets::get().GetSprite("Pantir's DaggerLeftFx"), RPG_Assets::get().GetSprite("Pantir's DaggerRightFx"), "+5 Dmg        \n", 5, 0, 0, 0, 0, 64, 3,1));
	load(new cWeapon("Broken Sword", RPG_Assets::get().GetSprite("Items"), RPG_Assets::get().GetSprite("Pantir's DaggerLeftFx"), RPG_Assets::get().GetSprite("Pantir's DaggerRightFx"), "it seems the sword was\nbroken quite a long time\nago\n", 10, 0, 0, 0, 0, 67, 3,1));
	load(new cWeapon("Bandit Sword", RPG_Assets::get().GetSprite("Items"), RPG_Assets::get().GetSprite("Bandit SwordLeftFx"), RPG_Assets::get().GetSprite("Bandit SwordRightFx"), "usually these kind of swords\nare used by bandits", 8, 0, 0, 0, 0,67,3,1));
	load(new cWeapon("Bandit Boss Sword", RPG_Assets::get().GetSprite("Items"), RPG_Assets::get().GetSprite("Bandit SwordLeftFx"), RPG_Assets::get().GetSprite("Bandit SwordRightFx"), "usually these kind of swords \nare used by bandits\n", 10, 0, 0, 0, 0,67,3,3));

//	load(new  cWeapon_BrokenSword());  // <--Quest broken sword
//	load(new cWeapon_BanditSword());
//	load(new cWeapon_BanditBossSword());

	load(new cCoin());
	load(new cSmallWallet());
	load(new cFullSmallWallet());

	load(new cEnergyElixir());
	load(new cHealthElixir());
	load(new cRageElixir());

}


void RPG_Assets::LoadUiElements()
{
	auto load = [&](cUI* z)
	{
		m_mapUiElements[z->sName] = z;
	};

	//
	load(new cExperience());
	load(new cLevel());
	load(new cNumber());
	//


	load(new cRightAttackAir());
	load(new cRightAttack());
	load(new cRightFlightUp());
	load(new cRightEpicLanding());


	load(new cAttacBack());
	load(new cSwirlAttack());


	load(new cAttackHigh());
	load(new cAttackLow());
	load(new cAttackMid());

	load(new cVanish());

	load(new cStepBack());

	load(new cNewEnergyIndicator( 0, 33, -86, "_1"));   //Easy indicator
	load(new cNewEnergyIndicator(1, 50 - (0.6 * 50)+33,-86,"_2"));  // Mid
	load(new cNewEnergyIndicator(2, 2 * (50 - (0.6 * 50))+33,-86,"_3")); //Hard

	load(new cNewRageIndicator(0, 33, -26, "_1"));   //Easy indicator
	load(new cNewRageIndicator(1, 50 - (0.6 * 50) + 33, -26, "_2"));  // Mid
	load(new cNewRageIndicator(2, 2 * (50 - (0.6 * 50)) + 33,  -26, "_3")); //Hard


	//load(new cSpellPlace());


//	load(new cHp());
}

void RPG_Assets::LoadQuests()
{
	auto load = [&](cQuest* z)
	{
		m_mapQuests[z->SaveSlot] = z;
	};


	load(new cQuset_MainQuest);
	load(new cQuset_FirstBandit);
	load(new cQuset_KeyPointsInVillage);
	load(new cQuset_KillBanditBoss);
	load(new cQuset_KillWerewolf);
	
}

void RPG_Assets::LoadMusc()
{
	// �������� �������� ��������
	loadSound("TakeDamage", "Sounds/TakeDamageSound.wav");

	loadSound("SwordSwing", "Sounds/HitSound.wav");

	loadSound("SwordSwingTwo", "Sounds/HitSound2.wav");

	loadSound("Jump", "Sounds/JumpSound.wav");
	
	loadSound("Landed", "Sounds/LandedSound.wav");

	loadSound("Run", "Sounds/RunSound.wav");

	loadSound("BackStab", "Sounds/HitLandedSound.wav");




}
