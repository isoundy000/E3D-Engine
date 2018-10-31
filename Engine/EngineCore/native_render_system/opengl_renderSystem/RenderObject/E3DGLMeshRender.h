#pragma once
#include "E3DGLRender.hpp"
#include <src/Mesh/E3DMeshRender.h>

namespace E3DEngine
{
	class GL_MeshRender : public MeshRender
	{
	public:
		GL_MeshRender()
		{
			pRender = new GL_Renderer;
			pRender->mName = mName;
		}
		virtual ~GL_MeshRender() override
		{
			SAFE_DELETE(pRender);
		}
		
	public:
		virtual void FillBegin(UINT objId);
		virtual void FillVertex(Vertex vb);
		virtual void FillIndex(uint ib);
		virtual void FillEnd(UINT objId, uint vertexCount, uint indexCount);
		virtual void RemoveInRenderer(UINT objId);
		virtual RendererBuffer* GetRendererBuffer(UINT objID);
		virtual void TransformChange() override;
		virtual void Render(float deltaTime) override;
		virtual void SetDrawModule(DWORD module) override;

	public:
		virtual UINT GetDrawModule() { return pRender->GetDrawModule(); }
		virtual CTransform * GetTransform();
		virtual void SetTransform(CTransform *_transform);
		virtual void CreateNewTransform();
		//virtual void SetRenderIndex(DWORD index) override;

		virtual void SetMaterial(Material *material);
		virtual void SetLayerMask(DWORD layer) { pRender->SetLayerMask(layer); }
		virtual Material *GetMaterial() { return pRender->GetMaterial(); }
		virtual void SetCamera(Camera * camera) { pRender->SetCamera(camera); }
		virtual void ClearVertexIndexBuffer() { pRender->ClearVertexIndexBuffer(); }
		virtual Camera *GetCamera() override;
	protected:
		GL_Renderer * pRender;
	};
}