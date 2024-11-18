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
	if (sMap == "Forest"&& m_nPhase <=3 )
	{
	
			

			olc::vf2d pos{ 81,26 };
			cDynamic* g1 = g_engine->SpawnBandit(pos);
			//g1->setFlag(g1->quested);  // when we don't want to add him in initializepool after death
			g1->sName = "FirstBandit";
			g1->px = 81;
			g1->py = 26;
			g1->setFlag(g1->quested);
			g1->clearFlag(g1->bControllable);
			//vecDyns.push_back(g1);


			pos.x = 178;
			pos.y = 26;
			cDynamic_creature_WereWolf* z1 = (cDynamic_creature_WereWolf *)g_engine->SpawnWerewolf(pos);
			z1->sName = "werewolf";
			z1->bTransformed = false;
			z1->setFlag(z1->quested);
			//z1->setFlag(z1->quested) ;
			z1->clearFlag(z1->bControllable);
			z1->px = 178;
			z1->py = 26;
		//	vecDyns.push_back(z1);

			cDynamic_ClipTrigger* c2 = new cDynamic_ClipTrigger("MeetScene");  //mett first bandit
			c2->px = 78;
			c2->py = 26;
			c2->CollbordersY = -10;

			vecDyns.push_back(c2);

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
			
		
			g_script->AddCommand(new cComand_moveTo(vecDyns[0], 6,25.5, 1));
			g_script->AddCommand(new cComand_ShowDialog({ "StoryTeller:","And our hero ended up in ","the forest" }));
		//	g_script->AddCommand(new cComand_SaverFunction());


			m_nPhase = 1;

			return false;
		
	

	}

	if (sMap == "VillageInFire" && m_nPhase <=4)
	{
	

		cDynamic_ClipTrigger* c3 = new cDynamic_ClipTrigger("MeetScene3");  // meet Boss
		c3->px = 30;
		c3->py = 12;
		c3->CollbordersY = -10;

		vecDyns.push_back(c3);

		g_script->AddCommand(new cComand_SaverFunction());

		cDynamic* BossFake = g_engine->SpawnBossBandt({ 3,12 });
		

		g_script->AddCommand(new cComand_CheatDeath(BossFake));
		g_script->AddCommand(new cComand_ShowDialog({ "Boss:","C'mon boys","Grab their asses..." }));
		g_script->AddCommand(new cComand_ShowDialog({ "Boss:","i gonna meet","some pussy"}));
		g_script->AddCommand(new cComand_moveTo(BossFake, 18, 12, 2));
		g_script->AddCommand(new cComand_CleanDeath(vecDyns, BossFake));
		cDynamic_Creature* entity = (cDynamic_Creature*)vecDyns[1];    // Crowd
		g_script->AddCommand(new cComand_moveCrowdTo(vecDyns, entity, -23, 13.5,
																	   18, 13.5,
																			   8, 50, entity->getWalkData()));
		g_script->AddCommand(new cComand_moveTo(vecDyns[0], 6, 13.5, 1));
		g_script->AddCommand(new cComand_ShowDialog({ "StoryTeller:","Village in fire","Bandit in the center" }));



		return false;
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
			g_script->AddCommand(new cComand_SaverFunction());
			g_script->AddCommand(new cComand_ShowDialog({ "StoryTeller:","the village was liberated from enemies","But people need your help" }));

			m_nPhase = 5;	
			return false;
	}
	
	if (sMap == "Forest" && m_nPhase > 4)   // <--After killing bandit Boss  we open the way in cave
	{
		
		cDynamic* g3 = g_engine->GetNpc(RPG_Engine::NpcStruct::CaveEntrance);
		g3->px = 180.0f;
		g3->py = 28.0f;
		vecDyns.push_back(g3);

		g3 = g_engine->GetNpc(RPG_Engine::NpcStruct::CaveOut);
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

			g_script->AddCommand(new cComand_ShowDialog({ "Bandit:","Hey wait...","a little patience" }));
			g_script->AddCommand(new cComand_ShowDialog({ "Bandit:","While i'm"," killing you" }));

			int quest = static_cast<int>(RPG_Engine::QuestStruct::FIRSTBANDIT);
			g_script->AddCommand(new cComand_AddQuest(RPG_Assets::get().GetActiveQuest(quest)));
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

		//g_engine->GetQuest("KillWerewolf");

		int quest = static_cast<int>(RPG_Engine::QuestStruct::KILLWEREWOLF);

		g_script->AddCommand(new cComand_AddQuest(RPG_Assets::get().GetActiveQuest(quest)));
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



	if (target->sName == "MeetScene3" && Nature == WALK)   // Getting close to village 
	{

		g_script->AddCommand(new cComand_LockCamera);
		g_script->AddCommand(new cComand_moveCamera(target, 47, 13.5, 1));  // <--Camera in center
		g_script->AddCommand(new cComand_ShowDialog({ "Bandit Boss:","well well well" }));
		g_script->AddCommand(new cComand_moveTo(vecDynobs[0], vecDynobs[0]->px, 13.5f, 0.1));
		g_script->AddCommand(new cComand_moveTo(vecDynobs[0], 43, 13.5f, 3));
		g_script->AddCommand(new cComand_ShowDialog({ "Bandit Boss:","I eat you and all in this","Village"}));

		//g_script->AddCommand(new cComand_moveTo(vecDynobs[0], vecDynobs[0]->px + 10, vecDynobs[0]->py, 2));
		target->bDead;

		return false;
	}




	return false;
}

