﻿//
//  Texture.cpp
//
//  Created by 潘庆云 on 2017/3/30.
//
#include "E3DGLTexture2D.hpp"
#include "../E3DGL_RenderSystem.h"

namespace E3DEngine
{

	GL_Texture2D::~GL_Texture2D()
	{
		glDeleteTextures(1, &m_nTextureBuffer);
	}

	void GL_Texture2D::Create(Texture2dData *tData)
	{
		Texture2D::Create(tData);
		DWORD  rgbModule = 0;
		if (tData->rgbModule == PixelFormat::R8G8B8)
		{
			rgbModule = GL_RGB;
		}
		else if (tData->rgbModule == PixelFormat::R8G8B8A8)
		{
			rgbModule = GL_RGBA;
		}
		else if (tData->rgbModule == PixelFormat::L8A8)
		{
			rgbModule = GL_LUMINANCE_ALPHA;
		}
		else if (tData->rgbModule == PixelFormat::L8)
		{
			rgbModule = GL_LUMINANCE;
		}
		else
		{
			assert(false);
		}

		glGenTextures(1, &m_nTextureBuffer);
		glBindTexture(GL_TEXTURE_2D, m_nTextureBuffer);
		setTextureParam(tData);
		glTexImage2D(GL_TEXTURE_2D, 0, rgbModule, tData->width, tData->height, 0, rgbModule, GL_UNSIGNED_BYTE, tData->imgData);
		if (tData->useMipMap)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GL_Texture2D::setTextureParam(Texture2dData *tData)
	{
		unsigned int clampType = 0;
		unsigned int filterType = 0;
		Texture2dData *data = static_cast<Texture2dData*>(tData);
		switch (data->clampType)
		{
		case CLAMP_TYPE::CLAMP_TO_EDGE:
			clampType = GL_CLAMP_TO_EDGE;
			break;
		case  CLAMP_TYPE::MIRRORED_REPEAT:
			clampType = GL_MIRRORED_REPEAT;
			break;
		case CLAMP_TYPE::REPEAT:
			clampType = GL_REPEAT;
			break;
		default:
			assert(false);
		}

		switch (data->filterType)
		{
		case FILTER_TYPE::LINEAR:
			filterType = GL_LINEAR;
			break;
		case  FILTER_TYPE::NEAREST:
			filterType = GL_NEAREST;
			break;
		case FILTER_TYPE::LINEAR_MIPMAP_LINEAR:
			filterType = GL_LINEAR_MIPMAP_LINEAR;
			data->useMipMap = true;
			break;
		case  FILTER_TYPE::LINEAR_MIPMAP_NEAREST:
			filterType = GL_LINEAR_MIPMAP_NEAREST;
			data->useMipMap = true;
			break;
		case  FILTER_TYPE::NEAREST_MIPMAP_LINEAR:
			filterType = GL_NEAREST_MIPMAP_LINEAR;
			data->useMipMap = true;
			break;
		case  FILTER_TYPE::NEAREST_MIPMAP_NEAREST:
			filterType = GL_NEAREST_MIPMAP_NEAREST;
			data->useMipMap = true;
			break;
		default:
			assert(false);
		}
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clampType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clampType);
	}

	void GL_Texture2D::SetTextureData(Texture2dData *tData)
	{
		Texture2D::SetTextureData(tData);
		glBindTexture(GL_TEXTURE_2D, m_nTextureBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, tData->rgbModule, tData->width, tData->height, 0, tData->rgbModule, GL_UNSIGNED_BYTE,tData->imgData);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	GL_Texture2D::GL_Texture2D()
	{
		m_nTextureBuffer = 0;
	}

	GLuint & GL_Texture2D::GetTextureBuffer()
	{
		return m_nTextureBuffer;
	}

}
