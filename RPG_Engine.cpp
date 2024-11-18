#include "RPG_Engine.h"

#define DEBUG_MODE  // Включить режим отладки


RPG_Engine::RPG_Engine()
{
    sAppName = "Pantrir platformer";
}

cDynamic* RPG_Engine::SpawnBoss(BossStruct name)
{

	BossAr[static_cast<int>(name)]->setFlag(BossAr[static_cast<int>(name)]->binitialized);    // устанавилваем флаг
	m_vecDynamics.push_back(BossAr[static_cast<int>(name)]);     // добавляем в сцену 



	switch (name)
	{
	case RPG_Engine::BossStruct::BanditBoss:
	{
		for (auto i = BanditBossPool.begin(); i != BanditBossPool.end(); ++i)    //удаляем его с пула *когда босс умрет, должен вернуться в массив 
		{
			cDynamic* it = *i;
			if (it == BossAr[static_cast<int>(name)])
			{
				*i = nullptr;                  // Удаляем заспаунинного бандита из пула
				break;
			}
		}
	}
		break;
	default:
		break;
	}

	return BossAr[static_cast<int>(name)];  //возвращаем указатель 
}

cDynamic* RPG_Engine::GetBoss(BossStruct name)
{
		return BossAr[static_cast<int>(name)];
}

void RPG_Engine::setUiCurrSpell(DataStruct Name, bool toggle)
{


	const SpriteData& data = GetSpriteData(Name);


	switch (toggle)
	{
	case 0:
		SpellsUiAr[0] = &data; // get data from CurrEnergy


		break;
		
	case 1:
		SpellsUiAr[1] = &data; // get data from CurrEnergy
		break;

	
	};

	
	//SpellsUiAr[1]
};


 cDynamic_creature_NPC* RPG_Engine::GetNpc(NpcStruct name)
{
	return NPC_Ar[static_cast<int>(name)];
}

void RPG_Engine::DrawElement(DataStruct ds, olc::vf2d position, float scale, olc::Decal* Decal)
{

	const auto& data = GetSpriteData(ds);  // возвращает элемент согласно DataStruct

	DrawPartialDecal(
		{ position.x * CellSize, position.y * CellSize },
		Decal,
		{ (float)data.Pos.x, (float)data.Pos.y },
		{ (float)data.Size.x, (float)data.Size.y },
		{ scale, scale }
	);

}

void RPG_Engine::Drawcursor(olc::vi2d mouse)
{
	const float x = 64 % 32;
	const float y = 64 / 32;
	//Mouse
	DrawPartialDecal({ (float)(mouse.x - 0.15f) , (float)(mouse.y - 0.1f) }, D_Items, { x * 64,y * 64 }, { (float)64,(float)(64) }, { (float)0.5*fscale,(float)0.5*fscale });
}


void RPG_Engine::LoadBaseUiSettings()
{
	
	/// 
	

	//



	AddUi((RPG_Assets::get().GetUiElements("Experience Indicator")));
	AddUi((RPG_Assets::get().GetUiElements("Level Ui")));
	AddUi((RPG_Assets::get().GetUiElements("Level Number")));
	//AddUi((RPG_Assets::get().GetUiElements("Spell Place")));


	//Talents  in book of proffesion
	AddTalentUi((RPG_Assets::get().GetUiElements("Attack Back")));

	AddTalentUi((RPG_Assets::get().GetUiElements("Swirl Attack")));

	AddTalentUi((RPG_Assets::get().GetUiElements("Attack Low")));
	AddTalentUi((RPG_Assets::get().GetUiElements("Attack Mid")));
	AddTalentUi((RPG_Assets::get().GetUiElements("Attack High")));
	AddTalentUi((RPG_Assets::get().GetUiElements("Appear Behind")));

	AddTalentUi((RPG_Assets::get().GetUiElements("Attack Right")));
	AddTalentUi((RPG_Assets::get().GetUiElements("Attack Right Air")));

	AddTalentUi((RPG_Assets::get().GetUiElements("Attack Right Flight Up")));

	AddTalentUi((RPG_Assets::get().GetUiElements("Attack Right Epic Landing")));

	AddTalentUi((RPG_Assets::get().GetUiElements("Vanish")));




	//Talent which saved   standart pack low attack and right mouse
	SetLearnedTalent((RPG_Assets::get().GetUiElements("Attack Low")));   // locks for attack
	SetLearnedTalent((RPG_Assets::get().GetUiElements("Attack Mid")));   // locks for attack
	SetLearnedTalent((RPG_Assets::get().GetUiElements("Attack High")));   // locks for attack
	SetLearnedTalent((RPG_Assets::get().GetUiElements("Attack Right")));

	//SetLearnedTalent((RPG_Assets::get().GetUiElements("Attack Right Flight Up")));   // locks for attack
	SetLearnedTalent((RPG_Assets::get().GetUiElements("Attack Right Air")));

	SetLearnedTalent((RPG_Assets::get().GetUiElements("Attack Back")));

	SetLearnedTalent((RPG_Assets::get().GetUiElements("Vanish")));

	//


	//                  THis is when we add in vector which we drawning ing game like backstub 
//	AddUi((RPG_Assets::get().GetUiElements("Attack Back")));
//	AddUi((RPG_Assets::get().GetUiElements("Appear Behind")));

	//AddUi((RPG_Assets::get().GetUiElements("Swirl Attack")));


	AddUi((RPG_Assets::get().GetUiElements("NewEnergyIndicator_1")));
	AddUi((RPG_Assets::get().GetUiElements("NewEnergyIndicator_2")));
	AddUi((RPG_Assets::get().GetUiElements("NewEnergyIndicator_3")));

	AddUi((RPG_Assets::get().GetUiElements("NewRageIndicator_1")));
	AddUi((RPG_Assets::get().GetUiElements("NewRageIndicator_2")));
	AddUi((RPG_Assets::get().GetUiElements("NewRageIndicator_3")));

//	AddUi((RPG_Assets::get().GetUiElements("Attack Low")));   // locks for attack
//	AddUi((RPG_Assets::get().GetUiElements("Attack High")));   // locks for attack
//	AddUi((RPG_Assets::get().GetUiElements("Attack Mid")));   // locks for attack

	//

	m_vecEquip.resize(6);
	m_vecVisibleDynamics.resize(30);

	for (size_t i = 0; i < 6; i++)
	{
		InventaryItem* empty = new InventaryItem();
		empty->index = i;
		//empty->bEquiped = true;
		empty->setFlag(empty->bEquiped);
		//empty->Item = (cEquip*)RPG_Assets::get().GetItem("Empty");
		m_vecEquip[i]=empty;

	}
	// Bind Vector with pointesr in char





	for (size_t i = 0; i < 2; i++)
	{
		InventaryItem* empty = nullptr;
		//empty->index = i;
		m_vecUi.push_back(empty);

	}


	for (int i = 0; i < 24; i++)
	{
		InventaryItem* empty6 = new InventaryItem();
		empty6->index = i;

		m_listItems.push_back(empty6);
	
		InventaryItem* empty7 = new InventaryItem();
		empty7->index = i;
		//empty->bInWarehouse = 1;
		empty7->setFlag(empty7->binWarehouse);

		m_listWarehouseItems.push_back(empty7);
	
	
		InventaryItem* empty8= new InventaryItem();
		empty8->index = i;
		//empty->bInWarehouse = 1;

		m_listBlackSmithItems.push_back(empty8);
	
	
		InventaryItem* empty9 = new InventaryItem();
		empty9->index = i;
		//empty->bInWarehouse = 1;

		m_listStoreItems.push_back(empty9);
	

	}
}

bool RPG_Engine::OnUserCreate()
{
	hWnd = GetConsoleWindow();
	CellSize = 64 * fscale;

	SetMouseFocus(true);
	cDynamic::g_engine = this;
	cItem::g_engine = this;
	cUI::g_engine = this;
	Environment::g_engine = this;
	
	cQuest::g_script = &m_script;
	cQuest::g_engine = this;
	cComand::g_engine = this;

	cMap::g_script = &m_script;
	cMap::g_engine = this;
	PerlinNoise::g_engine = this;


    RPG_Assets::get().LoadSprites();               //Load sprites
	RPG_Assets::get().LoadItems();

	RPG_Assets::get().LoadAllCache();
	RPG_Assets::get().ScaleCache((*RPG_Assets::get().GetCache("SprayFx")), 1.0f / 20.0f, 10);
//	RPG_Assets::get().ScaleCache((*RPG_Assets::get().GetCache("RainFx")), 1.0f / 20.0f,100);


	LoadenemyInstances();    // Load All Pools

	RPG_Assets::get().LoadMaps();
	RPG_Assets::get().LoadUiElements();
	RPG_Assets::get().LoadQuests();
	RPG_Assets::get().LoadMusc();


	LoadSoundPool(20);

	m_sprFont = RPG_Assets::get().GetSprite("font");   //take font


	
	m_listQusets.push_front(RPG_Assets::get().GetActiveQuest(0)); // add in lust First Main quest

	LoadBaseUiSettings();

    m_pPlayer = new cDynamic_creature_Pantir();   //create char 


	m_vecEquip[0]->Item = m_pPlayer->pEquipedNeck;  // if in m_pPlayer pointers have some objects, we show them and let manipulate like change tham on other
	m_vecEquip[1]->Item = m_pPlayer->pEquipedHelmet;
	m_vecEquip[2]->Item = m_pPlayer->pEquipedWeapon;
	m_vecEquip[3]->Item = m_pPlayer->pEquipedChest;
	m_vecEquip[4]->Item = m_pPlayer->pEquipedBack;
	m_vecEquip[5]->Item = m_pPlayer->pEquipedBoots;
	

	
	GiveItem("Pantir's Dagger",1);
	GiveItem("Broken Sword",1);
//	GiveItem("Coin");
	//GiveItem("Coin");
	GiveItem("SmallWallet",1);
	//GiveItem("Energy Elixir");
	GiveItem("Rage Elixir",3);
	GiveItem("Health Elixir",3);
	GiveItem("Energy Elixir",3);
	//GiveItem("Rage Elixir");
	//GiveItem("Health Elixir");
	//GiveItem("Energy Elixir");
	//GiveItem("Rage Elixir");
	//GiveItem("Health Elixir");

	

	

//	PerlinNoise noise(64, 64, 5);

	
	nVisibleTileX = ScreenWidth() / CellSize;
	nVisibleTileY = ScreenHeight() / CellSize;
	batchZeroCharsLayer.reserve((nVisibleTileX + 8) * (nVisibleTileY + 8));
	//AddUi((RPG_Assets::get().GetUiElements("Jump")));


	
	//m_vecHpBars.push_back(RPG_Assets::get().GetIndicators("HpFirst"));

	olc::vi2d MainUiBlockPos = { 0,(ScreenHeight() -  static_cast<int>(4*CellSize*fscale)) };
	int UiButtonsY = ScreenHeight() - (1 * CellSize * fscale);
	int fixCellSize = CellSize * fscale;

	UIPosOnScreenAr =             //Ui pos on screen 
	{
		olc::vi2d{MainUiBlockPos},      //MainBlock UI Pos
		olc::vi2d{ 14 * fixCellSize,UiButtonsY},      //Pos auest 
		olc::vi2d{ 19 * fixCellSize,UiButtonsY },      //inv 
		olc::vi2d{ 25 * fixCellSize,UiButtonsY },       //Talent 
		// For drowing

			olc::vi2d{ MainUiBlockPos.x+72,MainUiBlockPos.y+40 },      //Energy Active Spell
			olc::vi2d{ MainUiBlockPos.x+120,MainUiBlockPos.y+40 },      //Rage  Active Spell

			olc::vi2d{ MainUiBlockPos.x+96,MainUiBlockPos.y+35 },      //Energy UI Indicator
			olc::vi2d{ MainUiBlockPos.x+147,MainUiBlockPos.y+35 },       //Rage UI Indicator
			


			olc::vi2d{ MainUiBlockPos.x + 14,MainUiBlockPos.y + 14 },      //First Ui Fast available slot
			olc::vi2d{ MainUiBlockPos.x + 14,MainUiBlockPos.y + 33 },      //Second UI Fast available slot
	};

	// SpawnBossBandt({ 0,0 });

	

#ifdef  DEBUG_MODE
	layer = CreateLayer();
	layerOne = CreateLayer();
	layerTwo = CreateLayer();
	
#endif // DEBUG




//	m_sprPressF = RPG_Assets::get().GetSprite("PressF");
	D_Items = RPG_Assets::get().GetSprite("Items");
	D_Inventory = RPG_Assets::get().GetSprite("inventory");
	D_FullUi = RPG_Assets::get().GetSprite("FullUi");
	D_Map = RPG_Assets::get().GetSprite("MapLayer");
	D_FX = RPG_Assets::get().GetSprite("VFX");
	
	m_vecCloseWeather_2.reserve(100);
	

	RPG_Assets::get().playMusic("Sounds/MainMenuMusic.wav");

    return true;
}

bool RPG_Engine::OnUserUpdate(float fElapsedTime)
{
	bool Result = true;
	
	//std::cout << "FPS: " << test <<"Thread ID: " << std::this_thread::get_id()<< std::endl;


	// В начало функции OnUserUpdate добавьте:
//	std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;

	

	removeStoppedSounds();

//	auto First = std::chrono::steady_clock::now();


	// Perform game logic updates
	switch (n_nGameMode) {
	case MODE_TITLE:
		Result = UpdateTitleScreen(fElapsedTime);
		break;
	case MODE_LOCAL_MAP:
	case MODE_INVENTORY:
	case MODE_MAP:
	case MODE_WAREHOUSE:
	case MODE_SHOP:
	case MODE_PROFESSION:
	case MODE_BLACKSMITH:
	case MODE_QUESTLOG:
		Result = UpdateLocalMap(fElapsedTime);
		break;
	}
	



	
	// Display FPS (for debugging)
	DrawString(ScreenWidth() - 100, 0, std::to_string(GetFPS()), olc::YELLOW, 3);

//	std::cout << fElapsedTime << " /n " << fSleepTime << std::endl;

	return Result;
}

void RPG_Engine::LoadSoundPool(size_t poolSize)
{
	for (size_t i = 0; i < poolSize; ++i) {

		sf::Sound* sound = new sf::Sound();
		m_vecSoundsPool.push_back(sound);
	}
}

void RPG_Engine::PlaySounds(std::string buffername)
{
	if (!m_vecSoundsPool.empty()) {
		// Берём первый доступный звук из пула
		sf::Sound* sound2 = m_vecSoundsPool.back();
		m_vecSoundsPool.pop_back();  // Убираем его из пула

		sf::SoundBuffer* buf = RPG_Assets::get().findSound(buffername);

		sound2->setBuffer(*buf);
		sound2->setVolume(50);  // Устанавливаем громкость по необходимости
		sound2->play();
		sounds.push_back(sound2);
	}

}


void RPG_Engine::UpdateAnimationFrame(float fElapsedTime)   // use for animate some in distance 8 frames 
{

	animsprite += fElapsedTime;
	// Logic For Dynamic Layer
	if (animsprite > framespeed)
	{
		frameIndex++;
		animsprite = 0;

	}
	if (frameIndex >= 8)
	{
		frameIndex = 0;
	}
}

bool RPG_Engine::UpdateTitleScreen(float fElapsedTime)   // <---MAIN MENU Start
{
	Clear(olc::BLANK);

#ifdef  DEBUG_MODE

	SetDrawTarget(layer);
	SetPixelMode(olc::Pixel::ALPHA);
	SetPixelMode(olc::Pixel::MASK);
#endif // DEBUG


	//Update script 
	m_script.ProcessCommands(fElapsedTime);

	olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };
	DrawPartialDecal({ (float)0, (float)0 }, RPG_Assets::get().GetSprite("MainMenuFarLayer"), { 0,0 }, { (float)RPG_Assets::get().GetSprite("MainMenuFarLayer")->sprite->width, (float)RPG_Assets::get().GetSprite("MainMenuFarLayer")->sprite->height }, { 1, 1 });   //<-- Draw sprite under rectangle
	DrawPartialDecal({ (float)0, (float)0 }, RPG_Assets::get().GetSprite("MainMenuMidLayer"), { 0,0 }, { (float)RPG_Assets::get().GetSprite("MainMenuMidLayer")->sprite->width,  (float)RPG_Assets::get().GetSprite("MainMenuMidLayer")->sprite->height }, { 1,1 });
	DrawPartialDecal({ (float)0, (float)ScreenHeight() - (float)RPG_Assets::get().GetSprite("MainMenuCloseLayer")->sprite->height }, RPG_Assets::get().GetSprite("MainMenuCloseLayer"), { 0,0 }, { (float)RPG_Assets::get().GetSprite("MainMenuCloseLayer")->sprite->width, (float)RPG_Assets::get().GetSprite("MainMenuCloseLayer")->sprite->height }, { 1,1 });

	MainMenuAnim += fElapsedTime;
	if (MainMenuAnim > 0.1f)
	{
		MainMenuAnim -= MainMenuAnim;
		MainMenuCalc++;

	}

	if (MainMenuCalc > 10)
	{
		MainMenuCalc = 0;
	}
	MainMenuX = MainMenuCalc % 6;
	MainMenuY = MainMenuCalc / 6;


	DrawPartialDecal({ ScreenWidth() / 2.0f + (75 * fscale), ScreenHeight() / 2.0f - 30 }, RPG_Assets::get().GetSprite("MainMenuBardChar"), { MainMenuX * 64.0f,MainMenuY * 64.0f }, { (float)64, (float)64 }, { (float)1.4,(float)1.4 });



	DrawPartialDecal({ ScreenWidth() - (250.0f * fscale), ScreenHeight() / 2.0f }, RPG_Assets::get().GetSprite("MainMenuFirstChar"), { MainMenuX * 64.0f,MainMenuY * 64.0f }, { (float)64, (float)64 }, { (float)1.4,(float)1.4 });


	DrawPartialDecal({ ScreenWidth() - (750.0f * fscale), ScreenHeight() / 2.0f - (30 * fscale) }, RPG_Assets::get().GetSprite("MainMenuSecondChar"), { MainMenuX * 64.0f,MainMenuY * 64.0f }, { (float)64, (float)64 }, { (float)1.4,(float)1.4 });

	DrawPartialDecal({ ScreenWidth() - (158.0f * fscale), ScreenHeight() / 2.0f - (200 * fscale) }, RPG_Assets::get().GetSprite("MainMenuWhisps"), { MainMenuX * (40.0f * fscale),MainMenuY * (13.0f * fscale) }, { (float)40 * fscale, (float)13 * fscale }, { (float)1.4,(float)1.4 });

	DrawPartialDecal({ ScreenWidth() / 2 + (35.0f * fscale), ScreenHeight() / 2 + (55.0f * fscale) }, RPG_Assets::get().GetSprite("MainMenuCampFire"), { MainMenuX * (76.0f * fscale),MainMenuY * (94.5f * fscale) }, { (float)(76 * fscale), (float)94 * fscale }, { (float)1.4,(float)1.4 });



	float spelluix, spelluiy;
	spelluix = 25.0f;
	spelluiy = ScreenHeight() - (150 * fscale);

	DrawPartialDecal({ spelluix, spelluiy }, D_FullUi, { 832,0 }, { 334,142 }, { (float)0.7 * fscale,(float)1 * fscale });  // spell Ui
	//DrawBigText("Travel ?", (ScreenWidth() / 2) - 72, ScreenHeight() / 2 - 62, 1, 1, olc::WHITE);

	//DrawBigText("Continue", 50, ScreenHeight() - 250, 1.5, 1.5, olc::GREY);
	DrawBigText("New Game", spelluix + (25 * fscale), spelluiy + (30 * fscale), 1 * fscale, 1 * fscale, olc::WHITE);

	DrawBigText("Options", spelluix + (25 * fscale), spelluiy + (60 * fscale), 1 * fscale, 1 * fscale, olc::WHITE);

	DrawBigText("Quit", spelluix + (25 * fscale), spelluiy + (90 * fscale), 1 * fscale, 1 * fscale, olc::WHITE);



	std::ofstream data;
	data.open("Load/CurrSave.bin", std::ofstream::in);
	if (data.is_open())
	{
		DrawBigText("Continue", spelluix + (25 * fscale), spelluiy, 1 * fscale, 1 * fscale, olc::WHITE);
	}
	else
	{
		DrawBigText("Continue", spelluix + (25 * fscale), spelluiy, 1 * fscale, 1 * fscale, olc::GREY);
	}

	if (GetMouse(0).bPressed) //LeftMouse
	{

		if ((int)mouse.x >= spelluix + (25 * fscale) && (int)mouse.y >= spelluiy + (30 * fscale) && (int)mouse.x <= spelluix + (233 * fscale) && (int)mouse.y <= spelluiy + (59 * fscale)) //New Game
		{



			data.close();
			m_script.AddCommand((new cComand_HideScreen(2)));
			m_script.AddCommand((new cComand_Changemap("Forest", -1, 25.5)));

			//m_script.AddCommand((new cComand_Changemap("ForestPartTwo", 5, 21.5)));
			//m_script.AddCommand(new cComand_Changemap("VillageTavern", 12, 9));

			//m_script.AddCommand((new cComand_Changemap("Forest", 249.0f, 54.5f))); //<--- Teleport in cave main 

			//vecDyns.push_back(new cDynamic_Teleport(97.0f, 58.0f, "Forest", 96.0f, 40.5f)); //<--- Teleport in cave main 

			m_script.AddCommand((new cComand_SetNgameMod(1)));

			std::string filename = "Load/CurrSave.txt";

			// Create a file (you might have code for saving the game here)

			// Delete the file
			if (std::remove(filename.c_str()) != 0) {
				perror("Error deleting file");
			}
			else {
				std::cout << "File successfully deleted" << std::endl;
			}



			//ChangeMap("Forest", -1, 24);
		//	fCameraPosX = 8;
		//	fCameraPosY = 6;

		

			//	n_nGameMode = MODE_LOCAL_MAP;
		}

		if ((int)mouse.x >= spelluix + (25 * fscale) && (int)mouse.y >= spelluiy + (90 * fscale) && (int)mouse.x <= (250 * fscale) && (int)mouse.y <= spelluiy + (120 * fscale)) //New Game
		{
			return false;
		}

		if ((int)mouse.x >= spelluix + (25 * fscale) && (int)mouse.y >= spelluiy && (int)mouse.x <= (266 * fscale) && (int)mouse.y <= spelluiy + (29 * fscale)) //Continue
		{
			if (data.is_open())
			{
				data.close();
				//		m_script.AddCommand((new cComand_HideScreen(2)));
				m_script.AddCommand((new cComand_LoadFunction));  // in load function in start allredy 2 comands but they call not instant 



			}




		}
	}
	if ((int)mouse.x >= spelluix + (25 * fscale) && (int)mouse.y >= spelluiy && (int)mouse.x <= (266 * fscale) && (int)mouse.y <= spelluiy + (29 * fscale)) //Continue
	{
		DrawBigText("Continue", spelluix + (25 * fscale), spelluiy, fscale * 1, fscale * 1, olc::YELLOW);
	}

	if ((int)mouse.x >= spelluix + (25 * fscale) && (int)mouse.y >= spelluiy + (30 * fscale) && (int)mouse.x <= spelluix + (233 * fscale) && (int)mouse.y <= spelluiy + (59 * fscale)) //New Game
	{
		DrawBigText("New Game", spelluix + (25 * fscale), spelluiy + (30 * fscale), fscale * 1, fscale * 1, olc::YELLOW);
	}

	if ((int)mouse.x >= spelluix + (25 * fscale) && (int)mouse.y >= spelluiy + (90 * fscale) && (int)mouse.x <= (250 * fscale) && (int)mouse.y <= spelluiy + (120 * fscale)) //New Game
	{
		DrawBigText("Quit", spelluix + (25 * fscale), spelluiy + (90 * fscale), fscale * 1, fscale * 1, olc::YELLOW);
	}



	Drawcursor(mouse);






	if (bSmoothAppearScreen)
	{

		DrawDecal({ (float)0, (float)0 }, RPG_Assets::get().GetSprite("DescriptionPattern"), { 2 * fscale,2 * fscale }, olc::Pixel{ 255,255,255,(uint8_t)AlphaAppearScreen });
	}




	//	void clockwiseMask(float felapsedtime, olc::Sprite* test)


	//	 sinusoidMask(fElapsedTime, test, 32-16);

		//Get offsets for smooth movement
	float fTileOffsetX = mouse.x / 32-(int)mouse.x/32;
	float fTileOffsetY = mouse.x / 32-(int)mouse.y/32;


#ifdef  DEBUG_MODE

	EnableLayer(layer, true);
	SetDrawTarget(nullptr);
#endif // DEBUG






		
	return true;     
}     
// <---MAIN MENU END



