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
#include "PerlinNoise.h"
#include <unordered_map>
#include <queue>
#include "InventorySocket.h"
#include "TalentController.h"
//#include "stb_image.h"
//#include "ParallaxManager.h"

//#define DEBUG_MODE  // Включить режим отладки



struct TileInfo {
	olc::vf2d position;
	olc::vf2d sourceRect;
	int textureIndex;
	int textureLayer;
	olc::vi2d size{ 64,64 };
};

class  UiController
{
public:
	


	void QueueCommand(std::function<void()> comand);
	void ProcessCommand();

//	void AddButton(UIButton* button);
//	void AddRect(UiRect* Rect);
//	void AddScroller(UIScroller* UIScroller);
//	void AddUiIcons(UIIcons* Icons);
//	void AddCurrentSpell(UICurrentSpell* CurreSpell);
//	void AddMainIndicator(UIMainIndicator* Indicator);
	void AddUI(cUI* UI);

	void AddFastSlots(UIFastSlot* SLots);
	void AddEvent(Event* event);

	void ConnectFastSlot(olc::vi2d& mouse,InventaryItem* GrabItem);

	void Update(olc::vi2d& mouse,float& fElapsedTIme);
	
	 void Draw(olc::vi2d& mouse);

	 bool OnClick(olc::vi2d& mouse);
	 bool UseFastButtons(cDynamic* player,int Index);
	// bool OnClick(olc::vi2d& mouse, cUI* Highlighted);

	 void Reset();

	 void LoadMainMainMenu();
	 void LoadPauseFunctional();
	 void LoadGameFunctional();

	 void LoadTalentFunctional();

private:

	// Очередь команд, которые должны быть выполнены после завершения цикла
	std::queue<std::function<void()>> uiCommands;
	
//	std::vector<UICurrentSpell*> UiCurrentSpellsVec;
//	std::vector<UIIcons*> UiIconsVec;
//	std::vector<UIButton*> UiButtonsVec;
//	std::vector<UiRect*> UiRectVec;
//	std::vector<UIScroller*> UiScrollerVec;
//	std::vector< UIMainIndicator*> UiMainIndicatorVec;
	std::vector<cUI*> UIVec;
	std::vector<UIFastSlot*> UiFastSlotsVec;
	std::vector<Event*> UiEventsVec;
};




class RPG_Engine : public olc::PixelGameEngine
{

public:
	RPG_Engine();




	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;
	int blockCamera : 1;

	bool bSmoothAppearScreen = false;
	int AlphaAppearScreen = 255;
	float fscale = 1.0f;

	uint8_t absorbCounter = 0;

	uint8_t goldcount = 0;

	
	
	//sf::Sound* currsound;
//	sf::Sound* WalkSound = new sf::Sound();
	std::vector<sf::Sound*> sounds;  // Контейнер для проигрываемых звуков
	
	void AddText(float px, float py, std::string text) {

		cDynamic_TextDamage* Text = new cDynamic_TextDamage(px, py, text);

		m_vecFightText.push_back(Text);
	}

	olc::Decal* D_FullUi = nullptr;   // full ui 

	enum class DataStruct {
		CharFull,
		Required,
		StoreLogo,
		WarehouseLogo,
		inventarySockets,
		UiPlatform,
		Map,
		MapLock,
		MapForest,
		MapIce,
		MapFire,
		MapMonastery,
		MapBridge,
		
		UIMainBox,
		UiQuestsButton,
		UiInventoryButton,
		UiTalentButton,
		UiEnergy,
		UiRage,

		

		FxDust,
		FXDeflect,
		FxTalentButton,

		QuestBook,
		
		QuestCompletedText,
	//	NotEnoughEnergyText,
	//	NotEnoughRageText,
		ChargeSpell,



	};

	enum class QuestStruct {

		MAINQUEST =0,
		FIRSTBANDIT =1,
		KILLWEREWOLF =2,
		KILLBANDITBOSS =3,
		KEYPOINTSINVILLAGE=4,
		BANDITCAMP =5,
		KILLSKELETON =6,
		KILLSPIDER = 7,


	};

	enum class NpcStruct {

		Map,
		blacksmith,
		luxary,
		proffesion,
		Warehouse,
		SaveMan,
		TavernVillageEntrance,    //<--Entrence in tavern
		CaveEntrance,
		CaveOut

	};

