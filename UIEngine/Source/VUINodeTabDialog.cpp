#include "VUINodeTabDialog.h"
#include "UIEngineCallbacks.h"

#include <Vision/Runtime/Base/VBase.hpp>
#include "VUINodePushButton.h"


#include "UIEventHandlerComponent.h"



class TabButton : public VStyledButton
{
public:
	TabButton(const char* szTabTitle) : VStyledButton(szTabTitle, false, VUISharedData::GetFont(), VUISharedData::GetScale(), VUISharedData::GetBorderWidth(), VUISharedData::GetFontScale())
	{
		m_TextCfg.FinishSetup();

		m_bIsSelected = false;
	}

	virtual void OnPaint(VGraphicsInfo& Graphics, const VItemRenderInfo& parentState) HKV_OVERRIDE
	{
		const VRectanglef rect = GetBoundingBox();
		VColorRef color = m_bIsSelected ? VAppMenuColors::GetColor(VAppMenuColors::COLOR_LIST_CONTROL_BG) : VAppMenuColors::GetColor(VAppMenuColors::COLOR_DIALOG_BG);
		const VSimpleRenderState_t iProperties = VGUIManager::DefaultGUIRenderState();

		Graphics.Renderer.DrawSolidQuad(rect.m_vMin, rect.m_vMax, color, iProperties);

		if (!m_bIsSelected)
		{
			const float fBorderWidth = VUISharedData::GetBorderWidth();
			color = VAppMenuColors::GetColor(VAppMenuColors::COLOR_LIST_CONTROL_BG);
			Graphics.Renderer.DrawSolidQuad(rect.m_vMin, hkvVec2(rect.m_vMax.x, rect.m_vMin.y + fBorderWidth), color, iProperties);
			Graphics.Renderer.DrawSolidQuad(hkvVec2(rect.m_vMin.x, rect.m_vMin.y), hkvVec2(rect.m_vMin.x + fBorderWidth, rect.m_vMax.y), color, iProperties);
			Graphics.Renderer.DrawSolidQuad(hkvVec2(rect.m_vMax.x, rect.m_vMin.y), hkvVec2(rect.m_vMax.x + fBorderWidth, rect.m_vMax.y), color, iProperties);
		}

		VPushButton::OnPaint(Graphics, parentState);
	}

	bool m_bIsSelected;
};


VUINodeTabDialog::VUINodeTabDialog(void)
{
	
}


VUINodeTabDialog::~VUINodeTabDialog(void)	
{	

}

VUINodeTabDialog* VUINodeTabDialog::CreateInstance( VUINodeTabDialog* pNewDialog )
{
	// CreateInstance
	if ( pNewDialog == NULL )
		pNewDialog = new VUINodeTabDialog();

	pNewDialog->m_pContext = VUINodeMananger_cl::GlobalManager().GetGUIContext();
	VUINodeMananger_cl::GlobalManager().GetGUIContext()->ShowDialog( pNewDialog );

	return pNewDialog;
}



void VUINodeTabDialog::OnPaint(VGraphicsInfo &Graphics, const VItemRenderInfo &parentState)
{
	if (!IsVisible())
		return;

	if (m_spCachedBuffer)
		if (m_spCachedBuffer->UpdateCache(this, Graphics, parentState))
			return; // finished rendering

	VSimpleRenderState_t state = VGUIManager::DefaultGUIRenderState();

	VItemRenderInfo thisState(parentState, this, m_fFadeInPos);

	// render multiplicative screen quad
	if (m_bFadeBack)
	{
		VRectanglef screenrect = GetContext()->GetClientRect();
		VSimpleRenderState_t multState = VGUIManager::DefaultGUIRenderState(VIS_TRANSP_MULTIPLICATIVE);
		VColorRef iColor;
		iColor.Lerp(V_RGBA_WHITE,m_iBackFadeColor,m_fBackFadePos);
		Graphics.Renderer.DrawSolidQuad(screenrect.m_vMin,screenrect.m_vMax,iColor,multState);
	}

	// constant background color
	if (m_iBackColor.a>0)
	{
		VRectanglef rect = GetClientRect();
		Graphics.Renderer.DrawSolidQuad(rect.m_vMin,rect.m_vMax,m_iBackColor,state);
	}

	// render dialog frame and background image
	if (m_pImage) m_pImage->OnPaint(Graphics,thisState);
	m_Frame.OnPaint(Graphics,thisState);

	// render all items in it and clip against the window rectangle (clipping against GetClientRect would cause the close button to disappear)
	//Graphics.ClippingStack.Push(GetBoundingBox(),true);
	m_Items.RenderAll(Graphics,thisState);	
}

