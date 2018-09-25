#include "Application.h"
#include "EngineDelegate.h"
#include "../Utils/Include.h"
#include "FilePath.h"

#ifdef __E3D_EDITOR__
extern "C"
{

	float cameraRotateX = 0, cameraRotateY = 0;
	bool mouseRButtonDown = false, mouseMButtonDown = false;
	vec2f curMousePosition;
	vec2f lastMousePosition;
	vec3f cameraRotation;
	vec3f curPos;
	vec3f curCameraPos;
	vec3f curCameraDir;
	float speed = 0.1f;
	Camera *editorCamera = nullptr;
	Scene *defaultScene = nullptr;

	void editorMouseButtonDown(MouseButtonInfo* mouseInfo)
	{
		if (editorCamera == nullptr)
			return;
		lastMousePosition.x = mouseInfo->mPositionX;
		lastMousePosition.y = mouseInfo->mPositionY;
		if (mouseInfo->mButton == eRightButton)
		{
			mouseRButtonDown = true;
		}
		else if (mouseInfo->mButton == eMiddleButton)
		{
			mouseMButtonDown = true;
		}
	}

	void editorMouseButtonUp(MouseButtonInfo *mouseInfo)
	{
		if (editorCamera == nullptr)
			return;
		if (mouseInfo->mButton == eRightButton)
		{
			mouseRButtonDown = false;
		}
		else if (mouseInfo->mButton == eMiddleButton)
		{
			mouseMButtonDown = false;
		}
	}

	void editorMouseMove(MouseButtonInfo* mouseInfo)
	{
		if (editorCamera == nullptr)
			return;
		curMousePosition.x = mouseInfo->mPositionX;
		curMousePosition.y = mouseInfo->mPositionY;

		if (mouseRButtonDown)
		{
			float rotaX = curMousePosition.y - lastMousePosition.y;
			float rotaY = curMousePosition.x - lastMousePosition.x;

			lastMousePosition = curMousePosition;

			cameraRotateX += rotaX * 0.2f;
			cameraRotateY += rotaY * 0.2f;
			cameraRotation = vec3f(-cameraRotateX, -cameraRotateY, 0);
			editorCamera->Transform->SetRotation(cameraRotation);
		}
		else if (mouseMButtonDown)
		{
			curPos = vec3f(-curMousePosition.x + lastMousePosition.x, curMousePosition.y - lastMousePosition.y, 0);

			vec3f dir = editorCamera->GetViewInverseMatrix() * curPos;
			dir.normalize();
			vec3f newPos = editorCamera->Transform->Position + dir;
			lastMousePosition = curMousePosition;
			editorCamera->Transform->SetPosition(newPos);
		}
	}

	void editorKeyDown(KeyCode keyCode)
	{
		if (editorCamera == nullptr)
			return;
		curCameraPos = editorCamera->Transform->Position;
		if (keyCode == KeyW)
		{
			curCameraDir = editorCamera->GetForwardVector();
			curCameraDir.normalize();
			editorCamera->Transform->SetPosition(curCameraPos + curCameraDir* speed);
		}
		else if (keyCode == KeyS)
		{
			curCameraDir = editorCamera->GetForwardVector() * -1;
			curCameraDir.normalize();
			editorCamera->Transform->SetPosition(curCameraPos + curCameraDir * speed);
		}
		else if (keyCode == KeyA)
		{
			curCameraDir = editorCamera->GetRigthVector() * -1 * speed;
			curCameraDir.normalize();
			editorCamera->Transform->SetPosition(curCameraPos + curCameraDir * speed);
		}
		else if (keyCode == KeyD)
		{
			curCameraDir = editorCamera->GetRigthVector() * speed;
			curCameraDir.normalize();
			editorCamera->Transform->SetPosition(curCameraPos + curCameraDir * speed);
		}
		else if (keyCode == KeyE)
		{
			curCameraDir = editorCamera->GetUpVector() * speed;
			curCameraDir.normalize();
			editorCamera->Transform->SetPosition(curCameraPos + curCameraDir * speed);
		}
		else if (keyCode == KeyQ)
		{
			curCameraDir = editorCamera->GetUpVector() * -1 * speed;
			curCameraDir.normalize();
			editorCamera->Transform->SetPosition(curCameraPos + curCameraDir * speed);
		}
	}

	void editorKeyUp(KeyCode keyCode)
	{
		if (editorCamera == nullptr)
			return;

	}

	__api_function_ void CreateEditor()
	{
		if (defaultScene != nullptr)
		{
			defaultScene = nullptr;
		}
		if (SceneManager::GetInstance().GetCurrentScene() == nullptr)
		{
			defaultScene = SceneManager::GetInstance().LoadScene("Data/Scene/default.scene");
		}
		editorCamera = Camera::CreateCamera();
		editorCamera->SetLayerMask(-1);
		ADD_IN_SCENE(editorCamera);
		cameraRotateX = 0;
		cameraRotateY = 0;
		editorCamera->Transform->SetPosition(0, 100, 200);
		Terrain * terrain = new Terrain();
		terrain->Create(512);
		terrain->SetIsEditorGrid(true);
		Material *m = GetRenderSystem()->GetMaterialManager()->CreateMaterial("Data/Material/Terrain.material", 1);
		terrain->SetMaterial(m);
		terrain->SetLayerMask(1);
		terrain->GetRenderer()->SetDrawModule(eDM_LINES);
		ADD_IN_SCENE(terrain);

	}

}

