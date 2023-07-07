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

#include "ScrollPanel.h"

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



ScrollPanel::ScrollPanel(Point center, Point size, Panel *child)
{

	int originX = round(center.X() - size.X() / 2);
	int originY = round(center.Y() - size.Y() / 2);
	this->origin = Point(originX, originY);
	
	this->size = size;
	this->child = child;
}



void ScrollPanel::Step()
{
	
}



void ScrollPanel::Draw()
{
	glEnable(GL_SCISSOR_TEST);
	glScissor(origin.X(), origin.Y(), size.X(), size.Y());
	
	child->Draw();
	
	glDisable(GL_SCISSOR_TEST);
}


bool ScrollPanel::KeyDown(SDL_Keycode key, Uint16 mod, const Command &command, bool isNewPress)
{
	return false;
}



bool ScrollPanel::Click(int x, int y, int clicks)
{


	return false;
}

// Allow dragging of the child panel
bool ScrollPanel::Drag(double dx, double dy)
{
	// We give 20px of padding top and bottom
	double maximumScroll = max(0., size.Y() - 40);
	scrollTop = max(0., min(maximumScroll, scrollTop - dy));

	return true;
}



// The scroll wheel can be used to scroll the plunder list.
bool ScrollPanel::Scroll(double dx, double dy)
{
	return Drag(0., dy * Preferences::ScrollSpeed());
}

