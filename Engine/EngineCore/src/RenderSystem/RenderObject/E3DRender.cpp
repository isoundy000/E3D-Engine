﻿//
//  E3DVertexBuffer.cpp
//
//  Created by 潘庆云 on 2017/7/17.
//

#include "E3DRenderManager.hpp"

namespace E3DEngine
{
	void Renderer::FillVertex(Vertex vb)
	{
		Vertices.push_back(vb);
	}
	
	void Renderer::FillIndex(uint ib)
	{
		Indices.push_back(m_vertexCount + ib);
	}

	void Renderer::FillEnd()
	{
		
	}

	Renderer::Renderer()
	{
		m_vertexCount = 0;
		bLock = false;
		m_indexCount = 0;
		CreateBehaviour();
	}

	void Renderer::CreateBehaviour()
	{
		m_pBehaviour->SetImage(MonoScriptManager::GetInstance().GetEngineImage());
		m_pBehaviour->Create(NAME_SPACE, "Render");
	}

	void Renderer::setBehaviourDefaultValue()
	{
		Object::setBehaviourDefaultValue();
	}

	Renderer::~Renderer()
	{
		m_objRendererBuffers.clear();
	}

	DWORD Renderer::RecordCurrentVextexStartIndex(QWORD objId)
	{
		m_objRendererBuffers[objId].VertextStartIndex = m_vertexCount;
		return m_vertexCount;
	}

	DWORD Renderer::RecordCurrentIndexStartIndex(QWORD objId)
	{
		m_objRendererBuffers[objId].IndexStartIndex = m_indexCount;
		return m_indexCount;
	}

	void Renderer::VertexCountAdd(QWORD objId, uint vertexCount)
	{
		m_objRendererBuffers[objId].VertextNumber = vertexCount;
		m_vertexCount += vertexCount;
	}

	void Renderer::IndexCountAdd(QWORD objId, uint indexCount)
	{
		m_objRendererBuffers[objId].IndexNumber = indexCount;
		m_indexCount += indexCount;
	}

	void Renderer::SetBlendType(DWORD blendType)
	{
		glBlendType = blendType;
	}

	void Renderer::RemoveInRenderer(QWORD objId)
	{
		std::map<QWORD , RendererBuffer>::iterator vbuffer = m_objRendererBuffers.find(objId);
		if (vbuffer ==  m_objRendererBuffers.end())
		{
			return;
		}
		for (auto & m_objVextexBuffer : m_objRendererBuffers)
		{
			if (m_objVextexBuffer.second.VertextStartIndex > vbuffer->second.VertextStartIndex)
			{
				m_objVextexBuffer.second.VertextStartIndex -= vbuffer->second.VertextNumber;
				m_objVextexBuffer.second.IndexStartIndex -= vbuffer->second.IndexNumber;
			}
		}
		Vertices.erase(Vertices.begin() + vbuffer->second.VertextStartIndex, Vertices.begin() + vbuffer->second.VertextStartIndex + vbuffer->second.VertextNumber);
		Indices.erase(Indices.begin() + vbuffer->second.IndexStartIndex, Indices.begin() + vbuffer->second.IndexStartIndex + vbuffer->second.IndexNumber);
		for (int i = 0; i < Indices.size(); i ++)
		{
			if (i >= vbuffer->second.IndexStartIndex)
			{
				Indices[i] -= vbuffer->second.VertextNumber;
			}
		}
		m_vertexCount -= vbuffer->second.VertextNumber;
		m_indexCount -= vbuffer->second.IndexNumber;
		m_objRendererBuffers.erase(vbuffer);
	}
	
	void Renderer::prepareRender(float deltaTime)
	{
	

	}
	
	RendererBuffer* Renderer::GetRendererBuffer(QWORD objID)
	{
		if (m_objRendererBuffers.find(objID) == m_objRendererBuffers.end())
		{
			return nullptr;
		}
		return &m_objRendererBuffers[objID];
	}

	void Renderer::TransformChange()
	{
		
	}
	
}