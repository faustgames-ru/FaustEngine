#ifndef BATCHER_H
#define BATCHER_H

#include "drawing_classes.h"
#include "Camera.h"
#include "PostProcess.h"

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
		void addMesh(uint color, float z, float *vertices, float *uvs, int verticesCount, int *indices, int indicesCount, bool additive)
		{
			for (int i = 0; i < indicesCount; i++)
			{
				_indices[_indicesCount + i] = (unsigned short)(_verticesCount + indices[i]);
			}

			TVertex* target = _vertices + _verticesCount;
			float* source = vertices;
			float* uvsource = uvs;
			for (int i = 0; i < verticesCount; i++, target++)
			{
				target->x = *source; ++source;
				target->y = *source; ++source;
				target->z = z;
				target->color = graphics::Color::premul(color, additive);
				target->u = *uvsource; ++uvsource;
				target->v = *uvsource; ++uvsource;
			}

			_verticesCount += verticesCount;
			_indicesCount += indicesCount;
		}

		void addMesh(TVertex *vertices, int verticesCount, ushort *indices, int indicesCount, bool additive)
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
				target->color = graphics::Color::premul(source->color, additive);
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

	struct BatcherState
	{
		graphics::EffectBase *Effect;
		graphics::BlendState::e Blend;
		uint TextureId;
		uint LightmapId;
	};

	struct BatcherSpineMesh
	{
		BatcherState State;
		uint Color;
		float *Vertices;
		float *Uvs;
		float Z;
		int VerticesCount;
		int *Indices;
		int IndicesCount;
		BatcherSpineMesh(){}
		BatcherSpineMesh(int verticesLimit)
		{
			Vertices = (float *)core::Mem::allocate(verticesLimit * sizeof(float) * 2);
		}
	};

	struct BatcherMesh
	{
		BatcherState State;
		TVertex *Vertices;
		int VerticesCount;
		ushort *Indices;
		int IndicesCount;
	};

	class Batcher : public llge::IBatch2d
	{
	public:
		Batcher();
		~Batcher();
		void start();
		void finish();
		void drawMesh(graphics::EffectBase *effect, graphics::BlendState::e blend, uint textureId, uint lightmapId, TVertex *vertices, int verticesCount, ushort *indices, int indicesCount);
		void drawSpineMesh(const BatcherSpineMesh &mesh);
		inline void drawMesh(const BatcherMesh &mesh)
		{
			drawMesh(
				mesh.State.Effect,
				mesh.State.Blend,
				mesh.State.TextureId,
				mesh.State.LightmapId,
				mesh.Vertices,
				mesh.VerticesCount,
				mesh.Indices,
				mesh.IndicesCount);
		}
		void executeRenderCommands(bool usePostProcess);

		virtual IntPtr API_CALL getNativeInstance()
		{
			return this;
		}

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

		virtual void API_CALL execute(bool usePostProcess)
		{
			executeRenderCommands(usePostProcess);
		}

		virtual void API_CALL setToneMap(uint tonemapId)
		{
			_tonemapId = tonemapId;
		}

	private:
		/*
		TBatchBuffers _buffers;
		TBatchEntries _entries;
		*/
		RenderBuffer *_buffer;
		//RenderBuffer *_backBuffer;
		uint _tonemapId;
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
		PostProcessBloom _bloom;
	};	
}

#endif /*BATCHER*/