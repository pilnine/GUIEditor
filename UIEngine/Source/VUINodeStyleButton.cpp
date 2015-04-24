#include "VUINodeStyleButton.h"
#include "VUINodeExportHelperXML.h"



V_IMPLEMENT_SERIAL( VUINodeStyleButton, VPushButton, 0, &g_NodeModule );

VUINodeStyleButton::VUINodeStyleButton(void)
{
	
}


VUINodeStyleButton::~VUINodeStyleButton(void)
{
}

VUINodeStyleButton* VUINodeStyleButton::CreateInstance()
{
	// CreateInstance
	VUINodeStyleButton* pNewPushButton = new VUINodeStyleButton();

	pNewPushButton->SetSize(200, 100 );
	pNewPushButton->SetPosition( 0,0); 
	pNewPushButton->SetEnabled(true);

	VTextureObject* spTexture = Vision::TextureManager.GetPlainWhiteTexture();
	pNewPushButton->Image().SetTexture( spTexture );

	pNewPushButton->m_pContext = VUINodeMananger_cl::GlobalManager()->GetGUIContext();

	pNewPushButton->Text().SetVerticalAlignment(VisFont_cl::ALIGN_CENTER);
	pNewPushButton->Text().SetHorizontalAlignment(VisFont_cl::ALIGN_CENTER);	

	return pNewPushButton;
}


void VUINodeStyleButton::Serialize( VArchive &ar )
{
	VPushButton::Serialize(ar);
	
	char iLocalVersion = 0;
	if (ar.IsLoading())
	{	
		//ar >> m_buttonID;

	}
	else
	{	
		//ar << m_buttonID;
	}
}



bool VUINodeStyleButton::Build(TiXmlElement *pNode, const char *szPath, bool bWrite) 
{
	if ( bWrite == false )
	{		
		return VPushButton::Build( pNode,szPath,bWrite);
	}

	if ( GetUniqueID() != 0 )
	{
		VString szID = VGUIManager::GetIDName( (int)GetUniqueID() );	
		XMLHelper::Exchange_String(pNode,"ID",szID,bWrite);
	}		

	XMLHelper::Exchange_Floats(pNode,"pos",m_vPosition.data,2,bWrite);
	XMLHelper::Exchange_Floats(pNode,"size",m_vSize.data,2,bWrite);

//	if (!VDlgControlBase::Build(pNode,szPath,bWrite))
//		return false;


	VUINodeExportHelperXML::GlobalManager().BuildImageState( &m_ButtonCfg , this , XMLHelper::SubNode(pNode,"image",bWrite) , szPath,bWrite );
	VUINodeExportHelperXML::GlobalManager().BuildTextState( &m_TextCfg , this , XMLHelper::SubNode(pNode,"text",bWrite) , szPath,bWrite );

//	m_ButtonCfg.Build(this,XMLHelper::SubNode(pNode,"image",bWrite),szPath,bWrite);
//	m_TextCfg.Build(this,XMLHelper::SubNode(pNode,"text",bWrite),szPath,bWrite);

	// size from texture
	if (m_vSize.x<=0.f)
		m_vSize = m_ButtonCfg.m_States[VDlgControlBase::NORMAL].GetSize();

	// ..or from text
	if (m_vSize.x<=0.f)
		m_vSize = m_TextCfg.m_States[VDlgControlBase::NORMAL].GetSize();

	return true;
}




