
#pragma once
#include "RPG_Items.h"

struct InventaryItem
{
	cItem* Item = nullptr;


	uint8_t Uiindex = 0;
	uint8_t index = 0;  // Index in inventory


	// ���������� ��� �������� ���� ������

	// ��������� ����� ����� enum
	enum InventaryFlagsEnum {
		Breserved = 1 << 0,  // 1-� ���
		Babsorbed = 1 << 1,  // 2-� ���
		Grabitem = 1 << 2,  // 3-� ���
		Highlighted = 1 << 3,  // 4-� ���
		binWarehouse = 1 << 4,  // 5-� ���
		bEquiped = 1 << 5,  // 6-� ���
		Objectselected = 1 << 6,

	};

	// ������ ��� ��������� � �������� ������
	void setFlag(InventaryFlagsEnum flag) {
		InventaryFlags |= flag;  // ������������� ����
	}

	void clearFlag(InventaryFlagsEnum flag) {
		InventaryFlags &= ~flag;  // ���������� ���� //revers number
	}

	bool checkFlag(InventaryFlagsEnum flag) const {
		return InventaryFlags & flag;  // ��������� ����
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
