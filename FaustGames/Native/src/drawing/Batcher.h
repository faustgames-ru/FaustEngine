#ifndef BATCHER_H
#define BATCHER_H

#include "drawing_classes.h"
#include "Camera.h"

namespace drawing
{
	class BatchBuffer;

	struct BatchEntry
	{
		ushort * IndicesStart;
		uint TextureId;
		uint LightmapId;
		int IndicesCount;
		int BatchBufferIndex;
		int TransformIndex;
		graphics::BlendState::e Blend;
		graphics::EffectBase *Effect;
	};
	
	typedef Mesh2dVertex TVertex;

	class BatchBuffer
	{
	public:
		const int VerticesLimit = 32768;
		const int IndicesLimit = 32768;

		BatchBuffer()
		{
			_vertices = (TVertex *)malloc(VerticesLimit * sizeof(TVertex));
			_indices = (ushort *)malloc(IndicesLimit * sizeof(unsigned short));
		}
		~BatchBuffer()
		{
			free(_vertices);
			free(_indices);
		}
		void reset()
		{
			_indicesCount = 0;
			_verticesCount = 0;
		}
		bool canAdd(int verticesCount, int indicesCount)
		{
			return ((_indicesCount + indicesCount) < IndicesLimit) && ((_verticesCount + verticesCount) < VerticesLimit);
		}
		void addMesh(TVertex *vertices, int verticesCount, ushort *indices, int indicesCount)
		{
			for (int i = 0; i < indicesCount; i++)
			{
				_indices[_indicesCount + i] = (unsigned short)(_verticesCount + indices[i]);
			}

			TVertex* target = _vertices + _verticesCount;
			TVertex* source = vertices;
			for (int i = 0; i < verticesCount; i++, target++, source++)
			{
				*target = *source;
			}

			_verticesCount += verticesCount;
			_indicesCount += indicesCount;
		}
		inline TVertex *getVertices(){ return _vertices; }
		inline int getVerticesCount(){ return _verticesCount; }
		inline ushort * getCurrentIndices(){ return _indices + _indicesCount; }
		inline int getIndicesCount(){ return _indicesCount; }
	private:
		TVertex *_vertices;
		ushort *_indices;
		int _indicesCount;
		int _verticesCount;
	};

	typedef std::vector<core::MatrixContainer> TBatchTransforms;
	typedef std::vector<BatchBuffer *> TBatchBuffers;
	typedef std::vector<BatchEntry> TBatchEntries;

	class RenderBuffer
	{
	public:
		TBatchBuffers Buffers;
		TBatchEntries Entries;
		TBatchTransforms Transforms;
		RenderBuffer()
		{
			Entries.reserve(128);
			Transforms.reserve(128);
			Buffers.push_back(new BatchBuffer());
		}
		~RenderBuffer()
		{
			for (TBatchBuffers::iterator i = Buffers.begin(); i != Buffers.end(); i++)
			{
				BatchBuffer *buffer = *i;
				delete buffer;
			}
		}
	};

	class Batcher : public llge::IBatch2d
	{
	public:
		Batcher();
		~Batcher();
		void start();
		void finish();
		void drawMesh(graphics::EffectBase *effect, graphics::BlendState::e blend, uint textureId, uint lightmapId, TVertex *vertices, int verticesCount, ushort *indices, int indicesCount);
		void executeRenderCommands();
		inline void applyEntry()
		{
			if (_currentEntry.IndicesCount != 0)
			{
				_buffer->Entries.push_back(_currentEntry);
				_currentEntry.IndicesCount = 0;
			}
		}

		virtual void API_CALL addProjection(void *floatMatrix) 
		{
			_projection.setValue((float *)floatMatrix);
			_buffer->Transforms.push_back(_projection);
			applyEntry();
		}
		virtual void API_CALL startBatch()
		{
			start();
		}
		virtual void API_CALL finishBatch()
		{
			finish();
		}
		virtual void API_CALL draw(llge::GraphicsEffects effect, llge::BlendMode blendMode, uint textureId, uint lightmapId, void *vertices, int verticesCount, void *indices, int indicesCount)
		{
			drawMesh(_converter.getEffect(effect), _converter.getBlend(blendMode), textureId, lightmapId, (TVertex *)vertices, verticesCount, (ushort *)indices, indicesCount);
		}

		virtual void API_CALL execute()
		{
			executeRenderCommands();
		}
	private:
		/*
		TBatchBuffers _buffers;
		TBatchEntries _entries;
		*/
		RenderBuffer *_buffer;
		//RenderBuffer *_backBuffer;

		BatchEntry _currentEntry;
		int _batchBufferIndex;
		
		uint _textureId;
		uint _lightmapId;
		core::MatrixContainer _projection;
		graphics::BlendState::e _blend;
		graphics::EffectBase *_effect;

		graphics::GraphicsDevice * _graphicsDevice;
		graphics::VertexFormat * _format;
		graphics::RenderConverter _converter;
	};	
}

#endif /*BATCHER*/