bool RPG_Engine::UpdateLocalMap(float fElapsedTime)
{
	

//	RPG_Assets::get().removeStoppedSounds();


	
	olc::vi2d mouse = { (GetMouseX()), (GetMouseY()) };


	//std::cout << mouse.x << "\t" << mouse.y << std::endl;

		if (m_pPlayer->checkFlag(m_pPlayer->bDead))
		{
		//	SaveFunction();
			m_pPlayer->clearFlag(m_pPlayer->bDead);
			m_pPlayer->clearFlag(m_pPlayer->bDraw);
			m_pPlayer->nHealth = 1;
		//	m_pPlayer->SwitchLayer(2);
			m_script.AddCommand((new cComand_HideScreen(2)));
			m_script.AddCommand((new  cComand_LoadFunction));

	
		}

		//Update script 
		m_script.ProcessCommands(fElapsedTime);

	
		m_vecCloseWeather_2.erase(
			remove_if(m_vecCloseWeather_2.begin(), m_vecCloseWeather_2.end(),
				[&](Environment* d) {
		if (((Environment*)d)->redundant){
		ReturnEnvironmentToPool(d);
		return true;  // Удалить указатель из контейнера
		}
		return false;
				}),
			m_vecCloseWeather_2.end());

		m_vecParticles.erase(
			remove_if(m_vecParticles.begin(), m_vecParticles.end(),
				[](cDynamic* d) {
					if (((VfxParticles*)d)->checkFlag(d->bRedundant)) {
						d->ReturnToPool();
						return true;  // Удалить указатель из контейнера
					}
		return false;  // Оставить указатель в контейнере
				}),
			m_vecParticles.end());

		m_vecProjectiles.erase(
			remove_if(m_vecProjectiles.begin(), m_vecProjectiles.end(),
				[&](cDynamic* d) {
					if (((cDynamic_Projectile*)d)->checkFlag(d->bRedundant)) {
						d->ReturnToPool();
						return true;
					}
		return false;
				}),
			m_vecProjectiles.end());

	

		m_vecDynamics.erase(                              //check m_vecProjectiles on flag -bredundand and erase it    hp bars the same in vecdynamics
			remove_if(m_vecDynamics.begin(), m_vecDynamics.end(),
				[&]( cDynamic* d) {
					if (((cDynamic_Creature*)d)->checkFlag(d->bDead)) {

						d->ReturnToPool();
						return true;
					}
					
					return false;
				}), m_vecDynamics.end());
				

		m_vecFightText.erase(
			remove_if(m_vecFightText.begin(), m_vecFightText.end(),
				[&](cDynamic* d) {
				if (((cDynamic_TextDamage*)d)->checkFlag(d->bRedundant)) {
						d->ReturnToPool();
						return true;
					}
		return false;  // Оставить в векторе
				}),
			m_vecFightText.end());
		

		m_vecVisibleDynamics.clear();
		for (auto& source : { &m_vecDynamics })  // <- fill visible m_vec
			for (auto& dyns : *source)
			{
				if (dyns->px >= fCameraPosX - nVisibleTileX && dyns->px <= fCameraPosX + nVisibleTileX && dyns->py >= fCameraPosY - nVisibleTileY && dyns->py <= fCameraPosY + nVisibleTileY)
				{

					//dyns->Update(fElapsedTime, m_pPlayer);

					cDynamic* entity = dyns;
					if (dynamic_cast<cDynamic*>(entity))
						m_vecVisibleDynamics.push_back(entity); // Add the Bandit 
				}
				else
				{
					if (dyns->checkFlag(dyns->quested))
					{
						m_vecVisibleDynamics.push_back(dyns); // Add the Bandit 
					}

				}
			}

	

		if (GetKey(olc::Key::ESCAPE).bPressed)
		{
			if (n_nGameMode == MODE_LOCAL_MAP || n_nGameMode==MODE_WAREHOUSE || n_nGameMode == MODE_SHOP ||n_nGameMode==MODE_INVENTORY|| n_nGameMode == MODE_MAP || n_nGameMode ==MODE_PROFESSION || n_nGameMode ==MODE_BLACKSMITH || n_nGameMode == MODE_QUESTLOG)
			{
			n_nGameMode = MODE_LOCAL_MAP;
			bPause = !bPause;
			}

			

		}

	
		

		

		if (!bPause)

		{
			

			

			if (m_script.bUserControlEnabled )
			{


				if (GetKey(olc::Key::I).bPressed)
				{


					n_nGameMode = (n_nGameMode == MODE_INVENTORY) ? MODE_LOCAL_MAP : MODE_INVENTORY;


				}
				if (GetKey(olc::Key::T).bPressed)
				{

					n_nGameMode = (n_nGameMode == MODE_PROFESSION) ? MODE_LOCAL_MAP : MODE_PROFESSION;

				}
				if (GetKey(olc::Key::U).bPressed)
				{

					n_nGameMode = (n_nGameMode == MODE_QUESTLOG) ? MODE_LOCAL_MAP : MODE_QUESTLOG;

				}


				if (IsFocused())
				{
				//	SetMouseFocus(true);

					if (m_pPlayer->checkFlag(m_pPlayer->bControllable))
					{


						


						if (GetKey(olc::Key::SPACE).bPressed)
						{
							m_pPlayer->vy = -9.0;
							if (m_pPlayer->energyAmount >= 15)
							{

								switch (m_pPlayer->Jumpcounter)
								{
								case 0:
									m_pPlayer->MoveJump();
								
									break;
								case 1:
									m_pPlayer->MoveDoubleJump();
									break;
								}

							}

						}

						


						//m_pPlayer->SetVerticalDirection(3);


						
						if (GetKey(olc::Key::Q).bReleased)
						{
							if (m_vecUi[0] != nullptr)
							{
								if (m_vecUi[0]->Item->OnUse(m_pPlayer, m_listItems, m_vecUi[0]))
									m_vecUi[0] = nullptr;

								ClearAbsorbedSlots(m_listItems); // find absorbed object and change them on empty sockets
							}
						}
						if (GetKey(olc::Key::E).bReleased)
						{

							if (m_vecUi[1] != nullptr)
							{
								if (m_vecUi[1]->Item->OnUse(m_pPlayer, m_listItems, m_vecUi[1]))
									m_vecUi[1] = nullptr;
								ClearAbsorbedSlots(m_listItems); // find absorbed object and change them on empty sockets
							}
						}

						if (GetKey(olc::Key::F1).bPressed)
						{
							m_script.AddCommand((new cComand_HideScreen(2)));
							m_script.AddCommand((new cComand_Changemap("Forest", 5, 25.5)));

							m_script.AddCommand((new cComand_SetNgameMod(1)));
						}

						if (GetKey(olc::Key::F2).bPressed)
						{
							m_script.AddCommand((new cComand_HideScreen(2)));
							m_script.AddCommand(new cComand_Changemap("VillageTavern", 12, 9));

							//m_script.AddCommand((new cComand_Changemap("Forest", -1, 25.5)));
							m_script.AddCommand((new cComand_SetNgameMod(1)));
						}

						if (GetKey(olc::Key::Z).bPressed) // Check if left Alt key is held down
						{
							//	if (GetLearnedTalent(5))
								//{
							m_pPlayer->BlinkBehind();
							//}
						}


						if (GetKey(olc::Key::F3).bPressed)
						{
							m_script.AddCommand((new cComand_HideScreen(2)));
							//m_script.AddCommand((new cComand_Changemap("ForestPartTwo", 5, 21.5)));  // First spot
							m_script.AddCommand((new cComand_Changemap("ForestPartTwo", 100, 21.5)));

							//m_script.AddCommand((new cComand_Changemap("Forest", -1, 25.5)));
							m_script.AddCommand((new cComand_SetNgameMod(1)));

						}
						if (GetKey(olc::Key::F4).bPressed)
						{
							m_script.AddCommand((new cComand_HideScreen(2)));
							//m_script.AddCommand((new cComand_Changemap("ForestPartTwo", 5, 21.5)));  // First spot
							m_script.AddCommand((new cComand_Changemap("VillageInFire", 16, 13.5)));

							//m_script.AddCommand((new cComand_Changemap("Forest", -1, 25.5)));
							m_script.AddCommand((new cComand_SetNgameMod(1)));

						}


						if (n_nGameMode == MODE_LOCAL_MAP)
						{


							if (GetMouse(2).bPressed)
							{

								system("cls");
								if (!m_vecVisibleDynamics.empty())
								{
									SetMouseTarget(mouse);
								}




							}
							if (GetMouse(1).bPressed)    // Right mouse 
							{


								switch (m_pPlayer->GetFacingDirectionVertical())
								{
								case 1:              //<-- Look Up
									if (!m_pPlayer->checkFlag(m_pPlayer->bOnGround) && !m_pPlayer->checkFlag(m_pPlayer->bOnLanded))   // air attack
									{

									}
									else  //means on the ground
									{
										if (m_pPlayer->rageAmount >= 35 && m_pPlayer->enumCounter != 17 && GetLearnedTalent(13))
										{
											//Sonner we change it decition
											m_pPlayer->RageMoveAttackJumpUp();
										}
									}


									break;
								case 0:             // <--Look Down
									if (!m_pPlayer->checkFlag(m_pPlayer->bOnGround) && !m_pPlayer->checkFlag(m_pPlayer->bOnLanded))   // air attack
									{
										if (m_pPlayer->rageAmount >= 35 && m_pPlayer->enumCounter != 18 && GetLearnedTalent(14))
										{
											m_pPlayer->RageMoveAttackAirDown();
										}
									}
									else  //means on the ground
									{

										if (m_pPlayer->rageAmount >= 35 && GetLearnedTalent(11) && m_pPlayer->enumCounter != 8)
										{
											m_pPlayer->RageMoveAttackUp();
										}

									}


									break;
								case 3:            // <--No Looking

									if (!m_pPlayer->checkFlag(m_pPlayer->bOnGround) && !m_pPlayer->checkFlag(m_pPlayer->bOnLanded))   // air attack
									{
										if (m_pPlayer->rageAmount >= 35 && GetLearnedTalent(12) && m_pPlayer->enumCounter != 12)
										{
											m_pPlayer->RageMoveAttackAir();
										}
									}
									else  //means on the ground
									{
										if (m_pPlayer->rageAmount >= 35 && GetLearnedTalent(11) && m_pPlayer->enumCounter != 26)
										{
											//Sonner we change it decition
											m_pPlayer->RageMoveAttck();
										}
									}

									break;
								}



							}
						}

							m_pPlayer->SetVerticalDirection(3);



							if (GetKey(olc::Key::W).bHeld)
							{

								m_pPlayer->SetVerticalDirection(1); // <--North

							}
							if (GetKey(olc::Key::S).bHeld)
							{

								m_pPlayer->SetVerticalDirection(2);  // <--South
							}


						if (!m_pPlayer->checkFlag(m_pPlayer->isAttack))
						{

						





						


							if (GetKey(olc::Key::D).bHeld)
							{


								
								m_pPlayer->vx += (m_pPlayer->checkFlag(m_pPlayer->bOnGround) ? 35.0f * m_pPlayer->GetMovement() : 25.0f * m_pPlayer->GetMovement()) * fElapsedTime;
							}
							if (GetKey(olc::Key::A).bHeld)
							{
							//	m_pPlayer->ReductionAcctn(fElapsedTime);
								m_pPlayer->vx += (m_pPlayer->checkFlag(m_pPlayer->bOnGround) ? -35.0f * m_pPlayer->GetMovement() : -25.0f * m_pPlayer->GetMovement()) * fElapsedTime;

							}

							
							

							if (GetKey(olc::Key::SHIFT).bPressed)
							{
								if (GetLearnedTalent(7))
									m_pPlayer->HideStage();
							}
							//////////////////////




							/// ///////////////////
							if (GetKey(olc::Key::NP0).bPressed)
							{

								bshowZero = !bshowZero;


								//	bshowSecond = false;
								//	bshowfirst = false;
							}


							if (GetKey(olc::Key::NP1).bPressed)
							{

								bshowfirst = !bshowfirst;


								//	bshowSecond = false;
								//	bshowfirst = false;
							}
							if (GetKey(olc::Key::NP2).bPressed)
							{

								bshowSecond = !bshowSecond;


								//	bshowfirst = false;
								//	bshowThird = false;

							}
							if (GetKey(olc::Key::NP3).bPressed)
							{

								bshowThird = !bshowThird;
								//	bshowfirst = false;
								//	bshowSecond = false;
							}


							
						
							if (GetMouse(0).bPressed && n_nGameMode == MODE_LOCAL_MAP)   // <--LeftAttack
							{
								if (highlighted != nullptr)
								{
									GrabItem = highlighted;
									//GrabItem->GrabItem = true;
									GrabItem->setFlag(GrabItem->Grabitem);
								}
								else
								{



									if (!m_pPlayer->checkFlag(m_pPlayer->bOnGround) && !m_pPlayer->checkFlag(m_pPlayer->bOnLanded))      // <- attack on air
									{

										if (m_pPlayer->energyAmount >= 35 && GetLearnedTalent(1))   //EnergyAmount - Spellcount 
											//
										{
											m_pPlayer->EnergyMoveAttackAir();



											




										}
										if (GetLearnedTalent(6))   // swirl attack learned
										{

											float fTestX, fTestY;
											defineFacingDirection(fTestX, fTestY);
											for (auto dyns : m_vecVisibleDynamics)
											{
												if ((fTestX + 0.9f) > dyns->px + dyns->CollbordersXF && (fTestX - 0.9f) < dyns->px + dyns->CollbordersX && fTestY > dyns->py + dyns->CollbordersY && fTestY < dyns->py + dyns->CollbordersYF)
												{
													if (!dyns->checkFlag(m_pPlayer->bOnGround))

														if (dyns->m_layer == 1 && m_pPlayer->checkFacingDirection(dyns))
														{

															m_pPlayer->SwirlGrab(dyns);

															break;

														}



												}
											}
										}



									}
									else
									{

										float fTestX, fTestY;
										defineFacingDirection(fTestX, fTestY);
										for (auto dyns : m_vecVisibleDynamics)
										{
											if ((fTestX + 0.9f) > dyns->px + dyns->CollbordersXF && (fTestX - 0.9f) < dyns->px + dyns->CollbordersX && fTestY > dyns->py + dyns->CollbordersY && fTestY < dyns->py + dyns->CollbordersYF)
											{

												if (dyns->m_layer == 1 && m_pPlayer->checkFacingDirection(dyns) && GetLearnedTalent(4))
												{

													m_pPlayer->EnergyMoveAttackBack();


													



													break;

												}



											}
										}

										if (m_pPlayer->enumCounter != 16) //  no EnergyMove AttackBack
										{



													if (m_pPlayer->energyAmount >= 35 && GetLearnedTalent(1))   //EnergyAmount - Spellcount 

													{

														m_pPlayer->comboManager();

														

													}
												
											
										}
									}
								}
							}

							if (GetKey(olc::Key::F).bPressed)
							{

							//	testCounter = 0;
								float fTestX, fTestY;
								defineFacingDirection(fTestX, fTestY);
								for (auto dyns : m_vecVisibleDynamics)
								{
									if (abs(m_pPlayer->px - dyns->px) < 2 && fTestY > dyns->py + dyns->CollbordersY && fTestY < dyns->py + dyns->CollbordersYF)
									{
										if (dyns->m_layer == 4 && dyns->sName != "Pantir")
										{
											//Iterate through quest stack untill something responds
											//interactions that are not specified in other quests
											for (auto& quest : m_listQusets)
												if (quest->OnInteraction(m_vecVisibleDynamics, dyns, cQuset_MainQuest::TALK))
												{
													//bHitSomething = true;
													break;
												}
											//Then check if it is map related
											m_pCurrentMap->OnInteraction(m_vecVisibleDynamics, dyns, cMap::TALK);

										}
									}
								}
							}

						}
					}
				}
			}
			else

			{//Scripting system is in control
				if (m_bShowDialog)
				{

					if (GetKey(olc::Key::SPACE).bPressed)
					{
						m_bShowDialog = false;
						ClearDisplayText();
						m_script.CompleteCommand();
					}

				}

			}
			////////////               locks for speed 


float cameraSpeed = fElapsedTime/0.25f;
float mouseSnapFactor = 0.001f; // Adjust this value as needed

if (IsFocused())
{
	

	// Get offset for smooth movement
	if (!blockCamera)
	{
	/*	if (m_pPlayer->Gettarget() == nullptr)
		{*/


		

		fCameraPosX = Lerp(fCameraPosX, m_pPlayer->px + ((m_pPlayer->CollbordersXF - m_pPlayer->CollbordersX) / 2), cameraSpeed);
		fCameraPosY = Lerp(fCameraPosY, m_pPlayer->py + (m_pPlayer->CollbordersYF - m_pPlayer->CollbordersY) / 2, cameraSpeed);

	
		//}
		//else
		//{


		//	

		//	float X = m_pPlayer->px -((m_pPlayer->px - m_pPlayer->Gettarget()->px)/2);
		//	float Y = m_pPlayer->py - ((m_pPlayer->py - m_pPlayer->Gettarget()->py)/2);

		//	float deltaX = (X + (GetMouseX() - ScreenWidth() / 2) * mouseSnapFactor) - fCameraPosX;
		//	float deltaY = (Y + (GetMouseY() - ScreenHeight() / 2) * mouseSnapFactor) - fCameraPosY;

		//	// Snap the camera towards the player and mouse position
		//	fCameraPosX = X + (GetMouseX() - ScreenWidth() / 2) * mouseSnapFactor;
		//	fCameraPosY = Y - 1 + (GetMouseY() - ScreenHeight() / 2) * mouseSnapFactor;

		//	// Apply interpolation to smooth out the movement
		//	fCameraPosX += deltaX * cameraSpeed;
		//	fCameraPosY += deltaY * cameraSpeed;
		//}


	}

	// Get current mouse position
	int mouseX = GetMouseX();
	int mouseY = GetMouseY();

	// Constrain mouse position within the game window
	if (mouseX < 0)
		mouseX = 0;
	else if (mouseX >= ScreenWidth())
		mouseX = ScreenWidth() - 1;

	if (mouseY < 0)
		mouseY = 0;
	else if (mouseY >= ScreenHeight())
		mouseY = ScreenHeight() - 1;
}

m_pPlayer->vy += m_pPlayer->accelerationY * fElapsedTime;
m_pPlayer->vx += m_pPlayer->accelerationX * fElapsedTime;

//std::cout << m_pPlayer->px << '\n' << m_pPlayer->py << std::endl;
			bool bWorkingWithProjectiles = false;
	
			for (auto& source : { &m_vecVisibleDynamics, &m_vecProjectiles })
			{
				for (auto& object : *source) // for every chars in this vector will be calculate their move
				{


					if (object->checkFlag(object->gravity))
					{
						object->vy += object->mass * fElapsedTime;                     //Gravitation for everyone



					}





					//object->bOnGround = false;

					float fNewObjectPosX = object->px + object->vx * fElapsedTime;
					float fNewObjectPosY = object->py + object->vy * fElapsedTime;





					//	fCameraPosX = fCameraPosX + fCameraVx * fElapsedTime;
					//	fCameraPosY = fCameraPosY + fCameraVy * fElapsedTime;

						////Collision




					bool bslope = false;
				//	bool in = false;

					//	float middleX = object->CollbordersX + ((object->CollbordersXF - object->CollbordersX) / 2.0f);

							//Check slopes
					//	std::cout << "Before: =" << m_pPlayer->vy <<"\n"<<"BOnGround =: " << m_pPlayer->checkFlag(m_pPlayer->bOnGround) << "\n" <<"BonLanded = :" << m_pPlayer->checkFlag(m_pPlayer->bOnLanded) << std::endl;
					if (object->bSolidVsMap)
					{


						if (object->vx < 0)  //move Left
						{
						if (m_pCurrentMap->GetColliziionIndex(fNewObjectPosX + object->CollbordersX, object->py + object->CollbordersY) == 1 || m_pCurrentMap->GetColliziionIndex(fNewObjectPosX + object->CollbordersX, object->py + object->CollbordersYF - 0.1f) == 1)
							{

								fNewObjectPosX = object->px;  //if there won't emprty cell we move right
								object->vx = 0;

							}
							else if (m_pCurrentMap->GetColliziionIndex(fNewObjectPosX + object->CollbordersXF, object->py + object->CollbordersYF) == 2)
							{

								if (object->obsticlepoints == nullptr)
									object->obsticlepoints = (m_pCurrentMap->getObsticlesPoints(fNewObjectPosX + object->CollbordersX, fNewObjectPosX + object->CollbordersXF, object->py + object->CollbordersY, object->py + object->CollbordersYF));

								float deltaX = object->obsticlepoints->second->x - object->obsticlepoints->first->x;
								float deltaY = object->obsticlepoints->first->y - object->obsticlepoints->second->y;
								float slopeAtangle = deltaY / deltaX; // artnagents
								float charX = fNewObjectPosX + object->CollbordersXF - object->obsticlepoints->first->x;
								float reqHigh = charX * slopeAtangle;
								if (fNewObjectPosY + object->CollbordersYF >= object->obsticlepoints->first->y - reqHigh)
								{
									object->vy = 49.0f;
									bslope = true;
								}

							}


						}
						else   //move right
						{
						if (m_pCurrentMap->GetColliziionIndex(fNewObjectPosX + object->CollbordersXF, object->py + object->CollbordersY) == 1 || m_pCurrentMap->GetColliziionIndex(fNewObjectPosX + object->CollbordersXF, object->py + object->CollbordersYF - 0.1f) == 1)
							{

								fNewObjectPosX = object->px;  //if there won't emprty cell we move right


								object->vx = 0;
							//	std::cout << "Move right Collision 3" << std::endl;
							}
							else if (m_pCurrentMap->GetColliziionIndex(fNewObjectPosX + object->CollbordersX, object->py + object->CollbordersYF) == 3)
							{

								if (object->obsticlepoints == nullptr)
									object->obsticlepoints = (m_pCurrentMap->getObsticlesPoints(fNewObjectPosX + object->CollbordersX, object->px + object->CollbordersXF, object->py + object->CollbordersY, object->py + object->CollbordersYF));

								float deltaX = object->obsticlepoints->second->x - object->obsticlepoints->first->x;
								float deltaY = object->obsticlepoints->second->y - object->obsticlepoints->first->y;
								float slopeRatio = deltaY / deltaX;  // proportion  (atan)
								float charX = object->obsticlepoints->second->x - object->px - object->CollbordersX;  // define pos on slope 
								float sloperatio = charX * slopeRatio;    // find heigh equal the pos in slope 
								//	fNewObjectPosX += DeltaX;
								if (fNewObjectPosY + object->CollbordersYF >= object->obsticlepoints->second->y - sloperatio)
								{

									object->vy = 49.0f;
									bslope = true;
								}
							//	std::cout << "Move right Collision 4" << std::endl;
							}
						}
					}
					if (object->vy < 0)  //move Up
					{
						if (m_pCurrentMap->GetColliziionIndex(object->px + object->CollbordersX, fNewObjectPosY + object->CollbordersY) == 1 || m_pCurrentMap->GetColliziionIndex(object->px + object->CollbordersXF, fNewObjectPosY + object->CollbordersY) == 1)
						{
							object->obsticlepoints = nullptr;
							fNewObjectPosY = object->py;  //if there won't emprty cell we move right
							//fNewObjectPosY = std::round(fNewObjectPosY * 10.0f) / 10.0f;
							object->vy = 0;
						}

					}
					else   //move Down
					{
						if (m_pCurrentMap->GetColliziionIndex(object->px + object->CollbordersX, fNewObjectPosY + object->CollbordersYF) == 2|| m_pCurrentMap->GetColliziionIndex(object->px + object->CollbordersXF, fNewObjectPosY + object->CollbordersYF) == 2)
						{
							if (object->obsticlepoints == nullptr)
								object->obsticlepoints = (m_pCurrentMap->getObsticlesPoints(object->px + object->CollbordersX, object->px + object->CollbordersXF, object->py + object->CollbordersY, fNewObjectPosY + object->CollbordersYF));

							float deltaX = object->obsticlepoints->second->x - object->obsticlepoints->first->x;
							float deltaY = object->obsticlepoints->first->y - object->obsticlepoints->second->y;
							float slopeRatio = deltaY / deltaX;
							float charX = object->px + object->CollbordersXF - object->obsticlepoints->first->x;
							float CharRequireHigh = charX * slopeRatio;

							float borderX = object->px + object->CollbordersXF - object->obsticlepoints->second->x;

							if (CharRequireHigh < 0.01f)  //for move out from slope  set high equal lower point
							{
								CharRequireHigh = 0;

							}
							//	fNewObjectPosX += DeltaX;
							if (fNewObjectPosY + object->CollbordersYF >= object->obsticlepoints->first->y - CharRequireHigh)
							{

								
								if (borderX > 0)
								{
									fNewObjectPosY = object->obsticlepoints->second->y - object->CollbordersYF;
								}
								else {

									fNewObjectPosY = object->obsticlepoints->first->y - CharRequireHigh - object->CollbordersYF;
								}

								if (!object->checkFlag(object->bOnGround))                                // <- we trying to make in OnLanded anim
								{
									object->IsLanded();


									auto& FxLand = GetSpriteData(DataStruct::FxDust);
									float centerX = object->nSheetSizeX / 2 - FxLand.Size.x / 2;
									float centerY = (FxLand.Size.y / CellSize) * fscale + 0.1f;
									centerX /= CellSize;

									SpawnMirrors({ object->px + centerX,object->py + object->CollbordersYF - centerY }, FxLand.Pos, 5, FxLand.Size.x, FxLand.Size.y, D_FX);
								}

								bslope = true;
								object->Jumpcounter = 0;
								object->setFlag(object->bOnGround);


					//			std::cout <<"char high =  " << borderX << "  assign : " << object->obsticlepoints->first->y - CharRequireHigh - object->CollbordersYF << "   Move Down Collision 1" << std::endl;
							}
						}
						else if (m_pCurrentMap->GetColliziionIndex(object->px + object->CollbordersX, fNewObjectPosY + object->CollbordersYF) == 3|| m_pCurrentMap->GetColliziionIndex(object->px + object->CollbordersXF, fNewObjectPosY + object->CollbordersYF) == 3)
						{
							if (object->obsticlepoints == nullptr)
								object->obsticlepoints = (m_pCurrentMap->getObsticlesPoints(object->px + object->CollbordersX, object->px + object->CollbordersXF, object->py + object->CollbordersY, object->py + object->CollbordersYF));
							float deltaX = object->obsticlepoints->second->x - object->obsticlepoints->first->x;
							float deltaY = object->obsticlepoints->second->y - object->obsticlepoints->first->y;
							float slopeRatio = deltaY / deltaX;
							float charX = object->obsticlepoints->second->x - object->px - object->CollbordersX;

							float borderX = object->obsticlepoints->first->x - (object->px + object->CollbordersX);
							float CharRequireHigh = charX * slopeRatio;
							if (CharRequireHigh < 0.01f)
							{
								CharRequireHigh = 0;

							}
							//	fNewObjectPosX += DeltaX;
							if (fNewObjectPosY + object->CollbordersYF >= object->obsticlepoints->second->y - CharRequireHigh)
							{

								if (borderX>0)
								{
									fNewObjectPosY = object->obsticlepoints->first->y - object->CollbordersYF;
								}
								else
								{
								fNewObjectPosY = object->obsticlepoints->second->y - CharRequireHigh - object->CollbordersYF;
								}

								if (!object->checkFlag(object->bOnGround))                                // <- we trying to make in OnLanded anim
								{
									object->IsLanded();


									auto& FxLand = GetSpriteData(DataStruct::FxDust);
									float centerX = object->nSheetSizeX / 2 - FxLand.Size.x / 2;
									float centerY = (FxLand.Size.y / CellSize) * fscale + 0.1f;
									centerX /= CellSize;

									SpawnMirrors({ object->px + centerX,object->py + object->CollbordersYF - centerY }, FxLand.Pos, 5, FxLand.Size.x, FxLand.Size.y, D_FX);
								}

								object->Jumpcounter = 0;
								object->setFlag(object->bOnGround);
								bslope = true;
							}
						//	std::cout <<"object->pX : =  " << object->px << "with coll : = " << object->px + object->CollbordersX << object->obsticlepoints->first->x << "\t" << object->obsticlepoints->second->x << "borderX : =" << borderX << std::endl;
						}
						else if (m_pCurrentMap->GetColliziionIndex(object->px + object->CollbordersXF, fNewObjectPosY + object->CollbordersYF) == 1 || m_pCurrentMap->GetColliziionIndex(object->px + object->CollbordersX, fNewObjectPosY + object->CollbordersYF) == 1)  // 1 means tough collision
						{
							if (!bslope)
							{

								object->obsticlepoints = nullptr;
								if (!object->checkFlag(object->bOnGround))                                // <- we trying to make in OnLanded anim
								{
									object->IsLanded();

									auto& FxLand = GetSpriteData(DataStruct::FxDust);

									float centerX = object->nSheetSizeX / 2 - FxLand.Size.x / 2;
									float centerY = (FxLand.Size.y / CellSize) * fscale +0.1f;
									centerX /= CellSize;

									SpawnMirrors({ object->px+centerX,object->py+object->CollbordersYF- centerY}, FxLand.Pos, 5, FxLand.Size.x, FxLand.Size.y, D_FX);

							//		testCounter++;
								}
								fNewObjectPosY = ceil(object->py*10)/10;  // example: 25.47*10= 254,7 -> ceil(254.7->255)->/10-?25.5
								object->vy = 0;
								object->Jumpcounter = 0;
								object->setFlag(object->bOnGround);

								
							}
						//	std::cout << "Move Down Collision 3" << std::endl;
						}
						else if (m_pCurrentMap->GetColliziionIndex(object->px + object->CollbordersX, fNewObjectPosY + object->CollbordersYF) == 5 || m_pCurrentMap->GetColliziionIndex(object->px + object->CollbordersXF, fNewObjectPosY + object->CollbordersYF) == 5)  // 5 means spikes
						{
							if (object == m_pPlayer)
							{
								if (charDeath == false)
								{
									charDeath = true;
									m_script.AddCommand((new  cComand_LoadFunction));
								}

							}
							else
							{
								object->setFlag(object->bDead);

							}
					//		std::cout << "Move Down Collision 4" << std::endl;
						}

					}





					if (object->vy <= -0.1f && !bslope || object->vy >= 0.1f && !bslope)
						object->clearFlag(object->bOnGround);


				

					float fDynamicObjectPosX = fNewObjectPosX;
					float fDynamicObjectPosY = fNewObjectPosY;



					// Object V Object collisions

					if (m_script.bUserControlEnabled)
					{
						if (object == m_vecDynamics[0] || blockCamera)
						{

							if (fDynamicObjectPosX + object->CollbordersX < fOffsetX + 1 || fDynamicObjectPosX + object->CollbordersXF > fOffsetX + nVisibleTileX)   // here we don't let character went out from
							{
								object->vx = 0;

								fDynamicObjectPosX = object->px;

							}
						}



						for (auto& dyn : m_vecVisibleDynamics)
						{



								// If the object is solid then the player must not overlap it. It means all objects
								if (object->m_layer == 0 && dyn->m_layer == 1 || object->m_layer == 1 && dyn->m_layer == 0)  // layer 0 - means freand, 1- enemy
								{
									// Check if bounding rectangles overlap
									if (fDynamicObjectPosX + object->CollbordersX < (dyn->px + dyn->CollbordersXF) && (fDynamicObjectPosX + object->CollbordersXF) > dyn->px + dyn->CollbordersX &&
										object->py + object->CollbordersY < (dyn->py + dyn->CollbordersYF) && (object->py + object->CollbordersYF) > dyn->py + dyn->CollbordersY)
									{
										// First Check Horizontally - Check Left
										if (object->sName != "projectile")    // if we not projectile we blocking walk through enemy
										{


											if (object->vx <= 0)
											{
												fDynamicObjectPosX = object->px;
												object->vx = 0;
											}
											else
												fDynamicObjectPosX = object->px;

										}

									}

									if (fDynamicObjectPosX + object->CollbordersX < (dyn->px + dyn->CollbordersXF) && (fDynamicObjectPosX + object->CollbordersXF) > dyn->px + dyn->CollbordersX &&
										fDynamicObjectPosY + object->CollbordersY < (dyn->py + dyn->CollbordersYF) && (fDynamicObjectPosY + object->CollbordersYF) > dyn->py + dyn->CollbordersY)
									{
										if (object == m_vecDynamics[0] && object->checkFlag(object->bIsAttackable))
										{

											// First Check Vertically - Check Left
											if (object->vy <= 0)
											{
												fDynamicObjectPosY = object->py;
												JumpDamage((cDynamic_Creature*)dyn, (cDynamic_Creature*)object);
												
											}

											else
											{
												fDynamicObjectPosY = object->py;
												JumpDamage((cDynamic_Creature*)dyn, (cDynamic_Creature*)object);
											}

										}

									}

								}



								else
								{
									if (object == m_vecDynamics[0])
									{


										//object is player and can interact with things
										if (fDynamicObjectPosX + object->CollbordersX < (dyn->px + dyn->CollbordersXF) && (fDynamicObjectPosX + object->CollbordersXF) > dyn->px + dyn->CollbordersX &&
											object->py + object->CollbordersY < (dyn->py + dyn->CollbordersYF) && (object->py + object->CollbordersYF)>dyn->py + dyn->CollbordersY)
										{
											//First check if object is part of quest
											for (auto& quest : m_listQusets)
												if (quest->OnInteraction(m_vecDynamics, dyn, cQuest::WALK))
													break;

											//Then check is it is map related
											m_pCurrentMap->OnInteraction(m_vecVisibleDynamics, dyn, cMap::WALK);  //mapInteraction keeps teleport



											//Finally just check the object
											dyn->OnInteract(object);
										}

									}

								}
								if (bWorkingWithProjectiles)
								{

									if (fDynamicObjectPosX + object->CollbordersX  < (dyn->px + dyn->CollbordersXF) && (fDynamicObjectPosX + object->CollbordersXF) > dyn->px + dyn->CollbordersX &&
										fDynamicObjectPosY + object->CollbordersY  < (dyn->py + dyn->CollbordersYF) && (fDynamicObjectPosY + object->CollbordersYF) > dyn->py + dyn->CollbordersY)
									{

									

										//Here  means       not equal layers       //m_Layer 2 - means NEUTRAL(for skip projectile)
										if (dyn->m_layer != object->m_layer && dyn->m_layer != 2 && object->m_layer != 2)
										{
											//we know object is projectile, so dyn is something 
											//opposite that it has overlapped with
											if (dyn->checkFlag(dyn->bIsAttackable))
											{
												//Dynamic object is a creature
												Damage((cDynamic_Projectile*)object, (cDynamic_Creature*)dyn);

											}
											
											//if (dyn->checkFlag(dyn->BisProjectile) == object->checkFlag(object->BisProjectile) && dyn != object)   // for reflect throw attacks 
											//{
											//	if (dyn->checkFlag(dyn->IsThrow) && !dyn->checkFlag(dyn->isReflected))  // here mean that dyn is flying projectile
											//	{
											//		dyn->vy = -dyn->vy* -dyn->vy;
											//		dyn->vx = -dyn->vx*-dyn->vx;
											//		dyn->setFlag(dyn->isReflected);
											//		dyn->reflectLayer();
											//	}

											//}
											
										}
										


									}

								}
							
						}


						for (auto& dyn : m_vecProjectiles)
						{
							if (fDynamicObjectPosX + object->CollbordersX  < (dyn->px + dyn->CollbordersXF) && (fDynamicObjectPosX + object->CollbordersXF) > dyn->px + dyn->CollbordersX &&
								fDynamicObjectPosY + object->CollbordersY  < (dyn->py + dyn->CollbordersYF) && (fDynamicObjectPosY + object->CollbordersYF) > dyn->py + dyn->CollbordersY)
							{
					
								//Here  means       not equal layers       //m_Layer 2 - means NEUTRAL(for skip projectile)
								if (dyn->m_layer != object->m_layer && dyn->m_layer != 2 && object->m_layer != 2)
								{
					
									if (dyn->checkFlag(dyn->IsThrow)&& object->checkFlag(object->BisProjectile))
									{
										dyn->m_layer = object->m_layer;

										dyn->vy = dyn->vy * 3.0f;
										
										dyn->vx = sign(dyn->vx)* -15.0f;


										auto& FxLand = GetSpriteData(DataStruct::FXDeflect);

										float centerX = object->nSheetSizeX / 2 - FxLand.Size.x / 2;
										float centerY = (FxLand.Size.y / CellSize) * fscale + 0.1f;
										centerX /= CellSize;

										SpawnMirrors({ object->px + centerX,object->py + object->CollbordersYF - centerY }, { FxLand.Pos.x/FxLand.Size.x,FxLand.Pos.y/ FxLand.Size.y }, 2, FxLand.Size.x, FxLand.Size.y, D_FX);
										PlaySounds("DeflectSound");

									}

								}


							}

						}
					}

				


					object->px = fDynamicObjectPosX;
					object->py = fDynamicObjectPosY;
				}
				bWorkingWithProjectiles = true;
				PressF = false;

			}
			
		
			//m_vecVisibleDynamics.clear();
			for (auto& source : { &m_vecVisibleDynamics,&m_vecProjectiles,&m_vecFightText,&m_vecParticles })  // <- run on vecDynamics array and use Update function in every person this array
				for (auto& dyns : *source)
				{
					if (dyns->px >= fCameraPosX - nVisibleTileX && dyns->px <= fCameraPosX + nVisibleTileX && dyns->py >= fCameraPosY - nVisibleTileY && dyns->py <= fCameraPosY + nVisibleTileY)
					{

						dyns->Update(fElapsedTime, m_pPlayer);

					}
					else
					{
						if (dyns->checkFlag(dyns->quested))   //for simulation crowd
						{
							dyns->Update(fElapsedTime, m_pPlayer);
						}
					}
				
					if (dyns->sName != "projectile" && dyns != m_pPlayer)
					{

					switch (dyns->m_layer)
					{
						//  m_layer 1 - enemy layer
					case 1:
						if (dyns->checkFlag(dyns->bDead))
						{
							//Iterate through quest stack untill something responds
							//interactions that are not specified in other quests
							for (auto& quest : m_listQusets)
								if (quest->OnInteraction(m_vecDynamics, dyns, cQuest::KILL))
								{

									//bHitSomething = true;
									//break;
								}
						}
						else  //Check for backstubUi
						{
							float fTestX, fTestY;
							defineFacingDirection(fTestX, fTestY);

							if ((fTestX + 0.9f) >= dyns->px + dyns->CollbordersXF && (fTestX - 0.9f) <= dyns->px + dyns->CollbordersX && fTestY > dyns->py + dyns->CollbordersY && fTestY < dyns->py + dyns->CollbordersYF)

							{
								if (dyns->m_layer == 1 && m_pPlayer->checkFacingDirection(dyns))
								{
									//

									bUibackstub = true;


								}
							}
						}

						break;

							
						case 4:   // Neutral
							{
								float fTestX, fTestY;
								defineFacingDirection(fTestX, fTestY);



								if (abs(m_pPlayer->px - dyns->px) < 2 && fTestY > dyns->py + dyns->CollbordersY && fTestY < dyns->py + dyns->CollbordersYF)
								{

									//	DrawCircle((fTestX - fOffsetX) * 64, (fTestY - fOffsetY) * 64, 0.5 * 64);

									
									PressF = true;
									break;

								}

							}


							break;
					}

					}
				

				}
		
			//Remove quests that have been completed
			auto i = remove_if(m_listQusets.begin(), m_listQusets.end(), [](const cQuest* d) {return d->bCompleted; });  //go through the list and check for any quests that have
			//be completed flag set to true -remove them
			if (i != m_listQusets.end())
				m_listQusets.erase(i);





			
			
		}
		std::cout << m_listQusets.size() << std::endl;
			fOldOffset = fOffsetX;

			//Calculate Top-Leftmost visible tile
			 fOffsetX = fCameraPosX - (float)nVisibleTileX / 2.0f;
			 fOffsetY = fCameraPosY - (float)nVisibleTileY / 2.0f;


			if (fOffsetX < 2) fOffsetX = 2;
			if (fOffsetY < 0) fOffsetY = 0;
			if (fOffsetX > m_pCurrentMap->nWidth - nVisibleTileX - 2) fOffsetX = m_pCurrentMap->nWidth - nVisibleTileX - 2;
			if (fOffsetY > m_pCurrentMap->nHeight - nVisibleTileY) fOffsetY = m_pCurrentMap->nHeight - nVisibleTileY;


			//Get offsets for smooth movement
			float fTileOffsetX = (fOffsetX - (int)fOffsetX) * CellSize;
			float fTileOffsetY = (fOffsetY - (int)fOffsetY) * CellSize;
		



				Clear(olc::BLANK);
				//SetDecalMode(olc::DecalMode::NORMAL);


				 // Update parallax layers based on the current scroll position
      //  UpdateParallax(ScrollingCurrX, scrollingbetweenX, ScrollingBackX, fOffsetX);

				if (m_pCurrentMap->FarParralax != nullptr)
				{

					// parralaxOffset = fOffsetX;
					 
			
			#ifdef DEBUG_MODE
			SetDrawTarget(layer);
			SetPixelMode(olc::Pixel::NORMAL);
			#endif


			if (fOffsetX!=fOldOffset&& fOffsetX-fOldOffset<1)
			{

			 fFarPrlxX -= (fOffsetX-fOldOffset) * 12;
			 fMidPrlxX -= (fOffsetX - fOldOffset) * 14;
			 fClosePrlxX -= (fOffsetX - fOldOffset) * 16;
			}
			fOldOffset = fOffsetX;

			//float DuplcParlxX = fFarPrlxX + (float)ScreenWidth();

					 WrapCoordinates(fFarPrlxX);
					 WrapCoordinates(fMidPrlxX);
					 WrapCoordinates(fClosePrlxX);

					
					
					
				//	 WrapCoordinates(DuplcParlxX);



				DrawDecal({ fFarPrlxX ,0 }, m_pCurrentMap->FarParralax);

				DrawDecal({ fFarPrlxX - (float)ScreenWidth(),0 }, m_pCurrentMap->FarParralax);


	

				DrawDecal({ fMidPrlxX ,(0+90) - (fOffsetY *3.0f) }, m_pCurrentMap->MidParralax);

				DrawDecal({ fMidPrlxX - (float)ScreenWidth() , (0+90) - (fOffsetY *3.0f) }, m_pCurrentMap->MidParralax);



				DrawDecal({fClosePrlxX ,(0+180) - (fOffsetY*6.0f) }, m_pCurrentMap->CloseParralax);

				DrawDecal({ fClosePrlxX - (float)ScreenWidth() , (0 + 180) -(fOffsetY*6.0f) }, m_pCurrentMap->CloseParralax);
				}

				/////////ParallaxLogic
				// 

				
				//SetDrawTarget(nullptr);
			
				if (m_pCurrentMap->sName == "Forest" || m_pCurrentMap->sName == "ForesttLvlPartTwo")
				{
					for (auto& source : { &m_vecFarWeather })    // DRAW Clouds
						for (auto& dyns : *source)
						{


							dyns->Update(fElapsedTime, ScreenWidth(), ScreenHeight()); // Update raindrop position

							dyns->DrawSelf(this, fOffsetX, fOffsetY);


						}
				}
				

					
			

					// Iterate over visible tiles
					for (int x = -4; x < nVisibleTileX+2 ; x++) {
						for (int y = -4; y < nVisibleTileY+2 ; y++) {
							Tile idx = m_pCurrentMap->GetIndexLayer( x + fOffsetX, y + fOffsetY);  // here we need to take needed information 

							idx.position.x = x;
							idx.position.y = y;
							
								if (idx.ThirdLayer.index != 0)
									batchThirdCharsLayer.push_back(idx);

								if (idx.SecondLayer.index != 0)
									batchSecondCharsLayer.push_back(idx);

								if (idx.DynamicLayer.index != 0)
									batchDynamicCharsLayer.push_back(idx);

								if (idx.FirstLayer.index != 0)
									batchFirstCharsLayer.push_back(idx);

								if (idx.ZeroLayer.index != 0)
									batchZeroCharsLayer.push_back(idx);






							}
					}
				
				

			//	if (m_pPlayer->bHideMode)
			//	{
			//	m_pPlayer->DrawSelf(this, fOffsetX, fOffsetY);
		//		}

				//Draw Objects
				

			


				if (!batchThirdCharsLayer.empty())
				{



					for (const auto& tile : batchThirdCharsLayer) {


						if (bshowThird == true)
							SetDrawTarget(bshowThird);

						DrawPartialDecal({ tile.position.x * CellSize - fTileOffsetX,tile.position.y * CellSize - fTileOffsetY }, m_pCurrentMap->pSprite, tile.ThirdLayer.sourcerect, tile.ThirdLayer.size);

						#ifdef DEBUG_MODE
						if (bshowThird == true)
						{

							EnableLayer(layer, true);
							SetDrawTarget(nullptr);
							DrawRect({ tile.position.x * (int)CellSize - (int)fTileOffsetX,tile.position.y * (int)CellSize - (int)fTileOffsetY }, tile.ThirdLayer.size);
						}
						#endif


					}
					batchThirdCharsLayer.clear();
				}
				if (!batchSecondCharsLayer.empty())
				{



					for (const auto& tile : batchSecondCharsLayer) {


						#ifdef DEBUG_MODE
						if (bshowSecond == true)
							SetDrawTarget(layer);
						#endif
						
						

						DrawPartialDecal({ tile.position.x * CellSize - fTileOffsetX,tile.position.y * CellSize - fTileOffsetY }, m_pCurrentMap->pSprite, tile.SecondLayer.sourcerect, tile.SecondLayer.size);

						#ifdef DEBUG_MODE
						if (bshowSecond == true)
						{

							EnableLayer(layer, true);
							SetDrawTarget(nullptr);
							DrawRect({ tile.position.x * (int)CellSize - (int)fTileOffsetX,tile.position.y * (int)CellSize - (int)fTileOffsetY }, tile.SecondLayer.size);
						}
						#endif


					}
					batchSecondCharsLayer.clear();
				}

				if (!batchDynamicCharsLayer.empty())
				{



					for ( auto& tile : batchDynamicCharsLayer) {

						tile.DynamicLayer.index -= 10;

						DrawPartialDecal({ tile.position.x * CellSize - fTileOffsetX,tile.position.y * CellSize - fTileOffsetY }, m_pCurrentMap->pDynsprite, { 128.0f * frameIndex,tile.DynamicLayer.index * 128.0f }, { 128,128 }, { 1,1 });

						//DrawPartialDecal({ tile.position.x * CellSize - fTileOffsetX,tile.position.y * CellSize - fTileOffsetY }, m_pCurrentMap->pSprite, tile.FirstLayer.sourcerect, tile.FirstLayer.size);

						


					}

					batchDynamicCharsLayer.clear();
				}



				if (!batchFirstCharsLayer.empty())
				{



					for (const auto& tile : batchFirstCharsLayer) {


						#ifdef DEBUG_MODE
						if (bshowfirst == true)
							SetDrawTarget(layer);
						#endif

						DrawPartialDecal({ tile.position.x * CellSize - fTileOffsetX,tile.position.y * CellSize - fTileOffsetY }, m_pCurrentMap->pSprite, tile.FirstLayer.sourcerect, tile.FirstLayer.size);

						#ifdef DEBUG_MODE
						if (bshowfirst == true)
						{

							EnableLayer(layer, true);
							SetDrawTarget(nullptr);
							DrawRect({ tile.position.x * (int)CellSize - (int)fTileOffsetX,tile.position.y * (int)CellSize - (int)fTileOffsetY }, tile.FirstLayer.size);
						}
						#endif


					}

				batchFirstCharsLayer.clear();
				}


				for (auto& source : { &m_vecFightText,&m_vecVisibleDynamics,&m_vecProjectiles,&m_vecParticles })
					for (auto& dyns : *source)
						if (dyns->px >= fCameraPosX - nVisibleTileX && dyns->px <= fCameraPosX + nVisibleTileX && dyns->py >= fCameraPosY - nVisibleTileX && dyns->py <= fCameraPosY + nVisibleTileX)   // && m_vecDynamics[0] !=dyns
						{
							if (dyns->checkFlag(dyns->bDraw))
								dyns->DrawSelf(this, fOffsetX, fOffsetY);

						}
						else
						{
							if (dyns->checkFlag(dyns->quested))
							{
								dyns->DrawSelf(this, fOffsetX, fOffsetY);
						}
						}

			//	m_vecVisibleDynamics[0]->DrawSelf(this, fOffsetX, fOffsetY);


				if (m_pCurrentMap->sName == "Forest")
				{

					for (auto& source : { &m_vecCloseWeather,&m_vecCloseWeather_2 })    // DRAW Rain
						for (auto& dyns : *source)
						{


							dyns->Update(fElapsedTime, fOffsetX, fOffsetY); // Update raindrop position

							dyns->DrawSelf(this, fOffsetX, fOffsetY);


						}

				}

				if (!batchZeroCharsLayer.empty())
				{
				


					for (const auto& tile : batchZeroCharsLayer) {
						
						
						#ifdef DEBUG_MODE
						if (bshowZero == true)
							SetDrawTarget(layer);
						#endif
						
						DrawPartialDecal({ tile.position.x * CellSize - fTileOffsetX,tile.position.y * CellSize - fTileOffsetY }, m_pCurrentMap->pSprite, tile.ZeroLayer.sourcerect, tile.ZeroLayer.size);
							
							#ifdef DEBUG_MODE
							if (bshowZero == true)
							{
							
							EnableLayer(layer, true);
							SetDrawTarget(nullptr);
							DrawRect({ tile.position.x * (int)CellSize - (int)fTileOffsetX,tile.position.y * (int)CellSize - (int)fTileOffsetY }, tile.ZeroLayer.size);
							}
							#endif
							

					}
						
					batchZeroCharsLayer.clear();
				}



				
				/*}*/

			/*	if (!m_pPlayer->bHideMode)
				{
				m_pPlayer->DrawSelf(this, fOffsetX, fOffsetY);
				}*/
			

				#ifdef DEBUG_MODE

				if (bshowZero)
				{

				for (auto& source : { &m_vecFightText,&m_vecVisibleDynamics,&m_vecProjectiles,&m_vecParticles }) //Draw chars Collizzion 
					for (auto& dyns : *source)
						if (dyns->px >= fCameraPosX - nVisibleTileX && dyns->px <= fCameraPosX + nVisibleTileX && dyns->py >= fCameraPosY - nVisibleTileX && dyns->py <= fCameraPosY + nVisibleTileX)   // && m_vecDynamics[0] !=dyns
						{
							
							DrawRect((dyns->px - fOffsetX) * CellSize, (dyns->py - fOffsetY) * CellSize, 128*fscale, 128*fscale);
							
							
							
							DrawLine((dyns->px + dyns->CollbordersX - fOffsetX)* CellSize, (dyns->py + dyns->CollbordersY - fOffsetY)* CellSize, (dyns->px + dyns->CollbordersX - fOffsetX+dyns->vx)* CellSize, (dyns->py + dyns->CollbordersY - fOffsetY+dyns->vy)* CellSize);
							//DrawRect((dyns->px+dyns->CollbordersX - fOffsetX)* (64 * fscale), (dyns->py+dyns->CollbordersY - fOffsetY)* (64 * fscale), 128 * fscale, 128 * fscale);

							DrawLine((dyns->px + dyns->CollbordersX - fOffsetX) * CellSize, (dyns->py + dyns->CollbordersY - fOffsetY) * CellSize, (dyns->px + dyns->CollbordersX - fOffsetX) * CellSize, (dyns->py + dyns->CollbordersYF - fOffsetY) * CellSize);
							DrawLine((dyns->px + dyns->CollbordersX - fOffsetX) * CellSize, (dyns->py + dyns->CollbordersY - fOffsetY) * CellSize, (dyns->px + dyns->CollbordersXF - fOffsetX) * CellSize, (dyns->py + dyns->CollbordersY - fOffsetY) * CellSize);
							DrawLine((dyns->px + dyns->CollbordersX - fOffsetX) * CellSize, (dyns->py + dyns->CollbordersYF - fOffsetY) * CellSize, (dyns->px + dyns->CollbordersXF - fOffsetX) * CellSize, (dyns->py + dyns->CollbordersYF - fOffsetY) * CellSize);
							DrawLine((dyns->px + dyns->CollbordersXF - fOffsetX) * CellSize, (dyns->py + dyns->CollbordersY - fOffsetY) * CellSize, (dyns->px + dyns->CollbordersXF - fOffsetX) * CellSize, (dyns->py + dyns->CollbordersYF - fOffsetY) * CellSize);

						}
				}
						#endif
	
						
			


				for (auto& source : { &m_vecUiBars })    // DRAW Ui elements above char 
					for (auto& dyns : *source)
						dyns->DrawSelf(this, (m_pPlayer->px-fOffsetX)* CellSize, (m_pPlayer->py - fOffsetY+1.5) * CellSize);

			

				if (PressF)
				DrawPartialDecal({ (m_pPlayer->px + (1.0f*fscale) - fOffsetX) * CellSize,((m_pPlayer->py-0.2f) - fOffsetY) * CellSize }, D_FullUi, { 976,384 }, { 112,30 },{0.5f*fscale,0.5f*fscale});


				//

				uiCellUpdate(mouse);

			

				//

			//	Drawcursor(mouse);



		
			
			
					switch (n_nGameMode)
					{
					case MODE_TITLE:

						break;
					case MODE_LOCAL_MAP:
					
					//	 DrawPause(mouse);
							
						break;
					case MODE_WAREHOUSE:
						UpdateWarehouse(fElapsedTime,mouse);
						break;
					case MODE_INVENTORY:
						UpdateInventory(fElapsedTime,mouse);
						break;
					case MODE_SHOP:
						UpdateShop(fElapsedTime,mouse);
						break;
					case MODE_PROFESSION:
						UpdateProfession(fElapsedTime,mouse);
						break;
					case MODE_MAP:
						UpdateMap(fElapsedTime,mouse);
						break;
					case MODE_BLACKSMITH:
						UpdateBlackSmith(fElapsedTime,mouse);
						break;
					case MODE_QUESTLOG:
						UpdateQuestLog(fElapsedTime,mouse);
						break;
					}
				

			



				uint8_t y = 0;
				for (auto& quest : m_listQusets)  // ui drawing quest right up corner on the screen
				{
				
				
					DrawBigText(quest->sName, ScreenWidth() - (250*fscale), y * (32*fscale), 0.35*fscale, 0.35*fscale, olc::YELLOW);
					y++;
					DrawBigText(quest->sDescription, ScreenWidth() - (250*fscale), y * (32*fscale), 0.25*fscale, 0.25*fscale);
					y++;

				}


				if (bSmoothAppearScreen)
				{
					
				DrawPartialDecal({ (float)0, (float)0 }, RPG_Assets::get().GetSprite("DescriptionPattern"), { 0,0 }, {(float)RPG_Assets::get().GetSprite("DescriptionPattern")->sprite->width, (float)RPG_Assets::get().GetSprite("DescriptionPattern")->sprite->height}, { 2,2 }, olc::Pixel{255,255,255,(uint8_t)AlphaAppearScreen});
				}
							//Draw any dialog being displayed
				if (m_bShowDialog)
					DisplayDialog(m_vecDialogToShow, 100*fscale, ScreenHeight() - (150*fscale));

				UpdateAnimationFrame(fElapsedTime);

				
				//
#ifdef  DEBUG_MODE

				SetPixelMode(olc::Pixel::NORMAL);
				EnableLayer(layer, true);
				SetDrawTarget(nullptr);
#endif // DEBUG

				// Limit the frame rate to 60 frames per second
			//	Sleep(4); // Sleep for the appropriate time to achieve desired frame rate

				return  DrawPause(mouse);
			
	
}

