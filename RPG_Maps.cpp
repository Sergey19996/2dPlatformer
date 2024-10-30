#include "RPG_Maps.h"
#include "RPG_Assets.h"
#include "RPG_Engine.h"

cScriptProcessor* cMap::g_script = nullptr;
RPG_Engine* cMap::g_engine = nullptr;

#define X(n) g_script->AddCommand(new cComand_ShowDialog ## n)

cMap::cMap()
{
	pSprite = nullptr;
	nWidth = 0;
	nHeight = 0;
	CloseParralax = nullptr;
	MidParralax = nullptr;
	FarParralax = nullptr;
	//m_solids = nullptr;
	//m_indices = nullptr;
}

cMap::~cMap()
{
	//delete[] m_solids;
	//delete[] m_indices;
}

bool cMap::Create(std::string fileData, olc::Decal* sprite, olc::Decal* sdinsprite, olc::Decal* CloseParralax, olc::Decal* MidParralax, olc::Decal* FarParralax, std::string name)
{
	pSprite = sprite;
	pDynsprite = sdinsprite;

	this->FarParralax = FarParralax;
	this->MidParralax = MidParralax;
	this->CloseParralax = CloseParralax;
	
	sName = name;
	
	std::ifstream data;
	data.open(fileData);

	if (data.is_open())
	{



		data >> nWidth;
		data >> nHeight;

		batchLayer.reserve(nWidth * nHeight);

		for (int i = 0; i < nWidth * nHeight; i++)
		{
			Tile tile;
			
		//	while (data >> str) {
			//	int temp = std::stoi(str);
			//	uint8_t value = static_cast<uint8_t>(temp);
				// Записывай значение в массив
		//	}
			
			
			tile.position.x = i % nWidth;
			tile.position.y = i / nWidth;

			int temp =0;
			data >> temp;   // in data we keep information in char or string. 
			tile.Collision = static_cast<uint8_t>(temp);;

			
			data >> tile.FirstLayer.index;
			tile.FirstLayer.sourcerect.x = (tile.FirstLayer.index %32)*32;
			tile.FirstLayer.sourcerect.y = (tile.FirstLayer.index /32)*32;
			tile.FirstLayer.layerindex = 1;

			data >> tile.SecondLayer.index;
			tile.SecondLayer.sourcerect.x = (tile.SecondLayer.index %32)*32;
			tile.SecondLayer.sourcerect.y = (tile.SecondLayer.index / 32)*32;
			tile.SecondLayer.layerindex = 2;

			data >> tile.ThirdLayer.index;
			tile.ThirdLayer.sourcerect.x = (tile.ThirdLayer.index % 32)*32;
			tile.ThirdLayer.sourcerect.y = (tile.ThirdLayer.index /32)*32;
			tile.ThirdLayer.layerindex = 3;

			batchLayer.push_back(tile);


		}
		for (int i = 0; i < nWidth * nHeight; i++) // becouse we allready did few locations... neew use other one loop run
		{
			

			
			data >> batchLayer[i].DynamicLayer.index;
		//	batchLayer[i].DynamicLayer.index -= 10;

			batchLayer[i].DynamicLayer.sourcerect.x = (batchLayer[i].DynamicLayer.index %8);
			batchLayer[i].DynamicLayer.sourcerect.y = (batchLayer[i].DynamicLayer.index);
			batchLayer[i].DynamicLayer.layerindex = 10;


			batchLayer[i].DynamicLayer.size.x = (128);
			batchLayer[i].DynamicLayer.size.y = (128);

			data >> batchLayer[i].ZeroLayer.index;
			batchLayer[i].ZeroLayer.sourcerect.x = (batchLayer[i].ZeroLayer.index % 32)*32;
			batchLayer[i].ZeroLayer.sourcerect.y = (batchLayer[i].ZeroLayer.index /32)*32;
			batchLayer[i].ZeroLayer.layerindex = 0;



		}
		
		
		CombineAdjacentTiles(batchLayer, batchLayer[0].ThirdLayer.layerindex);
		CombineAdjacentTiles(batchLayer, batchLayer[0].SecondLayer.layerindex);
		CombineAdjacentTiles(batchLayer, batchLayer[0].FirstLayer.layerindex);
		CombineAdjacentTiles(batchLayer, batchLayer[0].ZeroLayer.layerindex);



	//	std::sort(batchLayer.begin(), batchLayer.end(), compareByLayerIndex);


		



		
		 // Load obsticle Points 
				
		auto load = [&](float v1x, float v1y, float v2x, float v2y)
		{
			olc::vf2d* v1 = new olc::vf2d{ v1x,v1y };
			olc::vf2d* v2 = new olc::vf2d{ v2x,v2y };

			std::pair<olc::vf2d*, olc::vf2d*>* g1 = new std::pair<olc::vf2d*, olc::vf2d*>(v1, v2);

			m_vecobsticles.push_back(g1);    //map array <key,value>



		};


		if (name=="Forest")
		{

		load(17, 27, 21, 25);
		load(57, 27, 69, 33);
		load(154, 25, 162, 29);

		load(172, 35, 184, 29);
		load(204, 34, 208, 32);

		load(222, 32, 242, 42);
		
		}

		if (name== "ForestPartTwo")
		{
			load(6, 23, 10, 25);
			load(46, 26, 50, 24);
			load(76, 25, 80, 27);

			load(114, 23, 118, 21);
			load(168, 26, 172, 28);

			load(192, 29, 196, 27);
		}

		
		/// ////////////////////
		
		std::cout << "Load succeed" << std::endl;
		
		data.close();
		return true;
	}
	data.close();
	return false;


}

