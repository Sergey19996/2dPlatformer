#include "RPG_Commands.h"
#include "RPG_Engine.h"
#include "RPG_Quests.h"

RPG_Engine* cComand::g_engine = nullptr;

cScriptProcessor::cScriptProcessor()
{

	bUserControlEnabled = true;

}

void cScriptProcessor::AddCommand(cComand* cmd)
{
	m_listCommands.push_back(cmd);
}



void cScriptProcessor::ProcessCommands(float fElapsedTime)
{
	//if command are available, halt user control
	bUserControlEnabled = m_listCommands.empty();

	
	if (!m_listCommands.empty())
	{
		//A command is available
		if (!m_listCommands.front()->bCompleted)
		{
			//Command Has not been started
			if (!m_listCommands.front()->bStarted)
			{
				m_listCommands.front()->Start();              //record previous position in two temp 
				m_listCommands.front()->bStarted = true;      //return in high if
			}

			else //Command has been started so process it
				m_listCommands.front()->Update(fElapsedTime);   //will work before return bcompleted
		}

		else
		{
			//Command has been completed
			delete m_listCommands.front();
			m_listCommands.pop_front();
		}


	}

}

//Marks currently active command as complete, from external sourse
void cScriptProcessor::CompleteCommand()
{
	if (!m_listCommands.empty())
	{
		m_listCommands.front()->bCompleted = true;

	}


}




cComand_moveTo::cComand_moveTo(cDynamic* object, float x, float y, float duration)
{
	m_fTargetPosX = x;
	m_fTargetPosY = y;
	m_fTimeSoFar = 0.0f;
	m_fDuration = std::max(duration, 0.01f); //it's an important to avoid divide by zero
	m_pObject = object;
	
}

cComand_moveTo::cComand_moveTo(std::vector<cDynamic*>& vecDynobs,std::string name ,float x, float y, float duration)
{

	m_vecDyn = &vecDynobs;
	Name = name;
	m_fTargetPosX = x;
	m_fTargetPosY = y;
	m_fTimeSoFar = 0.0f;
	m_fDuration = std::max(duration, 0.01f); //it's an important to avoid divide by zero
}


void cComand_moveTo::Start()
{
	if (m_vecDyn != nullptr)
	{
		for (auto& quest : *m_vecDyn)
			if (quest->sName == Name)
			{
				m_pObject = quest;
				break;
			}
	m_fTargetPosX += m_pObject->px;
	}
	
	m_fStartPosX = m_pObject->px;
	m_fStartPosY = m_pObject->py;

}

void cComand_moveTo::Update(float fElapsedTime)
{
	m_fTimeSoFar += fElapsedTime;
	float t = m_fTimeSoFar / m_fDuration;   //получаем пропорции секундомера от секунд к конечному времени
	if (t > 1.0f) t = 1.0f;


	

	//	object->vy += object->mass * fElapsedTime;                     //Gravitation for everyone
	

	
	m_pObject->vx = (m_fTargetPosX - m_fStartPosX) / m_fDuration;
	m_pObject->vy = (m_fTargetPosY - m_fStartPosY) / m_fDuration;

	m_pObject->px = (m_fTargetPosX - m_fStartPosX) * t + m_fStartPosX;
	m_pObject->py = (m_fTargetPosY - m_fStartPosY) * t + m_fStartPosY;
	


	if (m_fTimeSoFar >= m_fDuration)
	{
		//Object Has reached Destination , so stop
		m_pObject->px = m_fTargetPosX;
		m_pObject->py = m_fTargetPosY;
		m_pObject->vx = 0.0f;
		m_pObject->vy = 0.0f;
		
		bCompleted = true;
	}
}


cComand_ShowDialog::cComand_ShowDialog(std::vector<std::string> line)
{
	vecLines = line;

}

void cComand_ShowDialog::Start()
{
	g_engine->ShowDialog(vecLines);

}


cComand_AddQuest::cComand_AddQuest(cQuest* quest)
{
	m_quest = quest;
}