bool RPG_Engine::UpdateWarehouse(float fElapsedTime,olc::vi2d& mouse)
{


	




	//olc::vf2d mouse = { (float)GetMouseX(), (float)GetMouseY() };
	olc::vf2d mousefix = { (mouse.x / (CellSize*fscale)), (mouse.y / (CellSize*fscale)) };

	


	 highlighted = nullptr;
	 GrabItem = nullptr;

	float squeY = 8 * fscale;
	float squeX = 4 * fscale;
	const float WHsqueX = 14 * fscale;
	
	const float ReqSocketsY = (float)(128 / 64) * fscale;
	const float RqSocketsX = (float)(192 / 64) * fscale;
	
	float Scale = 0.5f * fscale;

	olc::vf2d inventorySockets = { 0,250 };

	DrawPartialDecal({ (float)4 * CellSize * fscale, (float)(4) * CellSize * fscale }, D_Inventory,{0,0}, {512,523}, { Scale,Scale });  // draw sockets and inventary name


	
	DrawPartialDecal({ (float)14 * CellSize * fscale, (float)(8) * CellSize * fscale }, D_Inventory, inventorySockets, { 512,199 }, { Scale,Scale });  // draw sockets 
	DrawPartialDecal({ (float)15.85 * CellSize * fscale, (float)11 * CellSize * fscale }, D_Inventory, { 725,75 }, { 273,76 }, { Scale,Scale });  // draw sockets 
//	DrawPartialDecal({ (float)14 * 64*fscale, (float)8 * 64*fscale }, InventorySockeys, { 0,0 }, { invSocketsWidth,invSocketsHeigh }, { fscale,fscale });
//	DrawPartialDecal({ (float)15.85 * 64*fscale, (float)11 * 64*fscale }, RPG_Assets::get().GetSprite("WarehouseName"), { 0,0 }, { 274,75 }, { fscale,fscale });
	
	DrawBigText(std::to_string(m_pPlayer->GetStats(2)), (9) * CellSize * fscale, (5 + 0.3) * CellSize * fscale, Scale, Scale); //Strength
	DrawBigText(std::to_string(m_pPlayer->GetStats(1)), (9) * CellSize * fscale, (6 + 0.3) * CellSize * fscale, Scale, Scale); //Agility
	DrawBigText(std::to_string(m_pPlayer->GetStats(3)), (9) * CellSize * fscale, (7 + 0.2) * CellSize * fscale, Scale, Scale); //Intelegence
	//DrawBigText(std::to_string(m_pPlayer->GetStats(5)), (4) * 64, (6) * 64, 0.5, 0.5); //MovementSpeed
	DrawBigText(std::to_string(m_pPlayer->GetStats(6)), (5.2) * CellSize * fscale, (5.8) * CellSize * fscale, Scale-0.1f, Scale -0.1f); //AverageAttack
	DrawBigText(std::to_string(m_pPlayer->GetStats(4)), (5.2 ) * CellSize * fscale, (6.8) * CellSize * fscale, Scale -0.1f, Scale -0.1f); //Defence
	//DrawBigText(std::to_string(m_pPlayer->GetStats(7)), (4) * 64, (8) * 64, 0.5, 0.5); //fHpRegeneration
	//DrawBigText(std::to_string(m_pPlayer->GetStats(8)), (4) * 64, (9) * 64, 0.5, 0.5); //Versality

//	DrawBigText(std::to_string(m_pPlayer->GetStats(6)), (squeX - 1.3 + ((inventoryWidth / 64) / 2) * fscale) * CellSize, (squeY - 1.1 + ((inventoryHeight / 64) / 2) * fscale) * CellSize, Scale - 0.1f, Scale - 0.1f); //AverageAttack
//	DrawBigText(std::to_string(m_pPlayer->GetStats(4)), (squeX - 1.3 + ((inventoryWidth / 64) / 2) * fscale) * CellSize, (squeY - 0.6 + ((inventoryHeight / 64) / 2) * fscale) * CellSize, Scale - 0.1f, Scale - 0.1f); //Defence


	float EqSqueX = (3 + 7) * fscale;
	float EqSqueY = (4 + 1) * fscale;



	for (auto& item : m_vecEquip)
	{

		int i = item->index;
		if (item->Item != nullptr)
		{

			int  x = i % 2;
			int  y = i / 2;

			float sprcoordX = item->Item->spriteindex % 32;
			float sprcoordY = item->Item->spriteindex / 32;


			//if(item->sName =="Coin")
		   // AddMoney(item->Item->GoldCount);




			if (item->checkFlag(item->Grabitem) == false)
			{

				DrawPartialDecal({ (float)(EqSqueX + (x * fscale)) * CellSize, (EqSqueY + (y * fscale)) * CellSize }, item->Item->pSprite, { sprcoordX * 64,sprcoordY * 64 }, { 64,64 }, { Scale,Scale });

				//item->ipy = y;



				if ((int)mousefix.x == ((EqSqueX / fscale) + x) && (int)mousefix.y == (EqSqueY / fscale) + y)
					highlighted = item;

			}
		//	else
		//	{
		//		DrawPartialDecal({ (float)(mouse.x - (32 / 2)) , (float)(mouse.y - (32 / 2)) }, item->Item->pSprite, { sprcoordX * 64,sprcoordY * 64 }, { 64,64 }, { fscale,fscale });
		//		GrabItem = item;

		//	}

		}
		i++;
	}






	DrawInventory(squeX, squeY, mouse, mousefix, highlighted, GrabItem);

	//
	DrawWarehouse(WHsqueX, squeY, mouse, mousefix, highlighted, GrabItem, Money);
	
	
		

	

	if (GetMouse(0).bReleased)
	{
		if (GrabItem != nullptr)
		{
			
			
			if ((int)mousefix.x >= squeX/fscale && (int)mousefix.y >= squeY/fscale && (int)mousefix.x <= 11 && (int)mousefix.y <= 10) //Inventory
			{

				if (GrabItem->checkFlag(GrabItem->binWarehouse))  //when we grabed item from warehouse
				{
				
				
					//GrabItem->bInWarehouse = false;
					GrabItem->clearFlag(GrabItem->binWarehouse);
					

					int x = (int)mousefix.x - (squeX/fscale) ;    //ere we calculate in local space in Warehouse object  /invnumber 
					int y = (int)mousefix.y - (squeY / fscale);


					int currIndex = 8 * y + x; // cell in inventary that need to check on reserved  
					int GrabitemIndex = GrabItem->index;


					m_listItems[currIndex]->index = GrabItem->index; //switch indexes
					GrabItem->index = currIndex;  // switch indexes

					m_listBlackSmithItems[currIndex]->setFlag(m_listBlackSmithItems[currIndex]->binWarehouse);

					std::swap(m_listItems[currIndex], m_listWarehouseItems[GrabitemIndex]);

			

				//	vector[currIndex]->index = GrabItem->index; //assign index from grabbed object to that place where it shoild be dropped
				//	GrabItem->index = currIndex;   // assign  index too bject

				//	std::swap(vector[currIndex], vector[GrabbedIndex]);

				}
				else
				{

					moveIItems(mousefix, squeX, squeY, GrabItem, m_listItems);
				}











				}
			

			if ((int)mousefix.x >= WHsqueX/fscale && (int)mousefix.y >= squeY/fscale && (int)mousefix.x <= 21 && (int)mousefix.y <= 10)   //Warehouse
			{

				 
				if (!GrabItem->checkFlag(GrabItem->binWarehouse))    // when we grabed from inv
				{
					
					//GrabItem->bInWarehouse = true;
						
					int x = (int)mousefix.x  - (WHsqueX/fscale) ;    //calculate drop position for draged object
					int y = (int)mousefix.y - (squeY / fscale);

					int currIndex = 8 *y + x; // cell in inventary that need to check on reserved  
					int GrabedIndex = GrabItem->index;
					
						
						m_listWarehouseItems[currIndex]->index= GrabItem->index; //switch indexes
						GrabItem->index = currIndex;  // switch indexes
						//m_listWarehouseItems[currIndex]->bInWarehouse = false;
						GrabItem->setFlag(GrabItem->binWarehouse);   
						m_listWarehouseItems[currIndex]->clearFlag(m_listWarehouseItems[currIndex]->binWarehouse);

						std::swap(m_listWarehouseItems[currIndex], m_listItems[GrabedIndex]);

					

				}
				else
				{
					moveIItems(mousefix, WHsqueX, squeY, GrabItem, m_listWarehouseItems);

				}

				

		}
			//GrabItem->GrabItem = false;
			GrabItem->clearFlag(GrabItem->Grabitem);

		}
	}






	if (highlighted != nullptr)
	{


		if (GetMouse(0).bPressed)
		{


			if ((int)mousefix.x >= squeX / fscale && (int)mousefix.y >= squeY / fscale) //Inventory
			highlighted->setFlag(highlighted->Grabitem);
			
		}







			DrawDescriptionPattern(highlighted, mouse,mousefix);

	





			if (GetMouse(1).bPressed)
			{
				// Use selected item 
				if (!highlighted->Item->bKeyItem)
				{
					if (highlighted->checkFlag(highlighted->binWarehouse))
					{

						if (highlighted->Item->OnUse(m_pPlayer, m_listWarehouseItems,highlighted))
						{
							// Item has signalled it must be consumed, so remove it
							ClearAbsorbedSlots(m_listWarehouseItems); // find absorbed object and change them on empty sockets
							highlighted = nullptr;
							//TakeItem(highlighted, m_listWarehouseItems);

							

						}
					}
					else if (highlighted->Item->OnUse(m_pPlayer, m_listItems,highlighted))
					{
						ClearAbsorbedSlots(m_listItems); // find absorbed object and change them on empty sockets
						highlighted = nullptr;
					//	TakeItem(highlighted, m_listItems);
					}
					

					
				}

			}
	}



	if (highlighted != nullptr)
	{

		DrawDescriptionPattern(highlighted, mouse,mousefix);


	}



	// MONEY
	squeX = (4 * fscale) * CellSize;
	squeY = (3 * fscale) * CellSize;
	DrawBigText(std::to_string(Money), squeX + Scale, squeY + (16 * fscale), 0.5f*fscale, 0.5f * fscale, olc::YELLOW); //Money
//	DrawPartialDecal({ (float)squeX, (float)squeY }, RPG_Assets::get().GetSprite("Coin"), { 0,0 }, { (float)64,(float)64 }, { fscale,fscale });
	//


	



	DrawString(0, 8, "LOCATION:", olc::WHITE, 3);
	DrawString(0, 64, m_pCurrentMap->sName, olc::WHITE, 1);


	//Mouse
	Drawcursor(mouse);

	//DrawPartialDecal({ (float)(mouse.x - 0.15f) , (float)(mouse.y - 0.1f) }, RPG_Assets::get().GetSprite("Pantir's Dagger"), { 0,0 }, { (float)64,(float)(64) }, { fscale,fscale });



	return true;
}