	enum class BossStruct
	{
		BanditBoss
	};
	enum UiFlags {
		BUIBACKSTUB = 1 << 0,  // 1-й бит
		BUISWIRL = 1 << 1,  // 2-й бит
		BPRESSF = 1 << 2  // 3-й бит
	};
	void clearBackstabUiFlags() {
		Flags &= ~(BUIBACKSTUB | BUISWIRL );  // Сбрасываем все установленные флаги
	}
private:


	
	std::vector<sf::Sound*> m_vecSoundsPool;  // Контейнер для пула
	std::array<Environment*,106> EnvironmentPool;  // |0-100 rain|101-400 rainSpray|400++ Other|

	

	//std::vector<cDynamic*> wolfPool;

	int bOpenTravelAsk : 1; // Map Logic
	bool bPause = false;

	int SelectedTeleport = 0;
	//

	




	int SparedFacDir = 0;

	// MainMenu
	float MainMenuX = 0.0f;
	float MainMenuY = 0.0f;
	int MainMenuCalc = 0;


	bool bshowfirst = false;
	bool bshowSecond = false;
	bool bshowThird = false;
	bool bshowZero = false;


	float fOldOffset = 0.0f;
	float fFarPrlxX = 0.0;
	float fMidPrlxX = 0.0;
	float fClosePrlxX = 0.0;


	float fCameraVx = 0.0f;
	float fCameraVy = 0.0f;


	
	int TalentPoint = 0;
	bool QuestChecked = false;
	bool InventoryChecked = false;

	uint16_t  Money = 0;


	float framespeed = 0.1f;
	float animsprite = 0.0f;
	int frameIndex = 0;

	const float FRAME_SPEED = 0.1f; // Скорость смены кадров (секунды на кадр)
	const int MAX_FRAMES = 8;       // Количество кадров в анимации
	const int MAIN_MENU_LIMIT = 10; // Лимит для MainMenuCalc



	

	uint8_t Flags;

	

	
	//

	//bool bFinishLoading = false;
	//int bUibackstub : 1;
	//int bUiSwirl : 1;
	//int PressF : 1;
//	uint32_t layer = 0;
//	uint32_t layerOne = 1;
//	uint32_t layerTwo = 2;
	cMap* m_pCurrentMap = nullptr;
	olc::Decal* m_sprFont = nullptr;

	//olc::Decal* m_sprPressF = nullptr;

	olc::Decal* D_Map = nullptr;
	olc::Decal* D_Items = nullptr;
	olc::Decal* D_Inventory = nullptr;
	olc::Sprite* spMask = nullptr;
	olc::Decal* D_FX = nullptr;
	//olc::Decal* testDecal = nullptr;
	
	std::vector<float> angles;
	std::vector<float> Sinnum;
	olc::vf2d maskcenter;
	float maskanim = 0.0f;


	std::vector<Tile > batchZeroCharsLayer; // (position, source rectangle) pairs // bool represent static or dynamic layer
	std::vector<Tile > batchFirstCharsLayer;
	std::vector<Tile > batchDynamicCharsLayer;
	std::vector<Tile > batchSecondCharsLayer;
	std::vector<Tile > batchThirdCharsLayer;


	std::vector<cDynamic*> m_vecVisibleDynamics;  // vector that keep only that dyn object that in our screen

	cDynamic_creature_Pantir* m_pPlayer = nullptr;                // before was cDynamic_creature
	//
	InventaryItem* highlighted = nullptr;
	InventaryItem* GrabItem = nullptr;
	//
	std::vector<cDynamic*> m_vecDynamics;    // Fixed

	
	std::array<cDynamic*, 30> BanditsPool;
	std::array<cDynamic*, 30> BanditsArcherPool;
	std::array<cDynamic*, 30> BanditBossPool;
	std::array<cDynamic*, 30> DireWolfsPool;
	std::array<cDynamic*, 30> BoarPool;
	std::array<cDynamic*, 30> WereWolfsPool;
	std::array<cDynamic*, 30> ItemPool;
	std::array<cDynamic*, 30> TextPool;
	std::array<cDynamic*, 30> ProjectilePool;
	std::array<cDynamic*, 30> VfxShotPool;
	std::array<cDynamic*, 30> MirrorsFx;

	

