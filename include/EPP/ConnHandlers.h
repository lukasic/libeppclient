#pragma once

#include <string>

#include <Poco/Net/SocketStream.h>
#include <Poco/Net/SecureStreamSocket.h>

using Poco::Net::SecureStreamSocket;
using Poco::Net::SocketStream;

namespace EPP
{
namespace ConnHandlers
{

class Interface
{
public:
	virtual inline void send(const std::string& req) = 0;
	virtual inline std::string receive() = 0;
	virtual inline std::string request(const std::string & req) = 0;

private:

};

class StringSocket: public Interface
{
public:
	StringSocket(const std::string& host, int port);

	virtual inline ~StringSocket()
	{
		delete sockstream;
	}

	StringSocket(const StringSocket& orig) = delete;
	StringSocket& operator= (const StringSocket& rhs) = delete;

	virtual void send(const std::string& req);

	virtual std::string receive();
	virtual std::string request(const std::string & req);


private:
	const std::string host;
	const int port;

	Poco::Net::SecureStreamSocket socket;
	Poco::Net::SocketStream * sockstream;

	std::string last_request;
	std::string last_response;
};

class LoggedStringSocket: public StringSocket
{
public:
	inline LoggedStringSocket(const std::string& host, int port) : StringSocket(host, port) {}

	virtual inline ~LoggedStringSocket() {}

	virtual void send(const std::string& req);
	virtual std::string receive();

private:

};

};
}
