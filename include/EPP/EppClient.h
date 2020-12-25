#pragma once

#include <map>

#include <mutex>

#include <EPP/ConnHandlers.h>
#include <EPP/Request.h>
#include <EPP/Response.h>
#include <EPP/XML/XmlHelpers.h>

#include <EPP/Session/Hello.h>
#include <EPP/Session/Greeting.h>

class NotImplemented {};

namespace EPP
{
	template<typename _Impl>
	class Client
	{
	public:

		inline Client()
		: low_client(nullptr), impl(nullptr)
		{}

		inline Client(EPP::ConnHandlers::Interface * lc)
		{
			Initialize(lc);
		}

		virtual inline void setInterface(EPP::ConnHandlers::Interface * lc)
		{
			Initialize(lc);
		}

		virtual inline ~Client()
		{
			delete impl;
			delete low_client;
		}

		template<
			typename _Request = typename _Impl::Request,
			typename _Response = typename _Impl::Response>
		inline _Response request(_Request& req)
		{
			std::lock_guard<std::mutex> lock(mtx_request);

			checkInstance();
			std::string xml_req = req.toXMLString();
			std::string xml_resp = this->low_client->request(xml_req);
			return _Response(xml_resp);
		}

		inline _Impl* operator->()
		{
			checkInstance();
			return impl;
		}

		template<
			typename _Request = EPP::Hello,
			typename _Response = EPP::Greeting>
		inline bool ping()
		{
			try {
				request<_Request, _Response>(_Request());
				return true;
			}
			catch (EPP::Exception e) {
				return false;
			}
		}

	private:

		inline void checkInstance() const {
			if (!low_client)
				throw EPP::Exception("EppClient not initialized.");
		}

		inline void Initialize(EPP::ConnHandlers::Interface * lc)
		{
			delete impl;
			delete low_client;

			impl = nullptr;
			low_client = nullptr;

			low_client = lc;
			impl = new _Impl(*this);
			low_client->receive();
		}

		EPP::ConnHandlers::Interface * low_client;
		_Impl * impl;

		std::mutex mtx_request;
	};

	template<
		typename _Impl,
		typename _SocketHandler
	>
	class AutoReconnectingClient
	{
	public:

		inline AutoReconnectingClient()
			: client(nullptr)
		{}

		inline AutoReconnectingClient(const std::string& host, int port)
			: host(host), port(port), client(nullptr)
		{}

		inline ~AutoReconnectingClient()
		{
			_releaseClient();
		}

		inline void setTarget(const std::string& host, int port)
		{
			this->host = host;
			this->port = port;
		}

		inline void autologin(const std::string& clid, const std::string& pw)
		{
			this->clid = clid;
			this->pw = pw;
		}

		inline _Impl* operator->()
		{
			Client<_Impl>& c = getOrCreateClient();
			return c.operator->();
		}

		template<
			typename _Request = typename _Impl::Request,
			typename _Response = typename _Impl::Response
		>
		inline _Response request(_Request& req)
		{
			auto c = getOrCreateClient();
			return c.request<_Request, _Response>(req);
		}


	private:

		Client<_Impl>& getOrCreateClient()
		{
			if (host == "")
				throw EPP::Exception("Client not initialized.");

			if (client != nullptr && client->ping() == false)
				_releaseClient();

			if (client == nullptr)
				_newClient();

			return *client;
		}

		void _releaseClient()
		{
			if (!client) return;
			delete client;
			client = nullptr;
		}

		void _newClient()
		{
			_releaseClient();

			client = new Client<_Impl>();
			client->setInterface(new _SocketHandler(host, port));

			if (clid != "")
			{
				Client<_Impl>& c = *client;
				c->login(clid, pw);
			}
		}

		std::string host;
		int port;

		std::string clid;
		std::string pw;

		Client<_Impl> * client;
	};

}