	std::vector<Environment*>m_vecCloseWeather; // Vector to store raindrops and clouds
	std::vector<Environment*>m_vecCloseWeather_2; // Vector to store raindrops and clouds
	std::vector<Environment*>m_vecFarWeather; // Vector to store raindrops and clouds
	std::vector<cDynamic*> m_vecProjectiles;  //Transient

//	std::vector<cDynamic*> m_vecIndicators;  //Hp Rage Energy and text

	std::vector<cDynamic*> m_vecParticles;

	std::vector<cDynamic*> m_vecFightText;  //Fight text
	
	cScriptProcessor m_script;

	//std::vector<Button*> MainMenuButtons;

	UiController UiController;
	TalentController TalentController;

	std::vector<InventaryItem*> m_vecEquip;
	

	std::vector<InventaryItem*> m_listStoreItems;
	std::vector<InventaryItem*> m_listBlackSmithItems;
	std::vector<InventaryItem*> m_listItems;
	std::vector<InventaryItem*> m_listWarehouseItems;

	std::list<cQuest*> m_listQusets;
	std::list<cQuest*> m_CompletedQuest;
	cQuest* CurrQuest =nullptr;
	uint8_t DescrqstIndex = 0;

	enum
	{
		MODE_TITLE,
		MODE_LOCAL_MAP,
		MODE_WAREHOUSE,
		MODE_INVENTORY,
		MODE_SHOP,
		MODE_MAP,
		MODE_PROFESSION,
		MODE_BLACKSMITH,
		MODE_QUESTLOG,




	};


	enum FastSLotsIndexes
	{
		UpSLot =0,
		DownSlot =1

	};
	int n_nGameMode = MODE_TITLE;


private:

	int SoundVolume = 50;
	
	int lvl = 1;
	int ExpRequred = 0;     //Amount
	int currExp = 0;


	float textCounter = 0.0f;
	int chartIndex = 0;
	int rowIndex = 0;

	std::vector<std::string> vecdisplayText;
	 std::string currentLine;  // Временная строка для текущей строки текста


	std::array<olc::vi2d, 9> TalentPositions =
	{
	olc::vi2d{192, 512}, // Mid
	olc::vi2d{64, 512},  // High
	olc::vi2d{64, 640},  // Backstab
	olc::vi2d{64, 384},  // ShadowStep
	olc::vi2d{64, 192},  // Swirl
	olc::vi2d{192, 384}, // Vanish
	olc::vi2d{320, 512}, // RightAir
	olc::vi2d{320, 256}, // RightFLIGHTUP
	olc::vi2d{320, 64}   // RightATTACK LANDING
	};
	std::array<olc::vi2d, 5> MapPosAr =    // all these coordinates from main background of map 
	{
		olc::vi2d{56,229},      //Forest destination on map
		olc::vi2d{196,164},      //Ice  destination on map
		olc::vi2d{136,335},      //Fire  destination on map
		olc::vi2d{350,205},      //Bridge  destination on map
		olc::vi2d{347,333},      //Map Monastery  destination on map
	};

	//std::array<olc::vi2d, 10> UIPosOnScreenAr;   // ui describe equal screensize, onusercreate 


	std::array< cDynamic_creature_NPC*, 9> NPC_Ar;


	std::array<cDynamic*, 1>BossAr;

	std::array<uint8_t, 9> TalentSavePoints =
	{

		2,  // <-- Mid
		3,  // <-- High
		4,  // <-- Backstab
		5,  // <-- ShadowStep
		6,  // <-- Swirl
		7,  // <-- Vanish
		12, // <-- RightAir
		13, // <-- RightFLIGHTUP
		14  // <-- RightATTACK LANDING


	};

	

	struct SpriteData
	{
		olc::vi2d Pos;
		olc::vi2d Size;

