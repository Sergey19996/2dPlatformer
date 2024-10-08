#include "RPG_Engine.h"




RPG_Engine::RPG_Engine()
{
    sAppName = "Pantrir platformer";
}

void RPG_Engine::Drawcursor(olc::vi2d mouse)
{
	const float x = 64 % 32;
	const float y = 64 / 32;
	//Mouse
	DrawPartialDecal({ (float)(mouse.x - 0.15f) , (float)(mouse.y - 0.1f) }, m_Items, { x * 64,y * 64 }, { (float)64,(float)(64) }, { (float)0.5*fscale,(float)0.5*fscale });
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
	SetLearnedTalent((RPG_Assets::get().GetUiElements("Attack Right")));

	//SetLearnedTalent((RPG_Assets::get().GetUiElements("Attack Right Flight Up")));   // locks for attack
	SetLearnedTalent((RPG_Assets::get().GetUiElements("Attack Right Air")));

	SetLearnedTalent((RPG_Assets::get().GetUiElements("Attack Back")));

	SetLearnedTalent((RPG_Assets::get().GetUiElements("Vanish")));

	//


	//                  THis is when we add in vector which we drawning ing game like backstub 
	AddUi((RPG_Assets::get().GetUiElements("Attack Back")));
	AddUi((RPG_Assets::get().GetUiElements("Appear Behind")));

	AddUi((RPG_Assets::get().GetUiElements("Swirl Attack")));


	AddUi((RPG_Assets::get().GetUiElements("NewEnergyIndicator_1")));
	AddUi((RPG_Assets::get().GetUiElements("NewEnergyIndicator_2")));
	AddUi((RPG_Assets::get().GetUiElements("NewEnergyIndicator_3")));

	AddUi((RPG_Assets::get().GetUiElements("NewRageIndicator_1")));
	AddUi((RPG_Assets::get().GetUiElements("NewRageIndicator_2")));
	AddUi((RPG_Assets::get().GetUiElements("NewRageIndicator_3")));
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


	



	//m_vecEquip[6]->Item =
//	m_vecEquip.push_back(m_pPlayer->pEquipedNeck);  //  6 equip index
	//m_vecEquip.push_back(m_pPlayer->pEquipedHelmet); // 5 equip index
//m_vecEquip.push_back(m_pPlayer->pEquipedWeapon); //1 index
//	m_vecEquip.push_back(m_pPlayer->pEquipedChest); // 4 equip index
//	m_vecEquip.push_back(m_pPlayer->pEquipedBack);  // 2 equipindex
//	m_vecEquip.push_back(m_pPlayer->pEquipedBoots); // 3 equip index





	for (size_t i = 0; i < 2; i++)
	{
		InventaryItem* empty = nullptr;
		//empty->index = i;
		m_vecUi.push_back(empty);

	}


	for (int i = 0; i < 24; i++)
	{
		InventaryItem* empty = new InventaryItem();
		empty->index = i;

		m_listItems.push_back(empty);
	}
	for (int i = 0; i < 24; i++)
	{
		InventaryItem* empty = new InventaryItem();
		empty->index = i;
		//empty->bInWarehouse = 1;
		empty->setFlag(empty->binWarehouse);

		m_listWarehouseItems.push_back(empty);
	}
	for (int i = 0; i < 24; i++)
	{
		InventaryItem* empty = new InventaryItem();
		empty->index = i;
		//empty->bInWarehouse = 1;

		m_listBlackSmithItems.push_back(empty);
	}
	for (int i = 0; i < 24; i++)
	{
		InventaryItem* empty = new InventaryItem();
		empty->index = i;
		//empty->bInWarehouse = 1;

		m_listStoreItems.push_back(empty);
	}

}

bool RPG_Engine::OnUserCreate()
{
	hWnd = GetConsoleWindow();
	

	SetMouseFocus(true);
	cDynamic::g_engine = this;
	cItem::g_engine = this;
	cUI::g_engine = this;
	
	
	cQuest::g_script = &m_script;
	cQuest::g_engine = this;
	cComand::g_engine = this;

	cMap::g_script = &m_script;
	cMap::g_engine = this;
	
    RPG_Assets::get().LoadSprites();               //Load sprites
	RPG_Assets::get().LoadItems();

	LoadenemyInstances();

	RPG_Assets::get().LoadMaps();
	RPG_Assets::get().LoadUiElements();
	RPG_Assets::get().LoadQuests();
	RPG_Assets::get().LoadMusc();
    


	LoadSoundPool(20);
	sf::SoundBuffer* buffer = RPG_Assets::get().findSound("Run");
	WalkSound->setBuffer(*buffer);
	WalkSound->setVolume(25);

	m_sprFont = RPG_Assets::get().GetSprite("font");   //take font


	
	m_listQusets.push_front(RPG_Assets::get().GetQuest(0)); // add in lust First Main quest

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



	CellSize = 64 * fscale;
	nVisibleTileX = ScreenWidth() / CellSize;
	nVisibleTileY = ScreenHeight() / CellSize;
	batchZeroCharsLayer.reserve((nVisibleTileX + 8) * (nVisibleTileY + 8));
	//AddUi((RPG_Assets::get().GetUiElements("Jump")));


	
	//m_vecHpBars.push_back(RPG_Assets::get().GetIndicators("HpFirst"));




	layer = CreateLayer();
	layerOne = CreateLayer();
	layerTwo = CreateLayer();
	




//	m_sprPressF = RPG_Assets::get().GetSprite("PressF");
	m_Items = RPG_Assets::get().GetSprite("Items");
	Inventoryback = RPG_Assets::get().GetSprite("inventory");
	D_Ui = RPG_Assets::get().GetSprite("FullUi");


	// Initialize the raindrops
	for (int i = 0; i < 100; ++i)
	{

	SpawnRainDrops();
	}


	for (int i = 0; i < 5; ++i)
	{
	SpawnClouds();
	}

    return true;
}

bool RPG_Engine::OnUserUpdate(float fElapsedTime)
{
	bool Result;

	// Define a fixed time step for achieving desired frame rate (60 FPS)
	const float targetFrameTime = 1.0f / 60.0f; // 60 FPS = 1/60 секунды

	// Remove stopped sounds
	removeStoppedSounds();

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
		Result = UpdateLocalMap(fElapsedTime);
		break;
	}

	// Check how much time has passed
	float fSleepTime = targetFrameTime - fElapsedTime; // Calculate remaining time

	// Sleep if necessary
//	if (fSleepTime > 0.0f) {
//		std::this_thread::sleep_for(std::chrono::duration<float>(fSleepTime));
	//}

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
		sound2->setVolume(25);  // Устанавливаем громкость по необходимости
		sound2->play();
		sounds.push_back(sound2);
	}

}


bool RPG_Engine::UpdateTitleScreen(float fElapsedTime)   // <---MAIN MENU Start
{
	Clear(olc::BLANK);

	SetDrawTarget(layer);
	//SetPixelMode(olc::Pixel::ALPHA);
	//SetPixelMode(olc::Pixel::MASK);
	
	
	//Update script 
	m_script.ProcessCommands(fElapsedTime);

	olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };
	DrawPartialDecal({ (float)0, (float)0 }, RPG_Assets::get().GetSprite("MainMenuFarLayer"), { 0,0 }, { (float)RPG_Assets::get().GetSprite("MainMenuFarLayer")->sprite->width, (float)RPG_Assets::get().GetSprite("MainMenuFarLayer")->sprite->height }, {1, 1});   //<-- Draw sprite under rectangle
	DrawPartialDecal({ (float)0, (float)0 }, RPG_Assets::get().GetSprite("MainMenuMidLayer"), { 0,0 }, { (float)RPG_Assets::get().GetSprite("MainMenuMidLayer")->sprite->width,  (float)RPG_Assets::get().GetSprite("MainMenuMidLayer")->sprite->height }, { 1,1});
	DrawPartialDecal({ (float)0, (float)ScreenHeight()- (float)RPG_Assets::get().GetSprite("MainMenuCloseLayer")->sprite->height }, RPG_Assets::get().GetSprite("MainMenuCloseLayer"), {0,0}, {(float)RPG_Assets::get().GetSprite("MainMenuCloseLayer")->sprite->width, (float)RPG_Assets::get().GetSprite("MainMenuCloseLayer")->sprite->height}, {1,1});

	MainMenuAnim += fElapsedTime;
	if (MainMenuAnim >0.1f)
	{
		MainMenuAnim -= MainMenuAnim;
		MainMenuCalc++;

	}

	if (MainMenuCalc >10)
	{
		MainMenuCalc = 0;
	}
	MainMenuX = MainMenuCalc %6;
	MainMenuY = MainMenuCalc /6;


	DrawPartialDecal({ ScreenWidth()/2.0f+(75*fscale), ScreenHeight()/2.0f-30}, RPG_Assets::get().GetSprite("MainMenuBardChar"), {MainMenuX * 64.0f,MainMenuY * 64.0f}, {(float)64, (float)64}, { (float)1.4,(float)1.4});



	DrawPartialDecal({ ScreenWidth()-(250.0f*fscale), ScreenHeight()/2.0f}, RPG_Assets::get().GetSprite("MainMenuFirstChar"), {MainMenuX * 64.0f,MainMenuY * 64.0f }, {(float)64, (float)64}, { (float)1.4,(float)1.4});


	DrawPartialDecal({ScreenWidth()-(750.0f*fscale), ScreenHeight()/2.0f-(30*fscale)}, RPG_Assets::get().GetSprite("MainMenuSecondChar"), {MainMenuX * 64.0f,MainMenuY * 64.0f }, {(float)64, (float)64}, { (float)1.4,(float)1.4});

	DrawPartialDecal({ ScreenWidth()-(158.0f*fscale), ScreenHeight()/2.0f-(200*fscale)}, RPG_Assets::get().GetSprite("MainMenuWhisps"), {MainMenuX * (40.0f*fscale),MainMenuY * (13.0f * fscale)}, {(float)40*fscale, (float)13*fscale}, { (float)1.4,(float)1.4});

	DrawPartialDecal({ ScreenWidth()/2+(35.0f*fscale), ScreenHeight()/2+(55.0f*fscale)}, RPG_Assets::get().GetSprite("MainMenuCampFire"), {MainMenuX * (76.0f * fscale),MainMenuY * (94.5f * fscale)}, {(float)(76*fscale), (float)94*fscale }, { (float)1.4,(float)1.4});



	float spelluix, spelluiy;
	spelluix = 25.0f;
	spelluiy = ScreenHeight() - (150*fscale);

	DrawPartialDecal({ spelluix, spelluiy }, D_Ui, { 832,0 }, { 334,142 },{(float)0.7*fscale,(float)1*fscale});  // spell Ui
	//DrawBigText("Travel ?", (ScreenWidth() / 2) - 72, ScreenHeight() / 2 - 62, 1, 1, olc::WHITE);

	//DrawBigText("Continue", 50, ScreenHeight() - 250, 1.5, 1.5, olc::GREY);
	DrawBigText("New Game", spelluix+(25*fscale), spelluiy +(30*fscale), 1*fscale, 1*fscale, olc::WHITE);

	DrawBigText("Options", spelluix+(25*fscale), spelluiy + (60*fscale), 1*fscale, 1*fscale, olc::WHITE);

	DrawBigText("Quit", spelluix+(25*fscale), spelluiy +(90*fscale), 1*fscale, 1*fscale, olc::WHITE);


	
	std::ofstream data;
	data.open("Load/CurrSave.txt", std::ofstream::in);
	if (data.is_open())
	{
		DrawBigText("Continue", spelluix + (25*fscale), spelluiy, 1*fscale, 1*fscale, olc::WHITE);
	}
	else
	{
		DrawBigText("Continue", spelluix + (25*fscale), spelluiy, 1*fscale, 1*fscale, olc::GREY);
	}

	if (GetMouse(0).bPressed) //LeftMouse
	{

		if ((int)mouse.x >= spelluix + (25*fscale) && (int)mouse.y >= spelluiy + (30*fscale) && (int)mouse.x <= spelluix + (233*fscale) && (int)mouse.y <= spelluiy + (59*fscale)) //New Game
		{

		

			data.close();
			m_script.AddCommand((new cComand_HideScreen(2)));
			m_script.AddCommand((new cComand_Changemap("Forest",-1,6)));

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

			ScrollingCurrX = fCameraPosX;

		//	n_nGameMode = MODE_LOCAL_MAP;
		}

		if ((int)mouse.x >= spelluix + (25*fscale) && (int)mouse.y >= spelluiy + (90*fscale) && (int)mouse.x <= (250*fscale) && (int)mouse.y <= spelluiy + (120*fscale) ) //New Game
		{
			return false;
		}

		if ((int)mouse.x >= spelluix + (25*fscale) && (int)mouse.y >= spelluiy && (int)mouse.x <= (266*fscale) && (int)mouse.y <= spelluiy + (29*fscale)) //Continue
		{
			if (data.is_open())
			{
				data.close();
		//		m_script.AddCommand((new cComand_HideScreen(2)));
				m_script.AddCommand((new cComand_LoadFunction));  // in load function in start allredy 2 comands but they call not instant 
			


			}
			
		


		}
	}
	if ((int)mouse.x >= spelluix + (25*fscale) && (int)mouse.y >= spelluiy && (int)mouse.x <= (266*fscale) && (int)mouse.y <= spelluiy + (29*fscale)) //Continue
	{
		DrawBigText("Continue", spelluix + (25*fscale), spelluiy, fscale * 1, fscale * 1, olc::YELLOW);
	}

	if ((int)mouse.x >= spelluix + (25*fscale) && (int)mouse.y >= spelluiy + (30*fscale) && (int)mouse.x <= spelluix +  (233*fscale) && (int)mouse.y <= spelluiy + (59*fscale)) //New Game
	{
		DrawBigText("New Game", spelluix + (25*fscale), spelluiy + (30*fscale), fscale * 1, fscale*1, olc::YELLOW);
	}

	if ((int)mouse.x >= spelluix + (25*fscale) && (int)mouse.y >= spelluiy +(90*fscale) && (int)mouse.x <= (250*fscale)&& (int)mouse.y <= spelluiy + (120*fscale)) //New Game
	{
		DrawBigText("Quit", spelluix + (25*fscale), spelluiy + (90*fscale), fscale * 1, fscale * 1, olc::YELLOW);
	}

	

	 Drawcursor(mouse);
	


	if (bSmoothAppearScreen)
	{

		DrawDecal({ (float)0, (float)0 }, RPG_Assets::get().GetSprite("DescriptionPattern"), { 2*fscale,2*fscale }, olc::Pixel{ 255,255,255,(uint8_t)AlphaAppearScreen });
	}


	

	//	void clockwiseMask(float felapsedtime, olc::Sprite* test)


	//	 sinusoidMask(fElapsedTime, test, 32-16);
		

	

	EnableLayer(layer, true);
	SetDrawTarget(nullptr);

	

	// Limit the frame rate to 60 frames per second

	

	DrawRect({ ScreenWidth() / 2, ScreenHeight() / 2 }, { 64,64 });
	

		
	return true;     
}     
// <---MAIN MENU END



