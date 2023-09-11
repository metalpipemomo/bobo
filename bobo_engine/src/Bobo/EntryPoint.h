#pragma once

#ifdef BOBO_PLATFORM_WINDOWS

extern Bobo::Application* Bobo::CreateApplication();

int main(int argc, char** argv)
{
	Bobo::Log::Init();
	BOBO_CORE_INFO("Logger initialized...");

	auto app = Bobo::CreateApplication();
	app->Run();
	delete app;
}
#else
	#error You can only peer at Bobo through a window(s operating system)!
#endif