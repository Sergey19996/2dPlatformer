#include "RPG_UI.h"
#include "RPG_Engine.h"

RPG_Engine* cUI::g_engine = nullptr;
cScriptProcessor* UIButton::g_script = nullptr;
cUI::cUI(std::string name)
{
	sName = name; 

}

Event::Event(std::string name,olc::vi2d soursepos, olc::vi2d soursesize, float MaxTImer) :cUI (name)
{
	timer = MaxTimer;
	sourcePos = soursepos;
	sourceSize = soursesize;
	MaxTimer = MaxTImer;
}

void Event::DrawSelf()
{

	g_engine->DrawPartialDecal({ (float)g_engine->ScreenWidth() / 2 - (sourceSize.x / 2), (float)g_engine->ScreenHeight() / 4 - sourceSize.y, }, g_engine->D_FullUi, sourcePos, sourceSize, { 1,1 }, olc::Pixel(255, 255, 255, Alpha));


}

bool Event::update(float FelapsedTime)
{
	timer -= FelapsedTime;

	Alpha = (timer / MaxTimer) * 255.0f;  // вычисление альфа-значения

	if (timer<=0.0f)
	{
		Redundant = true;
		Alpha = 0.0f;
		return true;
	}

	return false;

}


UIButton::UIButton(std::string name) : cUI (name)
{
}

bool UIButton::OnInteraction(olc::vi2d& Mouse)
{
	if (Mouse.x >=Pos.x && Mouse.y>=Pos.y && 
		Mouse.x <=Pos.x+ sourceSize.x && Mouse.y<=Pos.y+sourceSize.y)
	{
	return true;

	}
	return false;
}

void UIButton::Update(const float& fElapsedTime , olc::vi2d& mouse )
{
	Interacted = OnInteraction(mouse);
}

void UIButton::DrawSelf()
{

	
	auto textColor = Interacted ? olc::YELLOW : olc::WHITE;

	g_engine->DrawBigText(sName,  Pos.x, Pos.y, 1 * g_engine->fscale, 1 * g_engine->fscale, textColor);



}

void UIButton::SwitchChain(UiRect*Back)
{

	Rect = Back;

	CalculatePosition();

}

UIStartButton::UIStartButton() : UIButton("Start")
{
	Rect = (UiRect*)RPG_Assets::get().GetUiElements("MainRect");

	CalculatePosition();


}

bool UIStartButton::Onclick(olc::vi2d& mouse)
{
	if (Interacted)
	{
		g_script->AddCommand((new cComand_HideScreen(2)));
		g_script->AddCommand((new cComand_Changemap("Forest", -1, 25.5)));
		g_script->AddCommand((new cComand_SetNgameMod(1)));
	}
	return true;
}

void UIStartButton::CalculatePosition()
{
	Pos = { static_cast<int>(25 * g_engine->fscale) + Rect->GetRectPosX(),static_cast<int>(30 * g_engine->fscale) + Rect->GetRectPosY() };
	sourceSize = { static_cast<int>(sName.size() * (18 * g_engine->fscale) + (32 * g_engine->fscale)),static_cast<int>(32 * g_engine->fscale) };

}

UIContinueButton::UIContinueButton() : UIButton("Continue")
{
	Rect = (UiRect*)RPG_Assets::get().GetUiElements("MainRect");

	CalculatePosition();

	std::ofstream data;
	data.open("Load/CurrSave.bin", std::ofstream::in);
	if (data.is_open())
	{
		bSaveExist = true;

	}
	else
	{
		bSaveExist = false;

	}
		data.close();

}

bool UIContinueButton::Onclick(olc::vi2d& mouse)
{
	if (Interacted)
	{
		g_script->AddCommand((new cComand_HideScreen(2)));
		g_script->AddCommand((new cComand_LoadFunction));  // in load function in start allredy 2 comands but they call not instant 
	}



		return true;

}

