#include "VUINodeSliderControl.h"


V_IMPLEMENT_SERIAL( VUINodeSliderControl, VSliderControl, 0, &g_NodeModule );

VUINodeSliderControl::VUINodeSliderControl(void)
{
}


VUINodeSliderControl::~VUINodeSliderControl(void)
{
}

VUINodeSliderControl* VUINodeSliderControl::CreateInstance()
{
	// CreateInstance
	VUINodeSliderControl* pVUINodeSliderControl = new VUINodeSliderControl();

  
	  pVUINodeSliderControl->SetSize(250,50);
	  pVUINodeSliderControl->SetPosition(100,100);


	  pVUINodeSliderControl->SetSliderRange( 0,1, 0);
	  pVUINodeSliderControl->SetVertical( false );

	  VTextureObject *pTextureFrame = Vision::TextureManager.Load2DTexture( "Textures/Checkerboard.dds");

	  VTextureObject *pTextureSlider = Vision::TextureManager.Load2DTexture( "Textures/vapp_slider.dds");

	  pVUINodeSliderControl->Frame().SetTexture( pTextureFrame );	
	  pVUINodeSliderControl->Frame().SetStretchMode(VImageState::STRETCHED);
	  pVUINodeSliderControl->GetSlider()->Images().SetTexture( pTextureSlider );
	  pVUINodeSliderControl->GetSlider()->Images().SetStretchMode(VImageState::STRETCHED);
	  pVUINodeSliderControl->GetSlider()->Images().SetTransparency(VIS_TRANSP_ALPHATEST);
	  pVUINodeSliderControl->GetSlider()->SetSize(50,50);
	  pVUINodeSliderControl->GetSlider()->OnBuildFinished();


	return pVUINodeSliderControl;
}

void VUINodeSliderControl::Serialize( VArchive &ar )
{
	VSliderControl::Serialize(ar);
	char iLocalVersion = 0;
	if (ar.IsLoading())
	{
	
	}
	else
	{	

	}
}



bool VUINodeSliderControl::Build(TiXmlElement *pNode, const char *szPath, bool bWrite) 
{
	if ( bWrite == false )
	{
		bool bResult = VSliderControl::Build( pNode,szPath, bWrite);
		return bResult;
	}

	if ( GetUniqueID() != 0 )
	{
		VString szID = VGUIManager::GetIDName( (int)GetUniqueID() );	
		XMLHelper::Exchange_String(pNode,"ID",szID,bWrite);
	}		


	// frame
	//m_Frame.Build(this, XMLHelper::SubNode(pNode,"frame",bWrite),szPath,bWrite);





	// slider button
// 	TiXmlElement *pSliderNode = XMLHelper::SubNode(pNode,"slider",bWrite);
// 	GetSlider()->Build(pSliderNode,szPath,bWrite);
// 	GetSlider()->OnBuildFinished();




	XMLHelper::Exchange_Floats(pNode,"pos",m_vPosition.data,2,bWrite);
	XMLHelper::Exchange_Floats(pNode,"size",m_vSize.data,2,bWrite);

	// frame
	VUINodeExportHelperXML::GlobalManager().BuildImageState( &m_Frame , this , XMLHelper::SubNode(pNode,"frame",bWrite) , szPath,bWrite );

	// border
	float *borderSizeData = (float*)(&m_BorderSize.m_vMin);
	XMLHelper::Exchange_Floats(pNode,"border",borderSizeData,4,bWrite);


	// slider button
	TiXmlElement *pSliderNode = XMLHelper::SubNode(pNode,"slider",bWrite);

	GetSlider()->m_pContext = m_pContext;
	GetSlider()->Build(pSliderNode,szPath,bWrite);
	GetSlider()->OnBuildFinished();

	// range
	float fRange[2] = {m_fRangeMin, m_fRangeMax};
	XMLHelper::Exchange_Floats(pNode,"range",fRange,2,bWrite);
	m_fRangeMin = fRange[0];
	m_fRangeMax = fRange[1];
	XMLHelper::Exchange_Int(pNode,"ticks",m_iTickCount,bWrite);
	XMLHelper::Exchange_Float(pNode,"value",m_fCurrentValue,bWrite);

	// slider control properties
	bool bVertical = m_bVertical;
	XMLHelper::Exchange_Bool(pNode,"vertical",bVertical,bWrite);

	float fRelSize = 0.f;
	XMLHelper::Exchange_Float(pSliderNode,"relsize",fRelSize,bWrite);

	SetSliderRelSize(fRelSize);

	return true;
}