void VUINodeStyleButton::OnVariableValueChanged(VisVariable_cl *pVar, const char * value)
{
	VASSERT (pVar);
	VASSERT (value);

	/// Texture Button
	if (pVar->name && strstr(pVar->name, "Texture") != 0)
	{
		VTextureObject* spTexture = Vision::TextureManager.Load2DTexture( value );

		if ( pVar->name && strstr(pVar->name, "DISABLED") != 0 )
		{
			Image().m_States[VWindowBase::DISABLED].SetTexture( spTexture );
		}
		else if ( pVar->name && strstr(pVar->name, "NORMAL" ) != 0 )
		{
				Image().m_States[VWindowBase::NORMAL].SetTexture( spTexture );
		}
		else if ( pVar->name && strstr(pVar->name, "MOUSEOVER" ) != 0 )
		{
			Image().m_States[VWindowBase::MOUSEOVER].SetTexture( spTexture );
		}
		else if ( pVar->name && strstr(pVar->name, "SELECTED" ) != 0 )
		{
			Image().m_States[VWindowBase::SELECTED].SetTexture( spTexture );
		}		
	}		
	else if (pVar->name && strcmp(pVar->name, "m_eTransp") == 0)
	{		
		sscanf(value,"%d",&m_eTransp);
		Image().SetTransparency(m_eTransp);
	}
	else if (pVar->name && strcmp(pVar->name, "Text") == 0)
	{
		int iComponents[4];	
		Text().SetText( value );			
		return;
	}
	else if (pVar->name && strcmp(pVar->name, "Scale") == 0)
	{
		float fScale = 1.0f;
		int iCount = sscanf(value,"%f",&fScale);		
		Text().SetScaling( fScale );
		return;
	}
	else if (pVar->name && strcmp(pVar->name, "Font") == 0)
	{
		VisFontPtr spFont = VGUIManager::GlobalManager().LoadFont( value );		
		Text().SetFont( spFont );
		return;
	}
	else if (pVar->name && strcmp(pVar->name, "AlignmentVertical") == 0)
	{
		int iValue = 1;
		sscanf(value,"%d",&iValue);		
		Text().SetVerticalAlignment( (VisFont_cl::Alignment_e)iValue  );
	}
	else if (pVar->name && strcmp(pVar->name, "AlignmentHorizontal") == 0)
	{
		int iValue = 1;
		sscanf(value,"%d",&iValue);		
		Text().SetHorizontalAlignment( (VisFont_cl::Alignment_e)iValue  );
	}
}

// *** The variable table is used by the plugin system of the engine and editor. In that case we do not have
// *** variables which have to be initialised or which have to be available in the editor.
START_VAR_TABLE(VUINodeStyleButton, VisTypedEngineObject_cl,"animated entity", VVARIABLELIST_FLAGS_NONE, NULL );
varList->Add (VisVariable_cl::NewVariable ("Texture", "Filename", VULPTYPE_VSTRING, -1, "", (VDisplayHintFlags_e)DISPLAY_HINT_TEXTUREFILE, 0, 0)  );
DEFINE_VAR_INT_AND_NAME(VUINodeStyleButton, m_eTransp, "Transparency" ,  "Transparency", "VIS_TRANSP_ALPHATEST", 0, "CSharpType(TransparencyType)");

varList->Add (VisVariable_cl::NewVariable ("Color DISABLED", "Filename", VULPTYPE_BYTE_COLOR4, -1, "128,128,128,255", (VDisplayHintFlags_e)DISPLAY_HINT_COLOR, 0, 0)  );
varList->Add (VisVariable_cl::NewVariable ("Color NORMAL", "Filename", VULPTYPE_BYTE_COLOR4, -1, "255,255,255,255", (VDisplayHintFlags_e)DISPLAY_HINT_COLOR, 0, 0)  );
varList->Add (VisVariable_cl::NewVariable ("Color SELECTED", "Filename", VULPTYPE_BYTE_COLOR4, -1, "177,52,30,255", (VDisplayHintFlags_e)DISPLAY_HINT_COLOR, 0, 0)  );

varList->Add (VisVariable_cl::NewVariable ("Text", "Text", VULPTYPE_VSTRING, -1, "TEXT_LABEL", (VDisplayHintFlags_e)DISPLAY_HINT_NONE, 0, 0)  );
varList->Add (VisVariable_cl::NewVariable ("TextColor", "TextColor", VULPTYPE_BYTE_COLOR4, -1, "0,0,0,255", (VDisplayHintFlags_e)DISPLAY_HINT_COLOR, 0, 0)  );
varList->Add (VisVariable_cl::NewVariable ("Scale", "TextScale", VULPTYPE_FLOAT, -1, "2.0", (VDisplayHintFlags_e)DISPLAY_HINT_NONE, 0, 0)  );	
varList->Add (VisVariable_cl::NewVariable ("Font", "Font", VULPTYPE_VSTRING, -1, "Fonts\\NanumGothic.fnt", (VDisplayHintFlags_e)DISPLAY_HINT_CUSTOMFILE, 0, 0)  );

varList->Add (VisVariable_cl::NewVariable ("AlignmentVertical", "AlignmentVertical", VULPTYPE_INT, -1, "ALIGN_CENTER", DISPLAY_HINT_NONE, "CSharpType(AlignmentVertical)", 0)  );
varList->Add (VisVariable_cl::NewVariable ("AlignmentHorizontal", "AlignmentHorizontal", VULPTYPE_INT, -1, "ALIGN_CENTER", DISPLAY_HINT_NONE, "CSharpType(AlignmentHorizontal)", 0)  );


END_VAR_TABLE