bool VUINodeTabDialog::BuildToXML( VString szFileName )
{
	bool bWrite = true;

	TiXmlDocument doc;
	TiXmlElement root("root");
	TiXmlNode *pRoot = doc.InsertEndChild(root);
	TiXmlElement pDlgNode("DIALOG");
	TiXmlNode *pAdded = pRoot->InsertEndChild(pDlgNode);
	TiXmlElement *pNode = pAdded->ToElement();

	char szPath[FS_MAX_PATH];
	VFileHelper::GetFileDir( szFileName.GetSafeStr() , szPath );

	if (!VWindowBase::Build( pNode,szPath,bWrite))
		return false;

	// additional size/position properties
	XMLHelper::Exchange_Floats(pNode,"clientMin",m_vClientMin.data,2,bWrite);
	XMLHelper::Exchange_Floats(pNode,"clientMax",m_vClientMax.data,2,bWrite);
	bool bFullscreen = false;
	XMLHelper::Exchange_Bool(pNode,"fullscreen",bFullscreen,bWrite);
	if (bFullscreen) m_uiDialogFlags|=DIALOGFLAGS_FULLSCREEN;

	// start position
	const char *szStartPos = XMLHelper::Exchange_String(pNode,"startPosition",NULL,bWrite);
	if (szStartPos)
		m_eStartPos = GetStartPositionType(szStartPos);

	// build frame and background image
	TiXmlElement *pImageNode = XMLHelper::SubNode(pNode,"image",bWrite);
	if (pImageNode)
	{
		V_SAFE_DELETE(m_pImage);
		m_pImage = new VImageStates();
		m_pImage->Build(this, pImageNode, szPath, bWrite);
	}
	m_Frame.Build(XMLHelper::SubNode(pNode,"frame",bWrite), szPath, bWrite);

	// dlg fading
	TiXmlElement *pFadein = XMLHelper::SubNode(pNode,"fadein",bWrite);
	if (pFadein)
	{
		m_fFadeInPos = 0.f;
		m_fFadeInSpeed = 2.f;
		float fFadeTime = 0.f;
		XMLHelper::Exchange_Float(pFadein,"start",m_fFadeInPos,bWrite);
		XMLHelper::Exchange_Float(pFadein,"speed",m_fFadeInSpeed,bWrite);
		XMLHelper::Exchange_Float(pFadein,"time",fFadeTime,bWrite);// optional
		if (fFadeTime>0.f) 
			m_fFadeInSpeed = (1.f-m_fFadeInPos)/fFadeTime;
	}

	TiXmlElement *pFadeBack = XMLHelper::SubNode(pNode,"fadeback",bWrite);
	if (pFadeBack)
	{
		m_bFadeBack = true;
		float fFadeTime = 0.f;
		XMLHelper::Exchange_Float(pFadeBack,"start",m_fBackFadePos,bWrite);
		XMLHelper::Exchange_Float(pFadeBack,"speed",m_fBackFadeSpeed,bWrite);
		XMLHelper::Exchange_Color(pFadeBack,"color",m_iBackFadeColor,bWrite);
		XMLHelper::Exchange_Float(pFadeBack,"time",fFadeTime,bWrite);// optional
		if (fFadeTime>0.f) 
			m_fBackFadeSpeed = (1.f-m_fBackFadePos)/fFadeTime;
	}

	// constant dialog background color
	XMLHelper::Exchange_Color(pNode,"backgroundcolor",m_iBackColor,bWrite);


	// build all items ("control" nodes)
//	 m_Items.Build(this,pNode,szPath,bWrite);

	
	for( int i = 0 ; i< m_Items.Count() ; i++ )
	{
		VDlgControlBase *pItem = m_Items.GetAt(i);

		TiXmlElement pItemNode("control");
		TiXmlNode *pAdded = pNode->InsertEndChild(pItemNode);
		TiXmlElement *pItemNodeAdded = pAdded->ToElement();		


		VType *pType = pItem->GetTypeId();
		const char *szClass = pType->m_lpszClassName;

	

		const char *szClassName = XMLHelper::Exchange_String(pItemNodeAdded,"class", szClass ,bWrite);
	//	VASSERT(szClassName != NULL && szClassName[0]); // must be defined!

		if( pItem->m_pContext == NULL )
		{
			pItem->m_pContext = VUINodeMananger_cl::GlobalManager().GetGUIContext();
		}

		pItem->Build( pItemNodeAdded ,szPath,bWrite );
	}

	//VString szXMLFileName = szFileName + ".xml";

	bool bResult = doc.SaveFile( szFileName);

	return bResult;
}


