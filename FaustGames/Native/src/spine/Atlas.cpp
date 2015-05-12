#include "Atlas.h"
#include "runtime\spine\Atlas.h"

namespace spine
{
	Atlas::Atlas() : _spAtlas(0)
	{
	}
	Atlas::~Atlas()
	{
	}
	
	void Atlas::create()
	{
		char *atlasFileData = "";
		int atalsDataLen = 0;
		char *atlasDir = "";
		_spAtlas = spAtlas_create(atlasFileData, atalsDataLen, atlasDir, 0);
	}
	
	void Atlas::cleanup()
	{
		if (_spAtlas != 0)
			spAtlas_dispose(_spAtlas);
		_spAtlas = 0;
	}

}