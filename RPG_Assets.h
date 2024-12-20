#pragma once
#include "olcPixelGameEngine.h"
#include <SFML/Audio.hpp>
#include <map>

class cMap;
class cItem;
class cUI;
class cQuest;
class cDynamic;
class CachedPosition;
struct spriteData
{

	spriteData(int width, int height,int channels, unsigned char* data) 
	{
		this->width = width;
		this->height = height;
		this->channels = channels;
		this->data = data;
	}
	spriteData(spriteData& a)
	{
		this->width = a.width;
		this->height = a.height;
		this->channels = a.channels;
		this->data = a.data;
	}
	int width, height, channels;
	unsigned char* data = nullptr;



};

class RPG_Assets
{
public:
	static RPG_Assets& get()
	{

		static RPG_Assets me;
		return me;
	}

	RPG_Assets(RPG_Assets const&) = delete;           //we delete it becouse we don/t want have any copies of this 
	void operator =(RPG_Assets const&) = delete;



	olc::Decal* GetSprite(std::string name)
	{
	
		return m_mapSprites[name];       //filled it in Load Sprites

	}
	cMap* GetMap(std::string name)
	{
		return m_mapMaps[name];
	}
	
	cItem* GetItem(std::string name)
	{

		return m_mapItems[name];
	}
	cUI* GetUiElements(std::string name)
	{

		return m_mapUiElements[name];
	}
	cQuest* GetActiveQuest(int slot)
	{

		return  m_mapQuests[slot];
	}
	void ResetQuests();
	void MakeQuestsActual();

	sf::SoundBuffer* findSound(const std::string& name)
	{
		return soundBuffers[name];
	}
	std::vector<std::vector<CachedPosition>>* GetCache(const std::string& name)
	{
		return  m_mapCacheFx[name];
	}

	

	void LoadSprites();
	void LoadMaps();
	void LoadItems();
	void LoadUiElements();
	void LoadQuests();
	void LoadMusc();
	void LoadAllCache();
	void playMusic(const std::string& filename, bool loop = true);
	void SetMusicVolume(float number);
	void UpdateSounds();
	
	void ScaleCache(std::vector<std::vector<CachedPosition>>& Cache, float size, int frames);
	

private:
	RPG_Assets();
	~RPG_Assets();

	void loadSound(const std::string& name, const std::string& filename);


	std::vector<std::vector<CachedPosition>>* loadCache(const std::string& filename);



	// ������ �������������
	std::vector<std::vector<CachedPosition>>* cachedPositions; // �������������

	
	sf::Sound soundcurr;
	sf::Music music;  // ������� ������
	
	std::map<std::string, sf::SoundBuffer*> soundBuffers;  // ������ ��� ������
	std::map<std::string, olc::Decal*> m_mapSprites;
	std::map<std::string, std::vector<std::vector<CachedPosition>>*> m_mapCacheFx;


	std::map<std::string, spriteData*> compressMap;

	std::map<std::string, cMap*> m_mapMaps;
	std::map<std::string, cItem*> m_mapItems;
	std::map<std::string, cUI*> m_mapUiElements;
	std::map<int, cQuest*> m_mapQuests;
};