void UIContinueButton::DrawSelf()
{



	if (bSaveExist)
	{
		//Interacted = OnInteraction(Mouse);
		auto textColor = Interacted ? olc::YELLOW : olc::WHITE;

		g_engine->DrawBigText(sName,  Pos.x, Pos.y, 1 * g_engine->fscale, 1 * g_engine->fscale, textColor);
	}
	else
	{
		olc::Pixel Color = olc::GREY;

		g_engine->DrawBigText(sName,  Pos.x, Pos.y,  1 * g_engine->fscale, 1 * g_engine->fscale, Color);
	}


		
}

void UIContinueButton::CalculatePosition()
{
	Pos = { static_cast<int>(25 * g_engine->fscale) + Rect->GetRectPosX(),0 + Rect->GetRectPosY() };
	sourceSize = { static_cast<int>(sName.size() * (18 * g_engine->fscale) + (32 * g_engine->fscale)),static_cast<int>(32 * g_engine->fscale) };

}

UIOptionButton::UIOptionButton() :UIButton("Options")
{
	Rect = (UiRect*)RPG_Assets::get().GetUiElements("MainRect");
	CalculatePosition();
	
}

bool UIOptionButton::Onclick(olc::vi2d& mouse)
{
	if (Interacted)
	{

		 g_engine->OptionSettings();

	}

	return true;
}

void UIOptionButton::CalculatePosition()
{
	
	

	Pos = { static_cast<int>(25 * g_engine->fscale) + Rect->GetRectPosX(),static_cast<int>(60 * g_engine->fscale) + Rect->GetRectPosY() };
	sourceSize = { static_cast<int>(sName.size() * (18 * g_engine->fscale) + (32 * g_engine->fscale)),static_cast<int>(32 * g_engine->fscale) };


}

UIQuitButton::UIQuitButton() :UIButton("Quit") 
{
	Rect = (UiRect*)RPG_Assets::get().GetUiElements("MainRect");
	CalculatePosition();


}

bool UIQuitButton::Onclick(olc::vi2d& mouse)
{
	if (Interacted)
	{

		return !Interacted;
	}
	else
	{
		return true;
	}

}

void UIQuitButton::CalculatePosition()
{
	Pos = { static_cast<int>(25 * g_engine->fscale) + Rect->GetRectPosX(),static_cast<int>(90 * g_engine->fscale) + Rect->GetRectPosY() };
	sourceSize = { static_cast<int>(sName.size() * (18 * g_engine->fscale) + (32 * g_engine->fscale)),static_cast<int>(32 * g_engine->fscale) };

}

UIFullScreenButton::UIFullScreenButton(): UIButton("FullScreen")
{
	Rect = (UiRect*)RPG_Assets::get().GetUiElements("OptionRect");

	CalculatePosition();

	


}

void UIFullScreenButton::DrawSelf()
{

	//Interacted = OnInteraction(mouse);
	auto textColor = Interacted ? olc::YELLOW : olc::WHITE;

	int draw = spritesAr.size() - 1 + Toggle;

	for (int i = 0; i < draw; i++)
	{
		
		g_engine->DrawPartialDecal({(float)spritesAr[i].Pos.x,(float)spritesAr[i].Pos.y }, g_engine->D_FullUi, spritesAr[i].SourcePos, spritesAr[i].SourceSize,{2*g_engine->fscale,2*g_engine->fscale}, textColor);

	}



}

bool UIFullScreenButton::OnInteraction(olc::vi2d& Mouse)
{ 
	if (Mouse.x >= spritesAr[1].Pos.x && Mouse.y >= spritesAr[1].Pos.y &&   // описываем задник галочки 
		Mouse.x <= spritesAr[1].Pos.x + spritesAr[1].SourceSize.x &&
		Mouse.y <= spritesAr[1].Pos.y + spritesAr[1].SourceSize.y)
	{
		return true;

	}
	return false;
}