void cQuset_MainQuest::makeActual()
{
	{
		switch (m_nPhase)
		{
			case 3:
			sDescription = "Resque Village";
			break;

			case 4:
				sDescription = "Visit Key Points in Village";
			break;

			default :
				break;
		}

	};

}



cQuset_FirstBandit::cQuset_FirstBandit(cDynamic* bob)
{
	Bob = bob;
	banditsCount = 0;
	sName ="First Bandit Meeting";
	sDescription = "Kill FirstBandit";
	SaveSlot = 1;  //  because we have quest when need kill 5 banditos
}

bool cQuset_FirstBandit::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE Nature)
{

	switch (m_nPhase)
	{
	case 0:

		if (target->sName == "FirstBandit" && Nature == KILL)   // Talking with first Bandit
		{



			g_script->AddCommand(new cComand_CheatDeath(target));


			g_script->AddCommand(new cComand_ShowDialog({ "Bandit:", "Well Done", "But our boss", "he's really strong" }));
			g_script->AddCommand(new cComand_CleanDeath(vecDynobs, target));
			sDescription = "Kill 5 bandits";
			g_script->AddCommand(new cComand_CreateItem(target, vecDynobs, RPG_Assets::get().GetItem("Broken Sword")));

			m_nPhase = 1;




			return true;

		}
		break;
	case 1:

		if (target->sName == "Bandit" && Nature == KILL && m_nPhase == 1)   // counting after talking
		{
			banditsCount++;
			sDescription = "Kill 5 bandits  " + std::to_string(banditsCount) + "/5";

			if (banditsCount == 5)
			{
				m_nPhase = 2;
				//bCompleted = true;
				Bob = g_engine->GetNpc(RPG_Engine::NpcStruct::luxary);
				sDescription = "Meet the village elder\n in tavern " + Bob->sName;
			}
			return true;
		}
		break;
	case 2:


		if (target == Bob)
		{
			g_script->AddCommand(new cComand_ShowDialog({ "Elder:", "Well Done", "You helped us"}));
			bCompleted =true;
			g_engine->AddCompletedQuest(this);
		}
		return true;

		break;
	}



	
	



	return false;
}

void cQuset_FirstBandit::makeActual()
{
	switch (m_nPhase)
	{
	case 1:
		sDescription = "Kill 5 bandits";
		break;
	case 2:
		Bob = g_engine->GetNpc(RPG_Engine::NpcStruct::luxary);
		sDescription = "Meet the village elder\n in tavern " + Bob->sName;
		break;
	}
}

cQuset_KillWerewolf::cQuset_KillWerewolf()
{
	
	m_nPhase = 0;
	sName = "KillWerewolf";
	SaveSlot = 2;
	
}