void RPG_Engine::DrawWarehouse(const float offestX, const float offsetY, olc::vf2d mouse, olc::vf2d mouseFixed, InventaryItem*& highlighted, InventaryItem*& Grabitem, int moneyamount)
{

	

	Money = moneyamount;
	int i = 0;
	int x = 0;
	int y = 0;

	for (auto& item : m_listWarehouseItems)
	{
		i = item->index;
		if (item->checkFlag(item->Breserved))
		{

			if (item->Item != nullptr)
			{

				i = item->index;
				if (item->checkFlag(item->Breserved))
				{

					AddMoney(item->Gold);
					x = i % 8;
					y = i / 8;
					float sprcoordX = item->Item->spriteindex % 32;
					float sprcoordY = item->Item->spriteindex / 32;


				




					if (dynamic_cast<cGold*>(item->Item))
					{
						cGold* itemHelp = (cGold*)item->Item;
						AddMoney(itemHelp->GoldCount);
					}



					if (item->checkFlag(item->Grabitem) == false)
					{

						DrawPartialDecal({ (float)(offestX + (x * fscale)) * CellSize, (offsetY + (y * fscale)) * CellSize }, item->Item->pSprite, { sprcoordX * 64,sprcoordY * 64 }, { 64,64 }, { 0.5f * fscale,0.5f * fscale });

						//item->ipy = y;




						if (item->Item->MaxStack > 1)
							DrawBigText(std::to_string(item->currStacks), (float)(offestX + (x * fscale) + 0.05) * CellSize, (offsetY + (y * fscale)) * CellSize, fscale * 0.5f, fscale * 0.5f, olc::WHITE); //Money
						if (item->Gold > 0)
							DrawBigText(std::to_string(item->Gold), (float)(offestX + (x * fscale) + 0.4) * CellSize, (offsetY + (y * fscale) + 0.3) * CellSize, fscale * 0.5f, fscale * 0.5f, olc::YELLOW); //Money



						if ((int)mouseFixed.x == ((offestX / fscale) + x) && (int)mouseFixed.y == (offsetY / fscale) + y)
							highlighted = item;

					}
					else
					{
						DrawPartialDecal({ (float)(mouse.x - ((32 / 2)*fscale)) , (float)(mouse.y - ((32 / 2)*fscale)) }, item->Item->pSprite, { sprcoordX * 64,sprcoordY * 64 }, { 64,64 }, {0.5f* fscale,0.5f*fscale });
						Grabitem = item;

					}
				}
			}
		}
		i++;
	}
}

bool RPG_Engine::SetMouseTarget(olc::vi2d& mouse)
{

	if (m_pPlayer->Gettarget() == nullptr)
	{


		for (auto dyns : m_vecVisibleDynamics)
		{


			//for (int x = 0; x < 1; x++)
			//{
			//	for (int y = 0; y < 1; y++)             
				//{

			if (dyns != m_vecVisibleDynamics[0])
			{


			if (((mouse.x) / 64) + fOffsetX >= dyns->px + dyns->CollbordersX - 1 && ((mouse.y) / 64) + fOffsetY >= dyns->py + dyns->CollbordersY - 1)
			{
				if (((mouse.x) / 64) + fOffsetX <= dyns->px + dyns->CollbordersXF + 1 && ((mouse.y) / 64) + fOffsetY <= dyns->py + dyns->CollbordersYF + 1)
				{
					m_pPlayer->ConnectTarget(dyns);
					return true;
					
				}

			}
			}
			//	}

		//	}

		}

	}
	else
	{
	m_pPlayer->ConnectTarget(nullptr);
	}


	return false;



}






void RPG_Engine::DrawInventoryFastCells(olc::vi2d mouse,uint8_t SelObjSize,olc::vi2d mousefix)
{
	uint8_t xcoor = 8;
	for (auto i : m_vecUi) // when we have on fast key sobe objects
	{
		if (i != nullptr)
		{
			float x = i->Item->spriteindex % 32;   // find in sprite texture  position
			float y = i->Item->spriteindex / 32;

			if (!i->checkFlag(i->Grabitem))

			{
				//UIPosAr[xcoor]




				DrawPartialDecal(UIPosOnScreenAr[xcoor], i->Item->pSprite, { x * 64,y * 64 }, { (float)SelObjSize, (float)SelObjSize }, { 0.5f * fscale,0.5f * fscale });

				//	std::cout << SelObjSize * xcoor * fscale / (64*fscale) << '\t' << mousefix.y << std::endl;

				if (mouse.x >= UIPosOnScreenAr[xcoor].x && mouse.x <= UIPosOnScreenAr[xcoor].x + (SelObjSize * 0.5f * fscale) &&
					mouse.y >= UIPosOnScreenAr[xcoor].y && mouse.y <= UIPosOnScreenAr[xcoor].y + (SelObjSize * 0.5f * fscale))
				{
					highlighted = i;
				}

			}
			else
			{

				DrawPartialDecal({ (float)(mouse.x - (32 / 2)) , (float)(mouse.y - (32 / 2)) }, i->Item->pSprite, { x * 64,y * 64 }, { 64,64 }, { fscale,fscale });
				GrabItem = i;
			}





		}
		xcoor++;
	}

	if (highlighted != nullptr)
	{
		float nLinesX = 0;
		int nLinesY = 3;
		float nsaveline = 0;
		float nsaveLine2 = 0;
		for (auto c : highlighted->Item->sDescription)
		{

			if (c == 10)  // <-- 10 is /n
			{
				if (nsaveLine2 <= nsaveline)
				{
					nLinesX = 32 + (nsaveline * 18);
					nsaveLine2 = nsaveline;
					nsaveline = 0;
				}
				nLinesY++;
				continue;
			}

			nsaveline++;     //  32/100 = 0.32-> 1 percent.   32-18 = 14   14/0,32 = 0.43   1-0.43 = 0.57    
		}


		olc::vi2d Discription(ScreenWidth() - (32 + nLinesX * fscale), ScreenHeight() - (ScreenHeight() / 6) - (nLinesY * 32 * fscale) * fscale);
		DrawDescriptionPattern(highlighted, Discription, mousefix);

	}






}

void RPG_Engine::uiCellUpdate(olc::vi2d& mouse)
{

	highlighted = nullptr;
	// GrabItem = nullptr;


	olc::vi2d mousefix = { (mouse.x / (int)(CellSize * fscale)),(mouse.y / (int)(CellSize * fscale)) };
	uint8_t SelObjSize = 64;



	uint8_t UiFirstCellY = (ScreenHeight() - (ScreenHeight() / 6)) / (CellSize);
	auto& MainBlock = GetSpriteData(DataStruct::UIMainBox);

	DrawPartialDecal(UIPosOnScreenAr[0], D_FullUi, MainBlock.Pos, MainBlock.Size, { 2.0f * fscale,2.0f * fscale });



	auto& UiEnergy = GetSpriteData(DataStruct::UiEnergy);
	auto& UiRage = GetSpriteData(DataStruct::UiRage);
	//here we get from 0 -> 1.0f
	float EnergyUpdate = UiEnergy.Size.y * (static_cast<float>(m_pPlayer->GetEnergy()) / static_cast<float>(m_pPlayer->MaxEnergy));
	float RageUpdate = UiEnergy.Size.y * (static_cast<float>(m_pPlayer->GetRage()) / static_cast<float>(m_pPlayer->MaxRage));


	
	DrawPartialDecal({ (float)UIPosOnScreenAr[6].x, (float)UIPosOnScreenAr[6].y + UiEnergy.Size.y - EnergyUpdate }, D_FullUi, { (float)UiEnergy.Pos.x,(float)UiEnergy.Pos.y + UiEnergy.Size.y - EnergyUpdate }, { (float)UiEnergy.Size.x, EnergyUpdate }, { 2.0f * fscale,2.0f * fscale });
	DrawPartialDecal({ (float)UIPosOnScreenAr[7].x, (float)UIPosOnScreenAr[7].y + UiRage.Size.y - RageUpdate }, D_FullUi, { (float)UiRage.Pos.x, (float)UiRage.Pos.y + UiRage.Size.y - RageUpdate }, { (float)UiRage.Size.x, RageUpdate }, { 2.0f * fscale,2.0f * fscale });



	for (int i = 1; i < 4; i++)
	{
		auto& spriteData = GetSpriteData(static_cast<DataStruct>(i+14));

		DrawPartialDecal(UIPosOnScreenAr[i], D_FullUi, spriteData.Pos, spriteData.Size, { 2.0f * fscale,2.0f * fscale });

	}
	
	for (int i = 0; i < SpellsUiAr.size(); i++)
	{


		DrawPartialDecal(UIPosOnScreenAr[i+4], D_FullUi, SpellsUiAr[i]->Pos, SpellsUiAr[i]->Size, {0.5f * fscale,0.5f * fscale});

	}

	DrawInventoryFastCells(mouse,SelObjSize,mousefix);
	
	


	if (TalentPoint >0)
	{
		
		int AnimCounter = frameIndex % 4;

		auto& TButton = GetSpriteData(DataStruct::FxTalentButton);
		
		float Xcoord = TButton.Pos.x / TButton.Size.x + AnimCounter;
		//int Ycoord = TButton.Pos.y / TButton.Size.y;

		DrawPartialDecal({ (float)UIPosOnScreenAr[3].x,(float)UIPosOnScreenAr[3].y-1 }, D_FullUi, { Xcoord * TButton.Size.x,(float)TButton.Pos.y }, TButton.Size, { 2.0f*fscale,2.0f*fscale });

	}

	
}

void RPG_Engine::UpdateQuestLog(float fElapsedTime, olc::vi2d& mouse)
{



	auto& BookPos = GetSpriteData(DataStruct::QuestBook);

	olc::vi2d Bookcellsize = { 18,18 };  // клетки для книг поудобнее
	olc::vi2d QuestBookPos = { ScreenWidth() / 2 - 150,ScreenHeight() / 2 - 100 };
	olc::vi2d ActiveMainPos = { QuestBookPos.x+Bookcellsize.x * 1,QuestBookPos.y + Bookcellsize.y * 2 };
	olc::vi2d DescriptionMainPos = { ActiveMainPos.x + (Bookcellsize.x * 8), ActiveMainPos.y };
	olc::vi2d CompletedPos = { ActiveMainPos.x,QuestBookPos.y + Bookcellsize.y * 9 };

	DrawPartialDecal(QuestBookPos, D_FullUi, BookPos.Pos, BookPos.Size, { 2.0f * fscale,2.0f * fscale });



	uint8_t y = 0;
	cQuest* Highlighted = nullptr;
	uint8_t LightIndex = 0;

	for (auto& quest : m_listQusets)  // ui drawing quest right up corner on the screen
	{

		if (mouse.x >= ActiveMainPos.x && mouse.y >= ActiveMainPos.y + (y * Bookcellsize.y/2) &&
			mouse.x <= ActiveMainPos.x + (Bookcellsize.x * 6) && mouse.y <= ActiveMainPos.y + ((y + 1) * Bookcellsize.y/2))
		{
			Highlighted = quest;
			LightIndex = y;
		}

		olc::Pixel color = (Highlighted == quest) ? olc::YELLOW : olc::WHITE;

		DrawBigText(quest->sName, ActiveMainPos.x,ActiveMainPos.y+ (y*Bookcellsize.y/2), 0.5 * fscale, 0.5 * fscale, color);


		if (DescrqstIndex == y)
		{
		DrawBigText(quest->sDescription, DescriptionMainPos.x, DescriptionMainPos.y, 0.5 * fscale, 0.5 * fscale);

		}
		
		y++;
	
	}

	y = 0;
	for (auto& quest : m_CompletedQuest)  // Completed quest
	{

		DrawBigText(quest->sName, CompletedPos.x, CompletedPos.y + (y * Bookcellsize.y / 2), 0.5 * fscale, 0.5 * fscale, olc::GREY);

	}

	if (Highlighted != nullptr)
	{

		if (GetMouse(0).bPressed)
		{



		

				DescrqstIndex = LightIndex;

				//Highlighted->SeeDescription = true;
		}
	
	}


	Drawcursor(mouse);
}

bool RPG_Engine::UpdateInventory(float fElapsedTIme, olc::vi2d& mouse)
{
	

	highlighted = nullptr;
	GrabItem = nullptr;

	

	const int inventoryHeight = 523;
	const int inventoryWidth = 512;
	float Scale =0.5f*fscale;
	//olc::vi2d mouse = {(GetMouseX()), (GetMouseY()) };


	olc::vi2d mousefix = { (mouse.x / (int)(CellSize * fscale)),(mouse.y / (int)(CellSize * fscale)) };
	
	

	float squeX = (nVisibleTileX / 3) * fscale;
	float squeY = (nVisibleTileX / 3) * fscale;

	
	 float EqSqueX = (squeX + (6 * fscale)) ;
	 float EqSqueY = (squeY + (1 * fscale)) ;
	
	


	
	DrawPartialDecal({ (float)squeX * CellSize, (float)(squeY * CellSize)  },D_Inventory, { 0,0 }, { (float)inventoryWidth,(float)inventoryHeight}, { Scale,  Scale});
	


	// DrawPartialDecal({ EqSqueX*64, EqSqueY*64 }, m_pPlayer->pEquipedWeapon->pSprite, { 0,0 }, { (float)inventoryWidth,(float)inventoryHeight }, { Scale,  Scale });


	 for (auto& item : m_vecEquip)
	 {
		
		 int i = item->index;
		 if (item->Item != nullptr)
		 {

			 int  x = i % 2;
			 int  y = i / 2;
			 
			float sprcoordX = item->Item->spriteindex % 32;
			float sprcoordY = item->Item->spriteindex / 32;


			 //if(item->sName =="Coin")
			// AddMoney(item->Item->GoldCount);




			 if (item->checkFlag(item->Grabitem) == false)
			 {

				 DrawPartialDecal({ (float)(EqSqueX + (x * fscale)) * CellSize, (EqSqueY + (y * fscale)) * CellSize }, item->Item->pSprite, { sprcoordX * 64,sprcoordY * 64 }, { 64,64 }, { Scale,Scale });

				 //item->ipy = y;



				 if ((int)mousefix.x == ((EqSqueX / fscale) + x) && (int)mousefix.y == (EqSqueY / fscale) + y)
					 highlighted = item;

			 }
			 else
			 {
				 DrawPartialDecal({ (float)(mouse.x - (32 / 2)) , (float)(mouse.y - (32 / 2)) }, item->Item->pSprite, { sprcoordX * 64,sprcoordY * 64 }, { 64,64 }, { Scale,Scale });
				 GrabItem = item;

			 }

		 }
		 i++;
	 }





	DrawBigText(std::to_string(m_pPlayer->GetStats(1)), (squeX + fscale + ((inventoryWidth / 64) / 2) * fscale) * CellSize, (squeY - 0.9 + ((inventoryHeight / 64) / 2) * fscale) * CellSize, Scale, Scale, olc::YELLOW); //Agility
	DrawBigText(std::to_string(m_pPlayer->GetStats(2)), (squeX +fscale + ((inventoryWidth / 64) / 2) * fscale) * CellSize, (squeY - 1.4 + ((inventoryHeight / 64) / 2) * fscale) * CellSize, Scale, Scale, olc::RED); //Strength
	DrawBigText(std::to_string(m_pPlayer->GetStats(3)), (squeX + fscale + ((inventoryWidth / 64) / 2) * fscale) * CellSize, (squeY - 0.4 + ((inventoryHeight / 64) / 2) * fscale) * CellSize, Scale, Scale,olc::BLUE); //Intelegence

	//DrawBigText(std::to_string(m_pPlayer->GetStats(5)), (4) * 64, (6) * 64, 0.5, 0.5); //MovementSpeed

	DrawBigText(std::to_string(m_pPlayer->GetStats(6)), (squeX - 1.3 + ((inventoryWidth / 64) / 2) * fscale) * CellSize, (squeY - 1.1 + ((inventoryHeight / 64) / 2) * fscale) * CellSize, Scale-0.1f, Scale-0.1f ); //AverageAttack
	DrawBigText(std::to_string(m_pPlayer->GetStats(4)), (squeX - 1.3 + ((inventoryWidth / 64) / 2) * fscale) * CellSize, (squeY - 0.6 + ((inventoryHeight / 64) / 2) * fscale) * CellSize, Scale-0.1f, Scale-0.1f); //Defence

	//DrawBigText(std::to_string(m_pPlayer->GetStats(7)), (4) * 64, (8) * 64, 0.5, 0.5); //fHpRegeneration
	//DrawBigText(std::to_string(m_pPlayer->GetStats(8)), (4) * 64, (9) * 64, 0.5, 0.5); //Versality
	
	
	
	// std::cout << offestX << '\t' << offsetY << std::endl;
	

	
	//if ((int)mousefix.x == 10*fscale && (int)mousefix.y == 6*fscale)
	//	highlighted = m_pPlayer->pEquipedWeapon;


	const float YcellOffset = (squeY + (inventoryHeight / 64 - 4) * 0.5f); // <-- offset from higher point of inventory picture to cells where they drawing

	DrawInventory(squeX, YcellOffset,mouse, mousefix,highlighted,GrabItem);
	

	




	if (highlighted != nullptr)
	{


		if (GetMouse(0).bPressed)
		{
		
				//grab = true;
			//	GrabItem = highlighted;

				highlighted->setFlag(highlighted->Grabitem);
				//highlighted = nullptr;
		}

		

		DrawDescriptionPattern(highlighted, mouse,mousefix);

		

		

		if (GetMouse(1).bPressed)
		{
			// Use selected item 
			if (!highlighted->Item->bKeyItem)
			{
				if (highlighted->Item->OnUse(m_pPlayer, m_listItems,highlighted))
				{
					ClearAbsorbedSlots(m_listItems); // find absorbed object and change them on empty sockets
					// Item has signalled it must be consumed, so remove it
					//TakeItem(highlighted,m_listItems);
				}
			}
			
		}

	}
	
	if (GetMouse(0).bReleased)
	{
		if (GrabItem != nullptr)
		{
			

			moveIItems(mousefix, squeX, YcellOffset, EqSqueX, EqSqueY, GrabItem,m_listItems, m_vecEquip);

		}
	}
	
	// MONEY
	
	float coinsprX = 3 % 32;
	float coinsprY = 3 / 32;

	DrawBigText(std::to_string(Money), (float)(squeX + ((64 * fscale) / 64))* CellSize, (float)(squeY + 0.1 - ((64 * fscale) / 64))* CellSize, Scale, Scale, olc::YELLOW); //Money
	DrawPartialDecal({ (float)squeX * CellSize, (float)(squeY - ((64 * fscale) / 64)) * CellSize }, D_Items, { coinsprX * 64,coinsprY }, { (float)64,(float)64 }, { Scale ,Scale });
	//


	
	
	//Mouse
		Drawcursor(mouse);



	return true;
}

