#include "VUINodeNumericUpDown.h"


V_IMPLEMENT_SERIAL( VUINodeNumericUpDown, VCheckBox, 0, &g_NodeModule );

VUINodeNumericUpDown::VUINodeNumericUpDown(void)
{
	m_Default = 0.0f;
	m_tick = 1.0f;;

	m_pButtonUp = NULL;
	m_pButtonDown = NULL;

	m_pButtonUp = new VPushButton();
	m_pButtonUp->SetSize(60,46);
	m_pButtonUp->SetPosition(139,2);
	m_pButtonUp->Image().SetTexture(Vision::TextureManager.GetPlainWhiteTexture());
	m_pButtonUp->Image().SetStretchMode( VImageState::STRETCHED );	
	
	m_pButtonUp->Image().m_States[VWindowBase::DISABLED].SetColor ( V_RGBA_BLUE );
	m_pButtonUp->Image().m_States[VWindowBase::NORMAL].SetColor ( V_RGBA_CYAN );
	m_pButtonUp->Image().m_States[VWindowBase::MOUSEOVER].SetColor ( V_RGBA_GREEN );
	m_pButtonUp->Image().m_States[VWindowBase::SELECTED].SetColor ( V_RGBA_YELLOW );

	m_pButtonUp->SetParent(this);

	m_pButtonDown = new VPushButton();
	m_pButtonDown->SetSize(60,46);
	m_pButtonDown->SetPosition(139,52);
	m_pButtonDown->Image().SetTexture(Vision::TextureManager.GetPlainWhiteTexture());
	m_pButtonDown->Image().SetStretchMode( VImageState::STRETCHED );	

	m_pButtonDown->Image().m_States[VWindowBase::DISABLED].SetColor ( V_RGBA_BLUE );
	m_pButtonDown->Image().m_States[VWindowBase::NORMAL].SetColor ( V_RGBA_CYAN );
	m_pButtonDown->Image().m_States[VWindowBase::MOUSEOVER].SetColor ( V_RGBA_GREEN );
	m_pButtonDown->Image().m_States[VWindowBase::SELECTED].SetColor ( V_RGBA_YELLOW );
	
	m_pButtonDown->SetParent(this);

	Text().SetHorizontalAlignment(VisFont_cl::ALIGN_LEFT);
	Text().SetVerticalAlignment(VisFont_cl::ALIGN_CENTER);
	Image().SetStretchMode( VImageState::STRETCHED );
}


VUINodeNumericUpDown::~VUINodeNumericUpDown(void)
{
}


void VUINodeNumericUpDown::OnPointerDown(VMenuEventDataObject *pEvent)
{
	SetSelected(true);

	if ( m_pButtonUp->GetBoundingBox().IsInside(pEvent->m_vMousePos )  == true )
	{
		m_pButtonUp->SetSelected(true);		
		m_Default += m_tick;		
	}
	if ( m_pButtonDown->GetBoundingBox().IsInside(pEvent->m_vMousePos )  == true )
	{
		m_pButtonDown->SetSelected(true);
		m_Default -= m_tick;		
	}

	hkvMath::clamp (m_Default , m_Min , m_Max );
}

void VUINodeNumericUpDown::OnPointerUp(VMenuEventDataObject *pEvent)
{
	SetSelected(false);

	if ( m_pButtonUp->GetBoundingBox().IsInside(pEvent->m_vMousePos )  == true )	
		m_pButtonUp->SetSelected(false);	

	if ( m_pButtonDown->GetBoundingBox().IsInside(pEvent->m_vMousePos )  == true )	
		m_pButtonDown->SetSelected(false);
}

void VUINodeNumericUpDown::OnMouseLeave(VGUIUserInfo_t &user)
{
	VDlgControlBase::OnMouseLeave(user);
	SetSelected(false);		
	m_pButtonUp->SetSelected(false);
	m_pButtonDown->SetSelected(false);
}

void VUINodeNumericUpDown::Serialize( VArchive &ar )
{
	VCheckBox::Serialize(ar);
	char iLocalVersion = 0;
	if (ar.IsLoading())
	{
	
	} else
	{

	}
}




