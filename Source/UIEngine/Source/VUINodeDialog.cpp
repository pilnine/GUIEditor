#include "VUINodeDialog.h"
#include "UIEngineCallbacks.h"

#include <Vision/Runtime/Base/VBase.hpp>
#include "VUINodePushButton.h"

#include "UIEventHandlerComponent.h"


V_IMPLEMENT_SERIAL( VUINodeDialog, VDialog, 0, &g_NodeModule );

VUINodeDialog::VUINodeDialog(void)
{
	this->m_pContext = VUINodeMananger_cl::GlobalManager()->GetGUIContext();	
	
	m_bFadeOut = false;
	m_bFadeOutColor = VColorRef(0,0,0,255); // BLACK
	m_fFadeOutPos = 0.0f; 
	m_fFadeOutSpeed = 0.5f; 
	m_nParam = -1;
	m_iStartDirection = 0;

	m_fFadeTime = 0.15f;
	m_fPositionOffsetX = 0;
	m_fPositionOffsetY = 0;
}


VUINodeDialog::~VUINodeDialog(void)	
{	
//	UIEngineCallbackManager_cl::GlobalManager().OnUIPropertyChanged -= this;
}

VUINodeDialog* VUINodeDialog::CreateInstance( VUINodeDialog* pNewDialog )
{
	// CreateInstance
	if ( pNewDialog == NULL )
		pNewDialog = new VUINodeDialog();

	//VDialogResource *pRes = VUINodeMananger_cl::GlobalManager()->GetGUIContext()->GetManager()->LoadDialog("gui/MainMenu_Win32.xml");

	pNewDialog->m_pContext = VUINodeMananger_cl::GlobalManager()->GetGUIContext();

//	pNewDialog->InitDialog(VUINodeMananger_cl::GlobalManager()->GetGUIContext(),pRes , NULL, DIALOGFLAGS_NONE );
//	pNewDialog->m_iBackColor = VColorRef(255,0,0,255);
//	pNewDialog->SetSize(500,500);
//	pNewDialog->SetVisible(true);
	VUINodeMananger_cl::GlobalManager()->GetGUIContext()->ShowDialog( pNewDialog );

	//VUINodeMananger_cl::GlobalManager()->GetGUIContext()->ShowDialog( "gui/MainMenu_Win32.xml" ); //  = true;	
	return pNewDialog;
}

void VUINodeDialog::DoLayout()
{
	DoFadeIn();

	return;
}


void VUINodeDialog::AddControl(VDlgControlBase *pItem)
{
	VDialog::AddControl(pItem );	
	m_Items.SortByPriority();
}

void VUINodeDialog::OnPaint(VGraphicsInfo &Graphics, const VItemRenderInfo &parentState)
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

		// Apply GUI Context Scale
	VRectanglef rect = GetBoundingBox();
	rect *= VUINodeMananger_cl::GlobalManager()->GetGUIContext()->GetScale();
	Graphics.ClippingStack.Push(rect,true);	

	m_Items.RenderAll(Graphics,thisState);
	Graphics.ClippingStack.Pop();
}


void VUINodeDialog::Reset()
{
	//SetPosition(0.0f, m_fOffsetY);
	m_fTimer = 0.0f;
	m_state = STATE_NONE;
	m_followUpState = STATE_NONE;

	IVGUIContext *pContext = GetContext();
	if (pContext->GetOpenDialogs().Contains(this))
	{
		pContext->CloseDialog(this);
	}

	SetVisible(false);

	if (VInputMap::AreInputMapsLocked())
		VInputMap::LockInputMaps(false);
}



void VUINodeDialog::OnTick(float dtime)
{
	if (!IsVisible())
		return;

	VDialog::OnTick( dtime );

	if (m_bFadeOut)
	{
		m_fFadeOutPos += m_fFadeOutSpeed*dtime;
		if (m_fFadeOutPos>1.f)
		{
			m_fFadeOutPos = 1.f;
			m_OnFadeoutFinished.TriggerCallbacks();
		}
	}	

// 
// 	if ( m_iStartDirection != 0 )
// 	{	
// 		hkvVec2 vPos = GetPosition();
// 
// 		vPos.x += 100.0f;
// 
// 		if ( vPos.x > m_vOriginalPos.x)
// 		{
// 			vPos.x = m_vOriginalPos.x;
// 			m_iStartDirection = 0;			
// 		}
// 		SetPosition( vPos.x , vPos.y );	
// 	}

	if (m_state == STATE_NONE)
	{		
		return;
	}

	// Update animation if there is one in progress
	if (m_fTimer > 0.0f)
	{
		m_fTimer -= dtime;

		const float fX = hkvMath::Min(1.0f, 1.0f - m_fTimer / m_fFadeTime);
		float fDelta = hkvMath::sinRad(hkvMath::pi()*fX - hkvMath::pi()*0.5f)*0.5f + 0.5f;
		if (m_state == STATE_FADE_IN)
			SetPosition((fDelta - 1.0f)*GetSize().x + m_fPositionOffsetX, m_fPositionOffsetY);
		else
			SetPosition(-fDelta*GetSize().x, 0);
	}
	else
	{
 		if ( m_state == STATE_FADE_OUT )
 		{
			SetVisible(false);
 	//		VUINodeMananger_cl::GlobalManager()->GetGUIContext()->CloseDialog( this );
 		}

		m_fTimer = 0.0f;
		m_state = STATE_NONE;
	}
}

