#pragma once
#include "olcPixelGameEngine.h"
//#include "olcPGEX_Sound.h"

//#define OLC_SOUNDWAVE

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

#include <mutex>
//#include "stb_image.h"
//#include "ParallaxManager.h"

struct TileInfo {
	olc::vf2d position;
	olc::vf2d sourceRect;
	int textureIndex;
	int textureLayer;
	olc::vi2d size{ 64,64 };
};

struct InventaryItem
{
	cItem* Item = nullptr;

	
	uint8_t Uiindex = 0;
	uint8_t index = 0;  // Index in inventory


	// Переменная для хранения всех флагов
	unsigned int InventaryFlags = 0;
	// Объявляем флаги через enum
	enum InventaryFlagsEnum {
		Breserved = 1 << 0,  // 1-й бит
		Babsorbed = 1 << 1,  // 2-й бит
		Grabitem = 1 << 2,  // 3-й бит
		Highlighted = 1 << 3,  // 4-й бит
		binWarehouse = 1 << 4,  // 5-й бит
		bEquiped = 1 << 5,  // 6-й бит
		Objectselected = 1 << 6,
		
	};

	// Методы для установки и проверки флагов
	void setFlag(InventaryFlagsEnum flag) {
		InventaryFlags |= flag;  // Устанавливаем флаг
	}

	void clearFlag(InventaryFlagsEnum flag) {
		InventaryFlags &= ~flag;  // Сбрасываем флаг //revers number
	}

	bool checkFlag(InventaryFlagsEnum flag) const {
		return InventaryFlags & flag;  // Проверяем флаг
	}
	void resetsocket()
	{
		currStacks = 1;
		Gold = 0;
		Item = nullptr;
	}
	uint8_t currStacks = 1;
	uint8_t Gold = 0;

};

class RPG_Engine : public olc::PixelGameEngine
{

public:
	RPG_Engine();



	// Переменные для мультипоточности
	std::thread physicsThread;
	std::mutex gameMutex;
	bool running = true;  // Для контроля потока физики


	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;
	int blockCamera : 1;

	bool bSmoothAppearScreen = false;
	int AlphaAppearScreen = 255;
	float fscale = 1.0f;

	uint8_t absorbCounter = 0;

	uint8_t goldcount = 0;

	
	sf::Sound* currsound;
	sf::Sound* WalkSound = new sf::Sound();
	std::vector<sf::Sound*> sounds;  // Контейнер для проигрываемых звуков
	
	void AddText(float px, float py, std::string text) {

		cDynamic_TextDamage* Text = new cDynamic_TextDamage(px, py, text);

		m_vecFightText.push_back(Text);
	}

	olc::Decal* D_Ui = nullptr;   // full ui 

private:

	
	

	
	std::vector<sf::Sound*> m_vecSoundsPool;  // Контейнер для пула
	std::vector<Environment*> EnvironmentPool;
	//std::vector<cDynamic*> wolfPool;

	int bOpenTravelAsk : 1; // Map Logic
	int bPause : 1;

	int SelectedTeleport = 0;
	//




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


	uint8_t lvl =1;
	int ExpRequred = 0;     //Amount
	int currExp =0;     
	uint16_t  Money = 0;


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

	//float speed = 6000;

	//

	//bool bFinishLoading = false;
	int bUibackstub : 1;
	int PressF : 1;
	uint32_t layer = 0;
	uint32_t layerOne = 1;
	uint32_t layerTwo = 2;
	cMap* m_pCurrentMap = nullptr;
	olc::Decal* m_sprFont = nullptr;
	olc::Decal* m_Items = nullptr;

	//olc::Decal* m_sprPressF = nullptr;


	olc::Decal* Inventoryback = nullptr;
	olc::Sprite* spMask = nullptr;
	//olc::Decal* testDecal = nullptr;
	
	std::vector<float> angles;
	std::vector<float> Sinnum;
	olc::vf2d maskcenter;
	float maskanim = 0.0f;


	std::vector<TileInfo > batchZeroCharsLayer; // (position, source rectangle) pairs // bool represent static or dynamic layer

	std::vector<cDynamic*> m_vecVisibleDynamics;  // vector that keep only that dyn object that in our screen

	cDynamic_creature_Pantir* m_pPlayer = nullptr;                // before was cDynamic_creature
	//
	InventaryItem* highlighted = nullptr;
	InventaryItem* GrabItem = nullptr;
	//
	std::vector<cDynamic*> m_vecDynamics;    // Fixed

