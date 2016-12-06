#include "spine_classes.h"
#include "spine/Atlas.h"
#include "spine/extension.h"
#include "SpineSkeletonResource.h"

namespace rounding
{
#define ROUNDING_EPSILON 0.0000001
	//--------------------------------------------------------------------------
	// round down
	// Bias: -Infinity
	using std::floor;

	//--------------------------------------------------------------------------
	// round up
	// Bias: +Infinity
	using std::ceil;

	//--------------------------------------------------------------------------
	// symmetric round down
	// Bias: towards zero
	float floor0(const float& value)
	{
		float result = std::floor(std::fabs(value));
		return (value < 0.0) ? -result : result;
	}

	//--------------------------------------------------------------------------
	// A common alias for floor0()
	// (notwithstanding hardware quirks)
	inline float trunc(const float& value)
	{
		return floor0(value);
	}

	//--------------------------------------------------------------------------
	// symmetric round up
	// Bias: away from zero
	float ceil0(const float& value)
	{
		float result = std::ceil(std::fabs(value));
		return (value < 0.0) ? -result : result;
	}

	//--------------------------------------------------------------------------
	// Common rounding: round half up
	// Bias: +Infinity
	float roundhalfup(const float& value)
	{
		return std::floor(value + 0.5);
	}

	//--------------------------------------------------------------------------
	// Round half down
	// Bias: -Infinity
	float roundhalfdown(const float& value)
	{
		return std::ceil(value - 0.5);
	}

	//--------------------------------------------------------------------------
	// symmetric round half down
	// Bias: towards zero
	float roundhalfdown0(const float& value)
	{
		float result = roundhalfdown(std::fabs(value));
		return (value < 0.0) ? -result : result;
	}

	//--------------------------------------------------------------------------
	// symmetric round half up
	// Bias: away from zero
	float roundhalfup0(const float& value)
	{
		float result = roundhalfup(std::fabs(value));
		return (value < 0.0) ? -result : result;
	}

	//--------------------------------------------------------------------------
	// round half even (banker's rounding)
	// Bias: none
	float roundhalfeven(
		const float& value,
		const float& epsilon = ROUNDING_EPSILON) 
	{
		if (value < 0.0) return -roundhalfeven(-value, epsilon);

		float ipart;
		std::modf(value, &ipart);

		// If 'value' is exctly halfway between two integers
		if ((value - (ipart + 0.5)) < epsilon)
		{
			// If 'ipart' is even then return 'ipart'
			if (std::fmod(ipart, 2.0) < epsilon)
				return ipart;

			// Else return the nearest even integer
			return ceil0(ipart + 0.5);
		}

		// Otherwise use the usual round to closest
		// (Either symmetric half-up or half-down will do0
		return roundhalfup0(value);
	}	
}
void _spAtlasPage_createTexture(spAtlasPage* self, const char* path)
{
	graphics::TextureImage2d* texture;
	spine::AtlasRenderObject* renderObject = static_cast<spine::AtlasRenderObject *>(self->atlas->rendererObject);

	if (std::string(path).find("Pirat_4_PNG_kist_lev") != std::string::npos)
	{
		renderObject = static_cast<spine::AtlasRenderObject *>(self->atlas->rendererObject);
	}

	int w = static_cast<int>(rounding::roundhalfdown(self->width*renderObject->applyedCompression));
	int h = static_cast<int>(rounding::roundhalfdown(self->height*renderObject->applyedCompression));
	
	if (resources::ContentManager::_replaceSeparator)
	{
		std::string replace = path;
		for (int i = 0; i < replace.size(); i++)
		{
			if (replace[i] == '/')
				replace[i] = '_';
		}


		texture = resources::ContentManager::Default.addLoadTexture(replace.c_str(), w, h, renderObject->pagesFormat);
		resources::ContentManager::Default._loadedImages[replace] = texture;
	}
	else 
	{
		texture = resources::ContentManager::Default.addLoadTexture(path, w, h, renderObject->pagesFormat);
		resources::ContentManager::Default._loadedImages[path] = texture;
	}

	self->rendererObject = texture;
}

void _spAtlasPage_disposeTexture(spAtlasPage* self)
{
	graphics::TextureImage2d* texture = (graphics::TextureImage2d*)self->rendererObject;
	resources::ContentManager::Default.addDisposeTexture(texture);
	resources::TexturesMap::iterator find = resources::ContentManager::Default._loadedImages.end();
	for (resources::TexturesMap::iterator it = resources::ContentManager::Default._loadedImages.begin(); it != resources::ContentManager::Default._loadedImages.end(); it++)
	{
		if (it->second == texture)
			find = it;
	}
	if (find != resources::ContentManager::Default._loadedImages.end())
		resources::ContentManager::Default._loadedImages.erase(find);
}

char* _spUtil_readFile(const char* path, int* length)
{
	*length = 0;
	return 0;
}