void RPG_Engine::DrawInventory(float offestX, float offsetY,olc::vi2d mouse,InventaryItem*& Highlighted )
{
	Money = 0;
	int x = 0;
	int y = 0;
	int i = 0;
	for (auto& item : m_listItems)
	{
		x = i % 8;

		y = i / 8;
		float sprcoordX = item->Item->spriteindex % 32;
		float sprcoordY = item->Item->spriteindex / 32;
		i++;

		if (dynamic_cast<cGold*>(item->Item))
		{
			cGold* itemHelp = (cGold*)item->Item;
			AddMoney(itemHelp->GoldCount);
		}



		DrawPartialDecal({ (float)(offestX + (x  * fscale)) * 64, (offsetY + ((float)y  * fscale)) * 64 }, item->Item->pSprite, { sprcoordX * 64,sprcoordY * 64 }, { 64,64 }, { fscale,fscale });
		item->index = i;
		//item->ipy = y;


		if ((int)mouse.x == (offestX + x  * fscale) && (int)mouse.y == offsetY + (y  * fscale))
			Highlighted = item;


	}
}

void RPG_Engine::DrawInventory(float offestX, float offsetY, olc::vi2d mouse, olc::vi2d mouseFixed, InventaryItem*& highlighted, InventaryItem*& Grabitem)
{

	

	Money = 0;
	int i = 0;
	int x = 0;
	int y = 0;
	

	for (auto& item : m_listItems)
	{
		if (item->Item !=nullptr)
		{

		i = item->index;
		if (item->checkFlag(item->Breserved))
		{

		AddMoney(item->Gold);
		x = i % 8;
		y = i / 8;
		float sprcoordX = item->Item->spriteindex % 32;
		float sprcoordY = item->Item->spriteindex / 32;


	//	if (dynamic_cast<cGold*>(item->Item))
	//	{
			cGold* itemHelp = (cGold*)item->Item;

			//item->currStacks
			
	//	}



		if (item->checkFlag(item->Grabitem) == false)
		{

			DrawPartialDecal({ (float)(offestX + (x  * fscale)) * CellSize, (offsetY  + (y  * fscale)) * CellSize }, item->Item->pSprite, { sprcoordX*64,sprcoordY*64 }, { 64,64 }, { 0.5f*fscale,0.5f*fscale });
			
			//item->ipy = y;
			

			if (item->Item->MaxStack>1)
			DrawBigText( std::to_string(item->currStacks), (float)(offestX + (x * fscale)+0.05) * CellSize, (offsetY + (y * fscale)) * CellSize, fscale*0.5f, fscale*0.5f,olc::WHITE); //Money
			if (item->Gold > 0)
			{
				if (item->Gold>10)
				{
					DrawBigText(std::to_string(item->Gold), (float)(offestX + (x * fscale) + 0.1) * CellSize, (offsetY + (y * fscale) + 0.3) * CellSize, fscale * 0.5f, fscale * 0.5f, olc::YELLOW); //Money
				}
				else
				{
					DrawBigText(std::to_string(item->Gold), (float)(offestX + (x * fscale)+0.3) * CellSize, (offsetY + (y * fscale)+0.3) * CellSize, fscale*0.5f, fscale*0.5f, olc::YELLOW); //Money

				}

			}

			if (mouseFixed.x == ((offestX / fscale) + x ) && mouseFixed.y == (offsetY / fscale) + y )
				highlighted = item;
			
		}
		else
		{
			DrawPartialDecal({ (float)(mouse.x - ((32 / 2)*fscale)) , (float)(mouse.y - ((32 / 2)*fscale)) }, item->Item->pSprite, { sprcoordX * 64,sprcoordY * 64 }, { 64,64 }, { 0.5f*fscale,0.5f*fscale });
			Grabitem = item;

		}

		}
		i++;
		}

		}
	

}

void RPG_Engine::moveIItems(olc::vi2d mouse, float x, float y, InventaryItem*& GrabItem,std::vector<InventaryItem*>& vector)
{
	const int cellsCoordX = mouse.x - (x / fscale);     // Coordinates x and y where first cell begins 0,0; 0,1; 0,2
	const int cellsCoordY = mouse.y - (y / fscale);

	bool bswitch = false;

	
	if (cellsCoordX >= 0 && cellsCoordY >= 0 && cellsCoordX <= 7 && cellsCoordY <= 2)
	{

		int currIndex = 8 * cellsCoordY + cellsCoordX; // cell in inventary that need to check on reserved  
		int GrabbedIndex = GrabItem->index;
		//int x = cellsCoordX - ((GrabItem->invnumber - 1) % 8);    //ere we calculate in local space
		//int y = cellsCoordY - ((GrabItem->invnumber - 1) / 8);




		

	

		if (currIndex != GrabbedIndex)   //for except point in one slot
		{



			vector[currIndex]->index = GrabItem->index; //assign index from grabbed object to that place where it shoild be dropped
			GrabItem->index = currIndex;   // assign  index too bject

			std::swap(vector[currIndex], vector[GrabbedIndex]);
			
		}
		
			

			

		


	}

		GrabItem->clearFlag(GrabItem->Grabitem);
}

void RPG_Engine::moveIItems(olc::vi2d mouse, float x, float y,float eqX,float eqY, InventaryItem*& grabitem, std::vector<InventaryItem*>& vector, std::vector<InventaryItem*>& EqVector)
{
	const int cellsInvCoordX = mouse.x - (x / fscale);     // Coordinates x and y where first cell in inv begins 0,0; 0,1; 0,2
	const int cellsInvCoordY = mouse.y - (y / fscale);


	const int cellsEqCoordX = mouse.x - (eqX / fscale);     // Coordinates x and y where first cell in eq begins 0,0; 0,1; 0,2
	const int cellsEqCoordY = mouse.y - (eqY / fscale);

	
	//std::cout << "Invenotry -" << '\t' << cellsInvCoordX << '\t' << cellsInvCoordY << std::endl;
	//std::cout << "Equip -" << '\t' << cellsEqCoordX << '\t' << cellsEqCoordY << std::endl;
//	std::cout << "Equip -" << '\t' << (int)mouse.x << '\t' << (int)mouse.y << std::endl;

	if (cellsEqCoordX >= 0 && cellsEqCoordY >= 0 && cellsEqCoordX <= 1 && cellsEqCoordY <= 2)   // Equip
	{
		int currIndex = 2 * cellsEqCoordY + cellsEqCoordX; // cell in Equip inventary that need to check on reserved  
		int GrabbedIndex = grabitem->index;
		if (!grabitem->checkFlag(grabitem->bEquiped)) // when not from eqVector
			if (EqVector[currIndex]->Item != nullptr)  // when we drop in empty slot
			{
				if (EqVector[currIndex]->Item->equipIndex == grabitem->Item->equipIndex) // when we drags weapon from eqinv to weapon in our inventary
				{
					cItem* help = EqVector[currIndex]->Item;


					EqVector[currIndex]->Item = grabitem->Item;
					vector[GrabbedIndex]->Item = help;




					AttachEq(EqVector, currIndex);



				}
			}
			else

			{
				if (grabitem->Item->equipIndex == currIndex + 1)  // for be shure that we drop exact equal equip they started from 1 so we add 1 for this
				{

					EqVector[currIndex]->Item = grabitem->Item;
					EqVector[currIndex]->setFlag(EqVector[currIndex]->Breserved);


					AttachEq(EqVector, currIndex);

					vector[GrabbedIndex]->Item = nullptr;
					vector[GrabbedIndex]->clearFlag(vector[GrabbedIndex]->Breserved);

				}

			}

	}

	if (cellsInvCoordX >= 0 && cellsInvCoordY >= 0 && cellsInvCoordX <= 7 && cellsInvCoordY <= 2 ) // frop in inventary
	{

		int currIndex = 8 * cellsInvCoordY + cellsInvCoordX; // cell in inventary that need to check on reserved  
		int GrabbedIndex = grabitem->index;
		//int x = cellsCoordX - ((GrabItem->invnumber - 1) % 8);    //ere we calculate in local space
		//int y = cellsCoordY - ((GrabItem->invnumber - 1) / 8);

		if (grabitem->checkFlag(grabitem->bEquiped)) // when  we droped equiped object in inventory
		{
			if (vector[currIndex]->Item !=nullptr)  // when we drop in empty slot
			{

			if (vector[currIndex]->Item->equipIndex == grabitem->Item->equipIndex)  // when we drop weapon from eqinv to weapon in our inventary
			{

				cItem* help = vector[currIndex]->Item;


				vector[currIndex]->Item = grabitem->Item;
				EqVector[GrabbedIndex]->Item = help;
				AttachEq(EqVector, GrabbedIndex);
			}

			}
			else
			{
				vector[currIndex]->Item = grabitem->Item;  
				vector[currIndex]->setFlag(vector[currIndex]->Breserved);

			//	EqVector[GrabbedIndex]->Item = RPG_Assets::get().GetItem("Empty");
				EqVector[GrabbedIndex]->Item = nullptr;

				AttachEq(EqVector, GrabbedIndex);
				EqVector[GrabbedIndex]->clearFlag(EqVector[GrabbedIndex]->Breserved);
			}


		}
		else
		{
			if (currIndex != GrabbedIndex)   //for except point in one slot
			{



				vector[currIndex]->index = grabitem->index; //assign index from grabbed object to that place where it shoild be dropped
				grabitem->index = currIndex;   // assign  index too bject

				std::swap(vector[currIndex], vector[GrabbedIndex]);

			}
		}
			
			
		






	}

	float test = static_cast<int>(CellSize * fscale);

	olc::vi2d mousehelp = { GetMouseX()-static_cast<int>(CellSize/2*fscale),GetMouseY()- static_cast<int>(CellSize/2 * fscale) };


	for (int i = 8; i < 10; i++)
	{

		// Сразу находим разницу векторов и вычисляем её длину
		if ((UIPosOnScreenAr[i] - mousehelp).mag() <4) /* пороговое значение расстояния */
		{
			
			if (grabitem->Item->equipIndex == 0)
			{
			m_vecUi[i-8] = grabitem;
			m_vecUi[i-8]->Uiindex = 0;
			}

		};

	
	}














	grabitem->clearFlag(grabitem->Grabitem);
	//grabitem->GrabItem = false;
}

bool RPG_Engine::AttachEq(std::vector<InventaryItem*>& EqVector,int Currindex)    // here two cases of work this method. 
{																				  // when we drop in slot equel index object we substract first object that was before
	InventaryItem* ex = nullptr;                                                  // than we add new object
	switch (Currindex)
	{

	case 0:
		ex = EqVector[0];
		if (dynamic_cast<cEquip*>(ex->Item)) {   //when slot is empty we point at specific empty pointer in pantir class
			m_pPlayer->SubstracAttributes(m_pPlayer->pEquipedNeck->Attributes);
			m_pPlayer->pEquipedNeck = (cEquip*)ex->Item;
			m_pPlayer->TakeAttributes(ex->Item->Attributes);
			m_pPlayer->MergeItemLvl();
			return true;
		}
		else
		{
			m_pPlayer->SubstracAttributes(m_pPlayer->pEquipedNeck->Attributes);       // substract that attributes what was in neck
			m_pPlayer->pEquipedNeck = m_pPlayer->pEmptySlot;                          //than say look at empty object 
			m_pPlayer->MergeItemLvl();                                                // in vector we have nullptr that let put there equel item
			return true;
		}

		break;

	case 1:
		ex = EqVector[1];
		if (dynamic_cast<cEquip*>(ex->Item)) {
			m_pPlayer->SubstracAttributes(m_pPlayer->pEquipedHelmet->Attributes);
			m_pPlayer->pEquipedHelmet = (cEquip*)ex->Item;
			m_pPlayer->TakeAttributes(ex->Item->Attributes);
			m_pPlayer->MergeItemLvl();
			return true;
		}
		else
		{
			m_pPlayer->SubstracAttributes(m_pPlayer->pEquipedHelmet->Attributes);
			m_pPlayer->pEquipedHelmet = m_pPlayer->pEmptySlot;
			m_pPlayer->MergeItemLvl();
			return true;
		}

		break;
	case 2:

		ex = EqVector[2];
		if (dynamic_cast<cEquip*>(ex->Item)) {
			m_pPlayer->SubstracAttributes(m_pPlayer->pEquipedWeapon->Attributes);
			m_pPlayer->pEquipedWeapon = (cWeapon*)ex->Item;
			m_pPlayer->TakeAttributes(ex->Item->Attributes);
			m_pPlayer->MergeItemLvl();
			return true;
		}
		else
		{
			m_pPlayer->SubstracAttributes(m_pPlayer->pEquipedWeapon->Attributes);
			m_pPlayer->pEquipedWeapon = m_pPlayer->pEmptySlot;
			m_pPlayer->MergeItemLvl();
			return true;
		}
		break;
	case 3:
		ex = EqVector[3];
		if (dynamic_cast<cEquip*>(ex->Item)) {
			m_pPlayer->SubstracAttributes(m_pPlayer->pEquipedChest->Attributes);
			m_pPlayer->pEquipedChest = (cEquip*)ex->Item;
			m_pPlayer->TakeAttributes(m_pPlayer->pEquipedChest->Attributes);
			m_pPlayer->MergeItemLvl();
			return true;
		}
		else
		{
			m_pPlayer->SubstracAttributes(m_pPlayer->pEquipedChest->Attributes);
			m_pPlayer->pEquipedChest = m_pPlayer->pEmptySlot;
			m_pPlayer->MergeItemLvl();
			return true;
		}
		break;
	case 4:

		ex = EqVector[4];
		if (dynamic_cast<cEquip*>(ex->Item)) {
			m_pPlayer->SubstracAttributes(m_pPlayer->pEquipedBack->Attributes);
			m_pPlayer->pEquipedBack = (cEquip*)ex->Item;
			m_pPlayer->TakeAttributes(m_pPlayer->pEquipedBack->Attributes);
			m_pPlayer->MergeItemLvl();
			return true;
		}
		else
		{
			m_pPlayer->SubstracAttributes(m_pPlayer->pEquipedBack->Attributes);
			m_pPlayer->pEquipedBack = m_pPlayer->pEmptySlot;
			m_pPlayer->MergeItemLvl();
			return true;
		}
		break;
	case 5:
		ex = EqVector[5];
		if (dynamic_cast<cEquip*>(ex->Item)) {
			m_pPlayer->SubstracAttributes(m_pPlayer->pEquipedBoots->Attributes);
			m_pPlayer->pEquipedBoots = (cEquip*)ex->Item;
			m_pPlayer->TakeAttributes(ex->Item->Attributes);
			m_pPlayer->MergeItemLvl();
			return true;
		}
		else
		{
			m_pPlayer->SubstracAttributes(m_pPlayer->pEquipedBoots->Attributes);
			m_pPlayer->pEquipedBoots = m_pPlayer->pEmptySlot;
			m_pPlayer->MergeItemLvl();
			return true;
		}
		break;


	default:
		return false;
		break;
	}

	


//	m_pPlayer->calculateStats();
}

void RPG_Engine::FillBatch(int TileIndex, int layer, int x, int y,  std::vector<TileInfo>& Before, std::vector<TileInfo>& After)
{
	if (TileIndex != 0) { // zero mean empty cell on texture
		// Compute source rectangle once


		int sx = TileIndex % 32;
		int sy = TileIndex / 32;

		// Compute position
		float posX = x;
		float posY = y;





		// Add to batch
		//  // Create a TileInfo instance and push it into the vector
		olc::vf2d pos = { posX,posY };
		olc::vf2d sourceRect;

		TileInfo tile;
		tile.textureIndex = TileIndex;
		tile.position = { posX, posY };
		tile.textureLayer = layer;
		// Determine if it's a dynamic layer
		if (layer == 2)
		{
			TileIndex -= 10; // we minus 10 here just becouse for indeces in map redactor i used integer for collision and there was occupied allredy before 10
			sx = TileIndex % 8;   // 32 cells - amount of cells in 2048/ 2048 texture - moduel give us X
			sy = TileIndex;   // divide give's us y
			tile.sourceRect = { (float)frameIndex * 256, (float)sy * 256 };

		}
		else
		{
			tile.sourceRect = { (float)sx * 64, (float)sy * 64 };
		};


		


		if (layer == 0)    // we have two baatches before chars and after chars
		{

			Before.push_back(tile);  //represent


		}
		else
		{

			After.push_back(tile); //represent

		}


		//batchBeforeCharsLayer.push_back(tile);
		//batch.push_back({ {posX, posY}, {(float)sx * 64, (float)sy * 64}}, bdynLayer);
	}
}
bool RPG_Engine::DrawPause(olc::vi2d mouse)
{
	if (bPause)
	{
		olc::vf2d quitPos{ (ScreenWidth() / 2) - (167 * fscale) ,ScreenHeight() / 2 + (31 * fscale) };
		DrawPartialDecal({ ((float)ScreenWidth() / 2) - (167 * fscale), (float)ScreenHeight() / 2 - (71 * fscale) }, D_FullUi, { 832,0 }, { 334,142 }, { fscale,fscale });
		//DrawBigText("Travel ?", (ScreenWidth() / 2) - 72, ScreenHeight() / 2 - 62, 1, 1, olc::WHITE);

		DrawBigText("Quit", quitPos.x, quitPos.y, 1 * fscale, 1 * fscale, olc::WHITE);

		DrawBigText("Options", (ScreenWidth() / 2) + ((167 - 144) * fscale), ScreenHeight() / 2 + (31 * fscale), 1 * fscale, 1 * fscale, olc::WHITE);



#ifdef  DEBUG_MODE

		SetPixelMode(olc::Pixel::NORMAL);
		EnableLayer(layer, true);
		SetDrawTarget(nullptr);
#endif // DEBUG


		DrawRect((ScreenWidth() / 2) - (167 * fscale), ScreenHeight() / 2 + (31 * fscale), 40, 20);


		if (GetMouse(0).bPressed) //LeftMouse
		{

			if ((int)mouse.x >= (int)quitPos.x && (int)mouse.y >= (int)quitPos.y && (int)mouse.x <= (int)quitPos.x + 40 && (int)mouse.y <= (int)quitPos.y + 40)
			{
				return false;
			}

		}
		Drawcursor(mouse);
		DrawBigText("Pause", ScreenWidth() / 2 - (45 * fscale), ScreenHeight() / 2 - (71 * fscale), 1 * fscale, 1 * fscale, olc::WHITE);

	}
	return true;
}


void RPG_Engine::ClearAbsorbedSlots(std::vector<InventaryItem*>& m_listItems)
{
	
	


	for (int i = 0; i <absorbCounter; i++)
	{
	

		for (auto it2 = m_listItems.begin(); it2 != m_listItems.end(); it2++)  // we go checking all list
		{
			InventaryItem* check = *it2;   // add access to data


			if (check->checkFlag(check->Babsorbed))   // check on absorb
			{
				//check->Absorbed = false;
				check->clearFlag(check->Babsorbed);
				check->Item = nullptr;
				//cEmptySlot* empty = new cEmptySlot();     //create empty space clost 
				//empty->index = check->index;    //copy index in empty

				//m_listItems[check->index] = empty;  // record in vector  created object on chek position;
				
				//delete check;
				break;
			}

		}

	}
	absorbCounter = 0;
}

bool RPG_Engine::UpdateShop(float fElapsedTime, olc::vi2d& mouse)                                                                                                       // < ----SHOP
{
	ClearAbsorbedSlots(m_listItems);

	olc::vi2d mousefix = { (mouse.x / (int)(CellSize * fscale)),(mouse.y / (int)(CellSize * fscale)) };



	olc::vf2d inventorysockets = { 0,250 };  // coordinates whith inv name
	
	

	DrawPartialDecal({ (float)(4*fscale) * CellSize, (float)(8 * fscale) * CellSize }, D_Inventory, inventorysockets, {512.0f,267.0f}, {0.5f*fscale,0.5f*fscale});

	


	//DrawPartialDecal({ (float)5.85 * 64*fscale, (float)(11) * 64*fscale }, RPG_Assets::get().GetSprite("inventoryName"), { 0,0 }, { 274,75 },{fscale,fscale});


	DrawPartialDecal({ (float)(14 * fscale) * CellSize, (float)(8 * fscale) * CellSize }, D_Inventory, inventorysockets, { 512,192}, { 0.5f*fscale,0.5f*fscale });  // sockets 
	DrawPartialDecal({ (float)(15.85 * fscale) * CellSize, (float)(11 * fscale) * CellSize }, D_Inventory, { 724,0 }, { 275,75 },{0.5f*fscale,0.5f*fscale}); //storeName




	
	
	 highlighted = nullptr;
	 GrabItem = nullptr;

	float squeY = 8*fscale;
	float squeX = 4*fscale;
	float storesqueX = 14*fscale;

	DrawInventory(squeX, squeY, mouse, mousefix, highlighted, GrabItem);




	DrawStoreInventory(storesqueX, squeY, mousefix, highlighted);


	




	if (GetMouse(0).bReleased)
	{
		if (GrabItem != nullptr)
		{

			moveIItems(mousefix, squeX, squeY, GrabItem, m_listItems);
			
		}
	}







	if (highlighted != nullptr)
	{


		if (GetMouse(0).bPressed)
		{

			//grab = true;
		//	GrabItem = highlighted;

			//highlighted->GrabItem = true;
			highlighted->setFlag(highlighted->Grabitem);
			//highlighted = nullptr;
		}


		DrawDescriptionPattern(highlighted, mouse,mousefix);
		
		//std::cout << (int)mousefix.x << '\t' << (int)mousefix.y << '\t' << squeX / fscale << '\t' << squeX / fscale + 8 << '\t' << squeY / fscale <<'\t' << squeY / fscale + 3 << std::endl;

			if (GetMouse(1).bPressed)
		{
				if ((int)mousefix.x >= squeX/fscale && (int)mousefix.x <=squeX/fscale+8 && (int)mousefix.y  >= squeY/fscale && (int)mousefix.y <= squeY/fscale+3)  // Inventory decription
				{

					// Use selected item 
					if (!highlighted->Item->bKeyItem)        //Try Sell Item
					{
						highlighted->Item->OnSell();

						if (SaleItem(highlighted->Item->sTradePrice*highlighted->currStacks, highlighted))
						{
							// Item has signalled it must be consumed, so remove it
							//TakeItem(highlighted);
							
						}
					}
				}

				if ((int)mousefix.x >= storesqueX/fscale && (int)mousefix.x <= storesqueX/fscale+8 && (int)mousefix.y  >= squeY/fscale && (int)mousefix.y <= squeY/fscale+3)  // Store decription
				{
					// Use selected item 
					if (!highlighted->Item->bKeyItem)        //Try Sell Item
					{


						if (highlighted->Item->OnBuy())     // must cheking on amount money ant consume them
						{
							
						}
					}
				}

		}
			
			

	}
	// MONEY
	squeX = (4*fscale) * CellSize;
	squeY = (7*fscale)* CellSize;
	DrawBigText(std::to_string(Money), squeX + (CellSize*fscale), squeY + (8*fscale), 0.5f*fscale, 0.5f*fscale, olc::YELLOW); //Money
	
	DrawPartialDecal({ (float)squeX, (float)squeY }, D_Items, {3.0f*64.0f,0}, {(float)64,(float)64}, {0.5f*fscale,0.5f*fscale});  //Gold
	//


//Mouse
	Drawcursor(mouse);
	//DrawPartialDecal({ (float)(mouse.x - 0.15f) , (float)(mouse.y - 0.1f)  }, RPG_Assets::get().GetSprite("Pantir's Dagger"), { 0,0 }, { (float)64,(float)(64) },{fscale,fscale});





	return true;
}


void RPG_Engine::DrawStoreInventory(float sX, float sY, olc::vi2d mousefix, InventaryItem*& highlighted)
{

	int x = 0;
	int y = 0;
	int i = 0;

	for (auto& item : m_listStoreItems)
	{

		if (item->Item != nullptr)
		{

			if (item->checkFlag(item->Breserved))
			{

				x = i % 8;

				y = i / 8;

				float sprcoordX = item->Item->spriteindex % 32;
				float sprcoordY = item->Item->spriteindex / 32;

				i++;

				DrawPartialDecal({ (float)(sX + x * fscale) * CellSize, (sY + y * fscale) * CellSize }, item->Item->pSprite, { sprcoordX*64, sprcoordY*64 }, { 64,64 }, { 0.5f*fscale,0.5f*fscale });
				//	item->invnumber = i;
					//item->ipy = y;



				if ((int)mousefix.x == ((sX / fscale) + x) && (int)mousefix.y == ((sY / fscale) + y))
					highlighted = item;
			}
		}
	}
}


