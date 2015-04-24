#include "VUINodeCheckBox.h"


V_IMPLEMENT_SERIAL( VUINodeCheckBox, VCheckBox, 0, &g_NodeModule );

VUINodeCheckBox::VUINodeCheckBox(void)
{
	Image().SetStretchMode(VImageState::STRETCHED);
}


VUINodeCheckBox::~VUINodeCheckBox(void)
{
}


VUINodeCheckBox* VUINodeCheckBox::CreateInstance()
{
	// CreateInstance
	VUINodeCheckBox* pNewImage = new VUINodeCheckBox();

	VisFontPtr spFont = VGUIManager::GlobalManager().GetDefaultFont();
	pNewImage->Text().SetFont(spFont );
	pNewImage->Text().SetScaling(2);
	pNewImage->Text().SetText( "CHECK_BOX" );

	VTextureObject* spTexture = Vision::TextureManager.GetPlainWhiteTexture();
	pNewImage->Image().SetTexture( spTexture );
	pNewImage->Image().SetStretchMode(VImageState::STRETCHED);

	return pNewImage;
}


void VUINodeCheckBox::OnVariableValueChanged(VisVariable_cl *pVar, const char * value)
{
	VASSERT (pVar);
	VASSERT (value);

	/// Texture Button
	if (pVar->name && strstr(pVar->name, "Texture") != 0)
	{
		VTextureObject* spTexture = Vision::TextureManager.Load2DTexture( value );		

		if ( pVar->name && strstr(pVar->name, "ON") != 0 )
		{
			Image().m_States[VWindowBase::SELECTED].SetTexture( spTexture );			
		}
		else if ( pVar->name && strstr(pVar->name, "OFF" ) != 0 )
		{			
			Image().m_States[VWindowBase::NORMAL].SetTexture( spTexture );
			Image().m_States[VWindowBase::MOUSEOVER].SetTexture( spTexture );
		}
		else
		{
			Image().SetTexture( spTexture );
		}
	}	
	else if (pVar->name && strcmp(pVar->name, "TextColor") == 0)
	{
		int iComponents[4];
		int iCount = sscanf(value,"%i/%i/%i/%i",&iComponents[0],&iComponents[1],&iComponents[2],&iComponents[3]);

		VColorRef color(iComponents[0],iComponents[1],iComponents[2],iComponents[3]);
		Text().SetColor( color );
		return;
	}
	else if (pVar->name && strcmp(pVar->name, "Text") == 0)
	{		
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
	else if (pVar->name && strcmp(pVar->name, "TextColor") == 0)
	{
		int iComponents[4];
		int iCount = sscanf(value,"%i/%i/%i/%i",&iComponents[0],&iComponents[1],&iComponents[2],&iComponents[3]);

		VColorRef color(iComponents[0],iComponents[1],iComponents[2],iComponents[3]);
		Text().SetColor( color );
		return;
	}

}

// toggle check status
void VUINodeCheckBox::OnClick(VMenuEventDataObject *pEvent)
{
	VCheckBox::OnClick(pEvent);	
}



void VUINodeCheckBox::Serialize( VArchive &ar )
{
	VCheckBox::Serialize(ar);
	char iLocalVersion = 0;
	if (ar.IsLoading())
	{
	
	} else
	{

	}
}



bool VUINodeCheckBox::Build(TiXmlElement *pNode, const char *szPath, bool bWrite) 
{
	if ( bWrite == false )
	{		
		return VCheckBox::Build( pNode,szPath,bWrite);
	}

	if ( GetUniqueID() != 0 )
	{
		VString szID = VGUIManager::GetIDName( (int)GetUniqueID() );	
		XMLHelper::Exchange_String(pNode,"ID",szID,bWrite);
	}

	XMLHelper::Exchange_Floats(pNode,"pos",m_vPosition.data,2,bWrite);
	XMLHelper::Exchange_Floats(pNode,"size",m_vSize.data,2,bWrite);

	VUINodeExportHelperXML::GlobalManager().BuildImageState( &m_ButtonCfg , this , XMLHelper::SubNode(pNode,"image",bWrite) , szPath,bWrite );
	VUINodeExportHelperXML::GlobalManager().BuildTextState( &m_TextCfg , this , XMLHelper::SubNode(pNode,"text",bWrite) , szPath,bWrite );
	m_ButtonCfg.SetStretchMode(VImageState::STRETCHED);
//	m_ButtonCfg.SetStretchMode(VImageState::TEXTURE_SIZE);

	// get the size of the control
	if (m_vSize.x<=0.f)
	{
		m_vSize = m_ButtonCfg.m_States[VDlgControlBase::NORMAL].GetSize();
		float fBoxWidth = m_vSize.x;
		hkvVec2 vTextSize = m_TextCfg.m_States[VDlgControlBase::NORMAL].GetSize();
		m_vSize.x += vTextSize.x + fBoxWidth;
		m_vSize.y = hkvMath::Max(m_vSize.y,vTextSize.y);

		// offset the text by the size of the checkbox
		for (int i=0;i<VWindowBase::STATE_COUNT;i++)
		{
			hkvVec2 vNewOfs = m_TextCfg.m_States[i].GetTextOfs();
			vNewOfs.x += fBoxWidth;
			m_TextCfg.m_States[i].SetTextOfs(vNewOfs);
		}
	}

	// initial checked status (actually same as selected)
	bool bChecked = false;
	XMLHelper::Exchange_Bool(pNode,"checked",bChecked,bWrite);
	SetChecked(bChecked);
	

	return true;
}


// *** The variable table is used by the plugin system of the engine and editor. In that case we do not have
// *** variables which have to be initialised or which have to be available in the editor.
START_VAR_TABLE(VUINodeCheckBox, VisTypedEngineObject_cl,"animated entity", VFORGE_USE_NATIVE_PARENTING, NULL );
	varList->Add (VisVariable_cl::NewVariable ("Texture ON", "TextureFilename", VULPTYPE_VSTRING, -1, "White.dds", (VDisplayHintFlags_e)DISPLAY_HINT_TEXTUREFILE, 0, 0)  );
	varList->Add (VisVariable_cl::NewVariable ("Texture OFF", "TextureFilename", VULPTYPE_VSTRING, -1, "White.dds", (VDisplayHintFlags_e)DISPLAY_HINT_TEXTUREFILE, 0, 0)  );
	varList->Add (VisVariable_cl::NewVariable ("Text", "TextureFilename", VULPTYPE_VSTRING, -1, "CHECK_BOX", (VDisplayHintFlags_e)DISPLAY_HINT_NONE, 0, 0)  );
	varList->Add (VisVariable_cl::NewVariable ("TextColor", "TextColor", VULPTYPE_BYTE_COLOR4, -1, "255,255,255,255", (VDisplayHintFlags_e)DISPLAY_HINT_COLOR, 0, 0)  );
	varList->Add (VisVariable_cl::NewVariable ("Scale", "TextScale", VULPTYPE_FLOAT, -1, "2.0", (VDisplayHintFlags_e)DISPLAY_HINT_NONE, 0, 0)  );	
	varList->Add (VisVariable_cl::NewVariable ("Font", "Font", VULPTYPE_VSTRING, -1, "Fonts\\DebugFont.fnt", (VDisplayHintFlags_e)DISPLAY_HINT_CUSTOMFILE, 0, 0)  );
END_VAR_TABLE