Tile cMap::GetIndexLayer( int x, int y)
{
	
	if (x < nWidth && y < nHeight && x >= 0 && y >= 0)
	{

	return batchLayer[y * nWidth + x];

	}
	else
	{
		Tile empty;
		empty.Collision = 0;
		empty.DynamicLayer.index = 0;
		empty.FirstLayer.index = 0;
		empty.SecondLayer.index = 0;
		empty.ThirdLayer.index = 0;
		empty.ZeroLayer.index = 0;
		return empty;
	}
	
}



int cMap::GetColliziionIndex(int x, int y)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
		return static_cast<int>(batchLayer[y * nWidth + x].Collision);
	else
		return 0;
}

std::pair<olc::vf2d*, olc::vf2d*>*::cMap::getObsticlesPoints(float pxX, float pxXF, float pyY, float pyYF)
{

	//m_vecobsticles



		for (auto& gsource : { &m_vecobsticles })
		{
			for (auto& gobject : *gsource) // gobject-> pair of points
			{
				
				if (gobject->first->y > gobject->second->y)
				{

					if (pxXF+2 >= gobject->first->x && pxX-2 <= gobject->second->x && pyYF <= gobject->first->y+2 && pyYF >= gobject->second->y-2 )
					{
						return gobject;
					}


				}else

				if (pxXF+2 >= gobject->first->x && pxX-2 <= gobject->second->x && pyYF >= gobject->first->y-2 && pyYF <= gobject->second->y+2 )
				{
					
					return gobject;
				}

				

			}
		}
	
		olc::vf2d v1 { 1,1 };
		olc::vf2d v2 { 1,1 };

		std::pair<olc::vf2d*, olc::vf2d*>* g1 = new std::pair<olc::vf2d*, olc::vf2d*>(&v1, &v2);

		return  g1;
	
}




cMap_Forest::cMap_Forest()
{
	bFinishloading = Create("save/LevelMap.txt",RPG_Assets::get().GetSprite("BlockFirstLvl"), RPG_Assets::get().GetSprite("ForestDynamicLayer"), RPG_Assets::get().GetSprite("ParralaxForestClose"), RPG_Assets::get().GetSprite("ParralaxForestMid"), RPG_Assets::get().GetSprite("ParralaxForestFar"), "Forest");


	setWolfsPos();
	setBanditsPos();
	setBoarPos();
	setWereWolfsPos();

	//RPG_Assets::get().playMusic("D:/C++/Pantir_Platformer/Sounds/FriendSong.wav");
}