Camera *GetEditorCamera()
{
	return editorCamera;
}

#endif

namespace E3DEngine
{
	std::string Application::AppDataPath = "";
	std::string Application::ResourcePath = "";
	bool Application::m_bIsStop = false;
	MonoBehaviour * Application::m_pEntryBehaviour = nullptr;
	MonoBehaviour * Application::m_pBehaviour = nullptr;
	MouseButtonInfo * Application::m_pMouseInfo = nullptr;

	void Application::CreatScript()
	{
        mono_add_internal_call("E3DEngine.Application::getAppDataPath", (const void *)Application::getAppDataPath);
        mono_add_internal_call("E3DEngine.Application::exitApplication", (const void *)Application::ExitApp);
        mono_add_internal_call("E3DEngine.Application::getResourcePath", (const void *)Application::getResourcePath);
	}

	void Application::StartApp()
	{
		m_bIsStop = false;
		MonoScriptManager::GetInstance().Initialize();
		CreatScript();
		std::string startApp = AppDataPath + "/app.config";
		std::string fileContent = getContentFromPath(startApp);
		if (fileContent != empty_string)
		{
#ifndef __E3D_EDITOR__
			std::vector<std::string> vestr = E3DEngine::StringBuilder::Split(fileContent, "\n");
			for (auto &cfg : vestr)
			{
				if (cfg.find("entryClass") != std::string::npos)
				{
					std::vector<std::string> entryConfig = E3DEngine::StringBuilder::Split(cfg, "=");
					startCSharp(entryConfig);
				}
				else if (cfg.find("debugWindow") != std::string::npos)
				{

				}

			}
#endif
		}
		else
		{
			Debug::Log(ell_Error, "Application::StartApp ERROR app.config");
			assert(false);
		}
		m_pMouseInfo = new MouseButtonInfo();
	}

	void Application::startCSharp(std::vector<std::string> &vestr)
	{
		if (vestr.size() == 2)
		{
			string entryClass = vestr[1];
			std::vector<std::string> nameSpace_className = StringBuilder::Split(entryClass, ":");
			std::string entryNameSpce = "";
			std::string entryClassName = "";
			if (nameSpace_className.size() == 2)
			{
				entryNameSpce = StringBuilder::Trim(nameSpace_className[0]);
				entryClassName = StringBuilder::Trim(nameSpace_className[1]);
			}
			else if (nameSpace_className.size() == 1)
			{
				entryClassName = StringBuilder::Trim(nameSpace_className[0]);
			}
			m_pEntryBehaviour = new MonoBehaviour;
			m_pEntryBehaviour->SetImage(MonoScriptManager::GetInstance().GetCodeImage());
			m_pEntryBehaviour->Create(entryNameSpce.c_str(), entryClassName.c_str());
			m_pEntryBehaviour->CallMethod("Main");
		}
	}

