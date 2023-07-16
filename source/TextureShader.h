/* TextureShader.h
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

#ifndef TEXTURE_SHADER_H_
#define TEXTURE_SHADER_H_

#include "opengl.h"

class Point;



// Class holding a function to fill a rectangular region of the screen with a
// given color. This can be used with translucent colors to darken or lighten a
// part of the screen, or with additive colors (alpha = 0) as well.
class TextureShader {
public:
	static void Init();
	//static void Texture(const Point &bottomLeft, const Point &size, GLuint textureName);
	static void Draw(const GLuint textureName, const Point &position, const Point &size);
};



#endif
