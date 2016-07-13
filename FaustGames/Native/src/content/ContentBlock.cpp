#include "ContentBlock.h"

namespace content
{
	ContentBlock::ContentBlock(): ContentScale(0.01f)
	{
	}

	void ContentBlock::setRoot(const std::string& rootPath)
	{
		_rootPath = rootPath;
	}

	ContentStatus* ContentBlock::enqueueResource(const char* fileName)
	{
		ContentStatus* status = getMapValue(_resources, fileName);
		if (status == nullptr)
		{
			status = new ContentStatus(this, fileName);
			_resources[fileName] = status;
			_loadingQueue.push(status);
		}
		return status;
	}

	void ContentBlock::update()
	{
		if (_loadingQueue.empty())
		{
			return;
		}
		ContentStatus* status = _loadingQueue.front();
		_loadingQueue.pop();
		_loadArgs.path = status->path;
		_loadArgs.fullPath = _rootPath + status->path;
		_loadArgs.status = status;
		_loadArgs.owner = status->owner;
		status->asContent()->load(_loadArgs);
	}
}