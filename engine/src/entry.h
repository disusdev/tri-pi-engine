
#include "core/engine.h"
#include "core/logger.h"
#include "core/mem.h"

extern CameraApp* CreateApp();

int
main(int argc, char** argv)
{
	u64 mem;
	logger_init(&mem, nullptr);
	void* logger_state = mem_alloc(mem, MEM_TAG_APPLICATION);
	logger_init(&mem, logger_state);

	CameraApp* app = CreateApp({ argc, argv });

	app->mainLoop();

	delete app;
}