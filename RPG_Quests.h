#pragma once

#include "RPG_Dynamic.h"
#include "RPG_Commands.h"

class RPG_Engine;

class cQuest
{
public:
	enum NATURE
	{
		TALK = 0,
		ATTACK = 1,
		KILL = 2,
		WALK = 3
	};

public:
	cQuest();

public:
	virtual bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE Nature);
	virtual bool PopulateDynamics(std::vector<cDynamic*>& vecDyns, std::string sMap);
	virtual void SetPhase(int phase) { };
	virtual int GetPhase() { return 0; };


public:
	std::string sName;
	std::string sDescription;
	bool bCompleted = false;
	static cScriptProcessor* g_script;
	static RPG_Engine* g_engine;
	
	int SaveSlot;


	
};


class cQuset_MainQuest : public cQuest
{
public:
	cQuset_MainQuest();

	bool PopulateDynamics(std::vector<cDynamic*>& vecDyns, std::string sMap) override;
	bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE Nature)override;
	void SetPhase(int phase)override { m_nPhase = phase; };
	int GetPhase() { return m_nPhase; };
	
private:

	int m_nPhase = 0;

};


class cQuset_FirstBandit : public cQuest
{
public:

	cQuset_FirstBandit(cDynamic* Bob =nullptr);

	
	bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE Nature)override;
	void SetPhase(int phase)override { m_nPhase = phase; };
	int GetPhase() { return m_nPhase; };

private:

	cDynamic* Bob = nullptr;
	int m_nPhase = 0;
	int banditsCount = 0;
};

class cQuset_KillWerewolf : public cQuest
{
public:

	cQuset_KillWerewolf();

	bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE Nature)override;
	void SetPhase(int phase)override { m_nPhase = phase; };
	int GetPhase() { return m_nPhase; };

private:

	cDynamic* Bob = nullptr;
	int m_nPhase = 0;

};

class cQuset_KillBanditBoss : public cQuest
{
public:

	cQuset_KillBanditBoss();

	bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE Nature)override;
	void SetPhase(int phase)override { m_nPhase = phase; };
	int GetPhase() { return m_nPhase; };

private:

	cDynamic* Bob = nullptr;
	int m_nPhase = 0;

};

class cQuset_KeyPointsInVillage : public cQuest
{
public:

	cQuset_KeyPointsInVillage();

	bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE Nature)override;
	void SetPhase(int phase)override { m_nPhase = phase; };
	int GetPhase() { return m_nPhase; };

private:

	cDynamic* Bob = nullptr;
	int m_nPhase = 0;
	bool VisitiLux;
	std::string sVisLux;

	bool VisitWerehouse;
	std::string sVisWerehouse;

	bool VisitSave;
	std::string sVisSave;

	bool VisitBlacksmith;
	std::string sVisBlackSmith;

	bool VisitMap;
	std::string sVisMap;

	bool VisitTrainer;
	std::string sVisTrainer;


};