bool RPG_Engine::UpdateLocalMap(float fElapsedTime)
{
	

//	RPG_Assets::get().removeStoppedSounds();


	
	olc::vi2d mouse = { (GetMouseX()), (GetMouseY()) };
	//std::cout << enemysPool.size() << std::endl;

	
	

		
		//std::cout <<fElapsedTime << std::endl;

		if (m_pPlayer->checkFlag(m_pPlayer->bDead))
		{
		//	SaveFunction();
			m_pPlayer->clearFlag(m_pPlayer->bDead);
			m_pPlayer->nHealth = 1;
		//	m_pPlayer->SwitchLayer(2);
			m_script.AddCommand((new  cComand_LoadFunction));

	
		}

		//Update script 
		m_script.ProcessCommands(fElapsedTime);

	

		m_vecParticles.erase(
			remove_if(m_vecParticles.begin(), m_vecParticles.end(),
				[](cDynamic* d) {
					if (((cDynamic_Projectile*)d)->checkFlag(d->bRedundant)) {
						delete d;  // Освобождение памяти объекта
						return true;  // Удалить указатель из контейнера
					}
		return false;  // Оставить указатель в контейнере
				}),
			m_vecParticles.end());

		m_vecProjectiles.erase(
			remove_if(m_vecProjectiles.begin(), m_vecProjectiles.end(),
				[&](cDynamic* d) {
					if (((cDynamic_Projectile*)d)->checkFlag(d->bRedundant)) {
						ReturnProjectileToPool(d);
						return true;
					}
		return false;
				}),
			m_vecProjectiles.end());

		m_vecIndicators.erase(
			remove_if(m_vecIndicators.begin(), m_vecIndicators.end(),
				[](cDynamic* d) {
					if (((cDynamic_HpBar*)d)->checkFlag(d->bRedundant)) {
						delete d;
						return true;
					}
		return false;
				}),
			m_vecIndicators.end());


		m_vecDynamics.erase(                              //check m_vecProjectiles on flag -bredundand and erase it    hp bars the same in vecdynamics
			remove_if(m_vecDynamics.begin(), m_vecDynamics.end(),
				[&]( cDynamic* d) {
					if (((cDynamic_Creature*)d)->checkFlag(d->bDead)) {

						ReturnToPool(d);
						return true;
					}
					
					return false;
				}), m_vecDynamics.end());
				

		m_vecFightText.erase(
			remove_if(m_vecFightText.begin(), m_vecFightText.end(),
				[&](cDynamic* d) {
				if (((cDynamic_TextDamage*)d)->checkFlag(d->bRedundant)) {
						ReturnTextToPool(d);
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
			}

	

		if (GetKey(olc::Key::ESCAPE).bPressed)
		{
			if (n_nGameMode == MODE_LOCAL_MAP || n_nGameMode==MODE_WAREHOUSE || n_nGameMode == MODE_SHOP ||n_nGameMode==MODE_INVENTORY|| n_nGameMode == MODE_MAP || n_nGameMode ==MODE_PROFESSION || n_nGameMode ==MODE_BLACKSMITH)
			{
			n_nGameMode = 1;
			bPause = !bPause;
			}

			

		}

		if (GetKey(olc::Key::I).bPressed)
		{
			
			 if (n_nGameMode == MODE_INVENTORY)
			{
				bPause = false;
				n_nGameMode = MODE_LOCAL_MAP;     // <-----Inventory
			}

		}
		

		if (!bPause)

		{
			



			if (m_script.bUserControlEnabled )
			{



				if (GetKey(olc::Key::I).bPressed)
				{
					if (n_nGameMode == MODE_LOCAL_MAP)
					{
						bPause = true;
						n_nGameMode = MODE_INVENTORY;     // <-----Inventory
					}
					

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
									WalkSound->stop();
									break;
								case 1:
									m_pPlayer->MoveDoubleJump();
									break;
								}

							}

						}
						if (GetKey(olc::Key::W).bHeld)
						{

							m_pPlayer->SetVerticalDirection(1); // <--North

						}
						if (GetKey(olc::Key::S).bHeld)
						{

							m_pPlayer->SetVerticalDirection(2);  // <--South
						}
						if (GetKey(olc::Key::Q).bReleased)
						{
							if (m_vecUi[0] != nullptr)
							{
							if(m_vecUi[0]->Item->OnUse(m_pPlayer,m_listItems,m_vecUi[0]))
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
						m_script.AddCommand((new cComand_Changemap("Forest",5,25.5)));

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
									if (m_pPlayer->rageAmount >= 35 && GetLearnedTalent(11) && m_pPlayer->enumCounter != 8)
									{
										//Sonner we change it decition
										m_pPlayer->RageMoveAttackUp();
									}
								}

								break;
							}



						}

						if (!m_pPlayer->checkFlag(m_pPlayer->isAttack))
						{





							m_pPlayer->SetVerticalDirection(3);

							if (GetKey(olc::Key::D).bHeld)
							{

								m_pPlayer->vx += (m_pPlayer->checkFlag(m_pPlayer->bOnGround) ? 35.0f * m_pPlayer->GetMovement() : 25.0f * m_pPlayer->GetMovement()) * fElapsedTime;
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


							if (GetKey(olc::Key::A).bHeld)
							{

								m_pPlayer->vx += (m_pPlayer->checkFlag(m_pPlayer->bOnGround) ? -35.0f * m_pPlayer->GetMovement() : -25.0f * m_pPlayer->GetMovement()) * fElapsedTime;
								
							}
						
							if (GetMouse(0).bPressed)   // <--LeftAttack
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



											PlaySounds("SwordSwingTwo");




										}

										float fTestX, fTestY;
										defineFacingDirection(fTestX, fTestY);
										for (auto dyns : m_vecVisibleDynamics)
										{
											if ((fTestX + 0.9f) > dyns->px + dyns->CollbordersXF && (fTestX - 0.9f) < dyns->px + dyns->CollbordersX && fTestY > dyns->py + dyns->CollbordersY && fTestY < dyns->py + dyns->CollbordersYF)
											{
												if (!dyns->checkFlag(m_pPlayer->bOnGround))

													if (dyns->m_layer == 1 && m_pPlayer->checkFacingDirection(dyns) && GetLearnedTalent(6))
													{

														m_pPlayer->SwirlGrab(dyns);

														break;

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


													PlaySounds("BackStab");



													break;

												}



											}
										}

										if (m_pPlayer->enumCounter != 16) //  no EnergyMove AttackBack
										{


											if (m_pPlayer->energyAmount >= 99 && GetLearnedTalent(3))                         // <-- Attack on ground
											{
												m_pPlayer->EnergyMoveAttackHigh();

												PlaySounds("SwordSwingTwo");

												//currsound = RPG_Assets::get().playSound("SwordSwingTwo");
												//currsound.play();

											}
											else
											{

												if (m_pPlayer->energyAmount >= 55 && GetLearnedTalent(2))
												{
													m_pPlayer->EnergyMoveAttackMid();

													PlaySounds("SwordSwingTwo");

													//currsound = RPG_Assets::get().playSound("SwordSwing");
												//	currsound.play();
												}
												else
												{

													if (m_pPlayer->energyAmount >= 35 && GetLearnedTalent(1))   //EnergyAmount - Spellcount 

													{

														m_pPlayer->EnergyMoveAttackLow();

														PlaySounds("SwordSwingTwo");

													}
												}
											}
										}
									}
								}
							}

								if (GetKey(olc::Key::F).bPressed)
								{
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
							if (GetMouse(0).bReleased)  // when we threw out object from ui 
							{
								if (GrabItem != nullptr)
								{
									GrabItem->clearFlag(GrabItem->Grabitem);
									//GrabItem->GrabItem = false;
									m_vecUi[GrabItem->Uiindex] = nullptr;
									GrabItem = nullptr;


								}
							}
						}

						if (m_pPlayer->checkFlag(m_pPlayer->bOnGround) && m_pPlayer->fAttackcount <= 0.5f && m_pPlayer->fAttackcount >= 0.1f)
						{
							if (GetMouse(0).bHeld)
							{
								if (m_pPlayer->fAttackcount <= 0.0f)
								{
									m_pPlayer->fAttackcount = 0.0f;
								}
								else
								{
									m_pPlayer->fAttackcount -= fElapsedTime;
								}

							}
							if (GetMouse(0).bReleased)
							{
								

								if ((0.5f - m_pPlayer->fAttackcount >= 0.17f && 0.5f - m_pPlayer->fAttackcount <= 0.32f && m_pPlayer->energyAmount >= 55 && m_pPlayer->enumCounter != 3) && GetLearnedTalent(2)) //enumcount ==3 - ATTACKMID
								{
									m_pPlayer->EnergyMoveAttackMid();
									PlaySounds("SwordSwing");
								}
								else if ((0.5f - m_pPlayer->fAttackcount >= 0.0f && 0.5f - m_pPlayer->fAttackcount <= 0.17 && m_pPlayer->energyAmount >= 35 && m_pPlayer->enumCounter != 4))  // enumcounter ==4 AttackEasy
								{
									m_pPlayer->EnergyMoveAttackLow();

									PlaySounds("SwordSwing");
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
						m_script.CompleteCommand();
					}

				}

			}
			////////////               locks for speed 


float cameraSpeed = 0.01f;
float mouseSnapFactor = 0.001f; // Adjust this value as needed

if (IsFocused())
{


	// Get offset for smooth movement
	if (!blockCamera)
	{
		if (m_pPlayer->Gettarget() == nullptr)
		{


		// Calculate the distance between the current camera position and the player's position
	//	float deltaX = (m_pPlayer->px + (GetMouseX() - ScreenWidth() / 2) * mouseSnapFactor) - fCameraPosX;
	//	float deltaY = (m_pPlayer->py + (GetMouseY() - ScreenHeight() / 2) * mouseSnapFactor) - fCameraPosY;

		// Snap the camera towards the player and mouse position
		fCameraPosX = m_pPlayer->px + ((m_pPlayer->CollbordersXF - m_pPlayer->CollbordersX) / 2);
		fCameraPosY = m_pPlayer->py + (m_pPlayer->CollbordersYF - m_pPlayer->CollbordersY) / 2;

		// Apply interpolation to smooth out the movement
	//	fCameraPosX += deltaX * cameraSpeed;
	//	fCameraPosY += deltaY * cameraSpeed;
		}
		else
		{


			

			float X = m_pPlayer->px -((m_pPlayer->px - m_pPlayer->Gettarget()->px)/2);
			float Y = m_pPlayer->py - ((m_pPlayer->py - m_pPlayer->Gettarget()->py)/2);

			float deltaX = (X + (GetMouseX() - ScreenWidth() / 2) * mouseSnapFactor) - fCameraPosX;
			float deltaY = (Y + (GetMouseY() - ScreenHeight() / 2) * mouseSnapFactor) - fCameraPosY;

			// Snap the camera towards the player and mouse position
			fCameraPosX = X + (GetMouseX() - ScreenWidth() / 2) * mouseSnapFactor;
			fCameraPosY = Y - 1 + (GetMouseY() - ScreenHeight() / 2) * mouseSnapFactor;

			// Apply interpolation to smooth out the movement
			fCameraPosX += deltaX * cameraSpeed;
			fCameraPosY += deltaY * cameraSpeed;
		}

		// Interpolate camera position towards the character
	//	fCameraPosX = Lerp(fCameraPosX, m_pPlayer->px, cameraSpeed);
	//	fCameraPosY = Lerp(fCameraPosY, m_pPlayer->py, cameraSpeed);
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

		

			bool bWorkingWithProjectiles = false;
			bUibackstub = false;
			for (auto& source : { &m_vecVisibleDynamics, &m_vecProjectiles })
			{
				for (auto& object : *source) // for every chars in this vector will be calculate their move
				{


					if (object->checkFlag(object->gravity) )
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
				

				//	float middleX = object->CollbordersX + ((object->CollbordersXF - object->CollbordersX) / 2.0f);

						//Check slopes

					if (object->bSolidVsMap)
					{


						if (object->vx <= 0)  //move Left
						{



							if (m_pCurrentMap->GetColliziionIndex(fNewObjectPosX + object->CollbordersX, object->py + object->CollbordersY) == 1 || m_pCurrentMap->GetColliziionIndex(fNewObjectPosX + object->CollbordersX, object->py + object->CollbordersYF - 0.2f) == 1)
							{

								fNewObjectPosX = object->px;  //if there won't emprty cell we move right
							
							
								object->vx = 0;

							}
							
							

							

							if (m_pCurrentMap->GetColliziionIndex(fNewObjectPosX + object->CollbordersX, object->py + object->CollbordersYF) == 2 && m_pCurrentMap->GetColliziionIndex(fNewObjectPosX + object->CollbordersXF,object->py + object->CollbordersYF) == 1)
							{
							


								if (object->obsticlepoints == nullptr)
									object->obsticlepoints = (m_pCurrentMap->getObsticlesPoints(object->px + object->CollbordersX, object->px + object->CollbordersXF, object->py + object->CollbordersY, object->py + object->CollbordersYF));




								float deltaX = object->obsticlepoints->second->x - object->obsticlepoints->first->x;
								float deltaY = object->obsticlepoints->first->y - object->obsticlepoints->second->y;
								float slopeRatio = deltaY / deltaX;
								float deltaYx = slopeRatio * -object->vx;


								float charX = fNewObjectPosX + object->CollbordersXF - object->obsticlepoints->first->x;


								float sloperatio = charX * slopeRatio;

								float reservgravity = object->mass * fElapsedTime;


								
								//	fNewObjectPosX += DeltaX;
								if (fNewObjectPosY + object->CollbordersYF >= object->obsticlepoints->first->y - sloperatio)
								{
								 
									object->vy = 49.0f;

									//object->vy = 

									fNewObjectPosY = object->obsticlepoints->second->y - object->CollbordersYF;

									if (!object->checkFlag(object->bOnGround))                                // <- we trying to make in OnLanded anim
									{
										object->IsLanded();
									}

									object->Jumpcounter = 0;
									object->setFlag(object->bOnGround);
									bslope = true;

								}
							}
							
							else if (m_pCurrentMap->GetColliziionIndex(fNewObjectPosX + object->CollbordersXF, object->py + object->CollbordersYF) == 2)
							{





								if (object->obsticlepoints == nullptr)
									object->obsticlepoints = (m_pCurrentMap->getObsticlesPoints(object->px + object->CollbordersX, object->px + object->CollbordersXF, object->py + object->CollbordersY, object->py + object->CollbordersYF));




								float deltaX = object->obsticlepoints->second->x - object->obsticlepoints->first->x;
								float deltaY = object->obsticlepoints->first->y - object->obsticlepoints->second->y;
								float slopeRatio = deltaY / deltaX;
								float charX = fNewObjectPosX + object->CollbordersXF - object->obsticlepoints->first->x;
								float sloperatio = charX * slopeRatio;
								




								if (sloperatio < 0.05f)
								{
									sloperatio = 0;
								}
								

								//	fNewObjectPosX += DeltaX;
								if (fNewObjectPosY + object->CollbordersYF >= object->obsticlepoints->first->y - sloperatio)
								{

										if (!object->checkFlag(object->bOnGround))                                // <- we trying to make in OnLanded anim
									{
										object->IsLanded();
									} 

									//object->vy = deltaYx+ reservgravity;
								
									object->vy = 49.0f;
									fNewObjectPosY = object->obsticlepoints->first->y - sloperatio - object->CollbordersYF;
									object->Jumpcounter = 0;
									object->setFlag(object->bOnGround);
									bslope = true;


									


								}



							}
						}
						else   //move right
						{
							

							if (m_pCurrentMap->GetColliziionIndex(fNewObjectPosX + object->CollbordersXF, object->py + object->CollbordersY ) == 1 || m_pCurrentMap->GetColliziionIndex(fNewObjectPosX + object->CollbordersXF, object->py + object->CollbordersYF - 0.2f) == 1)
							{

							fNewObjectPosX = object->px;  //if there won't emprty cell we move right


								object->vx = 0;

							}
								
							if (m_pCurrentMap->GetColliziionIndex(object->px + object->CollbordersX,fNewObjectPosY + object->CollbordersYF) == 1 && m_pCurrentMap->GetColliziionIndex(object->px + object->CollbordersXF, fNewObjectPosY + object->CollbordersYF) == 3)
							{



								if (object->obsticlepoints == nullptr)
									object->obsticlepoints = (m_pCurrentMap->getObsticlesPoints(object->px + object->CollbordersX, object->px + object->CollbordersXF, object->py + object->CollbordersY, object->py + object->CollbordersYF));




								float deltaX = object->obsticlepoints->second->x - object->obsticlepoints->first->x;
								float deltaY = object->obsticlepoints->second->y - object->obsticlepoints->first->y;
								float slopeRatio = deltaY / deltaX;

								float deltaYx = slopeRatio * object->vx;


								float charX = object->obsticlepoints->second->x - object->px - object->CollbordersX;


								float sloperatio = charX * slopeRatio;

								float reservgravity = object->mass * fElapsedTime;



								//	fNewObjectPosX += DeltaX;
								if (fNewObjectPosY + object->CollbordersYF >= object->obsticlepoints->second->y - sloperatio)
								{

									object->vy = 49.0f;

									//object->vy = 

									fNewObjectPosY = object->obsticlepoints->first->y - object->CollbordersYF;

									if (!object->checkFlag(object->bOnGround))                                // <- we trying to make in OnLanded anim
									{
										object->IsLanded();
									}

									object->Jumpcounter = 0;
									object->setFlag(object->bOnGround);
									bslope = true;

								}
							}

							else if (m_pCurrentMap->GetColliziionIndex(fNewObjectPosX + object->CollbordersX, object->py + object->CollbordersYF) == 3)
							{




								if (object->obsticlepoints == nullptr)
									object->obsticlepoints = (m_pCurrentMap->getObsticlesPoints(object->px + object->CollbordersX, object->px + object->CollbordersXF, object->py + object->CollbordersY, object->py + object->CollbordersYF));




								float deltaX = object->obsticlepoints->second->x - object->obsticlepoints->first->x;
								float deltaY = object->obsticlepoints->second->y - object->obsticlepoints->first->y;
								float slopeRatio = deltaY / deltaX;

								float deltaYx = slopeRatio * object->vx;
								float reservgravity = object->mass * fElapsedTime;


								// deltaYx -= reservgravity;

								float charX = object->obsticlepoints->second->x - object->px - object->CollbordersX;


								float sloperatio = charX * slopeRatio;


								if (sloperatio < 0.05f)
								{
									sloperatio = 0;
								}



								//	fNewObjectPosX += DeltaX;
								if (fNewObjectPosY + object->CollbordersYF >= object->obsticlepoints->second->y - sloperatio)
								{

									object->vy = 49.0f;

									fNewObjectPosY = object->obsticlepoints->second->y - sloperatio - object->CollbordersYF;

									if (!object->checkFlag(object->bOnGround))                                // <- we trying to make in OnLanded anim
									{
										object->IsLanded();
									}

									object->Jumpcounter = 0;
									object->setFlag(object->bOnGround);
									bslope = true;




							




								}



							}
						}
					}
						if (object->vy <= 0)  //move Up
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

						



							 if (m_pCurrentMap->GetColliziionIndex(object->px + object->CollbordersXF, fNewObjectPosY + object->CollbordersYF) == 2)
							{


						
								

							if (object->obsticlepoints == nullptr)
							object->obsticlepoints = (m_pCurrentMap->getObsticlesPoints(object->px + object->CollbordersX, object->px + object->CollbordersXF, object->py + object->CollbordersY, object->py + object->CollbordersYF));


								

							float deltaX = object->obsticlepoints->second->x - object->obsticlepoints->first->x;
							float deltaY = object->obsticlepoints->first->y - object->obsticlepoints->second->y;
							float slopeRatio = deltaY / deltaX;
							


							float charX = object->px + object->CollbordersXF - object->obsticlepoints->first->x;


							float sloperatio = charX * slopeRatio;


							if (sloperatio < 0.1f)
							{
								sloperatio = 0;
								
							}

						



								//	fNewObjectPosX += DeltaX;
								if (fNewObjectPosY + object->CollbordersYF >= object->obsticlepoints->first->y - sloperatio)
								{
							
								

									fNewObjectPosY = object->obsticlepoints->first->y - sloperatio - object->CollbordersYF;

									if (!object->checkFlag(object->bOnGround))                                // <- we trying to make in OnLanded anim
									{
										object->IsLanded();
									}

									object->Jumpcounter = 0;
									object->setFlag(object->bOnGround);
									bslope = true;




								}
							 }
							 else if (m_pCurrentMap->GetColliziionIndex(object->px + object->CollbordersX, fNewObjectPosY + object->CollbordersYF) == 3)
							 {



								 if (object->obsticlepoints == nullptr)
									 object->obsticlepoints = (m_pCurrentMap->getObsticlesPoints(object->px + object->CollbordersX, object->px + object->CollbordersXF, object->py + object->CollbordersY, object->py + object->CollbordersYF));


							

								 float deltaX = object->obsticlepoints->second->x - object->obsticlepoints->first->x;
								 float deltaY = object->obsticlepoints->second->y - object->obsticlepoints->first->y;
								 float slopeRatio = deltaY / deltaX;

								


								// deltaYx -= reservgravity;

								 float charX = object->obsticlepoints->second->x - object->px-object->CollbordersX;


								 float sloperatio = charX * slopeRatio;


								 if (sloperatio < 0.1f)
								 {
									 sloperatio = 0;

								 }



								 //	fNewObjectPosX += DeltaX;
								 if (fNewObjectPosY + object->CollbordersYF >= object->obsticlepoints->second->y - sloperatio)
								 {

									 fNewObjectPosY = object->obsticlepoints->second->y - sloperatio- object->CollbordersYF;

									 if (!object->checkFlag(object->bOnGround))                                // <- we trying to make in OnLanded anim
									 {
										 object->IsLanded();
									 }

									 object->Jumpcounter = 0;
									 object->setFlag(object->bOnGround);
									 bslope = true;




								 }
							 }




									 if(bslope != true)
									 if (m_pCurrentMap->GetColliziionIndex(object->px + object->CollbordersXF, fNewObjectPosY + object->CollbordersYF) == 1 || m_pCurrentMap->GetColliziionIndex(object->px + object->CollbordersX, fNewObjectPosY + object->CollbordersYF) == 1)  // 1 means tough collision
								{


									object->obsticlepoints = nullptr;
									if (!object->checkFlag(object->bOnGround))                                // <- we trying to make in OnLanded anim
									{
										object->IsLanded();
									}


									fNewObjectPosY = object->py;  //if there won't emprty cell we move right

									//fNewObjectPosY = std::round(fNewObjectPosY * 10.0f) / 10.0f;

									object->vy = 0;
									object->Jumpcounter = 0;
									object->setFlag(object->bOnGround);

								}

							

							
							
							




								if (m_pCurrentMap->GetColliziionIndex(object->px + object->CollbordersX, fNewObjectPosY + object->CollbordersYF) == 5 || m_pCurrentMap->GetColliziionIndex(object->px + object->CollbordersXF, fNewObjectPosY + object->CollbordersYF) == 5)  // 5 means spikes
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




								}
							
					}

				
					
					


					if (object->vy <=-0.1f &&!bslope || object->vy>=0.1f && !bslope)
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
							if (dyn != object)  //for except cheking himself
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
											m_pCurrentMap->OnInteraction(m_vecDynamics, dyn, cMap::WALK);  //mapInteraction keeps teleport



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
										}


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

					//cDynamic* entity = dyns;
				//	if (dynamic_cast<cDynamic_Creature*>(entity)) 
				//		m_vecVisibleDynamics.push_back(entity); // Add the Bandit 
					}

					if (dyns->sName != "projectile" && dyns->sName != "Pantir")
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
									break;
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

				//ParallaxLogic
				ScrollingbeforeX = fOffsetX;

				if (ScrollingbeforeX != ScrollingAfterX)
				{
					ScrollingCurrX -= (ScrollingbeforeX - ScrollingAfterX) * 12;
					scrollingbetweenX -= (ScrollingbeforeX - ScrollingAfterX) * 14;
					ScrollingBackX -= (ScrollingbeforeX - ScrollingAfterX) * 16;  // hEre just add new parametr and drawning method below for addin new layer 

				}

				ScrollingAfterX = ScrollingbeforeX;

				WrapCoordinates(ScrollingCurrX, ScrollingCurrX);

				float fx;

				

				WrapCoordinates(ScrollingCurrX, fx);


			SetDrawTarget(layer);
			//SetPixelMode(olc::Pixel::ALPHA);

				DrawDecal({ ScrollingCurrX ,0 }, m_pCurrentMap->FarParralax);

				DrawDecal({ fx - (float)ScreenWidth(),0 }, m_pCurrentMap->FarParralax);


				WrapCoordinates(scrollingbetweenX, scrollingbetweenX);

				WrapCoordinates(scrollingbetweenX, fx);



				DrawDecal({ scrollingbetweenX ,(ScrollingY+90) - (fOffsetY *3.0f) }, m_pCurrentMap->MidParralax);

				DrawDecal({ fx - (float)ScreenWidth() , (ScrollingY+90) - (fOffsetY *3.0f) }, m_pCurrentMap->MidParralax);



				WrapCoordinates(ScrollingBackX, ScrollingBackX);

				WrapCoordinates(ScrollingBackX, fx);



				DrawDecal({ ScrollingBackX ,(ScrollingY+180) - (fOffsetY*6.0f) }, m_pCurrentMap->CloseParralax);

				DrawDecal({ fx - (float)ScreenWidth() , (ScrollingY + 180) -(fOffsetY*6.0f) }, m_pCurrentMap->CloseParralax, { 0,0 });
				}

				/////////ParallaxLogic
				// 

				
				
				//SetDrawTarget(nullptr);
			
				if (m_pCurrentMap->sName == "Forest" || m_pCurrentMap->sName == "ForesttLvlPartTwo")
				{
					for (auto& source : { &m_vecFarWeather })    // DRAW Rain
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

							TileInfo tile;

		
						if (idx.ThirdLayer.index !=0)
							{
							if (bshowThird == true)
							SetDrawTarget(layer);

					
							DrawPartialDecal({x * CellSize - fTileOffsetX,y * CellSize - fTileOffsetY }, m_pCurrentMap->pSprite, idx.ThirdLayer.sourcerect, idx.ThirdLayer.size);

							if (bshowThird == true)
							{

								//SetPixelMode(olc::Pixel::NORMAL);
								EnableLayer(layer, true);
								SetDrawTarget(nullptr);


								DrawRect({ (int)x * (int)CellSize - (int)fTileOffsetX,(int)y * (int)CellSize - (int)fTileOffsetY }, idx.ThirdLayer.size);

							}
							}

							if (idx.SecondLayer.index != 0)
							{
								if (bshowSecond == true)
								SetDrawTarget(layer);
							

								DrawPartialDecal({ x * CellSize - fTileOffsetX,y * CellSize - fTileOffsetY }, m_pCurrentMap->pSprite, idx.SecondLayer.sourcerect, idx.SecondLayer.size);
							
								if (bshowSecond == true)
								{

									SetPixelMode(olc::Pixel::NORMAL);
									EnableLayer(layer, true);
									SetDrawTarget(nullptr);


									DrawRect({ (int)x * (int)CellSize - (int)fTileOffsetX,(int)y * (int)CellSize - (int)fTileOffsetY }, idx.SecondLayer.size);

								}


							}	

							if (idx.DynamicLayer.index != 0)
							{
								//SetDrawTarget(layer);


								idx.DynamicLayer.index -= 10;

								DrawPartialDecal({x * CellSize - fTileOffsetX,y * CellSize - fTileOffsetY }, m_pCurrentMap->pDynsprite, { 128.0f * frameIndex,idx.DynamicLayer.index * 128.0f }, { 128,128 }, { fscale,fscale });

							}

							if (idx.FirstLayer.index != 0)
							{
								if (bshowfirst == true)
								SetDrawTarget(layer);


								DrawPartialDecal({ x * CellSize - fTileOffsetX,y * CellSize - fTileOffsetY }, m_pCurrentMap->pSprite, idx.FirstLayer.sourcerect, idx.FirstLayer.size);
						


								if (bshowfirst == true)
								{

									SetPixelMode(olc::Pixel::NORMAL);
									EnableLayer(layer, true);
									SetDrawTarget(nullptr);


									DrawRect({ (int)x * (int)CellSize - (int)fTileOffsetX,(int)y * (int)CellSize - (int)fTileOffsetY }, idx.FirstLayer.size);

								}

							}



						


							if (idx.ZeroLayer.index != 0)
							{
								tile.position.x = x;
								tile.position.y = y;
								tile.size = idx.ZeroLayer.size;
								tile.sourceRect = idx.ZeroLayer.sourcerect;
								tile.textureIndex = 0;
								tile.textureLayer = 0;
								batchZeroCharsLayer.push_back(tile);

							}


							

						}
					}
					//std::cout << batchDynamicLayer.size() + batchSecondLayer.size() + batchThirdLayer.size() + batchFirstLayer.size()+batchZeroCharsLayer.size() << std::endl;
			
				

					//   spherese check 
				
					

				//	float targetX = point2X - targetpointX;
				//	float targetY = point2Y - targetpointY;

				//	float dist = std::sqrtf(targetX * targetX + targetY * targetY);

				////	if(CheckZeroDivide(targetX, targetpointVx))
				//	targetpointVx = (targetX / dist) * (speed * fElapsedTime);
				//	float checkcos = std::cos(dist);
				//	float checktarget = targetX / dist;

				////	if (CheckZeroDivide(targetY, targetpointVy))
				//	targetpointVy = (targetY / dist)*speed*fElapsedTime ;
				//	
				//	targetpointX = targetpointX + targetpointVx * fElapsedTime;
				//	targetpointY = targetpointY + targetpointVy * fElapsedTime;

				////	std::cout << targetpointX << '\t' << targetpointY << std::endl;
				//	
				//	
				//	DrawCircle(point1X, point1Y, 5);
				//	DrawCircle(point2X, point2Y, 5);
				//	DrawCircle(targetpointX, targetpointY, 5);


				//	DrawLine((m_pPlayer->px + m_pPlayer->CollbordersX - fOffsetX) * 64, (m_pPlayer->py + m_pPlayer->CollbordersYF - fOffsetY) * 64, (m_pPlayer->px + m_pPlayer->CollbordersX - fOffsetX + m_pPlayer->vx) * 64, (m_pPlayer->py + m_pPlayer->CollbordersYF - fOffsetY + m_pPlayer->vy) * 64);


				//	DrawRect((m_pPlayer->px - fOffsetX)*64, (m_pPlayer->py-fOffsetY)*64, 128, 128);



				
					


				

				

				if (m_pPlayer->bHideMode)
				{
				m_pPlayer->DrawSelf(this, fOffsetX, fOffsetY);
				}

				//Draw Objects
				

				for (auto& source : { &m_vecFightText,&m_vecVisibleDynamics,&m_vecProjectiles,&m_vecParticles })
					for (auto& dyns : *source)
						if (dyns->px >= fCameraPosX - nVisibleTileX && dyns->px <= fCameraPosX + nVisibleTileX && dyns->py >= fCameraPosY - nVisibleTileX && dyns->py <= fCameraPosY + nVisibleTileX)   // && m_vecDynamics[0] !=dyns
						{
							if (dyns->checkFlag(dyns->bDraw))
							dyns->DrawSelf(this, fOffsetX, fOffsetY);
							
						}
				

				if (!batchZeroCharsLayer.empty())
				{
					// Extract positions and source rectangles from the batch
					std::vector<olc::vi2d> positions;
					std::vector<olc::vi2d> sourceRects;
					std::vector<olc::vi2d> sourceSize;
					//std::vector<int> isLayer;

				

					for (const auto& tile : batchZeroCharsLayer) {
						positions.push_back(tile.position);
						sourceRects.push_back(tile.sourceRect);
						//isLayer.push_back(tile.textureIndex);
						sourceSize.push_back(tile.size);
					}

					if (bshowZero == true)
					{
						SetDrawTarget(layer);
						//SetPixelMode(olc::Pixel::ALPHA);
					}


					for (size_t i = 0; i < positions.size(); ++i) {
					

							
						
						DrawPartialDecal({ positions[i].x * CellSize - fTileOffsetX,positions[i].y * CellSize - fTileOffsetY }, m_pCurrentMap->pSprite,  sourceRects[i],  sourceSize[i]);
							

							if (bshowZero == true)
							{
							SetPixelMode(olc::Pixel::NORMAL);
							EnableLayer(layer, true);
							SetDrawTarget(nullptr);
							DrawRect({ positions[i].x * (int)CellSize - (int)fTileOffsetX,positions[i].y * (int)CellSize - (int)fTileOffsetY }, sourceSize[i]*fscale);
							}
							

						}
					batchZeroCharsLayer.clear();
				}



				
				/*}*/

			/*	if (!m_pPlayer->bHideMode)
				{
				m_pPlayer->DrawSelf(this, fOffsetX, fOffsetY);
				}*/
			

				
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
	
						
			


				for (auto& source : { &m_vecUiBars })    // DRAW Ui elements above char 
					for (auto& dyns : *source)
						dyns->DrawSelf(this, (m_pPlayer->px-fOffsetX)* (64 * fscale), (m_pPlayer->py - fOffsetY+1.5) * (64 * fscale));

			//	std::cout <<m_pPlayer->px << '\t'<< (ScrollingY + 90) - (fOffsetY * 6.0f) <<'\t'<< m_pPlayer->py << std::endl;


			
					
				/*defineFacingDirection(fTestX, fTestY);
				
				DrawCircle((fTestX-fOffsetX) * 64, (fTestY-fOffsetY) * 64, 0.5*64);
				*/
			


			
			//std::cout << m_pPlayer->py<< '\t'<< fOffsetY << '\t' << fCameraPosY <<'\t'<< std::endl;
			
					//if (fTestX + 0.5f > m_pPlayer->px + dyns->CollbordersX && fTestX <(dyns->px + dyns->CollbordersXF) && fTestY > dyns->py + dyns->CollbordersY && fTestY < dyns->py + dyns->CollbordersYF)
					

		

					//		DrawLine((m_pPlayer->px+m_pPlayer->CollbordersXF - fOffsetX) * 64 , (m_pPlayer->py + m_pPlayer->CollbordersYF - fOffsetY) * 64, ((m_pPlayer->px + m_pPlayer->CollbordersXF - fOffsetX) * 64 )+ (m_pPlayer->vx )*64, ((m_pPlayer->py + m_pPlayer->CollbordersYF - fOffsetY) * 64)+ (m_pPlayer->vy)*64);


				
				if (m_pCurrentMap->sName == "Forest")
				{

				for (auto& source : { &m_vecCloseWeather})    // DRAW Rain
					for (auto& dyns : *source)
					{
					
				
					dyns->Update(fElapsedTime,fOffsetX,fOffsetY); // Update raindrop position

					dyns->DrawSelf(this, fOffsetX, fOffsetY);
				

					}

				}
			

				if (PressF)
				DrawPartialDecal({ (m_pPlayer->px + (1.0f*fscale) - fOffsetX) * CellSize,((m_pPlayer->py-0.2f) - fOffsetY) * CellSize }, D_Ui, { 976,384 }, { 112,30 },{fscale,fscale});


				//

				uiCellUpdate(mouse);

			

				//

				Drawcursor(mouse);



		
				if (bPause)
			{
					olc::vf2d quitPos{ (ScreenWidth() / 2) - (167 * fscale) ,ScreenHeight() / 2 + (31 * fscale) };
			
					switch (n_nGameMode)
					{
					case MODE_TITLE:

						break;
					case MODE_LOCAL_MAP:
						DrawPartialDecal({ ((float)ScreenWidth() / 2) - (167*fscale), (float)ScreenHeight() / 2 - (71*fscale) }, D_Ui, { 832,0 }, { 334,142 },{fscale,fscale});
						//DrawBigText("Travel ?", (ScreenWidth() / 2) - 72, ScreenHeight() / 2 - 62, 1, 1, olc::WHITE);

						DrawBigText("Quit", (ScreenWidth() / 2) - (167*fscale), ScreenHeight() / 2 + (31*fscale), 1*fscale, 1*fscale, olc::WHITE);

						DrawBigText("Options", (ScreenWidth() / 2) + ((167 - 144)*fscale), ScreenHeight() / 2 + (31*fscale), 1*fscale, 1*fscale, olc::WHITE);

						SetPixelMode(olc::Pixel::NORMAL);
						EnableLayer(layer, true);
						SetDrawTarget(nullptr);

					
						DrawRect((ScreenWidth() / 2) - (167 * fscale), ScreenHeight() / 2 + (31 * fscale), 40,20);


						if (GetMouse(0).bPressed) //LeftMouse
						{

							if ((int)mouse.x >=(int)quitPos.x && (int)mouse.y >= (int)quitPos.y && (int)mouse.x <= (int)quitPos.x+40 && (int)mouse.y <= (int)quitPos.x + 40)
							{
								return false;
							}

						}
						Drawcursor(mouse);
						DrawBigText("Pause", ScreenWidth() / 2 - (45*fscale), ScreenHeight() / 2 - (71 * fscale) , 1 * fscale, 1 * fscale, olc::WHITE);

					
						break;
					case MODE_WAREHOUSE:
						UpdateWarehouse(fElapsedTime);
						break;
					case MODE_INVENTORY:
						UpdateInventory(fElapsedTime);
						break;
					case MODE_SHOP:
						UpdateShop(fElapsedTime);
						break;
					case MODE_PROFESSION:
						UpdateProfession(fElapsedTime);
						break;
					case MODE_MAP:
						UpdateMap(fElapsedTime);
						break;
					case MODE_BLACKSMITH:
						UpdateBlackSmith(fElapsedTime);
						break;
					}
				

			}





				uint8_t y = 0;
				for (auto& quest : m_listQusets)  // ui drawing quest right up corner on the screen
				{
				
					DrawBigText(quest->sName, ScreenWidth() - 250, y * 32, 0.35, 0.35, olc::YELLOW);
					y++;
					DrawBigText(quest->sDescription, ScreenWidth() - 250, y * 32, 0.25, 0.25);
					y++;
				}


				if (bSmoothAppearScreen)
				{
					
				DrawPartialDecal({ (float)0, (float)0 }, RPG_Assets::get().GetSprite("DescriptionPattern"), { 0,0 }, {(float)RPG_Assets::get().GetSprite("DescriptionPattern")->sprite->width, (float)RPG_Assets::get().GetSprite("DescriptionPattern")->sprite->height}, { 2,2 }, olc::Pixel{255,255,255,(uint8_t)AlphaAppearScreen});
				}
							//Draw any dialog being displayed
				if (m_bShowDialog)
					DisplayDialog(m_vecDialogToShow, 100, ScreenHeight() - 150);

		

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
				//
				SetPixelMode(olc::Pixel::NORMAL);
				EnableLayer(layer, true);
				SetDrawTarget(nullptr);

				// Limit the frame rate to 60 frames per second
			//	Sleep(4); // Sleep for the appropriate time to achieve desired frame rate

				return true;
			
	
}

