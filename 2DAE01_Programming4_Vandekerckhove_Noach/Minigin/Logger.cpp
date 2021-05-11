#include "MiniginPCH.h"
#include "Logger.h"
using namespace dae;

Logger::Logger()
	: m_Handle(GetStdHandle(STD_OUTPUT_HANDLE))
{
}

void Logger::Log(LogType type, const std::string& message)
{
	switch (type)
	{
	case LogType::Info:
		std::cout << "LOG [INFO] : " << message << "\n";
		break;
	case LogType::Warning:
		SetConsoleTextAttribute(m_Handle, FOREGROUND_RED | FOREGROUND_GREEN);
		std::cout << "LOG [WARNING] : " << message << "\n";
		break;
	case LogType::Error:
		SetConsoleTextAttribute(m_Handle, FOREGROUND_RED);
		std::cout << "LOG [ERROR] : " << message <<"\n";
		break;
	}

	SetConsoleTextAttribute(m_Handle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