void cComand_AddQuest::Start()
{
	g_engine->AddQuest(m_quest);
	bCompleted = true;
}

cComand_waiting::cComand_waiting(cDynamic* object)   
{
	m_pObject = object;
}

void cComand_waiting::Start()
{
	m_pObject->setFlag(m_pObject->bControllable);
	m_pObject->vx = 0;
	bCompleted = true;

}

cComand_Unlock::cComand_Unlock(cDynamic* object)
{
	m_pObject = object;
}

void cComand_Unlock::Start()
{
	m_pObject->clearFlag(m_pObject->bControllable);
	bCompleted = true;
}

cComand_CheatDeath::cComand_CheatDeath(cDynamic* object )
{
	

	


	m_pObject =object;

	//cDynamic_creature_WereWolf* fake = new cDynamic_creature_WereWolf();

	//vecDyn->push_back(m_pObject);

}

void cComand_CheatDeath::Start()
{


	if (cDynamic_creature_WereWolf* derivedobj = dynamic_cast<cDynamic_creature_WereWolf*>(m_pObject))
	{
		cDynamic* fake = new cDynamic_creature_WereWolf();

		fake->SetHealth(1);
		fake->sName = "Fake";
		fake->clearFlag(fake->bControllable);
		fake->px = m_pObject->px;
		fake->py = m_pObject->py;

		g_engine->AddVecDynamic(fake);

	}

	if (cDynamic_creature_DireWolf* derivedobj = dynamic_cast<cDynamic_creature_DireWolf*>(m_pObject))
	{
		cDynamic* fake = new cDynamic_creature_DireWolf();

		fake->sName = "Fake";
		fake->clearFlag(fake->bControllable);
		fake->px = m_pObject->px;
		fake->py = m_pObject->py;
		fake->SetHealth(1);
		//vecDyn->push_back(fake);
		g_engine->AddVecDynamic(fake);

	}

	if (cDynamic_creature_Bandit* derivedobj2 = dynamic_cast<cDynamic_creature_Bandit*>(m_pObject))
	{
		cDynamic* fake = new cDynamic_creature_Bandit();

		fake->sName = "Fake";
		fake->clearFlag(fake->bControllable);
		fake->px = m_pObject->px;
		fake->py = m_pObject->py;
		fake->SetHealth(1);
		g_engine->AddVecDynamic(fake);



	}

	if (cDynamic_creature_Boar* deruvedobj3 = dynamic_cast<cDynamic_creature_Boar*>(m_pObject))
	{
		cDynamic* fake = new cDynamic_creature_Boar();


		fake->sName = "Fake";
		fake->clearFlag(fake->bControllable);
		fake->px = m_pObject->px;
		fake->py = m_pObject->py;
		fake->SetHealth(1);
		g_engine->AddVecDynamic(fake);

	}
	

	
	bCompleted = true;
}

cComand_CleanDeath::cComand_CleanDeath(std::vector<cDynamic*>& vecDynobs ,std::string Name)
{
	vecDyn = &vecDynobs;
	this->Name = Name;

}

void cComand_CleanDeath::Start()
{
	for (auto& quest : *vecDyn)
		if (quest->sName == Name)
		{
			quest->setFlag(quest->bDead);

			break;
		}
	bCompleted = true;
	
}

cComand_CreateItem::cComand_CreateItem(cDynamic* object, std::vector<cDynamic*>& vecDynobs)
{
	m_pObject = object;
	vecDyn = &vecDynobs;
}

void cComand_CreateItem::Start()
{
	vecDyn->push_back(new cDynamic_Item(m_pObject->px + m_pObject->CollbordersX, m_pObject->py + m_pObject->CollbordersY, RPG_Assets::get().GetItem("Broken Sword")));

	bCompleted = true;

}

cComand_moveCamera::cComand_moveCamera(cDynamic* object, float x, float y, float duration)
{
	m_fTargetPosX = x;
	m_fTargetPosY = y;
	m_fTimeSoFar = 0.0f;
	m_fDuration = std::max(duration, 0.01f); //it's an important to avoid divide by zero
	m_pObject = object;
}

