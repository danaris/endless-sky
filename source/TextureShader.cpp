/* TextureShader.cpp
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

#include "TextureShader.h"

#include "Logger.h"
#include "Point.h"
#include "Screen.h"
#include "Shader.h"
#include "UI.h"

#include <sstream>
#include <vector>

using namespace std;

namespace {
	Shader shader;
	GLint scaleI;
	GLint positionI;

	GLuint vao;
	GLuint vbo;
}

// Initialize the shaders.
void TextureShader::Init()
{

	static const char *vertexCode =
		"// vertex sprite shader\n"
		"precision mediump float;\n"
		"uniform vec2 scale;\n"
		"uniform vec2 position;\n"

		"in vec2 vert;\n"
		"out vec2 fragTexCoord;\n"

		"void main() {\n"
		"  gl_Position = vec4((vert + position) * scale, 0, 1);\n"
		"  vec2 texCoord = vert + vec2(.5, .5);\n"
		"  fragTexCoord = vec2(texCoord.x, texCoord.y);\n"
		"}\n";

	ostringstream fragmentCodeStream;
	fragmentCodeStream <<
		"// fragment sprite shader\n"
		"precision mediump float;\n"
		"uniform sampler2D tex;\n"
		"const int range = 5;\n"

		"in vec2 fragTexCoord;\n"

		"out vec4 finalColor;\n"

		"void main() {\n"
		"  finalColor = texture(tex, fragTexCoord);\n"
		"}\n";

	static const string fragmentCodeString = fragmentCodeStream.str();
	static const char *fragmentCode = fragmentCodeString.c_str();

	shader = Shader(vertexCode, fragmentCode);
	scaleI = shader.Uniform("scale");
	positionI = shader.Uniform("position");

	glUseProgram(shader.Object());
	glUniform1i(shader.Uniform("tex"), 0);
	glUseProgram(0);

	// Generate the vertex data for drawing sprites.
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLfloat vertexData[] = {
		-.5f, -.5f,
		-.5f,  .5f,
		 .5f, -.5f,
		 .5f,  .5f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(shader.Attrib("vert"));
	glVertexAttribPointer(shader.Attrib("vert"), 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);

	// unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	UI::HandleGLError("TSInit", __FILE__, to_string(__LINE__));
}



void TextureShader::Draw(const GLuint textureName, const Point &position)
{
	glUseProgram(shader.Object());
	UI::HandleGLError("TS-A", __FILE__, to_string(__LINE__));
	glBindVertexArray(vao);
	UI::HandleGLError("TS-B", __FILE__, to_string(__LINE__));

	GLfloat scale[2] = {2.f / Screen::Width(), -2.f / Screen::Height()};
	glUniform2fv(scaleI, 1, scale);
	UI::HandleGLError("TS-C", __FILE__, to_string(__LINE__));
	
	glBindTexture(GL_TEXTURE_2D, textureName);
	UI::HandleGLError("TS-D", __FILE__, to_string(__LINE__));

	Point glPosition = Screen::GLPoint(position);
	GLfloat positionV[2] = {static_cast<float>(glPosition.X()), static_cast<float>(glPosition.Y())};
	glUniform2fv(positionI, 1, positionV);
	UI::HandleGLError("TS-E", __FILE__, to_string(__LINE__));

	//glTexParameteriv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_RGBA, SWIZZLE[swizzle].data());

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	UI::HandleGLError("TS-F", __FILE__, to_string(__LINE__));

	glBindVertexArray(0);
	glUseProgram(0);
	UI::HandleGLError("TS-G", __FILE__, to_string(__LINE__));
}


