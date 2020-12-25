#pragma once

#include <string>

#include <Epp/Exception.h>

#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/Node.h>
#include <Poco/DOM/AutoPtr.h>
#include <Poco/SharedPtr.h>

using Poco::XML::AutoPtr;
using Poco::XML::Document;
using Poco::XML::Element;
using Poco::XML::Node;

const std::string XmlToString(AutoPtr<Node> pDoc);
AutoPtr<Element> CreateTextElement(AutoPtr<Document> pDoc, const std::string& name, const std::string& value);

class XmlSerializable
{
public:
	typedef Poco::SharedPtr<XmlSerializable> Ptr;

	virtual AutoPtr<Document> toXMLDocument() const;

	virtual AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const = 0;

	virtual inline std::string toXMLString() const
	{
		AutoPtr<Document> doc = new Document;
		return XmlToString(toXML(doc));
	}

};


AutoPtr<Element> AddElement(AutoPtr<Document> doc, AutoPtr<Element> elem, const std::string& name);
AutoPtr<Element> AddElement(AutoPtr<Document> doc, AutoPtr<Element> elem, const std::string& name, const std::string& value);
AutoPtr<Element> AddElement(AutoPtr<Document> doc, AutoPtr<Element> elem, const std::string& name, int value);
void AddElement(AutoPtr<Document> doc, AutoPtr<Element> elem, AutoPtr<Element> addelem);

