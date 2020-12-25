#include <EPP/ConnHandlers.h>

#include <string>
#include <iostream>
#include <sstream>

#include <Poco/Net/Context.h>
#include <Poco/Net/NetException.h>

#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"

#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/SecureStreamSocket.h>

#include <Poco/AutoPtr.h>
#include <Poco/ByteOrder.h>
#include "Poco/StreamCopier.h"

#include <EPP/Exception.h>

using namespace Poco;
using namespace Poco::Net;

using namespace EPP::ConnHandlers;

StringSocket::StringSocket(const std::string& host, int port) : host(host), port(port)
{
	SharedPtr<InvalidCertificateHandler> ptrCert = new AcceptCertificateHandler(false);
	Context::Ptr ptrContext = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_RELAXED, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	SSLManager::instance().initializeClient(nullptr, ptrCert, ptrContext);

	Poco::Net::SocketAddress sa(host, port);
	socket = Poco::Net::SecureStreamSocket(sa);
	sockstream = new Poco::Net::SocketStream(socket);
}


void StringSocket::send(const std::string& req)
{
	uint32_t size = req.size() + 4;
	uint32_t header = Poco::ByteOrder::toBigEndian(size);

	sockstream->write((char *)&header, 4);
	sockstream->write((char *)&req[0], req.size());
	sockstream->flush();

	last_request = req;
}

std::string StringSocket::receive()
{
	std::string str;
	uint32_t header = 0;
	sockstream->read((char *)&header, 4);

	if (header == 0)
		throw EPP::Exception("Invalid header received. Broken connection?");

	uint32_t length = Poco::ByteOrder::fromBigEndian(header) - 4;

	char * _data = new char[length + 1];
	sockstream->read(_data, length);
	_data[length] = '\0';

	std::string data(_data);

	for (int i = 0; i < length; i++)
		_data[i] = '\0';
	delete [] _data;

	if (data.size() != length)
	{
		std::ostringstream oss;
		oss << "Invalid data length: received " << data.size() << " bytes, expected " << length << "bytes.";
		throw ::EPP::Exception(oss.str());
	}

	last_response = std::string(data);
	return last_response;
}

std::string StringSocket::request(const std::string & req)
{
	send(req);
	return receive();
}

void LoggedStringSocket::send(const std::string& req)
{
	std::cout << "Request:" << std::endl;
	std::cout << "========" << std::endl;
	std::cout << req << std::endl << std::endl;
	StringSocket::send(req);
}

std::string LoggedStringSocket::receive()
{
	std::cout << "Response:" << std::endl;
	std::cout << "=========" << std::endl;
	std::string receive = StringSocket::receive();
	std::cout << receive << std::endl << std::endl;
	return receive;
}
