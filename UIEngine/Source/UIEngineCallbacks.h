
#ifndef UIEngineCALLBACKS_HPP_INCLUDED
#define UIEngineCALLBACKS_HPP_INCLUDED

#include <Vision/Runtime/Base/VBase.hpp>
#include <Vision/Runtime/Engine/System/Vision.hpp>
#include <Vision/Runtime/Engine/System/VisApiCallbacks.hpp>
#include <Vision/Runtime/Base/System/VCallbacks.hpp>
#include <Vision/Runtime/Engine/SceneElements/VisApiStaticMeshInstance.hpp>
#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/GUI/VGUIIncludes.hpp>

#include "UIEngineIncludes.hpp"

// This header file must not include Havok Physics related header!
//#include <Vision/Runtime/EnginePlugins/Havok/HavokPhysicsEnginePlugin/vHavokPhysicsIncludes.hpp>

class UIPropertyDataBaseObject : public IVisCallbackDataObject_cl
{
public:
	UIPropertyDataBaseObject(VCallback *Sender ,VString szPropertyName)
		 : IVisCallbackDataObject_cl(Sender)
	{
 		m_szPropertyName = szPropertyName;
	//	m_value = value;
	}

 	VString m_szPropertyName;
//	T    m_value;
};


template<class T> class UIPropertyDataObject : public UIPropertyDataBaseObject
{
public:
	UIPropertyDataObject(VCallback *Sender ,VString szPropertyName, T &value )
		: UIPropertyDataBaseObject(Sender , szPropertyName )
	{
		m_value = value;
	}

	T    m_value;
};


class UIEventDataObject : public IVisCallbackDataObject_cl
{
public:
	UIEventDataObject(VisCallback_cl *pSender)
	{
		//meshInstances = NULL;
		// 		m_pMesh = NULL;
		// 		m_szExportPath = "";
		m_bMaintainFolderStructure = false;
	}

	// 	VisStaticMeshInstance_cl* m_pMesh;
	// 	VString m_szExportPath;
	bool m_bMaintainFolderStructure;
};

class UIItemDataEventObject : public IVisCallbackDataObject_cl
{
public:

	/// \brief
	///   Constructor that takes the sender callback instance and a render hook constant
	UIItemDataEventObject(VisCallback_cl *pSender, VMenuEventDataObject *pEvent ) : IVisCallbackDataObject_cl(pSender)
	{
		m_pEvent = pEvent;	
	}


	/// \brief
	///   The render hook constant that defines the current position inside the render loop.
	/// 
	/// Can be any enum value of VRenderHook_e
	/// 
	/// See also VisMeshBufferObject_cl::SetOrder for a list of constants.
	VMenuEventDataObject *m_pEvent;
};




class UIEngineCallbackManager_cl 
{
public:
	UIEngineCallbackManager_cl()
	{

	}

	static UIEngineCallbackManager_cl g_GlobalUICallbackManager;

	NODE_IMPEXP static UIEngineCallbackManager_cl& GlobalManager();


	VisCallback_cl OnUIPropertyChanged;	

	VisCallback_cl OnUIEventRaised;	

	void TestCall();
};

#endif
