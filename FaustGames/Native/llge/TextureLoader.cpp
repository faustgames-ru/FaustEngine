#include "stdafx.h"
#include "TextureLoader.h"

namespace graphics
{
	TextureTestLoader TextureLoader::_testLoader;

	TextureAbstractLoader * TextureLoader::instance()
	{
		return &_testLoader;
	}


	TextureTestLoader::TextureTestLoader() : _data(2*2)
	{
	}

	Image2dData * TextureTestLoader::loadImage2dData(int id)
	{
		_data.Width = 2;
		_data.Height = 2;
		switch (id)
		{
		case(0) :
			_data.Pixels[0] = 0xff000000;
			_data.Pixels[1] = 0xff000000;
			_data.Pixels[2] = 0xff000000;
			_data.Pixels[3] = 0xff000000;
			break;
		case(1) :
			_data.Pixels[0] = 0xff000000;
			_data.Pixels[1] = 0xff000000;
			_data.Pixels[2] = 0xff000000;
			_data.Pixels[3] = 0xffffffff;
			break;
		case(2) :
			_data.Pixels[0] = 0xff000000;
			_data.Pixels[1] = 0xff000000;
			_data.Pixels[2] = 0xffffffff;
			_data.Pixels[3] = 0xff000000;
			break;
		case(3) :
			_data.Pixels[0] = 0xff000000;
			_data.Pixels[1] = 0xff000000;
			_data.Pixels[2] = 0xffffffff;
			_data.Pixels[3] = 0xffffffff;
			break;
		case(4) :
			_data.Pixels[0] = 0xff000000;
			_data.Pixels[1] = 0xffffffff;
			_data.Pixels[2] = 0xff000000;
			_data.Pixels[3] = 0xff000000;
			break;
		case(5) :
			_data.Pixels[0] = 0xff000000;
			_data.Pixels[1] = 0xffffffff;
			_data.Pixels[2] = 0xff000000;
			_data.Pixels[3] = 0xffffffff;
			break;
		case(6) :
			_data.Pixels[0] = 0xff000000;
			_data.Pixels[1] = 0xffffffff;
			_data.Pixels[2] = 0xffffffff;
			_data.Pixels[3] = 0xff000000;
			break;
		case(7) :
			_data.Pixels[0] = 0xff000000;
			_data.Pixels[1] = 0xffffffff;
			_data.Pixels[3] = 0xffffffff;
			_data.Pixels[3] = 0xffffffff;
		case(8) :
			_data.Pixels[0] = 0xffffffff;
			_data.Pixels[1] = 0xff000000;
			_data.Pixels[2] = 0xff000000;
			_data.Pixels[3] = 0xff000000;
			break;
		case(9) :
			_data.Pixels[0] = 0xffffffff;
			_data.Pixels[1] = 0xff000000;
			_data.Pixels[2] = 0xff000000;
			_data.Pixels[3] = 0xffffffff;
			break;
		case(10) :
			_data.Pixels[0] = 0xffffffff;
			_data.Pixels[1] = 0xff000000;
			_data.Pixels[2] = 0xffffffff;
			_data.Pixels[3] = 0xff000000;
			break;
		case(11) :
			_data.Pixels[0] = 0xffffffff;
			_data.Pixels[1] = 0xff000000;
			_data.Pixels[3] = 0xffffffff;
			_data.Pixels[3] = 0xffffffff;
			break;
		case(12) :
			_data.Pixels[0] = 0xffffffff;
			_data.Pixels[1] = 0xffffffff;
			_data.Pixels[2] = 0xff000000;
			_data.Pixels[3] = 0xff000000;
			break;
		case(13) :
			_data.Pixels[0] = 0xffffffff;
			_data.Pixels[1] = 0xffffffff;
			_data.Pixels[2] = 0xff000000;
			_data.Pixels[3] = 0xffffffff;
			break;
		case(14) :
			_data.Pixels[0] = 0xffffffff;
			_data.Pixels[1] = 0xffffffff;
			_data.Pixels[2] = 0xffffffff;
			_data.Pixels[3] = 0xff000000;
			break;
		case(15) :
			_data.Pixels[0] = 0xffffffff;
			_data.Pixels[1] = 0xffffffff;
			_data.Pixels[3] = 0xffffffff;
			_data.Pixels[3] = 0xffffffff;
			break;
		default:
			break;
		}		
		return &_data;
	}
}