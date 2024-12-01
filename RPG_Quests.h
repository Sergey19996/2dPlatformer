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
	virtual void reset() {};
	virtual void makeActual() {};

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

	void reset() override {
		m_nPhase = 0;

	}
	void makeActual() override;
	

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

	void reset() override {
		m_nPhase = 0;
		Bob = nullptr;
		banditsCount = 0;

	}
	void makeActual() override ;
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

	void reset() override {
		m_nPhase = 0;
		Bob = nullptr;
	
	}

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

	void reset() override {
		m_nPhase = 0;
		Bob = nullptr;

	}
	void makeActual() override;


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

	void updateQuestNaming();

	void reset() override {
		m_nPhase = 0;
		Bob = nullptr;
		VisitiLux = false;
		VisitWerehouse = false;
		VisitSave = false;
		VisitBlacksmith = false;
		VisitMap = false;
		VisitTrainer = false;
		for (int i = 0; i < namesAr.size(); i++)
		{
			namesAr[0] = "\0";

		}

	}
	void makeActual() override;
private:

	cDynamic* Bob = nullptr;
	int m_nPhase = 0;
	bool VisitiLux;
	cDynamic* sVisLux;

	bool VisitWerehouse;
	cDynamic* sVisWerehouse;

	bool VisitSave;
	cDynamic* sVisSave;

	bool VisitBlacksmith;
	cDynamic* sVisBlackSmith;

	bool VisitMap;
	cDynamic* sVisMap;

	bool VisitTrainer;
	cDynamic* sVisTrainer;


	std::array<std::string, 6> namesAr;

};