	std::vector<cDynamic*> enemysPool;
	std::vector<cDynamic*> BanditsPool;
	std::vector<cDynamic*> BanditsArcherPool;
	std::vector<cDynamic*> DireWolfsPool;
	std::vector<cDynamic*> BoarPool;
	std::vector<cDynamic*> WereWolfsPool;
	std::vector<cDynamic*> ItemPool;
	std::vector<cDynamic*> TextPool;
	std::vector<cDynamic*> ProjectilePool;


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

	std::vector<InventaryItem*> m_vecEquip;
	std::vector<InventaryItem*> m_vecUi;

	std::vector<InventaryItem*> m_listStoreItems;
	std::vector<InventaryItem*> m_listBlackSmithItems;
	std::vector<InventaryItem*> m_listItems;
	std::vector<InventaryItem*> m_listWarehouseItems;

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

	 void Drawcursor(olc::vi2d mouse);

public:
	
	void LoadBaseUiSettings();

	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

	void LoadSoundPool(size_t poolSize);
	void PlaySounds(std::string buffername);

	void SetGameMode(int set) {  n_nGameMode = set; };
	int GetGameMode() { return n_nGameMode; }

	int GetLearnedTalent(int talentsave);
	void SetLearnedTalent(cUI* Talent);

	void SetPause(int set) { bPause = set; };

	bool UpdateTitleScreen(float fElapsedTime);
	bool UpdateLocalMap(float fElapsedTime);
	bool UpdateWarehouse(float fElapsedTime);
	void DrawWarehouse(const float squex, const float squeYm, olc::vf2d mouse, olc::vf2d mousefix, InventaryItem*& highlighted, InventaryItem*& Grabitem,int moneyamount);

	bool SetMouseTarget(olc::vi2d mouse);

	void uiCellUpdate(olc::vi2d mouse);
	bool UpdateInventory(float fElapsedTIme);
	void DrawInventory(float offestX,float offsetY, olc::vi2d mouse, InventaryItem*& highlighted);
	void DrawInventory(float offestX, float offsetY, olc::vi2d mouse, olc::vi2d mouseFixed,  InventaryItem*& highlighted, InventaryItem*& Grabitem);
	void moveteItems(olc::vi2d mouse, float x, float y, InventaryItem*& grabitem, std::vector<InventaryItem*>& vector);
	void moveteItems(olc::vi2d mouse, float x, float y,float eqX, float eqy, InventaryItem*& grabitem, std::vector<InventaryItem*>& vector, std::vector<InventaryItem*>& eqVector);

	bool AttachEq(std::vector<InventaryItem*>& EqVector,int currindex);

	void FillBatch(int TileLayer, int layer, int x, int y,std::vector<TileInfo>& Before, std::vector<TileInfo>& After);
	

	void ClearAbsorbedSlots(std::vector<InventaryItem*>& m_listItems);  // find absorbed objects in sockets and change them on empty sockets
	bool UpdateShop(float fElapsedTime);
	bool SaleItem(int Price,InventaryItem* Sale);
	void DrawStoreInventory(float sX, float sY, olc::vi2d mousefix, InventaryItem*& highlighted);
	bool UpdateProfession(float fElapsedTime);
	bool UpdateMap(float FelapsedTime);
	bool UpdateBlackSmith(float FelapsedTime);

	void DrawBlacksmithInentory(float offestX, float offsetY, olc::vi2d mouse, InventaryItem*& Selected, InventaryItem*& Highlighted);
	void DrawCraftedRequires(float offestX, float offsetY, olc::vi2d mouse, InventaryItem*& Selectedm, InventaryItem*& Highlighted);


	void DrawDescriptionPattern(InventaryItem* highlighted, olc::vi2d mouse, olc::vf2d mousefix);
	void AddMoney(int money) { this->Money += money; };
	int  CheckMoney() { return Money; };

	std::vector<InventaryItem*>& Getequip() { return m_vecEquip; };
	float CellSize;

protected:

	
	std::vector<std::string> m_vecDialogToShow;

	
	bool m_bShowDialog = false;
	float m_fDialogX = 0.0f;
	float m_fDialogY = 0.0f;



	int nVisibleTileX = ScreenWidth() / CellSize;
	int nVisibleTileY = ScreenHeight() / CellSize;


