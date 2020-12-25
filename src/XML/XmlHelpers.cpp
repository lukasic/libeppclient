#include <EPP/XML/XmlHelpers.h>

#include <string>
#include <sstream>

#include <Epp/Exception.h>

#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/Text.h>
#include <Poco/DOM/Node.h>
#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/DOMWriter.h>
#include <Poco/XML/XMLWriter.h>
#include <Poco/SharedPtr.h>
#include <Poco/Format.h>

using Poco::XML::AutoPtr;
using Poco::XML::Document;
using Poco::XML::Element;
using Poco::XML::Text;
using Poco::XML::Node;
using Poco::XML::XMLWriter;
using Poco::XML::DOMWriter;
using Poco::format;


const std::string XmlToString(AutoPtr<Node> pDoc)
{
	std::ostringstream oss;
	DOMWriter writer;
	writer.setNewLine("\n");
	writer.setIndent("  ");
	writer.setOptions(XMLWriter::WRITE_XML_DECLARATION | XMLWriter::PRETTY_PRINT);
	writer.writeNode(oss, pDoc);
	return oss.str();
}

AutoPtr<Element> CreateTextElement(AutoPtr<Document> pDoc, const std::string& name, const std::string& value)
{
	AutoPtr<Element> child = pDoc->createElement(name);
	AutoPtr<Text> text = pDoc->createTextNode(value);
	child->appendChild(text);
	return child;
}

AutoPtr<Document> XmlSerializable::toXMLDocument() const
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element> pElem = this->toXML(pDoc);
	pDoc->appendChild(pElem);
	return pDoc;
}

AutoPtr<Element> AddElement(AutoPtr<Document> doc, AutoPtr<Element> elem, const std::string& name)
{
	AutoPtr<Element> newelem = doc->createElement(name);
	elem->appendChild(newelem);
	return newelem;
}

AutoPtr<Element> AddElement(AutoPtr<Document> doc, AutoPtr<Element> elem, const std::string& name, const std::string& value)
{
	AutoPtr<Element> newelem = CreateTextElement(doc, name, value);
	elem->appendChild(newelem);
	return newelem;
}

AutoPtr<Element> AddElement(AutoPtr<Document> doc, AutoPtr<Element> elem, const std::string& name, int value)
{
	return AddElement(doc, elem, name, format("%d", value));
}

void AddElement(AutoPtr<Document> doc, AutoPtr<Element> elem, AutoPtr<Element> addelem)
{
	elem->appendChild(addelem);
}