void VUINodeNumericUpDown::OnVariableValueChanged(VisVariable_cl *pVar, const char * value)
{
	VASSERT (pVar);
	VASSERT (value);

	/// Texture Button
	if (pVar->name && strstr(pVar->name, "Texture") != 0)
	{
		VTextureObject* spTexture = Vision::TextureManager.Load2DTexture( value );		

		if ( pVar->name && strstr(pVar->name, "UpButton") != 0 )
		{
			m_pButtonUp->Image().SetTexture( spTexture );			
		}
		else if ( pVar->name && strstr(pVar->name, "DownButton" ) != 0 )
		{			
			m_pButtonDown->Image().SetTexture( spTexture );			
		}
		else if ( pVar->name && strstr(pVar->name, "Frame" ) != 0 )
		{			
			Image().SetTexture( spTexture );			
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
	else if (pVar->name && strcmp(pVar->name, "m_fButtonLength") == 0)
	{
		OnSizeChanged(); 
		return;
	}
}



bool VUINodeNumericUpDown::Build(TiXmlElement *pNode, const char *szPath, bool bWrite) 
{
	if ( bWrite == false )
	{		
		VCheckBox::Build( pNode,szPath,bWrite);
	}
	else
	{
		if ( GetUniqueID() != 0 )
		{
			VString szID = VGUIManager::GetIDName( (int)GetUniqueID() );	
			XMLHelper::Exchange_String(pNode,"ID",szID,bWrite);
		}

		XMLHelper::Exchange_Floats(pNode,"pos",m_vPosition.data,2,bWrite);
		XMLHelper::Exchange_Floats(pNode,"size",m_vSize.data,2,bWrite);

		VUINodeExportHelperXML::GlobalManager().BuildImageState( &m_ButtonCfg , this , XMLHelper::SubNode(pNode,"image",bWrite) , szPath,bWrite );
		VUINodeExportHelperXML::GlobalManager().BuildTextState( &m_TextCfg , this , XMLHelper::SubNode(pNode,"text",bWrite) , szPath,bWrite );

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
	}


	XMLHelper::Exchange_Float(pNode,"defaultValue",m_Default,bWrite);
	XMLHelper::Exchange_Float(pNode,"m_tick",m_tick,bWrite);
	XMLHelper::Exchange_Float(pNode,"m_Min",m_Min,bWrite);
	XMLHelper::Exchange_Float(pNode,"m_Max",m_Max,bWrite);
	XMLHelper::Exchange_Float(pNode,"m_fButtonLength",m_fButtonLength,bWrite);


	TiXmlElement *pNodeUpButton = XMLHelper::SubNode(pNode,"upbutton",bWrite);
	if (pNodeUpButton)
	{			
		if ( bWrite )
		{
			const char *szTexture = m_pButtonUp->Image().m_States[VWindowBase::NORMAL].GetCurrentTexture()->GetFilename();
			XMLHelper::Exchange_String(pNodeUpButton,"texture",szTexture,bWrite);	
		}
		else
		{
			const char *szState = XMLHelper::Exchange_String(pNodeUpButton,"texture",NULL,bWrite);		
			VTextureObject* spTexture = Vision::TextureManager.Load2DTexture( szState );
			m_pButtonUp->Image().SetTexture( spTexture );		
		}		
	}	

	TiXmlElement *pNodeDownButton = XMLHelper::SubNode(pNode,"downbutton",bWrite);
	if (pNodeDownButton)
	{
		if ( bWrite )
		{
			const char *szTexture = m_pButtonDown->Image().m_States[VWindowBase::NORMAL].GetCurrentTexture()->GetFilename();
			XMLHelper::Exchange_String(pNodeDownButton,"texture",szTexture,bWrite);	
		}
		else
		{
			const char *szState = XMLHelper::Exchange_String(pNodeDownButton,"texture",NULL,bWrite);		
			VTextureObject* spTexture = Vision::TextureManager.Load2DTexture( szState );
			m_pButtonDown->Image().SetTexture( spTexture );		
		}		
	}

	OnSizeChanged();

	return true;
}



void VUINodeNumericUpDown::OnPaint(VGraphicsInfo &Graphics, const VItemRenderInfo &parentState)
{
	VString szText;
	szText.Format("%d" , (int)m_Default );
	SetText( szText );

	VPushButton::OnPaint( Graphics,parentState );

	if ( m_pButtonUp )
		m_pButtonUp->OnPaint( Graphics,parentState );

	if ( m_pButtonDown )
		m_pButtonDown->OnPaint( Graphics,parentState );
}

void VUINodeNumericUpDown::OnTick(float dtime)
{

}

void VUINodeNumericUpDown::OnDragBegin(const hkvVec2 &vMousePos, int iButtonMask)
{

}

void VUINodeNumericUpDown::OnDragging(const hkvVec2 &vMouseDelta)
{
	m_Default -= ( vMouseDelta.y * m_tick );
	hkvMath::clamp (m_Default , m_Min , m_Max );
}

void VUINodeNumericUpDown::OnDragEnd(VWindowBase *pOver)
{

}


void VUINodeNumericUpDown::OnSizeChanged() 
{
	VDlgControlBase::OnSizeChanged();

	hkvVec2 vSize = GetSize();
	float fButtonPosX = vSize.x  - m_fButtonLength;
	float fButtonHeight = ( vSize.y / 2.0f ) - 2 ;
	
	m_pButtonUp->SetPosition(fButtonPosX,2);
	m_pButtonUp->SetSize( m_fButtonLength , fButtonHeight );

	m_pButtonDown->SetPosition(fButtonPosX,fButtonHeight + 2 );		
	m_pButtonDown->SetSize( m_fButtonLength , fButtonHeight );
}


// *** The variable table is used by the plugin system of the engine and editor. In that case we do not have
// *** variables which have to be initialised or which have to be available in the editor.
START_VAR_TABLE(VUINodeNumericUpDown, VisTypedEngineObject_cl,"animated entity", VFORGE_USE_NATIVE_PARENTING, NULL );
	varList->Add (VisVariable_cl::NewVariable ("Texture Frame", "TextureFilename", VULPTYPE_VSTRING, -1, "White.dds", (VDisplayHintFlags_e)DISPLAY_HINT_TEXTUREFILE, 0, 0)  );
	varList->Add (VisVariable_cl::NewVariable ("Texture UpButton", "TextureFilename", VULPTYPE_VSTRING, -1, "White.dds", (VDisplayHintFlags_e)DISPLAY_HINT_TEXTUREFILE, 0, 0)  );
	varList->Add (VisVariable_cl::NewVariable ("Texture DownButton", "TextureFilename", VULPTYPE_VSTRING, -1, "White.dds", (VDisplayHintFlags_e)DISPLAY_HINT_TEXTUREFILE, 0, 0)  );	
	varList->Add (VisVariable_cl::NewVariable ("Text", "TextureFilename", VULPTYPE_VSTRING, -1, "CHECK_BOX", (VDisplayHintFlags_e)DISPLAY_HINT_NONE, 0, 0)  );
	varList->Add (VisVariable_cl::NewVariable ("TextColor", "TextColor", VULPTYPE_BYTE_COLOR4, -1, "255,255,255,255", (VDisplayHintFlags_e)DISPLAY_HINT_COLOR, 0, 0)  );
	varList->Add (VisVariable_cl::NewVariable ("Scale", "TextScale", VULPTYPE_FLOAT, -1, "2.0", (VDisplayHintFlags_e)DISPLAY_HINT_NONE, 0, 0)  );	
	varList->Add (VisVariable_cl::NewVariable ("Font", "Font", VULPTYPE_VSTRING, -1, "Fonts\\DebugFont.fnt", (VDisplayHintFlags_e)DISPLAY_HINT_CUSTOMFILE, 0, 0)  );

	DEFINE_VAR_FLOAT(VUINodeNumericUpDown, m_Default, "", "100.0", 0, 0);
	DEFINE_VAR_FLOAT(VUINodeNumericUpDown, m_tick, "", "1.0", 0, 0);
	DEFINE_VAR_FLOAT(VUINodeNumericUpDown, m_Min, "", "0.0", 0, 0);
	DEFINE_VAR_FLOAT(VUINodeNumericUpDown, m_Max, "", "500.0", 0, 0);
	DEFINE_VAR_FLOAT(VUINodeNumericUpDown, m_fButtonLength, "", "60.0", 0, 0);

END_VAR_TABLE