void cComand_moveCamera::Start()
{
	m_fStartPosX = m_pObject->px;
	m_fStartPosY = m_pObject->py;
}

void cComand_moveCamera::Update(float fElapsedTime)
{


	m_fTimeSoFar += fElapsedTime;
	
	float t = m_fTimeSoFar / m_fDuration;   //получаем пропорции секундомера от секунд к конечному времени
	if (t > 1.0f) t = 1.0f;


	g_engine->fCameraPosX = (m_fTargetPosX - m_fStartPosX) * t + m_fStartPosX;
	g_engine->fCameraPosY = (m_fTargetPosY - m_fStartPosY) * t + m_fStartPosY;
	
	





	if (m_fTimeSoFar >= m_fDuration)
	{
		//Object Has reached Destination , so stop
		g_engine->fCameraPosX = m_fTargetPosX;
		g_engine->fCameraPosY = m_fTargetPosY;
		bCompleted = true;
	}



}

cComand_AppearScreen::cComand_AppearScreen( float duration)
{
	m_fDuration = duration;
	
	m_fTimeSoFar = 0.0f;
	
}

void cComand_AppearScreen::Start()
{
	g_engine->bSmoothAppearScreen = true;
	g_engine->drawPlayer(1);
	oneproc = m_fDuration / 255;
	g_engine->AlphaAppearScreen = 255;
}

void cComand_AppearScreen::Update(float fElapsedTime)
{

	m_fTimeSoFar += fElapsedTime;
	float t = m_fTimeSoFar / m_fDuration;   //получаем пропорции секундомера от секунд к конечному времени
	if (t > 1.0f) t = 1.0f;

		g_engine->AlphaAppearScreen = 255-(t*255);



	if (m_fTimeSoFar >= m_fDuration)
	{
	
		bCompleted = true;
		g_engine->bSmoothAppearScreen = false;
	}

}


void cComand_LockCamera::Start()
{
	g_engine->blockCamera = !g_engine->blockCamera;
	g_engine->SetMouseFocus(true);
	bCompleted = true;
}

cComand_HideScreen::cComand_HideScreen(float duration)
{
	m_fDuration = duration;

	m_fTimeSoFar = 0.0f;
}

void cComand_HideScreen::Start()
{
	g_engine->bSmoothAppearScreen = true;
	//oneproc = m_fDuration / 255;
	g_engine->AlphaAppearScreen = 0;
}

void cComand_HideScreen::Update(float fElapsedTime)
{

	m_fTimeSoFar += fElapsedTime;
	float t = m_fTimeSoFar / m_fDuration;   //получаем пропорции секундомера от секунд к конечному времени
	if (t > 1.0f) t = 1.0f;

	g_engine->AlphaAppearScreen = (t * 255);



	if (m_fTimeSoFar >= m_fDuration)
	{

		bCompleted = true;
		//g_engine->bSmoothAppearScreen = false;
	}
}

cComand_Changemap::cComand_Changemap(std::string name, float px, float py)
{
	this->Name = name;
	this->px = px;
	this->py = py;
}

void cComand_Changemap::Start()
{
	g_engine->ChangeMap(Name, px, py);
	
	bCompleted = true;

}

cComand_SetNgameMod::cComand_SetNgameMod(int mod)
{
	mode = mod;
}

void cComand_SetNgameMod::Start()
{
	g_engine->SetGameMode(mode);
	bCompleted = true;
}

cComand_SetPause::cComand_SetPause(int mod)
{
	mode = mod;
}

void cComand_SetPause::Start()
{
	g_engine->SetPause(mode);
	bCompleted = true;
}

cComand_SetDeath::cComand_SetDeath(cDynamic* object)
{
	m_pObject = object;

}

void cComand_SetDeath::Start()
{
	m_pObject->SetHealth(0);
	m_pObject->setFlag(m_pObject->bDead);
	bCompleted = true;
}

cComand_CreatePantir::cComand_CreatePantir(cDynamic_creature_Pantir*& object, std::vector<cDynamic*>& vecDynobs)
{

	object = m_pObject;
	
}