bool UIFullScreenButton::Onclick(olc::vi2d& mouse)
{
	if(Interacted)
	Toggle = SwitchScreen();



	return true;
}
bool UIFullScreenButton::SwitchScreen()
{

	HWND hWnd = FindWindowW(L"OLC_PIXEL_GAME_ENGINE", NULL);

	// Получаем текущие стили окна
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	DWORD dwExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);

	// Проверяем, находимся ли мы в полноэкранном режиме
	bool bFullScreen = (dwStyle & WS_OVERLAPPEDWINDOW) == 0;

	// Если окно в полноэкранном режиме
	if (bFullScreen)
	{
		// Переводим в оконный режим
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW); // Восстанавливаем оконный стиль
		SetWindowLong(hWnd, GWL_EXSTYLE, dwExStyle | WS_EX_APPWINDOW); // Восстанавливаем стиль
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED); // Устанавливаем размеры окна

		// Восстанавливаем размер и положение окна (можно заменить на ваши значения)
		ShowWindow(hWnd, SW_SHOWNORMAL); // Показываем окно
		return false;
	}
	else
	{
		// Переводим в полноэкранный режим
		SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW); // Убираем оконные рамки
		SetWindowLong(hWnd, GWL_EXSTYLE, dwExStyle & ~WS_EX_APPWINDOW); // Убираем стиль окна приложения
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_NOZORDER | SWP_FRAMECHANGED); // Полный экран

		ShowWindow(hWnd, SW_MAXIMIZE); // Максимизируем окно
		return true;
	}



	
}
void UIFullScreenButton::CalculatePosition()
{

	spritesAr =
	{
		ButtonData({191 + Rect->GetRectPosX(),27 + Rect->GetRectPosY()},{560,482},{78,11}), // <--Text   Pos, soursepos, size
		ButtonData({272 + Rect->GetRectPosX(),16 + Rect->GetRectPosY()},{448,448},{32,32}), // <--ArrowBack   Pos, soursepos, size
		ButtonData({276 + Rect->GetRectPosX(),26 + Rect->GetRectPosY()},{448,426},{24,17}) // <--Arrow   Pos, soursepos, size
	};

}

void UiRect::DrawSelf()
{

	g_engine->DrawPartialDecal({ (float)offsetX,(float)offsetY }, g_engine->D_FullUi, sourcePos, sourceSize,{fscale*g_engine->fscale,fscale*g_engine->fscale });

}

UIMainRect::UIMainRect() : UiRect("MainRect")
{ 
	sourcePos = { 832,0 };
	sourceSize = { 334,142 };

	offsetX = 25;     // позиция на ээкране
	offsetY = g_engine->ScreenHeight() - (150 * g_engine->fscale);




}

UIOptionRect::UIOptionRect() : UiRect("OptionRect")
{

	sourcePos = { 449,530 };
	sourceSize = { 336,144 };

	offsetX = 2 * g_engine->CellSize;
	offsetY = 1 * g_engine->CellSize;


	

}


UIGameRect::UIGameRect() : UiRect("GameRect")
{



	sourcePos = { 448,128 };
	sourceSize = { 167,65 };

	offsetX = 0;  //позиция на экране 
	offsetY = g_engine->ScreenHeight() - sourceSize.y;
}



UIExitButton::UIExitButton() :UIButton("ExitButton")
{
	Rect = (UiRect*)RPG_Assets::get().GetUiElements("OptionRect");
	CalculatePosition();

	

}

bool UIExitButton::Onclick(olc::vi2d& mouse)
{

	if (Interacted)
	{

		 g_engine->ReturnBackSettings();

	}
	return true;
}

void UIExitButton::DrawSelf()
{


	//Interacted = OnInteraction(Mouse);
	//auto textColor = Interacted ? olc::YELLOW : olc::WHITE;

	//g_engine->DrawBigText(sName, Rect->GetRectPosX() + Pos.x, Rect->GetRectPosY() + Pos.y, 1 * g_engine->fscale, 1 * g_engine->fscale, textColor);

	g_engine->DrawPartialDecal(Pos, g_engine->D_FullUi, sourcePos, sourceSize, { 2 * g_engine->fscale,2 * g_engine->fscale });

}

void UIExitButton::CalculatePosition()
{
	sourcePos = { 615 ,128 };
	sourceSize = { 16 ,16 };
	Pos = { 320 + Rect->GetRectPosX(),0 + Rect->GetRectPosY() };


}

