#include <EPP/Response.h>

#include <EPP/XML/XmlParserWrapper.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/DOMParser.h>
using namespace Poco::XML;

#include <Poco/NumberParser.h>

using EPP::Response;

void Response::checkResponse()
{
	AutoPtr<Element> e = parser.getElementByPath("response/result", false);

	if (e.isNull())
		throw EPP::Exception("Cannot find element 'result' in response.");

	code = Poco::NumberParser::parse(
		e->getAttribute("code"));

	if (code == 1000) return; // success
	if (code == 1001) return; // success, action pending (transfer op=req)
	if (code == 1300) return; // success, no messages (poll op=req)
	if (code == 1301) return; // success, message to dequeue (poll op=req)
	if (code == 1500) return; // success, ending session (logout)

	std::cout << "Response code: " << code << std::endl;
	AutoPtr<Element> ptr = parser.getElementByPath("response/result/msg", false);
	std::string msg = ptr->innerText();

	throw EPP::Exception(code, msg);
}

