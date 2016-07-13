#ifndef CONTENT_STATUS_H
#define CONTENT_STATUS_H

#include "content_classes.h"

namespace content
{
	class ContentStatus;
	
	class IContentCallback: public IBaseObject
	{
	public:
		virtual void invoke(ContentStatus* status) = 0;
		virtual void dispose() = 0;
	};

	template <typename T>
	class ContentCallback : public IContentCallback
	{
	public:
		typedef void(T::*Callback)(ContentStatus* status);
		T* delegateInstance;
		Callback delegateMethod;
		virtual void invoke(ContentStatus* status) OVERRIDE;
		virtual void dispose() OVERRIDE;
	};

	template <typename T>
	void ContentCallback<T>::invoke(ContentStatus* status)
	{
		(delegateInstance->*delegateMethod)(status);
	}

	template <typename T>
	void ContentCallback<T>::dispose()
	{
		delete this;
	}

	class ContentStatus
	{
	public:
		ContentBlock* owner;
		std::string path;
		ContentType::e type;
		ContentStatus(ContentBlock* owner, const char* path);
		~ContentStatus();

		template <typename T>
		void setLoadedCallback(T* delegateInstance, typename ContentCallback<T>::Callback delegateMethod);

		IContent* asContent() const;
		ContentImage* asImage() const;
		ContentFrameAnimation* asFrameAnimation() const;
		ContentBitmapFont* asBitmapFont() const;
		void invokeCallback();
	private:
		void detectType();
		void createContentInstance();
		void* _castInstance;
		IContent* _instance;
		IContentCallback* _callback;
	};

	template <typename T>
	void ContentStatus::setLoadedCallback(T* delegateInstance, typename ContentCallback<T>::Callback delegateMethod)
	{
		if (_callback != nullptr)
		{
			delete _callback;
		}
		ContentCallback<T> *callback = new ContentCallback<T>();
		callback->delegateInstance = delegateInstance;
		callback->delegateMethod = delegateMethod;
		_callback = callback;
	}
}

#endif /*CONTENT_STATUS_H*/