bool cMap_Forest::PopulateDynamics(std::vector<cDynamic*>& vecDyns)
{

	// Initialize the raindrops
	//for (uint16_t i = 0; i < 100; ++i)  // they have indexcies from 0 to 100
//	{

		g_engine->SpawnRainDrops(0);
//	}


	for (uint16_t i = 200; i < 205; ++i)
	{
		g_engine->SpawnClouds(i);
	}







	RPG_Assets::get().playMusic("Sounds/ForestMusic.wav");

	int Mqphase = g_engine->GetQuest("MainQuest")->GetPhase();
	//SetupParallax(RPG_Assets::get().GetSprite("ParralaxForestClose"), RPG_Assets::get().GetSprite("ParralaxForestMid"), RPG_Assets::get().GetSprite("ParralaxForestFar"));


	//olc::vf2d test = { 8.0f,25.5 };
	//g_engine->SpawnBanditArcher(&test);

	for (int i = 0; i < BanditsPositions.size(); )
	{
		
		//g1->px = 128;
		//g1->py = 1;
	//	g_engine->SpawnBandit(*&(BanditsPositions[i]));
		
	//	i++;
		g_engine->SpawnBanditArcher(BanditsPositions[i]);
		i++;
		g_engine->SpawnBandit(BanditsPositions[i]);
		i++;
	}


	for (int i = 0; i < WolfsPositions.size(); )
	{
		
	
		g_engine->SpawnDireWolf(WolfsPositions[i]);
		i++;
		

		
	}



	for (int i = 0; i < BoarPositions.size(); )
	{


		g_engine->SpawnBoar(*&(BoarPositions[i]));
		i++;



	}
	for (int i = 0; i < WerewolfPositions.size(); )
	{


		g_engine->SpawnWerewolf(*&(WerewolfPositions[i]));
		i++;



	}
	
	//cDynamic* s1 = new cDynamic_creature_BossBandit();
	//s1->px = 5;
	//s1->py = 22;
	//vecDyns.push_back(s1);

	//Add Teleporters
	vecDyns.push_back(new cDynamic_Teleport(226.0f, 29.0f, "Village", 6.0f, 12.0f)); //<--- Teleport From Village in Forest Left side (Day)

	//vecDyns.push_back(new cDynamic_Teleport(180.0f, 28.0f, "Forest", 249.0f, 55.0f)); //<--- Teleport in cave main 
	
	if (Mqphase > 5)
	{
		cDynamic* g3 = new cDynamic_creature_NPCBandit("CaveEntrence");
		g3->SwitchLayer(4);
		g3->px = 180.0f;
		g3->py = 27.5f;
		
		vecDyns.push_back(g3);

	g3 = new cDynamic_creature_NPCBandit("CaveOut");
	g3->SwitchLayer(4);
	g3->px = 249.0f;
	g3->py = 54.5f;
	vecDyns.push_back(g3);
	};



	vecDyns.push_back(new cDynamic_Teleport(97.0f, 58.0f, "Forest", 96.0f, 40.5f)); //<--- Teleport in cave main 



	g_script->AddCommand((new cComand_AppearScreen(2)));






	return true;

	
}

bool cMap_Forest::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature)
{

	if (target->sName == "Teleport")
	{ 
		g_script->AddCommand((new cComand_HideScreen(2)));
		g_script->AddCommand(new cComand_Changemap(
			((cDynamic_Teleport*)target)->sMapName,
			((cDynamic_Teleport*)target)->fMapPosX,
			((cDynamic_Teleport*)target)->fMapPosY));

	}


	if (target->sName == "CaveEntrence" && nature == TALK)   //  <-------------------Map NPC Teleport From Village in Tavern
	{
		g_script->AddCommand((new cComand_HideScreen(3)));
		g_script->AddCommand(new cComand_Changemap("Forest", 249.0f, 54.5f));

	}

	if (target->sName == "CaveOut" && nature == TALK)   //  <-------------------Map NPC Teleport From Village in Tavern
	{
		g_script->AddCommand((new cComand_HideScreen(3)));
		g_script->AddCommand(new cComand_Changemap("Forest", 180.0f, 27.5f));

	}


	return false;
}

void cMap_Forest::setWolfsPos()
{

	olc::vf2d v1 =  olc::vf2d{ 25,23.5 };
	


	WolfsPositions.push_back(v1);

	



	v1 =  olc::vf2d{ 33,26.5 };
	WolfsPositions.push_back(v1);

	v1 =  olc::vf2d{ 55,25.5 };
	WolfsPositions.push_back(v1);

	v1 =  olc::vf2d{ 54,25.5 };
	WolfsPositions.push_back(v1);

	v1 =  olc::vf2d{ 152,23.5 };
	WolfsPositions.push_back(v1);

	v1 =  olc::vf2d{ 196,32.5 };
	WolfsPositions.push_back(v1);

}

