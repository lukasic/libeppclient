#pragma once

#include <EPP/XML/XmlHelpers.h>
#include <EPP/Command.h>

#include <EPP/Tools.h>

#include <Poco/SharedPtr.h>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/AutoPtr.h>
using namespace Poco::XML;

namespace EPP
{

class Request: public XmlSerializable
{
public:
	typedef Poco::SharedPtr<Request> Ptr;

	inline Request(Command::Ptr command)
	: XmlSerializable(), command(command)
	{
		clTRID = "IGNM-SK-" + gen_cltrid();
	}

	virtual inline ~Request() {}

	virtual AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const;

protected:
	inline Request()
	: XmlSerializable()
	{}

	inline std::string gen_cltrid() const
	{
		int length = 10;

		std::string out;
		out.resize(length);

		static const char alphanum[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

		for (unsigned i = 0; i < length; i++) {
			char c = alphanum[rand() % (sizeof(alphanum) - 1)];
			out[i] = c;
		}

		return out;
	}

private:
	Command::Ptr command;
	std::string clTRID;

};

}
