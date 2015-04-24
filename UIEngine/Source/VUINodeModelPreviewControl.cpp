#include "VUINodeModelPreviewControl.h"

#include "VUINodePushButton.h"


V_IMPLEMENT_SERIAL( VUINodeModelPreviewControl, VModelPreviewControl, 0, &g_NodeModule );

VUINodeModelPreviewControl::VUINodeModelPreviewControl(void)
{
	CreateDefaultModelView();
}


VUINodeModelPreviewControl::~VUINodeModelPreviewControl(void)
{
	VisBaseEntity_cl *pEntity = m_pComponent->GetPreviewEntity();

	if ( pEntity != NULL )
		Vision::Game.RemoveEntity(pEntity );

	//m_pComponent->SetPreviewEntity(NULL);

//	m_pEntity = NULL;
// 	delete m_pComponent;
// 	m_pComponent = NULL;
}


VUINodeModelPreviewControl* VUINodeModelPreviewControl::CreateInstance()
{
	// CreateInstance
	VUINodeModelPreviewControl* pNewList = new VUINodeModelPreviewControl();

	pNewList->SetPosition(0,0);
	pNewList->SetSize(500,500);
	pNewList->CreateDefaultModelView();


	return pNewList;
}

void VUINodeModelPreviewControl::CreateDefaultModelView()
{

	m_pComponent = new VModelPreviewComponent();
	AddComponent(m_pComponent);

#if !defined(_VISION_MOBILE)
	m_pComponent->CreateEmptyLightGrid();
#endif

	m_pComponent->SetYawSpeed(100.0f);

	hkvVec3 vPos(100.f,0,0);
	hkvVec3 vOri(0,0,0);

	VisBaseEntity_cl *pEnt = Vision::Game.CreateEntity( "VisBaseEntity_cl",vPos,"Models/MagicBall.model");
	VASSERT(pEnt);
	// 		if (szAnimation)
	// 		{
	// 			VisAnimConfig_cl::StartSkeletalAnimation(pEnt, szAnimation, VANIMCTRL_LOOP|VSKELANIMCTRL_DEFAULTS);
	// 			pEnt->SetAlwaysUpdateAnimations(true);
	// 		}
	pEnt->SetOrientation(vOri);
	m_pComponent->SetPreviewEntity(pEnt);


	int iResolution[2] = {256,256};
	float fFOV[2] = {90.f,0.f};
	bool bRetVal = m_pComponent->InitComponent(iResolution[0], iResolution[1], (int)GetSize().x, (int)GetSize().y, fFOV[0], fFOV[1]);
	if(!bRetVal)
		return;

	m_ButtonCfg.SetTexture(m_pComponent->GetRenderableTexture());
}

void VUINodeModelPreviewControl::Serialize( VArchive &ar )
{
	VModelPreviewControl::Serialize(ar);
	char iLocalVersion = 0;
	if (ar.IsLoading())
	{

	}
	else
	{	

	}
}



