#pragma once

#ifdef BR_PLATFORM_WINDOWS

extern Brbanje::Application* Brbanje::CreateApplication();

int main(char argc, char** argv)
{
	Brbanje::Log::Init();
	
	BR_PROFILE_BEGIN_SESSION("Startup", "Startup-profiling.json");
	auto app = Brbanje::CreateApplication();
	BR_PROFILE_END_SESSION;
	
	BR_PROFILE_BEGIN_SESSION("RunTime", "Runtime-profiling.json");
	app->Run();
	BR_PROFILE_END_SESSION;

	BR_PROFILE_BEGIN_SESSION("Shutdown", "Shutdown-profiling.json");
	delete app;
	BR_PROFILE_END_SESSION;

	return 0;
}




#endif