void VUINodeDialog::DoFadeIn()
{
// 	if (m_state != STATE_NONE)
// 		return;

	SetPosition(-GetSize().x, 0);
	m_fTimer = 0.15f; //s_fFadeTime; // OnDoLayout 에서 셋팅되서 넘어옴
	m_state = STATE_FADE_IN;
	m_followUpState = STATE_NONE;
}

void VUINodeDialog::DoFadeOut(const VDialogState& followUpState)
{
//	if (m_state != STATE_NONE)
//		return;

//	SetPosition(0.0f, m_fOffsetY);
	m_fTimer = 0.15f;//s_fFadeTime;
	m_state = STATE_FADE_OUT;
	m_followUpState = followUpState;
}


bool VUINodeDialog::BuildToXML( VString szFileName )
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

// 	if (!VWindowBase::Build( pNode,szPath,bWrite))
// 		return false;


	//////////////////////////////////////////////////////////////////////////
	// VWindowBase::Build
	//////////////////////////////////////////////////////////////////////////

	// register the ID
	//XMLHelper::Exchange_String(pNode,"ID",m_szID,bWrite);

	if ( GetUniqueID() != 0 )
	{
		VString szID = VGUIManager::GetIDName( (int)GetUniqueID() );	
		XMLHelper::Exchange_String(pNode,"ID",szID,bWrite);
	}		
	
	// size and position
//	XMLHelper::Exchange_Floats(pNode,"pos",m_vPosition.data,2,bWrite);
	XMLHelper::Exchange_Floats(pNode,"size",m_vSize.data,2,bWrite);

	// client border
// 	float b[4];
// 	int iCount = XMLHelper::Exchange_Floats(pNode,"clientborder",b,4,bWrite);
// 	switch (iCount)
// 	{
// 	case 0:m_ClientAreaBorder.Set(0,0,0,0);break;
// 	case 1:m_ClientAreaBorder.Set(b[0],b[0],b[0],b[0]);break;
// 	case 2:m_ClientAreaBorder.Set(b[0],b[1],b[0],b[1]);break;
// 	case 3:m_ClientAreaBorder.Set(b[0],b[1],b[2],b[1]);break;
// 	case 4:m_ClientAreaBorder.Set(b[0],b[1],b[2],b[3]);break;
// 	}

	// flags
// 	bool bEnabled = true, bVisible = true, bSelected = false;
// 	bool bCaching = m_spCachedBuffer!=NULL;
// 
// 	XMLHelper::Exchange_Bool(pNode,"enabled", bEnabled, bWrite);    SetStatus(ITEMSTATUS_ENABLED,bEnabled);
// 	XMLHelper::Exchange_Bool(pNode,"visible", bVisible, bWrite);    SetStatus(ITEMSTATUS_VISIBLE,bVisible);
// 	XMLHelper::Exchange_Bool(pNode,"selected", bSelected, bWrite);  SetStatus(ITEMSTATUS_SELECTED,bSelected);
// 	XMLHelper::Exchange_Bool(pNode,"usecaching", bCaching, bWrite);
	// tooltip
	const char *szUTF8 = XMLHelper::Exchange_String(pNode,"tooltiptext", NULL, bWrite);
	const char *szText = GetMenuManager()->TranslateString(this, szUTF8);
	SetTooltipText(szText);

	// scripting
// 	IVScriptManager *pScriptMan = Vision::GetScriptManager();
// 	TiXmlElement *pScriptNode = XMLHelper::SubNode(pNode,"script",bWrite);
// 	if (pScriptNode && pScriptMan)
// 	{
// 		char szPathname[FS_MAX_PATH];
// 		const char *szFilename = XMLHelper::Exchange_String(pScriptNode,"filename", NULL, bWrite);
// 		if (szFilename != NULL && szFilename[0])
// 		{
// 			VFileHelper::CombineDirAndFile(szPathname,szPath,szFilename);
// 			IVScriptInstance *pScriptObj = pScriptMan->CreateScriptInstanceFromFile(szPathname);
// 			if (pScriptObj)
// 				pScriptMan->SetScriptInstance(this, pScriptObj);
// 		}
// 	}