bool cQuset_KillWerewolf::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE Nature)
{

	if (target->sName == "werewolf" && Nature == KILL && m_nPhase == 0) 
	{
 		cDynamic* ReturnCamera = nullptr;
		//cDynamic* FakeWerewolf = nullptr;
		for (auto& quest : vecDynobs)
			if (quest->sName == "WerefolfReturnCamera")
			{
				ReturnCamera = quest;
				break;
			}
		
		cDynamic_creature_WereWolf* Werewolf = (cDynamic_creature_WereWolf*)target;
		//Werewolf->DeathWolfs();
	//	Werewolf->SetHealth(1);
	//	Werewolf->clearFlag(Werewolf->bDead);
		g_script->AddCommand(new cComand_CheatDeath(target));   // original died allready,  but we create copy of it for scene
		g_script->AddCommand(new cComand_ShowDialog({ "Werewolf:", "I..", "Just want...", "to protect.." }));
		g_script->AddCommand(new cComand_ShowDialog({ "Werewolf:", "My Village..." }));
		
	

		g_script->AddCommand(new cComand_moveTo(target, 185, 27.5,2)); //Situation when we want to animating fake object. 10 means. that we add current position 10 or -10

		g_script->AddCommand(new cComand_CleanDeath(vecDynobs, target));  // <--Cleaning fake death puppet

		
		g_script->AddCommand(new cComand_moveCamera(ReturnCamera, vecDynobs[0]->px, 27.5, 1));
		g_script->AddCommand(new cComand_LockCamera);
		//g_script->AddCommand(new cComand_CreateItem(target, vecDynobs));

		int quest = static_cast<int>(RPG_Engine::QuestStruct::KILLBANDITBOSS);
		g_script->AddCommand(new cComand_AddQuest(RPG_Assets::get().GetActiveQuest(quest)));

		
		g_engine->GetActiveQuest("MainQuest")->SetPhase(3);
		g_engine->GetActiveQuest("MainQuest")->sDescription = "Resque Village";
		//ReturnCamera->bDead = true;

		bCompleted = true;
		g_engine->AddCompletedQuest(this);
		return false;

	}




	return false;
}

cQuset_KillBanditBoss::cQuset_KillBanditBoss()
{

	Bob = g_engine->GetBoss(RPG_Engine::BossStruct::BanditBoss);
	m_nPhase = 0;
	sName = "KillBanditBoss";
	sDescription = "Kill Bandit Boss";
	SaveSlot = 3;

}

bool cQuset_KillBanditBoss::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE Nature)
{

	if (target == Bob && Nature == KILL && m_nPhase ==0)   // After Killing Bandit Boss in Village of fire
	{
		g_script->AddCommand(new cComand_ShowDialog({ "Bandit Boss:","New God lead us on the way of light..." }));
		g_script->AddCommand((new cComand_HideScreen(3)));
		g_script->AddCommand(new cComand_Changemap("Village", vecDynobs[0]->px, vecDynobs[0]->py));
		g_engine->GetActiveQuest("MainQuest")->SetPhase(4);  //<--When we kill Bandit Boss 4phase for main quest
		g_engine->GetActiveQuest("MainQuest")->sDescription= "Visit Key Points in Village";

		bCompleted = true;
		g_engine->AddCompletedQuest(this);
		int quest = static_cast<int>(RPG_Engine::QuestStruct::KEYPOINTSINVILLAGE);
		g_script->AddCommand(new cComand_AddQuest(RPG_Assets::get().GetActiveQuest(quest)));
		return false;
	}
	return false;
}

void cQuset_KillBanditBoss::makeActual()
{
	Bob = g_engine->GetBoss(RPG_Engine::BossStruct::BanditBoss);

}

