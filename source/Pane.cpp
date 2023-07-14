/* Pane.cpp
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

#include "Pane.h"

#include "Rectangle.h"
#include "Point.h"

using namespace std;



Point Pane::GetTopLeft()
{
	return topLeft;
}

Point Pane::GetSize()
{
	return size;
}

void Pane::SetTopLeft(Point newTopLeft)
{
	topLeft = newTopLeft;
}

void Pane::SetSize(Point newSize)
{
	size = newSize;
}


string Pane::PanelType() const
{
	return "Pane";
}

Rectangle Pane::Bounds()
{
	return Rectangle(Point(topLeft.X() + size.X() / 2, topLeft.Y() + size.Y() / 2), size);
}