bool RPG_Engine::UpdateProfession(float fElapsedTime, olc::vi2d& mouse )   //<--profession
{

	//olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };

	const float TalentPlateWidth = 960;
	const float TalentPlateHeigh = 768;


	float fixsSize = 0.5f * fscale;

	float BaseposX = (float)(ScreenWidth() / 2 - ((TalentPlateWidth / 2)* fixsSize));
	float BasePosY = (float)(ScreenHeight() / 2 - ((TalentPlateHeigh / 2)* fixsSize));

	auto& ProfBack = GetSpriteData(DataStruct::ProfessionBack);

	DrawPartialDecal({ BaseposX,  BasePosY }, D_FullUi, ProfBack.Pos,ProfBack.Size, { fixsSize,fixsSize });





	/////////  STICK DRAWING /////  START

	//Low-Mid Attack
	DrawPartialDecal({ BaseposX + (222* fixsSize),  BasePosY + (576* fixsSize) }, D_FullUi, { 1227,0 }, { (float)4,(float)74 }, { fixsSize,fixsSize });

	if(GetLearnedTalent(2)) //  <-- MidAttack 
	{
	DrawPartialDecal({ BaseposX + (222* fixsSize),  BasePosY + (576* fixsSize) }, D_FullUi, { 1223,0 }, { (float)4,(float)74 }, { fixsSize,fixsSize });

	//Mid-High Attack
	DrawPartialDecal({ BaseposX + (126* fixsSize),  BasePosY + (542* fixsSize) }, D_FullUi, { 0,910 }, { (float)74,(float)4 }, { fixsSize,fixsSize });
	}
	if(GetLearnedTalent(3))
	DrawPartialDecal({ BaseposX + (126* fixsSize),  BasePosY + (542* fixsSize) }, D_FullUi, { 0,914 }, { (float)74,(float)4 }, { fixsSize,fixsSize });


	//BackStab-ShadowStab Attack
	if (GetLearnedTalent(4))
	{

		DrawPartialDecal({ BaseposX + (31* fixsSize),  BasePosY + (670* fixsSize) }, D_FullUi, { 0,910 }, { (float)74,(float)4 }, { fixsSize,fixsSize });
		DrawPartialDecal({ BaseposX + (31* fixsSize),  BasePosY + (414* fixsSize) }, D_FullUi, { 0,910 }, { (float)74,(float)4 }, { fixsSize,fixsSize });
		DrawPartialDecal({ BaseposX + (30* fixsSize),  BasePosY + (415* fixsSize) }, D_FullUi, { 1227,0 }, { (float)4,(float)257 }, { fixsSize,fixsSize });

	}
	if (GetLearnedTalent(5))
	{

		DrawPartialDecal({ BaseposX + (31 * fixsSize),  BasePosY + (670 * fixsSize) }, D_FullUi, { 0,914 }, { (float)74,(float)4 }, { fixsSize,fixsSize });
	DrawPartialDecal({ BaseposX + (31 * fixsSize),  BasePosY + (414 * fixsSize) }, D_FullUi, { 0,914 }, { (float)74,(float)4 }, { fixsSize,fixsSize });
	DrawPartialDecal({ BaseposX + (30* fixsSize),  BasePosY + (415* fixsSize) }, D_FullUi, { 1223,0 }, { (float)4,(float)257 }, { fixsSize,fixsSize });
	DrawPartialDecal({ BaseposX + (95* fixsSize),  BasePosY + (251* fixsSize) }, D_FullUi, { 1227,0 }, { (float)4,(float)138 }, { fixsSize,fixsSize });
	}

	if (GetLearnedTalent(6))
	{
		DrawPartialDecal({ BaseposX + (95* fixsSize),  BasePosY + (251* fixsSize) }, D_FullUi, { 1223,0 }, { (float)4,(float)138 }, { fixsSize,fixsSize });
	}


	//RighAttack-RightAirAttack
	DrawPartialDecal({ BaseposX + (351 * fixsSize),  BasePosY + (572 * fixsSize) }, D_FullUi, { 1227,0 }, { (float)4,(float)74 }, { fixsSize,fixsSize });
	if (GetLearnedTalent(12))
	{

	DrawPartialDecal({ BaseposX + (351* fixsSize),  BasePosY + (572* fixsSize) }, D_FullUi, { 1223,0 }, { (float)4,(float)74 }, { fixsSize,fixsSize }, olc::Pixel(255, 0, 0));   //offset pulling player back into the screen
	
	DrawPartialDecal({ BaseposX + (351* fixsSize),  BasePosY +(315* fixsSize) }, D_FullUi, { 1227,0 }, { (float)4,(float)202 }, { fixsSize,fixsSize }, olc::Pixel(255, 0, 0));   //offset pulling player back into the screen


	}

	//Flight//Landing
	if (GetLearnedTalent(13))
	{
	DrawPartialDecal({ BaseposX + (351* fixsSize),  BasePosY + (315* fixsSize) }, D_FullUi, { 1223,0 }, { (float)4,(float)202 }, { fixsSize,fixsSize }, olc::Pixel(255, 0, 0));   //offset pulling player back into the screen

	DrawPartialDecal({ BaseposX + (351* fixsSize),  BasePosY + (123* fixsSize) }, D_FullUi, { 1227,0 }, { (float)4,(float)138 }, { fixsSize,fixsSize }, olc::Pixel(255, 0, 0));   //offset pulling player back into the screen
	}
	if (GetLearnedTalent(14))
	DrawPartialDecal({ BaseposX + (351* fixsSize),  BasePosY + (123* fixsSize) }, D_FullUi, { 1223,0 }, { (float)4,(float)138 }, { fixsSize,fixsSize }, olc::Pixel(255, 0, 0));   //offset pulling player back into the screen
	


	/////////  STICK DRAWING /////  END
	


	for (auto& source : { &m_vecTalentsBars })    // DRAW Ui elements
		for (auto& dyns : *source)
			dyns->DrawSelfTalent(this, BaseposX , BasePosY );





	for (auto i = 0; i < TalentPositions.size(); i++)
	{

		if (mouse.x >= BaseposX + TalentPositions[i].x * fixsSize && mouse.x <= BaseposX + (TalentPositions[i].x + 64) * fixsSize && mouse.y >= BasePosY + TalentPositions[i].y * fixsSize && mouse.y <= BasePosY + (TalentPositions[i].y + 64) * fixsSize)
		{
			DrawPartialDecal({ BaseposX + (TalentPositions[i].x * fixsSize), BasePosY + (TalentPositions[i].y * fixsSize) }, D_Items, { 256,0 }, { (float)64,(float)(64) }, { fixsSize,fixsSize });
			break;
		}



	};

	


	/// SELECT RECTANGLE END




	if (GetMouse(0).bPressed)  // when we learn spell
	{


		if (TalentPoint>0)
		{


		for (auto i = 0; i < TalentPositions.size(); i++)
		{

			if (mouse.x >= BaseposX + TalentPositions[i].x * fixsSize && mouse.x <= BaseposX + (TalentPositions[i].x + 64) * fixsSize && mouse.y >= BasePosY + TalentPositions[i].y * fixsSize && mouse.y <= BasePosY + (TalentPositions[i].y + 64) * fixsSize)
			{
				TalentPoint--;

				m_vecSaveTalents.push_back(TalentSavePoints[i]);
				break;
			}

		};

		}


	}



	Drawcursor(mouse);



	return true;
}


bool RPG_Engine::UpdateBlackSmith(float fElapsedTime, olc::vi2d& mouse)
{
	ClearAbsorbedSlots(m_listItems);

	
//	olc::vi2d mouse = { GetMouseX(),GetMouseY() };
	olc::vi2d mousefix = { (mouse.x / (int)(CellSize * fscale)),(mouse.y / (int)(CellSize * fscale)) };

	
	
	float fixScale = 0.5f * fscale;

	 float squeY = 8 * fscale;
	 float squeX = 4 * fscale;
	 const float SmithsqueX = 14 * fscale;


	const auto UiPlatform = GetSpriteData(DataStruct::UiPlatform);
	const auto ReqPlatform = GetSpriteData(DataStruct::Required);

	const float DescrUiWidth = (UiPlatform.Size.x * fixScale) * 0.5f;
	const float DescrUiHeigh = (UiPlatform.Size.y * fixScale) * 0.6f;

	const float ReqSocketsY = (ReqPlatform.Size.y / 64.0f) * fscale;
	const float RqSocketsX = (ReqPlatform.Size.x / 64.0f) * fscale;



	DrawElement(DataStruct::inventarySockets, { squeX,squeY }, fixScale, D_Inventory);
	//DrawPartialDecal({ squeX * 64, squeY * 64 }, D_Inventory, { InvData_ar[static_cast<int>(DataStruct::inventarySockets)].Pos.x,InvData_ar[static_cast<int>(DataStruct::inventarySockets)].Pos.y }, { InvData_ar[static_cast<int>(DataStruct::inventarySockets)].Size.x,InvData_ar[static_cast<int>(DataStruct::inventarySockets)].Size.y }, {fscale,fscale}); //Sockets

	//DrawPartialDecal({ (float)(5.85 * fscale) * 64, (float)(11 * fscale) * 64 }, RPG_Assets::get().GetSprite("inventoryName"), { 0,0 }, { 274,75 }, { fscale,fscale });

	DrawElement(DataStruct::inventarySockets, { SmithsqueX,squeY }, fixScale, D_Inventory);

	//DrawPartialDecal({ SmithsqueX * 64, squeY * 64 }, D_Inventory, { InvData_ar[inventarySockets].Pos.x,InvData_ar[inventarySockets].Pos.y }, { InvData_ar[inventarySockets].Size.x,InvData_ar[inventarySockets].Size.y }, { fscale,fscale }); //sockets
	//DrawPartialDecal({ (float)(15.85 * fscale) * 64, (float)(11 * fscale) * 64 }, D_Inventory, { 0,0 }, { InvData_ar[DataStruct::WarehouseLogo].Size.x,InvData_ar[DataStruct::WarehouseLogo].Size.y }, { fscale,fscale });





	// Draw Consumables


	InventaryItem* Selected = nullptr;

	highlighted = nullptr;
	GrabItem = nullptr;


	

	 DrawInventory(squeX, squeY, mouse, mousefix, highlighted,GrabItem);
	 DrawBlacksmithInentory(SmithsqueX, squeY, mousefix, Selected, highlighted);

	float CraftBottomX = (SmithsqueX + RqSocketsX) * CellSize + (DescrUiWidth / 5);
	float CraftBottomY = squeY * CellSize - DescrUiHeigh + (DescrUiHeigh / 3);

	

	if (highlighted != nullptr)
	{

		DrawDescriptionPattern(highlighted, mouse, mousefix);


	}
	
	
	if ((int)mousefix.x >= SmithsqueX/fscale) // half screeb
	{

		

		if (GetMouse(0).bPressed)
		{


			if (highlighted != nullptr)
			{
				if (Selected != nullptr)
					Selected->clearFlag(Selected->Objectselected);

				//highlighted->Objectselect = true;  // Here mousegrabed works in two dif case. in our invenotry we grabbed in blacksmith we craft
				highlighted->setFlag(highlighted->Objectselected);
				Selected = highlighted;
			}
			else
			{
				if (Selected != nullptr)
				{
				//	if (GetMouse(1).bPressed)
						Selected->clearFlag(Selected->Objectselected);
						//Selected->Objectselect = false;
						
					if (mousefix.x >= CraftBottomX / (64 * fixScale)-0.5 && mousefix.x <= CraftBottomX / (64 * fixScale) + 0.5 && mousefix.y >= CraftBottomY / (64 * fixScale)-0.5 && mousefix.y <= CraftBottomY / (64 * fixScale) + 0.5)  // Craft Word description
					{



						// Use selected item 
						if (!Selected->Item->bKeyItem)        //Try Sell Item
						{


							if (Selected->Item->OnCraft(1))     // must cheking on objects on onventory ant consume them
							{

								TakeItem(Selected->Item->sCraftRequaries);

							}
						}
					}


				}
			}

		}
	}
	else
	{
		if (highlighted != nullptr)
		{
		if (GetMouse(1).bPressed)
			// Use selected item 
			if (!highlighted->Item->bKeyItem)
			{
				if (highlighted->Item->OnUse(m_pPlayer, m_listItems, highlighted))
				{
					ClearAbsorbedSlots(m_listItems); // find absorbed object and change them on empty sockets
					// Item has signalled it must be consumed, so remove it
					//TakeItem(highlighted,m_listItems);
				}
			}


		if (GetMouse(0).bPressed)
			{


			highlighted->setFlag(highlighted->Grabitem);
				//highlighted->GrabItem = true;
			
			}
		}

	}
	if (GetMouse(0).bReleased)
	{
		if (GrabItem != nullptr)
		{


			moveIItems(mousefix, squeX, squeY, GrabItem, m_listItems);
			
		}
	}



	if (Selected != nullptr)
	{
		



		//


		DrawElement(DataStruct::UiPlatform, { SmithsqueX + RqSocketsX,squeY - ReqSocketsY+1.2f }, 0.5f*fixScale, D_FullUi);

	//	DrawPartialDecal({ (SmithsqueX + RqSocketsX) * 64, squeY * 64 - DescrUiHeigh }, SpellUi, { 0,0 }, { 334,142 }, { 0.5f * fscale,0.6f * fscale });

		if (Selected->Item->OnCraft(0))
		{
			DrawBigText("Craft", CraftBottomX, CraftBottomY, fixScale, fixScale, { 169,223,227 });

		}
		else
		{
			DrawBigText("Craft", CraftBottomX, CraftBottomY, fixScale, fixScale, olc::GREY);
		}
		
	//	DrawPartialDecal({ (SmithsqueX + ((Selected->index % 8)) * fscale) * CellSize, (float)(squeY + ((Selected->index / 8) * fscale)) * CellSize }, D_Items, { 0,0 }, { (float)64,(float)(64) }, { fixScale,fixScale });
		//DrawPartialDecal({ SmithsqueX * 64, (float)(squeY - ReqSocketsY) * 64 }, d, { 0,0 }, { 192,128 }, { fscale,fscale });

		DrawElement(DataStruct::Required, { SmithsqueX ,squeY-ReqSocketsY}, fixScale, D_Inventory);
	//	DrawPartialDecal({ SmithsqueX * 64 - (12 * fscale), (float)(squeY - ReqSocketsY) * 64 - ((75 + 1) * fscale) }, RPG_Assets::get().GetSprite("RequiredName"), { 0,0 }, { 274,75 }, { fscale,fscale });

		//std::cout << (int)(mouse.x / (64 * fscale)) * fscale << '\t' << (int)(mouse.y / (64 * fscale)) * fscale << std::endl;
		


		 DrawCraftedRequires(SmithsqueX, squeY - ReqSocketsY+0.6f, mousefix,Selected,highlighted);

	}






		// MONEY
		squeX = (4 * fscale) * CellSize;
		squeY = (7 * fscale) * CellSize;
		DrawBigText(std::to_string(Money), squeX + (64 * fixScale), squeY + (16 * fixScale), fixScale, fixScale, olc::YELLOW); //Money
		DrawPartialDecal({ (float)squeX, (float)squeY }, D_Items, { 3*64,0 }, { (float)64,(float)64 }, { 0.5f*fscale,0.5f*fscale });
		//



		Drawcursor(mouse);
		//Mouse
		//DrawPartialDecal({ (float)(mouse.x - 0.15f), (float)(mouse.y - 0.1f) }, RPG_Assets::get().GetSprite("Pantir's Dagger"), { 0,0 }, { (float)64,(float)(64) }, { fscale,fscale });




		return true;
}

void RPG_Engine::DrawBlacksmithInentory(float offestX, float offsetY, olc::vi2d mouse, InventaryItem*& Selected,InventaryItem*& Highlighted)
{
	int i = 0;
	int x = 0;
	int y = 0;
	

	for (auto& item : m_listBlackSmithItems)
	{
		if (item->Item ==nullptr)
		{
			break;
		}

		x = i % 8;

		y = i / 8;
		float sprcoordX = item->Item->spriteindex % 32;
		float sprcoordY = item->Item->spriteindex / 32;
		i++;


		DrawPartialDecal({ (float)(offestX + (x ) * fscale) * CellSize, (offsetY + (float)y  * fscale) * CellSize }, item->Item->pSprite, { sprcoordX*64,sprcoordY*64 }, { 64,64 }, { 0.5f*fscale,0.5f * fscale });

		if (item->checkFlag(item->Objectselected))  // item has boolean so thank's that we can find selected object
			Selected = item;


		if (mouse.x == ((offestX / fscale) + x ) && mouse.y == (offsetY / fscale) + y )
			Highlighted= item;
	}

}



	

void RPG_Engine::DrawCraftedRequires(float offestX, float offsetY, olc::vi2d mouse,  InventaryItem*& Selected, InventaryItem*& Highlighted)
{
	std::string ItemName;
	std::string ItemAmount;

	int counter = 0;
	int domain = 0;
	int x = 0;
	int y = 0;
	int i = 0;

	for (auto c : Selected->Item->sCraftRequaries)
	{

		if (c == 10)    //<---  \n  mean end first object
		{
			
			cItem& decal = (*RPG_Assets::get().GetItem(ItemName));
			x = i % 2;
			y = i / 2;
			
			float sprcoordX = decal.spriteindex % 32;
			float sprcoordY = decal.spriteindex / 32;


			i++;
			DrawPartialDecal({ (float)(offestX + (x * fscale)) * CellSize, (float)(offsetY + (y * fscale)) * CellSize }, RPG_Assets::get().GetItem(ItemName)->pSprite, { sprcoordX*64,sprcoordY*64 }, { 64,64 }, { 0.5f*fscale,0.5f*fscale });


			if (mouse.x == (offestX + x ) && mouse.y == (offsetY + y ))
				Highlighted->Item = RPG_Assets::get().GetItem(ItemName);


			for (auto& item : m_listItems)   // Count in our inventory this item
			{
				if (item->Item != nullptr)
				{

				if (item->Item->sName == ItemName)
				{
					domain++;
				}
				}
			}


			DrawBigText(std::to_string(domain) + "/" + ItemAmount, (offestX + (x * fscale)) * CellSize, (offsetY + ((y + 0.5f) * fscale)) * CellSize, 0.25f * fscale, 0.25f * fscale, { 169,223,227 });
			counter = 0;
			ItemName.clear();
			ItemAmount.clear();
			continue;

		}

		if (c >= 48 && c <= 57)  // в ASCII это 0 - 10 включительео
		{
			ItemAmount += c;
		}
		else
		{
			ItemName += c;  // gather name by latters

		}
		counter++;
		domain = 0;
	}


}


void RPG_Engine::DrawDescriptionPattern(InventaryItem* highlighted, olc::vi2d mouse, olc::vf2d mousefix)
{

	
		float nLinesX = 0;
		int nLinesY = 3;
		uint8_t lineCounter = 0;
		float nsaveLine2 = 0;
		//const float AddlineX = 32;
		 float DiscripTex = 0;
		std::string Stext;
		//std::string SCntuse = "(Can't Use)";
		//std::string SSale = "(Can't Use)";

		for (auto c : highlighted->Item->sDescription)
		{

			if (c == 10)  // <-- 10 is /n
			{
				if (nsaveLine2 <= lineCounter)
				{
					nLinesX = 32 + (lineCounter * 18);
					nsaveLine2 = lineCounter;
					lineCounter = 0;
				}
				nLinesY++;
				continue;
			}

			lineCounter++;
		}
		
			
		nLinesX = nLinesX * 0.5f * fscale;
		nLinesY = (nLinesY)*CellSize * 0.5f * fscale;
		const float mouseoffset = (CellSize * fscale);
		
		DrawPartialDecal({ (float)(mouse.x + mouseoffset) , (float)(mouse.y + mouseoffset) }, RPG_Assets::get().GetSprite("DescriptionPattern"), { 0,0 }, { (float)nLinesX,(float)(nLinesY)});
	

		//DrawString(4 * 64, squeY - 64, highlighted->sName, olc::WHITE,3);
		DrawBigText(highlighted->Item->sName, (mouse.x + mouseoffset), mouse.y + mouseoffset, 0.5f*fscale, 0.5f * fscale);

		//DrawString(8, 190, "SELECTED", olc::WHITE,3);
		//DrawString(4*64, squeY-32, highlighted->sDescription, olc::WHITE,2);

		DrawBigText(highlighted->Item->sDescription, (mouse.x + mouseoffset), mouse.y + (32*fscale) + mouseoffset, 0.5f * fscale, 0.5f * fscale, { 169,223,227 });

		




		switch (n_nGameMode)
		{
		case MODE_TITLE:
			break;
		case MODE_LOCAL_MAP:
			break;
		case MODE_WAREHOUSE:
		case MODE_BLACKSMITH:
		case MODE_INVENTORY:
			// Warehouse.
			if (!highlighted->Item->equipIndex>0)
			{

			if (!highlighted->Item->bKeyItem )
			{
				//DrawString(80, squeY-160, "(Press SPACE to use)", olc::WHITE,2);
				DrawBigText("(Useable)", (mouse.x + (CellSize * fscale)), (mouse.y + ((nLinesY + 1)*fscale) * (CellSize*fscale)), 0.4f * fscale, 0.4f * fscale, { 218,14,0,255 });
			}
			else
			{
				DrawBigText("(Can't Use)", (mouse.x + (CellSize * fscale)), (mouse.y + ((nLinesY + 1)*fscale) * (CellSize*fscale)) , 0.4f * fscale, 0.4f*fscale, { 218,14,0,255 });
			}
			}
			//
			break;
		case MODE_SHOP:
			//shop
			if ((int)mousefix.x >= 4  && (int)mousefix.x <= 4 + 8 && (int)mousefix.y >= 8  && (int)mousefix.y <= 8 + 3)  // Inventory decription
			{
				if (!highlighted->Item->bKeyItem)
				{
					Stext.clear();

					Stext = "Sale:" + std::to_string(highlighted->Item->sTradePrice * highlighted->currStacks);
					DiscripTex = (18+ Stext.size() * 18) * 0.5f * fscale;
					//nLinesY = -32;

					DrawPartialDecal({ (float)(mouse.x + mouseoffset + nLinesX - DiscripTex) , (float)(mouse.y + mouseoffset + nLinesY) }, RPG_Assets::get().GetSprite("DescriptionPattern"), { 0,0 }, { (float)DiscripTex,(float)CellSize*fscale });

					DrawBigText(Stext , (mouse.x+mouseoffset+nLinesX - DiscripTex ), (mouse.y+mouseoffset+nLinesY), 0.5f*fscale, 0.5f*fscale, olc::YELLOW); //Money
				}
				else
				{
					DrawBigText("(Can't Use)", (mouse.x + (CellSize * fscale)), (mouse.y + ((nLinesY + 1) * fscale) * (CellSize * fscale)), 0.4f*fscale, 0.4f * fscale, {218,14,0,255});
				}
			}

			if ((int)mousefix.x >= 14  && (int)mousefix.x <= 14  + 8 && (int)mousefix.y >= 8 && (int)mousefix.y <= 8  + 3)  // Store decription
			{

				Stext.clear();
				Stext = "Buy:" + std::to_string(highlighted->Item->sTradeBuyPrice);
				DiscripTex = (18 + Stext.size() * 18) * 0.5f * fscale;

				DrawPartialDecal({ (float)(mouse.x + mouseoffset + nLinesX - DiscripTex) , (float)(mouse.y + mouseoffset + nLinesY) }, RPG_Assets::get().GetSprite("DescriptionPattern"), { 0,0 }, { (float)DiscripTex,(float)CellSize * fscale });

				DrawBigText(Stext, (mouse.x + mouseoffset + nLinesX - DiscripTex), (mouse.y + mouseoffset + nLinesY), 0.5f * fscale, 0.5f * fscale, olc::YELLOW); //Money
			}
			break;
		case MODE_PROFESSION:
			
			break;
		case MODE_MAP:
			
			break;
		
		}


	

}


bool RPG_Engine::UpdateMap(float fElapsedTime, olc::vi2d& mouse)
{

	
//	olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY())};
	
	
	const auto& DMap = GetSpriteData(DataStruct::Map);

	const float OffsetX = (ScreenWidth() / 2.0f - (DMap.Size.x/2)*fscale);
	const float OffsetY = (ScreenHeight() / 50.0f);

	const float calculatedOffsetX = (float)ScreenWidth() / 2 - (OffsetX * fscale);

	DrawElement(DataStruct::Map, { OffsetX /CellSize,OffsetY / CellSize }, fscale, D_Map);

	//DrawPartialDecal({ calculatedOffsetX, ((float)Yoffset)*fscale}, RPG_Assets::get().GetSprite("MapLayer"), {0,0}, {962,941},{fscale,fscale});

//	DrawPartialDecal({ (calculatedOffsetX +(100*fscale)), (float)494*fscale }, RPG_Assets::get().GetSprite("VillageForestScetch"), { 0,0 }, { 285,241 }, { fscale,fscale });


	int highlighted = 0;
	for (int i = 0; i < MapSpritesIndexes.size(); i++)
	{


		if (mouse.x >= OffsetX + MapPosAr[i].x * fscale && mouse.x <= OffsetX + (MapPosAr[i].x + SpritesData_map[MapSpritesIndexes[i]].Size.x) * fscale &&
			mouse.y >= OffsetY + MapPosAr[i].y * fscale && mouse.y <= OffsetY + (MapPosAr[i].y + SpritesData_map[MapSpritesIndexes[i]].Size.y) * fscale) {

			highlighted = i + 1;
			DrawElement(MapSpritesIndexes[i], { (OffsetX + MapPosAr[i].x*fscale)/CellSize ,(OffsetY + MapPosAr[i].y*fscale)/CellSize }, fscale, D_Map);

		}
	


	}




	if (GetMouse(0).bPressed)
	{
		if (!bOpenTravelAsk)
		{
			if (highlighted>0)
			{
				bOpenTravelAsk = true;
				SelectedTeleport = highlighted;
			}

		}

		if (bOpenTravelAsk)
		{
			
			if (mouse.x >= (ScreenWidth() / 2) + (160 - 36 - 7)*fscale && mouse.x <= (ScreenWidth() / 2) + (160 - 36 - 7+64)*fscale && mouse.y >= ScreenHeight() / 2 + 31*fscale && mouse.y <= ScreenHeight() / 2 + (31+32)*fscale) //No
			{

				bOpenTravelAsk = false;
				SelectedTeleport = 0;
			}


			if (mouse.x >= (ScreenWidth() / 2) - 160*fscale  && mouse.x <= (ScreenWidth() / 2) - (160 -72)*fscale && mouse.y >= ScreenHeight() / 2 + 31*fscale && mouse.y <= ScreenHeight() / 2 + (31 + 32)*fscale) //Yes
			{
				switch (SelectedTeleport)
				{
				case 1:
					ChangeMap("Village", 5, 12);
					n_nGameMode = MODE_LOCAL_MAP;     // <-----
					break;
				case 2:
					ChangeMap("Mountains", 5, 12);
					n_nGameMode = MODE_LOCAL_MAP;     // <-----
					break;
				case 3:
					//ChangeMap("DesertVale", 5, 12);
					break;
				case 4:
					//ChangeMap("HorseManBridge", 5, 12);
					break;
				case 5:
					//ChangeMap("Monastery", 5, 12);
					break;

				}
				
				//ChangeMap("DesertVale", 5, 12);
			}



		}
	}


	if (bOpenTravelAsk)
	{
		const auto& SPellUi = GetSpriteData(DataStruct::UiPlatform);


		DrawElement(DataStruct::UiPlatform, { ((float)ScreenWidth() / 2 - 167 * fscale) / CellSize, ((float)ScreenHeight() / 2 - 71 * fscale) / CellSize }, fscale, D_FullUi);
	//DrawPartialDecal({ ((float)ScreenWidth() / 2 )-167*fscale, (float)ScreenHeight()/2-71*fscale}, RPG_Assets::get().GetSprite("SpellUi"), {0,0}, {334,142});
	DrawBigText("Travel ?", (ScreenWidth() / 2) -72*fscale, ScreenHeight() / 2 -62*fscale , fscale,fscale,olc::WHITE);

	DrawBigText("Yes", (ScreenWidth() / 2) - 160*fscale, ScreenHeight() / 2 + 31*fscale, fscale, fscale, olc::WHITE);
	DrawBigText("No", (ScreenWidth() / 2) + (160-36 -7)*fscale, ScreenHeight() / 2 + 31*fscale, fscale, fscale, olc::WHITE);
	}


	



	//Mouse
	Drawcursor(mouse);



	return true;
}

bool RPG_Engine::SaveFunction()
{


 	std::ofstream data("Load/CurrSave.bin", std::ios::binary);
	if (!data.is_open())
	{
		std::cout << "Save Denied" << std::endl;
		return false;
	}

	uint32_t AmountQuests = 0;
	uint32_t AmountTalents = 0;

	// Записываем основные параметры
	int lvl = GetLvl();
	data.write(reinterpret_cast<const char*>(&lvl), sizeof(int));

	int currExp = GetCurrExp();
	data.write(reinterpret_cast<const char*>(&currExp), sizeof(int));

	int requiredExp = GetRequredExp();
	data.write(reinterpret_cast<const char*>(&requiredExp), sizeof(int));

	// Записываем карту
	std::string mapName = GetCurrentMap()->sName;
	uint32_t numLetters = static_cast<uint32_t>(mapName.size());
	data.write(reinterpret_cast<const char*>(&numLetters), sizeof(numLetters));
	data.write(mapName.data(), numLetters);

	// Записываем координаты игрока
	float px = m_pPlayer->px;
	float py = m_pPlayer->py;
	data.write(reinterpret_cast<const char*>(&px), sizeof(float));
	data.write(reinterpret_cast<const char*>(&py), sizeof(float));

	// Записываем количество квестов и талантов
	AmountQuests = static_cast<uint32_t>(GetListQuest().size());
	AmountTalents = static_cast<uint32_t>(GetLearnedTalentVector().size());
	data.write(reinterpret_cast<const char*>(&AmountQuests), sizeof(AmountQuests));
	data.write(reinterpret_cast<const char*>(&AmountTalents), sizeof(AmountTalents));

	// Записываем квесты
	for (auto& quest : GetListQuest())
	{
		int saveSlot = quest->SaveSlot;
		int phase = quest->GetPhase();
		data.write(reinterpret_cast<const char*>(&saveSlot), sizeof(int));
		data.write(reinterpret_cast<const char*>(&phase), sizeof(int));
	}

	// Записываем таланты
	data.write(reinterpret_cast<const char*>(&TalentPoint), sizeof(int));

	

	for (int talent : GetLearnedTalentVector())
	{
		data.write(reinterpret_cast<const char*>(&talent), sizeof(int));
	}

	data.close();
	std::cout << "Save Succeeded" << std::endl;

	for (int i = 0; i < m_vecSaveTalents.size(); i++)
	{
		std::cout << " " << m_vecSaveTalents[i] << " ";
	}
	std::cout << m_vecSaveTalents.size() << "/////" << "SAVE FILE COMPLETE  " << std::endl;

	return true;

}



