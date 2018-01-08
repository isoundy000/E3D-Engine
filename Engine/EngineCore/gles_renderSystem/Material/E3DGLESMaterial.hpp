﻿//
//  Material.hpp
//
//  Created by 潘庆云 on 2017/3/30.
//

#ifndef _GLES_METRAIL_HPP_
#define _GLES_METRAIL_HPP_

#include "../Shader/E3DGLESShader.hpp"
#include "../Texture/E3DGLESTexture.hpp"
#include "../Texture/E3DGLESCubeMapTexture.hpp"
#include <src/Config/Table.h>
#include <src/Source/E3DDebug.h>
#include <src/RenderSystem/Material/E3DMaterial.hpp>
#include "../Include/Include.h"

namespace E3DEngine
{

	class GLES_Material : public Material
	{
	public:
		GLES_Material();
		virtual void UseMaterial();
		virtual void Destory();
		virtual void CreateMaterial(MaterialConfig * config, ShaderConfig * sCfg);
		virtual void SetTexture(Texture * texture, int index);
		virtual void BindTexture();
		virtual void UseProgram();		
		virtual void UseNullProgram();
		virtual void InitShaderVar();
		virtual void InvalidMaterial();
	public:
		virtual void CreateShader(ShaderConfig *cfg)
		{
			pShader = new GLES_Shader;
			InitShaderVar();
			parseShaderConfig(cfg);
		}
		virtual void CreateCubeTexture( std::string dirPath,std::string xPName,
									   std::string xNName,
									   std::string yPName,std::string yNName,std::string zPName,std::string ZNName);
	protected:
		virtual void parseShaderConfig(ShaderConfig *cfg)override;
		virtual void LoadShader(const char * vertexShaderString, const char * fragmentShaderString);

		std::string processVS();
		void enableStencil()override;
		virtual void processUniformVar(ShaderConfig * cfg)override;
		virtual void beforeUpdate()override;
		virtual void afterUpdate()override;
		virtual void createTexture(std::string textureName, std::string textureUniform)override;
		virtual void createTexture(Texture *texture, std::string textureUniform)override;
		
	};
}

#endif /* Material_hpp */