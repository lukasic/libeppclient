#include <EPP/CheckObjectResponse.h>

#include <iostream>

#include <map>
#include <string>

#include <EPP/XML/XmlParserWrapper.h>
#include <EPP/Response.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Element.h>
using namespace Poco::XML;

using EPP::CheckObjectResponse;


CheckObjectResponse CheckObjectResponse::parseOne(XmlParserWrapper& parser, const std::string& objectType, const std::string& objectField)
{
	std::string name = parser.getInnerTextByPath(objectType + ":" + objectField);
	bool avail = parser.getAttributeByPath(objectType + ":" + objectField, "avail") == "1";

	std::string reason = "";
	if (avail == false)
		reason = parser.getInnerTextByPath(objectType + ":reason");

	return CheckObjectResponse(name, avail, reason);
}

std::map<std::string, CheckObjectResponse> CheckObjectResponse::parseAll(XmlParserWrapper& parser, const std::string& objectType, const std::string& objectField)
{
	std::map<std::string, CheckObjectResponse> data;

	auto elements = parser.getElementsByPath(objectType + ":chkData/" + objectType + ":cd");

	for (int i = 0; i < elements.size(); i++) {
		XmlParserWrapper subparser = parser.subparser(elements[i]);
		CheckObjectResponse cor = CheckObjectResponse::parseOne(subparser, objectType, objectField);
		data.insert(std::pair<std::string, CheckObjectResponse>(cor.getName(), cor));
	}

	return data;
}


std::ostream& EPP::operator<< (std::ostream& os, const CheckObjectResponse& o)
{
	os << o.name << " -> ";

	if (o.isAvailable())
		return os << "available";

	os << "NOT available";
	if (o.reason.size())
		os << ": " << o.reason;

	return os;
}