void VUINodeTabDialog::BuildFromXML(VString szFileName)
{
	//	VUINodeMananger_cl::GlobalManager().

	TiXmlDocument doc;
	// 
	// 	char szPathName[FS_MAX_PATH];
	// 	const char *szFilename = GetFilename();
	// 	if (VFileHelper::GetExtensionPos(szFilename)<0)
	// 	{
	// 		VFileHelper::AddExtension(szPathName,szFilename,"XML");
	// 		szFilename = szPathName;
	// 	}

	bool bResult = doc.LoadFile(szFileName);
	if (!bResult)
	{
		//Vision::Error.Warning("GUI dialog resource '%s' failed to load : %s",szFileName.GetSafeStr(),doc.ErrorDesc());
		//	return FALSE;
	}

	TiXmlElement *pDlgNode = XMLHelper::SubNode(doc.RootElement(),"DIALOG",false);

	

	//this->Build( pDlgNode, Vision::File.GetAbsoluteDirectory() .GetDataDirectory(),false);
	this->Build( pDlgNode, "",false);


	this->m_bFadeBack = false;
	// 	if (!pDlgNode)
	// 		return FALSE;

}


void VUINodeTabDialog::SetFullScreen(bool bFullScreen)
{
	if ( bFullScreen == true)
		m_uiDialogFlags|=DIALOGFLAGS_FULLSCREEN;
	else
		m_uiDialogFlags &= ~(DIALOGFLAGS_FULLSCREEN);
}


void VUINodeTabDialog::OnHandleCallback(IVisCallbackDataObject_cl *pData)
{
	if(pData->m_pSender == &UIEngineCallbackManager_cl::GlobalManager().OnUIPropertyChanged)
	{
		UIPropertyDataBaseObject *pHavokData = (UIPropertyDataBaseObject*)pData;

		if ( pHavokData->m_szPropertyName == "PosX" || pHavokData->m_szPropertyName == "PosY" )
		{
			UIPropertyDataObject<float> *pHavokData = (UIPropertyDataObject<float>*)pData;

			float fValue = pHavokData->m_value;

			this->SetPosition(500,500);
		}
		
		m_uiDialogFlags = DIALOGFLAGS_NONE;
		return;
	}	
	

	return;
}


void VUINodeTabDialog::SetBackgroundTexture(VString filename)
{
	VTextureObject *pTex = NULL;
	pTex = Vision::TextureManager.Load2DTexture(filename);

		  if ( pTex!=NULL )
		  {
			  
			  V_SAFE_DELETE(m_pImage);
			  m_pImage = new VImageStates();
			 // m_pImage->Build(this, pImageNode, szPath, bWrite);			
			  // first, get the normal state from this node
			 // m_States[VWindowBase::NORMAL].Build(pOwner,pNode,szPath,bWrite);

			  // copy over to the other states...
			  m_pImage->SetTexture( pTex );			
		  }
}

