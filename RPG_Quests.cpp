#include "RPG_Quests.h"
#include "RPG_Engine.h"


cScriptProcessor* cQuest::g_script = nullptr;
RPG_Engine* cQuest::g_engine = nullptr;

#define X(n) g_script->AddCommand(new cCommand_ ## n)


cQuest::cQuest()
{
	
}

bool cQuest::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE Nature)
{
	return false;
}

bool cQuest::PopulateDynamics(std::vector<cDynamic*>& vecDyns, std::string sMap)
{
	return false;
}

cQuset_MainQuest::cQuset_MainQuest()
{
	sName = "MainQuest";
	SaveSlot = 0;
	sDescription = "Leave Forest";
	
}

bool cQuset_MainQuest::PopulateDynamics(std::vector<cDynamic*>& vecDyns, std::string sMap)
{
	if (sMap == "Forest"&& m_nPhase ==0 )
	{
	
			cDynamic_ClipTrigger* c2 = new cDynamic_ClipTrigger("MeetScene");  //mett first bandit
			c2->px = 76;
			c2->py = 26;
			c2->CollbordersY = -10;

			vecDyns.push_back(c2);

			olc::vf2d pos{ 81,26 };
			cDynamic* g1 = g_engine->SpawnBandit(pos);
			//g1->setFlag(g1->quested);  // when we don't want to add him in initializepool after death
			g1->sName = "FirstBandit";
			g1->px = 81;
			g1->py = 26;
			g1->clearFlag(g1->bControllable);
			//vecDyns.push_back(g1);


			pos.x = 178;
			pos.y = 26;
			cDynamic_creature_WereWolf* z1 = (cDynamic_creature_WereWolf *)g_engine->SpawnWerewolf(pos);
			z1->sName = "werewolf";
			z1->bTransformed = false;
			//z1->setFlag(z1->quested) ;
			z1->clearFlag(z1->bControllable);
			z1->px = 178;
			z1->py = 26;
		//	vecDyns.push_back(z1);

			//cDynamic_Creature* c3 = new cDynamic_Creature("MeetScene1", RPG_Assets::get().GetSprite("BanditRight"), RPG_Assets::get().GetSprite("BanditLeft"));  // meet werewolf
			cDynamic_ClipTrigger* c3 = new cDynamic_ClipTrigger("MeetScene1");  // meet werewolf
			c3->px = 163;
			c3->py = 27.5;
			c3->CollbordersY = -10;

			vecDyns.push_back(c3);


			//cDynamic_Creature* c4 = new cDynamic_Creature("MeetScene2", RPG_Assets::get().GetSprite("BanditRight"), RPG_Assets::get().GetSprite("BanditLeft"));  //enrence in village
			cDynamic_ClipTrigger* c4 = new cDynamic_ClipTrigger("MeetScene2");
			c4->px = 223;
			c4->py = 29;
			c4->CollbordersY = -10;

			vecDyns.push_back(c4);

			cDynamic_ClipTrigger* c5 = new cDynamic_ClipTrigger("WerefolfReturnCamera");  //mett return camera from killing werewolf
			c5->px = 175;
			c5->py = 26;
			vecDyns.push_back(c5);
			
			cDynamic_Creature* entity = (cDynamic_Creature*)vecDyns[1];
			g_script->AddCommand(new cComand_moveCrowdTo(vecDyns, entity, -30, 25.5, 30, 25.5, 10,100, entity->getWalkData()));
			g_script->AddCommand(new cComand_moveTo(vecDyns[0], 6,25.5, 1));
			g_script->AddCommand(new cComand_ShowDialog({ "StoryTeller:","And our hero ended up in ","the forest" }));



			m_nPhase = 1;

		
		
	

	}

	if (sMap == "VillageInFire")
	{
		g_script->AddCommand(new cComand_moveTo(vecDyns[0], 6, vecDyns[0]->py, 1));
		g_script->AddCommand(new cComand_ShowDialog({ "StoryTeller:","Village in fire","Bandit in the center" }));
	}

	/*if (sMap == "home")
	{

		cDynamic_Creature* c1 = new cDynamic_Creature("bob", RPG_Assets::get().GetSprite("player"));
		c1->px = 12.0f;
		c1->py = 4.0f;
		vecDyns.push_back(c1);


	}*/
	if (sMap == "Village"&& m_nPhase == 4)   // <--After killing bandit Boss
	{
			//g_script->AddCommand(new cComand_moveTo(vecDyns[0], 6, vecDyns[0]->py, 3));
			g_script->AddCommand(new cComand_ShowDialog({ "StoryTeller:","the village was liberated from enemies","But people need your help" }));
			m_nPhase = 5;	

	}
	
	if (sMap == "Forest" && m_nPhase > 4)   // <--After killing bandit Boss  we open the way in cave
	{
		cDynamic* g3 = new cDynamic_creature_NPCBandit("CaveEntrence");
		g3->px = 180.0f;
		g3->py = 28.0f;
		vecDyns.push_back(g3);

		g3 = new cDynamic_creature_NPCBandit("CaveOut");
		g3->px = 249.0f;
		g3->py = 55.0f;
		vecDyns.push_back(g3);
	}

	

	return false;
}

