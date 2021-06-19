#pragma once

#include <string>

namespace MAGE::Core::PlatformUtil
{
	class FileDialogs
	{
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}