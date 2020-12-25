#pragma once

#include <EPP/Request.h>
#include <EPP/XML/XmlHelpers.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
using namespace Poco::XML;

namespace EPP
{
	class Hello: public EPP::Request
	{
	public:
		inline Hello(): EPP::Request()
		{}
		
		virtual inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
		{
			AutoPtr<Element> epp = pDoc->createElement("epp");
			epp->setAttribute("xmlns", "urn:ietf:params:xml:ns:epp-1.0");

			AutoPtr<Element> hello = pDoc->createElement("hello");
			epp->appendChild(hello);

			return epp;
		}

	private:

	};
}
