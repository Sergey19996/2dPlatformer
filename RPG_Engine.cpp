#include "RPG_Engine.h"



RPG_Engine::RPG_Engine()
{
    sAppName = "Pantrir platformer";
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

    

	m_sprFont = RPG_Assets::get().GetSprite("font");   //take font


	
	m_listQusets.push_front(RPG_Assets::get().GetQuest(0)); // add in lust First Main quest


    m_pPlayer = new cDynamic_creature_Pantir();   //create char 


	CalculateExp();


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

	//GiveItem(new cWeapon_PantirSword());
	//GiveItem(new cWeapon_BrokenSword());
	/*GiveItem(new cCoin());
	GiveItem(new cCoin());
	GiveItem(new cCoin());
	GiveItem(new cCoin());
	GiveItem(new cCoin());*/
	GiveItem(new cSmallWallet());
	/*GiveItem(new cCoin());
	GiveItem(new cWeapon_BanditSword());
	GiveItem(new cCoin());*/




	nVisibleTileX = ScreenWidth() / 64;
	nVisibleTileY = ScreenHeight() / 64;
	
	//AddUi((RPG_Assets::get().GetUiElements("Jump")));


	
	//m_vecHpBars.push_back(RPG_Assets::get().GetIndicators("HpFirst"));




	layer = CreateLayer();

	

	m_sprPressF = RPG_Assets::get().GetSprite("PressF");
	m_sprGameCursor = RPG_Assets::get().GetSprite("Pantir's Dagger");
	



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

	// Define a fixed time step for updating game logic
	const float fixedTimeStep = 1.0f / 60.0f; // Assuming 120 updates per second



	// Perform game logic updates
	switch (n_nGameMode) {
	case MODE_TITLE:
		Result = UpdateTitleScreen(fixedTimeStep);
		break;
	case MODE_LOCAL_MAP:
	case MODE_INVENTORY:
	case MODE_MAP:
	case MODE_WAREHOUSE:
	case MODE_SHOP:
	case MODE_PROFESSION:
	case MODE_BLACKSMITH:
		Result = UpdateLocalMap (fixedTimeStep);
		break;
	}

	// Calculate sleep duration to achieve desired frame rate
	float fSleepTime = fixedTimeStep - fElapsedTime;

	// Check if sleep time is needed and sleep if necessary
	if (fSleepTime > 0.0f) {
	/*	 Perform more accurate timing or yield CPU instead of sleeping
		 ...

		 For demonstration purposes, sleeping is used here*/
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(fSleepTime * 1000)));
		
	}


	DrawString(ScreenWidth() - 100, 0, std::to_string(GetFPS()),olc::YELLOW, 3);

	return Result;
}

bool RPG_Engine::UpdateTitleScreen(float fElapsedTime)   // <---MAIN MENU Start
{
	

	//Update script 
	m_script.ProcessCommands(fElapsedTime);

	olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };
	DrawPartialDecal({ (float)0, (float)0 }, RPG_Assets::get().GetSprite("MainMenuFarLayer"), { 0,0 }, { (float)RPG_Assets::get().GetSprite("MainMenuFarLayer")->sprite->width, (float)RPG_Assets::get().GetSprite("MainMenuFarLayer")->sprite->height }, {1, 1});   //<-- Draw sprite under rectangle
	DrawPartialDecal({ (float)0, (float)0 }, RPG_Assets::get().GetSprite("MainMenuMidLayer"), { 0,0 }, { (float)RPG_Assets::get().GetSprite("MainMenuMidLayer")->sprite->width,  (float)RPG_Assets::get().GetSprite("MainMenuMidLayer")->sprite->height }, { 1,1 });
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


	DrawPartialDecal({ ScreenWidth()/2.0f+75, ScreenHeight()/2.0f}, RPG_Assets::get().GetSprite("MainMenuBardChar"), {MainMenuX * 128.0f,MainMenuY * 128.0f}, {(float)128, (float)128}, {1.4,1.4});



	DrawPartialDecal({ ScreenWidth()-250.0f, ScreenHeight()/2.0f+20}, RPG_Assets::get().GetSprite("MainMenuFirstChar"), {MainMenuX * 128.0f,MainMenuY * 128.0f}, {(float)128, (float)128}, {1.4,1.4});


	DrawPartialDecal({ScreenWidth()-750.0f, ScreenHeight()/2.0f+20}, RPG_Assets::get().GetSprite("MainMenuSecondChar"), {MainMenuX * 128.0f,MainMenuY * 128.0f}, {(float)128, (float)128}, {1.4,1.4});

	DrawPartialDecal({ ScreenWidth()-158.0f, ScreenHeight()/2.0f-100}, RPG_Assets::get().GetSprite("MainMenuWhisps"), {MainMenuX * 40.0f,MainMenuY * 13.0f}, {(float)40, (float)13}, {1.4,1.4});

	DrawPartialDecal({ ScreenWidth()/2+35.0f, ScreenHeight()/2+75.0f}, RPG_Assets::get().GetSprite("MainMenuCampFire"), {MainMenuX * 76.0f,MainMenuY * 94.5f}, {(float)76, (float)94}, {1.4,1.4});




	float spelluix, spelluiy;
	spelluix = 25.0f;
	spelluiy = ScreenHeight() - 150;

	DrawPartialDecal({ spelluix, spelluiy }, RPG_Assets::get().GetSprite("SpellUi"), { 0,0 }, { 334,142 },{0.7,1});
	//DrawBigText("Travel ?", (ScreenWidth() / 2) - 72, ScreenHeight() / 2 - 62, 1, 1, olc::WHITE);

	//DrawBigText("Continue", 50, ScreenHeight() - 250, 1.5, 1.5, olc::GREY);
	DrawBigText("New Game", spelluix+25, spelluiy +30, 1, 1, olc::WHITE);

	DrawBigText("Options", spelluix+25, spelluiy + 60, 1, 1, olc::WHITE);

	DrawBigText("Quit", spelluix+25, spelluiy +90, 1, 1, olc::WHITE);


	
	std::ofstream data;
	data.open("Load/CurrSave.txt", std::ofstream::in);
	if (data.is_open())
	{
		DrawBigText("Continue", spelluix + 25, spelluiy, 1, 1, olc::WHITE);
	}
	else
	{
		DrawBigText("Continue", spelluix + 25, spelluiy, 1, 1, olc::GREY);
	}

	if (GetMouse(0).bPressed) //LeftMouse
	{

		if ((int)mouse.x >= spelluix + 25 && (int)mouse.y >= spelluiy + 30 && (int)mouse.x <= spelluix + 25 + 208 && (int)mouse.y <= spelluiy + 59) //New Game
		{
			data.close();
			m_script.AddCommand((new cComand_HideScreen(2)));
			m_script.AddCommand((new cComand_Changemap("Forest",-1,25.5)));
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

		if ((int)mouse.x >= spelluix + 25 && (int)mouse.y >= spelluiy + 90 && (int)mouse.x <= 50 + 200 && (int)mouse.y <= spelluiy + 90 + 30) //New Game
		{
			return false;
		}

		if ((int)mouse.x >= spelluix + 25 && (int)mouse.y >= spelluiy && (int)mouse.x <= 50 + 216 && (int)mouse.y <= spelluiy + 29) //Continue
		{
			if (data.is_open())
			{
				data.close();
		//		m_script.AddCommand((new cComand_HideScreen(2)));
				m_script.AddCommand((new cComand_LoadFunction));  // in load function in start allredy 2 comands but they call not instant 
			


			}
			
		


		}
	}
	if ((int)mouse.x >= spelluix + 25 && (int)mouse.y >= spelluiy && (int)mouse.x <= 50 + 216 && (int)mouse.y <= spelluiy + 29) //Continue
	{
		DrawBigText("Continue", spelluix + 25, spelluiy, 1, 1, olc::YELLOW);
	}

	if ((int)mouse.x >= spelluix + 25 && (int)mouse.y >= spelluiy + 30 && (int)mouse.x <= spelluix + 25 + 208 && (int)mouse.y <= spelluiy + 59) //New Game
	{
		DrawBigText("New Game", spelluix + 25, spelluiy + 30, 1, 1, olc::YELLOW);
	}

	if ((int)mouse.x >= spelluix + 25 && (int)mouse.y >= spelluiy +90 && (int)mouse.x <= 50 + 200 && (int)mouse.y <= spelluiy + 90+30) //New Game
	{
		DrawBigText("Quit", spelluix + 25, spelluiy + 90, 1, 1, olc::YELLOW);
	}

	

	//Mouse
	DrawPartialDecal({ (float)(mouse.x - 0.15f) , (float)(mouse.y - 0.1f)  }, RPG_Assets::get().GetSprite("Pantir's Dagger"), { 0,0 }, { (float)64,(float)(64) }, {0.5,0.5});


	if (bSmoothAppearScreen)
	{

		DrawPartialDecal({ (float)0, (float)0 }, RPG_Assets::get().GetSprite("DescriptionPattern"), { 0,0 }, { (float)RPG_Assets::get().GetSprite("DescriptionPattern")->sprite->width, (float)RPG_Assets::get().GetSprite("DescriptionPattern")->sprite->height }, { 2,2 }, olc::Pixel{ 255,255,255,(uint8_t)AlphaAppearScreen });
	}



	// Limit the frame rate to 60 frames per second
	

		
	return true;     
}     
// <---MAIN MENU END



