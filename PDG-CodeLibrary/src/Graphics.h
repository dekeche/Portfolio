#pragma once
#include <Generator.h>

class GraphicInterface
{
public:
	GraphicInterface();

	void Setup();
	void run();
	void Debug();
private:
	Generator mGenerator;
};