/* TextPanel.cpp
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

#include "TextPane.h"

#include "Color.h"
#include "Files.h"
#include "text/Font.h"
#include "text/FontSet.h"
#include "text/Format.h"
#include "Logger.h"
#include "Point.h"
#include "Rectangle.h"
#include "GameData.h"
#include "Screen.h"
#include "FillShader.h"
#include "UI.h"

#include <algorithm>
#include <cassert>
#include <stdexcept>

using namespace std;

TextPane::TextPane(Point topLeft, int width, string text, int fontSize, string colorName) {
	this->width = width;
	this->text = text;
	this->topLeft = topLeft;
	
	const Font &font = FontSet::Get(fontSize);
	this->textWrap = WrappedText(font);
	this->textWrap.SetWrapWidth(width);
	textWrap.Wrap(text);
	
	this->colorName = colorName;
	SetTrapAllEvents(false);
	
	Logger::LogError("Screen size is " + to_string(Screen::Width()) + "x" + to_string(Screen::Height()));
	Logger::LogError("Text Pane initialized with top left (" + to_string(topLeft.X()) + ", " + to_string(topLeft.Y()) + ") and size " + to_string(width) + "x" + to_string(textWrap.Height()));
	
	Render();
}

void TextPane::Render()
{
	framebufferName = 0;
	Point glSize = Screen::GLSize(Point(width, textWrap.Height()));
	Point glBottomLeft = Screen::GLPoint(Point(topLeft.X(), topLeft.Y() + textWrap.Height()));
	Logger::LogError("Text Pane rendering with GL size " + to_string(glSize.X()) + "x" + to_string(glSize.Y()));
	UI::HandleGLError("0", __FILE__, to_string(__LINE__));
	glGenFramebuffers(1, &framebufferName);
	UI::HandleGLError("A1", __FILE__, to_string(__LINE__));
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferName);
	UI::HandleGLError("A2", __FILE__, to_string(__LINE__));
	
	textureName = 0;
	glGenTextures(1, &textureName);
	glBindTexture(GL_TEXTURE_2D, textureName);
	UI::HandleGLError("B", __FILE__, to_string(__LINE__));
	
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, glSize.X(), glSize.Y(), 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
	UI::HandleGLError("C", __FILE__, to_string(__LINE__));
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	UI::HandleGLError("D", __FILE__, to_string(__LINE__));
	
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureName, 0);
	UI::HandleGLError("E", __FILE__, to_string(__LINE__));
	
	GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, drawBuffers);
	UI::HandleGLError("F", __FILE__, to_string(__LINE__));
	
	//glBindFramebuffer(GL_FRAMEBUFFER, framebufferName);
	glViewport(glBottomLeft.X(),glBottomLeft.Y(),glSize.X(),glSize.Y());
	const Color &frontColor = *GameData::Colors().Get("active mission");
	const Color &backColor = *GameData::Colors().Get("blocked mission");
	//FillShader::Fill(Point(topLeft.X() + width / 2, topLeft.Y() + textWrap.Height() / 2),Point(width, textWrap.Height()), backColor);
	int fillStartX = 0;
	int fillStartY = 0;
	int fillEndX = Screen::Width();
	int fillEndY = Screen::Height();
	int fillWidth = fillEndX - fillStartX;
	int fillHeight = fillEndY - fillStartY;
	FillShader::Fill(Point(fillStartX, fillStartY),Point(fillWidth, fillHeight), backColor);
	FillShader::Fill(Point(fillStartX + 20, fillStartY + 20),Point(fillWidth-40, fillHeight-40), frontColor);
	
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	Logger::LogError("GL Viewport from (" + to_string(viewport[0]) + ", " + to_string(viewport[1]) + ") at " + to_string(viewport[2]) + "x" + to_string(viewport[3]));
	
	int chunkCount = 100;
	float xZero = -.5 * Screen::Width();
	float yZero =-.5 * Screen::Height();
	float widthChunk = Screen::Width() / chunkCount;
	float heightChunk = Screen::Height() / chunkCount;
	float colorChunk = 0.95 / chunkCount;
	Point stripSize = Point(widthChunk, heightChunk);
	for (int i = 0; i < chunkCount; i++) {
		for (int j = 0; j < chunkCount; j++) {
			Color stripColor = Color(colorChunk * i, colorChunk * j, .95);
			Point stripPoint = Point(xZero + i * widthChunk, yZero + j * heightChunk);
			FillShader::Fill(stripPoint,stripSize, stripColor);
			//Logger::LogError("Text Pane drawing test from (" + to_string(stripPoint.X()) + ", " + to_string(stripPoint.Y()) + ") to (" + to_string(stripPoint.X() + stripSize.X()) + ", " + to_string(stripPoint.Y() + stripSize.Y()));
		}
	}
	const Color &textColor = *GameData::Colors().Get(colorName);
	textWrap.Draw(Point(topLeft.X(),textWrap.Height()), textColor);
	UI::HandleGLError("G", __FILE__, to_string(__LINE__));
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Screen::Width()*2, Screen::Height()*2);
	UI::HandleGLError("H", __FILE__, to_string(__LINE__));
}

void TextPane::Draw() {

	Point glBottomLeft = Screen::GLPoint(Point(topLeft.X(), topLeft.Y() + textWrap.Height()));
	Point glSize = Screen::GLSize(Point(width, textWrap.Height()));
	//Logger::LogError("Text Pane drawing with GL bottom left (" + to_string(glBottomLeft.X()) + ", " + to_string(glBottomLeft.Y()) + ") and GL size " + to_string(glSize.X()) + "x" + to_string(glSize.Y()) + " and screen size " + to_string((GLint)Screen::Width()*2) + "x" + to_string((GLint)Screen::Height()*2));
	//glViewport(glBottomLeft.X(),glBottomLeft.Y(),glSize.X(),glSize.Y());
	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebufferName);
	UI::HandleGLError("I", __FILE__, to_string(__LINE__));
	glBlitFramebuffer(0,0,4000,400,//(GLint)Screen::Width()*2, (GLint)Screen::Height()*2,//glSize.X(),glSize.Y(),
					  //0,0,(GLint)Screen::Width()*2, (GLint)Screen::Height()*2,
					  glBottomLeft.X(),glBottomLeft.Y(),glBottomLeft.X() + glSize.X(),glBottomLeft.Y() + glSize.Y(),
					  GL_COLOR_BUFFER_BIT, GL_LINEAR);
	const Color &backColor = *GameData::Colors().Get("available job");
	UI::HandleGLError("J", __FILE__, to_string(__LINE__));
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	UI::HandleGLError("K", __FILE__, to_string(__LINE__));
	//glViewport(0, 0, Screen::Width()*2, Screen::Height()*2);
}

bool TextPane::Drag(double dx, double dy) {
	
	return false;
}
bool TextPane::Scroll(double dx, double dy) {
	
	return false;
}

string TextPane::GetText()
{
	return text;
}

void TextPane::SetText(string newText)
{
	text = newText;
	textWrap.Wrap(text);
}
