#pragma once

#include "Object.h"
#include "Material.h"

class CubeMap : public Object
{
public:
	void Initialize();

	GLuint textureid;
	Texture tmptexture;
};

inline void CubeMap::Initialize()
{
	mesh = new Mesh();
	this->SetMesh(mesh);
	mesh->data.resize(36);
	mesh->index.resize(12);

	mesh->data[0].vertex = cy::Point3f(-10.0,  10.0, -10.0);
	mesh->data[1].vertex = cy::Point3f(-10.0, -10.0, -10.0);
	mesh->data[2].vertex = cy::Point3f( 10.0, -10.0, -10.0);
	mesh->data[3].vertex = cy::Point3f( 10.0, -10.0, -10.0);
	mesh->data[4].vertex = cy::Point3f( 10.0,  10.0, -10.0);
	mesh->data[5].vertex = cy::Point3f(-10.0,  10.0, -10.0);

	mesh->data[6].vertex = cy::Point3f(-10.0, -10.0,  10.0);
	mesh->data[7].vertex = cy::Point3f(-10.0, -10.0, -10.0);
	mesh->data[8].vertex = cy::Point3f(-10.0,  10.0, -10.0);
	mesh->data[9].vertex = cy::Point3f(-10.0,  10.0, -10.0);
	mesh->data[10].vertex = cy::Point3f(-10.0,  10.0, 10.0);
	mesh->data[11].vertex = cy::Point3f(-10.0, -10.0, 10.0);

	mesh->data[12].vertex = cy::Point3f(10.0, -10.0, -10.0);
	mesh->data[13].vertex = cy::Point3f(10.0, -10.0,  10.0);
	mesh->data[14].vertex = cy::Point3f(10.0,  10.0,  10.0);
	mesh->data[15].vertex = cy::Point3f(10.0,  10.0,  10.0);
	mesh->data[16].vertex = cy::Point3f(10.0,  10.0, -10.0);
	mesh->data[17].vertex = cy::Point3f(10.0, -10.0, -10.0);

	mesh->data[18].vertex = cy::Point3f(-10.0, -10.0, 10.0);
	mesh->data[19].vertex = cy::Point3f(-10.0,  10.0, 10.0);
	mesh->data[20].vertex = cy::Point3f( 10.0,  10.0, 10.0);
	mesh->data[21].vertex = cy::Point3f( 10.0,  10.0, 10.0);
	mesh->data[22].vertex = cy::Point3f( 10.0, -10.0, 10.0);
	mesh->data[23].vertex = cy::Point3f(-10.0, -10.0, 10.0);

	mesh->data[24].vertex = cy::Point3f(-10.0, 10.0, -10.0);
	mesh->data[25].vertex = cy::Point3f( 10.0, 10.0, -10.0);
	mesh->data[26].vertex = cy::Point3f( 10.0, 10.0,  10.0);
	mesh->data[27].vertex = cy::Point3f( 10.0, 10.0,  10.0);
	mesh->data[28].vertex = cy::Point3f(-10.0, 10.0,  10.0);
	mesh->data[29].vertex = cy::Point3f(-10.0, 10.0, -10.0);

	mesh->data[30].vertex = cy::Point3f(-10.0, -10.0, -10.0);
	mesh->data[31].vertex = cy::Point3f(-10.0, -10.0,  10.0);
	mesh->data[32].vertex = cy::Point3f( 10.0, -10.0, -10.0);
	mesh->data[33].vertex = cy::Point3f( 10.0, -10.0, -10.0);
	mesh->data[34].vertex = cy::Point3f(-10.0, -10.0,  10.0);
	mesh->data[35].vertex = cy::Point3f( 10.0, -10.0,  10.0);

	mesh->index[0].v[0] = 0;
	mesh->index[0].v[1] = 1;
	mesh->index[0].v[2] = 2;
	mesh->index[1].v[0] = 3;
	mesh->index[1].v[1] = 4;
	mesh->index[1].v[2] = 5;
	mesh->index[2].v[0] = 6;
	mesh->index[2].v[1] = 7;
	mesh->index[2].v[2] = 8;
	mesh->index[3].v[0] = 9;
	mesh->index[3].v[1] = 10;
	mesh->index[3].v[2] = 11;
	mesh->index[4].v[0] = 12;
	mesh->index[4].v[1] = 13;
	mesh->index[4].v[2] = 14;
	mesh->index[5].v[0] = 15;
	mesh->index[5].v[1] = 16;
	mesh->index[5].v[2] = 17;
	mesh->index[6].v[0] = 18;
	mesh->index[6].v[1] = 19;
	mesh->index[6].v[2] = 20;
	mesh->index[7].v[0] = 21;
	mesh->index[7].v[1] = 22;
	mesh->index[7].v[2] = 23;
	mesh->index[8].v[0] = 24;
	mesh->index[8].v[1] = 25;
	mesh->index[8].v[2] = 26;
	mesh->index[9].v[0] = 27;
	mesh->index[9].v[1] = 28;
	mesh->index[9].v[2] = 29;
	mesh->index[10].v[0] = 30;
	mesh->index[10].v[1] = 31;
	mesh->index[10].v[2] = 32;
	mesh->index[11].v[0] = 33;
	mesh->index[11].v[1] = 34;
	mesh->index[11].v[2] = 35;

	mesh->InitializeBuffer();

	////////////////////////////////////
	glActiveTexture(GL_TEXTURE0 + 5);
	glGenTextures(1, &textureid);

	int tmpwidth, tmpheight;

	std::vector<cy::Color24> data;
	tmptexture.Load("../Objfiles/cubemap_posx.png", data, tmpwidth, tmpheight);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureid);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());

	std::vector<cy::Color24> data2;
	tmptexture.Load("../Objfiles/cubemap_negx.png", data2, tmpwidth, tmpheight);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureid);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data2.data());

	std::vector<cy::Color24> data3;
	tmptexture.Load("../Objfiles/cubemap_posy.png", data3, tmpwidth, tmpheight);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureid);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data3.data());

	std::vector<cy::Color24> data4;
	tmptexture.Load("../Objfiles/cubemap_negy.png", data4, tmpwidth, tmpheight);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureid);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data4.data());

	std::vector<cy::Color24> data5;
	tmptexture.Load("../Objfiles/cubemap_posz.png", data5, tmpwidth, tmpheight);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureid);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data5.data());

	std::vector<cy::Color24> data6;
	tmptexture.Load("../Objfiles/cubemap_negz.png", data6, tmpwidth, tmpheight);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureid);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data6.data());

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	////////////////////////////////////
	mesh->material = new Material();
	mesh->material->Load("environmentmap.vert.glsl", "environmentmap.frag.glsl");

	tmptexture.uniformid = glGetUniformLocation(mesh->material->programid, "skybox");
	if (tmptexture.uniformid == -1)
	{
		std::cerr << "The texture variable doesn't exist in the shader file" << std::endl;
	}

}