//	SetUseCaching(bCaching);

	//////////////////////////////////////////////////////////////////////////
	// VWindowBase::Build
	//////////////////////////////////////////////////////////////////////////

	// additional size/position properties
// 	XMLHelper::Exchange_Floats(pNode,"clientMin",m_vClientMin.data,2,bWrite);
// 	XMLHelper::Exchange_Floats(pNode,"clientMax",m_vClientMax.data,2,bWrite);
// 	bool bFullscreen = false;
// 	XMLHelper::Exchange_Bool(pNode,"fullscreen",bFullscreen,bWrite);
// 	if (bFullscreen) m_uiDialogFlags|=DIALOGFLAGS_FULLSCREEN;

	VString szStartPos;	
	if ( m_eStartPos == ScreenCenter )
		szStartPos = "ScreenCenter";
	else if ( m_eStartPos == ParentPos )
		szStartPos = "ParentPos";
	else if ( m_eStartPos == ParentCenter )
		szStartPos = "ParentCenter";
	else 
		szStartPos = "AbsolutePos";	

	// start position
 	XMLHelper::Exchange_String(pNode,"startPosition",szStartPos,bWrite); 	

	if ( m_pImage != NULL )
		VUINodeExportHelperXML::GlobalManager().BuildImageState( m_pImage , this , XMLHelper::SubNode(pNode,"image",bWrite) , szPath,bWrite );


	// frame texture name
	//(VGUIManager::LoadTexture(XMLHelper::Exchange_String(pNode,"texture",NULL,bWrite),szPath));
	
	if ( m_Frame.GetTexture() != NULL )
	{
		TiXmlElement *pFrame = XMLHelper::SubNode(pNode,"frame",bWrite);
		if (pFrame)
		{
			XMLHelper::Exchange_String(pFrame,"texture",m_Frame.GetTexture()->GetFilename(),bWrite);

			VColorRef color = m_Frame.GetColor();
			XMLHelper::Exchange_Color(pFrame, "color", color, bWrite);
		}
	}
	//m_Frame.Build(XMLHelper::SubNode(pNode,"frame",bWrite), szPath, bWrite);

	// dlg fading
	if ( m_fFadeInSpeed != 0)
	{
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
	}

	if ( m_bFadeBack )
	{
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
			pItem->m_pContext = VUINodeMananger_cl::GlobalManager()->GetGUIContext();
		}

		pItem->Build( pItemNodeAdded ,szPath,bWrite );
	}

	//VString szXMLFileName = szFileName + ".xml";

	bool bResult = doc.SaveFile( szFileName);

	return bResult;
}


void VUINodeDialog::BuildFromXML(VString szFileName)
{
	//	VUINodeMananger_cl::GlobalManager()->

	m_pContext = VUINodeMananger_cl::GlobalManager()->GetGUIContext();

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
	if (bResult == false)
	{
		//hkvLog::Error("No GUI File - %s!", szFileName);
		return ;
	}

	TiXmlElement *pDlgNode = XMLHelper::SubNode(doc.RootElement(),"DIALOG",false);

	

	//this->Build( pDlgNode, Vision::File.GetAbsoluteDirectory() .GetDataDirectory(),false);
	this->Build( pDlgNode, "",false);

	this->m_bFadeOut = false;
	this->m_bFadeBack = false;
	// 	if (!pDlgNode)
	// 		return FALSE;

	SetStartPosition();


}

void VUINodeDialog::SetStartPosition()
{
	float fDeltaX;
	float fDeltaY;

	switch (m_eStartPos)
	{
	case ScreenCenter:
		{
			VDefaultGUIContext *pContext = (VDefaultGUIContext*)GetContext();
			VRectanglef rect = pContext->GetClientRect();

			float fX = rect.GetSizeX();
			float fY = rect.GetSizeY();

			hkvVec2 center = (hkvVec2(fX,fY) - GetSize())*0.5f + GetPosition();
			if (m_pOwner) 
				center -= m_pOwner->GetAbsPosition();

			//SetPosition( center.x , center.y );
			m_fPositionOffsetX = center.x;
			m_fPositionOffsetY = center.y;
		}
		break;
	case ParentPos:
		{
			if ( m_pOwner != NULL )
			{
				m_fPositionOffsetX = m_pOwner->GetAbsPosition().x;
				m_fPositionOffsetY = m_pOwner->GetAbsPosition().y;					
			}
		}
		break;
	case ParentCenter:
		{

			m_fPositionOffsetX = 0;
			m_fPositionOffsetY = 0;
		}
		break;
	}	

// 	if ( m_iStartDirection != 0 )
// 	{
// 		float fX = GetSize().x;
// 		m_vOriginalPos = GetPosition();
// 		m_vTempPos = hkvVec2(-fX , m_vOriginalPos.y);
// 
// 		SetPosition( m_vTempPos.x + fDeltaX , m_vTempPos.y + fDeltaY);
// 
// 		//	float m_fMovingTime; 
// 	}
}