bool RPG_Engine::UpdateWarehouse(float fElapsedTime)
{


	




	olc::vf2d mouse = { (float)GetMouseX(), (float)GetMouseY() };
	olc::vf2d mousefix = { (mouse.x / CellSize), (mouse.y / CellSize) };

	


	 highlighted = nullptr;
	 GrabItem = nullptr;

	float squeY = 8 * fscale;
	float squeX = 4 * fscale;
	const float WHsqueX = 14 * fscale;
	
	const float ReqSocketsY = (float)(128 / 64) * fscale;
	const float RqSocketsX = (float)(192 / 64) * fscale;
	


	olc::vf2d inventorySockets = { 0,250 };

	DrawPartialDecal({ (float)4 * 64 * fscale, (float)(4) * 64 * fscale }, Inventoryback,{0,0}, {512,523}, {fscale,fscale});  // draw sockets and inventary name


	
	DrawPartialDecal({ (float)14 * 64 * fscale, (float)(8) * 64 * fscale }, Inventoryback, inventorySockets, { 512,199 }, { fscale,fscale });  // draw sockets 
	DrawPartialDecal({ (float)15.85 * 64 * fscale, (float)11 * 64 * fscale }, Inventoryback, { 725,75 }, { 273,76 }, { fscale,fscale });  // draw sockets 
//	DrawPartialDecal({ (float)14 * 64*fscale, (float)8 * 64*fscale }, InventorySockeys, { 0,0 }, { invSocketsWidth,invSocketsHeigh }, { fscale,fscale });
//	DrawPartialDecal({ (float)15.85 * 64*fscale, (float)11 * 64*fscale }, RPG_Assets::get().GetSprite("WarehouseName"), { 0,0 }, { 274,75 }, { fscale,fscale });
	
	DrawBigText(std::to_string(m_pPlayer->GetStats(2)), (9 ) * 64 * fscale, (5 + 0.3) * 64 * fscale, fscale, fscale); //Strength
	DrawBigText(std::to_string(m_pPlayer->GetStats(1)), (9 ) * 64 * fscale, (6 + 0.3) * 64 * fscale, fscale, fscale); //Agility
	DrawBigText(std::to_string(m_pPlayer->GetStats(3)), (9 ) * 64 * fscale, (7 + 0.2) * 64 * fscale, fscale, fscale); //Intelegence
	DrawBigText(std::to_string(m_pPlayer->GetStats(4)), (5 ) * 64 * fscale, 7 * 64 * fscale, 0.5, 0.5); //Defence
	//DrawBigText(std::to_string(m_pPlayer->GetStats(5)), (4) * 64, (6) * 64, 0.5, 0.5); //MovementSpeed
	DrawBigText(std::to_string(m_pPlayer->GetStats(6)), (5) * 64 * fscale, (6) * 64 * fscale, 0.5, 0.5); //AverageAttack
	//DrawBigText(std::to_string(m_pPlayer->GetStats(7)), (4) * 64, (8) * 64, 0.5, 0.5); //fHpRegeneration
	//DrawBigText(std::to_string(m_pPlayer->GetStats(8)), (4) * 64, (9) * 64, 0.5, 0.5); //Versality


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

				DrawPartialDecal({ (float)(EqSqueX + (x * fscale)) * 64, (EqSqueY + (y * fscale)) * 64 }, item->Item->pSprite, { sprcoordX * 64,sprcoordY * 64 }, { 64,64 }, { fscale,fscale });

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

					moveteItems(mousefix, squeX, squeY, GrabItem, m_listItems);
				}











				}
			

			if ((int)mousefix.x >= WHsqueX/fscale && (int)mousefix.y >= squeY/fscale && (int)mousefix.x <= 21 && (int)mousefix.y <= 10)   //Warehouse
			{


				if (!GrabItem->checkFlag(GrabItem->binWarehouse))
				{
					
					//GrabItem->bInWarehouse = true;
					GrabItem->setFlag(GrabItem->binWarehouse);
						
					int x = (int)mousefix.x  - (WHsqueX/fscale) ;    //calculate drop position for draged object
					int y = (int)mousefix.y - (squeY / fscale);

					int currIndex = 8 *y + x; // cell in inventary that need to check on reserved  
					int GrabedIndex = GrabItem->index;
					
						
						m_listWarehouseItems[currIndex]->index= GrabItem->index; //switch indexes
						GrabItem->index = currIndex;  // switch indexes
						//m_listWarehouseItems[currIndex]->bInWarehouse = false;
						m_listWarehouseItems[currIndex]->clearFlag(m_listWarehouseItems[currIndex]->binWarehouse);

						std::swap(m_listWarehouseItems[currIndex], m_listItems[GrabedIndex]);

					

				}
				else
				{
					moveteItems(mousefix, WHsqueX, squeY, GrabItem, m_listWarehouseItems);

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

						if (highlighted->Item->OnUse(m_pPlayer, m_listWarehouseItems))
						{
							// Item has signalled it must be consumed, so remove it
							ClearAbsorbedSlots(m_listWarehouseItems); // find absorbed object and change them on empty sockets
							//TakeItem(highlighted, m_listWarehouseItems);

							

						}
					}
					else if (highlighted->Item->OnUse(m_pPlayer, m_listItems))
					{
						ClearAbsorbedSlots(m_listItems); // find absorbed object and change them on empty sockets
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
	squeX = (4 * fscale) * 64;
	squeY = (3 * fscale) * 64;
	DrawBigText(std::to_string(Money), squeX + (64 * fscale), squeY + (16 * fscale), fscale, fscale, olc::YELLOW); //Money
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

						DrawPartialDecal({ (float)(offestX + (x * fscale)) * 64, (offsetY + (y * fscale)) * 64 }, item->Item->pSprite, { sprcoordX*64,sprcoordY*64 }, { 64,64 }, { fscale,fscale });

						//item->ipy = y;




						if (item->Item->MaxStack > 1)
							DrawBigText(std::to_string(item->currStacks), (float)(offestX + (x * fscale) + 0.05) * 64, (offsetY + (y * fscale)) * 64, fscale * 0.5f, fscale * 0.5f, olc::WHITE); //Money
						if (item->Gold > 0)
							DrawBigText(std::to_string(item->Gold), (float)(offestX + (x * fscale) + 0.4) * 64, (offsetY + (y * fscale) + 0.3) * 64, fscale * 0.5f, fscale * 0.5f, olc::YELLOW); //Money



						if ((int)mouseFixed.x == ((offestX / fscale) + x) && (int)mouseFixed.y == (offsetY / fscale) + y)
							highlighted = item;

					}
					else
					{
						DrawPartialDecal({ (float)(mouse.x - (32 / 2)) , (float)(mouse.y - (32 / 2)) }, item->Item->pSprite, { sprcoordX * 64,sprcoordY * 64 }, { 64,64 }, { fscale,fscale });
						Grabitem = item;

					}
				}
			}
		}
		i++;
	}
}