bool RPG_Engine::LoadFunction()
{
	std::ifstream inFile("Load/CurrSave.bin", std::ios::binary);
	if (!inFile.is_open())
	{
		std::cout << "Load Denied" << std::endl;

		m_script.AddCommand(new cComand_HideScreen(2));
		m_script.AddCommand(new cComand_ResetQuestList());
		m_script.AddCommand(new cComand_Changemap("Forest", 4, 24));
		m_script.AddCommand(new cComand_SetNgameMod(1));
		m_script.AddCommand(new cComand_CalculateExp());
		return false;
	}

	// Очистка текущих данных
	m_vecSaveTalents.clear();
	m_listQusets.clear();
	RPG_Assets::get().ResetQuests();

	// Чтение основных параметров
	int lvl, currExp, ExpRequred;
	if (!inFile.read(reinterpret_cast<char*>(&lvl), sizeof(lvl))) return false;
	if (!inFile.read(reinterpret_cast<char*>(&currExp), sizeof(currExp))) return false;
	if (!inFile.read(reinterpret_cast<char*>(&ExpRequred), sizeof(ExpRequred))) return false;

	// Устанавливаем параметры
	SetLvl(lvl);
	SetCurrExp(currExp);
	SetRequredExp(ExpRequred);

	// Чтение карты
	uint32_t numLetters;
	if (!inFile.read(reinterpret_cast<char*>(&numLetters), sizeof(numLetters))) return false;

	std::string mapName(numLetters, '\0'); // Резервируем место для строки
	if (!inFile.read(&mapName[0], numLetters)) return false;

	// Чтение координат игрока
	float px, py;
	if (!inFile.read(reinterpret_cast<char*>(&px), sizeof(px))) return false;
	if (!inFile.read(reinterpret_cast<char*>(&py), sizeof(py))) return false;

	// Устанавливаем координаты игрока
	m_pPlayer->px = px;
	m_pPlayer->py = py;

	// Чтение количества квестов и талантов
	uint32_t AmountQuests, AmountTalents;
	if (!inFile.read(reinterpret_cast<char*>(&AmountQuests), sizeof(AmountQuests))) return false;
	if (!inFile.read(reinterpret_cast<char*>(&AmountTalents), sizeof(AmountTalents))) return false;

	// Чтение квестов
	for (uint32_t i = 0; i < AmountQuests; ++i)
	{
		int QuestSlot, phase;
		if (!inFile.read(reinterpret_cast<char*>(&QuestSlot), sizeof(QuestSlot))) return false;
		if (!inFile.read(reinterpret_cast<char*>(&phase), sizeof(phase))) return false;

		// Восстанавливаем квест
		auto* quest = RPG_Assets::get().GetActiveQuest(QuestSlot);
		if (quest)
		{
			quest->SetPhase(phase);
			AddActiveQuest(quest);
			quest->makeActual();
		}
	}

	// Чтение очков талантов
	if (!inFile.read(reinterpret_cast<char*>(&TalentPoint), sizeof(int))) {
		std::cerr << "Ошибка чтения TalentPoint" << std::endl;
		return false;
	}

	// Чтение талантов
	m_vecSaveTalents.clear();
	for (uint32_t i = 0; i < AmountTalents; ++i)
	{
		int talent;
		if (!inFile.read(reinterpret_cast<char*>(&talent), sizeof(talent))) return false;
		m_vecSaveTalents.push_back(talent);
	}

	// Устанавливаем направление игрока
	m_pPlayer->SetFacingDirection(1);

	// Настройка карты и игровых режимов
	m_script.AddCommand(new cComand_Changemap(mapName, px, py));
	m_script.AddCommand(new cComand_SetNgameMod(1));
	m_script.AddCommand(new cComand_CalculateExp());

	inFile.close();
	std::cout << "Load Succeeded" << std::endl;


	std::cout << "Save Succeeded" << std::endl;

	for (int i = 0; i < m_vecSaveTalents.size(); i++)
	{
		std::cout << " " << m_vecSaveTalents[i] << " ";
	}
	std::cout << m_vecSaveTalents.size() << "/////" << "LOAD FILE  COMPLETE  " << std::endl;

	return true;
}

void RPG_Engine::AddActiveQuest(cQuest* quest)
{
	m_listQusets.push_back(quest);
}

void RPG_Engine::AddCompletedQuest(cQuest* quest)
{	// Если список содержит более 3 элементов, удаляем первый
	if (m_CompletedQuest.size() >3)
	{
		m_CompletedQuest.erase(m_CompletedQuest.begin());
	}
	// Добавляем новый элемент в конец списка
	m_CompletedQuest.push_back(quest);
}

cQuest* RPG_Engine::GetActiveQuest(std::string name)
{
	

	for (auto& quest : m_listQusets)
	{
		if (quest->sName == name)
			return quest;

	}
		

		return nullptr;
	
}

void RPG_Engine::ChangeMap(std::string sMapName, float x, float y)
{
	
	

	charDeath = false;    // <--- Let char die on spikes and other dunger slots
    //Destroy all dynamics
	
	for (auto& source : m_vecDynamics)  // <- return all entitys back in pool
		source->ReturnToPool();
		//for (auto& dyns : *source)

	for (auto& source : m_vecProjectiles)  // <- return all entitys back in pool
		source->ReturnToPool();

	for (auto& source : { &m_vecCloseWeather,&m_vecFarWeather,&m_vecCloseWeather_2 })
	{
		for (auto& entity : *source)
			ReturnEnvironmentToPool(entity);
	}


	m_vecParticles.clear();
    m_vecDynamics.clear();
	m_vecProjectiles.clear();
	m_vecCloseWeather.clear();
	m_vecCloseWeather_2.clear();
	m_vecFarWeather.clear();
	//m_listBlackSmithItems.clear();
	//m_listStoreItems.clear();
	for (auto& slot : m_listStoreItems) // clean lyxary shop
	{
		slot->resetsocket();
	}


	//m_listQusets.clear();
	//m_listItems.clear();

    m_vecDynamics.push_back(m_pPlayer);      //push in first place player in vecDynamics
	m_pPlayer->ConnectTarget(nullptr);

    m_pCurrentMap = RPG_Assets::get().GetMap(sMapName);


    //Update player location
    m_pPlayer->px = x;
    m_pPlayer->py = y;

	fFarPrlxX = 0;
	fMidPrlxX = 0;
	fClosePrlxX = 0;

	m_pCurrentMap->PopulateDynamics(m_vecDynamics);

	if (sMapName == "VillageTavern")
	{
		fCameraPosX = 12;
		fCameraPosY = 9;
		blockCamera = true;
	}
	else
	{
		fCameraPosX = x;
		fCameraPosY = y;
		blockCamera = false;
	}


	for (auto q : m_listQusets)    // Add Quests objects for quests
	{
		q->PopulateDynamics(m_vecDynamics, m_pCurrentMap->sName);
	}

}

void RPG_Engine::defineFacingDirection(float& fTestX, float& fTestY)
{

	//if (m_pPlayer->GetFacingDirection() == 0)  //South
	//{
	//	fTestX = m_pPlayer->px + 0.5;
	//	fTestY = m_pPlayer->py + 1f;
	//}

	if (m_pPlayer->GetFacingDirection() == 1)  //West
	{
		fTestX = m_pPlayer->px +m_pPlayer->CollbordersX - 0.85f;
		fTestY = m_pPlayer->py + m_pPlayer->CollbordersY+0.8f;
	}

	//if (m_pPlayer->GetFacingDirection() == 2)  //North
	//{
	//	fTestX = m_pPlayer->px + 0.5;
	//	fTestY = m_pPlayer->py + 0.0f;
	//}

	if (m_pPlayer->GetFacingDirection() == 3)  //East
	{
		fTestX = m_pPlayer->px + m_pPlayer->CollbordersXF+0.85f;
		fTestY = m_pPlayer->py + m_pPlayer->CollbordersY + 0.8f;
	}
}

bool RPG_Engine::SaleItem(int Price, InventaryItem* Sale)
{

	TakeItem(Sale,m_listItems);  // first step  we need change on empty slot object, which must be absorbed, it helps save slot
	
	int helpprice = Price;

	

	GiveItem("Coin", Price);

	return false;
}

bool RPG_Engine::GiveItem(std::string Name,uint8_t count, bool offstack)
{
	
	

	uint8_t hcount = count;   // how many objects need to give
	cItem* Item = RPG_Assets::get().GetItem(Name);     // that item what we want to add in our inventary

//	Item->MaxStack                                     // how much they cant stuck in one slot
	


	
	if (offstack)
	{

		for (auto invItem : m_listItems)  //for money 
		{

			if (invItem->Item == Item)   // if we find in our inv element eq that what we want to add 
			{
				if (dynamic_cast<cGold*>(invItem->Item))  // when item wallet or coint

				{

					cGold* helper = (cGold*)invItem->Item;    // we switch sale on cgold for achive MaxGold Parametr

					if (invItem->Gold > 0 && invItem->Gold < helper->MaxGold)   // this socket has more than 0 - means wallet or gold stack
					{


						int8_t reminder = invItem->Gold + hcount - helper->MaxGold;    //  we take reminder for cases when we want to sal for 3 gold object in wallet where 58 gold

						if (reminder > 0)  // means that more than max gold in this socket
						{
							invItem->Gold = helper->MaxGold;   //set plase on high money
							hcount = reminder;    //substract from price max gold 
							continue;   // go check next slot
						}
						else if (reminder == 0)
						{
							invItem->Gold = helper->MaxGold;
							return false;
						}
						else
						{
							invItem->Gold += hcount;
							hcount = 0;
							return false;
						}

						//	Item->OnInteract(m_pPlayer);   // add gold
						return false;
						break;


					}
				}
				else    //when object not coin or wallet
				{
					if (invItem->Item->MaxStack>1)
					{


					int8_t reminder = invItem->currStacks + hcount - invItem->Item->MaxStack; //we the same calculate reminder  only for stacks

					if (reminder > 0)  // means that more than max gold in this socket
					{
						invItem->currStacks = invItem->Item->MaxStack;   //set plase on high money
						hcount = reminder;    //substract from price max gold 
						continue;   // go check next slot
					}
					else if (reminder == 0)
					{
						invItem->currStacks = invItem->Item->MaxStack;
						return false;
					}
					else
					{
						invItem->currStacks += hcount;
						hcount = 0;
						return false;
					}

					}
				}
			}
		}

	}

		



	for (auto it = m_listItems.begin(); it != m_listItems.end(); it++)
	{
		
		InventaryItem* entity = *it;

		if (entity->checkFlag(entity->Breserved) == false )   // when slot is empty 
		{


			entity->setFlag(entity->Breserved);    // we set it in reserved
			 
			m_listItems[entity->index]->Item = RPG_Assets::get().GetItem(Name);   // attach item in slot
			m_listItems[entity->index]->Gold = m_listItems[entity->index]->Item->Gold;   // and in slot we add   gold that keep in library   0 for other objects and one or more for gold
			hcount--;
			if (hcount!=0)   //when we came here from first cylw and count still not 0
			{
				GiveItem(Name, hcount);  // we add new object in array and call function back for calculate all for new coin for example
			}
	

		return false;
		}
		else if(entity->checkFlag(entity->Babsorbed) == true)   // when object was absorbed 
		{
		
			entity->clearFlag(entity->Babsorbed);      // we clear absrob flag
			m_listItems[entity->index]->Item = RPG_Assets::get().GetItem(Name);   //and attach item here
			m_listItems[entity->index]->Gold = m_listItems[entity->index]->Item->Gold;
			absorbCounter--;
			hcount--;
			if (hcount != 0)   //when we came here from first cylw and count still not 0
			{
				GiveItem(Name, hcount);  // we add new object in array and call function back for calculate all for new coin for example
			}


			return false;

		} 

	}
	
	



	return true;
}
//
//bool RPG_Engine::FillList(cItem * item,int number)
//{
//	if (number <=24)
//	{
//	m_listItems.assign(number, item);
//	return true;
//	}
//
//	return false;
//}

bool RPG_Engine::ByeItem(InventaryItem * item)

{

	//item->invnumber = m_listItems.size() + 1;

	//for (int z = 1; z < 24; )   // we start cicle from first slot
	//{
	//	int zx = (z - 1) % 8;    // transfer index in world coordinate
	//	int zy = (z - 1) / 8;
	//	bool Breserved = false;

	//	for (auto s : m_listItems)    //start cheking every slot on reserve
	//	{

	//		int sx = (s->invnumber - 1) % 8 + s->ipx;   // <--Transfer in world coordinate offset
	//		int sy = (s->invnumber - 1) / 8 + s->ipy;

	//		if (sx == zx && sy == zy)   // situation when the slot reserved we add index on next slot
	//		{
	//			Breserved = true;
	//			z++;
	//			break;
	//		}


	//	} // when we go through all list and didn't find reserv slot

	//	if (Breserved == false)
	//	{
	//		item->ipx = zx - ((item->invnumber - 1) % 8);
	//		item->ipy = zy - ((item->invnumber - 1) / 8);
	//		break;
	//	}

	//}

	//m_listItems.push_back(item);   //push the pointer 

	return false;
}



//bool RPG_Engine::GiveWarehouseItem(cItem* item)
//{
//	item->bInWarehouse = true;
//	m_listWarehouseItems.push_back(item);   //push the pointer 
//
//
//
//	return false;
//}

bool RPG_Engine::GiveNewText(cDynamic_TextDamage* Text)
{
	if (Text !=nullptr)
	{
		m_vecFightText.push_back(Text);
		return true;
	}
	return false;
}
int RPG_Engine::GetFreespaceInventory()
{
	int freesocket = 0;

	for (auto& it : m_listItems)
	{
		if (it->Item == nullptr)
			freesocket++;
	}

	return freesocket;
}
bool RPG_Engine::GiveStoreItem(std::string item)
{

	for (auto slot : m_listStoreItems)
	{

		if (!slot->checkFlag(slot->Breserved))
		{
			//slot->breserved = true;
			slot->setFlag(slot->Breserved);
			m_listStoreItems[slot->index]->Item = RPG_Assets::get().GetItem(item);
			return true;
		}

	}

	//item->bInWarehouse = true;
	



	return false;
}

bool RPG_Engine::GiveBlackSmithItem(std::string item)
{
	
	for (auto slot : m_listBlackSmithItems)
	{
		
		if (!slot->checkFlag(slot->Breserved))
		{
			slot->setFlag(slot->Breserved);

			//slot->breserved = true;
			m_listBlackSmithItems[slot->index]->Item = RPG_Assets::get().GetItem(item);
			return true;
		}

	}

	//item->bInWarehouse = true;
	//m_listBlackSmithItems.push_back(item);   //push the pointer 
	


	return false;
}


bool RPG_Engine::TakeItem(InventaryItem* item, std::vector<InventaryItem*>& m_listItems)
{





	if (item != nullptr)
	{
	
				item->clearFlag(item->Breserved);
				m_listItems[item->index]->Item = nullptr;
				item->resetsocket();
				//slot->breserved = false;

				return true;
			
		

	}
	else

		return false;
}
bool RPG_Engine::TakeItem(int price) //for price buy
{
	
	int helpPrice = price;
	int counter = 0;
	

	for (auto& item :GetListItem())  // go in list check cross with mouse 
	{



		if (dynamic_cast<cGold*>(item->Item))
		{
			cGold* itemHelp = (cGold*)item->Item;
			//AddMoney(itemHelp->GoldCount);


			if (item->Gold > 0 && helpPrice != 0)    //First find object that has money
		{
			

				int reminder = item->Gold - helpPrice;

				if (reminder > 0)  // gold in socket was more than price
				{
					item->Gold = reminder;
					return false;
				}
				else  if (reminder == 0)   // gold in socket was equel to price 
				{
					m_listItems[item->index]->Item = nullptr;
					item->Gold = reminder;
					m_listItems[item->index]->clearFlag(m_listItems[item->index]->Breserved);
					return false;
				}
				else      // gol in socket was less than price
				{
					m_listItems[item->index]->Item = nullptr;  // we said socket that item will be empty
					item->Gold = 0;  // gold counr set to 0
					item->clearFlag(item->Breserved);   // could be reserved
					TakeItem(std::abs(reminder));  // and call fucntion back with less price
					
				}


			

		}


		}
		

	}
	
	


	return false;
	
}

bool RPG_Engine::TakeItem(std::string names) //for price
{
	int squeX =0;
	std::string ItemName;
	int Reqcount=0;
	int Counter = 0;
	for (auto c :names)
	{

		if (c == 10)    //<---  \n  here we gathered full name of item
		{
	
			for (auto& item : m_listItems)   // go through inventory and find equal this name 
			{

				if (item->Item !=nullptr)
				{

				if (item->Item->sName == ItemName && Reqcount>0) //we find 
				{
					squeX++;
					item->setFlag(item->Babsorbed);
					//item->Absorbed = true;   // Delete it
					absorbCounter++;
					Reqcount--;
					Counter = 0;
				}
				
				}

			}
			
			ItemName.clear();
			continue;

		}
		
		if (Counter ==0) // check if we on beginning letter 
		{

			Reqcount = c- '0';// Convert char to int and write it in integer
		}
		else
		{
		ItemName += c;   // Gather word

		}
		Counter++;
	}

	

	return true;
}


bool RPG_Engine::HasItem(std::string item)
{
	
	if (!item.empty())
	{


		for (auto& items : m_listItems)
		{
			if (items->Item !=nullptr)
			{

			if (items->Item->sName == item )
				return true;
			}
		}
	}
	
		return false;
}

void RPG_Engine::AddProjectile(cDynamic* proj)
{
	m_vecProjectiles.push_back(proj);
}

void RPG_Engine::AddVecDynamic(cDynamic* proj)
{
	m_vecDynamics.push_back(proj);
}

//void RPG_Engine::AddEnvironment(Environment* env)
//{
//	m_vecEnviroment.push_back(env);
//}


void RPG_Engine::AddUi(cUI* hpBar)
{
	m_vecUiBars.push_back(hpBar);
}
void RPG_Engine::AddTalentUi(cUI* UiTalent)
{
	m_vecTalentsBars.push_back(UiTalent);
}

int RPG_Engine::GetLvl()
{
	return lvl;
}

void RPG_Engine::SetLvl(int lvl)
{
	this->lvl = lvl;
}

int RPG_Engine::GetCurrExp()
{
	return currExp;
}

void RPG_Engine::SetCurrExp(int currexp)
{
	currExp = currexp;
}

cMap* RPG_Engine::GetCurrentMap()
{
	if (m_pCurrentMap !=nullptr)
	{
	return m_pCurrentMap;
	}

	return nullptr;
}

int RPG_Engine::GetRequredExp()
{
	return ExpRequred;
}

void RPG_Engine::SetRequredExp(int reqexp)
{
	ExpRequred = reqexp;
}

void RPG_Engine::CalculateExp()
{
//	ExpRequred = ((lvl+1)*100) + (100 * lvl);

	int sup = 0;
	for (int  i = 2; i <= lvl+2; i++)
	{
		sup = sup + i * 100; //200 for 2 lvl

		if (i-1==lvl)
		{
			
			ExpRequred = sup;

			m_pPlayer->LvL = lvl;
			m_pPlayer->calculateStats();
			m_pPlayer->MergeItemLvl();
			m_pPlayer->nHealth = m_pPlayer->hpUpdate->hpMax;
			
			break;
		}

	}


	

}

void RPG_Engine::SetCurrentExp(int DeathExp)
{

	currExp += DeathExp;       
	if (currExp >=ExpRequred)   
	{
		do
		{
			lvl++;
			TalentPoint++;
			m_vecParticles.push_back(new VfxLevel(m_pPlayer->px, m_pPlayer->py));

			int spare = currExp - ExpRequred;
			CalculateExp();
			PlaySounds("NewLvlSound");

			currExp = spare;
		} while (currExp>ExpRequred);
	}
}

//void RPG_Engine::Attack(cDynamic_Creature* aggressor, cWeapon* weapon)
//{
//	weapon->OnUse(aggressor);
//}

void RPG_Engine::Damage(cDynamic_Projectile* projectile, cDynamic_Creature* victim)
{
	
	PlaySounds("TakeDamage");

	
	

	if (victim != nullptr)
	{

		if (projectile->gettarget && m_pPlayer->ConnectTarget(victim))
		{
			std::cout << "Target Taken" << std::endl;
		}


		m_pPlayer->rageAmount+=(projectile->rageamount);

		int CalculatedDamage = (victim->GetDefence() * 4.4f)/100;
		CalculatedDamage =(1.0f - CalculatedDamage)*projectile->nDamage;


		//Attack victim with damage
		victim->nHealth -= CalculatedDamage;

		//cDynamic_TextDamage* Text = new cDynamic_TextDamage(victim->px+victim->CollbordersX+0.25f, victim->py+victim->CollbordersY+0.6f, std::to_string(CalculatedDamage));

		//m_vecFightText.push_back(Text);

		olc::vf2d textdamagecoord{ (float)victim->px + victim->CollbordersX + 0.25f, (float)victim->py + victim->CollbordersY + 0.6f };
		cDynamic_TextDamage* T = (cDynamic_TextDamage*)SpawnBattleText(textdamagecoord, std::to_string(CalculatedDamage));


		AddParticle(victim->px + victim->CollbordersX + 0.25f, victim->py + victim->CollbordersY + 0.3f);


		//m_vecParticles.push_back(new VfxShot(victim->px + victim->CollbordersX + 0.25f, victim->py + victim->CollbordersY + 0.3f));


		//Knock victim back
		float tx = victim->px - projectile->px;
		float ty = victim->py - projectile->py;
		float d = sqrtf(tx * tx + ty * ty);
		if (d < 1) d = 1.0f;
		if (d > 1) d = -1.0f;
		{

		}
		//Affter a hit, they object experiences knock back, where it is temprorarily
		//under system control.This delivers two functions, the first being
		//a virtual indicator to the player that something has happened, and the second
		//it stops the ability to spam attacks on a single creature
		//projectile->knockbacktime
		

		victim->KnockBack( d*projectile->attackDirectionX,  projectile->attackDirectionY, projectile->knockbacktime);

		
		



	
		
		projectile->HitCount++;



		if (projectile->bOneHit && projectile->HitCount >=projectile->HitAmount)
		{
			projectile->SwitchLayer(2);  //if we hit - sword switch in Neutral position

		};
	}
}

void RPG_Engine::AddParticle(float px, float py)
{

	SpawnVfxShot({ px,py });
//	m_vecParticles.push_back(new VfxShot(px, py));
}

void RPG_Engine::AddParticle(VfxParticles* particle)
{
	m_vecParticles.push_back(particle);
}



void RPG_Engine::JumpDamage(cDynamic_Creature* projectile, cDynamic_Creature* victim)
{



	if (victim != nullptr)
	{

		int CalculatedDamage = (victim->GetDefence() * 4.4f) / 100;
		CalculatedDamage = (1.0f - CalculatedDamage) * 10;


		//Attack victim with damage
		victim->nHealth -= CalculatedDamage;

		olc::vf2d textdamagecoord{ (float)victim->px + victim->CollbordersX + 0.25f, (float)victim->py + victim->CollbordersY + 0.6f };
		cDynamic_TextDamage* T = (cDynamic_TextDamage*)SpawnBattleText(textdamagecoord, std::to_string(CalculatedDamage));


		//cDynamic_TextDamage* Text = new cDynamic_TextDamage(victim->px + victim->CollbordersX + 0.25f, victim->py + victim->CollbordersY + 0.6f, std::to_string(CalculatedDamage));

		//m_vecFightText.push_back(Text);
		
		victim->SwitchLayer(2);
		//Knock victim back
		float tx = victim->px - projectile->px;
		float ty = victim->py - projectile->py;
		float d = sqrtf(tx * tx + ty * ty);
		if (d < 1) d = 1.0f;

		//Affter a hit, they object experiences knock back, where it is temprorarily
		//under system control.This delivers two functions, the first being
		//a virtual indicator to the player that something has happened, and the second
		//it stops the ability to spam attacks on a single creature
		victim->KnockBack(d *tx, d * ty/5, 0.1);







	//	if (projectile->bOneHit)
	//	{
	//		projectile->SwitchLayer(2);  //if we hit - sword switch in Neutral position

	//	};
	}
}

void RPG_Engine::ShowDialog(std::vector<std::string> vecLines)
{
	m_vecDialogToShow = vecLines;
	m_bShowDialog = true;


}

void RPG_Engine::DrawBigText(std::string sText, float x, float y, float scalex, float scaley, olc::Pixel color)
{
	int i = 0, z = 0;  // i — позиция символа по X, z — по Y

	for (auto c : sText)
	{
		if (c == '\n')  // Если символ — новая строка, переходим на следующую строку
		{
			z++;  // Увеличиваем индекс строки
			i = 0;  // Сброс индекса по X
			continue;
		}

		// Определяем координаты символа в текстурном атласе
		float sx = ((c - 32) % 16) * 32;
		float sy = ((c - 32) / 16) * 32;

		// Отрисовка символа
		DrawPartialDecal({ x + (i * (18 * scalex)), y + (z * (32 * scaley)) },
			m_sprFont, { sx, sy }, { 32, 32 }, { scalex, scaley }, color);
		i++;  // Переходим к следующему символу по X
	}

}

void RPG_Engine::DisplayDialog(std::vector<std::string> vecText, int x, int y)
{
 	int nMaxLineLength = 0;
	int nLines = vecText.size();
	int Fullsize = 0;

	for (auto i : vecText)
	{
		Fullsize += i.size();

		if (i.size() > nMaxLineLength)
		nMaxLineLength = i.size();
	}

//	std::cout << vecText.size() << << std::endl;
	olc::Decal* BlackPattern = RPG_Assets::get().GetSprite("DescriptionPattern");


	DrawPartialDecal({ (float)x - (1*fscale) , (float)y - (1 * fscale) }, BlackPattern, { 0,0 }, { (float)nMaxLineLength * 18 + 10,(float)nLines * 32 +10 }, {fscale,fscale});

	DrawPartialDecal({ (float)x - (1 * fscale),  (float)y - (1 * fscale) }, D_FullUi, { 1227,0 }, { (float)4,(float)nLines * 32 + 15 }, { 1*fscale,1*fscale}, {66,83,100});
	DrawPartialDecal({ (float)x + ((nMaxLineLength * 18 + 9)*fscale),  (float)y - (1*fscale) }, D_FullUi, { 1227,0 }, { (float)4,(float)nLines * 32 + 15 }, { 1*fscale,1*fscale }, { 66,83,100 });

	DrawPartialDecal({ (float)x - (1 * fscale), (float)y - (1 * fscale) }, D_FullUi, { 0,910 }, { (float)nMaxLineLength * 18 + 14,(float)4 }, { 1*fscale,1*fscale }, { 66,83,100 });
	DrawPartialDecal({ (float)x - (1 * fscale), (float)y + ((nLines * 32 + 9)*fscale) }, D_FullUi, { 0,910 }, { (float)nMaxLineLength * 18 + 14,(float)4 },{1*fscale,1*fscale},{66,83,100});


	

	

	// Логика для отображения текста по символам с задержкой
	textCounter += GetElapsedTime();

	


	if (textCounter > 0.1f) {
		if (rowIndex < nLines) {  // Проверка, что не вышли за границы текста
			if (chartIndex < vecText[rowIndex].size()) {
				// Добавляем очередной символ в текущую строку
				currentLine += vecText[rowIndex][chartIndex];
				chartIndex++;

				if (chartIndex % 2 == 0)
				{
				PlaySounds("ReleaseButtonsSound");
				}

			}
			else {
				// Когда текущая строка полностью отображена, добавляем её в vecdisplayText
				vecdisplayText.push_back(currentLine);
				currentLine.clear();  // Очищаем временную строку для следующей строки
				rowIndex++;
				chartIndex = 0;
			}
		}
		textCounter = 0.0f; // Сбрасываем таймер
	}

	// Отображаем накопленный текст  строчек
	for (int l = 0; l < vecdisplayText.size(); l++) {
		DrawBigText(vecdisplayText[l], x, y + (l * 32 * fscale), fscale, fscale, olc::WHITE);
	}

	// Отображаем текущую неполную строку  динамическую 
	if (!currentLine.empty()) {
		DrawBigText(currentLine, x, y + (vecdisplayText.size() * 32 * fscale), fscale, fscale, olc::WHITE);
	}

}