	float fOffsetX = 0;
	float fOffsetY = 0;
	

	bool charDeath = false;


public:

	

	float getOffsetX() { return fOffsetX; };
	float getOffsetY() { return fOffsetY; };
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

	bool GiveItem(std::string Name,uint8_t count);
	//bool FillList(cItem* item, int number);
	bool ByeItem(InventaryItem* item);
	bool GiveWarehouseItem(cItem* item);
	bool GiveStoreItem(std::string item);
	bool GiveBlackSmithItem(std::string Name);
	bool GiveNewText(cDynamic_TextDamage* Text) ;


	std::vector<InventaryItem*> GetListItem() { return  m_listItems; };  // Get invntory
	std::vector<InventaryItem*> GetListWarehouseItem() { return  m_listWarehouseItems; };  // Get invntory
	int GetFreespaceInventory();
	std::list<cQuest*> GetListQuest() {return  m_listQusets;};
	std::vector<int> GetLearnedTalentVector() { return m_vecSaveTalents; };
	std::vector<InventaryItem*> GetListStoreItem() { return  m_listStoreItems; };  // Get invntory
	std::vector<InventaryItem*> GetListBlackSmithItem() { return  m_listBlackSmithItems; };  // Get invntory

	//std::vector<cDynamic*> GetFightText(){return m_vecFightText; }  //Fight text
	bool TakeItem(InventaryItem* item, std::vector<InventaryItem*>& m_listItems);   // take it away if it exist 
	bool TakeItem(int Price);  // Delete money
	bool TakeItem(std::string Names);  // Delete money
	bool HasItem(std::string item);    //Doesn't item exist within the inventory list


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
	uint8_t GetEnergy();

	bool GetBackStab();
	bool GetTarget();
	bool GetbOnGraund();


	void CalculateExp();
	void SetCurrentExp(int DeathExp);
	

	//void Attack(cDynamic_Creature* aggressor, cWeapon* weapon);
	void Damage(cDynamic_Projectile* projectile, cDynamic_Creature* victim);

	void AddParticle(float px, float py);

	void AddParticle(VfxParticles* particle);

	void JumpDamage(cDynamic_Creature* projectile, cDynamic_Creature* victim);

	void ShowDialog(std::vector<std::string> vecLines);
	void DrawBigText(std::string sText, float x, float y, float scalex, float scaley, olc::Pixel color = olc::WHITE);
	void DisplayDialog(std::vector<std::string> vecText, int x, int y);



	void DrawParallaxLayer(float x, olc::Decal* decal);
	void WrapCoordinates(float ix, float& ox);



	bool CheckPosition(int pxX, int pxXF, int pyY, int pyYf);
	bool CheckParticlePosition(float pxX, float pyYf);
	bool CheckPosition(int pxX, int pyYf);

	void drawPlayer(bool bdraw);

	void LoadenemyInstances();


	void ReturnToPool(cDynamic* bandit);
	void ReturnProjectileToPool(cDynamic* Projectile);
	void ReturnEnvironmentToPool(Environment* entity);
	void ReturnTextToPool(cDynamic* Text);
	
	cDynamic* SpawnBattleText(const olc::vf2d position, std::string Text, olc::Pixel COlor =olc::WHITE);
	cDynamic* SpawnBandit(const olc::vf2d position);
	cDynamic* SpawnBanditArcher(const olc::vf2d position);
	cDynamic* SpawnWerewolf(const olc::vf2d position);
	cDynamic* SpawnBoar(const olc::vf2d position);
	cDynamic* SpawnProjectile(const olc::vf2d position);
	void SpawnRainDrops();
	void SpawnClouds();



	//cDynamic* FindpoolEntity( cDynamic* object);
	cDynamic* SpawnDireWolf(const olc::vf2d position);

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

	void removeStoppedSounds();


	private:

		void bufferClockMask();
		void bufferSinusMask(float amplitude, float waves, float position, float phaseoffset);
		

		void clockwiseMask(float felapsedtime, olc::Sprite* test);
		void sinusoidMask(float felapsedtime, olc::Sprite* test, float amplitude);
	//void LoadSound();
	//void LoadMusc();
};

void CombineAdjacentTiles(std::vector<Tile>& batchLayer, int layer);

float Lerp(float a, float b, float t);
bool CheckZeroDivide(float check, float& variable);
