#include "UIEnginePluginPCH.h"
#include "VUINode.hpp"



static int g_bModuleRegCount = 0;

#define TABLE_SIZE 10000;

VUINodeMananger_cl* VUINodeMananger_cl::g_VUINodeManager = NULL;

void VUINodeMananger_cl::OneTimeInit()
{  
	Vision::Callbacks.OnWorldInit += this;
	Vision::Callbacks.OnAfterSceneUnloaded += this; 
	Vision::Callbacks.OnEditorModeChanging += this;	
}

void VUINodeMananger_cl::OneTimeDeInit()
{
  Vision::Callbacks.OnWorldInit -= this;
  Vision::Callbacks.OnAfterSceneUnloaded -= this;
  Vision::Callbacks.OnEditorModeChanging -= this;  

  DeInitializeUIEngine();
}

void VUINodeMananger_cl::InitializeUIEngine()
{
	if ( m_pGUIContext == NULL)
	{
		m_pGUIContext = new VDefaultGUIContext(NULL); // NULL means using the VGUIManager::GlobalManager			
	}

	m_pGUIContext->SetActivate(true);

	bool bTrue = VGUIManager::GlobalManager().LoadResourceFile("gui/MenuSystem.xml");

	if ( bTrue)
	{
		VTooltip *pCustomTooltip = new VTooltip();
		pCustomTooltip->SetDelay(0.5f); // shorter delay than usual
		m_pGUIContext->SetTooltipTemplate(pCustomTooltip);
		m_pGUIContext->SetShowCursor( true );

		m_bMenuResourceLoaded = true;
	}


	VUISharedData::Init();

	if ( m_bEditorMode == true )
	{
		m_pGUIContext->SetScale(hkvVec2(0.5f,0.5f));		
		m_pGUIContext->SetRenderHookConstant( VRH_PRE_SCREENMASKS );
	}
	else
	{
		hkvVec2 vScaleFactor = VUISharedData::GetUIScalingVector();

		hkvLog::Debug("UI Scale : %f , %f", vScaleFactor.x , vScaleFactor.y);

#if defined (_VISION_IOS) || defined(_VISION_ANDROID)
		m_pGUIContext->SetRenderHookConstant( VRH_AFTER_RENDERING );
		m_pGUIContext->SetScale(vScaleFactor);
#else
		m_pGUIContext->SetRenderHookConstant( VRH_AFTER_RENDERING );
		m_pGUIContext->SetScale(vScaleFactor);
#endif
	}

	m_bInitialized = true;
}


void VUINodeMananger_cl::DeInitializeUIEngine()
{
	VUISharedData::DeInit();
}

void VUINodeMananger_cl::ClearDialogs()
{
	m_pGUIContext->GetOpenDialogs().Clear();
}

void VUINodeMananger_cl::OnHandleCallback(IVisCallbackDataObject_cl *pData)
{
	if (pData->m_pSender==&Vision::Callbacks.OnWorldInit) // vForge 에서는 Play 상태에서만 호출된다.
	{	
 		if ( m_bEditorMode == true && m_bInitialized == false )
 		{
 			InitializeUIEngine();
 		}	
		else if ( m_bInitialized == true)
		{
			m_pGUIContext->SetActivate(true);
		}

		if ( m_bMenuResourceLoaded == false )
		{
			bool bTrue = VGUIManager::GlobalManager().LoadResourceFile("gui/MenuSystem.xml");

			if ( bTrue)
			{
				VTooltip *pCustomTooltip = new VTooltip();
				pCustomTooltip->SetDelay(0.5f); // shorter delay than usual


				if ( m_pGUIContext == NULL)
				{
					m_pGUIContext = new VDefaultGUIContext(NULL); // NULL means using the VGUIManager::GlobalManager			
				}		

				m_pGUIContext->SetTooltipTemplate(pCustomTooltip);
				m_pGUIContext->SetShowCursor( true );

				m_bMenuResourceLoaded = true;
			}
		}

		m_pGUIContext->SetActivate(true);
	}
	if (pData->m_pSender==&Vision::Callbacks.OnAfterSceneUnloaded)
	{		

	}	

	if (pData->m_pSender==&Vision::Callbacks.OnEditorModeChanging)
	{
		VisEditorModeChangedDataObject_cl* data = (VisEditorModeChangedDataObject_cl *)pData;

		if ( data->m_eNewMode == VisEditorManager_cl::EDITORMODE_ANIMATING || data->m_eNewMode == VisEditorManager_cl::EDITORMODE_PLAYING_IN_GAME  || data->m_eNewMode == VisEditorManager_cl::EDITORMODE_PLAYING_IN_EDITOR)
		{
			m_pGUIContext->SetShowCursor( true );
		}
		else
		{
			m_pGUIContext->SetShowCursor( false );
		}	   
	}
}

void VUINodeMananger_cl::ShowDialog(VDialog* vDialog)
{
	if ( VUINodeMananger_cl::GlobalManager()->GetGUIContext()->IsActive() == false )
	{
		VUINodeMananger_cl::GlobalManager()->GetGUIContext()->SetActivate(true);
	}

	VUINodeMananger_cl::GlobalManager()->GetGUIContext()->ShowDialog( vDialog );
}

void VUINodeMananger_cl::CloseDialog(VDialog* vDialog)
{
	VUINodeMananger_cl::GlobalManager()->GetGUIContext()->CloseDialog( vDialog );
}

int VUINodeMananger_cl::GetUIKey(VString szString)
{
	return VGUIManager::GetID( szString );
}

VDialog* VUINodeMananger_cl::FindDialog( VString szDialogClassName )
{
	int iCount = VUINodeMananger_cl::GlobalManager()->GetGUIContext()->GetOpenDialogs().Count();
	for( int i = 0 ; i< iCount ; i++ ) 
	{
		VDialog *pDialog = GetGUIContext()->GetOpenDialogs().GetAt(i);

		if ( szDialogClassName.CompareI( pDialog->GetTypeId()->m_lpszClassName ) == 0 )
		{
			return pDialog;
		}
	}

	return NULL;
}