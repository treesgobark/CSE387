#include "FigureTwo.h"

FigureTwo::~FigureTwo()
{
	glDeleteBuffers(1, &VBO);

} // end destructor

struct pctVertexData
{
	glm::vec4 m_pos;
	glm::vec4 m_color;
	glm::vec2 m_textCord;

	pctVertexData() {}

	pctVertexData(glm::vec4 pos, glm::vec4 col, glm::vec2 text)
	{
		m_pos = pos;
		m_color = col;
		m_textCord = text;

	}
};

void FigureTwo::initialize(GLuint shaderProgram)
{
	this->shaderProgram = shaderProgram;
	glUseProgram(this->shaderProgram);

	//	cout << "fig one initialize" << endl;

	std::vector<pctVertexData> pct;

	pct.push_back(pctVertexData(vec4(-0.25, -0.25, -0.25, 1.0), vec4(0.9, 0.1, 0.1, 1.0), vec2(0.0f, 0.0f)));
	pct.push_back(pctVertexData(vec4(-0.25, 0.25, -0.25, 1.0), vec4(0.9, 0.1, 0.1, 1.0), vec2(0.0f, 1.0f)));
	pct.push_back(pctVertexData(vec4(0.25, -0.25, -0.25, 1.0), vec4(0.9, 0.1, 0.1, 1.0), vec2(1.0f, 0.0f)));
	pct.push_back(pctVertexData(vec4(-0.25, 0.25, -0.25, 1.0), vec4(0.9, 0.1, 0.1, 1.0), vec2(0.0f, 1.0f)));
	pct.push_back(pctVertexData(vec4(0.25, -0.25, -0.25, 1.0), vec4(0.9, 0.1, 0.1, 1.0), vec2(1.0f, 0.0f)));
	pct.push_back(pctVertexData(vec4(0.25, 0.25, -0.25, 1.0), vec4(0.9, 0.1, 0.1, 1.0), vec2(1.0f, 1.0f)));

	pct.push_back(pctVertexData(vec4(0.25, 0.25, -0.25, 1.0), vec4(0.9, 0.5, 0.1, 1.0), vec2(0.0f, 0.0f)));
	pct.push_back(pctVertexData(vec4(0.25, -0.25, -0.25, 1.0), vec4(0.9, 0.5, 0.1, 1.0), vec2(0.0f, 1.0f)));
	pct.push_back(pctVertexData(vec4(0.25, 0.25, 0.25, 1.0), vec4(0.9, 0.5, 0.1, 1.0), vec2(1.0f, 0.0f)));
	pct.push_back(pctVertexData(vec4(0.25, -0.25, -0.25, 1.0), vec4(0.9, 0.5, 0.1, 1.0), vec2(0.0f, 1.0f)));
	pct.push_back(pctVertexData(vec4(0.25, 0.25, 0.25, 1.0), vec4(0.9, 0.5, 0.1, 1.0), vec2(1.0f, 0.0f)));
	pct.push_back(pctVertexData(vec4(0.25, -0.25, 0.25, 1.0), vec4(0.9, 0.5, 0.1, 1.0), vec2(1.0f, 1.0f)));

	pct.push_back(pctVertexData(vec4(-0.25, -0.25, 0.25, 1.0), vec4(0.9, 0.9, 0.1, 1.0), vec2(0.0f, 0.0f)));
	pct.push_back(pctVertexData(vec4(-0.25, 0.25, 0.25, 1.0), vec4(0.9, 0.9, 0.1, 1.0), vec2(0.0f, 1.0f)));
	pct.push_back(pctVertexData(vec4(0.25, -0.25, 0.25, 1.0), vec4(0.9, 0.9, 0.1, 1.0), vec2(1.0f, 0.0f)));
	pct.push_back(pctVertexData(vec4(-0.25, 0.25, 0.25, 1.0), vec4(0.9, 0.9, 0.1, 1.0), vec2(0.0f, 1.0f)));
	pct.push_back(pctVertexData(vec4(0.25, -0.25, 0.25, 1.0), vec4(0.9, 0.9, 0.1, 1.0), vec2(1.0f, 0.0f)));
	pct.push_back(pctVertexData(vec4(0.25, 0.25, 0.25, 1.0), vec4(0.9, 0.9, 0.1, 1.0), vec2(1.0f, 1.0f)));

	pct.push_back(pctVertexData(vec4(-0.25, 0.25, -0.25, 1.0), vec4(0.1, 0.9, 0.1, 1.0), vec2(0.0f, 0.0f)));
	pct.push_back(pctVertexData(vec4(-0.25, -0.25, -0.25, 1.0), vec4(0.1, 0.9, 0.1, 1.0), vec2(0.0f, 1.0f)));
	pct.push_back(pctVertexData(vec4(-0.25, 0.25, 0.25, 1.0), vec4(0.1, 0.9, 0.1, 1.0), vec2(1.0f, 0.0f)));
	pct.push_back(pctVertexData(vec4(-0.25, -0.25, -0.25, 1.0), vec4(0.1, 0.9, 0.1, 1.0), vec2(0.0f, 1.0f)));
	pct.push_back(pctVertexData(vec4(-0.25, 0.25, 0.25, 1.0), vec4(0.1, 0.9, 0.1, 1.0), vec2(1.0f, 0.0f)));
	pct.push_back(pctVertexData(vec4(-0.25, -0.25, 0.25, 1.0), vec4(0.1, 0.9, 0.1, 1.0), vec2(1.0f, 1.0f)));

	pct.push_back(pctVertexData(vec4(-0.25, 0.25, -0.25, 1.0), vec4(0.1, 0.1, 0.9, 1.0), vec2(0.0f, 0.0f)));
	pct.push_back(pctVertexData(vec4(-0.25, 0.25, 0.25, 1.0), vec4(0.1, 0.1, 0.9, 1.0), vec2(0.0f, 2.0f)));
	pct.push_back(pctVertexData(vec4(0.25, 0.25, -0.25, 1.0), vec4(0.1, 0.1, 0.9, 1.0), vec2(2.0f, 0.0f)));
	pct.push_back(pctVertexData(vec4(-0.25, 0.25, 0.25, 1.0), vec4(0.1, 0.1, 0.9, 1.0), vec2(0.0f, 2.0f)));
	pct.push_back(pctVertexData(vec4(0.25, 0.25, -0.25, 1.0), vec4(0.1, 0.1, 0.9, 1.0), vec2(2.0f, 0.0f)));
	pct.push_back(pctVertexData(vec4(0.25, 0.25, 0.25, 1.0), vec4(0.1, 0.1, 0.9, 1.0), vec2(2.0f, 2.0f)));

	pct.push_back(pctVertexData(vec4(-0.25, -0.25, -0.25, 1.0), vec4(0.5, 0.1, 0.9, 1.0), vec2(0.0f, 0.0f)));
	pct.push_back(pctVertexData(vec4(-0.25, -0.25, 0.25, 1.0), vec4(0.5, 0.1, 0.9, 1.0), vec2(0.0f, 1.0f)));
	pct.push_back(pctVertexData(vec4(0.25, -0.25, -0.25, 1.0), vec4(0.5, 0.1, 0.9, 1.0), vec2(2.0f, 0.0f)));
	pct.push_back(pctVertexData(vec4(-0.25, -0.25, 0.25, 1.0), vec4(0.5, 0.1, 0.9, 1.0), vec2(0.0f, 1.0f)));
	pct.push_back(pctVertexData(vec4(0.25, -0.25, -0.25, 1.0), vec4(0.5, 0.1, 0.9, 1.0), vec2(2.0f, 0.0f)));
	pct.push_back(pctVertexData(vec4(0.25, -0.25, 0.25, 1.0), vec4(0.5, 0.1, 0.9, 1.0), vec2(2.0f, 1.0f)));

	// Generate vertex array object and bind it for the first time
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Load positions and colors into the same buffer
	glBufferData(GL_ARRAY_BUFFER, pct.size() * sizeof(pctVertexData), &pct.front(), GL_STATIC_DRAW);

	// Specify the location and data format of an array of vertex positions
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(pctVertexData), 0);
	glEnableVertexAttribArray(0);

	// Specify the location and data format of an array of vertex colors
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(pctVertexData), (const void*)sizeof(vec4));
	glEnableVertexAttribArray(1);

	// Specify the location and data format of an array of texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(pctVertexData), (const void*)(2 * sizeof(vec4)));
	glEnableVertexAttribArray(2);

	renderMode = ORDERED;

	// Set the number of vertices to be processed in the vertex fetch
	count = 36;

	modelLocation = 2;

}