bool RPG_Engine::SetMouseTarget(olc::vi2d mouse)
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






void RPG_Engine::uiCellUpdate(olc::vi2d mouse)
{

	 highlighted = nullptr;
	// GrabItem = nullptr;
	
	
	olc::vi2d mousefix = { (mouse.x / (int)(64 * fscale)),(mouse.y / (int)(64 * fscale)) };
	uint8_t SelObjSize =64;

	uint8_t spriteCoordX = 4 % 32;
	uint8_t spriteCoordY = 4/32;



	//uint8_t UiFirstCellX = 64 * fscale / (64 * fscale);
	uint8_t UiFirstCellY = (ScreenHeight() - (ScreenHeight() / 6)) / (64 * fscale);

//	uint8_t UiSecondCellX = 64 * 3 * fscale / (64 * fscale);
	//uint8_t UiSecondCellY = (ScreenHeight() - (ScreenHeight() / 6)) / (64 * fscale);

	DrawPartialDecal({ (float)SelObjSize * fscale, (float)ScreenHeight() - (ScreenHeight() / 6) }, m_Items, { (float)spriteCoordX*64,(float)spriteCoordY*64 }, { (float)SelObjSize, (float)SelObjSize }, { fscale*fscale,fscale*fscale });
	DrawBigText("Q", (float)SelObjSize * fscale + (32 / 2 * (fscale - 0.2f)), (float)ScreenHeight() - (ScreenHeight() / 9), (fscale + 0.2f)*fscale, (fscale + 0.2f)*fscale, olc::WHITE);


	DrawPartialDecal({ (float)SelObjSize * 3 * fscale, (float)ScreenHeight() - (ScreenHeight() / 6) }, m_Items, { (float)spriteCoordX * 64,(float)spriteCoordY * 64 }, { (float)SelObjSize, (float)SelObjSize }, { fscale*fscale,fscale*fscale });
	DrawBigText("E", (float)SelObjSize * 3 * fscale + (32 / 2 * (fscale - 0.2f)), (float)ScreenHeight() - (ScreenHeight() / 9), (fscale + 0.2f)*fscale, (fscale + 0.2f)*fscale, olc::WHITE);

	//std::cout << mousefix.x << '\t' << mousefix.y << std::endl;
	uint8_t xcoor = 1;
	for (auto i : m_vecUi)
	{

		//i->spriteindex
		


		if (i != nullptr)
		{
			float x = i->Item->spriteindex % 32;
			float y = i->Item->spriteindex / 32;

			if (!i->checkFlag(i->Grabitem))

			{

			DrawPartialDecal({ (float)SelObjSize * xcoor * fscale, (float)ScreenHeight() - (ScreenHeight() / 6) }, i->Item->pSprite, { x*64,y*64 }, { (float)SelObjSize, (float)SelObjSize }, { fscale,fscale });

		//	std::cout << SelObjSize * xcoor * fscale / (64*fscale) << '\t' << mousefix.y << std::endl;

			if ((int)mousefix.x == SelObjSize * xcoor * fscale / (64 * fscale) && mousefix.y ==UiFirstCellY)
			{
				highlighted = i;
			}

			}
			else
			{

				DrawPartialDecal({ (float)(mouse.x - (32 / 2)) , (float)(mouse.y - (32 / 2)) }, i->Item->pSprite, { x*64,y*64 }, { 64,64 }, { fscale,fscale });
				GrabItem = i;
			}
			

			


		}
		xcoor += 2;
	}
	
	if (highlighted !=nullptr)
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
	//	if (highlighted->bEquipable)
	//	{
	//		nLinesY+=3;
	//	}
	//	else
	//	{
		//	if (nLinesX < 32 + 20 * 18 && !highlighted->bKeyItem)  // fill @press space to use like default value
		//	{
		//		nLinesX = 32 + 20 * 18;

	//	}
	//	}
		
	olc::vi2d Discription(ScreenWidth()-(32+nLinesX*fscale), ScreenHeight() - (ScreenHeight() / 6)-(nLinesY*32*fscale)*fscale);
	DrawDescriptionPattern(highlighted, Discription, mousefix);

	}
}

bool RPG_Engine::UpdateInventory(float fElapsedTIme)
{
	

	highlighted = nullptr;
	GrabItem = nullptr;

	

	const int inventoryHeight = 523;
	const int inventoryWidth = 512;
	float Scale =fscale;
	olc::vi2d mouse = {(GetMouseX()), (GetMouseY()) };


	olc::vi2d mousefix = { (mouse.x / (int)(64 * fscale)),(mouse.y / (int)(64 * fscale)) };
	
	

	float squeX = (nVisibleTileX / 3) * fscale;
	float squeY = (nVisibleTileX / 3) * fscale;

	
	 float EqSqueX = (squeX + (6 * fscale)) ;
	 float EqSqueY = (squeY + (1 * fscale)) ;
	
	


	
	DrawPartialDecal({ (float)squeX * 64, (float)(squeY * 64)  },Inventoryback, { 0,0 }, { (float)inventoryWidth,(float)inventoryHeight}, { Scale,  Scale});
	


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

				 DrawPartialDecal({ (float)(EqSqueX + (x * fscale)) * 64, (EqSqueY + (y * fscale)) * 64 }, item->Item->pSprite, { sprcoordX * 64,sprcoordY * 64 }, { 64,64 }, { fscale,fscale });

				 //item->ipy = y;



				 if ((int)mousefix.x == ((EqSqueX / fscale) + x) && (int)mousefix.y == (EqSqueY / fscale) + y)
					 highlighted = item;

			 }
			 else
			 {
				 DrawPartialDecal({ (float)(mouse.x - (32 / 2)) , (float)(mouse.y - (32 / 2)) }, item->Item->pSprite, { sprcoordX * 64,sprcoordY * 64 }, { 64,64 }, { fscale,fscale });
				 GrabItem = item;

			 }

		 }
		 i++;
	 }





	DrawBigText(std::to_string(m_pPlayer->GetStats(1)), (squeX + Scale + ((inventoryWidth / 64) / 2) * Scale) * 64, (squeY - 0.9 + ((inventoryHeight / 64) / 2) * Scale) * 64, Scale, Scale, olc::YELLOW); //Agility
	DrawBigText(std::to_string(m_pPlayer->GetStats(2)), (squeX + Scale + ((inventoryWidth / 64) / 2) * Scale) * 64, (squeY - 1.4 + ((inventoryHeight / 64) / 2) * Scale) * 64, Scale, Scale, olc::RED); //Strength
	DrawBigText(std::to_string(m_pPlayer->GetStats(3)), (squeX + Scale + ((inventoryWidth / 64) / 2) * Scale) * 64, (squeY - 0.4 + ((inventoryHeight / 64) / 2) * Scale) * 64, Scale, Scale,olc::BLUE); //Intelegence

	//DrawBigText(std::to_string(m_pPlayer->GetStats(5)), (4) * 64, (6) * 64, 0.5, 0.5); //MovementSpeed

	DrawBigText(std::to_string(m_pPlayer->GetStats(6)), (squeX - 1.3 + ((inventoryWidth / 64) / 2) * Scale) * 64, (squeY - 1.1 + ((inventoryHeight / 64) / 2) * Scale) * 64, Scale-0.2, Scale - 0.2); //AverageAttack
	DrawBigText(std::to_string(m_pPlayer->GetStats(4)), (squeX - 1.3 + ((inventoryWidth / 64) / 2) * Scale) * 64, (squeY - 0.5 + ((inventoryHeight / 64) / 2) * Scale) * 64, Scale-0.2, Scale-0.2); //Defence

	//DrawBigText(std::to_string(m_pPlayer->GetStats(7)), (4) * 64, (8) * 64, 0.5, 0.5); //fHpRegeneration
	//DrawBigText(std::to_string(m_pPlayer->GetStats(8)), (4) * 64, (9) * 64, 0.5, 0.5); //Versality
	
	
	
	// std::cout << offestX << '\t' << offsetY << std::endl;
	

	
	//if ((int)mousefix.x == 10*fscale && (int)mousefix.y == 6*fscale)
	//	highlighted = m_pPlayer->pEquipedWeapon;


	const float YcellOffset = (squeY + (inventoryHeight / 64 - 4) * Scale); // <-- offset from higher point of inventory picture to cells where they drawing

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
			

			moveteItems(mousefix, squeX, YcellOffset, EqSqueX, EqSqueY, GrabItem,m_listItems, m_vecEquip);

		}
	}
	
	// MONEY
	
	float coinsprX = 3 % 32;
	float coinsprY = 3 / 32;

	DrawBigText(std::to_string(Money), (float)(squeX + ((64 * Scale) / 64)) * 64, (float)(squeY + 0.1-((64 * Scale) / 64)) * 64, Scale, Scale, olc::YELLOW); //Money
	DrawPartialDecal({ (float)squeX*64, (float)(squeY-((64*Scale)/64))*64 }, m_Items, { coinsprX*64,coinsprY }, { (float)64,(float)64 }, { Scale ,Scale });
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

			DrawPartialDecal({ (float)(offestX + (x  * fscale)) * 64, (offsetY  + (y  * fscale)) * 64 }, item->Item->pSprite, { sprcoordX*64,sprcoordY*64 }, { 64,64 }, { fscale,fscale });
			
			//item->ipy = y;
			

			if (item->Item->MaxStack>1)
			DrawBigText( std::to_string(item->currStacks), (float)(offestX + (x * fscale)+0.05) * 64, (offsetY + (y * fscale)) * 64, fscale*0.5f, fscale*0.5f,olc::WHITE); //Money
			if (item->Gold > 0)
			DrawBigText(std::to_string(item->Gold), (float)(offestX + (x * fscale)+0.4) * 64, (offsetY + (y * fscale)+0.3) * 64, fscale*0.5f, fscale*0.5f, olc::YELLOW); //Money

			if (mouseFixed.x == ((offestX / fscale) + x ) && mouseFixed.y == (offsetY / fscale) + y )
				highlighted = item;
			
		}
		else
		{
			DrawPartialDecal({ (float)(mouse.x - (32 / 2)) , (float)(mouse.y - (32 / 2)) }, item->Item->pSprite, { sprcoordX * 64,sprcoordY * 64 }, { 64,64 }, { fscale,fscale });
			Grabitem = item;

		}

		}
		i++;
		}

		}
	

}