void VUINodeDialog::SetFullScreen(bool bFullScreen)
{
	if ( bFullScreen == true)
		m_uiDialogFlags|=DIALOGFLAGS_FULLSCREEN;
	else
		m_uiDialogFlags &= ~(DIALOGFLAGS_FULLSCREEN);
}


void VUINodeDialog::OnHandleCallback(IVisCallbackDataObject_cl *pData)
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


void VUINodeDialog::SetBackgroundTexture(VString filename)
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

void VUINodeDialog::SetBackgroundColor(VColorRef color)
{
	this->m_iBackColor = color;
}

void VUINodeDialog::OnItemClicked(VMenuEventDataObject *pEvent)
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

void VUINodeDialog::SetFadeIn(bool bFadeIn , float fFadeInPos , float fFadeInTime )
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

void VUINodeDialog::SetFadeOut(bool bFadeOut , VColorRef iBackFadeColor , float fBackFadePos , float fFadeOutTime)
{
	m_bFadeBack = bFadeOut;
	m_iBackFadeColor = iBackFadeColor;
	m_fBackFadePos = fBackFadePos;
	m_fBackFadeSpeed = (1.f-fBackFadePos)/fFadeOutTime;
	return;
}

void VUINodeDialog::OnVariableValueChanged(VisVariable_cl *pVar, const char * value)
{
	VASSERT (pVar);
	VASSERT (value);

	if (pVar->category && strcmp(pVar->category, "m_vClientMin") == 0)
	{
	//	m_vClientMin = 	
	}
}

void VUINodeDialog::Serialize( VArchive &ar )
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

void VUINodeDialog::OnSerialized( VArchive &ar )
{
	CommonInit();
}

void VUINodeDialog::CommonInit()
{
	SetVisible(m_bVisible);
}

VDlgControlBase* VUINodeDialog::GetControl(VString szControlID)
{
	for( int i = 0 ;i < m_Items.Count() ; i++ )
	{
		VDlgControlBase* control = Items().GetAt(i);
		VString szID = VGUIManager::GetIDName( control->GetID() );	
		if ( szID == szControlID )
			return control;
	}
	return NULL;
}

// 
// void VUINodeDialog::OnDeserializationCallback(const VSerializationContext &context)
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
START_VAR_TABLE(VUINodeDialog, VisTypedEngineObject_cl,"animated entity", VVARIABLELIST_FLAGS_NONE, NULL );
	DEFINE_VAR_COLORREF(VUINodeDialog, m_iBackFadeColor, "Fadeback Color", "255/255/255/255", 0, 0);	
	DEFINE_VAR_COLORREF(VUINodeDialog, m_iBackColor, "Sun color", "128/128/128/255", 0, 0);
//	DEFINE_VAR_BOOL(VUINodeDialog, m_bFadeBack, "", "TRUE", 0, 0);	
	DEFINE_VAR_INT_AND_NAME(VUINodeDialog, m_uiDialogFlags, "DialogFlags" ,  "Dialog Flag", "0", 0, "CSharpType(DialogFlags)");
	DEFINE_VAR_INT(VUINodeDialog, m_eStartPos, "Dialog Flag", "0", 0, "CSharpType(StartPositionType)");
	DEFINE_VAR_FLOAT(VUINodeDialog, m_fFadeInPos, "", "1.0", 0, 0);	
	DEFINE_VAR_FLOAT(VUINodeDialog, m_fFadeInSpeed, "", "0.0", 0, 0);	

	DEFINE_VAR_FLOAT(VUINodeDialog, m_fBackFadePos, "", "1.0", 0, 0);	
	DEFINE_VAR_FLOAT(VUINodeDialog, m_fBackFadeSpeed, "", "0.0", 0, 0);

//	DEFINE_VAR_VECTOR2_FLOAT(VUINodeDialog, m_vClientMin, "Dialog Flag", "6.0/6.0", 0, 0);
// 	DEFINE_CATEGORY (VUINodeDialog, "ImageState")
//	varList->Add (VisVariable_cl::NewVariable ("m_vClientMin", "m_vClientMin", VULPTYPE_FLOAT_VECTOR2, -1, "\\flare12.dds", (VDisplayHintFlags_e)DISPLAY_HINT_NONE, 0, 0)  );	
// 	varList->Add (VisVariable_cl::NewVariable ("FlareTexture[0]", "Filename", VULPTYPE_VSTRING, -1, "\\flare12.dds", (VDisplayHintFlags_e)DISPLAY_HINT_TEXTUREFILE, 0, 0)  );	
END_VAR_TABLE