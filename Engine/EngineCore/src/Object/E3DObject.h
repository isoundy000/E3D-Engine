/************************************************************************/
/* Node                                                                  
/* Create by PanQingyun
/* 2017-9-30
/************************************************************************/
#ifndef __ENGINE_3D_NODE_H__
#define __ENGINE_3D_NODE_H__
#include "../Source/Interface.h"

class MonoBehaviour;
namespace E3DEngine
{
	typedef enum
	{
		eT_Mesh,
		eT_Bone,
		eT_Particle,
		eT_MagicWord,
		eT_NormalRenderObject,
		eT_GameObject,
		eT_Scene,
		eT_RTT,
		eT_Object,
		eT_Camera,
	} ObjectType;

#define ADD_IN_SCENE(obj)\
	if (SceneManager::GetInstance().GetCurrentScene() != nullptr)\
	{\
		SceneManager::GetInstance().GetCurrentScene()->AddChild(obj);\
	}

	class CTransform;
	class Object extends Ref
	{
	public:
		Object();
		virtual ~Object();
	public:
		virtual void Update(float deltaTime);
		MonoBehaviour * GetMonoBehaviour();
		virtual void CreateBehaviour();
	protected:
		virtual void setBehaviourDefaultValue();
	public:
		UINT			ID;
		std::string		TypeName;
		std::string		Name;
		ObjectType		NodeType;
		MonoBehaviour * m_pBehaviour;
	};
}

#endif // !__ENGINE_3D_NODE_H__