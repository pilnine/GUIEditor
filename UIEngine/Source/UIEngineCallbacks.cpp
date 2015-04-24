

#include "UIEngineCallbacks.h"
//VisCallback_cl OnCreateCacheFile;


//VisCallback_cl HavokCacheCallbackManager_cl::OnCreateCacheFile;

// HavokCacheCallbackManager_cl::HavokCacheCallbackManager_cl()
//{
//}

UIEngineCallbackManager_cl UIEngineCallbackManager_cl::g_GlobalUICallbackManager;
// -------------------------------------------------------------------------- //
// Init/ Deinit                                                 
// -------------------------------------------------------------------------- //
UIEngineCallbackManager_cl& UIEngineCallbackManager_cl::GlobalManager() 
{
	return g_GlobalUICallbackManager;
}


void UIEngineCallbackManager_cl::TestCall()
{	
// 	int iValue = 0 ;
// 	UIPropertyDataObject<int> eventItemINT( &UIEngineCallbackManager_cl::GlobalManager().OnUIPropertyChanged ,"property" , iValue );
// 	UIEngineCallbackManager_cl::GlobalManager().OnUIPropertyChanged.TriggerCallbacks( &eventItemINT );
// 
// 	float fValue = 0 ;
// 	UIPropertyDataObject<float> eventItemFLOAT( &UIEngineCallbackManager_cl::GlobalManager().OnUIPropertyChanged ,"property" , fValue );
// 	UIEngineCallbackManager_cl::GlobalManager().OnUIPropertyChanged.TriggerCallbacks( &eventItemFLOAT );
}

