#pragma once
#include "olcPixelGameEngine.h"
#include "RPG_Assets.h"
#include "RPG_Dynamic.h"
#include "RPG_Commands.h"
#include <map>
class Tile;
class TileInfo;
class cMap
{

public:
	enum NATURE
	{
		TALK,
		WALK
	};




public:
	cMap();
	~cMap();


public:
	int nWidth;
	int nHeight;

	int* m_firstlayer = nullptr; // Grid
	int* m_secondlayer = nullptr; // Grid
	int* m_thirdlayer = nullptr; // Grid
	int* m_DynamicLayer = nullptr;
	int* m_zerolayer = nullptr;

	std::vector<Tile> batchLayer; // (position, source rectangle) pairs // bool represent static or dynamic layer

	bool bFinishloading = false;


	int* m_Collision = nullptr;  //Collision

	std::string sName;
	olc::Decal* pSprite;           //<--Sprite
	olc::Decal* pDynsprite;

	olc::Decal* FarParralax;
	olc::Decal* MidParralax;
	olc::Decal* CloseParralax;

	std::vector<std::pair<olc::vf2d*, olc::vf2d*>*>m_vecobsticles;
	std::pair<olc::vf2d, olc::vf2d>* Points;
	




	bool Create(std::string, olc::Decal* sprite,olc::Decal* sdinsprite, olc::Decal* CloseParralax, olc::Decal* MidParralax , olc::Decal* FarParralax, std::string);   //         	bool Create( std::string fileData, olc::Decal* sprite, std::string name);
	Tile GetIndexLayer( int x, int y);
	TileInfo GetIndexLayerBefore(int x, int y);
	TileInfo GetIndexLayerAfter(int x, int y);
		
	int GetColliziionIndex(int x, int y);

	virtual bool PopulateDynamics(std::vector<cDynamic*>& vecDyns)
	{
		return false;
	}
	virtual bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature)
	{
		return false;
	}


	std::pair<olc::vf2d*, olc::vf2d*>* getObsticlesPoints(float pxX, float pxXF, float pyY, float pyYF);
	
public:
	static cScriptProcessor* g_script;
	static RPG_Engine* g_engine;
	
};



class cMap_Forest : public cMap
{
public:
	cMap_Forest();
	bool PopulateDynamics(std::vector<cDynamic*>& vecDyns) override;

	//bool PopulateDynamics(vector<cDynamic*>& vecDyns) override;
	bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature) override;


private:
	void setWolfsPos();
	void setBanditsPos();
	void setWereWolfsPos();
	void setBoarPos();
	std::vector< olc::vf2d*>WolfsPositions;
	std::vector< olc::vf2d*>BanditsPositions;
	std::vector< olc::vf2d*>BoarPositions;
	std::vector< olc::vf2d*>WerewolfPositions;

};


class  cMap_ForestPartTwo : public cMap
{
public:
	cMap_ForestPartTwo();
	bool PopulateDynamics(std::vector<cDynamic*>& vecDyns) override;

	//bool PopulateDynamics(vector<cDynamic*>& vecDyns) override;
	bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature) override;


private:
	void setWolfsPos();
	void setBoarsPos();
	std::vector< olc::vf2d*>WolfsPositions;
	std::vector< olc::vf2d*>BoarsPositions;

};


class cMap_Mountains : public cMap
{
public:
	cMap_Mountains();
	bool PopulateDynamics(std::vector<cDynamic*>& vecDyns) override;

	//bool PopulateDynamics(vector<cDynamic*>& vecDyns) override;
	bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature) override;
};



class cMap_VillageInFire : public cMap
{
public:
	cMap_VillageInFire();
	bool PopulateDynamics(std::vector<cDynamic*>& vecDyns) override;

	//bool PopulateDynamics(vector<cDynamic*>& vecDyns) override;
	bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature) override;


private:
	void setBanditsPos();
	std::vector< olc::vf2d*>BanditsPositions;
};


class cMap_Village : public cMap
{
public:
	cMap_Village();
	bool PopulateDynamics(std::vector<cDynamic*>& vecDyns) override;

	//bool PopulateDynamics(vector<cDynamic*>& vecDyns) override;
	bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature) override;
};


class cMap_VillageTavern : public cMap
{
public:
	cMap_VillageTavern();
	bool PopulateDynamics(std::vector<cDynamic*>& vecDyns) override;

	//bool PopulateDynamics(vector<cDynamic*>& vecDyns) override;
	bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature) override;
};
struct Tile {
	//olc::vf2d position;
	//olc::vf2d sourceRect;

	int Collision;
	olc::vf2d position;

	//int ZeroLayer;
	//int FirstLayer;
	//int DynamicLayer;
	//int SecondLayer;
	//int ThirdLayer;
	//int FourLayer;

	struct layerinfo
	{
		int index;
		olc::vf2d sourcerect = {0,0};
		olc::vf2d size = {64,64};
		int layerindex;
	};


	layerinfo ZeroLayer;
	layerinfo FirstLayer;
	layerinfo DynamicLayer;
	layerinfo SecondLayer;
	layerinfo ThirdLayer;
;};

bool compareByLayerIndex(const Tile& a, const Tile& b);