#include "VUINodeTextLabel.h"

V_IMPLEMENT_SERIAL( VUINodeTextLabel, VTextLabel, 0, &g_NodeModule );

VUINodeTextLabel::VUINodeTextLabel(void)
{
	
}


VUINodeTextLabel::~VUINodeTextLabel(void)
{
}


VUINodeTextLabel* VUINodeTextLabel::CreateInstance()
{
	// CreateInstance
	VUINodeTextLabel* pNewImage = new VUINodeTextLabel();
	
	pNewImage->Text().SetFont(VUISharedData::GetFont());
	pNewImage->Text().SetScaling(2);
	pNewImage->Text().SetText( "TEXT_LABEL" );
	pNewImage->Text().SetHorizontalAlignment( VisFont_cl::ALIGN_CENTER );
	pNewImage->Text().SetVerticalAlignment( VisFont_cl::ALIGN_CENTER );
	

	return pNewImage;
}


void VUINodeTextLabel::OnVariableValueChanged(VisVariable_cl *pVar, const char * value)
{
	VASSERT (pVar);
	VASSERT (value);

	if (pVar->name && strcmp(pVar->name, "TextColor") == 0)
	{
		int iComponents[4];
		int iCount = sscanf(value,"%i/%i/%i/%i",&iComponents[0],&iComponents[1],&iComponents[2],&iComponents[3]);

		VColorRef color(iComponents[0],iComponents[1],iComponents[2],iComponents[3]);
		Text().SetColor( color );
		
		return;
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
	else if (pVar->name && strcmp(pVar->name, "TextWrap") == 0)
	{
		if (value != NULL && strcmp(value, "True") == 0)
			Text().SetTextWrap(true);			
		else
			Text().SetTextWrap(false);		
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

void VUINodeTextLabel::Serialize( VArchive &ar )
{
	VTextLabel::Serialize(ar);
	char iLocalVersion = 0;
	if (ar.IsLoading())
	{
	
	} else
	{

	}
}




bool VUINodeTextLabel::Build(TiXmlElement *pNode, const char *szPath, bool bWrite) 
{
	if ( bWrite == false )
	{			
		return VTextLabel::Build( pNode,szPath,bWrite);
	}

	if ( GetUniqueID() != 0 )
	{
		VString szID = VGUIManager::GetIDName( (int)GetUniqueID() );	
		XMLHelper::Exchange_String(pNode,"ID",szID,bWrite);
	}		

	// size and position
	XMLHelper::Exchange_Floats(pNode,"pos",m_vPosition.data,2,bWrite);
	XMLHelper::Exchange_Floats(pNode,"size",m_vSize.data,2,bWrite);

// 	if (!VDlgControlBase::Build(pNode,szPath,bWrite))
// 		return false;

	VUINodeExportHelperXML::GlobalManager().BuildTextState( &m_TextCfg , this , XMLHelper::SubNode(pNode,"text",bWrite) , szPath,bWrite );

	// ..or from text
	if (m_vSize.x<=0.f)
		m_vSize = m_TextCfg.m_States[VDlgControlBase::NORMAL].GetSize();

	return true;
}


// *** The variable table is used by the plugin system of the engine and editor. In that case we do not have
// *** variables which have to be initialised or which have to be available in the editor.
START_VAR_TABLE(VUINodeTextLabel, VisTypedEngineObject_cl,"animated entity", VVARIABLELIST_FLAGS_NONE, NULL );
	varList->Add (VisVariable_cl::NewVariable ("Texture", "TextureFilename", VULPTYPE_VSTRING, -1, "White.dds", DISPLAY_HINT_TEXTUREFILE, 0, 0)  );
	varList->Add (VisVariable_cl::NewVariable ("Text", "TextureFilename", VULPTYPE_VSTRING, -1, "TEXT_LABEL", DISPLAY_HINT_NONE, 0, 0)  );
	varList->Add (VisVariable_cl::NewVariable ("TextColor", "TextColor", VULPTYPE_BYTE_COLOR4, -1, "255,255,255,255", DISPLAY_HINT_COLOR, 0, 0)  );
	varList->Add (VisVariable_cl::NewVariable ("Scale", "TextScale", VULPTYPE_FLOAT, -1, "2.0", DISPLAY_HINT_NONE, 0, 0)  );	
	varList->Add (VisVariable_cl::NewVariable ("Font", "Font", VULPTYPE_VSTRING, -1, "Fonts\\NanumGothic.fnt", DISPLAY_HINT_CUSTOMFILE, 0, 0)  );
	varList->Add (VisVariable_cl::NewVariable ("TextWrap", "TextWrap", VULPTYPE_BOOL, -1, "True", DISPLAY_HINT_NONE, 0, 0)  );
	varList->Add (VisVariable_cl::NewVariable ("AlignmentVertical", "AlignmentVertical", VULPTYPE_INT, -1, "ALIGN_CENTER", DISPLAY_HINT_NONE, "CSharpType(AlignmentVertical)", 0)  );
	varList->Add (VisVariable_cl::NewVariable ("AlignmentHorizontal", "AlignmentHorizontal", VULPTYPE_INT, -1, "ALIGN_CENTER", DISPLAY_HINT_NONE, "CSharpType(AlignmentHorizontal)", 0)  );
END_VAR_TABLE