cQuset_KeyPointsInVillage::cQuset_KeyPointsInVillage()
{
	m_nPhase = 0;
	sName = "KeyPoints";
	SaveSlot = 4;
	sVisLux = g_engine->GetNpc(RPG_Engine::NpcStruct::luxary);
	sVisMap = g_engine->GetNpc(RPG_Engine::NpcStruct::Map);
	sVisSave = g_engine->GetNpc(RPG_Engine::NpcStruct::SaveMan);
	sVisBlackSmith = g_engine->GetNpc(RPG_Engine::NpcStruct::blacksmith);
	sVisTrainer = g_engine->GetNpc(RPG_Engine::NpcStruct::proffesion);
	sVisWerehouse = g_engine->GetNpc(RPG_Engine::NpcStruct::Warehouse);
	updateQuestNaming();
}

bool cQuset_KeyPointsInVillage::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE Nature)
{
	if (target == sVisSave && Nature == TALK && VisitSave ==false)   // After Killing Bandit Boss in Village of fire
	{
		g_script->AddCommand(new cComand_ShowDialog({ "SavePlace:","Here we can Save Our Project..." }));

		VisitSave = true;
		sVisSave = nullptr;
		updateQuestNaming();
	}

	if (target == sVisWerehouse && Nature == TALK&& VisitWerehouse ==false)   // After Killing Bandit Boss in Village of fire
	{
		g_script->AddCommand(new cComand_ShowDialog({ "Warehouse:","Here we can Keep Our Goods..." }));
		g_script->AddCommand((new cComand_SetNgameMod(2)));
		g_script->AddCommand((new cComand_SetPause(1)));
		VisitWerehouse = true;
		sVisWerehouse = nullptr;
		updateQuestNaming();
	}
	if (target == sVisLux && Nature == TALK&& VisitiLux ==false)   // After Killing Bandit Boss in Village of fire
	{
		g_script->AddCommand(new cComand_ShowDialog({ "Luxary:","Here we can Bye Usefool items..." }));
		g_script->AddCommand((new cComand_SetNgameMod(4)));
		g_script->AddCommand((new cComand_SetPause(1)));
		VisitiLux = true;
		sVisLux = nullptr;
		updateQuestNaming();
	}
	if (target == sVisBlackSmith && Nature == TALK && VisitBlacksmith ==false)   // After Killing Bandit Boss in Village of fire
	{
		g_script->AddCommand(new cComand_ShowDialog({ "BlackSmith:","Here we can Craft items..." }));
		VisitBlacksmith = true;
		sVisBlackSmith =nullptr;
		updateQuestNaming();
	}
	if (target == sVisMap && Nature == TALK && VisitMap == false)   // After Killing Bandit Boss in Village of fire
	{
		g_script->AddCommand(new cComand_ShowDialog({ "Map:","Here we can Travel in other places..." }));
		g_script->AddCommand(new cComand_ShowDialog({ "Map:","But beware...","Must be strong enough to get in other places"}));
		g_script->AddCommand((new cComand_SetNgameMod(5)));
		g_script->AddCommand((new cComand_SetPause(1)));
		VisitMap = true;
		sVisMap = nullptr;
		updateQuestNaming();
	}
	if (target== sVisTrainer && Nature == TALK && VisitTrainer == false)   // After Killing Bandit Boss in Village of fire
	{
		g_script->AddCommand(new cComand_ShowDialog({ "ProfessionTrainer:","Hello..." }));
		g_script->AddCommand(new cComand_ShowDialog({ "ProfessionTrainer:","I help you Teach...","New abilities..." }));
		g_script->AddCommand((new cComand_SetNgameMod(6)));
		g_script->AddCommand((new cComand_SetPause(1)));
		VisitTrainer = true;
		sVisTrainer = nullptr;
		updateQuestNaming();
	}

	if (VisitMap && VisitBlacksmith && VisitiLux && VisitWerehouse && VisitSave && VisitTrainer)
	{
		m_nPhase = 1;
		sDescription = "Complete";
		bCompleted = true;
		g_engine->AddCompletedQuest(this);
	}

	return false;
}

void cQuset_KeyPointsInVillage::updateQuestNaming()
{
	
		sDescription = sVisLux->sName + "\n" + sVisBlackSmith->sName + "\n" + sVisTrainer->sName + "\n" + sVisSave->sName + "\n" + sVisWerehouse->sName + "\n" + sVisMap->sName;

}