void cComand_CreatePantir::Start()
{
	
	
	m_pObject =new cDynamic_creature_Pantir();
	bCompleted = true;
}



cComand_LoadFunction::cComand_LoadFunction()
{
}

void cComand_LoadFunction::Start()
{

	g_engine->LoadFunction();
	
	bCompleted =true;
}

void cComand_CalculateExp::Start()
{
	g_engine->CalculateExp();
	bCompleted = true;
}

cComand_SaveFunction::cComand_SaveFunction()
{
}

void cComand_SaveFunction::Start()
{
	g_engine->SaveFunction();
	bCompleted = true;
}

void cComand_ResetQuestList::Start()
{
	g_engine->QuestListClear();
	g_engine->AddQuest(RPG_Assets::get().GetQuest(0));
	RPG_Assets::get().GetQuest(0)->SetPhase(0);
	bCompleted = true;
}

cComand_JumpTo::cComand_JumpTo(cDynamic* object, float x, float y, float jumpAmpl,float duration)
{
	jumpAmplitude = jumpAmpl;
	m_fTargetPosX = x;
	m_fTargetPosY = y;
	m_fTimeSoFar = 0.0f;
	m_fDuration = std::max(duration, 0.01f); //it's an important to avoid divide by zero
	m_pObject = object;
}

void cComand_JumpTo::Start()
{
	m_fStartPosX = m_pObject->px;
	m_fStartPosY = m_pObject->py;

	float distx =  m_fTargetPosX - m_fStartPosX;
	float disty = m_fTargetPosY - m_fStartPosY;


	m_pObject->clearFlag(m_pObject->gravity);
	m_Xdist = sqrtf(distx * distx + disty * disty);
}

void cComand_JumpTo::Update(float fElapsedTime)
{


	m_fTimeSoFar += fElapsedTime;
	float t = m_fTimeSoFar / m_fDuration;   //получаем пропорции секундомера от секунд к конечному времени
	if (t > 1.0f) t = 1.0f;



	float jump = jumpAmplitude * std::sin( 3.14 *t);
	

	//	object->vy += object->mass * fElapsedTime;                     //Gravitation for everyone

	float fnewypos = jumpAmplitude * std::sin(3.14 * (t+0.1f));

	m_pObject->vx = (m_fTargetPosX - m_fStartPosX) / m_fDuration;
	//m_pObject->vy = (m_fTargetPosY - m_fStartPosY) / m_fDuration;

	m_pObject->px = (m_fTargetPosX - m_fStartPosX) * t + m_fStartPosX;
	m_pObject->py = (m_fTargetPosY - m_fStartPosY) * t + m_fStartPosY;

	if (fnewypos-jump  <0.0f)
	{
		m_pObject->vy = 5;   // >-2.8 and <2.8 is zenit stage
	}
	else
	{
		m_pObject->vy = -5;
	}

	m_pObject->py -= jump;

	std::cout << m_pObject->vy << std::endl;

	if (m_fTimeSoFar >= m_fDuration)
	{
		//Object Has reached Destination , so stop
		m_pObject->px = m_fTargetPosX;
		m_pObject->py = m_fTargetPosY;
		m_pObject->vx = 0.0f;
		m_pObject->vy = 0.0f;
		m_pObject->setFlag(m_pObject->gravity);
		bCompleted = true;
	}
}

cComand_PlayAnimation::cComand_PlayAnimation(cDynamic* character, int numberplay)
{
	m_pObject = character;
	animNumb = numberplay;
}

void cComand_PlayAnimation::Start()
{
	cDynamic_Creature* helpObject = (cDynamic_Creature*)m_pObject;
	helpObject->SetAnimation(animNumb);
	bCompleted = true;
}

cComand_PlayFunction::cComand_PlayFunction(cDynamic* character)
{
	chars = character;
}

void cComand_PlayFunction::Start()
{
	cDynamic_Creature* actchar= (cDynamic_Creature*)chars;
	actchar->Action();
	bCompleted = true;
}
