#include "TextureDrawable.h"

CLBBMLGraphicsTextureDrawable::CLBBMLGraphicsTextureDrawable(void):m_VAO(0), m_VBO(0)
{
	__initFBO();
}

CLBBMLGraphicsTextureDrawable::~CLBBMLGraphicsTextureDrawable(void)
{

}

//********************************************************
//Function:
void CLBBMLGraphicsTextureDrawable::createOBBV(CLBBMLGraphicsOBB* vOBB)
{
	if(vOBB == nullptr) vOBB = new CLBBMLGraphicsOBB;

	std::vector<glm::vec3> Vertices;
	Vertices.push_back(glm::vec3(-1,-1,0));
	Vertices.push_back(glm::vec3(-1,1,0));
	Vertices.push_back(glm::vec3(1,1,0));
	Vertices.push_back(glm::vec3(1,-1,0));
	vOBB->createV(Vertices);
}

//********************************************************
//Function:
void CLBBMLGraphicsTextureDrawable::drawV()
{
	if(m_VAO == 0 || m_VBO == 0) __initFBO();

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_QUADS, 0, 4);
	glBindVertexArray(0);
}

//********************************************************
//Function:
void CLBBMLGraphicsTextureDrawable::__initFBO()
{
	GLfloat TextureVertices[] = {
		-1.0f,  -1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TextureVertices), &TextureVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	_setBatches(1);
	_setTriangles(1);
}