		SpriteData(olc::vi2d Pos, olc::vi2d Size) : Pos(Pos), Size(Size) {}
		// Конструктор по умолчанию
		SpriteData() : Pos{ 0, 0 }, Size{ 0, 0 } {}

	};

	
	// Объявляем unordered_map
	std::unordered_map<DataStruct, SpriteData> SpritesData_map = {
		//Decal inventory
		{DataStruct::CharFull, SpriteData{olc::vi2d{0, 0}, olc::vi2d{512, 523}}},       // Character stat+sockets+name 
		{DataStruct::Required, SpriteData{olc::vi2d{513, 0}, olc::vi2d{213, 224}}},     // Required 
		{DataStruct::StoreLogo, SpriteData{olc::vi2d{724, 0}, olc::vi2d{274, 75}}},      // Store
		{DataStruct::WarehouseLogo, SpriteData{olc::vi2d{723, 75}, olc::vi2d{275, 77}}}, // Warehouse
		{DataStruct::inventarySockets, SpriteData{olc::vi2d{0, 256}, olc::vi2d{512, 192}}}, // InventorySockets
		//

		{DataStruct::UiPlatform, SpriteData{olc::vi2d{832, 0}, olc::vi2d{334, 142}}},    // UiPlatform

		//Decal Mao
		{DataStruct::Map, SpriteData{olc::vi2d{259,0},olc::vi2d{479,523}}},
		{DataStruct::MapLock, SpriteData{olc::vi2d{0,230},olc::vi2d{62,103}}},
		{DataStruct::MapForest, SpriteData{olc::vi2d{117,118},olc::vi2d{140,119}}},
		{DataStruct::MapIce, SpriteData{olc::vi2d{0,118},olc::vi2d{117,112}}},
		{DataStruct::MapFire, SpriteData{olc::vi2d{0,0},olc::vi2d{177,118}}},
		{DataStruct::MapMonastery, SpriteData{olc::vi2d{177,0},olc::vi2d{71,87}}},
		{DataStruct::MapBridge, SpriteData{olc::vi2d{62,237},olc::vi2d{82,94}}},
	
		//FX
		{ DataStruct::FxDust, SpriteData{olc::vi2d{0,0},olc::vi2d{52,10}} },
		{ DataStruct::FXDeflect, SpriteData{olc::vi2d{50,25},olc::vi2d{25,25}} },
		{ DataStruct::FxTalentButton, SpriteData{olc::vi2d{486,204},olc::vi2d{18,17}} },

		{ DataStruct::QuestBook, SpriteData{olc::vi2d{449,224},olc::vi2d{300,200}} },


		{ DataStruct::QuestCompletedText, SpriteData{olc::vi2d{480,465},olc::vi2d{151,17}} },
		{ DataStruct::ChargeSpell, SpriteData{olc::vi2d{704,64},olc::vi2d{59,61}} },

	};


	std::array<DataStruct, 5> MapSpritesIndexes =
	{
		DataStruct::MapForest,   //0
		DataStruct::MapIce,      //1
  		DataStruct::MapFire,     //2
		DataStruct::MapBridge,   //3
		DataStruct::MapMonastery, //4
	};


	SpriteData energyRageEmpty{ olc::vi2d{0, 0}, olc::vi2d{0, 0} };

	
	const SpriteData& GetSpriteData(DataStruct ds)
	{
		return SpritesData_map[ds] ; // Используем at() для безопасного доступа
	}



	void DrawElement(DataStruct ds, olc::vf2d position, float fscale, olc::Decal* Decal);



	HWND hWnd; // Store the window handle internally

	float fAccumulatedTime = 0.0f;

	 int nFrames = 0;

	 void Drawcursor(olc::vi2d mouse);
public:

	// Методы для установки и проверки флагов
	void setUiFlag(UiFlags flag) {
		Flags |= flag;  // Устанавливаем флаг
	}

	void clearUiFlag(UiFlags flag) {
		Flags &= ~flag;  // Сбрасываем флаг //revers number
	}

	bool checkUiFlag(UiFlags flag) const {
		return Flags & flag;  // Проверяем флаг  вернет true если там будет 1 и 1 и ноль во всех других вариантах 
	}


	cDynamic* SpawnBoss(BossStruct name);
	cDynamic* GetBoss(BossStruct name);




	// Геттеры, возвращающие ссылки на массивы
	std::array<cDynamic*, 30>& getBanditsPool() {
		return BanditsPool;
	}

	std::array<cDynamic*, 30>& getBanditsArcherPool() {
		return BanditsArcherPool;
	}

	std::array<cDynamic*, 30>& getBanditBossPool() {
		return BanditBossPool;
	}

	std::array<cDynamic*, 30>& getDireWolfsPool() {
		return DireWolfsPool;
	}

	std::array<cDynamic*, 30>& getBoarPool() {
		return BoarPool;
	}

	std::array<cDynamic*, 30>& getWereWolfsPool() {
		return WereWolfsPool;
	}

	std::array<cDynamic*, 30>& getItemPool() {
		return ItemPool;
	}

