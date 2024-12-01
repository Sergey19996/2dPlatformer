#pragma once
#include "RPG_UI.h"


class TalentController
{
public:

	void AddTalent(cUI* Talent);

	void SaveTalent();

	void loadTalent();

private:
	std::vector<cUI*> TalentVec;



};