void RPG_Engine::moveteItems(olc::vi2d mouse, float x, float y, InventaryItem*& GrabItem,std::vector<InventaryItem*>& vector)
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

void RPG_Engine::moveteItems(olc::vi2d mouse, float x, float y,float eqX,float eqY, InventaryItem*& grabitem, std::vector<InventaryItem*>& vector, std::vector<InventaryItem*>& EqVector)
{
	const int cellsInvCoordX = mouse.x - (x / fscale);     // Coordinates x and y where first cell in inv begins 0,0; 0,1; 0,2
	const int cellsInvCoordY = mouse.y - (y / fscale);


	const int cellsEqCoordX = mouse.x - (eqX / fscale);     // Coordinates x and y where first cell in eq begins 0,0; 0,1; 0,2
	const int cellsEqCoordY = mouse.y - (eqY / fscale);



	uint8_t UiFirstCellX = 64 * fscale / (64 * fscale);
	uint8_t UiFirstCellY = (ScreenHeight() - (ScreenHeight() / 6)) / (64 * fscale);

	uint8_t UiSecondCellX = 64 * 3 * fscale / (64 * fscale);
	uint8_t UiSecondCellY = (ScreenHeight() - (ScreenHeight() / 6)) / (64 * fscale);
	
	//std::cout << "Invenotry -" << '\t' << cellsInvCoordX << '\t' << cellsInvCoordY << std::endl;
	//std::cout << "Equip -" << '\t' << cellsEqCoordX << '\t' << cellsEqCoordY << std::endl;
	std::cout << "Equip -" << '\t' << (int)mouse.x << '\t' << (int)mouse.y << std::endl;

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

				/*grabitem->bEquiped = false;
				vector[currIndex]->Item->equipIndex = grabitem->Item->equipIndex;
				vector[currIndex]->bEquiped = true;
				grabitem->index = currIndex;
				vector[currIndex]->index = GrabbedIndex;
				std::swap(vector[currIndex], EqVector[GrabbedIndex]);
				AttachEq(EqVector, GrabbedIndex);*/
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

	if ((int)mouse.x >= UiFirstCellX && (int)mouse.y >= UiFirstCellY && (int)mouse.x <= UiFirstCellX && (int)mouse.y <= UiFirstCellY)
	{
		//cItem* helper = grabitem->Item;

		m_vecUi[0] = grabitem;
		m_vecUi[0]->Uiindex = 0;

	}
	if ((int)mouse.x >= UiSecondCellX && (int)mouse.y >= UiSecondCellY && (int)mouse.x <= UiSecondCellX && (int)mouse.y <= UiSecondCellY)
	{
		//cItem* helper = grabitem->Item;

		m_vecUi[1] = grabitem;
		m_vecUi[1]->Uiindex = 1;

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

bool RPG_Engine::UpdateShop(float fElapsedTime)                                                                                                       // < ----SHOP
{
	ClearAbsorbedSlots(m_listItems);



	olc::vi2d mouse = { (GetMouseX()), (GetMouseY()) };


	olc::vi2d mousefix = { (mouse.x / (int)(64 * fscale)),(mouse.y / (int)(64 * fscale)) };



	olc::vf2d inventorysockets = { 0,250 };  // coordinates whith inv name
	
	

	DrawPartialDecal({ (float)4 * 64*fscale, (float)(8) * 64*fscale }, Inventoryback, inventorysockets, {512.0f,267.0f}, {fscale,fscale});

	


	//DrawPartialDecal({ (float)5.85 * 64*fscale, (float)(11) * 64*fscale }, RPG_Assets::get().GetSprite("inventoryName"), { 0,0 }, { 274,75 },{fscale,fscale});


	DrawPartialDecal({ (float)14 * 64 * fscale, (float)(8) * 64 * fscale }, Inventoryback, inventorysockets, { 512,192}, { fscale,fscale });  // sockets 
	DrawPartialDecal({ (float)15.85 * 64*fscale, (float)(11) * 64*fscale }, Inventoryback, { 724,0 }, { 275,75 },{fscale,fscale}); //storeName




	
	
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

			moveteItems(mousefix, squeX, squeY, GrabItem, m_listItems);
			
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
	squeX = 4 * 64*fscale;
	squeY = 7* 64*fscale;
	DrawBigText(std::to_string(Money), squeX + (64*fscale), squeY + (16*fscale), fscale, fscale, olc::YELLOW); //Money
	
	DrawPartialDecal({ (float)squeX, (float)squeY }, m_Items, {3.0f*64.0f,0}, {(float)64,(float)64}, {fscale,fscale});  //Gold
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

				DrawPartialDecal({ (float)(sX + x * fscale) * 64, (sY + y * fscale) * 64 }, item->Item->pSprite, { sprcoordX*64, sprcoordY*64 }, { 64,64 }, { fscale,fscale });
				//	item->invnumber = i;
					//item->ipy = y;



				if ((int)mousefix.x == ((sX / fscale) + x) && (int)mousefix.y == ((sY / fscale) + y))
					highlighted = item;
			}
		}
	}
}


bool RPG_Engine::UpdateProfession(float fElapsedTime)   //<--profession
{

	olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };

	const float TalentPlateWidth = 960;
	const float TalentPlateHeigh = 768;

//	olc::Decal* HrzStickEmptry = RPG_Assets::get().GetSprite("Horizontal StickEmpty");
//	olc::Decal* HrzStickFull = RPG_Assets::get().GetSprite("Horizontal StickFill");


//	olc::Decal* VertStickEmptry = RPG_Assets::get().GetSprite("Vertical StickEmpty");
//	olc::Decal* VertStickFull = RPG_Assets::get().GetSprite("Vertical StickFill");


//	olc::Decal* SelectedObject = RPG_Assets::get().GetSprite("SelectedObject");

	float BaseposX = (float)(ScreenWidth() / 2 - ((TalentPlateWidth / 2)*fscale));
	float BasePosY = (float)(ScreenHeight() / 2 - ((TalentPlateHeigh / 2)*fscale));

	


	DrawPartialDecal({ BaseposX,  BasePosY }, D_Ui, { (float)0,(float)142 }, { TalentPlateWidth,TalentPlateHeigh }, { fscale,fscale });
	//DrawPartialDecal({ BaseposX,  BasePosY }, RPG_Assets::get().GetSprite("Talent Plate"), {0,0}, { TalentPlateWidth,TalentPlateHeigh },{fscale,fscale});




	/////////  STICK DRAWING /////  START

	//Low-Mid Attack
	DrawPartialDecal({ BaseposX + (222*fscale),  BasePosY + (576*fscale) }, D_Ui, { 1227,0 }, { (float)4,(float)74 }, { fscale,fscale });

	if(GetLearnedTalent(2)) //  <-- MidAttack 
	{
	DrawPartialDecal({ BaseposX + (222*fscale),  BasePosY + (576*fscale) }, D_Ui, { 1223,0 }, { (float)4,(float)74 }, { fscale,fscale });

	//Mid-High Attack
	DrawPartialDecal({ BaseposX + (126*fscale),  BasePosY + (542*fscale) }, D_Ui, { 0,910 }, { (float)74,(float)4 }, { fscale,fscale });
	}
	if(GetLearnedTalent(3))
	DrawPartialDecal({ BaseposX + (126*fscale),  BasePosY + (542*fscale) }, D_Ui, { 0,914 }, { (float)74,(float)4 }, { fscale,fscale });


	//BackStab-ShadowStab Attack
	if (GetLearnedTalent(4))
	{

		DrawPartialDecal({ BaseposX + (31*fscale),  BasePosY + (670*fscale) }, D_Ui, { 0,910 }, { (float)74,(float)4 }, { fscale,fscale });
		DrawPartialDecal({ BaseposX + (31*fscale),  BasePosY + (414*fscale) }, D_Ui, { 0,910 }, { (float)74,(float)4 }, { fscale,fscale });
		DrawPartialDecal({ BaseposX + (30*fscale),  BasePosY + (415*fscale) }, D_Ui, { 1227,0 }, { (float)4,(float)257 }, { fscale,fscale });

	}
	if (GetLearnedTalent(5))
	{

	DrawPartialDecal({ BaseposX + (31*fscale),  BasePosY + (670*fscale) }, D_Ui, { 0,914 }, { (float)74,(float)4 }, { fscale,fscale });
	DrawPartialDecal({ BaseposX + (31*fscale),  BasePosY +(414*fscale) }, D_Ui, { 0,914 }, { (float)74,(float)4 }, { fscale,fscale });
	DrawPartialDecal({ BaseposX + (30*fscale),  BasePosY + (415*fscale) }, D_Ui, { 1223,0 }, { (float)4,(float)257 }, { fscale,fscale });
	DrawPartialDecal({ BaseposX + (95*fscale),  BasePosY + (251*fscale) }, D_Ui, { 1227,0 }, { (float)4,(float)138 }, { fscale,fscale });
	}

	if (GetLearnedTalent(6))
	{
		DrawPartialDecal({ BaseposX + (95*fscale),  BasePosY + (251*fscale) }, D_Ui, { 1223,0 }, { (float)4,(float)138 }, { fscale,fscale });
	}


	//RighAttack-RightAirAttack
	DrawPartialDecal({ BaseposX + (351*fscale),  BasePosY + (572*fscale) }, D_Ui, { 1227,0 }, { (float)4,(float)74 }, { fscale,fscale });
	if (GetLearnedTalent(12))
	{

	DrawPartialDecal({ BaseposX + (351*fscale),  BasePosY + (572*fscale) }, D_Ui, { 1223,0 }, { (float)4,(float)74 }, { fscale,fscale }, olc::Pixel(255, 0, 0));   //offset pulling player back into the screen
	
	DrawPartialDecal({ BaseposX + (351*fscale),  BasePosY +(315*fscale) }, D_Ui, { 1227,0 }, { (float)4,(float)202 }, { fscale,fscale }, olc::Pixel(255, 0, 0));   //offset pulling player back into the screen


	}

	//Flight//Landing
	if (GetLearnedTalent(13))
	{
	DrawPartialDecal({ BaseposX + (351*fscale),  BasePosY + (315*fscale) }, D_Ui, { 1223,0 }, { (float)4,(float)202 }, { fscale,fscale }, olc::Pixel(255, 0, 0));   //offset pulling player back into the screen

	DrawPartialDecal({ BaseposX + (351*fscale),  BasePosY + (123*fscale) }, D_Ui, { 1227,0 }, { (float)4,(float)138 }, { fscale,fscale }, olc::Pixel(255, 0, 0));   //offset pulling player back into the screen
	}
	if (GetLearnedTalent(14))
	DrawPartialDecal({ BaseposX + (351*fscale),  BasePosY + (123*fscale) }, D_Ui, { 1223,0 }, { (float)4,(float)138 }, { fscale,fscale }, olc::Pixel(255, 0, 0));   //offset pulling player back into the screen
	


	/////////  STICK DRAWING /////  END
	


	for (auto& source : { &m_vecTalentsBars })    // DRAW Ui elements
		for (auto& dyns : *source)
			dyns->DrawSelfTalent(this, BaseposX , BasePosY );



	




				/// SELECT RECTANGLE START
	if (mouse.x >= BaseposX + (192*fscale) && mouse.x <= BaseposX + ((192 + 64)*fscale) && mouse.y >= BasePosY + (512*fscale) && mouse.y <= BasePosY + ((512 + 64)*fscale))    // <-- Mid
	{
		DrawPartialDecal({ BaseposX + (192*fscale), BasePosY + (512*fscale) }, m_Items, { 256,0 }, { (float)64,(float)(64) }, { fscale,fscale });
	}

	if (mouse.x >= BaseposX + (64*fscale) && mouse.x <= BaseposX + ((64 + 64)*fscale) && mouse.y >= BasePosY + (512*fscale) && mouse.y <= BasePosY + ((512 + 64)*fscale))    // <-- High
	{
		DrawPartialDecal({ BaseposX + (64*fscale), BasePosY + (512*fscale) }, m_Items, { 256,0 }, { (float)64,(float)(64) }, { fscale,fscale });
	}

	if (mouse.x >= BaseposX + (64*fscale) && mouse.x <= BaseposX + ((64 + 64)*fscale) && mouse.y >= BasePosY + (640*fscale) && mouse.y <= BasePosY + ((640 + 64)*fscale))    // <-- Backstab
	{
		DrawPartialDecal({ BaseposX + (64*fscale), BasePosY + (640*fscale) }, m_Items, { 256,0 }, { (float)64,(float)(64) }, { fscale,fscale });
	}

	if (mouse.x >= BaseposX + (64*fscale) && mouse.x <= BaseposX + ((64 + 64)*fscale) && mouse.y >= BasePosY + (384*fscale) && mouse.y <= BasePosY + ((384 + 64)*fscale))    // <-- ShadowStap
	{
		DrawPartialDecal({ BaseposX + (64*fscale), BasePosY + (384*fscale) }, m_Items, { 256,0 }, { (float)64,(float)(64) }, { fscale,fscale });
	}

	if (mouse.x >= BaseposX + (64*fscale) && mouse.x <= BaseposX + ((64 + 64)*fscale) && mouse.y >= BasePosY + (192*fscale) && mouse.y <= BasePosY + ((192 + 64)*fscale))    // <-- Swirl
	{
		DrawPartialDecal({ BaseposX + (64*fscale), BasePosY + (192*fscale) }, m_Items, { 256,0 }, { (float)64,(float)(64) }, { fscale,fscale });
	}

	if (mouse.x >= BaseposX + (192*fscale) && mouse.x <= BaseposX + ((192 + 64)*fscale) && mouse.y >= BasePosY + ((384)*fscale) && mouse.y <= BasePosY + ((384 + 64)*fscale))    // <-- Vanish
	{
		DrawPartialDecal({ BaseposX + (192*fscale), BasePosY + (384*fscale) }, m_Items, { 256,0 }, { (float)64,(float)(64) }, { fscale,fscale });
	}


	if (mouse.x >= BaseposX + (320*fscale) && mouse.x <= BaseposX + ((320 + 64)*fscale) && mouse.y >= BasePosY + (512*fscale) && mouse.y <= BasePosY + ((512 + 64)*fscale))    // <-- RightAir
	{
		DrawPartialDecal({ BaseposX + (320*fscale),  BasePosY + (512*fscale) }, m_Items, { 256,0 }, { (float)64,(float)(64) }, { fscale,fscale });
	}


	if (mouse.x >= BaseposX + (320*fscale) && mouse.x <= BaseposX + ((320 + 64)*fscale) && mouse.y >= BasePosY + (256*fscale) && mouse.y <= BasePosY + ((256 + 64)*fscale))    // <-- RightFLIGHTUP
	{
		DrawPartialDecal({ BaseposX + (320*fscale), BasePosY + (256*fscale) }, m_Items, { 256,0 }, { (float)64,(float)(64) }, { fscale,fscale });
	}

	if (mouse.x >= BaseposX + (320*fscale) && mouse.x <= BaseposX + ((320 + 64)*fscale) && mouse.y >= BasePosY + (64*fscale) && mouse.y <= BasePosY + ((64 + 64)*fscale))    // <-- RightATTACK LANDING
	{
	DrawPartialDecal({ BaseposX + (320*fscale),  BasePosY + (64*fscale) }, m_Items, { 256,0 }, { (float)64,(float)(64) },{fscale,fscale});
		
	}

	/// SELECT RECTANGLE END




	if (GetMouse(0).bPressed)
	{
		if (mouse.x >=BaseposX+(192*fscale) && mouse.x <=BaseposX+((192+64)*fscale) && mouse.y >=BasePosY+(512*fscale) && mouse.y <=BasePosY+((512+64)*fscale) )    // <-- Mid
		{
			m_vecSaveTalents.push_back(2);
		}

		if (mouse.x >= BaseposX +(64*fscale) && mouse.x <= BaseposX +((64+64)*fscale) && mouse.y >= BasePosY +(512*fscale) && mouse.y <= BasePosY +((512+64)*fscale))    // <-- High
		{
			m_vecSaveTalents.push_back(3);
		}

		if (mouse.x >= BaseposX +(64*fscale) && mouse.x <= BaseposX +((64+64)*fscale) && mouse.y >= BasePosY +(640*fscale) && mouse.y <= BasePosY +((640+64)*fscale) )    // <-- Backstab
		{
			m_vecSaveTalents.push_back(4);
		} 

		if (mouse.x >= BaseposX +(64*fscale) && mouse.x <= BaseposX +((64+64)*fscale) && mouse.y >= BasePosY +(384*fscale) && mouse.y <= BasePosY +((384+64)*fscale) )    // <-- ShadowStap
		{
			m_vecSaveTalents.push_back(5);
		}

		if (mouse.x >= BaseposX + (64*fscale) && mouse.x <= BaseposX + ((64 + 64)*fscale) && mouse.y >= BasePosY + (192*fscale) && mouse.y <= BasePosY + ((192 + 64)*fscale))    // <-- Swirl
		{
			m_vecSaveTalents.push_back(6);
		}

		if (mouse.x >= BaseposX + (192*fscale) && mouse.x <= BaseposX + ((192 + 64)*fscale) && mouse.y >= BasePosY + (384*fscale) && mouse.y <= BasePosY + ((384 + 64)*fscale))    // <-- Vanish
		{
			m_vecSaveTalents.push_back(7);
		}


		if (mouse.x >= BaseposX +(320*fscale) && mouse.x <= BaseposX +((320+64)*fscale) && mouse.y >= BasePosY +(512*fscale) && mouse.y <= BasePosY +((512+64)*fscale))    // <-- RightAir
		{
			m_vecSaveTalents.push_back(12);
		}


		if (mouse.x >= BaseposX + (320*fscale) && mouse.x <= BaseposX + ((320 + 64)*fscale) && mouse.y >= BasePosY + (256*fscale) && mouse.y <= BasePosY + ((256 + 64)*fscale))    // <-- RightFLIGHTUP
		{
			m_vecSaveTalents.push_back(13);
		}

		if (mouse.x >= BaseposX + (320*fscale) && mouse.x <= BaseposX + ((320 + 64)*fscale) && mouse.y >= BasePosY + (64*fscale) && mouse.y <= BasePosY + ((64 + 64)*fscale))    // <-- RightATTACK LANDING
		{
			m_vecSaveTalents.push_back(14);
		}

	}



	Drawcursor(mouse);

	//Mouse
	//DrawPartialDecal({ (float)(mouse.x - 0.15f), (float)(mouse.y - 0.1f) }, RPG_Assets::get().GetSprite("Pantir's Dagger"), { 0,0 }, { (float)64,(float)(64) },{fscale,fscale});




	return true;
}


