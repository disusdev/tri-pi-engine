#pragma once

#include "../defines.h"

#include "logger.h"

#include <string>

#include "rendering/vk/vkApp.h"

int main(int argc, char** argv);

struct AppCMDArgs
{
	int Count = 0;
	char** Args = nullptr;

	const char* operator[](int index) const
	{
		// ASSERT(index < Count);
		return Args[index];
	}
};

//struct AppSpec
//{
//	std::string Name = "Default App";
//	std::string WorkingDirectory;
//	AppCMDArgs CMDArgs;
//};

//class
//App : public CameraApp
//{
//public:
//	App(AppSpec spec);
//  void Run();
//
//	void draw3D() override;
//
//private:
//	//VulkanTexture envMap;
//	//VulkanTexture irrMap;
//
//	//VKSceneData sceneData;
//	//VKSceneData sceneData2;
//
//	//MultiRenderer multiRenderer;
//	//MultiRenderer multiRenderer2;
//	//GuiRenderer imgui;
//};

CameraApp* CreateApp(AppCMDArgs args);