void cMap_Forest::setBanditsPos()
{

	olc::vf2d v1 =  olc::vf2d{ 101,23.5 };
	BanditsPositions.push_back(v1);


	 v1 =  olc::vf2d{ 103,23.5 };
	BanditsPositions.push_back(v1);


	v1 =  olc::vf2d{ 115.5,23.5 };
	BanditsPositions.push_back(v1);

	v1 =  olc::vf2d{ 116.5,23.5 };
	BanditsPositions.push_back(v1);

	v1 =  olc::vf2d{ 130,23.5 };
	BanditsPositions.push_back(v1);

	v1 =  olc::vf2d{ 132,23.5 };
	BanditsPositions.push_back(v1);

	v1 =  olc::vf2d{ 144,23.5 };
	BanditsPositions.push_back(v1);

	v1 =  olc::vf2d{ 146,23.5 };
	BanditsPositions.push_back(v1);

	v1 =  olc::vf2d{ 209,30.5 };
	BanditsPositions.push_back(v1);

	v1 =  olc::vf2d{ 215,30.5 };
	BanditsPositions.push_back(v1);

}

void cMap_Forest::setWereWolfsPos()
{

	olc::vf2d v1 =  olc::vf2d{ 248,60 };
		WerewolfPositions.push_back(v1);






	v1 =  olc::vf2d{ 202,59 };
	WerewolfPositions.push_back(v1);

	v1 =  olc::vf2d{ 175,57 };
	WerewolfPositions.push_back(v1);

	v1 =  olc::vf2d{ 152,59 };
	WerewolfPositions.push_back(v1);

	v1 =  olc::vf2d{ 135,59 };
	WerewolfPositions.push_back(v1);


	v1 =  olc::vf2d{ 114,59 };
	WerewolfPositions.push_back(v1);

	v1 =  olc::vf2d{ 100,54 };
	WerewolfPositions.push_back(v1);

	v1 =  olc::vf2d{ 110,39 };
	WerewolfPositions.push_back(v1);
	v1 =  olc::vf2d{ 122,44 };
	WerewolfPositions.push_back(v1);
	v1 =  olc::vf2d{ 131,47 };
	WerewolfPositions.push_back(v1);

}

void cMap_Forest::setBoarPos()
{

	olc::vf2d v1 =  olc::vf2d{ 222,57 };
	BoarPositions.push_back(v1);


	v1 =  olc::vf2d{ 195,56 };
	BoarPositions.push_back(v1);

	v1 =  olc::vf2d{ 182,60 };
	BoarPositions.push_back(v1);

	v1 =  olc::vf2d{ 165,60 };
	BoarPositions.push_back(v1);

	v1 =  olc::vf2d{ 158,60 };
	BoarPositions.push_back(v1);

	v1 =  olc::vf2d{ 150,60 };
	BoarPositions.push_back(v1);

	v1 =  olc::vf2d{ 115,66 };  // seem to change
	BoarPositions.push_back(v1);

	v1 =  olc::vf2d{ 103,42 };
	BoarPositions.push_back(v1);

}

cMap_VillageInFire::cMap_VillageInFire()
{
	bFinishloading = Create("save/LevelOneVillageInfire.txt", RPG_Assets::get().GetSprite("BlockVillageLvl"), RPG_Assets::get().GetSprite("ForestDynamicLayer"), RPG_Assets::get().GetSprite("ParralaxVillageInFireClose"), RPG_Assets::get().GetSprite("ParralaxVillageInFireMid"), RPG_Assets::get().GetSprite("ParralaxVillageInFireFar"), "VillageInFire");


	setBanditsPos();
}

bool cMap_VillageInFire::PopulateDynamics(std::vector<cDynamic*>& vecDyns)
{

	RPG_Assets::get().playMusic("Sounds/VillageInFireMusic.wav");
	RPG_Assets::get().SetMusicVolume(1);


	//for (int i = 0; i < BanditsPositions.size(); )
	//{

		//g1->px = 128;
		//g1->py = 1;
	//	g_engine->SpawnBandit(BanditsPositions[i]);

	//	i++;

//	}
		
		
			cDynamic* g1 = new cDynamic_creature_BossBandit();
			g1->sName = "BossBandit";
			g1->px = 50;
			g1->py = 12;
			vecDyns.push_back(g1);
			
		
			g_script->AddCommand((new cComand_AppearScreen(2)));


	return true;
}

bool cMap_VillageInFire::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature)
{

	if (target->sName == "Teleport")
	{
		g_script->AddCommand((new cComand_HideScreen(2)));
		g_script->AddCommand(new cComand_Changemap(
			((cDynamic_Teleport*)target)->sMapName,
			((cDynamic_Teleport*)target)->fMapPosX,
			((cDynamic_Teleport*)target)->fMapPosY));
	}


	return false;
}