bool cQuset_MainQuest::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE Nature)
{

	
	if (target->sName == "MeetScene" && Nature == WALK && m_nPhase == 1)
	{
		cDynamic* Gena = nullptr;

		for (auto& quest : vecDynobs)
			if (quest->sName == "FirstBandit")
			{
				Gena = quest;
			
				break;
			}
			g_script->AddCommand(new cComand_waiting(Gena));

			g_script->AddCommand(new cComand_ShowDialog({ "Bandit","Hey wait","a little patience" }));
			g_script->AddCommand(new cComand_ShowDialog({ "Bandit:","While i'm"," killing you" }));

			g_script->AddCommand(new cComand_AddQuest(new cQuset_FirstBandit(Gena)));
			g_script->AddCommand(new cComand_Unlock(Gena));

			//sDescription = "Kill Bandit";
		
			
			target->setFlag(target->bDead);
			
		
			
			m_nPhase = 2;

			return  true;
	}


	if (target->sName == "MeetScene1" && Nature == WALK && m_nPhase == 2)   //Werewolf trigger
	{
		cDynamic* Gena = nullptr;
		for (auto& quest : vecDynobs)
			if (quest->sName == "werewolf")
			{
				Gena = quest;

				break;
			}


		g_script->AddCommand(new cComand_waiting(Gena));


		g_script->AddCommand(new cComand_LockCamera);
		g_script->AddCommand(new cComand_moveCamera(target, 175, 27.5, 1));  // <--Camera in center

		

		g_script->AddCommand(new cComand_ShowDialog({ "Man:","Another one"}));

		//g_script->AddCommand(new cComand_moveTo(vecDynobs[0], 161, 27.5, 0.1));
		g_script->AddCommand(new cComand_moveTo(vecDynobs[0], 171, 27.5, 2));
		g_script->AddCommand(new cComand_ShowDialog({ "Man:","I won't find peace "," until I kill you all" }));

		g_script->AddCommand(new cComand_AddQuest(new cQuset_KillWerewolf()));
		g_script->AddCommand(new cComand_PlayAnimation(Gena,25));
		g_script->AddCommand(new cComand_ShowDialog({ "Werewolf:","Roooooaaarrrr!!!" }));
		g_script->AddCommand(new cComand_ShowDialog({ "WereWolf:","you shouldn't have come to these places..." }));
		g_script->AddCommand(new cComand_PlayFunction(Gena));
		g_script->AddCommand(new cComand_Unlock(Gena));
		sDescription = "Kill WereWolf";


		target->setFlag(target->bDead);



		return  true;
	}


	if (target->sName == "MeetScene2" && Nature == WALK && m_nPhase == 3)   // Getting close to village 
	{

		g_script->AddCommand(new cComand_ShowDialog({ "StoryTeller:","it was too late..." }));
		g_script->AddCommand(new cComand_ShowDialog({ "StoryTeller:","Bandits in village..." }));
		g_script->AddCommand(new cComand_moveTo(vecDynobs[0], vecDynobs[0]->px, 30.5, 0.1));
		g_script->AddCommand(new cComand_moveTo(vecDynobs[0], vecDynobs[0]->px + 20, 30.5, 3));
		g_script->AddCommand((new cComand_HideScreen(2)));
		g_script->AddCommand(new cComand_Changemap("VillageInFire", -1, 12));
		//g_script->AddCommand(new cComand_moveTo(vecDynobs[0], vecDynobs[0]->px + 10, vecDynobs[0]->py, 2));
		target->bDead;

		return false;
	}







	return false;
}

cQuset_FirstBandit::cQuset_FirstBandit(cDynamic* bob)
{
	Bob = bob;
	banditsCount = 0;
	sName ="FirstBanditMeeting";
	sDescription = "Kill FirstBandit";
	SaveSlot = 1;  //  because we have quest when need kill 5 banditos
}