	void Application::StopApp()
	{
		SceneManager::GetInstance().Destory();
		MonoScriptManager::GetInstance().Destory();
		m_bIsStop = true;
	}

	void Application::UpdateApp(float deltaTime)
	{
		if (m_bIsStop)
		{
			return;
		}
		if (m_pEntryBehaviour == nullptr)
		{
			return;
		}
		m_pEntryBehaviour->Update(deltaTime);
	}

	void Application::Destory()
	{
		if (m_pEntryBehaviour == nullptr)
		{
			return;
		}
		m_pEntryBehaviour->Destory();
	}


	void Application::ExitApp()
	{
		StopApp();
		exit(0);
	}

	Application::Application()
	{
		
	}

	Application::~Application()
	{
	}

	MonoString* Application::getAppDataPath()
	{
		return mono_string_new(MonoScriptManager::GetInstance().GetEngineDomain(), AppDataPath.c_str());
	}

	MonoString* Application::getResourcePath()
	{
		return mono_string_new(MonoScriptManager::GetInstance().GetEngineDomain(), ResourcePath.c_str());
	}

	void Application::MouseButtonDown(MouseButton btn, vec2f pos)
	{
		m_pMouseInfo->Set(btn, pos.x, pos.y);
#ifndef __E3D_EDITOR__
		void * args = m_pMouseInfo->GetMonoBehaviour()->GetMonoObject();
		m_pEntryBehaviour->CallMethod(MOUSE_BUTTON_DOWN, &args);
#else
		editorMouseButtonDown(m_pMouseInfo);
#endif
	}

	void Application::MouseButtonUp(MouseButton btn, vec2f pos)
	{
		m_pMouseInfo->Set(btn, pos.x, pos.y);
#ifndef __E3D_EDITOR__
		void * args = m_pMouseInfo->GetMonoBehaviour()->GetMonoObject();
		m_pEntryBehaviour->CallMethod(MOUSE_BUTTON_UP, &args);
#else
		editorMouseButtonUp(m_pMouseInfo);
#endif
	}

	void Application::MouseMove(vec2f pos)
	{
		m_pMouseInfo->Set(eUnKnown, pos.x, pos.y);
#ifndef __E3D_EDITOR__
		void * args = m_pMouseInfo->GetMonoBehaviour()->GetMonoObject();
		m_pEntryBehaviour->CallMethod(MOUSE_MOVE, &args);
#else
		editorMouseMove(m_pMouseInfo);
#endif
	}

	void Application::KeyDown(char key)
	{
		KeyCode keyCode = NativeSystem::GetKeyCode(key);
#ifndef __E3D_EDITOR__
		void *keyChar = &keyCode;
		m_pEntryBehaviour->CallMethod(KEY_DOWN, &keyChar);
#else
		editorKeyDown(keyCode);
#endif
	}

	void Application::KeyUp(char key)
	{
		KeyCode keyCode = NativeSystem::GetKeyCode(key);
#ifndef __E3D_EDITOR__
		void *keyChar = &keyCode;
		m_pEntryBehaviour->CallMethod(KEY_UP, &keyChar);
#else
		editorKeyUp(keyCode);
#endif
	}


	MouseButtonInfo::MouseButtonInfo(UINT mBtn, int posX, int posY)
	{
		Set(mBtn, posX, posY);
	}

	void MouseButtonInfo::CreateBehaviour()
	{
		mBehaviour->SetImage(MonoScriptManager::GetInstance().GetEngineImage());
		NEW_INSTANCE(MouseButtonInfo);
		Object::setBehaviourDefaultValue();
	}


	void MouseButtonInfo::Set(UINT mBtn, int posX, int posY)
	{
		CreateBehaviour();
		mButton = mBtn;
		mPositionX = posX;
		mPositionY = posY;
		TRANSFER_FIELD_VALUE(mButton);
		TRANSFER_FIELD_VALUE(mPositionX);
		TRANSFER_FIELD_VALUE(mPositionY);
	}

}