void cMap_VillageInFire::setBanditsPos()
{

	olc::vf2d v1 = olc::vf2d{ 16,13.5 };
	BanditsPositions.push_back(v1);

	v1 =  olc::vf2d{ 21.5,13.5 };
	BanditsPositions.push_back(v1);

	v1 =  olc::vf2d{ 29.5,13.5 };
	BanditsPositions.push_back(v1);

	v1 =  olc::vf2d{ 50,13.5 };
	BanditsPositions.push_back(v1);

	v1 =  olc::vf2d{ 60,13.5 };
	BanditsPositions.push_back(v1);

	v1 =  olc::vf2d{ 55,13.5 };
	BanditsPositions.push_back(v1);
}

cMap_Village::cMap_Village()
{

	bFinishloading = Create("save/LevelOneVillage.txt", RPG_Assets::get().GetSprite("BlockVillageLvl"), RPG_Assets::get().GetSprite("ForestDynamicLayer"),RPG_Assets::get().GetSprite("ParralaxVillageClose"), RPG_Assets::get().GetSprite("ParralaxVillageMid"), RPG_Assets::get().GetSprite("ParralaxVillageFar"), "Village");

}

bool cMap_Village::PopulateDynamics(std::vector<cDynamic*>& vecDyns)
{

	RPG_Assets::get().playMusic("Sounds / VillgaeMusic.wav");

	g_script->AddCommand((new cComand_AppearScreen(2)));

	cDynamic* g1 = new cDynamic_creature_NPCBandit("BlackSmith");
	g1->SwitchLayer(4); // Questable layer tag
	g1->px =53;
	g1->py = 12;
	vecDyns.push_back(g1);


	


	cDynamic* g3 = new cDynamic_creature_NPCBandit("TavernVillage");
	g3->SwitchLayer(4); // Questable layer tag
	g3->px =80;
	g3->py = 12;
	vecDyns.push_back(g3);


	cDynamic* g4 = new cDynamic_creature_NPCBandit("Map");
	g4->SwitchLayer(4); // Questable layer tag
	g4->px = 11;
	g4->py = 12;
	vecDyns.push_back(g4);


	cDynamic* g5 = new cDynamic_creature_NPCBandit("ProfessionMan");
	g5->SwitchLayer(4); // Questable layer tag
	g5->px = 71;
	g5->py = 12;
	vecDyns.push_back(g5);

	//Add Teleporters
	vecDyns.push_back(new cDynamic_Teleport(4.0f, 13.5f, "Forest", 225.0f, 29.0f)); //<--- Teleport From Village in Forest Left side (Day)

	
	vecDyns.push_back(new cDynamic_Teleport(96.0f, 13.5f, "ForestPartTwo", 5.0f, 21.5f)); //<--- Teleport From Village in Forest Right side (Day)





	g_engine->GiveBlackSmithItem("SmallWallet");
	g_engine->GiveBlackSmithItem("Basic Helment");
	g_engine->GiveBlackSmithItem("Basic Chest");
	g_engine->GiveBlackSmithItem("Basic Neck");

	
	return false;
}

bool cMap_Village::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature)
{
	if (target->sName == "Teleport")
	{
		g_script->AddCommand((new cComand_HideScreen(2)));
		g_script->AddCommand(new cComand_Changemap(
			((cDynamic_Teleport*)target)->sMapName,
			((cDynamic_Teleport*)target)->fMapPosX,
			((cDynamic_Teleport*)target)->fMapPosY));
	}

	if (target->sName == "TavernVillage" && nature == TALK)   //  <-------------------Map NPC Teleport From Village in Tavern
	{
		g_script->AddCommand((new cComand_HideScreen(3)));
		g_script->AddCommand(new cComand_Changemap("VillageTavern", 12, 10));

	}

	if (target->sName == "Map" && nature == TALK)   //  <-------------------Map NPC First Village
	{
		g_script->AddCommand((new cComand_SetNgameMod(5)));
		g_script->AddCommand((new cComand_SetPause(1)));
		
	}

	if (target->sName == "ProfessionMan" && nature == TALK)   //  <-------------------Profession NPC First Village
	{
		g_script->AddCommand((new cComand_SetNgameMod(6)));
		g_script->AddCommand((new cComand_SetPause(1)));

	}
	if (target->sName == "BlackSmith" && nature == TALK)   //  <-------------------BlacksmithNPC First Village
	{
		g_script->AddCommand((new cComand_SetNgameMod(7)));
		g_script->AddCommand((new cComand_SetPause(1)));

	}


	return false;
}