void RPG_Engine::WrapCoordinates( float& ox)
{


	if (ox > ScreenWidth())
	{
 		ox -= ScreenWidth();
	}
	if (ox  < 0.0f)
	{
		ox += ScreenWidth();

	}
	//if (iy < 0.0f) oy = iy + (float)ScreenHeight();
	//if (iy >= ScreenHeight()) oy = iy - (float)ScreenHeight();

	
}

int RPG_Engine::GetRage()
{
	return m_pPlayer->rageAmount;
}

int8_t RPG_Engine::GetEnergy()
{
	if (m_pPlayer->energyAmount < 0)
	{
		return 0;
	}
	else
	{

	return m_pPlayer->energyAmount;
	}
}
bool RPG_Engine::GetBackStab()
{
	return bUibackstub;
	
}
void RPG_Engine::SetBackStab(bool toggle)
{
	bUibackstub = toggle;
}
bool RPG_Engine::GetTarget()
{
	if (m_pPlayer->Gettarget()!=nullptr)
	{
		return true;

	}
	else
		return false;
	


}

bool RPG_Engine::GetbOnGraund()
{
	return m_pPlayer->bOnGround;

}

int RPG_Engine::GetLearnedTalent(int save)
{
	//for (auto& source :  &m_vecTalentsBars )    // DRAW Ui elements
	for (auto dyns : m_vecSaveTalents)
		if (dyns == save)
			return true;
		
	return false;
}
void RPG_Engine::SetLearnedTalent(cUI* Talent)
{
	m_vecSaveTalents.push_back(Talent->SaveSlotTalent());
}


bool  RPG_Engine::CheckPosition(int pxX, int pxXF, int pyY, int pyYf)
{
	                                           // x/  y
	if (m_pCurrentMap->GetColliziionIndex(pxX, pyY) == 1 || m_pCurrentMap->GetColliziionIndex(pxX, pyYf) == 1  || (m_pCurrentMap->GetColliziionIndex(pxXF, pyY) == 1 || m_pCurrentMap->GetColliziionIndex(pxXF, pyYf) == 1))
	return false;
	
	return true;
}

bool  RPG_Engine::CheckParticlePosition(float pxX, float pyYf)

{
	if (m_pCurrentMap->GetColliziionIndex(pxX, pyYf) == 1 )
		return false;

	if (m_pCurrentMap->GetColliziionIndex(pxX, pyYf) == 2 )
	{
		

		std::pair<olc::vf2d*, olc::vf2d*>* object = m_pCurrentMap->getObsticlesPoints(pxX, pxX, pyYf, pyYf);


		


		float deltaX = object->second->x - object->first->x;
		float deltaY = object->first->y - object->second->y;
		float slopeRatio = deltaY / deltaX; // proportion (atan)
		float charX = pxX - object->first->x;    // define X position in slope

		float sloperatio = charX * slopeRatio;   // get high need for up object


	
		if (pyYf >= object->first->y - sloperatio)
		{

			
			return false;

		}



	}
	if ((m_pCurrentMap->GetColliziionIndex(pxX, pyYf) == 3))
	{

		std::pair<olc::vf2d*, olc::vf2d*>* object = m_pCurrentMap->getObsticlesPoints(pxX, pxX, pyYf, pyYf);




 		float deltaX = object->second->x - object->first->x;
		float deltaY = object->second->y - object->first->y;
		float slopeRatio = deltaY / deltaX; // proportion (atan)
		float charX = object->second->x - pxX;    // define X position in slope

		float sloperatio = charX * slopeRatio;   // get high need for up object





		


		//	fNewObjectPosX += DeltaX;
		if (pyYf >= object->second->y - sloperatio)
		{
			
			return false;
		}







	}




		return true;

}

bool  RPG_Engine::CheckPosition(int pxX,int pyYf)
{
	// x/  y
	if (m_pCurrentMap->GetColliziionIndex(pxX, pyYf) == 1 || m_pCurrentMap->GetColliziionIndex(pxX, pyYf) == 2 || (m_pCurrentMap->GetColliziionIndex(pxX, pyYf) == 3))
		return false;

	if (m_pCurrentMap->GetColliziionIndex(pxX, pyYf) == 2 || (m_pCurrentMap->GetColliziionIndex(pxX, pyYf) == 3))
	{

		
		std::pair<olc::vf2d*, olc::vf2d*>* object =m_pCurrentMap->getObsticlesPoints(pxX, pxX, pyYf, pyYf);

		float atan = atan2(object->first->y - object->second->y, object->second->x - object->first->x);
		float normal = atan + 3.14 / 2;

		float charatan = atan2(pyYf - object->second->y, object->second->x - pxX);




		if (cos(normal) * cos(charatan) + sin(normal) * sin(charatan) >= 0) //dot product
		{

			
			for (float i = 0; i <= 2; )
			{
				float innernormal = atan2((pyYf - i) - object->second->y, object->second->x - pxX);

				if (cos(normal) * cos(innernormal) + sin(normal) * sin(innernormal) >= 0)
				{

					return false;
					break;

				}

				i += 0.1f;


			}

		}




	}


	

	return true;
}
void RPG_Engine::drawPlayer(bool bdraw)
{
	if (bdraw)
	{
	m_pPlayer->setFlag(m_pPlayer->bDraw);

	}
	else
	{
		m_pPlayer->clearFlag(m_pPlayer->bDraw);
	}

}
void CombineAdjacentTiles(std::vector<Tile>& batchLayer, int layer)
{
	// Iterate through each tile in the batch
	for (auto& tile : batchLayer) {
		// Get the position of the current tile
		int tilePosX = tile.position.x;
		int tilePosY = tile.position.y;

		int* currentLayerIndex = nullptr;
		int* neighborLayerIndex = nullptr;

		olc::vi2d* currentTileSize = nullptr;
		olc::vi2d* currentTileSourceRect = nullptr;

		olc::vi2d* neighborTileSize = nullptr;
		olc::vi2d* neighborTileSourceRect = nullptr;

		// Get the layerinfo corresponding to the specified layer
		Tile::layerinfo* currentLayer = nullptr;
		switch (layer) {
		case 0:
			currentLayer = &tile.ZeroLayer;
			currentLayerIndex = &tile.ZeroLayer.index;
			currentTileSize = &tile.ZeroLayer.size;
			currentTileSourceRect = &tile.ZeroLayer.sourcerect;
			break;
		case 1:
			currentLayer = &tile.FirstLayer;
			currentLayerIndex = &tile.FirstLayer.index;
			currentTileSize = &tile.FirstLayer.size;
			currentTileSourceRect = &tile.FirstLayer.sourcerect;
			break;
		case 2:
			currentLayer = &tile.SecondLayer;
			currentLayerIndex = &tile.SecondLayer.index;
			currentTileSize = &tile.SecondLayer.size;
			currentTileSourceRect = &tile.SecondLayer.sourcerect;
			break;
		case 3:
			currentLayer = &tile.ThirdLayer;
			currentLayerIndex = &tile.ThirdLayer.index;
			currentTileSize = &tile.ThirdLayer.size;
			currentTileSourceRect = &tile.ThirdLayer.sourcerect;
			break;
		default:
			break;
		}

		// If the current layer is nullptr, skip this tile
		if (!currentLayer) continue;

		// Lambda function to find neighboring tile based on the current layer
		auto setupNeighborLayerInfo = [&](Tile& neighborTile) {
			switch (layer) {
			case 0:
				neighborLayerIndex = &neighborTile.ZeroLayer.index;
				neighborTileSize = &neighborTile.ZeroLayer.size;
				neighborTileSourceRect = &neighborTile.ZeroLayer.sourcerect;
				break;
			case 1:
				neighborLayerIndex = &neighborTile.FirstLayer.index;
				neighborTileSize = &neighborTile.FirstLayer.size;
				neighborTileSourceRect = &neighborTile.FirstLayer.sourcerect;
				break;
			case 2:
				neighborLayerIndex = &neighborTile.SecondLayer.index;
				neighborTileSize = &neighborTile.SecondLayer.size;
				neighborTileSourceRect = &neighborTile.SecondLayer.sourcerect;
				break;
			case 3:
				neighborLayerIndex = &neighborTile.ThirdLayer.index;
				neighborTileSize = &neighborTile.ThirdLayer.size;
				neighborTileSourceRect = &neighborTile.ThirdLayer.sourcerect;
				break;
			default:
				return false;
			}
			return true;
		};

		// Check if the current tile's index is not zero
		if (*currentLayerIndex != 0)
		{
			int offsetX = 0;
			int offsetY = 0;
			bool isHorizontalMerged = false;
			int verticalMergeCounter = 0;

			// Iterate through neighboring tiles (Right and down)
			for (int dx = 0; dx <= 4 - offsetX; ++dx) {
				isHorizontalMerged = false;

				for (int dy = 0; dy <= 4 - offsetY; ++dy) {
					// Skip the current tile itself
					if (dx == 0 && dy == 0) continue;

					int neighborPosX = tilePosX + dx;
					int neighborPosY = tilePosY + dy;

					// Find the neighboring tile in the batch
					auto neighborTileIt = std::find_if(batchLayer.begin(), batchLayer.end(),
						[&](Tile& neighborTile) { return neighborTile.position.x == neighborPosX && neighborTile.position.y == neighborPosY; });

					// If the neighboring tile exists
					if (neighborTileIt != batchLayer.end())
					{
						bool neighborLayerSetup = setupNeighborLayerInfo(*neighborTileIt);

						// Check if both the current and neighbor layer indices are non-zero
						if (*neighborLayerIndex != 0 && *currentLayerIndex != 0)
						{
							// Check if the neighboring tile shares the same index
							if (*neighborLayerIndex == *currentLayerIndex)
							{
								continue;
							}

							// Horizontal merging (right cell)
							if (currentTileSourceRect->x + currentTileSize->x == neighborTileSourceRect->x &&
								currentTileSourceRect->y == neighborTileSourceRect->y &&
								tilePosY == neighborPosY)
							{
								currentTileSize->x += neighborTileSize->x;
								*neighborLayerIndex = 0;
								isHorizontalMerged = true;
								continue;
							}

							if (!isHorizontalMerged && dx > 0)
							{
								break;
							}

							// Vertical merging (down cell)
							if (currentTileSourceRect->x == neighborTileSourceRect->x &&
								currentTileSourceRect->y + currentTileSize->y == neighborTileSourceRect->y &&
								neighborTileSourceRect->x + neighborTileSize->x == currentTileSourceRect->x + currentTileSize->x)
							{
								currentTileSize->y += neighborTileSize->y;
								*neighborLayerIndex = 0;
								continue;
							}

							// If the neighbor tile is within the boundaries of the current tile
							if (neighborTileSourceRect->x > currentTileSourceRect->x &&
								neighborTileSourceRect->x < currentTileSourceRect->x + currentTileSize->x &&
								neighborTileSourceRect->y > currentTileSourceRect->y &&
								neighborTileSourceRect->y < currentTileSourceRect->y + currentTileSize->y &&
								neighborTileSourceRect->x + neighborTileSize->x <= currentTileSourceRect->x + currentTileSize->x &&
								neighborTileSourceRect->y + neighborTileSize->y <= currentTileSourceRect->y + currentTileSize->y)
							{
								*neighborLayerIndex = 0;
								continue;
							}

							if (dy > 0)
							{
								break;
							}

							verticalMergeCounter++;
							offsetY = verticalMergeCounter;
							if (verticalMergeCounter == 0)
							{
								offsetY = 4;
							}
						}
					}
				}

				if (!isHorizontalMerged && dx > 0)
				{
					break;
				}
			}
		}
	}
}
float Lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

bool CheckZeroDivide(float check, float& variable)
{
	if (check ==0)
	{
	variable = 0;
	return  false;
	}
	return  true;

}

int sign(float x)
{
		return(x > 0) - (x < 0);
}

void RPG_Engine::LoadenemyInstances()  /// load all pools 
{
	// Initialize the Bandit pool
	for (int i = 0; i < 30; ++i) {
		BanditsPool[i] =new cDynamic_creature_Bandit();
		DireWolfsPool[i]=new cDynamic_creature_DireWolf();
		BoarPool[i] = new cDynamic_creature_Boar();
		WereWolfsPool[i]=new cDynamic_creature_WereWolf();
		BanditsArcherPool[i]=new cDynamic_creature_BanditArcher();
		ItemPool[i]=new cDynamic_Item();
		TextPool[i]=new cDynamic_TextDamage();
		BanditBossPool[i]=new cDynamic_creature_BossBandit();


		ProjectilePool[i]=new cDynamic_Projectile();
		VfxShotPool[i] = new VfxShot(0,0);   // Initialize Fx  shot
		MirrorsFx[i] = new mirror(0, 0, { 0,0 }, 0,0, nullptr);

	}

	BossAr =
	{
		BanditBossPool[0]
	};


	NPC_Ar =
	{
	 new cDynamic_creature_NPC("Map", 3),
	 new cDynamic_creature_NPC("BlackSmith", 1),
	 new cDynamic_creature_NPC("LuxurySeller", 0),
	 new cDynamic_creature_NPC("ProfessionMan", 2),
	 new cDynamic_creature_NPC("Warehouse", -1),
	 new cDynamic_creature_NPC("TavernVillage", -1),
	 new cDynamic_creature_NPC("CaveEntrence", -1),
	 new cDynamic_creature_NPC("CaveOut", -1),
	};


	// Initialize the raindrops
	for (uint16_t i = 0; i < 1; ++i)
	{
		
		EnvironmentPool[i] = new ERaindrop(i);

	}


	for (uint16_t i = 1; i < 101; ++i)
	{
	
		EnvironmentPool[i] = new ERainSpray(0, 0,  i);

	}

	for (uint16_t i = 101; i < 106; ++i)
	{
		EnvironmentPool[i] = new Edynamic_Cloud(rand() % 256 * 64, 0.0f, RPG_Assets::get().GetSprite("ForestCloud"), 1, i);
		
	}


}
cDynamic* RPG_Engine::SpawnBanditArcher(const olc::vf2d position)
{
	for (auto it = BanditsArcherPool.begin(); it != BanditsArcherPool.end(); ++it) {
		cDynamic* entity = *it;
		if (entity == nullptr) {
			continue; // Пропустить пустые ячейки
		}
		entity->setFlag(entity->binitialized);    // Устанавливаем флаг инициализации
		m_vecDynamics.push_back(entity);          // Добавляем в активный вектор динамических объектов
		entity->px = position.x;                 // Устанавливаем позицию
		entity->py = position.y;

		*it = nullptr;                  // Удаляем заспаунинного бандита из пула
		return entity;                            // Возвращаем указатель на заспаунинного бандита

	}

	return nullptr;  // Если бандитов в пуле не осталось
}



cDynamic* RPG_Engine::SpawnBandit(const olc::vf2d position)
{
	for (auto it = BanditsPool.begin(); it != BanditsPool.end(); ++it) {
		cDynamic* entity = *it;
		if (entity == nullptr) {
			continue; // Пропустить пустые ячейки
		}
			entity->setFlag(entity->binitialized);    // Устанавливаем флаг инициализации
			m_vecDynamics.push_back(entity);          // Добавляем в активный вектор динамических объектов
			entity->px = position.x;                 // Устанавливаем позицию
			entity->py = position.y;

			*it = nullptr;                  // Удаляем заспаунинного бандита из пула
			return entity;                            // Возвращаем указатель на заспаунинного бандита
		
	}

	return nullptr;  // Если бандитов в пуле не осталось
}


cDynamic* RPG_Engine::SpawnWerewolf(const olc::vf2d position)
{
	for (auto it = WereWolfsPool.begin(); it != WereWolfsPool.end(); ++it) {
		cDynamic* entity = *it;
		if (entity == nullptr) {
			continue; // Пропустить пустые ячейки
		}
			entity->setFlag(entity->binitialized);
			m_vecDynamics.push_back(entity); // Add the Bandit entity to the game
			entity->px = position.x;
			entity->py = position.y;
			*it = nullptr;                  // Удаляем заспаунинного бандита из пула
			return entity; // Exit the function after spawning a Bandit
		
	}
	return nullptr;  // Если бандитов в пуле не осталось
}

cDynamic* RPG_Engine::SpawnBoar(const olc::vf2d position)
{
	for (auto it = BoarPool.begin(); it != BoarPool.end(); ++it) {
		cDynamic* entity = *it;
		if (entity == nullptr) {
			continue; // Пропустить пустые ячейки
		}
			entity->setFlag(entity->binitialized);
			m_vecDynamics.push_back(entity); // Add the Bandit entity to the game
			entity->px = position.x;
			entity->py = position.y;
			*it = nullptr;                  // Удаляем заспаунинного бандита из пула   не удаляет а говорит смотреть на Nullptr
			return entity; // Exit the function after spawning a Bandit
		
	}
	return nullptr;  // Если бандитов в пуле не осталось
}

cDynamic* RPG_Engine::SpawnBossBandt(const olc::vf2d position)
{
	for (auto it = BanditBossPool.begin(); it != BanditBossPool.end(); ++it) {
		cDynamic* entity = *it;
		if (entity == nullptr) {
			continue; // Пропустить пустые ячейки
		}

		entity->setFlag(entity->binitialized);
		m_vecDynamics.push_back(entity); // Add the Bandit entity to the game
		entity->px = position.x;
		entity->py = position.y;
		*it = nullptr;                  // Удаляем заспаунинного бандита из пула


		return entity; // Exit the function after spawning a Bandit

	}
	return nullptr; // Exit the function after spawning a Bandit
}


cDynamic* RPG_Engine::SpawnDireWolf(const olc::vf2d position)
{

	for (auto it = DireWolfsPool.begin(); it != DireWolfsPool.end(); ++it) {
		cDynamic* entity = *it;
		if (entity == nullptr) {
			continue; // Пропустить пустые ячейки
		}

		entity->setFlag(entity->binitialized);
		m_vecDynamics.push_back(entity); // Add the Bandit entity to the game
		entity->px = position.x;
		entity->py = position.y;
		*it = nullptr;                  // Удаляем заспаунинного бандита из пула



		return entity; // Exit the function after spawning a Bandit

	}
	return nullptr; // Exit the function after spawning a Bandit
}



cDynamic* RPG_Engine::SpawnProjectile(const olc::vf2d position)
{

	for (auto it = ProjectilePool.begin(); it != ProjectilePool.end(); ++it) {
		cDynamic* entity = *it;
		if (entity == nullptr) {
			continue; // Пропустить пустые ячейки
		}

		entity->setFlag(entity->binitialized);
		m_vecProjectiles.push_back(entity); // Add the Bandit entity to the game
		entity->px = position.x;
		entity->py = position.y;
		*it = nullptr;                  // Удаляем заспаунинного бандита из пула


		return entity; // Exit the function after spawning a Bandit

	}
	return nullptr; // Exit the function after spawning a Bandit
}

cDynamic* RPG_Engine::SpawnVfxShot(const olc::vf2d position)
{
	for (auto it = VfxShotPool.begin(); it != VfxShotPool.end(); it++)
	{
		cDynamic* entity = *it;
		if (entity == nullptr) {
			continue; // Пропустить пустые ячейки
		}
		VfxShot* Shot = (VfxShot*)entity;
		entity->clearFlag(entity->bRedundant);
		Shot->px = position.x;
		Shot->py = position.y;
		Shot->Spawn();
		m_vecParticles.push_back(Shot); // Add the Bandit entity to the game
		*it = nullptr;                  // Удаляем заспаунинного бандита из пула
		return entity;
	}
	return nullptr;;
}

cDynamic* RPG_Engine::SpawnMirrors(const olc::vf2d position, olc::vi2d framePos, int sizeX, int sizeY, int amountframes,olc::Decal* Decal)
{
	for (auto it = MirrorsFx.begin(); it != MirrorsFx.end(); it++)
	{
		cDynamic* entity = *it;
		if (entity == nullptr) {
			continue; // Пропустить пустые ячейки
		}
		mirror* Mirror = (mirror*)entity;
		entity->clearFlag(entity->bRedundant);
		
		Mirror->Spawn(position,framePos,sizeX,sizeY,amountframes,Decal);
		m_vecParticles.push_back(Mirror); // Add the Bandit entity to the game
		*it = nullptr;                  // Удаляем заспаунинного бандита из пула разименовав указатель
		return entity;
	}
	return nullptr;;
}




cDynamic* RPG_Engine::SpawnItem(const olc::vf2d position, cItem* item)
{
	for (auto it = ItemPool.begin(); it != ItemPool.end(); it++)
	{
		cDynamic* entity = *it;
		if (entity == nullptr) {
			continue; // Пропустить пустые ячейки
		}
		cDynamic_Item* Item = (cDynamic_Item*)entity;
		Item->clearFlag(Item->bDead);
		Item->Spawn(position, item);
		m_vecDynamics.push_back(Item); // Add the Bandit entity to the game
		*it = nullptr;                  // Удаляем заспаунинного бандита из пула
		return entity;
	}
	return nullptr;
}

cDynamic* RPG_Engine::SpawnBattleText(const olc::vf2d position, std::string Text, olc::Pixel Color )
{

	for (auto it = TextPool.begin(); it != TextPool.end(); ++it) {
		cDynamic* entity = *it;
		
		cDynamic_TextDamage* Textentity = (cDynamic_TextDamage*)entity;
		if (entity == nullptr) {
			continue; // Пропустить пустые ячейки
		}

		Textentity->setFlag(Textentity->bRedundant);
		m_vecFightText.push_back(Textentity); // Add the Bandit entity to the game
		Textentity->px = position.x;
		Textentity->py = position.y;
		Textentity->SetText(Text);
		*it = nullptr;                  // Удаляем заспаунинного бандита из пула


		return entity; // Exit the function after spawning a Bandit

	}
	return nullptr; // Exit the function after spawning a Bandit
}


void RPG_Engine::SpawnRainDrops(const uint16_t index)  // 0-100
{
	if (EnvironmentPool[index] !=nullptr)
	{
		
	m_vecCloseWeather.push_back(EnvironmentPool[index]);
	EnvironmentPool[index] = nullptr;
	}
	else
	{
		std::cout << "SpawnRainDrops() Return Nullptr" << std::endl;
	}


}
void RPG_Engine::SpawnRainSpray(float px, float py, const uint16_t Index)
{


//	{
	//	int freeIndex = freeRainSprayIndices.back(); // Получаем последний свободный индекс
	//	freeRainSprayIndices.pop_back(); // Удаляем его из списка свободных

		if (EnvironmentPool[Index] != nullptr)
		{
			EnvironmentPool[Index]->SetPos(px, py);
			m_vecCloseWeather_2.push_back(EnvironmentPool[Index]);
			EnvironmentPool[Index] = nullptr;  // освобождаем слот согласно индексу
		}

	

}
void RPG_Engine::SpawnClouds(const uint16_t index) // 400-405
{
	if (EnvironmentPool[index] != nullptr)
	{

		m_vecFarWeather.push_back(EnvironmentPool[index]);
		EnvironmentPool[index] = nullptr;
	}
	else
	{
		std::cout << "SpawnClouds() Return Nullptr" << std::endl;
	}
}

void RPG_Engine::removeStoppedSounds()
{
	auto it = sounds.begin();
	while (it != sounds.end()) {

		sf::Sound* ex = *it;
		
		if (ex->getStatus() == sf::Sound::Stopped) {
			// Возвращаем звук обратно в пул
			m_vecSoundsPool.push_back(std::move(*it));
			it = sounds.erase(it);  // Удаляем его из списка проигрываемых звуков
		}
		else {
			++it;
		}
	}
}
void RPG_Engine::bufferClockMask()
{


	spMask = new olc::Sprite(64, 64);

	for (int y = 0; y < 64; y++)
		for (int x = 0; x < 64; x++)
			spMask->SetPixel(x, y, olc::Pixel(0, 0, 0, 0));  // Чёрные пиксели — спрайт скрыт

	angles.resize(64 * 64);

	maskcenter = { 64.0f / 2.0f,64.0f / 2.0f };

	for (int y = 0; y < 64; y++) {
		for (int x = 0; x < 64; x++) {
			float dx = x - maskcenter.x;
			float dy = y - maskcenter.y;

			float angle = atan2f(dy, dx);
			angle += (3.14159f / 2);
			if (angle < 0) angle += 2.0f * 3.14159f;    // атан2f возвращает значения от -пи до +пи, а нам надо от 0 до 2пи, для корректности
			angles[y * spMask->width + x] = angle;
		}
	}
}
void RPG_Engine::bufferSinusMask(float amplitude, float waves, float position, float phaseoffset)
{
	float x = 0.0f;

	for (int y = 0; y < 64; y++) {



		float sinx = amplitude * std::sin(waves * x + phaseoffset) + position;




		x += 0.1f;
		Sinnum[y] = sinx;

	}

}
void RPG_Engine::clockwiseMask(float felapsedtime, olc::Sprite* test)
{
	maskanim += felapsedtime;

	float angle = maskanim * 10; // Увеличиваем угол со временем  начиная с 0, дойдя до 1, мы совершим полный оборот по окружности

	for (int y = 0; y < 64; y++)
	{

		for (int x = 0; x < 64; x++)
		{
			if (test->GetPixel(x, y).a == 0) // Проверяем только прозрачные пиксели
			{

				float pixelAngle = angles[y * 64 + x];

				if (pixelAngle <= angle)
				{
					test->SetPixel(x, y, olc::Pixel(0, 0, 0, 255));
				}
			}

		}
	}
}
void RPG_Engine::sinusoidMask(float felapsedtime, olc::Sprite* test, float amplitude)
{
	maskanim += felapsedtime * 10;

	for (int y =amplitude; y < 64; y++)
	{

		for (int x = 0; x < 64; x++)
		{

			float flowx = x + maskanim;

			flowx = (int)flowx % 64;



			float xin = Sinnum[flowx];


			if (xin < y)
			{

				test->SetPixel(x, y, olc::Pixel(0, 0, 0, 255));
			}
			else
			{
				test->SetPixel(x, y, olc::Pixel(0, 0, 0, 0));
			}

		}
	}
}
//void::RPG_Engine::LoadSound()
//{
//
//}
//void RPG_Engine::LoadMusc()
//{
//	// Загрузка звуковых эффектов
//	loadSound("TakeDamage", "D:/C++/Pantir_Platformer/Sounds/TakeDamageSound.wav");
//
//	loadSound("SwordSwing", "D:/C++/Pantir_Platformer/Sounds/HitSound.wav");
//
//	loadSound("SwordSwingTwo", "D:/C++/Pantir_Platformer/Sounds/HitSound2.wav");
//
//	loadSound("Jump", "D:/C++/Pantir_Platformer/Sounds/JumpSound.wav");
//
//	loadSound("Landed", "D:/C++/Pantir_Platformer/Sounds/LandedSound.wav");
//
//	loadSound("Run", "D:/C++/Pantir_Platformer/Sounds/RunSound.wav");
//
//	loadSound("BackStab", "D:/C++/Pantir_Platformer/Sounds/HitLandedSound.wav");
//
//
//
//}



void RPG_Engine::ReturnEnvironmentToPool(Environment* entity) {


	

	if (EnvironmentPool[entity->arrIndex] == nullptr)
	{
		entity->redundant = false;
		EnvironmentPool[entity->arrIndex] = entity;

	}
	else
	{
		std::cout << "ReturnToPool() Mistake" << std::endl;
	}
}

