#ifndef CONTENT_JSON_H
#define CONTENT_JSON_H

#include "content_classes.h"

namespace content
{
	class ContentJson : public IContent
	{
	public:
		std::string name;
		ContentValue* value;
		static ContentJson* create();
		void internalLoad(const LoadArgs& e);
		virtual void dispose() OVERRIDE;
		virtual void load(const LoadArgs& e) OVERRIDE;
	private:
		ContentJson();
		~ContentJson();
	};
}

#endif /*CONTENT_JSON_H*/