bool RPG_Engine::UpdateBlackSmith(float fElapsedTime)
{
	ClearAbsorbedSlots(m_listItems);

	
	olc::vf2d mouse = { (float)GetMouseX(), (float)GetMouseY() };
	olc::vf2d mousefix = { (mouse.x / (64 * fscale)) , (mouse.y / (64 * fscale)) };

	



	olc::Decal* Inventory = RPG_Assets::get().GetSprite("inventorySockets");
	olc::Decal* SpellUi = RPG_Assets::get().GetSprite("SpellUi");

	 float squeY = 8 * fscale;
	 float squeX = 4 * fscale;
	const float SmithsqueX = 14 * fscale;
	const float DescrUiWidth = (SpellUi->sprite->width * fscale) * 0.5f;
	const float DescrUiHeigh = (SpellUi->sprite->height * fscale) * 0.6f;
	const float ReqSocketsY = (float)(128 / 64) * fscale;
	const float RqSocketsX = (float)(192 / 64) * fscale;

	DrawPartialDecal({ squeX * 64, squeY * 64 }, Inventory, { 0,0 }, { (float)Inventory->sprite->width,(float)Inventory->sprite->height }, { fscale,fscale });

	DrawPartialDecal({ (float)(5.85 * fscale) * 64, (float)(11 * fscale) * 64 }, RPG_Assets::get().GetSprite("inventoryName"), { 0,0 }, { 274,75 }, { fscale,fscale });


	DrawPartialDecal({ SmithsqueX * 64, squeY * 64 }, Inventory, { 0,0 }, { (float)Inventory->sprite->width,(float)Inventory->sprite->height }, { fscale,fscale });
	DrawPartialDecal({ (float)(15.85 * fscale) * 64, (float)(11 * fscale) * 64 }, RPG_Assets::get().GetSprite("BlackSMithName"), { 0,0 }, { 274,75 }, { fscale,fscale });


	



	// Draw Consumables


	highlighted = nullptr;
	InventaryItem* Selected = nullptr;

	GrabItem = nullptr;


	

	 DrawInventory(squeX, squeY, mouse, mousefix, highlighted,GrabItem);




	  DrawBlacksmithInentory(SmithsqueX, squeY, mousefix, Selected, highlighted);

	float CraftBottomX = (SmithsqueX + RqSocketsX) * 64 + (DescrUiWidth / 5);
	float CraftBottomY = squeY * 64 - DescrUiHeigh + (DescrUiHeigh / 3);

	

	
	
	if ((int)mousefix.x >= SmithsqueX/fscale) // half screeb
	{

		

		if (GetMouse(0).bPressed)
		{


			if (highlighted != nullptr)
			{

				//highlighted->Objectselect = true;  // Here mousegrabed works in two dif case. in our invenotry we grabbed in blacksmith we craft
				highlighted->setFlag(highlighted->Objectselected);
				Selected = highlighted;
			}
			else
			{
				if (Selected != nullptr)
				{
					if (GetMouse(1).bPressed)
						Selected->clearFlag(Selected->Objectselected);
						//Selected->Objectselect = false;
						
					if (mousefix.x >= CraftBottomX / (64 * fscale)-0.5 && mousefix.x <= CraftBottomX / (64 * fscale) + 0.5 && mousefix.y >= CraftBottomY / (64 * fscale)-0.5 && mousefix.y <= CraftBottomY / (64 * fscale) + 0.5)  // Craft Word description
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
				if (highlighted->Item->OnUse(m_pPlayer,m_listItems))
				{
					// Item has signalled it must be consumed, so remove it
					TakeItem(highlighted,m_listItems);
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


			moveteItems(mousefix, squeX, squeY, GrabItem, m_listItems);
			
		}
	}



	if (Selected != nullptr)
	{
		



		//




		DrawPartialDecal({ (SmithsqueX + RqSocketsX) * 64, squeY * 64 - DescrUiHeigh }, SpellUi, { 0,0 }, { 334,142 }, { 0.5f * fscale,0.6f * fscale });

		if (Selected->Item->OnCraft(0))
		{
			DrawBigText("Craft", CraftBottomX, CraftBottomY, fscale, fscale, { 169,223,227 });

		}
		else
		{
			DrawBigText("Craft", CraftBottomX, CraftBottomY, fscale, fscale, olc::GREY);
		}

		DrawPartialDecal({ (SmithsqueX + ((Selected->index % 8)) * fscale) * 64, (float)(squeY + ((Selected->index / 8) * fscale)) * 64 }, RPG_Assets::get().GetSprite("SelectedObject"), { 0,0 }, { (float)64,(float)(64) }, { fscale,fscale });
		DrawPartialDecal({ SmithsqueX * 64, (float)(squeY - ReqSocketsY) * 64 }, Inventory, { 0,0 }, { 192,128 }, { fscale,fscale });
		DrawPartialDecal({ SmithsqueX * 64 - (12 * fscale), (float)(squeY - ReqSocketsY) * 64 - ((75 + 1) * fscale) }, RPG_Assets::get().GetSprite("RequiredName"), { 0,0 }, { 274,75 }, { fscale,fscale });

		//std::cout << (int)(mouse.x / (64 * fscale)) * fscale << '\t' << (int)(mouse.y / (64 * fscale)) * fscale << std::endl;



		 DrawCraftedRequires(SmithsqueX, squeY - ReqSocketsY, mousefix,Selected,highlighted);

	}



		if (highlighted != nullptr)
		{
			
			DrawDescriptionPattern(highlighted, mouse,mousefix);


		}



		// MONEY
		squeX = (4 * fscale) * 64;
		squeY = (7 * fscale) * 64;
		DrawBigText(std::to_string(Money), squeX + (64 * fscale), squeY + (16 * fscale), fscale, fscale, olc::YELLOW); //Money
		DrawPartialDecal({ (float)squeX, (float)squeY }, RPG_Assets::get().GetSprite("Coin"), { 0,0 }, { (float)64,(float)64 }, { fscale,fscale });
		//


		//Mouse
		DrawPartialDecal({ (float)(mouse.x - 0.15f), (float)(mouse.y - 0.1f) }, RPG_Assets::get().GetSprite("Pantir's Dagger"), { 0,0 }, { (float)64,(float)(64) }, { fscale,fscale });




		return true;
}

void RPG_Engine::DrawBlacksmithInentory(float offestX, float offsetY, olc::vi2d mouse, InventaryItem*& Selected,InventaryItem*& Highlighted)
{
	int i = 0;
	int x = 0;
	int y = 0;
	

	for (auto& item : m_listBlackSmithItems)
	{
		x = i % 8;

		y = i / 8;
		i++;


		DrawPartialDecal({ (float)(offestX + (x ) * fscale) * 64, (offsetY + (float)y  * fscale) * 64 }, item->Item->pSprite, { 0,0 }, { 64,64 }, { fscale,fscale });

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

			x = i % 2;

			y = i / 2;

			i++;
			DrawPartialDecal({ (float)(offestX + (x * fscale)) * 64, (float)(offsetY + (y * fscale)) * 64 }, RPG_Assets::get().GetItem(ItemName)->pSprite, { 0,0 }, { 192,128 }, { fscale,fscale });


			if (mouse.x == (offestX + x ) && mouse.y == (offsetY + y ))
				Highlighted->Item = RPG_Assets::get().GetItem(ItemName);


			for (auto& item : m_listItems)   // Count in our inventory this item
			{
				if (item->Item->sName == ItemName)
				{
					domain++;
				}
			}


			DrawBigText(std::to_string(domain) + "/" + ItemAmount, (offestX + (x * fscale)) * 64, (offsetY + ((y + 0.5f) * fscale)) * 64, fscale * fscale, fscale * fscale, { 169,223,227 });
			counter = 0;
			ItemName.clear();
			continue;

		}

		if (counter == 0)
		{
			ItemAmount = c;
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
		if (highlighted->Item->equipIndex>0)
		{
			nLinesY--;
		}
		else
		{
		if (nLinesX < 32 + 22 * 18 && !highlighted->Item->bKeyItem)  // fill @press space to use like default value
		{
			nLinesX = 32 + 22 * 18;

		}
		}
			
		const float mouseoffset = (64 * fscale);
		
		DrawPartialDecal({ (float)(mouse.x + mouseoffset) , (float)(mouse.y + mouseoffset) }, RPG_Assets::get().GetSprite("DescriptionPattern"), { 0,0 }, { (float)nLinesX,(float)(nLinesY) * (64*fscale) }, { fscale,fscale });
	

		//DrawString(4 * 64, squeY - 64, highlighted->sName, olc::WHITE,3);
		DrawBigText(highlighted->Item->sName, (mouse.x + mouseoffset), mouse.y + mouseoffset, fscale, fscale);

		//DrawString(8, 190, "SELECTED", olc::WHITE,3);
		//DrawString(4*64, squeY-32, highlighted->sDescription, olc::WHITE,2);

		DrawBigText(highlighted->Item->sDescription, (mouse.x + mouseoffset), mouse.y + (32*fscale) + mouseoffset, fscale, fscale, { 169,223,227 });

		




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
				DrawBigText("(Useable)", (mouse.x + (64 * fscale)), (mouse.y + ((nLinesY + 1)*fscale) * (64*fscale)), fscale - 0.1, fscale - 0.1, { 218,14,0,255 });
			}
			else
			{
				DrawBigText("(Can't Use)", (mouse.x + (64 * fscale)), (mouse.y + ((nLinesY + 1)*fscale) * (64*fscale)) , fscale - 0.1, fscale - 0.1, { 218,14,0,255 });
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
					//DrawString(80, squeY-160, "(Press SPACE to use)", olc::WHITE,2);
			//		DrawBigText("(Press Right Button to use)", (mouse.x + (64 * fscale)), (mouse.y + ((nLinesY + 1) * fscale) * (64 * fscale)), fscale - 0.1, fscale - 0.1, { 218,14,0,255 });
					DrawBigText("Sale:" + std::to_string(highlighted->Item->sTradePrice*highlighted->currStacks), (mouse.x + (64 * fscale)), (mouse.y + ((nLinesY + 1) * fscale) * (64 * fscale)), fscale, fscale, olc::YELLOW); //Money
				}
				else
				{
					DrawBigText("(Can't Use)", (mouse.x + (64 * fscale)), (mouse.y + ((nLinesY + 1) * fscale) * (64 * fscale)), fscale - 0.1, fscale - 0.1, { 218,14,0,255 });
				}
			}

			if ((int)mousefix.x >= 14  && (int)mousefix.x <= 14  + 8 && (int)mousefix.y >= 8 && (int)mousefix.y <= 8  + 3)  // Store decription
			{


				//DrawString(80, squeY-160, "(Press SPACE to use)", olc::WHITE,2);
			//	DrawBigText("(Press Right Button to use)", (mouse.x + (64 * fscale)), (mouse.y + ((nLinesY + 1) * fscale) * (64 * fscale)), fscale - 0.1, fscale - 0.1, { 218,14,0,255 });
				DrawBigText("Buy:" + std::to_string(highlighted->Item->sTradeBuyPrice), (mouse.x + (64 * fscale)), (mouse.y + ((nLinesY + 1) * fscale) * (64 * fscale)), fscale, fscale, olc::YELLOW); //Money


			}
			break;
		case MODE_PROFESSION:
			
			break;
		case MODE_MAP:
			
			break;
		
		}




}


bool RPG_Engine::UpdateMap(float fElapsedTime)
{

	
	olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY())};
	
	
	//if (mouse.x >= (ScreenWidth() / 2 - 512) + 331 && mouse.x <= (ScreenWidth() / 2 - 512) + 331+234 && mouse.y >= 60 + 301 && mouse.y <= 60 + 301+223)

	olc::Decal* lock = RPG_Assets::get().GetSprite("Lock");

	const float Xoffset = 512;
	const float Yoffset = 60;

	const float calculatedOffsetX = (float)ScreenWidth() / 2 - (Xoffset * fscale);
	DrawPartialDecal({ calculatedOffsetX, ((float)Yoffset)*fscale}, RPG_Assets::get().GetSprite("MapLayer"), {0,0}, {962,941},{fscale,fscale});

	DrawPartialDecal({ (calculatedOffsetX +(100*fscale)), (float)494*fscale }, RPG_Assets::get().GetSprite("VillageForestScetch"), { 0,0 }, { 285,241 }, { fscale,fscale });
	if (m_pCurrentMap->sName == "Village"|| (m_pCurrentMap->sName == "Forest" || mouse.x >= calculatedOffsetX + (100*fscale) && mouse.x <= calculatedOffsetX + (385*fscale) && mouse.y >= 494*fscale && mouse.y <= (494+241)*fscale) || SelectedTeleport == 1)
	DrawPartialDecal({ calculatedOffsetX + (100*fscale), (float)494*fscale }, RPG_Assets::get().GetSprite("VillageForestPaint"), { 0,0 }, { 285,241 }, { fscale,fscale });


	DrawPartialDecal({ calculatedOffsetX + (329*fscale),( Yoffset + 593)*fscale }, RPG_Assets::get().GetSprite("DesertValeScetch"), { 0,0 }, { 354,236 }, { fscale,fscale });
	if (m_pCurrentMap->sName == "DesertVale" || mouse.x >= calculatedOffsetX + (329*fscale) && mouse.x <= calculatedOffsetX + ((329+354)*fscale) && mouse.y >= (Yoffset + 593)*fscale && mouse.y <= (Yoffset + 593+236)*fscale || SelectedTeleport == 3)
	DrawPartialDecal({ (calculatedOffsetX + (329*fscale)), (Yoffset + 593)*fscale }, RPG_Assets::get().GetSprite("DesertValePaint"), { 0,0 }, { 354,236 }, { fscale,fscale });


	DrawPartialDecal({ calculatedOffsetX + (331*fscale), (Yoffset +301)*fscale }, RPG_Assets::get().GetSprite("MountainsScetch"), { 0,0 }, { 234,223 }, { fscale,fscale });
	if (m_pCurrentMap->sName == "Mountains" || mouse.x >= calculatedOffsetX + (331*fscale) && mouse.x <= calculatedOffsetX + ((331 + 234)*fscale) && mouse.y >= (Yoffset + 301)*fscale && mouse.y <= (Yoffset + 301 + 223)*fscale || SelectedTeleport == 2)
	DrawPartialDecal({ ((float)ScreenWidth() / 2 - (Xoffset*fscale)) + (331*fscale), (Yoffset +301)*fscale }, RPG_Assets::get().GetSprite("MountainsPaint"), { 0,0 }, { 234,223 }, { fscale,fscale });


	DrawPartialDecal({ calculatedOffsetX + (689*fscale), (Yoffset +294)*fscale }, RPG_Assets::get().GetSprite("HorseManGateScetch"), { 0,0 }, { 200,230 }, { fscale,fscale });
	if (m_pCurrentMap->sName == "HorseManGate" || mouse.x >= calculatedOffsetX + (689*fscale) && mouse.x <= calculatedOffsetX + ((689 + 200)*fscale) && mouse.y >= (Yoffset + 294)*fscale && mouse.y <= (Yoffset + 294 + 230)*fscale || SelectedTeleport == 4)
	DrawPartialDecal({ calculatedOffsetX + (689*fscale), (Yoffset +294)*fscale }, RPG_Assets::get().GetSprite("HorseManGatePaint"), { 0,0 }, { 200,230 }, { fscale,fscale });


	DrawPartialDecal({ calculatedOffsetX + (751*fscale), (Yoffset + 595)*fscale }, RPG_Assets::get().GetSprite("MonasteryScetch"), { 0,0 }, {142,174 }, { fscale,fscale });
	if (m_pCurrentMap->sName == "Monastery" || mouse.x >= calculatedOffsetX + (751*fscale) && mouse.x <= calculatedOffsetX + ((751+142)*fscale) && mouse.y >= (Yoffset + 595)*fscale && mouse.y <= (Yoffset + 595+174)*fscale || SelectedTeleport == 5)
	DrawPartialDecal({ calculatedOffsetX + (751*fscale), (Yoffset + 595)*fscale }, RPG_Assets::get().GetSprite("MonasteryPaint"), { 0,0 }, { 142,174 }, { fscale,fscale });



	DrawPartialDecal({ calculatedOffsetX + (643*fscale), (Yoffset + 66)*fscale }, RPG_Assets::get().GetSprite("SkullCaveScetch"), { 0,0 }, { 218,207 }, { fscale,fscale });
	DrawPartialDecal({ calculatedOffsetX + (79*fscale), (Yoffset + 79)*fscale }, RPG_Assets::get().GetSprite("ElvishGraveyardScetch"), { 0,0 }, { 191,257 }, { fscale,fscale });
	DrawPartialDecal({ calculatedOffsetX + (308*fscale), (Yoffset + 52)*fscale }, RPG_Assets::get().GetSprite("SwampOfWitchScetch"), { 0,0 }, { 314,213 }, { fscale,fscale });


	DrawPartialDecal({ calculatedOffsetX + (122*fscale), (Yoffset +126)*fscale }, lock, { 0,0 }, { 314,213 }, { fscale,fscale });
	DrawPartialDecal({ calculatedOffsetX + (405*fscale), (Yoffset + 98)*fscale }, lock, { 0,0 }, { 314,213 }, { fscale,fscale });
	DrawPartialDecal({ calculatedOffsetX + (698*fscale), (Yoffset + 96)*fscale }, lock, { 0,0 }, { 314,213 }, { fscale,fscale });






	if (GetMouse(0).bPressed)
	{
		if (!bOpenTravelAsk)
		{
			if (mouse.x >= calculatedOffsetX + (329*fscale) && mouse.x <= calculatedOffsetX + ((329 + 354)*fscale) && mouse.y >= ((60 + 593)*fscale) && mouse.y <= (60 + 593 + 236)*fscale) //DesertValePaint
			{
				bOpenTravelAsk = true;
				SelectedTeleport = 3;
			}


			if (mouse.x >= calculatedOffsetX + (751*fscale) && mouse.x <= calculatedOffsetX + ((751 + 142)*fscale) && mouse.y >= (60 + 595)*fscale && mouse.y <= (60 + 595 + 174)*fscale) //Monastery
			{
				bOpenTravelAsk = true;
				SelectedTeleport = 5;
			}

			if (mouse.x >= calculatedOffsetX + (689*fscale) && mouse.x <= calculatedOffsetX + ((689 + 200)*fscale) && mouse.y >= (60 + 294)*fscale && mouse.y <= (60 + 294 + 230)*fscale) //HorseManBridge
			{
				bOpenTravelAsk = true;
				SelectedTeleport = 4;
			}
			if (mouse.x >= calculatedOffsetX + (100*fscale) && mouse.x <= calculatedOffsetX + ((100 + 285)*fscale) && mouse.y >= 494*fscale && mouse.y <= (494 + 241)*fscale) //Village
			{
				bOpenTravelAsk = true;

				SelectedTeleport = 1;
			}
			if (mouse.x >= calculatedOffsetX + (331*fscale) && mouse.x <= calculatedOffsetX + ((331 + 234)*fscale) && mouse.y >= (60 + 301)*fscale && mouse.y <= (60 + 301 + 223)*fscale) //Mountains
			{

				bOpenTravelAsk = true;
				SelectedTeleport = 2;

			}

		}

		if (bOpenTravelAsk)
		{
			
			if (mouse.x >= (ScreenWidth() / 2) + 160 - 36 - 7 && mouse.x <= (ScreenWidth() / 2) + 160 - 36 - 7+64 && mouse.y >= ScreenHeight() / 2 + 31 && mouse.y <= ScreenHeight() / 2 + 31+32) //No
			{

				bOpenTravelAsk = false;
				SelectedTeleport = 0;
			}


			if (mouse.x >= (ScreenWidth() / 2) - 160  && mouse.x <= (ScreenWidth() / 2) - 160 +72 && mouse.y >= ScreenHeight() / 2 + 31 && mouse.y <= ScreenHeight() / 2 + 31 + 32) //Yes
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

	DrawPartialDecal({ ((float)ScreenWidth() / 2 )-167, (float)ScreenHeight()/2-71}, RPG_Assets::get().GetSprite("SpellUi"), {0,0}, {334,142});
	DrawBigText("Travel ?", (ScreenWidth() / 2) -72, ScreenHeight() / 2 -62 , 1,1,olc::WHITE);

	DrawBigText("Yes", (ScreenWidth() / 2) - 160, ScreenHeight() / 2 + 31, 1, 1, olc::WHITE);
	DrawBigText("No", (ScreenWidth() / 2) + 160-36 -7, ScreenHeight() / 2 + 31, 1, 1, olc::WHITE);
	}


	



	//Mouse
	DrawPartialDecal({ (float)(mouse.x - 0.15f), (float)(mouse.y - 0.1f)  }, RPG_Assets::get().GetSprite("Pantir's Dagger"), { 0,0 }, { (float)64,(float)(64) });



	return true;
}

