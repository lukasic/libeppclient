#pragma once

#include <iostream>

#include <map>
#include <string>

#include <EPP/XML/XmlParserWrapper.h>
#include <EPP/Response.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Element.h>
using namespace Poco::XML;

namespace EPP
{
	class CheckObjectResponse
	{
	public:
		typedef std::map<std::string, CheckObjectResponse> Map;

		inline CheckObjectResponse()
		{}

		inline CheckObjectResponse(const std::string& name, bool isAvail, const std::string& reason = "")
		: name(name), isAvail(isAvail), reason(reason)
		{}

		inline const std::string& getName() const {
			return name;
		}

		inline bool isAvailable() const {
			return isAvail;
		}

		inline const std::string& getReason() const {
			return reason;
		}

		static CheckObjectResponse parseOne(XmlParserWrapper& parser, const std::string& objectType, const std::string& objectField = "name");
		static std::map<std::string, CheckObjectResponse> parseAll(XmlParserWrapper& parser, const std::string& objectType, const std::string& objectField = "name");

		friend std::ostream& operator<< (std::ostream& os, const CheckObjectResponse& o);

	private:
		const std::string name;
		bool isAvail;
		const std::string reason;
	};
}