cMap_Mountains::cMap_Mountains()
{


	bFinishloading = Create("save/LevelOneVillage.txt", RPG_Assets::get().GetSprite("BlockFirstLvl"), RPG_Assets::get().GetSprite("ForestDynamicLayer"), RPG_Assets::get().GetSprite("ParralaxMountainsClose"), RPG_Assets::get().GetSprite("ParralaxMountainsMid"), RPG_Assets::get().GetSprite("ParralaxMountainsFar"), "Mountains");

}

bool cMap_Mountains::PopulateDynamics(std::vector<cDynamic*>& vecDyns)
{
	g_script->AddCommand((new cComand_AppearScreen(2)));
	return false;
}

bool cMap_Mountains::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature)
{
	return false;
}

cMap_VillageTavern::cMap_VillageTavern()
{

	bFinishloading = Create("save/LevelTractirVillage.txt", RPG_Assets::get().GetSprite("BlockTavernLvl"), RPG_Assets::get().GetSprite("ForestDynamicLayer"), RPG_Assets::get().GetSprite("Pantir's Dagger"), RPG_Assets::get().GetSprite("Pantir's Dagger"), RPG_Assets::get().GetSprite("Pantir's Dagger"), "VillageTavern");

}

bool cMap_VillageTavern::PopulateDynamics(std::vector<cDynamic*>& vecDyns)    //<----Tavern SPawn
{

	RPG_Assets::get().playMusic("Sounds/TavernMusic.wav");

	cDynamic* g1 = new cDynamic_creature_NPCBandit("SaveMan");
	g1->SwitchLayer(4); // Questable layer tag
	g1->px = 7;
	g1->py =10;
	vecDyns.push_back(g1);

	cDynamic* g2 = new cDynamic_creature_NPCBandit("LuxarySeller");
	g2->SwitchLayer(4); // Questable layer tag
	g2->px = 11;
	g2->py = 10;
	vecDyns.push_back(g2);

	cDynamic* g3 = new cDynamic_creature_NPCBandit("Warehouse");
	g3->SwitchLayer(4); // Questable layer tag
	g3->px = 17;
	g3->py = 10;
	vecDyns.push_back(g3);



	g_engine->GiveStoreItem("SmallWallet");


	vecDyns.push_back(new cDynamic_Teleport(6.0f, 10.5f, "Village", 92.0f, 13.0f)); //<--- Teleport From Tavern in Village Left side (Day)



	g_script->AddCommand((new cComand_AppearScreen(2)));

	return false;
}

bool cMap_VillageTavern::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature)
{
	if (target->sName == "Teleport")
	{
		g_script->AddCommand(new cComand_HideScreen(2));
		g_script->AddCommand(new cComand_Changemap(
			((cDynamic_Teleport*)target)->sMapName,
			((cDynamic_Teleport*)target)->fMapPosX,
			((cDynamic_Teleport*)target)->fMapPosY));
	//	g_script->AddCommand(new cComand_LockCamera());
	}


	if (target->sName == "SaveMan" && nature == TALK)   // Talking with first Bandit  4
	{
		g_engine->SaveFunction();

	}

	if (target->sName == "Warehouse" && nature == TALK)
	{
		g_script->AddCommand((new cComand_SetNgameMod(2)));
		g_script->AddCommand((new cComand_SetPause(1)));
	}

	if (target->sName == "LuxarySeller" && nature == TALK)
	{
		g_script->AddCommand((new cComand_SetNgameMod(4)));
		g_script->AddCommand((new cComand_SetPause(1)));
	}


	return false;
}

cMap_ForestPartTwo::cMap_ForestPartTwo()
{

	bFinishloading = Create("save/LevelMapPartTwo.txt", RPG_Assets::get().GetSprite("ForesttLvlPartTwo"), RPG_Assets::get().GetSprite("ForestDynamicLayer"), RPG_Assets::get().GetSprite("ParralaxForestClose"), RPG_Assets::get().GetSprite("ParralaxForestMid"), RPG_Assets::get().GetSprite("ParralaxForestFar"), "ForestPartTwo");

	setWolfsPos();
	setBoarsPos();


}

