#include <EPP/Request.h>

#include <EPP/XML/XmlHelpers.h>
#include <EPP/Command.h>

#include <Poco/DOM/Element.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/AutoPtr.h>

using Poco::XML::AutoPtr;
using Poco::XML::Document;
using Poco::XML::Element;

using EPP::Request;

AutoPtr<Element> Request::toXML(AutoPtr<Document> pDoc) const
{
	AutoPtr<Element> epp = pDoc->createElement("epp");
	epp->setAttribute("xmlns", "urn:ietf:params:xml:ns:epp-1.0");
	epp->setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	epp->setAttribute("xsi:schemaLocation", "urn:ietf:params:xml:ns:epp-1.0 epp-1.0.xsd");

	AutoPtr<Element> command = pDoc->createElement("command");

	command->appendChild(
		this->command->toXML(pDoc));

	auto extensions = this->command->getExtensions();
	if (extensions.size() > 0)
	{
		AutoPtr<Element> extension = AddElement(pDoc, command, "extension");

		for (auto i = extensions.begin(); i != extensions.end(); ++i)
			AddElement(pDoc, extension, (*i)->toXML(pDoc));
	}

	command->appendChild(CreateTextElement(pDoc,
		"clTRID", clTRID));

	epp->appendChild(command);
	return epp;
}

