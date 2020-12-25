#pragma once

#include <list>
#include <Poco/SharedPtr.h>
#include <EPP/XML/XmlHelpers.h>

namespace EPP
{
	class Command: public XmlSerializable
	{
	public:
		typedef Poco::SharedPtr<Command> Ptr;
	
		virtual std::list<XmlSerializable::Ptr> getExtensions() const
		{
			return std::list<XmlSerializable::Ptr>();
		}
		
		virtual inline ~Command() {}

	private:

	};

}