bool RPG_Engine::SaveFunction()
{
	std::ofstream data;
	int AmountQuests = 0;
	int AmountTalents = 0;
	data.open("Load/CurrSave.txt");
	if (data.is_open())
	{
		data << GetLvl();
		data << "\t";
		data << GetCurrExp();
		data << "\t";
		data << GetRequredExp();
		data << "\t";
		data << GetCurrentMap()->sName;
		data << "\t";
		data << m_pPlayer->px;
		data << "\t";
		data << m_pPlayer->py;
		data << "\t";
		

		AmountQuests = GetListQuest().size();
		AmountTalents = GetLearnedTalentVector().size();




		data << AmountQuests;
		data << "\t";
		data << AmountTalents;
		data << "\t";
		for (auto i : GetListQuest())
		{
			data << "\n";
			data << i->SaveSlot;  // 0 main quest
			data << " ";
			data << i->GetPhase();

		}
		data << "\t";
		for (auto i : GetLearnedTalentVector())
		{
			data << "\n";
			data << i;  // 0 main talents

		}









		return true;
		std::cout << "save Succeed" << std::endl;

	}
	else
	{
		return false;
		std::cout << "save Denied" << std::endl;
	}
	data.close();

}



bool RPG_Engine::LoadFunction()
{
	std::ifstream data;
	std::string mapName;
	data.open("Load/CurrSave.txt", std::ofstream::in);

	if (data.is_open())
	{

		m_listQusets.clear();
		//m_listQusets.push_front(new cQuset_MainQuest()); // add in lust First Main quest
		int QuestSlot;
		int AmountQuests;
		int AmountTalents;

		data >> lvl;
		data >> currExp;
		data >> ExpRequred;
		data >> mapName;
		data >> m_pPlayer->px;
		data >> m_pPlayer->py;

		//m_pPlayer->GetFacingDirection
		data >> AmountQuests;
		data >> AmountTalents;

		for (int i = 0; i < AmountQuests; i++)
		{
			int phase;

			data >> QuestSlot;

			data >> phase;

			RPG_Assets::get().GetQuest(QuestSlot)->SetPhase(phase);
			AddQuest(RPG_Assets::get().GetQuest(QuestSlot));
			
	
		}
		for (int i = 0; i < AmountTalents; i++)
		{
			int help;
			data >> help;
			m_vecSaveTalents.push_back(help);
		}




		m_pPlayer->SetFacingDirection(1);

		std::cout << "Load succeed" << std::endl;
		
		//m_script.AddCommand((new cComand_AppearScreen(2)));
		data.close();
		m_script.AddCommand((new cComand_HideScreen(2)));
		m_script.AddCommand((new cComand_Changemap(mapName, m_pPlayer->px, m_pPlayer->py)));   //clear all vectors
		m_script.AddCommand((new cComand_SetNgameMod(1)));
		m_script.AddCommand((new cComand_CalculateExp));   //clear all vectors
		//fCameraPosX = m_pPlayer->px;
		//fCameraPosY = m_pPlayer->py;
	//	ScrollingCurrX = fCameraPosX;
	//	n_nGameMode = MODE_LOCAL_MAP;


	}
	else
	{
		m_script.AddCommand((new cComand_HideScreen(2)));

		m_script.AddCommand((new cComand_ResetQuestList));   //clear all vectors

		m_script.AddCommand((new cComand_Changemap("Forest", 4, 24)));
		m_script.AddCommand((new cComand_SetNgameMod(1)));
		m_script.AddCommand((new cComand_CalculateExp));   //clear all vectors
		
	

		//ScrollingCurrX = fCameraPosX;

		//	n_nGameMode = MODE_LOCAL_MAP;
		return false;

	}
	data.close();
	return true;

}

void RPG_Engine::AddQuest(cQuest* quest)
{
	m_listQusets.push_back(quest);
}

cQuest* RPG_Engine::GetQuest(std::string name)
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
		ReturnToPool(source);
		//for (auto& dyns : *source)
		

    m_vecDynamics.clear();
	m_vecProjectiles.clear();
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

	ScrollingY = y;    //  <--Logic for parralax  y coordinate we find position of changes

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