	std::array<cDynamic*, 30>& getTextPool() {
		return TextPool;
	}

	std::array<cDynamic*, 30>& getProjectilePool() {
		return ProjectilePool;
	}
	std::array<cDynamic*, 30>& getVfxShotPool() {
		return VfxShotPool;
	}
	std::array<cDynamic*, 30>& getMirrorsPool() {
		return MirrorsFx;
	}

	bool OptionSettings();
	bool ReturnBackSettings();

	bool getScriptActive() { return m_script.bUserControlEnabled; }
	void ClearDisplayText() { vecdisplayText.clear(); currentLine.clear(); rowIndex = 0; chartIndex = 0; textCounter = 0.0f; };

	void setSoundVolume(int sound) { SoundVolume = sound; };
	//void setMuscVolume(int sound) { RPMuscVolume = sound; };


     template<std::size_t N>
	 bool PoolsController(std::array<cDynamic*, N>& name, cDynamic* obj)
	 {
		 {
			 for (auto it = name.begin(); it != name.end(); it++)
			 {
				 cDynamic* entity = *it;
				 if (entity == nullptr) {
					 *it = obj;
					 return true;
				 }
			 }
			 return false;
		 }
	 };
	void LoadBaseUiSettings();

	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

	void LoadSoundPool(size_t poolSize);
	void PlaySounds(std::string buffername);

	void SetGameMode(int set) {  n_nGameMode = set; };
	int GetGameMode() { return n_nGameMode; }

//	int GetLearnedTalent(int talentsave);
//	void SetLearnedTalent(cUI* Talent);

	void SetPause(int set) { bPause = set; };

	void UpdateAnimationFrame(float felapsedTime);


	bool UpdateTitleScreen(float fElapsedTime);
	bool UpdateLocalMap(float fElapsedTime);
	bool UpdateWarehouse(float fElapsedTime,olc::vi2d& mouse);
	bool UpdateInventory(float fElapsedTIme, olc::vi2d& mouse);
	bool UpdateShop(float fElapsedTime, olc::vi2d& mouse);
	bool UpdateProfession(float fElapsedTime, olc::vi2d& mouse);
	bool UpdateMap(float FelapsedTime, olc::vi2d& mouse);
	bool UpdateBlackSmith(float FelapsedTime,olc::vi2d& Mouse);
	void uiCellUpdate(olc::vi2d& mouse, float& FelapsedTime);
	void UpdateQuestLog(float fElapsedTime, olc::vi2d& Mouse);

	bool SetMouseTarget(olc::vi2d& mouse);


	void DrawWarehouse(const float squex, const float squeYm, olc::vf2d mouse, olc::vf2d mousefix, InventaryItem*& highlighted, InventaryItem*& Grabitem,int moneyamount);
//	void DrawInventoryFastCells(olc::vi2d mouse,uint8_t Selobjectsize, olc::vi2d mousefix);
	void DrawInventory(float offestX,float offsetY, olc::vi2d mouse, InventaryItem*& highlighted);
	void DrawInventory(float offestX, float offsetY, olc::vi2d mouse, olc::vi2d mouseFixed,  InventaryItem*& highlighted, InventaryItem*& Grabitem);
	void moveIItems(olc::vi2d mouse, float x, float y, InventaryItem*& grabitem, std::vector<InventaryItem*>& vector);
	void moveIItems(olc::vi2d mouse, float x, float y,float eqX, float eqy, InventaryItem*& grabitem, std::vector<InventaryItem*>& vector, std::vector<InventaryItem*>& eqVector);

	bool AttachEq(std::vector<InventaryItem*>& EqVector,int currindex);

	void FillBatch(int TileLayer, int layer, int x, int y,std::vector<TileInfo>& Before, std::vector<TileInfo>& After);
	

	bool DrawPause(olc::vi2d mouse,float& fElapsedTime);


	void SetQuestChecked(bool check) { QuestChecked = check; };
	void SetInventoryChecked(bool check) { InventoryChecked = check; };

	bool GetTalentPoint() { return (TalentPoint > 0) ? true : false; };
	bool GetCheckedQuests() { return QuestChecked; };
	bool GetInventoryChecked() { return InventoryChecked; };

