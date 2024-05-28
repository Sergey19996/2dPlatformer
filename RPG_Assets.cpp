#include "RPG_Assets.h"
#include "RPG_Maps.h"
#include "RPG_Items.h"
#include "RPG_UI.h"
#include "RPG_Quests.h"
RPG_Assets::RPG_Assets()
{
}


RPG_Assets::~RPG_Assets()
{
}



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
	load("PressF", "graphs/UI/Font/PressF.png");

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
	load("Pantir's Dagger", "graphs/inventory/Pantir'sDagger.png");

	load("inventory", "graphs/inventory/InventoryMap.png");
	load("inventorySockets", "graphs/inventory/InventoryShelfs.png");

	load("inventoryName", "graphs/inventory/InventoryShelfsName.png");
	load("DescriptionPattern", "graphs/inventory/DescriptionPattern.png");

	load("PantirEquipSlots", "graphs/inventory/PantirEquipSlots.png");

	load("StatisticShelf", "graphs/inventory/StatisticShelf.png");

	load("Coin", "graphs/inventory/Coin.png");

	//Warehouse
	load("WarehouseName", "graphs/inventory/WarehouseShelfsName.png");
	//Store
	load("StoreName", "graphs/inventory/StoreShelfsName.png");

	//BlackSmith
	load("BlackSMithName", "graphs/inventory/BlackSMithShelfsName.png");
	load("RequiredName", "graphs/inventory/RequiredShelfsName.png");
	load("SelectedObject", "graphs/inventory/SelectedObject.png");


	//StreGoods
	load("PocketLowEmpty", "graphs/inventory/PocketLowEmpty.png");
	load("PocketLowFull", "graphs/inventory/PocketLowFull.png");

	//HPbars
	load("HPBarFull", "graphs/UI/UnitBars/HPBarFull.png");
	load("HPBarEmprty", "graphs/UI/UnitBars/HPBarEmpty.png");


	//QuestItems
	load("BrokenSword", "graphs/inventory/BrokenSword.png");


	//Energy bar
	load("EnergyBarFull", "graphs/UI/UnitBars/EnergyBarFull.png");
	load("EnergyBarEmprty", "graphs/UI/UnitBars/EnergyBarEmpty.png");

	//Rage bar
	load("RageBarFull", "graphs/UI/UnitBars/AngryBarFull.png");
	load("RageBarEmprty", "graphs/UI/UnitBars/AngryBarEmpty.png");

	//Expirience Bar
	load("ExperienceFull", "graphs/UI/UnitBars/ExperienceFull.png");
	load("ExperienceEmprty", "graphs/UI/UnitBars/ExperienceEmpty.png");


	//Level place Ui
	load("UiLevelPlace", "graphs/UI/UiLvlPlace.png");


	//Spell Ui
	load("SpellUi", "graphs/UI/SpellUi/SpellUi.png");

	load("AttackEasyFullUi", "graphs/UI/SpellUi/AttackEasyFullUi.png");   
	load("AttackEasyEmptyUi", "graphs/UI/SpellUi/AttackEasyEmptyUi.png");

	load("AttackHighEmptyUi", "graphs/UI/SpellUi/AttackHighEmptyUi.png");
	load("AttackHighFullUi", "graphs/UI/SpellUi/AttackHighFullUi.png");

	load("AttackMidEmptyUi", "graphs/UI/SpellUi/AttackMidEmptyUi.png");
	load("AttackMidFullUi", "graphs/UI/SpellUi/AttackMidFullUi.png");

	load("BackStubEmptyUi", "graphs/UI/SpellUi/BackStubEmptyUi.png");
	load("BackStubFullUi", "graphs/UI/SpellUi/BackStubFullUi.png");

	load("EviscirateDownEmptyUi", "graphs/UI/SpellUi/EviscirateDownEmptyUi.png");
	load("EviscirateDownFull", "graphs/UI/SpellUi/EviscirateDownFull.png");

	load("EviscirateUpEmptyUi", "graphs/UI/SpellUi/EviscirateUpEmptyUi.png");
	load("EviscirateUpFullUi", "graphs/UI/SpellUi/EviscirateUpFullUi.png");

	load("JumpEmptyUi", "graphs/UI/SpellUi/JumpEmptyUi.png");
	load("JumpFullUi", "graphs/UI/SpellUi/JumpFullUi.png");

	load("ShadowStepEmptyUi", "graphs/UI/SpellUi/ShadowStepEmptyUi.png");
	load("ShadowStepFullUi", "graphs/UI/SpellUi/ShadowStepFullUi.png");


	load("VanishEmptyUi", "graphs/UI/SpellUi/VanishEmptyUi.png");
	load("VanishFullUi", "graphs/UI/SpellUi/VanishFullUi.png");

	load("EviscirateFlightEmptyUi", "graphs/UI/SpellUi/EviscirateFlightEmptyUi.png");
	load("EviscirateFlightFullUi", "graphs/UI/SpellUi/EviscirateFlightFullUi.png");


	load("EviscirateFallLandingEmptyUi", "graphs/UI/SpellUi/EviscirateFallLandingEmptyUi.png");
	load("EviscirateFallLandingFullUi", "graphs/UI/SpellUi/EviscirateFallLandingFullUi.png");

	load("GrabSwirlEmptyUi", "graphs/UI/SpellUi/GrabSwirlEmptyUi.png");
	load("GrabSwirlFullUi", "graphs/UI/SpellUi/GrabSwirlFullUi.png");



	//Talent Ui
	load("Talent Plate", "graphs/UI/SpellUi/TalentPlateUi.png");
	
	//Talent Stick
	load("Horizontal StickEmpty", "graphs/UI/SpellUi/Sticks/HorizontalStickEmptyUi.png");
	load("Horizontal StickFill", "graphs/UI/SpellUi/Sticks/HorizontalStickWhiteUi.png");
	load("Vertical StickEmpty", "graphs/UI/SpellUi/Sticks/VerticalStickEmptyUi.png");
	load("Vertical StickFill", "graphs/UI/SpellUi/Sticks/VerticalStickWhiteUi.png");
	


	//New indicator Angry
	load("AngryBarNewEmpty", "graphs/UI/UnitBars/AngryBarNewEmpty.png");
	load("AngryBarNewFill", "graphs/UI/UnitBars/AngryBarNewFill.png");
	load("AngryBarNewReady", "graphs/UI/UnitBars/AngryBarNewReady.png");

	//New indicator Energy
	load("EnergyBarNewFill", "graphs/UI/UnitBars/EnergyBarNewFill.png");
	load("EnergyBarNewReady", "graphs/UI/UnitBars/EnergyBarNewReady.png");
	


	//Enemy

	//Bosses
	
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



	load("CutItem", "graphs/cutscenes/Cutitem.png");


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
	load(new  cWeapon_PantirSword());

	load(new  cWeapon_BrokenSword());  // <--Quest broken sword
	load(new cWeapon_BanditSword());
	load(new cWeapon_BanditBossSword());

	load(new cCoin());
	load(new cSmallWallet());

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