bool RPG_Engine::GiveItem(std::string Name,uint8_t count)
{
	
	

	uint8_t hcount = count;   // how many objects need to give
	cItem* Item = RPG_Assets::get().GetItem(Name);     // that item what we want to add in our inventary

//	Item->MaxStack                                     // how much they cant stuck in one slot
	


	
	
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
	

		for (auto& items : m_listItems)
		{
			if (items->Item->sName == item )
				return true;
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
void RPG_Engine::EraseEnemysPool(cDynamic* proj)
{
	

	for (auto it = enemysPool.begin(); it != enemysPool.end(); ++it) {
		cDynamic* entity = *it;
	
		if (proj == entity){

			std::cout << "Hello" << std::endl;
		entity->setFlag(entity->binitialized);
		entity->px = proj->px;
		entity->py = proj->py;
		m_vecDynamics.push_back(proj); // Add the Bandit entity to the game
		enemysPool.erase(it); // Remove the Bandit entity from the pool
			break;

		
		}
	}


	
}
//void RPG_Engine::AddEnvironment(Environment* env)
//{
//	m_vecEnviroment.push_back(env);
//}
void RPG_Engine::AddIndicators(cDynamic* ind)
{
	m_vecIndicators.push_back(ind);
}

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

int RPG_Engine::GetCurrExp()
{
	return currExp;
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

			m_vecParticles.push_back(new VfxLevel(m_pPlayer->px, m_pPlayer->py));

			int spare = currExp - ExpRequred;
			CalculateExp();

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

		cDynamic_TextDamage* Text = new cDynamic_TextDamage(victim->px+victim->CollbordersX+0.25f, victim->py+victim->CollbordersY+0.6f, std::to_string(CalculatedDamage));

		//m_vecFightText.push_back(Text);
		

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
	m_vecParticles.push_back(new VfxShot(px, py));
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

void RPG_Engine::DrawBigText(std::string sText, int x, int y, float scalex, float scaley, olc::Pixel color)
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
		DrawPartialDecal({ (float)x + (i * (18 * scalex)), (float)y + (z * (32 * scaley)) },
			m_sprFont, { sx, sy }, { 32, 32 }, { scalex, scaley }, color);
		i++;  // Переходим к следующему символу по X
	}

}

void RPG_Engine::DisplayDialog(std::vector<std::string> vecText, int x, int y)
{
	int nMaxLineLength = 0;
	int nLines = vecText.size();

	for (auto i : vecText)  if (i.size() > nMaxLineLength) nMaxLineLength = i.size();


	
	olc::Decal* BlackPattern = RPG_Assets::get().GetSprite("DescriptionPattern");


	DrawPartialDecal({ (float)x - (1*fscale) , (float)y - (1 * fscale) }, BlackPattern, { 0,0 }, { (float)nMaxLineLength * 18 + 10,(float)nLines * 32 +10 }, {fscale,fscale});

	DrawPartialDecal({ (float)x - (1 * fscale),  (float)y - (1 * fscale) }, D_Ui, { 1227,0 }, { (float)4,(float)nLines * 32 + 15 }, { 1*fscale,1*fscale}, {66,83,100});
	DrawPartialDecal({ (float)x + ((nMaxLineLength * 18 + 9)*fscale),  (float)y - (1*fscale) }, D_Ui, { 1227,0 }, { (float)4,(float)nLines * 32 + 15 }, { 1*fscale,1*fscale }, { 66,83,100 });

	DrawPartialDecal({ (float)x - (1 * fscale), (float)y - (1 * fscale) }, D_Ui, { 0,910 }, { (float)nMaxLineLength * 18 + 14,(float)4 }, { 1*fscale,1*fscale }, { 66,83,100 });
	DrawPartialDecal({ (float)x - (1 * fscale), (float)y + ((nLines * 32 + 9)*fscale) }, D_Ui, { 0,910 }, { (float)nMaxLineLength * 18 + 14,(float)4 },{1*fscale,1*fscale},{66,83,100});


	

	

	

	for (int l = 0; l < vecText.size(); l++)
		DrawBigText(vecText[l], x, y + ((l * 32)*fscale), 1*fscale,1*fscale);

}

void RPG_Engine::WrapCoordinates(float ix,  float& ox)
{
	ox = ix;
	//oy = iy;

	if (ix < 0.0f) ox = ix + (float)ScreenWidth();

	
	if (ix >= ScreenWidth()) ox = ix-(float)ScreenWidth();
	//if (iy < 0.0f) oy = iy + (float)ScreenHeight();
	//if (iy >= ScreenHeight()) oy = iy - (float)ScreenHeight();

	
}

int RPG_Engine::GetRage()
{
	return m_pPlayer->rageAmount;
}

uint8_t RPG_Engine::GetEnergy()
{
	return m_pPlayer->energyAmount;
}
bool RPG_Engine::GetBackStab()
{
	return bUibackstub;
	
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
// Function to draw a parallax layer
void RPG_Engine::DrawParallaxLayer(float x, olc::Decal* decal)
{
	WrapCoordinates(x, x);
	float fx;
	WrapCoordinates(x, fx);

	DrawPartialDecal({ x, 0 }, decal, { 0, 0 }, { (float)ScreenWidth(), (float)ScreenHeight() });
	DrawPartialDecal({ fx - (float)ScreenWidth(), 0 }, decal, { 0, 0 }, { (float)ScreenWidth(), (float)ScreenHeight() });
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


		

		float obsticleangle = atan2( object->second->y-object->first->y  , object->second->x - object->first->x);

		float normal =  3.14 / 2 + obsticleangle;

		

		float charangle = atan2( object->second->y-pyYf, object->second->x - pxX);


			//std::cout <<std::sin(normal) <<'\t' << std::cos(normal) << '\t' << cos(normal) * cos(charangle) + sin(normal) * sin(charangle) << std::endl;
		

		if (cos(normal) * cos(charangle) + sin(normal) * sin(charangle) <= 0) //dot product
		{



			for (float i = 0; i <= 2; )
			{
				float innernormal = atan2(  object->second->y- (pyYf - i), object->second->x - pxX);

				if (cos(normal) * cos(innernormal) + sin(normal) * sin(innernormal) <= 0)
				{

					return false;
					break;

				}

				i += 0.1f;


			}

		}




	}
	if ((m_pCurrentMap->GetColliziionIndex(pxX, pyYf) == 3))
	{

		std::pair<olc::vf2d*, olc::vf2d*>* object = m_pCurrentMap->getObsticlesPoints(pxX, pxX, pyYf, pyYf);

		float slopeangle = atan2(object->second->y - object->first->y, object->second->x - object->first->x);
		float normal = slopeangle + 3.14 / 2;

		float charangle = atan2(pyYf - object->first->y, pxX- object->first->x  );


	
		if (cos(normal) * cos(charangle) + sin(normal) * sin(charangle) >= 0) //dot product
		{


			for (float i = 0; i <= 2; )
			{
				float innernormal = atan2((pyYf + i) - object->first->y, pxX - object->first->x);

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


		int x = tile.position.x;
		int y = tile.position.y;


	//	bool horizontal = true;
	//	bool vertical = true;
	//	bool firstcheck = true;

		int* tileindex = nullptr;
		int* itindex = nullptr;

		olc::vi2d* tilesize = nullptr;
		olc::vi2d* tilesource = nullptr;

		olc::vi2d* itsize = nullptr;
		olc::vi2d* itsource = nullptr;


		// Get the layerinfo corresponding to the specified layer
		Tile::layerinfo* currentLayer = nullptr;
		switch (layer) {
		case 0:
			currentLayer = &tile.ZeroLayer;
			tileindex = &tile.ZeroLayer.index;
			tilesize = &tile.ZeroLayer.size;
			tilesource = &tile.ZeroLayer.sourcerect;
			break;
		case 1:
			currentLayer = &tile.FirstLayer;
			tileindex = &tile.FirstLayer.index;
			tilesize = &tile.FirstLayer.size;
			tilesource = &tile.FirstLayer.sourcerect;
			break;
		case 2:
			currentLayer = &tile.SecondLayer;
			tileindex = &tile.SecondLayer.index;
			tilesize = &tile.SecondLayer.size;
			tilesource = &tile.SecondLayer.sourcerect;

			break;
		case 3:
			currentLayer = &tile.ThirdLayer;
			tileindex = &tile.ThirdLayer.index;
			tilesize = &tile.ThirdLayer.size;
			tilesource = &tile.ThirdLayer.sourcerect;

			break;
		default:
			break;
		}

		// If the current layer is nullptr, exit the loop
		if (!currentLayer) continue;

		// Lambda function to find neighboring tile based on the current layer
		auto findNeighboringTileOne = [&](Tile& t) {
			switch (layer) {
			case 0:
				itindex = &t.ZeroLayer.index;
				itsize = &t.ZeroLayer.size;
				itsource = &t.ZeroLayer.sourcerect;
				break;
			case 1:
				itindex = &t.FirstLayer.index;
				itsize = &t.FirstLayer.size;
				itsource = &t.FirstLayer.sourcerect;
				break;
			case 2:
				itindex = &t.SecondLayer.index;
				itsize = &t.SecondLayer.size;
				itsource = &t.SecondLayer.sourcerect;
				break;
			case 3:
				itindex = &t.ThirdLayer.index;
				itsize = &t.ThirdLayer.size;
				itsource = &t.ThirdLayer.sourcerect;
				break;
			default:
				return false;
				break;
			}
			return true;
		};


		if ( *tileindex != 0)
		{

			int diffx = 0;
			int diffy = 0;
			bool horizontalCell;
			int countery = 0;
			// Iterate through neighboring tiles (Down and right)
			for (int dx = 0; dx <=4-diffx; ++dx) {

				horizontalCell = false;

				for (int dy =0; dy <= 4-diffy; ++dy) {
					// Skip the current tile itself
					if (dx == 0 && dy == 0) continue;



				///	horizontal = false;
				///	vertical = false;
					// Calculate the position of the neighboring tile
					int nx = x + dx;
					int ny = y + dy;





					// Find the neighboring tile in the batch
					auto zt = std::find_if(batchLayer.begin(), batchLayer.end(),
						[&](Tile& t) { return t.position.x == nx && t.position.y == ny; });


					// If the neighboring tile exists and has the same texture index
					if (zt != batchLayer.end() )
					{
						// Merge the size of the neighboring tile with the current tile
					bool neighboringTileFound = findNeighboringTileOne(*zt);

					if (*itindex != 0 && *tileindex !=0)
					{

						if (*itindex==*tileindex )
						{
							continue;
						}
						if (tilesource->x + tilesize->x == itsource->x && tilesource->y == itsource->y && y == ny)  // right cell
						{
							tilesize->x += itsize->x;
							*itindex = 0;
							horizontalCell = true;
							continue;
						}

					//	if (itsource->x + itsize->x == tilesource->x && tilesource->y == itsource->y && itsource->y + itsize->y == tilesource->y + tilesize->y)  // left cell
					//	{
					//		itsize->x += tilesize->x;
					//		*tileindex = 0;
					//		horizontalCell = true;

					//		continue;
					//	}



						if (horizontalCell == false && dx > 0)
						{
							break;
						}


						if (tilesource->x == itsource->x && tilesource->y + tilesize->y == itsource->y && itsource->x + itsize->x == tilesource->x + tilesize->x) // down cell
						{
							tilesize->y += itsize->y;
							*itindex = 0;
						
							continue;

						}


					


						//if (tilesource->x+tilesize->x == itsource->x+tilesize->x && tilesource->y + tilesize->y == itsource->y+itsize->y) // down right cell
					//	{
					//		*itindex = 0;
					//		continue;
					//	}

					//	if (itsource->x == tilesource->x && itsource->y + itsize->y == itsource->y && itsource->x + itsize->x == tilesource->x + tilesize->x)  // Up cell
					//	{
					//		itsize->y += tilesize->y;
					//		*tileindex = 0;
							
					//		continue;
					//	}
						if (itsource->x > tilesource->x && itsource->x < tilesource->x + tilesize->x)       // when it in tile
							if (itsource->y > tilesource->y && itsource->y < tilesource->y + tilesize->y)
								if (itsource->x + itsize->x <= tilesource->x + tilesize->x && itsource->x + itsize->x > tilesource->x)
									if (itsource->y + itsize->y <= tilesource->y + tilesize->y && itsource->y + itsize->y > tilesource->y)
									{
										*itindex = 0;
										continue;
									}



						if (dy > 0)
						{
							break;
						}
						
						countery++;


						diffy = countery;
						
						if (countery ==0)
						{
							diffy = 4;
						}
				}

			}

		}
		if (horizontalCell ==false && dx >0)
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

void RPG_Engine::LoadenemyInstances()
{
	// Initialize the Bandit pool
	for (int i = 0; i < 30; ++i) {
		BanditsPool.push_back(new cDynamic_creature_Bandit());
	}

	// Initialize the Wolf pool
	for (int i = 0; i <30; ++i) {
		DireWolfsPool.push_back(new cDynamic_creature_DireWolf());
	}


	for (int i = 0; i < 30; ++i) {
		BoarPool.push_back(new cDynamic_creature_Boar());
	}

	for (int i = 0; i < 30; ++i) {
		WereWolfsPool.push_back(new cDynamic_creature_WereWolf());
	}

	for (int i = 0; i < 30; ++i) {
		BanditsArcherPool.push_back(new cDynamic_creature_BanditArcher());
	}
	for (int i = 0; i < 30; ++i) {
		ItemPool.push_back(new cDynamic_Item());
	}
	for (int i = 0; i < 30; ++i) {
		TextPool.push_back(new cDynamic_TextDamage());
	}
	for (int i = 0; i < 40; ++i) {
		ProjectilePool.push_back(new cDynamic_Projectile());
	}



	// Initialize the raindrops
	for (int i = 0; i < 100; ++i)
	{
		// Randomize starting positions, speeds, and angles for the raindrops
		float startX = static_cast<float>(rand() %ScreenWidth()/64);
		float startY = static_cast<float>(rand() %ScreenHeight()/64);
		float speed = static_cast<float>(rand() % 500) + 450; // Random speed between 50 and 150 pixels per second
		float angle = static_cast<float>(rand() % 45) * 3.14159f / 180.0f; // Random angle between 0 and pi radians
		EnvironmentPool.push_back(new ERaindrop(startX, startY, speed, angle));
	}


	for (int i = 0; i <= 5; ++i)
	{
	
		EnvironmentPool.push_back(new Edynamic_Cloud(rand()%256*64, 0.0f, RPG_Assets::get().GetSprite("ForestCloud"), 1));
	}


}
cDynamic* RPG_Engine::SpawnBanditArcher(const olc::vf2d* position)
{
	for (auto it = BanditsArcherPool.begin(); it != BanditsArcherPool.end(); ++it) {
		cDynamic* entity = *it;

		// Предполагаем, что если мы нашли подходящий объект, спауним его

		entity->setFlag(entity->binitialized);    // Устанавливаем флаг инициализации
		m_vecDynamics.push_back(entity);          // Добавляем в активный вектор динамических объектов
		entity->px = position->x;                 // Устанавливаем позицию
		entity->py = position->y;

		BanditsArcherPool.erase(it);                    // Удаляем заспаунинного бандита из пула
		return entity;                            // Возвращаем указатель на заспаунинного бандита

	}

	return nullptr;  // Если бандитов в пуле не осталось
}



cDynamic* RPG_Engine::SpawnBandit(const olc::vf2d* position)
{
	for (auto it = BanditsPool.begin(); it != BanditsPool.end(); ++it) {
		cDynamic* entity = *it;

		// Предполагаем, что если мы нашли подходящий объект, спауним его
		
			entity->setFlag(entity->binitialized);    // Устанавливаем флаг инициализации
			m_vecDynamics.push_back(entity);          // Добавляем в активный вектор динамических объектов
			entity->px = position->x;                 // Устанавливаем позицию
			entity->py = position->y;

			BanditsPool.erase(it);                    // Удаляем заспаунинного бандита из пула
			return entity;                            // Возвращаем указатель на заспаунинного бандита
		
	}

	return nullptr;  // Если бандитов в пуле не осталось
}


cDynamic* RPG_Engine::SpawnWerewolf(const olc::vf2d* position)
{
	for (auto it = WereWolfsPool.begin(); it != WereWolfsPool.end(); ++it) {
		cDynamic* entity = *it;
		
			entity->setFlag(entity->binitialized);
			m_vecDynamics.push_back(entity); // Add the Bandit entity to the game
			entity->px = position->x;
			entity->py = position->y;
			WereWolfsPool.erase(it); // Remove the Bandit entity from the pool
			return entity; // Exit the function after spawning a Bandit
		
	}
	return nullptr;  // Если бандитов в пуле не осталось
}

cDynamic* RPG_Engine::SpawnBoar(const olc::vf2d* position)
{
	for (auto it = BoarPool.begin(); it != BoarPool.end(); ++it) {
		cDynamic* entity = *it;
		
			entity->setFlag(entity->binitialized);
			m_vecDynamics.push_back(entity); // Add the Bandit entity to the game
			entity->px = position->x;
			entity->py = position->y;
			BoarPool.erase(it); // Remove the Bandit entity from the pool
			return entity; // Exit the function after spawning a Bandit
		
	}
	return nullptr;  // Если бандитов в пуле не осталось
}


cDynamic* RPG_Engine::SpawnDireWolf(const olc::vf2d* position)
{

	for (auto it = DireWolfsPool.begin(); it != DireWolfsPool.end(); ++it) {
		cDynamic* entity = *it;


		entity->setFlag(entity->binitialized);
		m_vecDynamics.push_back(entity); // Add the Bandit entity to the game
		entity->px = position->x;
		entity->py = position->y;
		DireWolfsPool.erase(it); // Remove the Bandit entity from the pool


		return entity; // Exit the function after spawning a Bandit

	}
	return nullptr; // Exit the function after spawning a Bandit
}



cDynamic* RPG_Engine::SpawnProjectile(const olc::vf2d* position)
{

	for (auto it = ProjectilePool.begin(); it != ProjectilePool.end(); ++it) {
		cDynamic* entity = *it;


		entity->setFlag(entity->binitialized);
		m_vecProjectiles.push_back(entity); // Add the Bandit entity to the game
		entity->px = position->x;
		entity->py = position->y;
		ProjectilePool.erase(it); // Remove the Bandit entity from the pool


		return entity; // Exit the function after spawning a Bandit

	}
	return nullptr; // Exit the function after spawning a Bandit
}

void RPG_Engine::ReturnTextToPool(cDynamic* Text)
{
	auto* proj = (cDynamic_TextDamage*)Text;
	proj->SetDeafult();


	ProjectilePool.push_back(proj);
}


cDynamic* RPG_Engine::SpawnBattleText(const olc::vf2d position, std::string Text, olc::Pixel Color )
{

	for (auto it = TextPool.begin(); it != TextPool.end(); ++it) {
		cDynamic* entity = *it;
		
		cDynamic_TextDamage* Textentity = (cDynamic_TextDamage*)entity;


		Textentity->setFlag(Textentity->bRedundant);
		m_vecFightText.push_back(Textentity); // Add the Bandit entity to the game
		Textentity->px = position.x;
		Textentity->py = position.y;
		Textentity->SetText(Text);
		TextPool.erase(it); // Remove the Bandit entity from the pool


		return entity; // Exit the function after spawning a Bandit

	}
	return nullptr; // Exit the function after spawning a Bandit
}


void RPG_Engine::SpawnRainDrops()
{
	for (auto it = EnvironmentPool.begin(); it != EnvironmentPool.end(); ++it) {
		Environment* entity = *it;
		if (dynamic_cast<ERaindrop*>(entity)) {
			m_vecCloseWeather.push_back(entity); // Add the Raindt entity to the game
			EnvironmentPool.erase(it); // Remove the Rain entity from the pool
			return; // Exit the function after spawning a Bandit
		}
	}
}
void RPG_Engine::SpawnClouds()
{
	for (auto it = EnvironmentPool.begin(); it != EnvironmentPool.end(); ++it) {
		Environment* entity = *it;
		if (dynamic_cast<Edynamic_Cloud*>(entity)) {
			
			m_vecFarWeather.push_back(entity); // Add the Raindt entity to the game
			EnvironmentPool.erase(it); // Remove the Rain entity from the pool
			return; // Exit the function after spawning a Bandit
		}
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
void RPG_Engine:: ReturnToPool(cDynamic* entity) {
	// Обнуляем общий флаг и состояние для любого существа
	entity->clearFlag(entity->binitialized);
	entity->setFlag(entity->bDraw);
	entity->clearFlag(entity->bDead);
	//entity->nHealth = entity->nHealthMax;

	// Приведение к конкретному типу и возврат в соответствующий пул
	if (auto* bandit = dynamic_cast<cDynamic_creature_Bandit*>(entity)) {
		bandit->nHealth = bandit->nHealthMax;
		BanditsPool.push_back(bandit);
	}
	else if (auto* wolf = dynamic_cast<cDynamic_creature_DireWolf*>(entity)) {
		wolf->nHealth = wolf->nHealthMax;
		DireWolfsPool.push_back(wolf);
	}
	else if (auto* boar = dynamic_cast<cDynamic_creature_Boar*>(entity)) {
		boar->nHealth = boar->nHealthMax;
		BoarPool.push_back(boar); // Создать отдельный пул для Boar, если его нет
	}
	else if (auto* werewolf = dynamic_cast<cDynamic_creature_WereWolf*>(entity)) {
		werewolf->nHealth = werewolf->nHealthMax;
		WereWolfsPool.push_back(werewolf);
	}
	else if (auto* archer = dynamic_cast<cDynamic_creature_BanditArcher*>(entity)) {
		archer->nHealth = archer->nHealthMax;
		BanditsArcherPool.push_back(archer);
	}
	else {
		// Логирование или выброс исключения при неизвестном типе
		std::cerr << "Unknown entity type. Could not return to pool.\n";
		// Или бросить исключение:
		// throw std::runtime_error("Unknown entity type");
	}
}
void RPG_Engine::ReturnProjectileToPool(cDynamic* Prjectile) {
	auto* proj = (cDynamic_Projectile*)Prjectile;
	proj->SetDeafult();
	

	ProjectilePool.push_back(proj);

}

void RPG_Engine::ReturnEnvironmentToPool(Environment* entity) {

	// Check the dynamic type of the entity
	if (ERaindrop* Drop = dynamic_cast<ERaindrop*>(entity)) {
		// If the entity is a Bandit, return it to the Bandit pool
		EnvironmentPool.push_back(Drop);
	}
	else if (Edynamic_Cloud* Cloud = dynamic_cast<Edynamic_Cloud*>(entity)) {
		// If the entity is a DireWolf, return it to the DireWolf pool
		EnvironmentPool.push_back(Cloud);
	} // Add more conditions for other types if needed
	else {
		// Handle unknown entity types or errors
		// You might want to log an error or throw an exception
	}
}
