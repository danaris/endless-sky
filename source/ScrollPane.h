/* ScrollPane.h
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

#ifndef SCROLL_PANE_H_
#define SCROLL_PANE_H_

#include "Pane.h"

#include <string>
#include <vector>

class Point;

// Class wrapping a Panel's drawing within a scrollable clipped view
class ScrollPane : public Pane {
public:
	ScrollPane(Point topLeft, Point size, Pane *child);

	virtual void Draw() override;


protected:
	// Only override the ones you need; the default action is to return false.
	virtual bool Drag(double dx, double dy) override;
	virtual bool Scroll(double dx, double dy) override;

	bool DoScroll(double dy);

private:
	long long int scroll = 0;
	
	Pane *child;
	Point childOrigTopLeft;
};



#endif
