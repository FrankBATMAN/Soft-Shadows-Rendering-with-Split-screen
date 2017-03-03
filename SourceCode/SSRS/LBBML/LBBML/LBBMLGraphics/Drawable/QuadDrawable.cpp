#include "QuadDrawable.h"

CLBBMLGraphicsQuadDrawable::CLBBMLGraphicsQuadDrawable(void):m_VAO(0), m_VBO(0)
{
	__initFBO();
}

CLBBMLGraphicsQuadDrawable::~CLBBMLGraphicsQuadDrawable(void)
{

}

//*************************************************************
//Function:
void CLBBMLGraphicsQuadDrawable::createOBBV(CLBBMLGraphicsOBB* vOBB)
{
	if(vOBB == nullptr) vOBB = new CLBBMLGraphicsOBB;

	std::vector<glm::vec3> Vertices;
	Vertices.push_back(glm::vec3(-1,0,-1));
	Vertices.push_back(glm::vec3(-1,0,1));
	Vertices.push_back(glm::vec3(1,0,1));
	Vertices.push_back(glm::vec3(1,0,-1));
	vOBB->createV(Vertices);
}

//*************************************************************
//Function:
void CLBBMLGraphicsQuadDrawable::drawV()
{
	if(m_VAO == 0 || m_VBO == 0) __initFBO();

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_QUADS, 0, 4);
	glBindVertexArray(0);
}

//*************************************************************
//Function:
void CLBBMLGraphicsQuadDrawable::__initFBO()
{
	GLfloat QuadVertices[] = {
		-1.0f,  0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
	};
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), &QuadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

	_setBatches(1);
	_setTriangles(1);
}