/* ScrollPane.cpp
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

#include "ScrollPane.h"

#include "Logger.h"
#include "Point.h"
#include "Rectangle.h"
#include "GameData.h"
#include "FillShader.h"
#include "Screen.h"
#include "Color.h"

#include "opengl.h"
#include <OpenGL/gl.h>

#include <cassert>
#include <stdexcept>

using namespace std;

namespace {
	const int SCROLL_MOD = 2;
	int scrollSpeed = 1;
	bool showCreditsWarning = true;
}

ScrollPane::ScrollPane(Point topLeft, Point size, Pane *child) {
	this->topLeft = topLeft;
	this->size = size;
	
	this->child = child;
	this->childOrigTopLeft = child->GetTopLeft();
	SetTrapAllEvents(false);
}

void ScrollPane::Draw() {
	
	double bottom = size.Y() + topLeft.Y();
	double scrollBottom = bottom + scroll;
	Point bottomLeft = Point(topLeft.X(), scrollBottom);
	child->SetTopLeft(Point(child->GetTopLeft().X(), childOrigTopLeft.Y() - scroll));
	
	//GLint m_viewport[4];
	//glGetIntegerv( GL_VIEWPORT, m_viewport );
	//Logger::LogError("Viewport is (" + to_string(m_viewport[0]) + ", " + to_string(m_viewport[1]) + ") " + to_string(m_viewport[2]) + "x" + to_string(m_viewport[3]));
	//Logger::LogError("Bounds are (" + to_string(bottomLeft.X()) + ", " + to_string(bottomLeft.Y()) + ") " + to_string(size.X()) + "x" + to_string(size.Y()));
	
	glEnable(GL_SCISSOR_TEST);
	GLint scaledX = (topLeft.X() + (Screen::Width() / 2)) * 2.f;
	GLint scaledY = (bottom + (Screen::Height() / 2));// * 2.f;
	GLsizei scaledWidth = size.X() * 2.f;
	GLsizei scaledHeight = size.Y() * 2.f;
	glScissor(scaledX, scaledY, scaledWidth, scaledHeight);
	
	double centerX = size.X() / 2 + topLeft.X();
	double centerY = size.Y() / 2 + topLeft.Y();
	Point center = Point(centerX, centerY);
	const Color &opaque = *GameData::Colors().Get("active mission");
	//FillShader::Fill(center, size, opaque);
	FillShader::Fill(Point(0,0), Point(Screen::RawWidth(), Screen::RawHeight()), opaque);
	
	GLfloat box[4];
	glGetFloatv(GL_SCISSOR_BOX, box);
	//Logger::LogError("Scissor box is (" + to_string(box[0]) + ", " + to_string(box[1]) + ") " + to_string(box[2]) + "x" + to_string(box[3]));
	
	child->Draw();
	glDisable(GL_SCISSOR_TEST);
	
}

bool ScrollPane::Drag(double dx, double dy)
{
	return false;
}
bool ScrollPane::Scroll(double dx, double dy)
{
	return DoScroll(dy);
}


bool ScrollPane::DoScroll(double dy)
{
	double newScroll = scroll;
	double maxScroll = size.Y();

	newScroll = max(0., min(maxScroll, scroll - dy));
	
	scroll = newScroll;

	return true;
}