UIScroller::UIScroller(std::string name) :UIButton(name)
{

	offsetX = 4;  //<--4 пикселя офсет по краям для бордеров

	Rect = (UiRect*)RPG_Assets::get().GetUiElements("OptionRect");
}

void UIScroller::DrawSelf()
{


	

	for (int i = 0; i < spritesAr.size(); i++)
	{

		g_engine->DrawPartialDecal(spritesAr[i].Pos, g_engine->D_FullUi, spritesAr[i].SourcePos, spritesAr[i].SourceSize, { 2 * g_engine->fscale,2*g_engine->fscale });
	}

}

void UIScroller::Update(const float& felapsedtime,olc::vi2d& Mouse)
{
	if (Interacted)
	{
		//int cursor = Cursor;



		spritesAr[Cursor].Pos.x = Mouse.x;

		
		checkBorder();
		CalculateRelation();
		ScrollerAction(ScrollLine);   // <--Poly is here

	}
	



}

bool UIScroller::Onclick(olc::vi2d& mouse)
{
	
	Interacted = (Interacted == true) ? false : OnInteraction(mouse);





	return true;
}

bool UIScroller::OnInteraction(olc::vi2d& Mouse)
{
	if (olc::vi2d{ spritesAr[Cursor].Pos - Mouse }.mag() < spritesAr[Cursor].SourceSize.mag())
	{
		return true;

	}
	return false;
}

void UIScroller::checkBorder()
{
	if (spritesAr[Cursor].Pos.x < MIN)
	{
		spritesAr[Cursor].Pos.x = MIN;
	}
	if (spritesAr[Cursor].Pos.x > MAX)
	{
		spritesAr[Cursor].Pos.x = MAX;
	}

}

void UIScroller::CalculateRelation()
{

	float cursorOnLine = (spritesAr[Cursor].Pos.x - MIN);

	ScrollLine = (cursorOnLine * 100) / (MAX - MIN);

	
}


UISoundScroller::UISoundScroller() : UIScroller("SoundScroller")
{
	/*sourcePos =
	sourceSize =
	Pos =*/
	CalculatePosition();


}

void UISoundScroller::ScrollerAction(int parametr)
{
	g_engine->setSoundVolume(parametr);
}

void UISoundScroller::CalculatePosition()
{
	spritesAr =
	{
		ButtonData({17 + Rect->GetRectPosX(),9 + Rect->GetRectPosY()},{448,482 + 24},{112,24}), // <--Back   Pos, soursepos, size
		ButtonData({72 + Rect->GetRectPosX(),23 + Rect->GetRectPosY()},{0,128},{3,9}) // <--Toggle   Pos, soursepos, size

	};

	MIN = spritesAr[Back].Pos.x + offsetX;
	MAX = spritesAr[Back].Pos.x + spritesAr[Back].SourceSize.x - offsetX - spritesAr[Cursor].SourceSize.x + 1;

}

UIMusicScroller::UIMusicScroller() : UIScroller("MusicScroller")
{

	CalculatePosition();


}

void UIMusicScroller::ScrollerAction(int parametr)
{

	RPG_Assets::get().SetMusicVolume(parametr);

}

void UIMusicScroller::CalculatePosition()
{
	spritesAr =
	{
		ButtonData({17 + Rect->GetRectPosX(),41 + Rect->GetRectPosY()},{448,482},{112,24}), // <--Back   Pos, soursepos, size
		ButtonData({72 + Rect->GetRectPosX(),55 + Rect->GetRectPosY()},{0,128},{3,9}) // <--Toggle   Pos, soursepos, size

	};

	MIN = spritesAr[Back].Pos.x + offsetX;
	MAX = spritesAr[Back].Pos.x + spritesAr[Back].SourceSize.x - offsetX - spritesAr[Cursor].SourceSize.x + 1;

}