	void ClearAbsorbedSlots(std::vector<InventaryItem*>& m_listItems);  // find absorbed objects in sockets and change them on empty sockets
	bool SaleItem(int Price,InventaryItem* Sale);
	void DrawStoreInventory(float sX, float sY, olc::vi2d mousefix, InventaryItem*& highlighted);

	void DrawBlacksmithInentory(float offestX, float offsetY, olc::vi2d mouse, InventaryItem*& Selected, InventaryItem*& Highlighted);
	void DrawCraftedRequires(float offestX, float offsetY, olc::vi2d mouse, InventaryItem*& Selectedm, InventaryItem*& Highlighted);


	void DrawDescriptionPattern(InventaryItem* highlighted, olc::vi2d mouse, olc::vf2d mousefix);
	void AddMoney(int money) { this->Money += money; };
	int  CheckMoney() { return Money; };

	int CheckVerticalDirection() { return m_pPlayer->GetFacingDirectionVertical(); };

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



#ifdef  DEBUG_MODE
	int layer ;
	int layerOne;
	int layerTwo;

#endif // DEBUG


public:

	 cDynamic_creature_NPC* GetNpc(NpcStruct name);

	float getOffsetX() { return fOffsetX; };
	float getOffsetY() { return fOffsetY; };
	bool LoadFunction();
	bool SaveFunction();
	void AddActiveQuest(cQuest* quest);
	void AddCompletedQuest(cQuest* quest);
	void QuestListClear()
	{
		m_listQusets.clear();
	};
	cQuest* GetActiveQuest(std::string name);


	cMap* GetCurrentMap();

	void ChangeMap(std::string sMapName, float x, float y);

	void defineFacingDirection(float& fTestX, float& fTestT);

	bool GiveItem(std::string Name, uint8_t count, bool offstack = true);
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
	std::vector<InventaryItem*> GetListStoreItem() { return  m_listStoreItems; };  // Get invntory
	std::vector<InventaryItem*> GetListBlackSmithItem() { return  m_listBlackSmithItems; };  // Get invntory

	//std::vector<cDynamic*> GetFightText(){return m_vecFightText; }  //Fight text
	bool TakeItem(InventaryItem* item, std::vector<InventaryItem*>& m_listItems);   // take it away if it exist 
	bool TakeItem(int Price);  // Delete money
	bool TakeItem(std::string Names);  // Delete money
	bool HasItem(std::string item);    //Doesn't item exist within the inventory list


	void AddProjectile(cDynamic* proj);
	void AddVecDynamic(cDynamic* proj);
	
	void AddEnvironment(Environment* env);
	

//	void AddUi(cUI* Ui);
//	void AddTalentUi(cUI* Ui);

	int GetLvl();
	void SetLvl(int lvl);

	int GetCurrExp();
	void SetCurrExp(int currexp);


	int GetRequredExp();
	void SetRequredExp(int reqexp);
	
	int GetRage();
	int8_t GetEnergy();

//	bool GetBackStab();

	bool GetSwirl();
	//void SetBackStab(bool toggle);

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



	
	void WrapCoordinates( float& ox);



	bool CheckPosition(int pxX, int pxXF, int pyY, int pyYf);
	bool CheckParticlePosition(float pxX, float pyYf);
	

	bool CheckPosition(int pxX, int pyYf);

	void drawPlayer(bool bdraw);

	void LoadenemyInstances();


	
	void ReturnEnvironmentToPool(Environment* entity);



	cDynamic* SpawnItem(const olc::vf2d position, cItem* item);
	cDynamic* SpawnBattleText(const olc::vf2d position, std::string Text, olc::Pixel COlor =olc::WHITE);
	cDynamic* SpawnBandit(const olc::vf2d position);
	cDynamic* SpawnBanditArcher(const olc::vf2d position);
	cDynamic* SpawnWerewolf(const olc::vf2d position);
	cDynamic* SpawnBoar(const olc::vf2d position);
	cDynamic* SpawnBossBandt (const olc::vf2d position);
	cDynamic* SpawnProjectile(const olc::vf2d position);
	cDynamic* SpawnVfxShot(const olc::vf2d position);
	cDynamic* SpawnMirrors(const olc::vf2d position, olc::vi2d framePos, int sizex,int sizey, int amountframes, olc::Decal* Decal);


	void SpawnRainDrops(const uint16_t Index);
	void SpawnRainSpray(float px, float py, const uint16_t Index);

	void SpawnClouds(const uint16_t Index);



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
int sign(float x);