bool RPG_Engine::UpdateLocalMap(float fElapsedTime)
{

	olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };
	//std::cout << enemysPool.size() << std::endl;

	std::cout << m_pPlayer->px << '\t' << m_pPlayer->py << std::endl;
		
		//std::cout <<fElapsedTime << std::endl;

		if (m_pPlayer->bDead == true)
		{
		//	SaveFunction();
			m_pPlayer->bDead = false;
			m_pPlayer->nHealth = 1;
		//	m_pPlayer->SwitchLayer(2);
			m_script.AddCommand((new  cComand_LoadFunction));

	
		}

		//Update script 
		m_script.ProcessCommands(fElapsedTime);



		m_vecParticles.erase(                              //check m_vecProjectiles on flag -bredundand and erase it
			remove_if(m_vecParticles.begin(), m_vecParticles.end(),
				[](const cDynamic* d) {return((cDynamic_Projectile*)d)->bRedundant; }), m_vecParticles.end());

		m_vecProjectiles.erase(                              //check m_vecProjectiles on flag -bredundand and erase it
			remove_if(m_vecProjectiles.begin(), m_vecProjectiles.end(),
				[](const cDynamic* d) {return((cDynamic_Projectile*)d)->bRedundant; }), m_vecProjectiles.end());

		m_vecIndicators.erase(                              //check m_vecProjectiles on flag -bredundand and erase it
			remove_if(m_vecIndicators.begin(), m_vecIndicators.end(),
				[](const cDynamic* d) {return((cDynamic_HpBar*)d)->bRedundant; }), m_vecIndicators.end());



		m_vecDynamics.erase(                              //check m_vecProjectiles on flag -bredundand and erase it    hp bars the same in vecdynamics
			remove_if(m_vecDynamics.begin(), m_vecDynamics.end(),
				[](const cDynamic* d) {return((cDynamic_Creature*)d)->bDead; }), m_vecDynamics.end());


		m_vecFightText.erase(                              //check m_vecProjectiles on flag -bredundand and erase it    hp bars the same in vecdynamics
			remove_if(m_vecFightText.begin(), m_vecFightText.end(),
				[](const cDynamic* d) {return((cDynamic_Creature*)d)->bRedundant; }), m_vecFightText.end());





		//                                            DEATH MECHANIC
	

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

					if (m_pPlayer->bControllable)
					{





						if (GetKey(olc::Key::SPACE).bPressed)
						{

							if (m_pPlayer->energeyAmount >= 15)
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
						if (GetKey(olc::Key::W).bHeld)
						{

							m_pPlayer->SetVerticalDirection(1); // <--North

						}
						if (GetKey(olc::Key::S).bHeld)
						{

							m_pPlayer->SetVerticalDirection(2);  // <--South
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
								if (!m_pPlayer->bOnGround && !m_pPlayer->bOnLanded)   // air attack
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
								if (!m_pPlayer->bOnGround && !m_pPlayer->bOnLanded)   // air attack
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

								if (!m_pPlayer->bOnGround && !m_pPlayer->bOnLanded)   // air attack
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

						if (!m_pPlayer->isAttack)
						{





							m_pPlayer->SetVerticalDirection(3);

							if (GetKey(olc::Key::D).bHeld)
							{

								m_pPlayer->vx += (m_pPlayer->bOnGround ? 35.0f * m_pPlayer->GetMovement() : 25.0f * m_pPlayer->GetMovement()) * fElapsedTime;
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

								m_pPlayer->vx += (m_pPlayer->bOnGround ? -35.0f * m_pPlayer->GetMovement() : -25.0f * m_pPlayer->GetMovement()) * fElapsedTime;
							}

							if (GetMouse(0).bPressed)   // <--LeftAttack
							{
								if (!m_pPlayer->bOnGround && !m_pPlayer->bOnLanded)      // <- attack on air
								{


									if (m_pPlayer->energeyAmount >= 35 && GetLearnedTalent(1))   //EnergyAmount - Spellcount 
										//
									{
										m_pPlayer->EnergyMoveAttackAir();
									}

									float fTestX, fTestY;
									defineFacingDirection(fTestX, fTestY);
									for (auto dyns : m_vecDynamics)
									{
										if ((fTestX + 0.9f) > dyns->px + dyns->CollbordersXF && (fTestX - 0.9f) < dyns->px + dyns->CollbordersX && fTestY > dyns->py + dyns->CollbordersY && fTestY < dyns->py + dyns->CollbordersYF)
										{
											if (!dyns->bOnGround)

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
									for (auto dyns : m_vecDynamics)
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


										if (m_pPlayer->energeyAmount >= 99 && GetLearnedTalent(3))                         // <-- Attack on ground
										{
											m_pPlayer->EnergyMoveAttackHigh();

										}
										else
										{

											if (m_pPlayer->energeyAmount >= 55 && GetLearnedTalent(2))
											{
												m_pPlayer->EnergyMoveAttackMid();
											}
											else
											{

												if (m_pPlayer->energeyAmount >= 35 && GetLearnedTalent(1))   //EnergyAmount - Spellcount 

												{

													m_pPlayer->EnergyMoveAttackLow();

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
						}

						if (m_pPlayer->bOnGround && m_pPlayer->fAttackcount <= 0.5f && m_pPlayer->fAttackcount >= 0.1f)
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
								if ((0.5f - m_pPlayer->fAttackcount >= 0.17f && 0.5f - m_pPlayer->fAttackcount <= 0.32f && m_pPlayer->energeyAmount >= 55 && m_pPlayer->enumCounter != 3) && GetLearnedTalent(2)) //enumcount ==3 - ATTACKMID
								{
									m_pPlayer->EnergyMoveAttackMid();
								}
								else if ((0.5f - m_pPlayer->fAttackcount >= 0.0f && 0.5f - m_pPlayer->fAttackcount <= 0.17 && m_pPlayer->energeyAmount >= 35 && m_pPlayer->enumCounter != 4))  // enumcounter ==4 AttackEasy
								{
									m_pPlayer->EnergyMoveAttackLow();
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
		float deltaX = (m_pPlayer->px + (GetMouseX() - ScreenWidth() / 2) * mouseSnapFactor) - fCameraPosX;
		float deltaY = (m_pPlayer->py + (GetMouseY() - ScreenHeight() / 2) * mouseSnapFactor) - fCameraPosY;

		// Snap the camera towards the player and mouse position
		fCameraPosX = m_pPlayer->px + (GetMouseX() - ScreenWidth() / 2) * mouseSnapFactor;
		fCameraPosY = m_pPlayer->py -1+ (GetMouseY() - ScreenHeight() / 2) * mouseSnapFactor;

		// Apply interpolation to smooth out the movement
		fCameraPosX += deltaX * cameraSpeed;
		fCameraPosY += deltaY * cameraSpeed;
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
			for (auto& source : { &m_vecDynamics, &m_vecProjectiles })
			{
				for (auto& object : *source) // for every chars in this vector will be calculate their move
				{


					if (object->gravity )
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
								
									object->vy = 2*deltaYx + reservgravity;

									//object->vy = 

									fNewObjectPosY = object->obsticlepoints->second->y - object->CollbordersYF;

									if (!object->bOnGround)                                // <- we trying to make in OnLanded anim
									{
										object->IsLanded();
									}

									object->Jumpcounter = 0;
									object->bOnGround = true;
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


										if (!object->bOnGround)                                // <- we trying to make in OnLanded anim
									{
										object->IsLanded();
									} 

									//object->vy = deltaYx+ reservgravity;
								
									fNewObjectPosY = object->obsticlepoints->first->y - sloperatio - object->CollbordersYF;
									object->Jumpcounter = 0;
									object->bOnGround = true;
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

									object->vy = 2 * deltaYx + reservgravity;

									//object->vy = 

									fNewObjectPosY = object->obsticlepoints->first->y - object->CollbordersYF;

									if (!object->bOnGround)                                // <- we trying to make in OnLanded anim
									{
										object->IsLanded();
									}

									object->Jumpcounter = 0;
									object->bOnGround = true;
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

								

									fNewObjectPosY = object->obsticlepoints->second->y - sloperatio - object->CollbordersYF;

									if (!object->bOnGround)                                // <- we trying to make in OnLanded anim
									{
										object->IsLanded();
									}

									object->Jumpcounter = 0;
									object->bOnGround = true;
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
									fNewObjectPosY = std::round(fNewObjectPosY * 10.0f) / 10.0f;
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

									if (!object->bOnGround)                                // <- we trying to make in OnLanded anim
									{
										object->IsLanded();
									}

									object->Jumpcounter = 0;
									object->bOnGround = true;
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

									 if (!object->bOnGround)                                // <- we trying to make in OnLanded anim
									 {
										 object->IsLanded();
									 }

									 object->Jumpcounter = 0;
									 object->bOnGround = true;
									 bslope = true;




								 }
							 }




									 if(bslope != true)
									 if (m_pCurrentMap->GetColliziionIndex(object->px + object->CollbordersXF, fNewObjectPosY + object->CollbordersYF) == 1 || m_pCurrentMap->GetColliziionIndex(object->px + object->CollbordersX, fNewObjectPosY + object->CollbordersYF) == 1)  // 1 means tough collision
								{


									object->obsticlepoints = nullptr;
									if (!object->bOnGround)                                // <- we trying to make in OnLanded anim
									{
										object->IsLanded();
									}


									fNewObjectPosY = object->py;  //if there won't emprty cell we move right

									//fNewObjectPosY = std::round(fNewObjectPosY * 10.0f) / 10.0f;

									object->vy = 0;
									object->Jumpcounter = 0;
									object->bOnGround = true;

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
										object->bDead = true;

									}




								}
							
					}

				


					if (object->vy <=-0.1f &&!bslope || object->vy>=0.1f && !bslope)
						object->bOnGround = false;
					

					float fDynamicObjectPosX = fNewObjectPosX;
					float fDynamicObjectPosY = fNewObjectPosY;

					



					// Object V Object collisions






					if (m_script.bUserControlEnabled)
					{
						if (object == m_vecDynamics[0] || blockCamera)
						{

							if (fDynamicObjectPosX + object->CollbordersX < fOffsetX + 2 || fDynamicObjectPosX + object->CollbordersXF > fOffsetX + nVisibleTileX)   // here we don't let character went out from
							{
								object->vx = 0;

								fDynamicObjectPosX = object->px;

							}
						}


						for (auto& dyn : m_vecDynamics)
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
										if (object == m_vecDynamics[0] && object->bIsAttackable)
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
											if (dyn->bIsAttackable)
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
			//std::cout << m_vecVisibleDynamics.size() << std::endl;

			m_vecVisibleDynamics.clear();
			for (auto& source : { &m_vecDynamics,&m_vecProjectiles,&m_vecFightText,&m_vecParticles })  // <- run on vecDynamics array and use Update function in every person this array
				for (auto& dyns : *source)
				{
					if (dyns->px >= fCameraPosX - nVisibleTileX && dyns->px <= fCameraPosX + nVisibleTileX && dyns->py >= fCameraPosY - nVisibleTileY && dyns->py <= fCameraPosY + nVisibleTileY)
					{

						dyns->Update(fElapsedTime, m_pPlayer);

					cDynamic* entity = dyns;
					if (dynamic_cast<cDynamic_Creature*>(entity)) 
						m_vecVisibleDynamics.push_back(entity); // Add the Bandit 
					}

					if (dyns->sName != "projectile" && dyns->sName != "Pantir")
					{

					switch (dyns->m_layer)
					{
						//  m_layer 1 - enemy layer
					case 1:
						if (dyns->bDead == true)
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





			
			


			//Calculate Top-Leftmost visible tile
			 fOffsetX = fCameraPosX - (float)nVisibleTileX / 2.0f;
			 fOffsetY = fCameraPosY - (float)nVisibleTileY / 2.0f;


			if (fOffsetX < 2) fOffsetX = 2;
			if (fOffsetY < 0) fOffsetY = 0;
			if (fOffsetX > m_pCurrentMap->nWidth - nVisibleTileX - 2) fOffsetX = m_pCurrentMap->nWidth - nVisibleTileX - 2;
			if (fOffsetY > m_pCurrentMap->nHeight - nVisibleTileY) fOffsetY = m_pCurrentMap->nHeight - nVisibleTileY;

		}

			//Get offsets for smooth movement
			float fTileOffsetX = (fOffsetX - (int)fOffsetX) * 64;
			float fTileOffsetY = (fOffsetY - (int)fOffsetY) * 64;
		
				Clear(olc::BLANK);
				//SetDecalMode(olc::DecalMode::NORMAL);


				 // Update parallax layers based on the current scroll position
      //  UpdateParallax(ScrollingCurrX, scrollingbetweenX, ScrollingBackX, fOffsetX);


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


			SetDrawTarget(1);
			SetPixelMode(olc::Pixel::ALPHA);

				DrawPartialDecal({ ScrollingCurrX ,0 }, m_pCurrentMap->FarParralax, { 0,0 }, { (float)ScreenWidth(),(float)ScreenHeight() });

				DrawPartialDecal({ fx - (float)ScreenWidth(),0 }, m_pCurrentMap->FarParralax, { 0,0 }, { (float)ScreenWidth(),(float)ScreenHeight() });


				WrapCoordinates(scrollingbetweenX, scrollingbetweenX);

				WrapCoordinates(scrollingbetweenX, fx);



				DrawPartialDecal({ scrollingbetweenX ,(ScrollingY+90) - (fOffsetY *3.0f) }, m_pCurrentMap->MidParralax, { 0,0 }, { (float)ScreenWidth(),(float)ScreenHeight() });

				DrawPartialDecal({ fx - (float)ScreenWidth() , (ScrollingY+90) - (fOffsetY *3.0f) }, m_pCurrentMap->MidParralax, { 0,0 }, { (float)ScreenWidth(),(float)ScreenHeight() });



				WrapCoordinates(ScrollingBackX, ScrollingBackX);

				WrapCoordinates(ScrollingBackX, fx);



				DrawPartialDecal({ ScrollingBackX ,(ScrollingY+180) - (fOffsetY*6.0f) }, m_pCurrentMap->CloseParralax, { 0,0 }, { (float)ScreenWidth(),(float)ScreenHeight() });

				DrawPartialDecal({ fx - (float)ScreenWidth() , (ScrollingY + 180) -(fOffsetY*6.0f) }, m_pCurrentMap->CloseParralax, { 0,0 }, { (float)ScreenWidth(),(float)ScreenHeight() });

				/////////ParallaxLogic
				// 


				if (m_pCurrentMap->sName == "Forest" || m_pCurrentMap->sName == "ForesttLvlPartTwo")
				{
					for (auto& source : { &m_vecFarWeather })    // DRAW Rain
						for (auto& dyns : *source)
						{


							dyns->Update(fElapsedTime, ScreenWidth(), ScreenHeight()); // Update raindrop position

							dyns->DrawSelf(this, fOffsetX, fOffsetY);


						}
				}
				
					std::vector<TileInfo> batchFirstLayer; // (position, source rectangle) pairs // bool represent static or dynamic layer
					std::vector<TileInfo> batchSecondLayer; // (position, source rectangle) pairs // bool represent static or dynamic layer
					std::vector<TileInfo> batchThirdLayer; // (position, source rectangle) pairs // bool represent static or dynamic layer
					std::vector<TileInfo> batchDynamicLayer; // (position, source rectangle) pairs // bool represent static or dynamic layer
					std::vector<TileInfo > batchZeroCharsLayer; // (position, source rectangle) pairs // bool represent static or dynamic layer

					batchFirstLayer.reserve((nVisibleTileX + 8)* (nVisibleTileY + 8));
					batchSecondLayer.reserve((nVisibleTileX + 8)* (nVisibleTileY + 8));
					batchThirdLayer.reserve((nVisibleTileX + 8)* (nVisibleTileY + 8));
					batchZeroCharsLayer.reserve((nVisibleTileX + 8)* (nVisibleTileY + 8));

					// Iterate over visible tiles
					for (int x = -4; x < nVisibleTileX+2 ; x++) {
						for (int y = -4; y < nVisibleTileY+2 ; y++) {
							Tile idx = m_pCurrentMap->GetIndexLayer( x + fOffsetX, y + fOffsetY);  // here we need to take needed information 

							TileInfo tile;

		
						if (idx.ThirdLayer.index !=0)
							{

							tile.position.x = x;
							tile.position.y = y;
							tile.size = idx.ThirdLayer.size;
							tile.sourceRect = idx.ThirdLayer.sourcerect;
							tile.textureIndex = 4;
							tile.textureLayer = 4;
							batchThirdLayer.push_back(tile);

					

							}

							if (idx.SecondLayer.index != 0)
							{
							
								tile.position.x = x;
								tile.position.y = y;
								tile.size = idx.SecondLayer.size;
								tile.sourceRect = idx.SecondLayer.sourcerect;
								tile.textureIndex = 3;
								tile.textureLayer = 3;
								batchSecondLayer.push_back(tile);


						

							}	

							if (idx.DynamicLayer.index != 0)
							{


								tile.position.x = x;
								tile.position.y = y;
								tile.size = idx.DynamicLayer.size;
								tile.sourceRect = idx.DynamicLayer.sourcerect;
								tile.textureIndex = idx.DynamicLayer.index;  // we implement dynamic loguc on index
								tile.textureLayer = 10;
								batchDynamicLayer.push_back(tile);



							}

							if (idx.FirstLayer.index != 0)
							{
								tile.position.x = x;
								tile.position.y = y;
								tile.size = idx.FirstLayer.size;
								tile.sourceRect = idx.FirstLayer.sourcerect;
								tile.textureIndex = 1;
								tile.textureLayer = 1;
								batchFirstLayer.push_back(tile);


						
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
					
					

					for (const auto& source : { &batchThirdLayer,&batchSecondLayer,&batchDynamicLayer ,&batchFirstLayer })
						for (auto& dyns : *source)
						{
							switch (dyns.textureLayer)
							{
							default:
								DrawPartialDecal({ dyns.position.x * 64 - fTileOffsetX,dyns.position.y * 64 - fTileOffsetY }, m_pCurrentMap->pSprite, dyns.sourceRect, dyns.size);
								break;
							case 10:  // means dynamic

								dyns.textureIndex -= 10;
								
								DrawPartialDecal({ dyns.position.x * 64 - fTileOffsetX,dyns.position.y * 64 - fTileOffsetY }, m_pCurrentMap->pDynsprite, { 256.0f * frameIndex,dyns.textureIndex * 256.0f }, { 256,256 });
								break;
							}
						

							
							


						}

					SetPixelMode(olc::Pixel::NORMAL);
				EnableLayer(layer, true);
					SetDrawTarget(nullptr);



				

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



					if (bshowThird == true)
					{
						for (const auto& source : { &batchThirdLayer })
							for (auto& dyns : *source)
							{
								DrawRect({ (int)dyns.position.x * 64 - (int)fTileOffsetX,(int)dyns.position.y * 64 - (int)fTileOffsetY }, dyns.size);
							}
					}
					if (bshowSecond == true)
					{
						for (const auto& source : { &batchSecondLayer })
							for (auto& dyns : *source)
							{
								DrawRect({ (int)dyns.position.x * 64 - (int)fTileOffsetX,(int)dyns.position.y * 64 - (int)fTileOffsetY }, dyns.size);
							}
					}

					if (bshowfirst == true)
					{
						for (const auto& source : { &batchFirstLayer })
							for (auto& dyns : *source)
							{
									DrawRect({ (int)dyns.position.x * 64 - (int)fTileOffsetX,(int)dyns.position.y * 64 - (int)fTileOffsetY }, dyns.size);	
							}
					}

				

				

				if (m_pPlayer->bHideMode)
				{
				m_pPlayer->DrawSelf(this, fOffsetX, fOffsetY);
				}

				//Draw Objects
				

				for (auto& source : { &m_vecFightText,&m_vecDynamics,&m_vecProjectiles,&m_vecParticles })
					for (auto& dyns : *source)
						if (dyns->px >= fCameraPosX - nVisibleTileX && dyns->px <= fCameraPosX + nVisibleTileX && dyns->py >= fCameraPosY - nVisibleTileX && dyns->py <= fCameraPosY + nVisibleTileX)   // && m_vecDynamics[0] !=dyns
						{
							if (dyns->bDraw)
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
						SetDrawTarget(1);
						SetPixelMode(olc::Pixel::ALPHA);
					}


					for (size_t i = 0; i < positions.size(); ++i) {
					

							
						
							DrawPartialDecal({ positions[i].x * 64 - fTileOffsetX,positions[i].y * 64 - fTileOffsetY }, m_pCurrentMap->pSprite, sourceRects[i], sourceSize[i]);
							

							if (bshowZero == true)
							{
							SetPixelMode(olc::Pixel::NORMAL);
							EnableLayer(layer, true);
							SetDrawTarget(nullptr);
							DrawRect({ positions[i].x * 64 - (int)fTileOffsetX,positions[i].y * 64 - (int)fTileOffsetY }, sourceSize[i]);
							}
							

						}
				}



				
				//}

	//			if (!m_pPlayer->bHideMode)
		//		{
		//		m_pPlayer->DrawSelf(this, fOffsetX, fOffsetY);
		//		}

			

				//
				//for (auto& source : { &m_vecFightText,&m_vecDynamics,&m_vecProjectiles,&m_vecParticles }) //Draw chars Collizzion 
				//	for (auto& dyns : *source)
				//		if (dyns->px >= fCameraPosX - nVisibleTileX && dyns->px <= fCameraPosX + nVisibleTileX && dyns->py >= fCameraPosY - nVisibleTileX && dyns->py <= fCameraPosY + nVisibleTileX)   // && m_vecDynamics[0] !=dyns
				//		{
				//			

				//		


				//			DrawLine((dyns->px + dyns->CollbordersX - fOffsetX) * 64, (dyns->py + dyns->CollbordersY - fOffsetY) * 64, (dyns->px + dyns->CollbordersX - fOffsetX) * 64, (dyns->py + dyns->CollbordersYF - fOffsetY) * 64);
				//			DrawLine((dyns->px + dyns->CollbordersX - fOffsetX) * 64, (dyns->py + dyns->CollbordersY - fOffsetY) * 64, (dyns->px + dyns->CollbordersXF - fOffsetX) * 64, (dyns->py + dyns->CollbordersY - fOffsetY) * 64);
				//			DrawLine((dyns->px + dyns->CollbordersX - fOffsetX) * 64, (dyns->py + dyns->CollbordersYF - fOffsetY) * 64, (dyns->px + dyns->CollbordersXF - fOffsetX) * 64, (dyns->py + dyns->CollbordersYF - fOffsetY) * 64);
				//			DrawLine((dyns->px + dyns->CollbordersXF - fOffsetX) * 64, (dyns->py + dyns->CollbordersY - fOffsetY) * 64, (dyns->px + dyns->CollbordersXF - fOffsetX) * 64, (dyns->py + dyns->CollbordersYF - fOffsetY) * 64);



				//		}
	
						
						
				for (auto& source : { &m_vecUiBars })    // DRAW Ui elements above char 
					for (auto& dyns : *source)
						dyns->DrawSelf(this, (m_pPlayer->px-fOffsetX)*64, (m_pPlayer->py - fOffsetY+1.5) * 64);

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
				DrawPartialDecal({ (m_pPlayer->px + (1.0f*fscale) - fOffsetX) * 64,((m_pPlayer->py-0.2f) - fOffsetY) * 64 }, m_sprPressF, { 0,0 }, { 155,32 },{fscale,fscale});




				DrawPartialDecal({ (float)(mouse.x - 0.15f), (float)(mouse.y - 0.1f) }, m_sprGameCursor, { 0,0 }, { (float)64,(float)(64) },{fscale,fscale});



		
				if (bPause)
			{
				
			
					switch (n_nGameMode)
					{
					case MODE_TITLE:

						break;
					case MODE_LOCAL_MAP:
						DrawPartialDecal({ ((float)ScreenWidth() / 2) - 167, (float)ScreenHeight() / 2 - 71 }, RPG_Assets::get().GetSprite("SpellUi"), { 0,0 }, { 334,142 });
						//DrawBigText("Travel ?", (ScreenWidth() / 2) - 72, ScreenHeight() / 2 - 62, 1, 1, olc::WHITE);

						DrawBigText("Quit", (ScreenWidth() / 2) - 167, ScreenHeight() / 2 + 31, 1, 1, olc::WHITE);

						DrawBigText("Options", (ScreenWidth() / 2) + 167 - 144, ScreenHeight() / 2 + 31, 1, 1, olc::WHITE);



						


						if (GetMouse(0).bPressed) //LeftMouse
						{

							if ((int)mouse.x >= (ScreenWidth() / 2) - 167 && (int)mouse.y >= ScreenHeight() / 2 + 31 && (int)mouse.x <= (ScreenWidth() / 2) - 167 + 90 && (int)mouse.y <= ScreenHeight() / 2 + 31 + 32)
							{
								return false;
							}

						}

						DrawPartialDecal({ (float)(mouse.x - 0.15f), (float)(mouse.y - 0.1f) }, m_sprGameCursor, { 0,0 }, { (float)64,(float)(64) });

						DrawBigText("Pause", ScreenWidth() / 2 - 45, ScreenHeight() / 2 - 71, 1, 1, olc::WHITE);

					
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





				int y=0;
				for (auto& quest : m_listQusets)  // ui drawing quest right up corner on the screen
				{
					y++;
					DrawBigText(quest->sName, ScreenWidth() - 250, y * 32, 0.35, 0.35, olc::YELLOW);
					y++;
					DrawBigText(quest->sDescription, ScreenWidth() - 250, y * 32, 0.25, 0.25);
					//y -=2;
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


				// Limit the frame rate to 60 frames per second
			//	Sleep(4); // Sleep for the appropriate time to achieve desired frame rate

				return true;
			
	
}

bool RPG_Engine::UpdateWarehouse(float fElapsedTime)
{

	olc::vf2d mouse = { (float)GetMouseX(), (float)GetMouseY() };
	olc::vf2d mousefix = { (mouse.x / (64 * fscale)), (mouse.y / (64 * fscale)) };

	


	cItem* highlighted = nullptr;
	cItem* GrabItem = nullptr;

	float squeY = 8 * fscale;
	float squeX = 4 * fscale;
	const float WHsqueX = 14 * fscale;
	
	const float ReqSocketsY = (float)(128 / 64) * fscale;
	const float RqSocketsX = (float)(192 / 64) * fscale;
	
	olc::Decal* InventorySockeys = RPG_Assets::get().GetSprite("inventorySockets");

	const float invSocketsWidth = InventorySockeys->sprite->width;
	const float invSocketsHeigh = InventorySockeys->sprite->height;


	DrawPartialDecal({ (float)4 * 64*fscale, (float)(8) * 64*fscale }, InventorySockeys, { 0,0 }, { invSocketsWidth,invSocketsHeigh }, {fscale,fscale});
	DrawPartialDecal({ (float)5.85 * 64*fscale, (float)(11) * 64*fscale }, RPG_Assets::get().GetSprite("inventoryName"), { 0,0 }, { 274,75 }, { fscale,fscale });
	DrawPartialDecal({ (float)10 * 64*fscale, (float)4 * 64*fscale }, RPG_Assets::get().GetSprite("PantirEquipSlots"), { 0,0 }, { 128,256 }, { fscale,fscale });   //<-- Draw sprite under rectangle
	DrawPartialDecal({ (float)10 * 64*fscale, (float)6 * 64*fscale }, m_pPlayer->pEquipedWeapon->pSprite, { 0,0 }, { 64,64 }, { fscale,fscale }); // <--Weapon wearing
	DrawPartialDecal({ (float)4 * 64*fscale, (float)4 * 64*fscale }, RPG_Assets::get().GetSprite("StatisticShelf"), { 0,0 }, { 384,256 }, { fscale,fscale });   //<-- Draw sprite under rectangle

	
	DrawPartialDecal({ (float)14 * 64*fscale, (float)8 * 64*fscale }, InventorySockeys, { 0,0 }, { invSocketsWidth,invSocketsHeigh }, { fscale,fscale });
	DrawPartialDecal({ (float)15.85 * 64*fscale, (float)11 * 64*fscale }, RPG_Assets::get().GetSprite("WarehouseName"), { 0,0 }, { 274,75 }, { fscale,fscale });

	DrawBigText(std::to_string(m_pPlayer->GetStats(1)), (7) * 64*fscale, (6 + 0.5) * 64*fscale, 0.5*fscale, 0.5*fscale); //Agility
	DrawBigText(std::to_string(m_pPlayer->GetStats(2)), (7) * 64*fscale, (5 + 0.6) * 64*fscale, 0.5*fscale, 0.5*fscale); //Strength
	DrawBigText(std::to_string(m_pPlayer->GetStats(3)), (7) * 64*fscale, (7 + 0.3) * 64*fscale, 0.5*fscale, 0.5*fscale); //Intelegence
	DrawBigText(std::to_string(m_pPlayer->GetStats(4)), (5) * 64*fscale, 7 * 64*fscale, 0.5, 0.5); //Defence
	//DrawBigText(std::to_string(m_pPlayer->GetStats(5)), (4) * 64, (6) * 64, 0.5, 0.5); //MovementSpeed
	DrawBigText(std::to_string(m_pPlayer->GetStats(6) + m_pPlayer->pEquipedWeapon->nDamage), (5) * 64 * fscale, (6) * 64 * fscale, 0.5, 0.5); //AverageAttack
	//DrawBigText(std::to_string(m_pPlayer->GetStats(7)), (4) * 64, (8) * 64, 0.5, 0.5); //fHpRegeneration
	//DrawBigText(std::to_string(m_pPlayer->GetStats(8)), (4) * 64, (9) * 64, 0.5, 0.5); //Versality



	if ((int)mousefix.x == (10*fscale) && (int)mousefix.y == 6*fscale)
		highlighted = m_pPlayer->pEquipedWeapon;



	DrawInventory(squeX, squeY, mouse, mousefix, highlighted, GrabItem);

	//
	DrawWarehouse(WHsqueX, squeY, mouse, mousefix, highlighted, GrabItem, Money);
	
	


	//if (GetMouse(0).bReleased)
	//{
	//	if (GrabItem != nullptr)
	//	{


	//		moveteItems(mouse, squeX, squeY, GrabItem);

	//	}
	//}
	

	if (GetMouse(0).bReleased)
	{
		if (GrabItem != nullptr)
		{
			bool bswitch = false;

			if ((int)mousefix.x >= squeX/fscale && (int)mousefix.y >= squeY/fscale && (int)mousefix.x <= 11 && (int)mousefix.y <= 10) //Inventory
			{

				if (GrabItem->bInWarehouse)  //when we grabed item from warehouse
				{
				
				
					GrabItem->bInWarehouse = false;

					

					int x = (int)mousefix.x - (squeX/fscale) ;    //ere we calculate in local space in Warehouse object  /invnumber 
					int y = (int)mousefix.y - (squeY / fscale);


					std::list<cItem*>::iterator its = m_listItems.begin();
					for (auto& item : m_listItems)
					{ //       we compare grabbed item with item in inventary for switch

						++its;
						if ((item->invnumber - 1) % 8 + item->ipx ==  x && (item->invnumber - 1) / 8 + item->ipy ==  y)
						{
							bswitch = true;
							int reservex = GrabItem->ipx;
							int reservey = GrabItem->ipy;
							int reservinv = GrabItem->invnumber;


							std::list<cItem*>::iterator it = m_listWarehouseItems.begin();

							for (auto& listitem : m_listWarehouseItems)  // go in warehouse list check cross with mouse 
							{

								++it;
								if (listitem->invnumber == GrabItem->invnumber)    // here we find position in inventary our grabbed ex
								{
									//listitem->invnumber % 8 + listitem->ipx == GrabItem->invnumber % 8 + GrabItem->ipx
									//listitem->invnumber / 8 + listitem->ipy == GrabItem->invnumber / 8 + GrabItem->ipy

									m_listItems.insert(its, GrabItem);
									m_listWarehouseItems.insert(it, item);     // insert in inventary item from werehouse

									GrabItem->invnumber = item->invnumber;
									GrabItem->ipx = item->ipx;
									GrabItem->ipy = item->ipy;

									item->invnumber = reservinv;
									item->ipx = reservex;
									item->ipy = reservey;

									
									break;

								}

							}



							
							item->bInWarehouse = true;
							break;

						}


					}



					m_listWarehouseItems.erase(                              //delete object in mlist
						remove_if(m_listWarehouseItems.begin(), m_listWarehouseItems.end(),
							[](const cItem* d) {return!((cItem*)d)->bInWarehouse; }), m_listWarehouseItems.end());


					m_listItems.erase(                              //delete object in mlist
						remove_if(m_listItems.begin(), m_listItems.end(),
							[](const cItem* d) {return((cItem*)d)->bInWarehouse; }), m_listItems.end());





					if (!bswitch)
					{
						m_listItems.push_back(GrabItem);

						for (auto z : m_listWarehouseItems)
						{

							if (z->invnumber > GrabItem->invnumber)
							{
								z->ipx = ((z->invnumber - 1) % 8 + z->ipx) - ((z->invnumber - 2) % 8);   //calculate offset for every object higher invnumber than our grabed 
								z->ipy = ((z->invnumber - 1) / 8 + z->ipy) - ((z->invnumber - 2) / 8);
							}
						}

						GrabItem->invnumber = m_listItems.size();  //Calculate inv for size

						x = (int)mousefix.x -(squeX/fscale) - (GrabItem->invnumber - 1) % 8;    //ere we calculate in local space
						y = (int)mousefix.y - (squeY / fscale) - (GrabItem->invnumber - 1) / 8;

						GrabItem->ipx = x;
						GrabItem->ipy = y;
					}






				}
				else  //drop in the same slot inventary or werehaouse without switch
			{

					moveteItems(mousefix, squeX, squeY, GrabItem,m_listItems);
			}
		}

			if ((int)mousefix.x >= WHsqueX/fscale && (int)mousefix.y >= squeY/fscale && (int)mousefix.x <= 21 && (int)mousefix.y <= 10)   //Warehouse
			{


				if (!GrabItem->bInWarehouse)
				{
					
					GrabItem->bInWarehouse = true;

						
					int x = (int)mousefix.x  - (WHsqueX/fscale) ;    //calculate drop position for draged object
					int y = (int)mousefix.y - (squeY / fscale) ;

			

						std::list<cItem*>::iterator its = m_listWarehouseItems.begin();
					for (auto& item : m_listWarehouseItems)  // go in warehouse list check cross with mouse 
					{
						++its;


						if ((item->invnumber - 1) % 8 + item->ipx == x && (item->invnumber - 1) / 8 + item->ipy == y)
						{
							bswitch = true;
						
							
							int reservex = GrabItem->ipx;
							int reservey = GrabItem->ipy;
							int reservinv = GrabItem->invnumber;


							
							
													
								std::list<cItem*>::iterator it = m_listItems.begin();
							for (auto& listitem : m_listItems)  // go in warehouse list check cross with mouse 
							{

								++it;
								if (listitem->invnumber == GrabItem->invnumber)    // here we find position in inventary our grabbed ex
								{
								m_listWarehouseItems.insert(its, GrabItem);
								m_listItems.insert(it, item);     // insert in inventary item from werehouse
								GrabItem->invnumber = item->invnumber;
								GrabItem->ipx = item->ipx;
								GrabItem->ipy = item->ipy;
								item->invnumber =reservinv;
								item->ipx = reservex;
								item->ipy = reservey;

							
								break;

								}
								
							}

							
								
							GrabItem->bInWarehouse = true;
							item->bInWarehouse = false;
							break;
						
						}


					

					}

					m_listWarehouseItems.erase(                              //delete object in mlist
						remove_if(m_listWarehouseItems.begin(), m_listWarehouseItems.end(),
							[](const cItem* d) {return!((cItem*)d)->bInWarehouse; }), m_listWarehouseItems.end());


					m_listItems.erase(                              //delete object in mlist
						remove_if(m_listItems.begin(), m_listItems.end(),
							[](const cItem* d) {return((cItem*)d)->bInWarehouse; }), m_listItems.end());



					if (!bswitch)
					{
						m_listWarehouseItems.push_back(GrabItem);

						for(auto z : m_listItems)
						{

							if (z->invnumber > GrabItem->invnumber)   // recalculate all invnumbers higher than ouse switch object
							{
								z->ipx = ((z->invnumber-1 ) % 8 + z->ipx) - ((z->invnumber - 2) % 8);
								z->ipy = ((z->invnumber-1 ) / 8 + z->ipy) - ((z->invnumber - 2) / 8);
							}
						}

						GrabItem->invnumber = m_listWarehouseItems.size() ;

						x = (int)mousefix.x - (WHsqueX/fscale) - (GrabItem->invnumber - 1) % 8;    //ere we calculate in local space
						y = (int)mousefix.y -(squeY/fscale)- (GrabItem->invnumber - 1) / 8;

						GrabItem->ipx = x;
						GrabItem->ipy = y;
					}

				}
				else     // Switch in warehouse to warehouse
				{
					 
					moveteItems(mousefix, WHsqueX, squeY, GrabItem, m_listWarehouseItems);

				
				}
			}
			GrabItem->mouseGrabbed = false;

		}
	}






	if (highlighted != nullptr)
	{


		if (GetMouse(0).bPressed)
		{

	

			highlighted->mouseGrabbed = true;
			
		}







			DrawDescriptionPattern(highlighted, mouse,mousefix);

	





		if (GetKey(olc::Key::SPACE).bReleased)     // <---Must remake
		{
			// Use selected item 
			if (!highlighted->bKeyItem)
			{
				if (highlighted->OnUse(m_pPlayer))
				{
					// Item has signalled it must be consumed, so remove it
					TakeItem(highlighted);
				}
			}
			else
			{

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
	DrawPartialDecal({ (float)squeX, (float)squeY }, RPG_Assets::get().GetSprite("Coin"), { 0,0 }, { (float)64,(float)64 }, { fscale,fscale });
	//


	//Mouse
	DrawPartialDecal({ (float)(mouse.x - 0.15f), (float)(mouse.y - 0.1f) }, RPG_Assets::get().GetSprite("Pantir's Dagger"), { 0,0 }, { (float)64,(float)(64) }, { fscale,fscale });




	DrawString(0, 8, "LOCATION:", olc::WHITE, 3);
	DrawString(0, 64, m_pCurrentMap->sName, olc::WHITE, 1);


	//Mouse
	DrawPartialDecal({ (float)(mouse.x - 0.15f) , (float)(mouse.y - 0.1f) }, RPG_Assets::get().GetSprite("Pantir's Dagger"), { 0,0 }, { (float)64,(float)(64) }, { fscale,fscale });



	return true;
}

void RPG_Engine::DrawWarehouse(const float offestX, const float offsetY, olc::vf2d mouse, olc::vf2d mouseFixed, cItem*& highlighted, cItem*& Grabitem, int moneyamount)
{

	m_listWarehouseItems.erase(                              //delete  6 Coins from inventary
		remove_if(m_listWarehouseItems.begin(), m_listWarehouseItems.end(),
			[](const cItem* d) {return((cItem*)d)->Absorbed; }), m_listWarehouseItems.end());

	Money = moneyamount;
	int i = 0;
	int x = 0;
	int y = 0;

	for (auto& item : m_listWarehouseItems)
	{
		x = i % 8;

		y = i / 8;
		i++;

		//if(item->sName =="Coin")
		AddMoney(item->GoldCount);



		if (item->mouseGrabbed == false)
		{

			DrawPartialDecal({ (float)(offestX + ((x + item->ipx) * fscale)) * 64, (offsetY + ((y + item->ipy) * fscale)) * 64 }, item->pSprite, { 0,0 }, { 64,64 }, { fscale,fscale });
			item->invnumber = i;
			//item->ipy = y;


			if ((int)mouseFixed.x == ((offestX / fscale) + (x + item->ipx)) && (int)mouseFixed.y == ((offsetY / fscale) + y + item->ipy))
				highlighted = item;

		}
		else
		{
			DrawPartialDecal({ (float)(mouse.x - (32 / 2)) , (float)(mouse.y - (32 / 2)) }, item->pSprite, { 0,0 }, { 64,64 }, { fscale,fscale });
			Grabitem = item;

		}


	}
}

bool RPG_Engine::SetMouseTarget(olc::vf2d mouse)
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






bool RPG_Engine::UpdateInventory(float fElapsedTIme)
{

	//Clear({51,51,57});

	cItem* highlighted = nullptr;
	cItem* GrabItem = nullptr;

	

	const int inventoryHeight = RPG_Assets::get().GetSprite("inventory")->sprite->height;
	const int inventoryWidth = RPG_Assets::get().GetSprite("inventory")->sprite->width;
	 float Scale =fscale;
	olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };


	olc::vf2d mousefix = { (mouse.x / (64 * fscale)),(mouse.y / (64 * fscale)) };
	
	

	 float squeX = (nVisibleTileX / 3)*Scale;
	 float squeY = (nVisibleTileX / 3)*Scale;

	


	
	
	DrawPartialDecal({ (float)squeX * 64, (float)(squeY * 64)  }, RPG_Assets::get().GetSprite("inventory"), { 0,0 }, { (float)inventoryWidth,(float)inventoryHeight}, { Scale,  Scale});
	

	DrawBigText(std::to_string(m_pPlayer->GetStats(1)), (squeX + Scale + ((inventoryWidth / 64) / 2) * Scale) * 64, (squeY - 0.9 + ((inventoryHeight / 64) / 2) * Scale) * 64, Scale, Scale, olc::YELLOW); //Agility
	DrawBigText(std::to_string(m_pPlayer->GetStats(2)), (squeX + Scale + ((inventoryWidth / 64) / 2) * Scale) * 64, (squeY - 1.4 + ((inventoryHeight / 64) / 2) * Scale) * 64, Scale, Scale, olc::RED); //Strength
	DrawBigText(std::to_string(m_pPlayer->GetStats(3)), (squeX + Scale + ((inventoryWidth / 64) / 2) * Scale) * 64, (squeY - 0.4 + ((inventoryHeight / 64) / 2) * Scale) * 64, Scale, Scale,olc::BLUE); //Intelegence

	//DrawBigText(std::to_string(m_pPlayer->GetStats(5)), (4) * 64, (6) * 64, 0.5, 0.5); //MovementSpeed

	DrawBigText(std::to_string(m_pPlayer->GetStats(6) + m_pPlayer->pEquipedWeapon->nDamage), (squeX - 1.3 + ((inventoryWidth / 64) / 2) * Scale) * 64, (squeY - 1.1 + ((inventoryHeight / 64) / 2) * Scale) * 64, Scale-0.2, Scale - 0.2); //AverageAttack
	DrawBigText(std::to_string(m_pPlayer->GetStats(4)), (squeX - 1.3 + ((inventoryWidth / 64) / 2) * Scale) * 64, (squeY - 0.5 + ((inventoryHeight / 64) / 2) * Scale) * 64, Scale-0.2, Scale-0.2); //Defence

	//DrawBigText(std::to_string(m_pPlayer->GetStats(7)), (4) * 64, (8) * 64, 0.5, 0.5); //fHpRegeneration
	//DrawBigText(std::to_string(m_pPlayer->GetStats(8)), (4) * 64, (9) * 64, 0.5, 0.5); //Versality

	



	if ((int)mousefix.x == 10*fscale && (int)mousefix.y == 6*fscale)
		highlighted = m_pPlayer->pEquipedWeapon;


	const float YcellOffset = (squeY + (inventoryHeight / 64 - 4) * Scale); // <-- offset from higher point of inventory picture to cells where they drawing

	DrawInventory(squeX, YcellOffset,mouse, mousefix,highlighted,GrabItem);
	

	




	if (highlighted != nullptr)
	{


		if (GetMouse(0).bPressed)
		{
		
				//grab = true;
			//	GrabItem = highlighted;

				highlighted->mouseGrabbed = true;
				//highlighted = nullptr;
		}

		

		DrawDescriptionPattern(highlighted, mouse,mousefix);

		

		

		if (GetMouse(1).bPressed)
		{
			// Use selected item 
			if (!highlighted->bKeyItem)
			{
				if (highlighted->OnUse(m_pPlayer))
				{
					// Item has signalled it must be consumed, so remove it
					TakeItem(highlighted);
				}
			}
			
		}

	}
	
	if (GetMouse(0).bReleased)
	{
		if (GrabItem != nullptr)
		{


			moveteItems(mousefix, squeX, YcellOffset, GrabItem,m_listItems);

		}
	}
	
	// MONEY
	
	DrawBigText(std::to_string(Money), (float)(squeX + ((RPG_Assets::get().GetSprite("Coin")->sprite->width * Scale) / 64)) * 64, (float)(squeY + 0.1-((RPG_Assets::get().GetSprite("Coin")->sprite->height * Scale) / 64)) * 64, Scale, Scale, olc::YELLOW); //Money
	DrawPartialDecal({ (float)squeX*64, (float)(squeY-((RPG_Assets::get().GetSprite("Coin")->sprite->height*Scale)/64))*64 }, RPG_Assets::get().GetSprite("Coin"), { 0,0 }, { (float)64,(float)64 }, { Scale ,Scale });
	//


	

	//Mouse
	DrawPartialDecal({ (float)(mouse.x ) , (float)(mouse.y )  }, RPG_Assets::get().GetSprite("Pantir's Dagger"), { 0,0 }, { (float)64,(float)(64)}, { Scale ,Scale });



	return true;
}

void RPG_Engine::DrawInventory(float offestX, float offsetY,olc::vf2d mouse,cItem*& Highlighted )
{
	Money = 0;
	int x = 0;
	int y = 0;
	int i = 0;
	for (auto& item : m_listItems)
	{
		x = i % 8;

		y = i / 8;
		i++;
		//if(item->sName =="Coin")
		AddMoney(item->GoldCount);



		DrawPartialDecal({ (float)(offestX + ((x + item->ipx) * fscale)) * 64, (offsetY + (((float)y + item->ipy) * fscale)) * 64 }, item->pSprite, { 0,0 }, { 64,64 }, { fscale,fscale });
		item->invnumber = i;
		//item->ipy = y;


		if ((int)mouse.x == (offestX + (x + item->ipx) * fscale) && (int)mouse.y == offsetY + ((y + item->ipy) * fscale))
			Highlighted = item;


	}
}

void RPG_Engine::DrawInventory(float offestX, float offsetY, olc::vf2d mouse, olc::vf2d mouseFixed, cItem*& highlighted, cItem*& Grabitem)
{
	m_listItems.erase(                              //delete  6 Coins from inventary
		remove_if(m_listItems.begin(), m_listItems.end(),
			[](const cItem* d) {return((cItem*)d)->Absorbed; }), m_listItems.end());

	Money = 0;
	int i = 0;
	int x = 0;
	int y = 0;
	
	for (auto& item : m_listItems)
	{
		x = i % 8;

		y = i / 8;
		i++;

		//if(item->sName =="Coin")
		AddMoney(item->GoldCount);



		if (item->mouseGrabbed == false)
		{

			DrawPartialDecal({ (float)(offestX + ((x + item->ipx) * fscale)) * 64, (offsetY  + ((y + item->ipy) * fscale)) * 64 }, item->pSprite, { 0,0 }, { 64,64 }, { fscale,fscale });
			item->invnumber = i;
			//item->ipy = y;

			

			if ((int)mouseFixed.x == ((offestX / fscale) + (x + item->ipx)) && (int)mouseFixed.y == ((offsetY / fscale) + y + item->ipy))
				highlighted = item;
			
		}
		else
		{
			DrawPartialDecal({ (float)(mouse.x - (32 / 2)) , (float)(mouse.y - (32 / 2)) }, item->pSprite, { 0,0 }, { 64,64 }, { fscale,fscale });
			Grabitem = item;

		}


	}
}

void RPG_Engine::moveteItems(olc::vf2d mouse, float x, float y, cItem*& GrabItem,std::list<cItem*> vector)
{
	const int cellsCoordX = (int)mouse.x - (x / fscale);     // Coordinates x and y where first cell begins
	const int cellsCoordY = (int)mouse.y - (y / fscale);

	bool bswitch = false;


	if (cellsCoordX >= 0 && cellsCoordY >= 0 && cellsCoordX <= 7 && cellsCoordY <= 2)
	{


		int x = cellsCoordX - ((GrabItem->invnumber - 1) % 8);    //ere we calculate in local space
		int y = cellsCoordY - ((GrabItem->invnumber - 1) / 8);




		for (auto& item : vector)
		{
			if ((item->invnumber - 1) % 8 + item->ipx == (GrabItem->invnumber - 1) % 8 + x && (item->invnumber - 1) / 8 + item->ipy == (GrabItem->invnumber - 1) / 8 + y)
			{
				bswitch = true;

				int reservex = ((GrabItem->invnumber - 1) % 8 + GrabItem->ipx) - ((item->invnumber - 1) % 8);
				int reservey = ((GrabItem->invnumber - 1) / 8 + GrabItem->ipy) - ((item->invnumber - 1) / 8);



				GrabItem->ipx = ((item->invnumber - 1) % 8 + item->ipx) - ((GrabItem->invnumber - 1) % 8);
				GrabItem->ipy = ((item->invnumber - 1) / 8 + item->ipy) - ((GrabItem->invnumber - 1) / 8);
				item->ipx = reservex;
				item->ipy = reservey;

			}


		}
		if (!bswitch)
		{
			GrabItem->ipx = x;
			GrabItem->ipy = y;
		}
	}


	GrabItem->mouseGrabbed = false;

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


bool RPG_Engine::UpdateShop(float fElapsedTime)                                                                                                       // < ----SHOP
{
	olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };


	olc::vf2d mousefix = { (mouse.x / (64 * fscale)),(mouse.y / (64 * fscale)) };


	
	olc::Decal* inventory = RPG_Assets::get().GetSprite("inventorySockets");
	

	DrawPartialDecal({ (float)4 * 64*fscale, (float)(8) * 64*fscale }, inventory, { 0,0 }, { (float)inventory->sprite->width,(float)inventory->sprite->height },{fscale,fscale});

	DrawPartialDecal({ (float)5.85 * 64*fscale, (float)(11) * 64*fscale }, RPG_Assets::get().GetSprite("inventoryName"), { 0,0 }, { 274,75 },{fscale,fscale});


	DrawPartialDecal({ (float)14 * 64*fscale, (float)(8) * 64*fscale }, inventory, { 0,0 }, { (float)inventory->sprite->width,(float)inventory->sprite->height },{fscale,fscale});
	DrawPartialDecal({ (float)15.85 * 64*fscale, (float)(11) * 64*fscale }, RPG_Assets::get().GetSprite("StoreName"), { 0,0 }, { 274,75 },{fscale,fscale});




	
	
	cItem* highlighted = nullptr;
	cItem* GrabItem = nullptr;

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

			highlighted->mouseGrabbed = true;
			//highlighted = nullptr;
		}


		DrawDescriptionPattern(highlighted, mouse,mousefix);
		
		std::cout << (int)mousefix.x << '\t' << (int)mousefix.y << '\t' << squeX / fscale << '\t' << squeX / fscale + 8 << '\t' << squeY / fscale <<'\t' << squeY / fscale + 3 << std::endl;

			if (GetMouse(1).bPressed)
		{
				if ((int)mousefix.x >= squeX/fscale && (int)mousefix.x <=squeX/fscale+8 && (int)mousefix.y  >= squeY/fscale && (int)mousefix.y <= squeY/fscale+3)  // Inventory decription
				{

					// Use selected item 
					if (!highlighted->bKeyItem)        //Try Sell Item
					{


						if (highlighted->OnSell())
						{
							// Item has signalled it must be consumed, so remove it
							TakeItem(highlighted);
						}
					}
				}

				if ((int)mousefix.x >= storesqueX/fscale && (int)mousefix.x <= storesqueX/fscale+8 && (int)mousefix.y  >= squeY/fscale && (int)mousefix.y <= squeY/fscale+3)  // Store decription
				{
					// Use selected item 
					if (!highlighted->bKeyItem)        //Try Sell Item
					{


						if (highlighted->OnBuy())     // must cheking on amount money ant consume them
						{
							TakeItem(highlighted->sTradeBuyPrice);
						}
					}
				}

		}
			
			

	}
	// MONEY
	squeX = 4 * 64*fscale;
	squeY = 7* 64*fscale;
	DrawBigText(std::to_string(Money), squeX + (64*fscale), squeY + (16*fscale), fscale, fscale, olc::YELLOW); //Money
	DrawPartialDecal({ (float)squeX, (float)squeY }, RPG_Assets::get().GetSprite("Coin"), { 0,0 }, { (float)64,(float)64 },{fscale,fscale});
	//


//Mouse
	DrawPartialDecal({ (float)(mouse.x - 0.15f) , (float)(mouse.y - 0.1f)  }, RPG_Assets::get().GetSprite("Pantir's Dagger"), { 0,0 }, { (float)64,(float)(64) },{fscale,fscale});





	return true;
}

void RPG_Engine::DrawStoreInventory(float sX, float sY, olc::vf2d mousefix, cItem*& highlighted)
{
	int x = 0;
	int y = 0;
	int i = 0;

	for (auto& item : m_listStoreItems)
	{
		x = i % 8;

		y = i / 8;
		i++;

		DrawPartialDecal({ (float)(sX + ((x + item->ipx) * fscale)) * 64, (sY + ((y + item->ipy) * fscale)) * 64 }, item->pSprite, { 0,0 }, { 64,64 }, { fscale,fscale });
		item->invnumber = i;
		//item->ipy = y;



		if ((int)mousefix.x == ((sX / fscale) + (x + item->ipx)) && (int)mousefix.y == ((sY / fscale) + y + item->ipy))
			highlighted = item;
	}
}


bool RPG_Engine::UpdateProfession(float fElapsedTime)   //<--profession
{

	olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };

	const float TalentPlateWidth = RPG_Assets::get().GetSprite("Talent Plate")->sprite->width;
	const float TalentPlateHeigh = RPG_Assets::get().GetSprite("Talent Plate")->sprite->height;

	olc::Decal* HrzStickEmptry = RPG_Assets::get().GetSprite("Horizontal StickEmpty");
	olc::Decal* HrzStickFull = RPG_Assets::get().GetSprite("Horizontal StickFill");


	olc::Decal* VertStickEmptry = RPG_Assets::get().GetSprite("Vertical StickEmpty");
	olc::Decal* VertStickFull = RPG_Assets::get().GetSprite("Vertical StickFill");


	olc::Decal* SelectedObject = RPG_Assets::get().GetSprite("SelectedObject");

	float BaseposX = (float)(ScreenWidth() / 2 - ((TalentPlateWidth / 2)*fscale));
	float BasePosY = (float)(ScreenHeight() / 2 - ((TalentPlateHeigh / 2)*fscale));

	

	DrawPartialDecal({ BaseposX,  BasePosY }, RPG_Assets::get().GetSprite("Talent Plate"), {0,0}, { TalentPlateWidth,TalentPlateHeigh },{fscale,fscale});




	/////////  STICK DRAWING /////  START

	//Low-Mid Attack
	DrawPartialDecal({ BaseposX + (222*fscale),  BasePosY + (576*fscale) }, HrzStickEmptry, { 0,0 }, { (float)4,(float)74 }, { fscale,fscale });

	if(GetLearnedTalent(2)) //  <-- MidAttack 
	{
	DrawPartialDecal({ BaseposX + (222*fscale),  BasePosY + (576*fscale) }, VertStickFull, { 0,0 }, { (float)4,(float)74 }, { fscale,fscale });

	//Mid-High Attack
	DrawPartialDecal({ BaseposX + (126*fscale),  BasePosY + (542*fscale) }, HrzStickEmptry, { 0,0 }, { (float)74,(float)4 }, { fscale,fscale });
	}
	if(GetLearnedTalent(3))
	DrawPartialDecal({ BaseposX + (126*fscale),  BasePosY + (542*fscale) }, VertStickFull, { 0,0 }, { (float)74,(float)4 }, { fscale,fscale });


	//BackStab-ShadowStab Attack
	if (GetLearnedTalent(4))
	{

		DrawPartialDecal({ BaseposX + (31*fscale),  BasePosY + (670*fscale) }, HrzStickEmptry, { 0,0 }, { (float)74,(float)4 }, { fscale,fscale });
		DrawPartialDecal({ BaseposX + (31*fscale),  BasePosY + (414*fscale) }, HrzStickEmptry, { 0,0 }, { (float)74,(float)4 }, { fscale,fscale });
		DrawPartialDecal({ BaseposX + (30*fscale),  BasePosY + (415*fscale) }, VertStickEmptry, { 0,0 }, { (float)4,(float)257 }, { fscale,fscale });

	}
	if (GetLearnedTalent(5))
	{

	DrawPartialDecal({ BaseposX + (31*fscale),  BasePosY + (670*fscale) }, VertStickFull, { 0,0 }, { (float)74,(float)4 }, { fscale,fscale });
	DrawPartialDecal({ BaseposX + (31*fscale),  BasePosY +(414*fscale) }, VertStickFull, { 0,0 }, { (float)74,(float)4 }, { fscale,fscale });
	DrawPartialDecal({ BaseposX + (30*fscale),  BasePosY + (415*fscale) }, VertStickFull, { 0,0 }, { (float)4,(float)257 }, { fscale,fscale });
	DrawPartialDecal({ BaseposX + (95*fscale),  BasePosY + (251*fscale) }, VertStickEmptry, { 0,0 }, { (float)4,(float)138 }, { fscale,fscale });
	}

	if (GetLearnedTalent(6))
	{
		DrawPartialDecal({ BaseposX + (95*fscale),  BasePosY + (251*fscale) }, VertStickFull, { 0,0 }, { (float)4,(float)138 }, { fscale,fscale });
	}


	//RighAttack-RightAirAttack
	DrawPartialDecal({ BaseposX + (351*fscale),  BasePosY + (572*fscale) }, VertStickEmptry, { 0,0 }, { (float)4,(float)74 }, { fscale,fscale });
	if (GetLearnedTalent(12))
	{

	DrawPartialDecal({ BaseposX + (351*fscale),  BasePosY + (572*fscale) }, VertStickFull, { 0,0 }, { (float)4,(float)74 }, { fscale,fscale }, olc::Pixel(255, 0, 0));   //offset pulling player back into the screen
	
	DrawPartialDecal({ BaseposX + (351*fscale),  BasePosY +(315*fscale) }, VertStickEmptry, { 0,0 }, { (float)4,(float)202 }, { fscale,fscale }, olc::Pixel(255, 0, 0));   //offset pulling player back into the screen


	}

	//Flight//Landing
	if (GetLearnedTalent(13))
	{
	DrawPartialDecal({ BaseposX + (351*fscale),  BasePosY + (315*fscale) }, VertStickFull, { 0,0 }, { (float)4,(float)202 }, { fscale,fscale }, olc::Pixel(255, 0, 0));   //offset pulling player back into the screen

	DrawPartialDecal({ BaseposX + (351*fscale),  BasePosY + (123*fscale) }, VertStickEmptry, { 0,0 }, { (float)4,(float)138 }, { fscale,fscale }, olc::Pixel(255, 0, 0));   //offset pulling player back into the screen
	}
	if (GetLearnedTalent(14))
	DrawPartialDecal({ BaseposX + (351*fscale),  BasePosY + (123*fscale) }, VertStickFull, { 0,0 }, { (float)4,(float)138 }, { fscale,fscale }, olc::Pixel(255, 0, 0));   //offset pulling player back into the screen
	


	/////////  STICK DRAWING /////  END
	


	for (auto& source : { &m_vecTalentsBars })    // DRAW Ui elements
		for (auto& dyns : *source)
			dyns->DrawSelfTalent(this, BaseposX , BasePosY );



	




				/// SELECT RECTANGLE START
	if (mouse.x >= BaseposX + (192*fscale) && mouse.x <= BaseposX + ((192 + 64)*fscale) && mouse.y >= BasePosY + (512*fscale) && mouse.y <= BasePosY + ((512 + 64)*fscale))    // <-- Mid
	{
		DrawPartialDecal({ BaseposX + (192*fscale), BasePosY + (512*fscale) }, SelectedObject, { 0,0 }, { (float)64,(float)(64) }, { fscale,fscale });
	}

	if (mouse.x >= BaseposX + (64*fscale) && mouse.x <= BaseposX + ((64 + 64)*fscale) && mouse.y >= BasePosY + (512*fscale) && mouse.y <= BasePosY + ((512 + 64)*fscale))    // <-- High
	{
		DrawPartialDecal({ BaseposX + (64*fscale), BasePosY + (512*fscale) }, SelectedObject, { 0,0 }, { (float)64,(float)(64) }, { fscale,fscale });
	}

	if (mouse.x >= BaseposX + (64*fscale) && mouse.x <= BaseposX + ((64 + 64)*fscale) && mouse.y >= BasePosY + (640*fscale) && mouse.y <= BasePosY + ((640 + 64)*fscale))    // <-- Backstab
	{
		DrawPartialDecal({ BaseposX + (64*fscale), BasePosY + (640*fscale) }, SelectedObject, { 0,0 }, { (float)64,(float)(64) }, { fscale,fscale });
	}

	if (mouse.x >= BaseposX + (64*fscale) && mouse.x <= BaseposX + ((64 + 64)*fscale) && mouse.y >= BasePosY + (384*fscale) && mouse.y <= BasePosY + ((384 + 64)*fscale))    // <-- ShadowStap
	{
		DrawPartialDecal({ BaseposX + (64*fscale), BasePosY + (384*fscale) }, SelectedObject, { 0,0 }, { (float)64,(float)(64) }, { fscale,fscale });
	}

	if (mouse.x >= BaseposX + (64*fscale) && mouse.x <= BaseposX + ((64 + 64)*fscale) && mouse.y >= BasePosY + (192*fscale) && mouse.y <= BasePosY + ((192 + 64)*fscale))    // <-- Swirl
	{
		DrawPartialDecal({ BaseposX + (64*fscale), BasePosY + (192*fscale) }, SelectedObject, { 0,0 }, { (float)64,(float)(64) }, { fscale,fscale });
	}

	if (mouse.x >= BaseposX + (192*fscale) && mouse.x <= BaseposX + ((192 + 64)*fscale) && mouse.y >= BasePosY + ((384)*fscale) && mouse.y <= BasePosY + ((384 + 64)*fscale))    // <-- Vanish
	{
		DrawPartialDecal({ BaseposX + (192*fscale), BasePosY + (384*fscale) }, SelectedObject, { 0,0 }, { (float)64,(float)(64) }, { fscale,fscale });
	}


	if (mouse.x >= BaseposX + (320*fscale) && mouse.x <= BaseposX + ((320 + 64)*fscale) && mouse.y >= BasePosY + (512*fscale) && mouse.y <= BasePosY + ((512 + 64)*fscale))    // <-- RightAir
	{
		DrawPartialDecal({ BaseposX + (320*fscale),  BasePosY + (512*fscale) }, SelectedObject, { 0,0 }, { (float)64,(float)(64) }, { fscale,fscale });
	}


	if (mouse.x >= BaseposX + (320*fscale) && mouse.x <= BaseposX + ((320 + 64)*fscale) && mouse.y >= BasePosY + (256*fscale) && mouse.y <= BasePosY + ((256 + 64)*fscale))    // <-- RightFLIGHTUP
	{
		DrawPartialDecal({ BaseposX + (320*fscale), BasePosY + (256*fscale) }, SelectedObject, { 0,0 }, { (float)64,(float)(64) }, { fscale,fscale });
	}

	if (mouse.x >= BaseposX + (320*fscale) && mouse.x <= BaseposX + ((320 + 64)*fscale) && mouse.y >= BasePosY + (64*fscale) && mouse.y <= BasePosY + ((64 + 64)*fscale))    // <-- RightATTACK LANDING
	{
	DrawPartialDecal({ BaseposX + (320*fscale),  BasePosY + (64*fscale) }, SelectedObject, { 0,0 }, { (float)64,(float)(64) },{fscale,fscale});
		
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



	//Mouse
	DrawPartialDecal({ (float)(mouse.x - 0.15f), (float)(mouse.y - 0.1f) }, RPG_Assets::get().GetSprite("Pantir's Dagger"), { 0,0 }, { (float)64,(float)(64) },{fscale,fscale});




	return true;
}


bool RPG_Engine::UpdateBlackSmith(float fElapsedTime)
{

	
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


	cItem* highlighted = nullptr;
	cItem* Selected = nullptr;

	cItem* GrabItem = nullptr;


	

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

				highlighted->objectselected = true;  // Here mousegrabed works in two dif case. in our invenotry we grabbed in blacksmith we craft
				Selected = highlighted;
			}
			else
			{
				if (Selected != nullptr)
				{
					if (GetMouse(1).bPressed)
						Selected->objectselected = false;

					if (mouse.x > (CraftBottomX / (64 * fscale)) * fscale && mouse.x <= (int)(CraftBottomX / (64 * fscale)) * fscale + 1 && mouse.y >= squeY - ReqSocketsY + (1 * fscale) && mouse.y <= squeY - ReqSocketsY + (1.5 * fscale))  // Craft Word description
					{



						// Use selected item 
						if (!Selected->bKeyItem)        //Try Sell Item
						{


							if (Selected->OnCraft(1))     // must cheking on objects on onventory ant consume them
							{

								TakeItem(Selected->sCraftRequaries);

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
			if (!highlighted->bKeyItem)
			{
				if (highlighted->OnUse(m_pPlayer))
				{
					// Item has signalled it must be consumed, so remove it
					TakeItem(highlighted);
				}
			}


		if (GetMouse(0).bPressed)
			{



				highlighted->mouseGrabbed = true;
			
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

		if (Selected->OnCraft(0))
		{
			DrawBigText("Craft", CraftBottomX, CraftBottomY, fscale, fscale, { 169,223,227 });

		}
		else
		{
			DrawBigText("Craft", CraftBottomX, CraftBottomY, fscale, fscale, olc::GREY);
		}

		DrawPartialDecal({ (SmithsqueX + ((Selected->invnumber % 8)) * fscale) * 64, (float)(squeY + ((Selected->invnumber / 8) * fscale)) * 64 }, RPG_Assets::get().GetSprite("SelectedObject"), { 0,0 }, { (float)64,(float)(64) }, { fscale,fscale });
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

void RPG_Engine::DrawBlacksmithInentory(float offestX, float offsetY, olc::vf2d mouse, cItem*& Selected,cItem*& Highlighted)
{
	int i = 0;
	int x = 0;
	int y = 0;
	

	for (auto& item : m_listBlackSmithItems)
	{
		x = i % 8;

		y = i / 8;
		i++;


		DrawPartialDecal({ (float)(offestX + (x + item->ipx) * fscale) * 64, (offsetY + ((float)y + item->ipy) * fscale) * 64 }, item->pSprite, { 0,0 }, { 64,64 }, { fscale,fscale });

		if (item->objectselected == true)  // item has boolean so thank's that we can find selected object
			Selected = item;


		if ((int)mouse.x == ((offestX / fscale) + (x + item->ipx)) && (int)mouse.y == ((offsetY / fscale) + y + item->ipy))
			Highlighted= item;
	}

}



	

void RPG_Engine::DrawCraftedRequires(float offestX, float offsetY, olc::vf2d mouse,  cItem*& Selected, cItem*& Highlighted)
{
	std::string ItemName;
	std::string ItemAmount;

	int counter = 0;
	int domain = 0;
	int x = 0;
	int y = 0;
	int i = 0;

	for (auto c : Selected->sCraftRequaries)
	{

		if (c == 10)    //<---  \n  mean end first object
		{

			x = i % 2;

			y = i / 2;

			i++;
			DrawPartialDecal({ (float)(offestX + (x * fscale)) * 64, (float)(offsetY + (y * fscale)) * 64 }, RPG_Assets::get().GetItem(ItemName)->pSprite, { 0,0 }, { 192,128 }, { fscale,fscale });


			if (mouse.x == (offestX + x ) && mouse.y == (offsetY + y ))
				Highlighted = RPG_Assets::get().GetItem(ItemName);


			for (auto& item : m_listItems)   // Count in our inventory this item
			{
				if (item->sName == ItemName)
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


void RPG_Engine::DrawDescriptionPattern(cItem* highlighted, olc::vf2d mouse, olc::vf2d mousefix)
{

	
		float nLinesX = 0;
		int nLinesY = 0;
		float nsaveline = 0;
		float nsaveLine2 = 0;
		for (auto c : highlighted->sDescription)
		{

			if (c == 10)  // <-- 10 is /n
			{
				if (nsaveLine2 <= nsaveline)
				{
					nLinesX = nsaveline - (((nsaveline / 32) - 1) * 14);
					nsaveLine2 = nsaveline;
					nsaveline = 0;
				}
				nLinesY++;
				continue;
			}

			nsaveline += 32.0f;     //  32/100 = 0.32-> 1 percent.   32-18 = 14   14/0,32 = 0.43   1-0.43 = 0.57    
		}
		if (nLinesX < 27 * 19)  // fill @press space to use like default value
		{
			nLinesX = 27 * 19;
		}
		const float mouseoffset = (64 * fscale);

		DrawPartialDecal({ (float)(mouse.x + mouseoffset) , (float)(mouse.y + mouseoffset) }, RPG_Assets::get().GetSprite("DescriptionPattern"), { 0,0 }, { (float)nLinesX,(float)(nLinesY +6) * (64*fscale) }, { fscale,fscale });


		//DrawString(4 * 64, squeY - 64, highlighted->sName, olc::WHITE,3);
		DrawBigText(highlighted->sName, (mouse.x + mouseoffset), mouse.y + mouseoffset, fscale, fscale);

		//DrawString(8, 190, "SELECTED", olc::WHITE,3);
		//DrawString(4*64, squeY-32, highlighted->sDescription, olc::WHITE,2);

		DrawBigText(highlighted->sDescription, (mouse.x + mouseoffset), mouse.y + (32*fscale) + mouseoffset, fscale, fscale, { 169,223,227 });

		




		switch (n_nGameMode)
		{
		case MODE_TITLE:
			break;
		case MODE_LOCAL_MAP:
			break;
		case MODE_WAREHOUSE:
		case MODE_BLACKSMITH:
		case MODE_INVENTORY:
			// Warehouse
			if (!highlighted->bKeyItem)
			{
				//DrawString(80, squeY-160, "(Press SPACE to use)", olc::WHITE,2);
				DrawBigText("(Press Right Button to use)", (mouse.x + (64 * fscale)), (mouse.y + ((nLinesY + 7)*fscale) * (64*fscale)), fscale - 0.1, fscale - 0.1, { 218,14,0,255 });
			}
			else
			{
				DrawBigText("(Can't Use)", (mouse.x + (64 * fscale)), (mouse.y + ((nLinesY + 7)*fscale) * (64*fscale)) , fscale - 0.1, fscale - 0.1, { 218,14,0,255 });
			}
			//
			break;
		case MODE_SHOP:
			//shop
			if ((int)mousefix.x >= 4  && (int)mousefix.x <= 4 + 8 && (int)mousefix.y >= 8  && (int)mousefix.y <= 8 + 3)  // Inventory decription
			{
				if (!highlighted->bKeyItem)
				{
					//DrawString(80, squeY-160, "(Press SPACE to use)", olc::WHITE,2);
					DrawBigText("(Press Right Button to use)", (mouse.x + (64 * fscale)), (mouse.y + ((nLinesY + 7) * fscale) * (64 * fscale)), fscale - 0.1, fscale - 0.1, { 218,14,0,255 });
					DrawBigText("Price " + std::to_string(highlighted->sTradePrice), (mouse.x + (64 * fscale)), mouse.y, fscale, fscale, olc::YELLOW); //Money
				}
				else
				{
					DrawBigText("(Can't Use)", (mouse.x + (64 * fscale)), (mouse.y + ((nLinesY + 7) * fscale) * (64 * fscale)), fscale - 0.1, fscale - 0.1, { 218,14,0,255 });
				}
			}

			if ((int)mousefix.x >= 14  && (int)mousefix.x <= 14  + 8 && (int)mousefix.y >= 8 && (int)mousefix.y <= 8  + 3)  // Store decription
			{


				//DrawString(80, squeY-160, "(Press SPACE to use)", olc::WHITE,2);
				DrawBigText("(Press Right Button to Buy)", (mouse.x + (64 * fscale)), (mouse.y + ((nLinesY + 7) * fscale) * (64 * fscale)), 0.9 * fscale, 0.9 * fscale, { 218,14,0,255 });
				DrawBigText("Price " + std::to_string(highlighted->sTradeBuyPrice), (mouse.x + (64 * fscale)), mouse.y , fscale, fscale, olc::YELLOW); //Money


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
		
	

		ScrollingCurrX = fCameraPosX;

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
	m_listBlackSmithItems.clear();
	m_listStoreItems.clear();
	
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

bool RPG_Engine::GiveItem(cItem* item)
{

	

	//GrabItem->ipx = ((item->invnumber - 1) % 8 + item->ipx) - ((GrabItem->invnumber - 1) % 8);

	
	item->invnumber = m_listItems.size()+1;
	
	


				for (int z = 1; z <24; )   // we start cicle from first slot
				{
					int zx = (z-1) % 8;    // transfer index in world coordinate
					int zy = (z-1) / 8;
					bool Breserved = false;

					for (auto s : GetListItem())    //start cheking every slot on reserve
					{

						int sx = (s->invnumber - 1) % 8 + s->ipx;   // <--Transfer in world coordinate offset
						int sy = (s->invnumber - 1) / 8 + s->ipy;

						if (sx ==zx && sy==zy )   // situation when the slot reserved we add index on next slot
						{
							Breserved = true;
							z++;
							break;
						}
					

					} // when we go through all list and didn't find reserv slot

					if (Breserved ==false)
					{
						item->ipx = zx-((item->invnumber - 1) % 8);
						item->ipy = zy-((item->invnumber-1) / 8);
						break;
					}
				
					

				}



	m_listItems.push_back(item);   //push the pointer 



	return false;
}


bool RPG_Engine::ByeItem(cItem* item)
{

	item->invnumber = m_listItems.size() + 1;

	for (int z = 1; z < 24; )   // we start cicle from first slot
	{
		int zx = (z - 1) % 8;    // transfer index in world coordinate
		int zy = (z - 1) / 8;
		bool Breserved = false;

		for (auto s : m_listItems)    //start cheking every slot on reserve
		{

			int sx = (s->invnumber - 1) % 8 + s->ipx;   // <--Transfer in world coordinate offset
			int sy = (s->invnumber - 1) / 8 + s->ipy;

			if (sx == zx && sy == zy)   // situation when the slot reserved we add index on next slot
			{
				Breserved = true;
				z++;
				break;
			}


		} // when we go through all list and didn't find reserv slot

		if (Breserved == false)
		{
			item->ipx = zx - ((item->invnumber - 1) % 8);
			item->ipy = zy - ((item->invnumber - 1) / 8);
			break;
		}

	}

	m_listItems.push_back(item);   //push the pointer 

	return false;
}



bool RPG_Engine::GiveWarehouseItem(cItem* item)
{
	item->bInWarehouse = true;
	m_listWarehouseItems.push_back(item);   //push the pointer 



	return false;
}

bool RPG_Engine::GiveNewText(cDynamic_TextDamage* Text)
{
	if (Text !=nullptr)
	{
		m_vecFightText.push_back(Text);
		return true;
	}
	return false;
}
bool RPG_Engine::GiveStoreItem(cItem* item)
{
	//item->bInWarehouse = true;
	m_listStoreItems.push_back(item);   //push the pointer 



	return false;
}

bool RPG_Engine::GiveBlackSmithItem(cItem* item)
{
	
	//item->bInWarehouse = true;
	m_listBlackSmithItems.push_back(item);   //push the pointer 
	


	return false;
}


bool RPG_Engine::TakeItem(cItem* item)
{
	if (item != nullptr)
	{

		int counter = 0;

		for (auto& citem : GetListItem())  // go in inventory list check cross with mouse 
		{




			if (citem->invnumber > item->invnumber)  // invenotry ==werehouse
			{
				
				citem->ipx = ((citem->invnumber-1) % 8 + citem->ipx) - ((citem->invnumber - 2) % 8);
				citem->ipy = ((citem->invnumber-1) / 8 + citem->ipy) - ((citem->invnumber - 2) / 8);

			}
		

		}



		m_listItems.erase(find(m_listItems.begin(), m_listItems.end(), item));
		return true;
	}
	else

		return false;
}
bool RPG_Engine::TakeItem(int price) //for price
{
	
	int helpPrice = price;
	int counter = 0;

	for (auto& item :GetListItem())  // go in list check cross with mouse 
	{


		


		if (item->GoldCount >0 && helpPrice != 0)    //First find object that has money
		{
			

			if (item->GoldCount >helpPrice) // if in wallet more money than price of object   6 >5
			{
				item->GoldCount -= helpPrice;
				helpPrice = 0;

				item->sDescription = "Keep 6 golds:  " + std::to_string(item->GoldCount) + "/6";

			}
			else if (item->GoldCount<=helpPrice) // if in wallet less money than price
			{
				helpPrice -= item->GoldCount;   // we  substract from price amount of gold in wallet

				if (item->sName == "Coin")
				{
				item->Absorbed = true;
				counter++;
				}
				else
				{
					item->GoldCount =0;
					item->pSprite = RPG_Assets::get().GetSprite("PocketLowEmpty");
					item->sDescription = "Keep 6 golds:  " + std::to_string(item->GoldCount) + "/6";
					
				}
			}
			
			

				//this->GolcCount++;
				//sDescription = "Keep 6 golds:  " + std::to_string(GolcCount);
			//	pSprite = RPG_Assets::get().GetSprite("PocketLowFull");

			


		}
		else
		{
			item->ipx = ((item->invnumber-1) % 8 + item->ipx) - ((item->invnumber - counter-1) % 8);
			item->ipy = ((item->invnumber-1) / 8 + item->ipy) - ((item->invnumber - counter-1) / 8);
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
				if (item->sName == ItemName && Reqcount>0) //we find 
				{
					squeX++;
					item->Absorbed = true;   // Delete it
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

	for (auto& item : m_listItems)   // go through inventory and find equal this name 
	{
		if (!item->Absorbed)
		{
			item->ipx = ((item->invnumber - 1) % 8 + item->ipx) - ((item->invnumber - squeX - 1) % 8);
			item->ipy = ((item->invnumber - 1) / 8 + item->ipy) - ((item->invnumber - squeX - 1) / 8);
		}
	}

	return true;
}


bool RPG_Engine::HasItem(cItem* item)
{
	if (item != nullptr)

		for (auto& items : m_listItems)
		{
			if (items->sName == item->sName )
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
		entity->binitialized = true;
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

void RPG_Engine::Attack(cDynamic_Creature* aggressor, cWeapon* weapon)
{
	weapon->OnUse(aggressor);
}

void RPG_Engine::Damage(cDynamic_Projectile* projectile, cDynamic_Creature* victim)
{
	


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

		m_vecFightText.push_back(Text);
		

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

		cDynamic_TextDamage* Text = new cDynamic_TextDamage(victim->px + victim->CollbordersX + 0.25f, victim->py + victim->CollbordersY + 0.6f, std::to_string(CalculatedDamage));

		m_vecFightText.push_back(Text);
		
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
	int i = 0;
	int z = 0;
	for (auto c : sText)
	{
		if (c == 10)
		{
			z++;
			i = 0;
			continue;
		}

		float sx = ((c-32) % 16) * 32;
		float sy = ((c-32) /16) *32;



		DrawPartialDecal({ (float)x + (i * (18*scalex)), (float)y+(z*(32*scaley)) }, m_sprFont, { sx, sy }, { 32, 32 }, {scalex,scaley}, color);
		i++;
	}


}

void RPG_Engine::DisplayDialog(std::vector<std::string> vecText, int x, int y)
{
	int nMaxLineLength = 0;
	int nLines = vecText.size();

	for (auto i : vecText)  if (i.size() > nMaxLineLength) nMaxLineLength = i.size();


	olc::Decal* Vertical = RPG_Assets::get().GetSprite("Vertical StickFill");
	olc::Decal* Horizontal = RPG_Assets::get().GetSprite("Horizontal StickFill");
	olc::Decal* BlackPattern = RPG_Assets::get().GetSprite("DescriptionPattern");

	DrawPartialDecal({ (float)x - 1 , (float)y - 1 }, BlackPattern, { 0,0 }, { (float)nMaxLineLength * 18 + 10,(float)nLines * 32 +10 });

	DrawPartialDecal({ (float)x - 1,  (float)y - 1 }, Vertical, { 0,0 }, { (float)4,(float)nLines * 32 + 15 }, { 1,1 }, { 66,83,100 });
	DrawPartialDecal({ (float)x + nMaxLineLength * 18 + 10,  (float)y - 1 }, Vertical, { 0,0 }, { (float)4,(float)nLines * 32 + 15 }, { 1,1 }, { 66,83,100 });

	DrawPartialDecal({ (float)x - 1, (float)y - 1 }, Horizontal, { 0,0 }, { (float)nMaxLineLength * 18 + 14,(float)4 }, { 1,1 }, { 66,83,100 });
	DrawPartialDecal({ (float)x - 1, (float)y + nLines * 32 + 10 }, Horizontal, { 0,0 }, { (float)nMaxLineLength * 18 + 14,(float)4 },{1,1},{66,83,100});



	for (int l = 0; l < vecText.size(); l++)
		DrawBigText(vecText[l], x, y + l * 32, 1,1);

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

int RPG_Engine::GetEnergy()
{
	return m_pPlayer->energeyAmount;
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
	m_pPlayer->bDraw = bdraw;
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

		olc::vf2d* tilesize = nullptr;
		olc::vf2d* tilesource = nullptr;

		olc::vf2d* itsize = nullptr;
		olc::vf2d* itsource = nullptr;


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
		enemysPool.push_back(new cDynamic_creature_Bandit());
	}

	// Initialize the Wolf pool
	for (int i = 0; i <30; ++i) {
		enemysPool.push_back(new cDynamic_creature_DireWolf());
	}


	for (int i = 0; i < 30; ++i) {
		enemysPool.push_back(new cDynamic_creature_Boar());
	}

	for (int i = 0; i < 30; ++i) {
		enemysPool.push_back(new cDynamic_creature_WereWolf());
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
cDynamic* RPG_Engine::SpawnBandit(const olc::vf2d* position)
{
	for (auto it = enemysPool.begin(); it != enemysPool.end(); ++it) {
		cDynamic* entity = *it;
		if (dynamic_cast<cDynamic_creature_Bandit*>(entity)) {
			entity->binitialized = true;
			m_vecDynamics.push_back(entity); // Add the Bandit entity to the game
			entity->px = position->x;
			entity->py = position->y;
			enemysPool.erase(it); // Remove the Bandit entity from the pool
			return entity; // Exit the function after spawning a Bandit
		}
	}
}


cDynamic* RPG_Engine::SpawnWerewolf(const olc::vf2d* position)
{
	for (auto it = enemysPool.begin(); it != enemysPool.end(); ++it) {
		cDynamic* entity = *it;
		if (dynamic_cast<cDynamic_creature_WereWolf*>(entity)) {
			entity->binitialized = true;
			m_vecDynamics.push_back(entity); // Add the Bandit entity to the game
			entity->px = position->x;
			entity->py = position->y;
			enemysPool.erase(it); // Remove the Bandit entity from the pool
			return entity; // Exit the function after spawning a Bandit
		}
	}
}

cDynamic* RPG_Engine::SpawnBoar(const olc::vf2d* position)
{
	for (auto it = enemysPool.begin(); it != enemysPool.end(); ++it) {
		cDynamic* entity = *it;
		if (dynamic_cast<cDynamic_creature_Boar*>(entity)) {
			entity->binitialized = true;
			m_vecDynamics.push_back(entity); // Add the Bandit entity to the game
			entity->px = position->x;
			entity->py = position->y;
			enemysPool.erase(it); // Remove the Bandit entity from the pool
			return entity; // Exit the function after spawning a Bandit
		}
	}
}

cDynamic* RPG_Engine::FindpoolEntity( cDynamic* object)
{


	//if (cDynamic_creature_DireWolf* derivedobj = dynamic_cast<cDynamic_creature_DireWolf*>(object)) {
	//	
	//}
	//if (cDynamic_creature_Bandit* derivedobj2 = dynamic_cast<cDynamic_creature_Bandit*>(object))
	//{
	//
	//}
	//if (cDynamic_creature_Boar* deruvedobj3 = dynamic_cast<cDynamic_creature_Boar*>(object))
	//{
	//	
	//}


	for (auto it = enemysPool.begin(); it != enemysPool.end(); ++it) {
		
		cDynamic* entity = *it;
		
		
		if (typeid(*object) == typeid(*entity)) { // Compare the types of the object and the entity

			m_vecDynamics.push_back(entity); // Add the Bandit entity to the game
			enemysPool.erase(it); // Remove the Bandit entity from the pool

			return  entity; // Exit the function after spawning a Bandit
		}
		
	}



	return nullptr;
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



cDynamic* RPG_Engine::SpawnDireWolf(const olc::vf2d* position)
{
	
	for (auto it = enemysPool.begin(); it != enemysPool.end(); ++it) {
		cDynamic* entity = *it;
		
		if (dynamic_cast<cDynamic_creature_DireWolf*>(entity)) {
			entity->binitialized = true;
			m_vecDynamics.push_back(entity); // Add the Bandit entity to the game
			entity->px = position->x;
			entity->py = position->y;
			enemysPool.erase(it); // Remove the Bandit entity from the pool


			return entity; // Exit the function after spawning a Bandit
		}
	}
	return nullptr; // Exit the function after spawning a Bandit
}
void RPG_Engine:: ReturnToPool(cDynamic* entity) {

	// Check the dynamic type of the entity
	if (cDynamic_creature_Bandit* bandit = dynamic_cast<cDynamic_creature_Bandit*>(entity))
	{
		bandit->binitialized = false;

		// If the entity is a Bandit, return it to the Bandit pool
		enemysPool.push_back(bandit);
	}
	else if (cDynamic_creature_DireWolf* wolf = dynamic_cast<cDynamic_creature_DireWolf*>(entity))
	{
		wolf->binitialized = false;
		// If the entity is a DireWolf, return it to the DireWolf pool
		enemysPool.push_back(wolf);
	} // Add more conditions for other types if needed
	else if (cDynamic_creature_Boar* Boar = dynamic_cast<cDynamic_creature_Boar*>(entity))
	{
		Boar->binitialized = false;
		enemysPool.push_back(Boar);
		// Handle unknown entity types or errors
		// You might want to log an error or throw an exception
	}
	else if (cDynamic_creature_WereWolf* Werewolf = dynamic_cast<cDynamic_creature_WereWolf*>(entity))
	{
		Werewolf->binitialized = false;
		enemysPool.push_back(Werewolf);
	}
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