UITalentIcon::UITalentIcon() : UIIcons("UiTalentIcon")
{

	int fscale = g_engine->CellSize * g_engine->fscale;

	SpriteDataAr =

	{

		ButtonData({25 * fscale,g_engine->ScreenHeight() - 18},{705,35},{66,18}),  // <ScreenPos, soursepos,size
		ButtonData({25 * fscale-1,g_engine->ScreenHeight() - 18},{486,204},{18,17})

	};


	//sourcePos = { 704,36 };
//	sourceSize = { 67,18 };

//	offsetX = 25*g_engine->CellSize*g_engine->fscale;  //позиция на экране 
//	offsetY = g_engine->ScreenHeight() - sourceSize.y;
}

bool UITalentIcon::Activate()
{
	
	return g_engine->GetTalentPoint();  // Return true when talent point is exist
}



UIInventoryIcon::UIInventoryIcon() : UIIcons("UiInventoryIcon")
{
	int fscale = g_engine->CellSize * g_engine->fscale;
	SpriteDataAr =

	{

		ButtonData({19 * fscale,g_engine->ScreenHeight() - 18},{705,17},{82,17}),  // <ScreenPos, soursepos,size
		ButtonData({19 * fscale+1,g_engine->ScreenHeight() - 18},{560,204},{17,17})

	};


}

bool UIInventoryIcon::Activate()
{
	return g_engine->GetInventoryChecked();
}

UIQuestIcon::UIQuestIcon() : UIIcons("UiQuestIcon")
{
	int fscale = g_engine->CellSize * g_engine->fscale;

	SpriteDataAr =

	{

		ButtonData({14 * fscale,g_engine->ScreenHeight() - 18},{705,0},{65,16}),  // <ScreenPos, soursepos,size
		ButtonData({14 * fscale+1,g_engine->ScreenHeight() - 18},{629,204},{18,16})

	};


}

bool UIQuestIcon::Activate()
{
	return g_engine->GetCheckedQuests();
}

void UIIcons::DrawSelf()
{

	if (activator)
	{


		float offsetX = (float)SpriteDataAr[Animlayer].SourcePos.x + (index * SpriteDataAr[Animlayer].SourceSize.x);



	g_engine->DrawPartialDecal(SpriteDataAr[BackLayer].Pos, g_engine->D_FullUi, SpriteDataAr[BackLayer].SourcePos, SpriteDataAr[BackLayer].SourceSize);

	g_engine->DrawPartialDecal(SpriteDataAr[Animlayer].Pos, g_engine->D_FullUi, { offsetX,(float)SpriteDataAr[Animlayer].SourcePos.y }, SpriteDataAr[Animlayer].SourceSize);
	
	}
	else
	{

		g_engine->DrawPartialDecal(SpriteDataAr[BackLayer].Pos, g_engine->D_FullUi, SpriteDataAr[BackLayer].SourcePos, SpriteDataAr[BackLayer].SourceSize);
	}
}

void UIIcons::Update(const float& fElapsedTime, olc::vi2d& mouse )
{
	if (activator =Activate())
	{


	animsprite -= fElapsedTime;

	if (animsprite <= 0.0f)
	{
		index = (index + 1) % 4; // Увеличиваем индекс и зацикливаем
		animsprite += 0.2f;      // Добавляем длительность текущего кадра
	}

	}
}

UICurrentSpell::UICurrentSpell(std::string name) : cUI(name)
{


}

bool UICurrentSpell::OnInteraction(olc::vi2d& Mouse)
{
	if (olc::vi2d{ (offsetX+sourceSize.x/2) - Mouse.x,(offsetY+sourceSize.y/2) - Mouse.y }.mag() < sourceSize.x)
	{
		return true;

	}
	return false;

}

void UICurrentSpell::Update(const float& fElapsedTime, olc::vi2d& mouse )
{

	
}

void UICurrentSpell::DrawSelf()
{

	g_engine->DrawPartialDecal({ (float)offsetX,(float)offsetY }, g_engine->D_FullUi, CurrSPell.first, CurrSPell.second, { 0.57f * g_engine->fscale,0.57f * g_engine->fscale });

}



