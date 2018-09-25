
#include "stdafx.h"
#include "GameObject.h"

#pragma managed
namespace E3DEngine
{
	GameObjectRef::GameObjectRef(GameObject *gameObject)
	{
		SetValue(gameObject);
	}

	System::String^ GameObjectRef::GetName()
	{
		return mName;
	}

	void GameObjectRef::SetValue(GameObject *gameObject)
	{
		mGameObject = gameObject;
		mTransform = gcnew TransformRef(gameObject->Transform);
		mName = gcnew String(gameObject->mName.c_str());
	}

	System::Collections::Generic::List<GameObjectRef^> ^ GameObjectRef::GetChilds()
	{
		List<GameObjectRef ^>^ lst = gcnew List<GameObjectRef ^>();
		std::map<UINT, GameObject*> childs = mGameObject->GetChilds();
		for (auto child : childs)
		{
			GameObjectRef ^ obj = gcnew GameObjectRef(child.second);
			lst->Add(obj);
		}
		return lst;
	}

	E3DEngine::TransformRef ^ GameObjectRef::GetTransform()
	{
		return mTransform;
	}

}
#pragma unmanaged