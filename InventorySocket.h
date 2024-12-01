
#pragma once
#include "RPG_Items.h"

struct InventaryItem
{
	cItem* Item = nullptr;


	uint8_t Uiindex = 0;
	uint8_t index = 0;  // Index in inventory


	// Переменная для хранения всех флагов

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
private:
	unsigned int InventaryFlags = 0;

};
