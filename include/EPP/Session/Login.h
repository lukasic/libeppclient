#pragma once

#include <iostream>
#include <map>
#include <EPP/Request.h>
#include <EPP/XML/XmlHelpers.h>

#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/Text.h>
#include <Poco/DOM/AutoPtr.h>

using Poco::XML::AutoPtr;
using Poco::XML::Document;
using Poco::XML::Element;
using Poco::XML::Text;


inline std::vector<std::string>& operator<< (std::vector<std::string>& v, const std::string& str) {
	v.push_back(str);
	return v;
}

namespace EPP
{
	namespace Commands
	{
		class Login: public EPP::Command
		{

		private:
			struct svcsHelper: public XmlSerializable
			{
				std::vector<std::string> objURI;
				std::vector<std::string> svcExtension;

				virtual inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
				{
					AutoPtr<Element> eSVCS = pDoc->createElement("svcs");

					for (unsigned i = 0; i < objURI.size(); i++)
						eSVCS->appendChild(CreateTextElement(pDoc, "objURI", objURI[i]));

					if (svcExtension.size() > 0)
					{
						AutoPtr<Element> eSVCExt = pDoc->createElement("svcExtension"); 
						for (unsigned i = 0; i < svcExtension.size(); i++)
							eSVCExt->appendChild(CreateTextElement(pDoc, "extURI", svcExtension[i]));

						eSVCS->appendChild(eSVCExt);
					}

					return eSVCS;
				}

			};

			struct optionsHelper
			{
				std::map<std::string, std::string> data;

				inline void set(const std::string& key, std::string& value)
				{
					if (data.find(key) == data.end())
						data.insert(std::pair<std::string, std::string>(key, value));
					else
						data[key] = value;
				}

				inline std::string& operator[] (const std::string& key)
				{
					if (data.find(key) == data.end())
						data.insert(std::pair<std::string, std::string>(key, ""));
					
					return data[key];
				}

			};

			struct optionsHelperSORTED: public XmlSerializable
			{
				std::vector<std::pair<std::string, std::string>> data;

				inline std::string& operator[] (const std::string& key)
				{
					for (unsigned i = 0; i < data.size(); i++)
						if (data[i].first == key)
							return data[i].second;

					data.push_back(std::pair<std::string, std::string>(key, ""));
					return data.back().second;
				}

				inline void set(const std::string& key, std::string& value)
				{
					this->operator[](key) = value;
				}

				virtual inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
				{
					AutoPtr<Element> pRoot = pDoc->createElement("options");

					for (auto i = data.begin(); i != data.end(); ++i)
						pRoot->appendChild(CreateTextElement(pDoc, i->first, i->second));

					return pRoot;
				}

			};

		public:
			inline Login(): EPP::Command()
			{}
			
			inline Login(const std::string& clID, const std::string& pw)
			: EPP::Command(), clID(clID), pw(pw)
			{}

			svcsHelper svcs;
			optionsHelperSORTED options;

			// Encapsulation not required - better interface
			std::string clID;
			std::string pw;
			std::string newPW;

			virtual inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
			{
				AutoPtr<Element> pRoot = pDoc->createElement("login");

				pRoot->appendChild(CreateTextElement(pDoc, "clID", clID));
				pRoot->appendChild(CreateTextElement(pDoc, "pw", pw));

				if (newPW.size() > 0)
					pRoot->appendChild(CreateTextElement(pDoc, "newPW", newPW));

				pRoot->appendChild(options.toXML(pDoc));
				pRoot->appendChild(svcs.toXML(pDoc));

				return pRoot;
			}

		};
	}
}