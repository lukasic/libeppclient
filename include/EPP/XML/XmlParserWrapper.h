#pragma once

#include <iostream>
#include <string>

#include <EPP/Tools.h>
#include <EPP/XML/XmlHelpers.h>
#include <EPP/Exception.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/DOMParser.h>
using namespace Poco::XML;

#include <Poco/SharedPtr.h>
using Poco::SharedPtr;

#include <Poco/NumberParser.h>
using Poco::NumberParser;

class XmlParserWrapper
{
public:
	XmlParserWrapper(const std::string xml, const std::string& basePath = "");
	AutoPtr<Element> getElementByPath(const std::string& path, bool useBasePath = true) const;

	inline AutoPtr<Element> getRootElement() const
	{
		return root;
	}

	std::vector<AutoPtr<Element>> getElementsByPath(const std::string& path, bool useBasePath = true) const;
	std::vector<AutoPtr<Element>> getChildElements() const;

	std::string getInnerTextByPath(const std::string& path, bool useBasePath = true) const;
	std::string getInnerTextByPath(const std::string& path, const char * def, bool useBasePath = true) const;

	template<typename T = std::string>
	inline std::vector<T> getInnerTextsByPath(const std::string& path, bool useBasePath = true) const
	{
		std::vector<AutoPtr<Element>> elems = getElementsByPath(path, useBasePath);
		std::vector<T> data;

		for (unsigned i = 0; i < elems.size(); i++)
			data.push_back(T(elems[i]->innerText()));

		return data;
	}

	template<typename T = std::string>
	inline T getAttributeByPath(const std::string& path, const std::string& attribute, bool useBasePath = true) const
	{
		AutoPtr<Element> e = getElementByPath(path, useBasePath);
		if (e.isNull())
			throw EPP::Exception("Element does not exists: " + path);

		std::string val = e->getAttribute(attribute);
		std::cout << "Gettings attribute" << path << " @ " << attribute << " -> " << val << std::endl;
		return T(val);
	}

	template<>
	inline int getAttributeByPath<int>(const std::string& path, const std::string& attribute, bool useBasePath) const
	{
		return Poco::NumberParser::parse(getAttributeByPath(path, attribute, useBasePath));
	}

	template<typename T = std::string>
	inline std::vector<T> getAttributesByPath(const std::string& path, const std::string& attribute, bool useBasePath = true) const
	{
		std::vector<AutoPtr<Element>> ev = getElementsByPath(path, useBasePath);
		std::vector<T> o;

		for each (auto e in ev)
		{
			std::string val = e->getAttribute(attribute);

			std::cout << "Gettings attribute" << path << " @ " << attribute << " -> " << val << std::endl;
			
			o.push_back(T(val));
		}

		return o;
	}

	virtual XmlParserWrapper subparser(const std::string& path) const;
	virtual XmlParserWrapper subparser(AutoPtr<Element> e);

	template<typename T>
	inline T parse(const std::string& path) const
	{
		return T(subparser(path));
	}


private:

	XmlParserWrapper(SharedPtr<DOMParser> p, AutoPtr<Document> d, AutoPtr<Element> e, const std::string& basePath = "");
	 
	std::string basePath;
	AutoPtr<Element> root;

	// Nasledujuce elementy su potrebne, pretoze objekty ulozene v AutoPtr
	// su objekty ziskane zo zivej instancie parseru a dokumentu.
	SharedPtr<DOMParser> parser;
	AutoPtr<Document> doc;
};