UICurrentEnergySpell::UICurrentEnergySpell() : UICurrentSpell("EnergySpell")
{

	UIGameRect rect;



	offsetX = rect.GetRectPosX() + 71;
	offsetY = rect.GetRectPosY() + 39;

	int size = g_engine->CellSize * g_engine->fscale;
	sourceSize = { size, size };


}

void UICurrentEnergySpell::Update(const float& fElapsedTime , olc::vi2d& mouse)
{
	CurrSPell.second = { 64,64 };


	if (g_engine->checkUiFlag(RPG_Engine::UiFlags::BUISWIRL))
	{
		CurrSPell.first = { 512,64 };
	}
	else if (g_engine->checkUiFlag(RPG_Engine::UiFlags::BUIBACKSTUB)) {

		CurrSPell.first = { 192,64 };
	}
	else if (g_engine->GetEnergy() > 99)
	{
		CurrSPell.first = { 128,64 };
		//	CurrSPell.second = { 64,64 };
	}
	else if (g_engine->GetEnergy() > 55)
	{
		CurrSPell.first = { 64,64 };
		//	CurrSPell.second = { 64,64 };
	}
	else if (g_engine->GetEnergy() > 35)
	{
		CurrSPell.first = { 0,64 };
		//	CurrSPell.second = { 64,64 };
	}
	else
	{
		CurrSPell.first = { 0,0 };
		//	CurrSPell.second = { 0,0 };
	}


}

UICurrRageSpell::UICurrRageSpell() :UICurrentSpell("RageSpell")
{
	UIGameRect rect;

	

	offsetX = rect.GetRectPosX() + 119;
	offsetY = rect.GetRectPosY() + 39;

	int size = g_engine->CellSize * g_engine->fscale;
	sourceSize = { size, size };


}

void UICurrRageSpell::Update(const float& fElapsedTime, olc::vi2d& mouse )
{
	g_engine->CheckVerticalDirection();
	CurrSPell.second = { 64,64 };

	switch (g_engine->CheckVerticalDirection())
	{
	case 1:   // < --Up

	//	if (g_engine->GetLearnedTalent(13))
	//	{

			if (g_engine->GetRage() > 35)
			{
				CurrSPell.first = { 384,64 };
			}
			else
			{
				CurrSPell.first = { 384,0 };
			}
	//	}
		break;
	case 0:    // <--Down
		if (g_engine->GetbOnGraund())
		{
			if (g_engine->GetRage() > 35)
			{
				CurrSPell.first = { 448,64 };

			}
			else
			{
				CurrSPell.first = { 448,0 };
			}

		}
		else
		{ 
	
		//	if (g_engine->GetLearnedTalent(14))
		//	{
				if (g_engine->GetRage() > 35)
				{
					CurrSPell.first = { 320,64 };
				}
				else
				{
					CurrSPell.first = { 320,0 };
				}
	//	}
		}

		break;

	default:

		if (g_engine->GetRage() > 35)
		{
			CurrSPell.first = { 256,64 };
			//	CurrSPell.second = { 64,64 };
		}
		else
		{
			CurrSPell.first = { 256,0 };
		}


		break;
	}


}

UIEnergyIndicator::UIEnergyIndicator() : UIMainIndicator("GameEnergy")
{
	UIGameRect rect;


	MAX = 30;
	MaxSoursePos = 194;
	sourcePos = { 449,MaxSoursePos };
	sourceSize = { 15,MAX };

	offsetX = rect.GetRectPosX() + 96;
	offsetY = rect.GetRectPosY() + 34;

}


uint16_t UIEnergyIndicator::GetIndicator()
{
	return g_engine->GetEnergy() * (MAX / 100.0f);
}

void UIMainIndicator::Update(const float& fElapsedTime , olc::vi2d& mouse)
{
	//       30//               0 -- 30 
	int Mainproportion = GetIndicator(); // <-- 0 до 30 

	proportion = MAX - Mainproportion;   // когда proportion будет 30 даст 0


	sourceSize = { sourceSize.x,static_cast<int>(Mainproportion) };
	sourcePos = { sourcePos.x,MaxSoursePos + static_cast<int>(proportion) };


}

