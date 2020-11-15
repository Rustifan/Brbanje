#pragma once

#include <string>
#include <optional>

namespace Brbanje
{
	class FileDialog
	{
	public:

		static std::optional<std::string> OpenFile(const char* filter);
		static std::optional<std::string> SaveFile(const char* filter);
		static bool YesNoWindow(const char* caption, const char* message);
		
	};

}