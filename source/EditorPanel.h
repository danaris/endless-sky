/* EditorPanel.h
Copyright (c) 2023 by Timothy Collett

Endless Sky is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

Endless Sky is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef EDITOR_PANEL_H_
#define EDITOR_PANEL_H_

#include "Panel.h"
#include "ClickZone.h"
#include "Point.h"

#include <string>
#include <vector>

class Interface;
class PlayerInfo;
class UI;



// Class representing the editor
class EditorPanel : public Panel {
public:
	EditorPanel(PlayerInfo &player, UI &gamePanels);

	virtual void Step() override;
	virtual void Draw() override;

	virtual std::string PanelType() const override;

protected:
	// Only override the ones you need; the default action is to return false.
	virtual bool KeyDown(SDL_Keycode key, Uint16 mod, const Command &command, bool isNewPress) override;
	virtual bool Click(int x, int y, int clicks) override;
	virtual bool Hover(int x, int y) override;
	//virtual bool Scroll(double dx, double dy) override;

private:
	void DrawTypeList();
	
private:
	

	const Interface *editorUi;

	long long int scroll = 0;
	bool scrollingPaused = false;
	
	int editing;
	int selected;
	int hover;
	int oldSelected;
	int oldHover;
	int latest;
	Point hoverPoint;
	
	std::vector<ClickZone<std::string>> zones;
	
	const std::vector<std::string> dataTypes = {"Colors","Conversations","Effects","Events","Fleets","Formations","Galaxies","Governments","Hazards","Interfaces","Minables","Missions","News","Outfits","Persons","Phrases","Planets","Ships","Systems","Tests","TestDataSets","ShipSales","OutfitSales","Wormholes"};
};



#endif