void UIMainIndicator::DrawSelf()
{
	g_engine->DrawPartialDecal({ (float)offsetX,(float)offsetY+proportion  }, g_engine->D_FullUi, sourcePos, sourceSize, { 2 * g_engine->fscale,2 * g_engine->fscale });

	
}

UIRageIndicator::UIRageIndicator() : UIMainIndicator("GameRage")
{
	UIGameRect rect;
	MAX = 31;
	MaxSoursePos = 193;
	sourcePos = { 464,MaxSoursePos };
	sourceSize = { 13,MAX };

	offsetX = rect.GetRectPosX() + 147;
	offsetY = rect.GetRectPosY() + 33;
}

uint16_t UIRageIndicator::GetIndicator()
{
	return g_engine->GetRage() * (MAX / 100.0f);
}

UIFastSlot::UIFastSlot(std::string name,uint16_t offsetX, uint16_t offsetY) : cUI (name)
{
	UIGameRect rect;

	this->offsetX = rect.GetRectPosX() + offsetX;
	this->offsetY = rect.GetRectPosY() + offsetY;

	int size = g_engine->CellSize * g_engine->fscale;
	sourceSize = { size ,size };
}

bool UIFastSlot::OneClick(cDynamic* player)
{
	if (Item !=nullptr)
	{
		Item = (Item->Item->OnUse(player, Item)) ? nullptr : Item;

		return false;
	}

	return true;
}

void UIFastSlot::DrawSelf()
{
	if (Item != nullptr)
	{
		if (Item->Item!=nullptr)
		{

		float sprcoordX = Item->Item->spriteindex % 32 * 64;
		float sprcoordY = Item->Item->spriteindex / 32 * 64;
		g_engine->DrawPartialDecal({ (float)offsetX,(float)offsetY }, Item->Item->pSprite, { sprcoordX, sprcoordY }, { 64,64 }, { 0.5f * g_engine->fscale,0.5f * g_engine->fscale });
		}
	}

}

bool UIFastSlot::OnInteraction(const olc::vi2d& Mouse)
{
	if (olc::vi2d{ (offsetX+sourceSize.x/2) - Mouse.x,(offsetY+sourceSize.y/2) - Mouse.y }.mag() < 8)
	{
		return true;
	}

	return false;
}

void UIFastSlot::ConnectSlot(const olc::vi2d& mouse, InventaryItem* grabitem)
{
	if (OnInteraction(mouse) && grabitem !=nullptr)
	{
		if (grabitem->Item->consumable)
		{

		Item = grabitem;
	
		}
	}

}

UITalentRect::UITalentRect() : UiRect("TalentRect", 1.0f)
{


	sourcePos = { 0,142 };
	sourceSize = { 446,446 };

	offsetX = g_engine->ScreenWidth() / 2 - (sourceSize.x/4);
	offsetY = g_engine->ScreenHeight() / 2 - (sourceSize.y/4);
}

UIExperienceIndicator::UIExperienceIndicator() : UIMainIndicator("ExperienceIndicator")
{

	MAX = 115;

	sourcePos = { 960,142 };
	sourceSize = {115,17 };

	offsetX = 0;
	offsetY = 0;

}

void UIExperienceIndicator::Update(const float& fElapsedTime , olc::vi2d& mouse )
{

	proportion = g_engine->GetCurrExp() * (MAX / 100);


}

void UIExperienceIndicator::DrawSelf()
{
	g_engine->DrawPartialDecal({ (float)offsetX,(float)offsetY }, g_engine->D_FullUi, sourcePos, sourceSize, { 2 * g_engine->fscale,2 * g_engine->fscale });
	g_engine->DrawPartialDecal({ (float)offsetX,(float)offsetY }, g_engine->D_FullUi, { (float)sourcePos.x,sourcePos.y + (float)17 }, { proportion, (float)sourceSize.y }, { 2 * g_engine->fscale,2 * g_engine->fscale });
}
uint16_t UIExperienceIndicator::GetIndicator()
{
	return proportion = g_engine->GetCurrExp() * (MAX / 100);
}
