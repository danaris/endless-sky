/* Pane.h
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

#ifndef PANE_H_
#define PANE_H_

#include "Panel.h"

class Point;
class Rectangle;

// Class representing an individual interface piece with its own bounding rect
class Pane : public Panel {
public:

	Point GetTopLeft();
	virtual Point GetSize();
	void SetTopLeft(Point newTopLeft);
	void SetSize(Point newSize);
	virtual std::string PanelType() const override;
	Rectangle Bounds();
	virtual void Render() = 0;
protected:
	Point topLeft;
	Point size;
};

#endif