bool cMap_ForestPartTwo::PopulateDynamics(std::vector<cDynamic*>& vecDyns)
{

	RPG_Assets::get().playMusic("Sounds/Forest.wav");

	for (int i = 0; i < WolfsPositions.size(); )
	{


		g_engine->SpawnDireWolf(WolfsPositions[i]);
		i++;



	}

	for (int i = 0; i < BoarsPositions.size(); )
	{



		cDynamic* g1 = new cDynamic_creature_Boar();
		g1->px = BoarsPositions[i].x;
		g1->py = BoarsPositions[i].y;
		vecDyns.push_back(g1);
		i++;



	}









	std::list<cComand*> Rider_comands;
	cDynamic_creature_Rider* r1 = new cDynamic_creature_Rider(RPG_Assets::get().GetSprite("DireWolfRight"), RPG_Assets::get().GetSprite("DireWolfLeft"));
	r1->px = 132;
	r1->py = 24, 5;
	Rider_comands.push_back(new cComand_moveTo(r1, 140.5, 24.5, 3));
	Rider_comands.push_back(new cComand_JumpTo(r1, 144, 23.5, 3,1.5));
	Rider_comands.push_back(new cComand_moveTo(r1, 146, 23.5, 1));
	Rider_comands.push_back(new cComand_JumpTo(r1, 149.5, 22.5, 3, 1.5));
	Rider_comands.push_back(new cComand_moveTo(r1, 152.5, 22.5, 1));
	Rider_comands.push_back(new cComand_JumpTo(r1, 156, 24.5, 3, 1.5));
	Rider_comands.push_back(new cComand_moveTo(r1, 165, 24.5, 3));
	r1->TakeComands(Rider_comands);
	vecDyns.push_back(r1);




	g_script->AddCommand((new cComand_AppearScreen(2)));


	return false;
}

bool cMap_ForestPartTwo::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature)
{

	if (target->sName == "Teleport")
	{
		g_script->AddCommand((new cComand_HideScreen(2)));
		g_script->AddCommand(new cComand_Changemap(
			((cDynamic_Teleport*)target)->sMapName,
			((cDynamic_Teleport*)target)->fMapPosX,
			((cDynamic_Teleport*)target)->fMapPosY));

	}




	return false;
}

void cMap_ForestPartTwo::setWolfsPos()
{

	olc::vf2d v1 =  olc::vf2d{ 17,23.5 };
	WolfsPositions.push_back(v1);

	v1 =  olc::vf2d{ 162,24.5 };
	WolfsPositions.push_back(v1);

	v1 =  olc::vf2d{ 202,25.5 };
	WolfsPositions.push_back(v1);

	v1 =  olc::vf2d{ 215,26.5 };
	WolfsPositions.push_back(v1);

	v1 =  olc::vf2d{ 245,27.5 };
	WolfsPositions.push_back(v1);

	v1 =  olc::vf2d{ 247,37.5 };
	WolfsPositions.push_back(v1);
}

void cMap_ForestPartTwo::setBoarsPos()
{
	olc::vf2d v1 =  olc::vf2d{ 28,28.5 };
	BoarsPositions.push_back(v1);

	v1 =  olc::vf2d{ 65,27.5 };
	BoarsPositions.push_back(v1);

	v1 =  olc::vf2d{ 134,245 };
	BoarsPositions.push_back(v1);

	v1 =  olc::vf2d{ 182,31.5 };
	BoarsPositions.push_back(v1);

	v1 =  olc::vf2d{ 182,31.5 };
	BoarsPositions.push_back(v1);

}

bool compareByLayerIndex(const Tile& a, const Tile& b)
{
	{
		if (a.ThirdLayer.layerindex != b.ThirdLayer.layerindex)
		{
			return a.ThirdLayer.layerindex < b.ThirdLayer.layerindex;
		}
		else if (a.SecondLayer.layerindex != b.SecondLayer.layerindex)
		{
			return a.SecondLayer.layerindex < b.SecondLayer.layerindex;
		}
		else if (a.FirstLayer.layerindex != b.FirstLayer.layerindex)
		{
			return a.FirstLayer.layerindex < b.FirstLayer.layerindex;
		}
		else
			if (a.ZeroLayer.layerindex != b.FirstLayer.layerindex) {
				return a.FirstLayer.layerindex < b.FirstLayer.layerindex;
			}
	}
}
