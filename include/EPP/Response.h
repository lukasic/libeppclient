#pragma once

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/DOMParser.h>
using namespace Poco::XML;

#include <EPP/XML/XmlParserWrapper.h>

namespace EPP
{

class Response
{
protected:
	XmlParserWrapper parser;

public:
	std::string xml;
	int code;

	inline Response(const std::string& xml, const std::string& basePath = "")
	: xml(xml), parser(xml, basePath)
	{
		checkResponse();
	}

	void checkResponse();

	virtual inline ~Response() {}
};

}

