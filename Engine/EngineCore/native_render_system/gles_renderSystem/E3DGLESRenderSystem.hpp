﻿//
//  GLES_RenderSystem.hpp
//
//  Created by 潘庆云 on 2017/1/22.
//

#ifndef _GLES_RENDERSYSTEM_HPP_
#define _GLES_RENDERSYSTEM_HPP_
#ifndef __IOS__
#include "EGL/EGLContext.h"
#endif
#include <stdio.h>
#include <src/RenderSystem/E3DRenderSystem.hpp>
#include "FrameBufferObject/E3DFrameBufferObject.h"
#include "Include/Include.h"

namespace E3DEngine
{
	class GLES_RenderSystem : public RenderSystem
	{
	public:
		GLES_RenderSystem();
		~GLES_RenderSystem() = default;
	public:
		virtual void Initilize() override;
		virtual void Cleanup() override;
		virtual void BindDefaultBackbuffer() override;
		virtual void ChangeRenderSurface(NATIVE_WINDOW windowHandle) override;
		void	CreateOpenGLES(EGLNativeDisplayType displayID, NATIVE_WINDOW windowHandle);
		void    Clear(Color4 color, int clearType);
		static GLES_RenderSystem * GetRenderSystem();
		virtual void UseRenderContext() override;
		virtual void UseShareContext() override;
		virtual void CreateShareContext() override;
		virtual void SetCullFaceType(CULL_FACE type) override;
	public:
		virtual void BeginFrame() override;
		void clearFrameBufferObject();
		void EndFrame() override;
	public:
		void	SetupDefaultFrameBuffer();

	private:
		GLESRenderSystem::FrameBufferObject * defaultFrameBuffer;
		EGL_Context *		m_pEGL_Context;
	};

}

#endif /* RenderSystem_hpp */