bool cQuset_FirstBandit::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE Nature)
{
	if (target->sName == "FirstBandit" && Nature == KILL && m_nPhase ==0)   // Talking with first Bandit
	{
		

		
		g_script->AddCommand(new cComand_CheatDeath(target));

		
		g_script->AddCommand(new cComand_ShowDialog({ "Bandit:", "Well Done", "But our boss", "he's really strong" }));
		g_script->AddCommand(new cComand_CleanDeath(vecDynobs, "Fake"));
		sDescription = "Kill 5 bandits";
		g_script->AddCommand(new cComand_CreateItem(target, vecDynobs));

		m_nPhase = 1;

		return true;

	}


	if (target->sName == "Bandit" && Nature == KILL && m_nPhase == 1)   // counting after talking
	{
		banditsCount++;
		sDescription = "Kill 5 bandits: "+std::to_string(banditsCount);

		if (banditsCount ==5 && m_nPhase == 1)
	    {
		m_nPhase = 2;
		//bCompleted = true;
		sDescription = "Meet the village elder " + std::to_string(banditsCount);
		}

	}
	



	return false;
}

cQuset_KillWerewolf::cQuset_KillWerewolf()
{
	
	m_nPhase = 0;
	sName = "KillWerewolf";
	SaveSlot = 2;
	
}

bool cQuset_KillWerewolf::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE Nature)
{

	if (target->sName == "werewolf" && Nature == KILL && m_nPhase == 0)   // Talking with first Bandit
	{
		cDynamic* ReturnCamera = nullptr;
		//cDynamic* FakeWerewolf = nullptr;
		for (auto& quest : vecDynobs)
			if (quest->sName == "WerefolfReturnCamera")
			{
				ReturnCamera = quest;
				break;
			}
		g_script->AddCommand(new cComand_CheatDeath(target));   // original died allready,  but we create copy of it for scene
		g_script->AddCommand(new cComand_ShowDialog({ "Werewolf:", "I..", "Just wont...", "to protect.." }));
		g_script->AddCommand(new cComand_ShowDialog({ "Werewolf:", "My Village..." }));
		
	

		g_script->AddCommand(new cComand_moveTo(vecDynobs,"Fake", 10, 27.5,2)); //Situation when we want to animating fake object. 10 means. that we add current position 10 or -10

		g_script->AddCommand(new cComand_CleanDeath(vecDynobs, "Fake"));  // <--Cleaning fake death puppet

		

		g_script->AddCommand(new cComand_moveCamera(ReturnCamera, vecDynobs[0]->px, 27.5, 1));
		g_script->AddCommand(new cComand_LockCamera);
		//g_script->AddCommand(new cComand_CreateItem(target, vecDynobs));

		g_script->AddCommand(new cComand_AddQuest(new cQuset_KillBanditBoss()));

		
		g_engine->GetQuest("MainQuest")->SetPhase(3);
		g_engine->GetQuest("MainQuest")->sDescription = "Resque Village";
		//ReturnCamera->bDead = true;

		bCompleted = true;

		return false;

	}




	return false;
}

cQuset_KillBanditBoss::cQuset_KillBanditBoss()
{

	
	m_nPhase = 0;
	sName = "KillBanditBoss";
	sDescription = "Kill Bandit Boss";
	SaveSlot = 3;

}

bool cQuset_KillBanditBoss::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE Nature)
{

	if (target->sName == "BossBandit" && Nature == KILL && m_nPhase ==0)   // After Killing Bandit Boss in Village of fire
	{
		g_script->AddCommand(new cComand_ShowDialog({ "Bandit Boss:","New God lead us in the light..." }));
		g_script->AddCommand((new cComand_HideScreen(3)));
		g_script->AddCommand(new cComand_Changemap("Village", vecDynobs[0]->px, vecDynobs[0]->py));
		bCompleted = true;
		g_engine->GetQuest("MainQuest")->SetPhase(4);  //<--When we kill Bandit Boss 4phase for main quest
		g_engine->GetQuest("MainQuest")->sDescription= "Visit Key Points in Village";
		g_script->AddCommand(new cComand_AddQuest(new cQuset_KeyPointsInVillage()));
		return false;
	}
	return false;
}

cQuset_KeyPointsInVillage::cQuset_KeyPointsInVillage()
{
	m_nPhase = 0;
	sName = "KeyPoints";
	SaveSlot = 4;
	sVisLux = "Luxary";
	sVisMap = "Map";
	sVisSave = "SavePlace";
	sVisBlackSmith = "BlackSmith";
	sVisTrainer = "ProfessionTrainer";
	sVisWerehouse = "Werehouse";
	sDescription = sVisLux+"\n" + sVisBlackSmith + "\n" + sVisTrainer + "\n" + sVisSave + "\n" + sVisWerehouse + "\n" + sVisMap;
}