void VUINodeTabDialog::SetBackgroundColor(VColorRef color)
{
	this->m_iBackColor = color;
}

void VUINodeTabDialog::OnItemClicked(VMenuEventDataObject *pEvent)
{
	VDialog::OnItemClicked( pEvent );

	if ( pEvent->m_pItem->IsOfType("VUINodePushButton")  )
	{
		VUINodePushButton *pItem = (VUINodePushButton*)pEvent->m_pItem;

		UIItemDataEventObject data(&UIEngineCallbackManager_cl::GlobalManager().OnUIEventRaised, pEvent );	
		UIEngineCallbackManager_cl::GlobalManager().OnUIEventRaised.TriggerCallbacks( &data );
	}	
	/*const int iCount = m_Components.Count();
	for (int i=0;i<iCount;i++)
	{
	if ( m_Components.GetAt(i)->IsOfType( V_RUNTIME_CLASS( UIEventHandlerComponent ) ) )
	{
	UIEventHandlerComponent
	}		
	}
	*/

	UIEventHandlerComponent *pUIEventHandlerComponent = static_cast<UIEventHandlerComponent*>(
		Components().GetComponentOfType(V_RUNTIME_CLASS(UIEventHandlerComponent)));

	if ( pUIEventHandlerComponent )
		pUIEventHandlerComponent->OnItemClicked(pEvent);

	return;
}

void VUINodeTabDialog::SetFadeIn(bool bFadeIn , float fFadeInPos , float fFadeInTime )
{
	if ( bFadeIn == false )
	{
		m_fFadeInPos = 1.0f;
		m_fFadeInSpeed = 0.0f;
	}
	else
	{
		m_fFadeInPos =  fFadeInPos;
		m_fFadeInSpeed = (1.f-fFadeInPos)/ fFadeInTime;
	}
	return;
}

void VUINodeTabDialog::SetFadeOut(bool bFadeOut , VColorRef iBackFadeColor , float fBackFadePos , float fFadeOutTime)
{
	m_bFadeBack = bFadeOut;
	m_iBackFadeColor = iBackFadeColor;
	m_fBackFadePos = fBackFadePos;
	m_fBackFadeSpeed = (1.f-fBackFadePos)/fFadeOutTime;
	return;
}

void VUINodeTabDialog::OnVariableValueChanged(VisVariable_cl *pVar, const char * value)
{
	VASSERT (pVar);
	VASSERT (value);

	if (pVar->category && strcmp(pVar->category, "m_vClientMin") == 0)
	{
	//	m_vClientMin = 	
	}
}

V_IMPLEMENT_SERIAL( VUINodeTabDialog, VDialog, 0, &g_NodeModule );
void VUINodeTabDialog::Serialize( VArchive &ar )
{
	VDialog::Serialize(ar);
	char iLocalVersion = 0;
	if (ar.IsLoading())
	{
		ar >> m_bVisible;;		
// 		ar >> iLocalVersion; VASSERT_MSG(iLocalVersion==0,"Invalid version");
// 		ar >> m_uiDialogFlags >> (int &)m_eStartPos;
// 		ar >> m_fFadeInPos >> m_fFadeInSpeed >> m_iBackColor >> m_bFadeBack;
// 		ar >> m_iBackFadeColor >> m_fBackFadePos >> m_fBackFadeSpeed;
// 
// 		V_SAFE_DELETE(m_pImage);
// 		bool bHasImg = false;
// 		ar >> bHasImg;
// 		if (bHasImg)
// 		{
// 			m_pImage = new VImageStates();
// 			ar >> *m_pImage;
// 		}
// 		ar >> m_Frame;
// 
// 		// item list:
// 		m_Items.SerializeX(ar);
	} else
	{
		ar << m_bVisible;	
// 		ar << iLocalVersion;
// 		ar << m_uiDialogFlags << (int)m_eStartPos;
// 		ar << m_fFadeInPos << m_fFadeInSpeed << m_iBackColor << m_bFadeBack;
// 		ar << m_iBackFadeColor << m_fBackFadePos << m_fBackFadeSpeed;
// 
// 		ar << (bool)(m_pImage!=NULL);
// 		if (m_pImage)
// 		{
// 			ar << *m_pImage;
// 		}
// 		ar << m_Frame;
// 
// 		// item list:
// 		m_Items.SerializeX(ar);

	}
}

