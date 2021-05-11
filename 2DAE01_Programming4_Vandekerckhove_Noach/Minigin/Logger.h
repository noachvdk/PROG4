#pragma once
#include "Singleton.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace dae
{
	enum class LogType
	{
		Info,
		Warning,
		Error
	};

	
	class Logger final : public Singleton<Logger>
	{
	public:
		friend class Singleton<Logger>;
		Logger();
		~Logger() = default;

		Logger(const Logger& other) = delete;
		Logger(Logger&& other) noexcept = delete;
		Logger& operator=(const Logger& other) = delete;
		Logger& operator=(Logger&& other) noexcept = delete;

		void Log(LogType type, const std::string& message);
	private:
		//used to change console output color
		HANDLE m_Handle; //https://stackoverflow.com/questions/17125440/c-win32-console-color
	};
}


