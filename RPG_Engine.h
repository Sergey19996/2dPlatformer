#pragma once
#include "olcPixelGameEngine.h"
#include "RPG_Assets.h"
#include "RPG_Dynamic.h"
#include "RPG_Maps.h"
#include "RPG_Items.h"
#include "RPG_UI.h"
#include "RPG_Quests.h"
#include "RPG_Commands.h"
#include "cGameSettings.h"
#include <cstdio>
#include <windows.h> // Include the Windows header for RECT and GetWindowRect
//#include "ParallaxManager.h"

struct TileInfo {
	olc::vf2d position;
	olc::vf2d sourceRect;
	int textureIndex;
	int textureLayer;
	olc::vf2d size{ 64,64 };
};

class RPG_Engine : public olc::PixelGameEngine
{

public:
	RPG_Engine();

	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;
	bool blockCamera = false;

	bool bSmoothAppearScreen = false;
	int AlphaAppearScreen = 255;
	float fscale = 1.0f;
private:

	std::vector<cDynamic*> enemysPool;
	std::vector<Environment*> EnvironmentPool;
	//std::vector<cDynamic*> wolfPool;

	bool bOpenTravelAsk = false; // Map Logic
	int SelectedTeleport = 0;
	//


	bool bPause = false;


	int SparedFacDir = 0;

	// MainMenu
	float MainMenuX = 0.0f;
	float MainMenuY = 0.0f;
	float MainMenuAnim = 0.0f;
	int MainMenuCalc = 0;


	bool bshowfirst = false;
	bool bshowSecond = false;
	bool bshowThird = false;
	bool bshowZero = false;


	

	float ScrollingbeforeX = 0.0f;
	float scrollingbetweenX = 0.0f;
	float ScrollingCurrX = 0.0f;
	float ScrollingAfterX = 0.0f;
	float ScrollingBackX = 0;
	float ScrollingY = 0.0f;


	float fCameraVx = 0.0f;
	float fCameraVy = 0.0f;


	int lvl =1;
	int ExpRequred = 0;     //Amount
	int currExp =0;     
	int Money = 0;


	float framespeed = 0.1f;
	float animsprite = 0.0f;
	int frameIndex = 0;







	//Test
	int point1X =247;
	int point1Y =256;

	int point2X = 743;
	int point2Y = 128;

	float targetpointX =point1X;
	float targetpointY = point1Y;

	float targetpointVx = 0.0f;
	float targetpointVy = 0.0f;

	float speed = 6000;

	//

	//bool bFinishLoading = false;
	bool bUibackstub = false;
	bool PressF = false;
	int layer = 0;
	cMap* m_pCurrentMap = nullptr;
	olc::Decal* m_sprFont = nullptr;
	olc::Decal* m_sprGameCursor = nullptr;
	olc::Decal* m_sprPressF = nullptr;

	std::vector<cDynamic*> m_vecVisibleDynamics;

	cDynamic_creature_Pantir* m_pPlayer = nullptr;                // before was cDynamic_creature
	std::vector<cDynamic*> m_vecDynamics;    // Fixed
	std::vector<Environment*>m_vecCloseWeather; // Vector to store raindrops and clouds
	std::vector<Environment*>m_vecFarWeather; // Vector to store raindrops and clouds
	std::vector<cDynamic*> m_vecProjectiles;  //Transient

	std::vector<cDynamic*> m_vecIndicators;  //Hp Rage Energy and text

	std::vector<cDynamic*> m_vecParticles;


	std::vector<cUI*> m_vecUiBars;  //Transient
	std::vector<cUI*> m_vecTalentsBars;  //Transient
	std::vector<int > m_vecSaveTalents;
	std::vector<cDynamic*> m_vecFightText;  //Fight text
	
	cScriptProcessor m_script;


	std::list<cItem*> m_listStoreItems;
	std::list<cItem*> m_listBlackSmithItems;
	std::list<cItem*> m_listItems;
	std::list<cItem*> m_listWarehouseItems;

	std::list<cQuest*> m_listQusets;


	enum
	{
		MODE_TITLE,
		MODE_LOCAL_MAP,
		MODE_WAREHOUSE,
		MODE_INVENTORY,
		MODE_SHOP,
		MODE_MAP,
		MODE_PROFESSION,
		MODE_BLACKSMITH



	};
	int n_nGameMode = MODE_TITLE;


private:
	HWND hWnd; // Store the window handle internally




	float fAccumulatedTime = 0.0f;

	 int nFrames = 0;

public:
	
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

	


	void SetGameMode(int set) {  n_nGameMode = set; };
	int GetGameMode() { return n_nGameMode; }

	int GetLearnedTalent(int talentsave);
	void SetLearnedTalent(cUI* Talent);

	void SetPause(int set) { bPause = set; };

	bool UpdateTitleScreen(float fElapsedTime);
	bool UpdateLocalMap(float fElapsedTime);
	bool UpdateWarehouse(float fElapsedTime);
	void DrawWarehouse(const float squex, const float squeYm, olc::vf2d mouse, olc::vf2d mousefix, cItem*& highlighted, cItem*& Grabitem,int moneyamount);

	bool SetMouseTarget(olc::vf2d mouse);

	bool UpdateInventory(float fElapsedTIme);
	void DrawInventory(float offestX,float offsetY, olc::vf2d mouse, cItem*& highlighted);
	void DrawInventory(float offestX, float offsetY, olc::vf2d mouse, olc::vf2d mouseFixed,  cItem*& highlighted, cItem*& Grabitem);
	void moveteItems(olc::vf2d mouse, float x, float y, cItem*& grabitem, std::list<cItem*> vector);


	void FillBatch(int TileLayer, int layer, int x, int y,std::vector<TileInfo>& Before, std::vector<TileInfo>& After);



