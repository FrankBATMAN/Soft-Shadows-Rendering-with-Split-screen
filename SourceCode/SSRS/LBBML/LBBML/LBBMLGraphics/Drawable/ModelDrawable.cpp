#include "ModelDrawable.h"
#include <iostream>
#include <Assimp/include/Importer.hpp>
#include <Assimp/include/postprocess.h>
#include <Assimp/include/scene.h>

CLBBMLGraphicsModelDrawable::CLBBMLGraphicsModelDrawable(const std::string& vFilePath):m_VAO(0), m_VBO(0), m_EBO(0)
{
	__LoadModelFromFile(vFilePath);
	__initFBO();
}

CLBBMLGraphicsModelDrawable::~CLBBMLGraphicsModelDrawable(void)
{
	
}

//*************************************************************************
//Function:
void CLBBMLGraphicsModelDrawable::createOBBV(CLBBMLGraphicsOBB* vOBB)
{
	if(vOBB == nullptr) vOBB = new CLBBMLGraphicsOBB;

	std::vector<glm::vec3> ModelVertices;
	for(auto Itr : m_Vertices)
		ModelVertices.push_back(Itr.Position);
	vOBB->createV(ModelVertices);
}

//************************************************************************
//Function:
void CLBBMLGraphicsModelDrawable::drawV()
{
	if(m_VAO == 0 || m_VBO == 0 || m_EBO == 0)
		__initFBO();

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

//************************************************************************
//Function:
void CLBBMLGraphicsModelDrawable::__LoadModelFromFile(const std::string& vFilePath)
{
	_ASSERT(!vFilePath.empty(), "empty file path");

	int Triangles = 0;
	Assimp::Importer ModelImporter;
	const aiScene* pScene = ModelImporter.ReadFile(vFilePath.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	if (pScene)
	{
		for (unsigned int i = 0; i < pScene->mNumMeshes; ++i)
		{
			for (unsigned int k = 0; k < pScene->mMeshes[i]->mNumVertices; ++k)
			{
				SVertex Vertex;
				Vertex.Position = glm::vec3(pScene->mMeshes[i]->mVertices[k].x, pScene->mMeshes[i]->mVertices[k].y, pScene->mMeshes[i]->mVertices[k].z);
				Vertex.Normal = glm::vec3(pScene->mMeshes[i]->mNormals[k].x, pScene->mMeshes[i]->mNormals[k].y, pScene->mMeshes[i]->mNormals[k].z);
				//TO DO: handle empty TexCoords
				Vertex.TexCoords = glm::vec2(0,0);
				m_Vertices.push_back(Vertex);
			}

			Triangles += pScene->mMeshes[i]->mNumFaces;

			for (unsigned int p = 0; p < pScene->mMeshes[i]->mNumFaces; ++p)
			{
				for (unsigned int n = 0; n < pScene->mMeshes[i]->mFaces[p].mNumIndices; ++n)
					m_Indices.push_back(pScene->mMeshes[i]->mFaces[p].mIndices[n]);
			}
		}
#ifdef _DEBUG
		std::cout << "Model Loaded from file : " << vFilePath << std::endl;
#endif
	}
	else
	{
		_ASSERT(false, ModelImporter.GetErrorString());
	}

	_setTriangles(Triangles);
}

//************************************************************************
//Function:
void CLBBMLGraphicsModelDrawable::__initFBO()
{
	_setBatches(1);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(SVertex), &m_Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(GLuint), &m_Indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (GLvoid*)offsetof(SVertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex), (GLvoid*)offsetof(SVertex, TexCoords));

	glBindVertexArray(0);
}