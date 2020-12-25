#pragma once

#include <string>
#include <iostream>
#include <sstream>

namespace EPP
{
	struct Exception
	{
		int code;
		std::string msg;

		inline Exception(const std::string& msg) : code(-1), msg(msg) {}

		inline Exception(int code, const std::string& msg) : code(code), msg(msg) {}

		inline friend std::ostream& operator<< (std::ostream& os, const Exception& e)
		{
			return os << e.msg << std::endl;
		}
	};

}