bool cQuset_KeyPointsInVillage::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE Nature)
{
	if (target->sName == "SaveMan" && Nature == TALK && VisitSave ==false)   // After Killing Bandit Boss in Village of fire
	{
		g_script->AddCommand(new cComand_ShowDialog({ "SavePlace:","Here we can Save Our Project..." }));

		VisitSave = true;
		sVisSave = "-";
		sDescription = sVisLux + "\n" + sVisBlackSmith + "\n" + sVisTrainer + "\n" + sVisSave + "\n" + sVisWerehouse + "\n" + sVisMap;
	}

	if (target->sName == "Warehouse" && Nature == TALK&& VisitWerehouse ==false)   // After Killing Bandit Boss in Village of fire
	{
		g_script->AddCommand(new cComand_ShowDialog({ "Warehouse:","Here we can Keep Our Goods..." }));
		g_script->AddCommand((new cComand_SetNgameMod(2)));
		g_script->AddCommand((new cComand_SetPause(1)));
		VisitWerehouse = true;
		sVisWerehouse = "-";
		sDescription = sVisLux + "\n" + sVisBlackSmith + "\n" + sVisTrainer + "\n" + sVisSave + "\n" + sVisWerehouse + "\n" + sVisMap;
	}
	if (target->sName == "LuxarySeller" && Nature == TALK&& VisitiLux ==false)   // After Killing Bandit Boss in Village of fire
	{
		g_script->AddCommand(new cComand_ShowDialog({ "Luxary:","Here we can Bye Usefool items..." }));
		g_script->AddCommand((new cComand_SetNgameMod(4)));
		g_script->AddCommand((new cComand_SetPause(1)));
		VisitiLux = true;
		sVisLux = "-";
		sDescription = sVisLux + "\n" + sVisBlackSmith + "\n" + sVisTrainer + "\n" + sVisSave + "\n" + sVisWerehouse + "\n" + sVisMap;
	}
	if (target->sName == "BlackSmith" && Nature == TALK && VisitBlacksmith ==false)   // After Killing Bandit Boss in Village of fire
	{
		g_script->AddCommand(new cComand_ShowDialog({ "BlackSmith:","Here we can Craft items..." }));
		VisitBlacksmith = true;
		sVisBlackSmith = "-";
		sDescription = sVisLux + "\n" + sVisBlackSmith + "\n" + sVisTrainer + "\n" + sVisSave + "\n" + sVisWerehouse + "\n" + sVisMap;
	}
	if (target->sName == "Map" && Nature == TALK && VisitMap == false)   // After Killing Bandit Boss in Village of fire
	{
		g_script->AddCommand(new cComand_ShowDialog({ "Map:","Here we can Travel in other places..." }));
		g_script->AddCommand(new cComand_ShowDialog({ "Map:","But beware...","Must be strong enough to get in other places"}));
		g_script->AddCommand((new cComand_SetNgameMod(5)));
		g_script->AddCommand((new cComand_SetPause(1)));
		VisitMap = true;
		sVisMap = "-";
		sDescription = sVisLux + "\n" + sVisBlackSmith + "\n" + sVisTrainer + "\n" + sVisSave + "\n" + sVisWerehouse + "\n" + sVisMap;
	}
	if (target->sName == "ProfessionMan" && Nature == TALK && VisitTrainer == false)   // After Killing Bandit Boss in Village of fire
	{
		g_script->AddCommand(new cComand_ShowDialog({ "ProfessionTrainer:","Hello..." }));
		g_script->AddCommand(new cComand_ShowDialog({ "ProfessionTrainer:","I help you Teach...","New abilities..." }));
		g_script->AddCommand((new cComand_SetNgameMod(6)));
		g_script->AddCommand((new cComand_SetPause(1)));
		VisitTrainer = true;
		sVisTrainer = "-";
		sDescription = sVisLux + "\n" + sVisBlackSmith + "\n" + sVisTrainer + "\n" + sVisSave + "\n" + sVisWerehouse + "\n" + sVisMap;
	}

	if (VisitMap && VisitBlacksmith && VisitiLux && VisitWerehouse && VisitSave && VisitTrainer)
	{
		m_nPhase = 1;
		sDescription = "Complete";
	}

	return false;
}
