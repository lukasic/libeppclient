#pragma once

#include <EPP/XML/XmlHelpers.h>
#include <EPP/Request.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
using namespace Poco::XML;

namespace EPP
{
	namespace Commands
	{
		class Logout: public EPP::Command
		{
		public:
			inline Logout()
			{}

			virtual inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
			{
				AutoPtr<Element> pElem = pDoc->createElement("logout");
				return pElem;
			}

		private:

		};
	}
}
