#ifndef LOADER_JSON_SCENE_H
#define LOADER_JSON_SCENE_H

#include "../../zombo_classes.h"
#include "../../scenes/ZomboScene.h"
#include "../../cjson/cJSON.h"

namespace zombo
{
	class ZomboJsonScene
	{
	public:
		std::vector<std::string> resources;		
		ZomboJsonScene(const char *jsonString);
		~ZomboJsonScene();
	private:
		cJSON * _json;
	};
}

#endif /*LOADER_JSON_SCENE_H*/
