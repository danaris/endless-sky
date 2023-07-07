/* MenuPanel.cpp
Copyright (c) 2014 by Michael Zahniser

Endless Sky is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

Endless Sky is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "EditorPanel.h"

#include "Audio.h"
#include "Command.h"
#include "Files.h"
#include "FillShader.h"
#include "text/Font.h"
#include "text/FontSet.h"
#include "text/Format.h"
#include "GameData.h"
#include "Information.h"
#include "Interface.h"
#include "LoadPanel.h"
#include "Logger.h"
#include "MainPanel.h"
#include "Planet.h"
#include "PlayerInfo.h"
#include "Point.h"
#include "PreferencesPanel.h"
#include "Rectangle.h"
#include "Screen.h"
#include "Ship.h"
#include "Sprite.h"
#include "StarField.h"
#include "StartConditionsPanel.h"
#include "System.h"
#include "text/Table.h"
#include "UI.h"

#include "opengl.h"

#include <algorithm>
#include <cassert>
#include <stdexcept>

using namespace std;

namespace {
	const int SCROLL_MOD = 2;
	int scrollSpeed = 1;
	bool showCreditsWarning = true;
}



EditorPanel::EditorPanel(PlayerInfo &player, UI &gamePanels)
	: player(player), gamePanels(gamePanels), editorUi(GameData::Interfaces().Get("editor"))
{
	assert(GameData::IsLoaded() && "EditorPanel should only be created after all data is fully loaded");
	SetIsFullScreen(true);

	if(gamePanels.IsEmpty())
	{
		gamePanels.Push(new MainPanel(player));
		// It takes one step to figure out the planet panel should be created, and
		// another step to actually place it. So, take two steps to avoid a flicker.
		gamePanels.StepAll();
		gamePanels.StepAll();
	}

	if(player.GetPlanet())
		Audio::PlayMusic(player.GetPlanet()->MusicName());
}



void EditorPanel::Step()
{
	
}

bool EditorPanel::Hover(int x, int y)
{
	hoverPoint = Point(x, y);

	hover = -1;
	for(unsigned index = 0; index < zones.size(); ++index)
		if(zones[index].Contains(hoverPoint))
			hover = index;

	return true;
}



void EditorPanel::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	GameData::Background().Draw(Point(), Point());

	Information info;
	info.SetCondition("editor test a");
	info.SetString("first", "Hello World!");
	info.SetString("second", "This is a test.");

	//GameData::Interfaces().Get("menu background")->Draw(info, this);
	editorUi->Draw(info, this);
	//GameData::Interfaces().Get("menu player info")->Draw(info, this);
	DrawTypeList();

}

void EditorPanel::DrawTypeList()
{
	
	const Color &opaque = *GameData::Colors().Get("panel background");
	const Color &back = *GameData::Colors().Get("faint");
	const Color &dim = *GameData::Colors().Get("dim");
	const Color &medium = *GameData::Colors().Get("medium");
	const Color &bright = *GameData::Colors().Get("bright");
	
	double leftEdge = Screen::Width() * -.5;
	double topEdge = Screen::Height() * -.5;
	double bottomEdge = Screen::Height() * .5;
	
	FillShader::Fill(Point(leftEdge + 20. + 150, 0.), Point(300., Screen::Height() - 40.), opaque);
	
	Table typeTable;
	typeTable.AddColumn(125, {150, Alignment::LEFT});
	typeTable.SetUnderline(0, 130);
	typeTable.DrawAt(Point(leftEdge + 30., topEdge + 30.));

	for(const string &dataType : dataTypes)
	{

//				table.SetHighlight(56, 120);
//				table.DrawHighlight(isEditing ? dim : isEmpty ? noCommand : warning);
		
		int index = zones.size();
		// Mark the selected row.
		bool isHovering = (index == hover);
		if(!isHovering && index == selected)
		{
			typeTable.SetHighlight(-120, 54);
			typeTable.DrawHighlight(back);
		}

		// Highlight whichever row the mouse hovers over.
		typeTable.SetHighlight(-120, 120);
		if(isHovering)
			typeTable.DrawHighlight(back);

		zones.emplace_back(typeTable.GetCenterPoint(), typeTable.GetRowSize(), dataType);

		typeTable.Draw(dataType, medium);
		//typeTable.Draw(command.KeyName(), isEditing ? bright : medium);
		typeTable.DrawGap(5);
		typeTable.Advance();
	}
}


bool EditorPanel::KeyDown(SDL_Keycode key, Uint16 mod, const Command &command, bool isNewPress)
{
	if(key == 'q')
		GetUI()->Quit();
	else
		return false;

	return true;
}



bool EditorPanel::Click(int x, int y, int clicks)
{
	// Double clicking on the credits pauses/resumes the credits scroll.
//	if(clicks == 2 && mainMenuUi->GetBox("credits").Contains(Point(x, y)))
//	{
//		scrollingPaused = !scrollingPaused;
//		return true;
//	}

	return false;
}


