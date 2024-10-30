#pragma once

#include "RPG_Dynamic.h"
#include <list>



class RPG_Engine;
class cQuest;

class cComand
{
public:

	cComand() {}
	virtual ~cComand() { }


	bool bStarted = false;
	bool bCompleted = false;

	virtual void Start() {};
	virtual void Update(float felapsedTime) {};

	static RPG_Engine* g_engine;




};

class cScriptProcessor
{

public:
	cScriptProcessor();

public:
	void AddCommand(cComand* cmd);
	void ProcessCommands(float fElapsedTime);
	void CompleteCommand();

public:

	bool bUserControlEnabled;

private:
	std::list<cComand*> m_listCommands;
};


class cComand_SaverFunction : public cComand
{
public:
	cComand_SaverFunction() {};
	void Start()override;
	
};

class cComand_moveTo : public cComand
{
public:
	cComand_moveTo(cDynamic* object, float x, float y, float duration = 0.0f);
	cComand_moveTo(std::vector<cDynamic*>& vecDynobs,  std::string name, float x, float y,  float duration = 0.0f);
	void Start() override;
	void Update(float fElapsedTime) override;

private:
	cDynamic* m_pObject;
	std::vector<cDynamic*>* m_vecDyn;
	std::string Name;
	float m_fStartPosX;
	float m_fStartPosY;
	float m_fTargetPosX;
	float m_fTargetPosY;
	float m_fDuration;
	float m_fTimeSoFar;
};

class cComand_moveCrowdTo : public cComand
{
public:
	cComand_moveCrowdTo(std::vector<cDynamic*>& vecDynobs,cDynamic_Creature* object,float Startx,float Starty, float x, float y, float duration = 0.0f, int Count = 0, uint32_t FrameData = 0);
	void Start() override;
	void Update(float fElapsedTime) override;

private:
	std::vector<cDynamic*>* m_vecDyn;
	cDynamic* m_pObject;
	olc::Decal* currDecal;
	float m_fStartPosX;
	float m_fStartPosY;
	float m_fTargetPosX;
	float m_fTargetPosY;
	int Count;
	float m_fDuration;
	float m_fTimeSoFar;
};


class cComand_JumpTo : public cComand
{
public:
	cComand_JumpTo(cDynamic* object, float x, float y, float jumpAmp ,float duration = 0.0f);
	void Start() override;
	void Update(float fElapsedTime) override;

private:
	cDynamic* m_pObject;
	float m_fStartPosX;
	float m_fStartPosY;
	float m_fTargetPosX;
	float m_fTargetPosY;
	float m_fDuration;
	float m_fTimeSoFar;
	float jumpAmplitude;
	float m_Xdist;
};


class cComand_moveCamera : public cComand
{
public:
	cComand_moveCamera(cDynamic* object,float x, float y, float duration = 0.0f);
	void Start() override;
	void Update(float fElapsedTime) override;

private:
	cDynamic* m_pObject;
	float m_fStartPosX;
	float m_fStartPosY;
	float m_fTargetPosX;
	float m_fTargetPosY;
	float m_fDuration;
	float m_fTimeSoFar;
};


class cComand_AppearScreen : public cComand
{
public:
	cComand_AppearScreen( float duration = 0.0f);
	void Start() override;
	void Update(float fElapsedTime) override;

private:
	float m_fTimeSoFar;
	float m_fDuration;
	float oneproc;

};



class cComand_HideScreen : public cComand
{
public:
	cComand_HideScreen(float duration = 0.0f);
	void Start() override;
	void Update(float fElapsedTime) override;

private:
	float m_fTimeSoFar;
	float m_fDuration;
	float oneproc;

};


class cComand_LoadFunction : public cComand
{

public:
	cComand_LoadFunction();
	
	void Start() override;


private:
	
};

class cComand_SaveFunction : public cComand
{

public:
	cComand_SaveFunction();

	void Start() override;


private:

};



class cComand_CalculateExp : public cComand
{

public:
	

	void Start() override;


private:

};
class cComand_ResetQuestList : public cComand
{

public:


	void Start() override;


private:

};




class cComand_waiting : public cComand
{
public:
	cComand_waiting(cDynamic* object);
	void Start() override;
private:
	cDynamic* m_pObject;
};


class cComand_Changemap : public cComand
{
public:
	cComand_Changemap(std::string name, float px, float py);
	void Start() override;
private:
	std::string Name;
	float px, py;
};





class cComand_CreateItem : public cComand
{
public:
	cComand_CreateItem(cDynamic* object, std::vector<cDynamic*>& vecDynobs,cItem* ItemName);
	void Start() override;
private:
	cDynamic* m_pObject;
	cItem* itemname;
	std::vector<cDynamic*>* vecDyn;
};



class cComand_CheatDeath : public cComand
{
public:
	cComand_CheatDeath(cDynamic* object);
	void Start() override;
private:
	cDynamic_Creature* m_pObject;
	//std::vector<cDynamic*>* vecDyn;
	olc::Decal* RightTex;
	olc::Decal* LeftTex;
};



class cComand_SetDeath : public cComand
{
public:
	cComand_SetDeath(cDynamic* object);
	void Start() override;
private:
	cDynamic* m_pObject;
	
};

class cComand_CreatePantir : public cComand
{
public:
	cComand_CreatePantir(cDynamic_creature_Pantir*& object, std::vector<cDynamic*>& vecDynobs);
	void Start() override;
private:
	cDynamic_creature_Pantir* m_pObject;
	std::vector<cDynamic*>* vecDyn;
};


class cComand_CleanDeath : public cComand        // clean puppet Death
{
public:
	cComand_CleanDeath(std::vector<cDynamic*>& vecDynobs, std::string name);
	void Start() override;
private:
	std::vector<cDynamic*>* vecDyn;
	std::string Name;
};


class cComand_Unlock : public cComand    //unlcok enemy
{
public:
	cComand_Unlock(cDynamic* object);
	void Start() override;

private:
	cDynamic* m_pObject;

};

class cComand_LockCamera : public cComand
{
public:
	void Start() override;

private:
};

class cComand_SetNgameMod : public cComand
{
public:
	cComand_SetNgameMod(int mod);

	void Start() override;

private:
	int mode;
};


class cComand_SetPause : public cComand
{
public:
	cComand_SetPause(int mod);

	void Start() override;

private:
	int mode;
};


class cComand_ShowDialog : public cComand
{

public:
	cComand_ShowDialog(std::vector<std::string> line);
	void Start() override;

private:
	std::vector<std::string> vecLines;

};

class cComand_PlayFunction : public cComand
{

public:
	cComand_PlayFunction(cDynamic* character);
	void Start() override;

private:
	cDynamic* chars;

};



class cComand_PlayAnimation : public cComand
{

public:
	cComand_PlayAnimation(cDynamic* character,int numberplay);
	void Start() override;

private:
	cDynamic* m_pObject;
	int animNumb;
};




class cComand_AddQuest : public cComand
{
public:
	cComand_AddQuest(cQuest* quest);
	void Start()override;


private:
	cQuest* m_quest;


};