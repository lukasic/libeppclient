#include <EPP/XML/XmlParserWrapper.h>

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


XmlParserWrapper::XmlParserWrapper(const std::string xml, const std::string& basePath)
	: basePath(basePath)
{
	parser = SharedPtr<DOMParser>(new DOMParser());
	parser->setFeature(XMLReader::FEATURE_NAMESPACES, false);

	std::cerr << xml << std::endl;

	doc = parser->parseString(xml);

	Element* r_ptr = (Element*)doc->firstChild();
	root = AutoPtr<Element>(r_ptr, true);

	std::cout << XmlToString(root) << std::endl;
}

AutoPtr<Element> XmlParserWrapper::getElementByPath(const std::string& path, bool useBasePath) const
{
	std::string _path = useBasePath ? basePath + "/" + path : path;
	std::cout << "Getting node " << _path;
	Element* e = (Element*)root->getNodeByPath(_path);
	std::cout << " -> " << e;
	if (e != nullptr)
		std::cout << " // " << e->innerText();
	std::cout << std::endl;
	return AutoPtr<Element>(e, true);
}

std::vector<AutoPtr<Element>> XmlParserWrapper::getElementsByPath(const std::string& path, bool useBasePath) const
{
	std::string _path = useBasePath ? basePath + "/" + path : path;
	std::vector<AutoPtr<Element>> data;

	Node* node = root->getNodeByPath(_path + "[0]");

	for (; ; node = node->nextSibling())
	{
		if (node == nullptr)
			break;

		if (ends_with(_path, node->nodeName()) == false)
			break;

		Element* e = (Element*)node;

		AutoPtr<Element> childElement(e, true);
		data.push_back(childElement);
	}

	return data;
};

std::vector<AutoPtr<Element>> XmlParserWrapper::getChildElements() const
{
	std::vector<AutoPtr<Element>> data;

	Node* node = root->firstChild();

	for (; ; node = node->nextSibling())
	{
		if (node == nullptr)
			break;

		Element* e = (Element*)node;

		AutoPtr<Element> childElement(e, true);
		data.push_back(childElement);
		
		if (node == root->lastChild())
			break;
	}

	return data;
};

std::string XmlParserWrapper::getInnerTextByPath(const std::string& path, bool useBasePath) const
{
	AutoPtr<Element> e = getElementByPath(path, useBasePath);
	if (e.isNull())
		throw EPP::Exception("Element does not exists: " + path);

	return e->innerText();
}

std::string XmlParserWrapper::getInnerTextByPath(const std::string& path, const char * def, bool useBasePath) const
{
	AutoPtr<Element> e = getElementByPath(path, useBasePath);
	if (e.isNull())
		return def;

	return e->innerText();
}

XmlParserWrapper XmlParserWrapper::subparser(const std::string& path) const
{
	return XmlParserWrapper(parser, doc, getElementByPath(path));
}

XmlParserWrapper XmlParserWrapper::subparser(AutoPtr<Element> e) {
	return XmlParserWrapper(parser, doc, e);
}

XmlParserWrapper::XmlParserWrapper(SharedPtr<DOMParser> p, AutoPtr<Document> d, AutoPtr<Element> e, const std::string& basePath)
	: basePath(basePath)
{
	doc = d;
	parser = p;
	root = e;
	std::cout << XmlToString(root) << std::endl;
}