void VUINodeSliderControl::OnVariableValueChanged(VisVariable_cl *pVar, const char * value)
{
	VASSERT (pVar);
	VASSERT (value);

//	return;

	if (pVar->name && strcmp(pVar->name, "FrameTexture") == 0)
	{
		VTextureObject* spTexture = Vision::TextureManager.Load2DTexture( value );
		Frame().SetTexture( spTexture );
		Frame().SetStretchMode(VImageState::STRETCHED);
	}	
	if (pVar->name && strstr(pVar->name, "FrameColor") != 0)
	{
		int iComponents[4];
		int iCount = sscanf(value,"%i/%i/%i/%i",&iComponents[0],&iComponents[1],&iComponents[2],&iComponents[3]);

		VColorRef color(iComponents[0],iComponents[1],iComponents[2],iComponents[3]);
		Frame().m_States->SetColor( color );
	}


	if (pVar->name && strcmp(pVar->name, "SliderTexture") == 0)
	{
		VTextureObject* spTexture = Vision::TextureManager.Load2DTexture( value );
		GetSlider()->Images().SetTexture( spTexture );		
		GetSlider()->Images().SetStretchMode(VImageState::STRETCHED);

		//GetSlider()->m_vSize = GetSize();
		GetSlider()->OnBuildFinished();
	}	
	if (pVar->name && strstr(pVar->name, "SliderColor") != 0)
	{
		int iComponents[4];
		int iCount = sscanf(value,"%i/%i/%i/%i",&iComponents[0],&iComponents[1],&iComponents[2],&iComponents[3]);

		VColorRef color(iComponents[0],iComponents[1],iComponents[2],iComponents[3]);
		GetSlider()->Images().m_States[VWindowBase::MOUSEOVER].SetColor( color );
		GetSlider()->Images().m_States[VWindowBase::SELECTED].SetColor( color );
		GetSlider()->Images().m_States[VWindowBase::DISABLED].SetColor( color );
		GetSlider()->Images().m_States[VWindowBase::NORMAL].SetColor( color );


		//GetSlider()->m_vSize = GetSize();
		//GetSlider()->OnBuildFinished();
	}


	if (pVar->name && strcmp(pVar->name, "m_fSliderRelWidth") == 0)
	{
		float fScale = 1.0f;
		int iCount = sscanf(value,"%f",&fScale);		
		
// 		
// 		if ( m_bVertical )
// 		{
// 			GetSlider()->m_vSize = GetSize();			
// 		}
// 		else
// 		{
// 			GetSlider()->m_vSize = GetSize();
// 			
// 		}	

		SetSliderRelSize(fScale);
	//	GetSlider()->OnBuildFinished();
	}
	if (pVar->name && strcmp(pVar->name, "Vertical") == 0)
	{
		if (value != NULL && strcmp(value, "True") == 0)
			m_bVertical = true;
		else
			m_bVertical = false;		
	}	
	if (pVar->name && strcmp(pVar->name, "SliderBorderMode") == 0)
	{
		if (value != NULL && strcmp(value, "True") == 0)
			GetSlider()->SetBorderMode(true);
		else
			GetSlider()->SetBorderMode(false);
	}	
}


// *** The variable table is used by the plugin system of the engine and editor. In that case we do not have
// *** variables which have to be initialised or which have to be available in the editor.
START_VAR_TABLE(VUINodeSliderControl, VisTypedEngineObject_cl,"animated entity", VVARIABLELIST_FLAGS_NONE, NULL );
	varList->Add (VisVariable_cl::NewVariable ("SliderTexture", "SliderTexture", VULPTYPE_VSTRING, -1, "White.dds", (VDisplayHintFlags_e)DISPLAY_HINT_TEXTUREFILE, 0, 0)  );
	varList->Add (VisVariable_cl::NewVariable ("SliderColor", "SliderColor", VULPTYPE_BYTE_COLOR4, -1, "128,128,128,255", (VDisplayHintFlags_e)DISPLAY_HINT_COLOR, 0, 0)  );

	varList->Add (VisVariable_cl::NewVariable ("FrameTexture", "FrameTexture", VULPTYPE_VSTRING, -1, "White.dds", (VDisplayHintFlags_e)DISPLAY_HINT_TEXTUREFILE, 0, 0)  );
	varList->Add (VisVariable_cl::NewVariable ("FrameColor", "FrameColor", VULPTYPE_BYTE_COLOR4, -1, "128,128,128,255", (VDisplayHintFlags_e)DISPLAY_HINT_COLOR, 0, 0)  );

// DEFINE_VAR_COLORREF(VUINodeSliderControl, m_iBackFadeColor, "Fadeback Color", "255/255/255/255", 0, 0);	
// DEFINE_VAR_COLORREF(VUINodeSliderControl, m_iBackColor, "Sun color", "128/128/128/255", 0, 0);
// //	DEFINE_VAR_BOOL(VUINodeDialog, m_bFadeBack, "", "TRUE", 0, 0);	
// DEFINE_VAR_INT_AND_NAME(VUINodeDialog, m_iDialogFlags, "DialogFlags" ,  "Dialog Flag", "0", 0, "CSharpType(DialogFlags_e)");

	 DEFINE_VAR_INT(VUINodeSliderControl, m_iTickCount, "Tick", "0", 0, 0);
 DEFINE_VAR_FLOAT(VUINodeSliderControl, m_fRangeMin, "m_fRangeMin", "0", 0, 0);	
 DEFINE_VAR_FLOAT(VUINodeSliderControl, m_fRangeMax, "m_fRangeMax", "1", 0, 0);	
 varList->Add (VisVariable_cl::NewVariable ("Vertical", "Vertical Scroll", VULPTYPE_BOOL, -1, "False", DISPLAY_HINT_NONE, 0, 0)  );
 varList->Add (VisVariable_cl::NewVariable ("SliderBorderMode", "SliderBorderMode", VULPTYPE_BOOL, -1, "True", DISPLAY_HINT_NONE, 0, 0)  );

 DEFINE_VAR_FLOAT(VUINodeSliderControl, m_fSliderRelWidth, "m_fSliderRelWidth", "0", 0, 0);
 
END_VAR_TABLE
