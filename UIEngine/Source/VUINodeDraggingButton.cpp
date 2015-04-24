#include "VUINodeDraggingButton.h"
#include "VUINodeExportHelperXML.h"



V_IMPLEMENT_SERIAL( VUINodeDraggingButton, VUINodePushButton, 0, &g_NodeModule );

VUINodeDraggingButton::VUINodeDraggingButton(void)
{
	m_vDeltaPos = hkvVec2::ZeroVector();
}


VUINodeDraggingButton::~VUINodeDraggingButton(void)
{
}

VUINodeDraggingButton* VUINodeDraggingButton::CreateInstance()
{
	// CreateInstance
	VUINodeDraggingButton* pNewPushButton = new VUINodeDraggingButton();

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

// 
// void VUINodeDraggingButton::Serialize( VArchive &ar )
// {
// 	VPushButton::Serialize(ar);
// 	
// 	char iLocalVersion = 0;
// 	if (ar.IsLoading())
// 	{	
// 		//ar >> m_buttonID;
// 
// 	}
// 	else
// 	{	
// 		//ar << m_buttonID;
// 	}
// }


void VUINodeDraggingButton::OnDragBegin(const hkvVec2 &vMousePos, int iButtonMask)
{	
	m_vPivotPoint = vMousePos;
	m_vInitPos = GetPosition();
	m_vDeltaPos = hkvVec2::ZeroVector();
	return;
}


void VUINodeDraggingButton::OnDragging(const hkvVec2 &vMouseDelta)
{
	hkvVec2 vPos = GetPosition();
	hkvVec2 vSize = GetSize();
	vPos += vMouseDelta;	

	VRectanglef rect;
	rect.Set(vPos, vPos+ vSize );

	if ( m_fDragRange.IsCompletelyInside( rect ) == true )
	{
		SetPosition( vPos.x , vPos.y );
		m_vDeltaPos = vMouseDelta;
	}
	else
	{
		RecoverPosition();
	}

	return;
}


void VUINodeDraggingButton::OnDragEnd(VWindowBase *pOver)
{
	m_vDeltaPos = hkvVec2::ZeroVector();
	return;
}

void VUINodeDraggingButton::RecoverPosition()
{
	SetPosition( m_vInitPos.x , m_vInitPos.y );
}



bool VUINodeDraggingButton::Build(TiXmlElement *pNode, const char *szPath, bool bWrite) 
{
	VUINodePushButton::Build( pNode,szPath,bWrite );

	m_vDeltaPos = hkvVec2::ZeroVector();
	m_fDragRange = GetParentDialog()->GetClientRect();

	return true;
}

void VUINodeDraggingButton::SetDragRange(VRectanglef rect)
{
	m_fDragRange = rect;
}


void VUINodeDraggingButton::OnVariableValueChanged(VisVariable_cl *pVar, const char * value)
{
	VASSERT (pVar);
	VASSERT (value);

	VUINodePushButton::OnVariableValueChanged( pVar, value);
}

// *** The variable table is used by the plugin system of the engine and editor. In that case we do not have
// *** variables which have to be initialised or which have to be available in the editor.
START_VAR_TABLE(VUINodeDraggingButton, VUINodeStyleButton,"VUINodeDraggingButton", VVARIABLELIST_FLAGS_NONE, NULL );

END_VAR_TABLE