	bool UpdateShop(float fElapsedTime);
	void DrawStoreInventory(float sX, float sY, olc::vf2d mousefix, cItem*& highlighted);
	bool UpdateProfession(float fElapsedTime);
	bool UpdateMap(float FelapsedTime);
	bool UpdateBlackSmith(float FelapsedTime);

	void DrawBlacksmithInentory(float offestX, float offsetY, olc::vf2d mouse, cItem*& Selected, cItem*& Highlighted);
	void DrawCraftedRequires(float offestX, float offsetY, olc::vf2d mouse, cItem*& Selectedm, cItem*& Highlighted);


	void DrawDescriptionPattern(cItem* highlighted, olc::vf2d mouse, olc::vf2d mousefix);
	void AddMoney(int money) { this->Money += money; };
	int  CheckMoney() { return Money; };
protected:
	std::vector<std::string> m_vecDialogToShow;

	
	bool m_bShowDialog = false;
	float m_fDialogX = 0.0f;
	float m_fDialogY = 0.0f;



	int nVisibleTileX = ScreenWidth() / 64;
	int nVisibleTileY = ScreenHeight() / 64;

	float fOffsetX = 0;
	float fOffsetY = 0;
	

	bool charDeath = false;


public:

	bool LoadFunction();
	bool SaveFunction();
	void AddQuest(cQuest* quest);
	void QuestListClear()
	{
		m_listQusets.clear();
	};
	cQuest* GetQuest(std::string name);


	cMap* GetCurrentMap();

	void ChangeMap(std::string sMapName, float x, float y);

	void defineFacingDirection(float& fTestX, float& fTestT);

	bool GiveItem(cItem* item);
	bool ByeItem(cItem* item);
	bool GiveWarehouseItem(cItem* item);
	bool GiveStoreItem(cItem* item);
	bool GiveBlackSmithItem(cItem* item);
	bool GiveNewText(cDynamic_TextDamage* Text) ;


	std::list<cItem*> GetListItem() { return  m_listItems; };  // Get invntory
	std::list<cQuest*> GetListQuest() {return  m_listQusets;};
	std::vector<int> GetLearnedTalentVector() { return m_vecSaveTalents; };
	std::list<cItem*> GetListStoreItem() { return  m_listStoreItems; };  // Get invntory
	std::list<cItem*> GetListBlackSmithItem() { return  m_listBlackSmithItems; };  // Get invntory

	//std::vector<cDynamic*> GetFightText(){return m_vecFightText; }  //Fight text
	bool TakeItem(cItem* item);   // take it away if it exist 
	bool TakeItem(int Price);  // Delete money
	bool TakeItem(std::string Names);  // Delete money
	bool HasItem(cItem* item);    //Doesn't item exist within the inventory list


	void AddProjectile(cDynamic* proj);
	void AddVecDynamic(cDynamic* proj);
	void EraseEnemysPool(cDynamic* proj);
	void AddEnvironment(Environment* env);
	void AddIndicators(cDynamic* Ind);

	void AddUi(cUI* Ui);
	void AddTalentUi(cUI* Ui);

	int GetLvl();
	int GetCurrExp();
	int GetRequredExp();

	
	int GetRage();
	int GetEnergy();
	bool GetBackStab();
	bool GetTarget();
	bool GetbOnGraund();


	void CalculateExp();
	void SetCurrentExp(int DeathExp);


	void Attack(cDynamic_Creature* aggressor, cWeapon* weapon);
	void Damage(cDynamic_Projectile* projectile, cDynamic_Creature* victim);

	void AddParticle(float px, float py);

	void AddParticle(VfxParticles* particle);

	void JumpDamage(cDynamic_Creature* projectile, cDynamic_Creature* victim);

	void ShowDialog(std::vector<std::string> vecLines);
	void DrawBigText(std::string sText, int x, int y, float scalex, float scaley, olc::Pixel color = olc::WHITE);
	void DisplayDialog(std::vector<std::string> vecText, int x, int y);



	void DrawParallaxLayer(float x, olc::Decal* decal);
	void WrapCoordinates(float ix, float& ox);



	bool CheckPosition(int pxX, int pxXF, int pyY, int pyYf);
	bool CheckParticlePosition(float pxX, float pyYf);
	bool CheckPosition(int pxX, int pyYf);

	void drawPlayer(bool bdraw);

	void LoadenemyInstances();


	void ReturnToPool(cDynamic* bandit);
	void ReturnEnvironmentToPool(Environment* entity);
	cDynamic* SpawnBandit(const olc::vf2d* position);

	cDynamic* SpawnWerewolf(const olc::vf2d* position);

	cDynamic* SpawnBoar(const olc::vf2d* position);

	cDynamic* FindpoolEntity( cDynamic* object);
	void SpawnRainDrops();
	void SpawnClouds();
	cDynamic* SpawnDireWolf(const olc::vf2d* position);

	void SetMouseFocus(bool enable)
	{
		if (enable)
		{

			int screenWidth = GetSystemMetrics(SM_CXSCREEN);
			int screenHeight = GetSystemMetrics(SM_CYSCREEN);

			int consoleWidth = screenWidth-1;
			int consoleHeight = screenHeight-1;


			RECT windowRect;
			GetWindowRect(hWnd, &windowRect); // Get the window's rectangle
			ClipCursor(&windowRect); // Clip the cursor within the window

			SetCursorPos(consoleWidth / 2, consoleHeight / 2);
		}
		else
		{
			ClipCursor(nullptr); // Release mouse capture
		}
	}


};

void CombineAdjacentTiles(std::vector<Tile>& batchLayer, int layer);

float Lerp(float a, float b, float t);
bool CheckZeroDivide(float check, float& variable);