void VUINodeTabDialog::OnSerialized( VArchive &ar )
{
	CommonInit();

}

void VUINodeTabDialog::CommonInit()
{
	SetVisible(m_bVisible);
}
// 
// void VUINodeTabDialog::OnDeserializationCallback(const VSerializationContext &context)
// {
// }
// 
// VMenuItemCollection m_Items;
// VDialogFrame m_Frame;
// VImageStates *m_pImage;
// float m_fFadeInPos, m_fFadeInSpeed;
// hkvVec2 m_vClientMin, m_vClientMax;
// 
// // const background color
// VColorRef m_iBackColor;
// 
// bool m_bFadeBack;
// VColorRef m_iBackFadeColor;
// float m_fBackFadePos, m_fBackFadeSpeed;

//#define DEFINE_VAR_VECTOR2_FLOAT(klass,the_var_name,desc,def_value,hints,editorhints)  DEFINE_VAR(klass,VULPTYPE_FLOAT_VECTOR2,the_var_name,desc,def_value,hints,editorhints)

// *** The variable table is used by the plugin system of the engine and editor. In that case we do not have
// *** variables which have to be initialised or which have to be available in the editor.
START_VAR_TABLE(VUINodeTabDialog, VisTypedEngineObject_cl,"animated entity", VVARIABLELIST_FLAGS_NONE, NULL );
	DEFINE_VAR_COLORREF(VUINodeTabDialog, m_iBackFadeColor, "Fadeback Color", "255/255/255/255", 0, 0);	
	DEFINE_VAR_COLORREF(VUINodeTabDialog, m_iBackColor, "Sun color", "128/128/128/255", 0, 0);
//	DEFINE_VAR_BOOL(VUINodeTabDialog, m_bFadeBack, "", "TRUE", 0, 0);	
	DEFINE_VAR_INT_AND_NAME(VUINodeTabDialog, m_uiDialogFlags, "DialogFlags" ,  "Dialog Flag", "0", 0, "CSharpType(DialogFlags_e)");
	DEFINE_VAR_INT(VUINodeTabDialog, m_eStartPos, "Dialog Flag", "0", 0, "CSharpType(StartPositionType_e)");
	DEFINE_VAR_FLOAT(VUINodeTabDialog, m_fFadeInPos, "", "1.0", 0, 0);	
	DEFINE_VAR_FLOAT(VUINodeTabDialog, m_fFadeInSpeed, "", "0.0", 0, 0);	

	DEFINE_VAR_FLOAT(VUINodeTabDialog, m_fBackFadePos, "", "1.0", 0, 0);	
	DEFINE_VAR_FLOAT(VUINodeTabDialog, m_fBackFadeSpeed, "", "0.0", 0, 0);

//	DEFINE_VAR_VECTOR2_FLOAT(VUINodeTabDialog, m_vClientMin, "Dialog Flag", "6.0/6.0", 0, 0);
// 	DEFINE_CATEGORY (VUINodeTabDialog, "ImageState")
//	varList->Add (VisVariable_cl::NewVariable ("m_vClientMin", "m_vClientMin", VULPTYPE_FLOAT_VECTOR2, -1, "\\flare12.dds", (VDisplayHintFlags_e)DISPLAY_HINT_NONE, 0, 0)  );	
// 	varList->Add (VisVariable_cl::NewVariable ("FlareTexture[0]", "Filename", VULPTYPE_VSTRING, -1, "\\flare12.dds", (VDisplayHintFlags_e)DISPLAY_HINT_TEXTUREFILE, 0, 0)  );	
END_VAR_TABLE