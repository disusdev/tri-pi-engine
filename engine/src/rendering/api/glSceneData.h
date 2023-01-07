#pragma once

#include "scene/scene.h"
#include "scene/material.h"
#include "scene/vtxData.h"
#include "glShader.h"
#include "glTexture.h"

class GLSceneData
{
public:
	GLSceneData(
		const char* meshFile,
		const char* sceneFile,
		const char* materialFile);

	std::vector<GLTexture> allMaterialTextures_;

	MeshFileHeader header_;
	MeshData meshData_;

	Scene scene_;
	std::vector<MaterialDescription> materials_;
	std::vector<DrawData> shapes_;

	void loadScene(const char* sceneFile);
};