bool VUINodeModelPreviewControl::Build(TiXmlElement *pNode, const char *szPath, bool bWrite) 
{
	

	if ( bWrite == false )
	{		
		if(m_pComponent!=NULL) // Loading time
		{
			RemoveComponent(m_pComponent);
			m_pComponent = NULL;
		}

		return VModelPreviewControl::Build( pNode,szPath,bWrite);
	}

	if ( GetUniqueID() != 0 )
	{
		VString szID = VGUIManager::GetIDName( (int)GetUniqueID() );	
		XMLHelper::Exchange_String(pNode,"ID",szID,bWrite);
	}		

	XMLHelper::Exchange_Floats(pNode,"pos",m_vPosition.data,2,bWrite);
	XMLHelper::Exchange_Floats(pNode,"size",m_vSize.data,2,bWrite);

	int iResolution[2] = {256,256};
	float fFOV[2] = {90.f,0.f};
	TiXmlElement *pRTNode = XMLHelper::SubNode(pNode,"rendertarget",bWrite);
	if (pRTNode)
	{
		XMLHelper::Exchange_Ints(pRTNode,"resolution",iResolution,2,bWrite);
		XMLHelper::Exchange_Floats(pRTNode,"fov",fFOV,2,bWrite);
	}

	VString szModelFileName;

	if ( m_pComponent && m_pComponent->GetPreviewEntity() && m_pComponent->GetPreviewEntity()->GetMesh() )
		szModelFileName = m_pComponent->GetPreviewEntity()->GetMesh()->GetFilename();

	TiXmlElement *pPreviewNode = XMLHelper::SubNode(pNode,"preview",bWrite);
	if (pPreviewNode)
	{
		const char *szModel = XMLHelper::Exchange_String(pPreviewNode,"model",szModelFileName,bWrite);
	//	const char *szAnimation = XMLHelper::Exchange_String(pPreviewNode,"animation",NULL,bWrite);
	//	const char *szEntityClass = XMLHelper::Exchange_String(pPreviewNode,"entityclass",NULL,bWrite);
		hkvVec3 vPos(100.f,0,0);
		hkvVec3 vOri(0,0,0);
		bool bFit = true;
		XMLHelper::Exchange_Floats(pPreviewNode,"position",vPos.data,3,bWrite);
		XMLHelper::Exchange_Floats(pPreviewNode,"orientation",vOri.data,3,bWrite);
		XMLHelper::Exchange_Bool(pPreviewNode,"fit",bFit,bWrite);
		float fYawSpeed = 100.0f;
		XMLHelper::Exchange_Float(pPreviewNode,"yawspeed",fYawSpeed,bWrite);	
	}
	


	return true;
}

void VUINodeModelPreviewControl::OnVariableValueChanged(VisVariable_cl *pVar, const char * value)
{
	VASSERT (pVar);
	VASSERT (value);

	if (pVar->name && strcmp(pVar->name, "Model") == 0)
	{
		GetPreviewEntity()->SetMesh( value );
	}	
	else if (pVar->name && strcmp(pVar->name, "Animation") == 0)
	{
	//	GetPreviewEntity()->SetMesh( value );
	}	
	else if (pVar->name && strcmp(pVar->name, "Position") == 0)
	{
		int iComponents[3];
		int iCount = sscanf(value,"%i/%i/%i",&iComponents[0],&iComponents[1],&iComponents[2]);

		GetPreviewEntity()->SetPosition( hkvVec3(iComponents[0],iComponents[1],iComponents[2]) );
	}	
	else if (pVar->name && strcmp(pVar->name, "Orientation") == 0)
	{
		int iComponents[3];
		int iCount = sscanf(value,"%i/%i/%i",&iComponents[0],&iComponents[1],&iComponents[2]);

		GetPreviewEntity()->SetOrientation( hkvVec3(iComponents[0],iComponents[1],iComponents[2]) );		
	}	
}


// *** The variable table is used by the plugin system of the engine and editor. In that case we do not have
// *** variables which have to be initialised or which have to be available in the editor.
START_VAR_TABLE(VUINodeModelPreviewControl, VisTypedEngineObject_cl,"animated entity", VVARIABLELIST_FLAGS_NONE, NULL );
varList->Add (VisVariable_cl::NewVariable ("Model", "Model Filename", VULPTYPE_VSTRING, -1, "Models/MagicBall.model", (VDisplayHintFlags_e)DISPLAY_HINT_MODELFILE, 0, 0)  );
DEFINE_VAR_VSTRING(VUINodeModelPreviewControl, AnimationName, "Name of the animation to playback", "", 0, 0, "dropdownlist(Animation)");

varList->Add (VisVariable_cl::NewVariable ("Position", "Entity Position", VULPTYPE_VECTOR_INT, -1, "100/0/0", (VDisplayHintFlags_e)DISPLAY_HINT_NONE, 0, 0));	
varList->Add (VisVariable_cl::NewVariable ("Orientation", "Entity Orientation", VULPTYPE_VECTOR_INT, -1, "0/0/0", (VDisplayHintFlags_e)DISPLAY_HINT_NONE, 0, 0));	


END_VAR_TABLE
