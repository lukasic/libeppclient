#pragma once

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/DOMParser.h>
using namespace Poco::XML;

#include <EPP/XML/XmlParserWrapper.h>

namespace EPP
{

	class Greeting
	{
	protected:
		XmlParserWrapper parser;

	public:
		std::string xml;

		inline Greeting(const std::string& xml, const std::string& basePath = "")
			: xml(xml), parser(xml, basePath)
		{
		}

		virtual inline ~Greeting() {}
	};

}

