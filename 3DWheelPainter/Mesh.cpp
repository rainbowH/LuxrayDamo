#include "Mesh.h"

#include <iostream>

#include <qopenglfunctions_3_3_core.h>

namespace OpenglObj {
	Mesh::Mesh(const std::vector<Vertex>& vertices,const std::vector<unsigned int>& indices, std::vector<Texture> textures)
		:vertices(vertices), indices(indices), textures(textures)
		//:VBO(QOpenGLBuffer::VertexBuffer),EBO(QOpenGLBuffer::IndexBuffer)//设定buffer类型
	{//需要放在上下文中！！

		//QOpenGLFunctions_3_3_Core::initializeOpenGLFunctions();//每次使用函数前都要初始化
		//this->textures = textures;
		//std::cout << "MESH:"<< this->vertices.size()<<" "<< this->indices.size() << std::endl;
		//初始化opengl函数
		this->SetupMesh();
	}
	void Mesh::Show()
	{
		std::cout << "Mesh:" << std::endl;
		for (auto &vertice : this->vertices)
		{
			std::cout <<"vertice position x:"<<vertice.Position.x <<" y:"<< vertice.Position.y<<" z:"<< vertice.Position.z << std::endl;
		}
		std::cout << std::endl;
	}
	Mesh::Mesh(const OpenglPointMsg& point_msg, std::vector<Texture> textures)
		:vertices(point_msg.GetVertices()), indices(point_msg.GetIndices()), textures(textures)
	{
		this->SetupMesh();
	}
	void Mesh::Draw(ShaderProgram &shaderProgram) const//mesh的draw对坐标系的转化不做处理
	{
		QOpenGLFunctions_3_3_Core glfun;   //不要继承，赋值会有问题
		glfun.initializeOpenGLFunctions();

		//unsigned int diffuseNr = 1;
		//unsigned int specularNr = 1;
		//unsigned int normalNr = 1;
		//unsigned int heightNr = 1;
		//for (unsigned int i = 0; i < textures.size(); i++)//绑定材质,默认材质已经加载完毕
		//{
		//	glfun.glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		//									  // retrieve texture number (the N in diffuse_textureN)
		//	std::string number;
		//	std::string name = textures[i].type;
		//	if (name == "texture_diffuse")
		//		number = std::to_string(diffuseNr++);
		//	else if (name == "texture_specular")
		//		number = std::to_string(specularNr++); // transfer unsigned int to stream
		//	else if (name == "texture_normal")
		//		number = std::to_string(normalNr++); // transfer unsigned int to stream
		//	else if (name == "texture_height")
		//		number = std::to_string(heightNr++); // transfer unsigned int to stream
		//	 
		//											 // now set the sampler to the correct texture unit
		//	shaderProgram.setInt(name + number, i);

		//	glBindTexture(GL_TEXTURE_2D, textures[i].id);
		//}
		if (textures.size() > 0)
		{
			glfun.glActiveTexture(GL_TEXTURE0);
			glfun.glBindTexture(GL_TEXTURE_2D, textures[0].id);
		}

		//std::cout << "Draw ok1" << std::endl;
		// draw mesh
		glfun.glBindVertexArray(VAO);
		glfun.glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glfun.glBindVertexArray(0);

		//std::cout << "Draw ok2" << std::endl;

	}
	void Mesh::SetupMesh()
	{
		QOpenGLFunctions_3_3_Core glfun;
		glfun.initializeOpenGLFunctions();

		//this->VAO.create();
		//this->VBO.create();
		//this->EBO.create();
		//
		//this->VAO.bind();
		//
		//this->VBO.bind();
		//this->VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
		//this->VBO.allocate(&vertices[0], vertices.size() * sizeof(Vertex));//绑定数据glBufferData
		//
		//this->EBO.bind();
		//this->EBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
		//this->EBO.allocate(&indices[0], indices.size() * sizeof(unsigned int));

		glfun.glGenVertexArrays(1, &VAO);
		glfun.glGenBuffers(1, &VBO);
		glfun.glGenBuffers(1, &EBO);

		//std::cout << "VAO:" << VAO << " VBO:" << VBO << " EBO:" << EBO << std::endl;

		glfun.glBindVertexArray(VAO);
		glfun.glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glfun.glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glfun.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glfun.glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
			&indices[0], GL_STATIC_DRAW);


		// 顶点位置
		glfun.glEnableVertexAttribArray(0);
		glfun.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// 顶点法线
		glfun.glEnableVertexAttribArray(1);
		glfun.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// 顶点纹理坐标
		glfun.glEnableVertexAttribArray(2);
		glfun.glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		glfun.glBindVertexArray(0);

	}

	Mesh::~Mesh()
	{
	}
}