﻿//
//  E3DMaterialManager.cpp
//
//  Created by 潘庆云 on 2017/7/17.
//

#include "E3DGLESMaterialManager.hpp"
#include "src/Config/Table.h"
#include "src/Config/TableRegister.h"
#include "3rd/header/Helpers.h"
#include "E3DGLESMaterial.hpp"

namespace E3DEngine
{
	Material * GLES_MaterialManager::GetMaterial(int id)
	{
		if (m_mapIDMaterials.find(id) == m_mapIDMaterials.end())
		{
			return nullptr;
		}
		return m_mapIDMaterials[id];
	}

	Material* GLES_MaterialManager::CreateMaterial(std::string path)
	{
		if (m_mapMaterials.find(path) != m_mapMaterials.end())
		{
			return m_mapMaterials[path];
		}

		TableManager* tblManager = TableRegister::RegisterAllTable(path.c_str());
		if (tblManager == nullptr)
		{
			return nullptr;
		}

		std::vector<MaterialConfig*> * materialConfigs = tblManager->GetTable<MaterialConfig>();
		if (materialConfigs->size() == 0)
		{
			return nullptr;
		}
		std::string folder, file;
		StringManipulator::SplitFileName(path, folder, file);
		MaterialConfig *config = (*materialConfigs)[0];
		GLES_Material * material = new GLES_Material;
		material->filePath = folder + "/";
		m_mapMaterials[path] = material;
		ShaderConfig * sCfg = tblManager->Select<ShaderConfig>(config->ShaderID);
		material->CreateMaterial(config, sCfg);
		m_mapIDMaterials[material->ID] = material;
		SAFE_DELETE(tblManager);
		return material;
	}
	

}