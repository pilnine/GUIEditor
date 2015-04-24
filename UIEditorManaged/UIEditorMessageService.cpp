// TKBMS v1.0 -----------------------------------------------------
//
// PLATFORM		: ALL
// PRODUCT		: VISION
// VISIBILITY		: PUBLIC
//
// ------------------------------------------------------TKBMS v1.0

#include "UIEditorManagedPCH.h"
#include "UIEditorMessageService.hpp"



#include "..\UIEngine\Source\VUINode.hpp"
#include "..\UIEngine\Source\UIEngineCallbacks.h"

#include "..\UIEngine\Source\VDefaultGUIContext.hpp"
//#include "..\UIEngine\Source\VUINode.hpp"

#using <mscorlib.dll>

//#pragma make_public(ConversionUtils)
//#pragma make_public(VType)
//#pragma make_public(DynamicPropertyManager^)




namespace UIEditorManaged
{
	UIEditorMessageService::UIEditorMessageService()
	{
		
	}

	UIEditorMessageService::~UIEditorMessageService()
	{
	
	}


	void UIEditorMessageService::OnPropertyChanged(System::String^ propertyName , int Value )
	{
		UIPropertyDataObject<int> eventItem( &UIEngineCallbackManager_cl::GlobalManager().OnUIPropertyChanged ,"property" , Value );

		UIEngineCallbackManager_cl::GlobalManager().OnUIPropertyChanged.TriggerCallbacks( &eventItem );

		return;
	}

	void UIEditorMessageService::OnPropertyChanged(System::String^ propertyName , float Value )
	{
		UIPropertyDataObject<float> eventItem( &UIEngineCallbackManager_cl::GlobalManager().OnUIPropertyChanged ,"property" , Value );

		UIEngineCallbackManager_cl::GlobalManager().OnUIPropertyChanged.TriggerCallbacks( &eventItem );

		return;
	}

	void UIEditorMessageService::OnPropertyChanged(System::String^ propertyName , System::String^ Value )
	{
		return;
	}

	void UIEditorMessageService::OnPropertyChanged(System::String^ propertyName , bool Value )
	{
		return;
	}

	float UIEditorMessageService::GetResolutionScaleX()
	{	
		return VUINodeMananger_cl::GlobalManager()->GetGUIContext()->GetScaleX();		
	}

	float UIEditorMessageService::GetResolutionScaleY()
	{
		return VUINodeMananger_cl::GlobalManager()->GetGUIContext()->GetScaleY();		
	}

	void UIEditorMessageService::RenderLine2D(float x1,float y1, float x2,float y2, UINT iColorRef, float fStrength)
	{
		VColorRef color;
		color.SetRGBA(iColorRef);
		Vision::Game.DrawSingleLine2D(x1,y1,x2,y2, color, fStrength);
	}
	
	DynamicPropertyManager^ UIEditorMessageService::UIPropertyManager::get()
	{
		if ( !_uiPropertyManager )
		{
			_uiPropertyManager = gcnew DynamicPropertyManager();
			ConvertUIRTTIClasses( _uiPropertyManager, V_RUNTIME_CLASS(VWindowBase), false );			
		}

		return _uiPropertyManager;
	}


// 	DynamicPropertyManager^ UIEditorMessageService::UIPropertyManager::get()
// 	{
// 		if ( !_uiPropertyManager )
// 		{
// 			_uiPropertyManager = gcnew DynamicPropertyManager();
// 			ConversionUtils::ConvertRTTIClasses( _uiPropertyManager, V_RUNTIME_CLASS(VUINode_cl), false );
// 		}
// 
// 		return _uiPropertyManager;
// 	}	

	void UIEditorMessageService::GetUIRTTIClasses( DynamicPropertyManager ^manager)
	{
		ConvertUIRTTIClasses( manager, V_RUNTIME_CLASS(VWindowBase), false );			
	}

	void UIEditorMessageService::GetGUIContextRTTIClasses( DynamicPropertyManager ^manager)
	{
		ConvertUIRTTIClasses( manager, V_RUNTIME_CLASS(VDefaultGUIContext), false );			
	}

	void UIEditorMessageService::GetContextClassNames(ArrayList^ Classes)
	{
	//	VString sClassName;
	//	ConversionUtils::StringToVString(baseClass, sClassName);

		VType *pBaseType = Vision::GetTypeManager()->GetType("VDynamicGUIContext");

		VPOSITION pos = Vision::GetTypeManager()->GetStartType();
		while( pos )
		{
			VType *pType = Vision::GetTypeManager()->GetNextType(pos);
			if(!pType->m_pfnCreateObject && !pType->IsDerivedFrom(pBaseType))
				continue;

			System::String^ className;

			className = ConversionUtils::VStringToString(pType->m_lpszClassName);

			Classes->Add( className);

		}		
	}

	void UIEditorMessageService::GetClassNames( System::String^ baseClass , ArrayList^ Classes)
	{
		VString sClassName;
		ConversionUtils::StringToVString(baseClass, sClassName);


		  VType *pBaseType = Vision::GetTypeManager()->GetType(sClassName);

		  VPOSITION pos = Vision::GetTypeManager()->GetStartType();
		  while( pos )
		  {
			  VType *pType = Vision::GetTypeManager()->GetNextType(pos);
			  if(!pType->m_pfnCreateObject && !pType->IsDerivedFrom(pBaseType))
				  continue;

			  System::String^ className;

			  className = ConversionUtils::VStringToString(pType->m_lpszClassName);
			 
			  Classes->Add( className);

		  }


	//	ConvertUIRTTIClasses( manager, V_RUNTIME_CLASS(VWindowBase), false );	
		
	}

 	void UIEditorMessageService::ConvertUIRTTIClasses(DynamicPropertyManager ^manager, VType *pBaseType, bool bIgnoreBaseClass)
 	{
 		// loop through all types and filter out entity
 		VPOSITION pos = Vision::GetTypeManager()->GetStartType();
 		while( pos )
 		{
 			VType *pType = Vision::GetTypeManager()->GetNextType(pos);
 			if(!pType->m_pfnCreateObject || ((bIgnoreBaseClass || pType!=pBaseType) && !pType->IsDerivedFrom(pBaseType)))
 				continue;
 
 			VARIABLE_ELEM *pVar = NULL;
 			if (pType->m_pVarList)
 				pVar = pType->m_pVarList->first;
 
 			int iFlags = (int)DynamicPropertyCollectionType::DynamicPropertyCollectionFlags_e::None;
 			if (pType->IsHidden())
 				iFlags |= (int)DynamicPropertyCollectionType::DynamicPropertyCollectionFlags_e::Hidden;
 
 			DynamicPropertyCollectionType ^pClassProperties = manager->CreateNewType(
 				ConversionUtils::VStringToString(pType->m_lpszClassName), (DynamicPropertyCollectionType::DynamicPropertyCollectionFlags_e)iFlags);
 			pClassProperties->NativeVTypePtr = System::IntPtr((void *)pType);
 			if (pType->m_pVarList)
 			{
 				pClassProperties->SetDescription(ConversionUtils::VStringToString(pType->m_pVarList->m_szVarList_Desc));
 				pClassProperties->SetNiceName(ConversionUtils::VStringToString(pType->m_pVarList->m_szCustomModel));
 			}
 
 
 			int iVarIndex = 0; //defines the index of the variable
 			for (VARIABLE_ELEM *pIter = pVar;pIter;pIter=pIter->next)
 			{
 				VisVariable_cl *pVar = (VisVariable_cl *)pIter->value;
 				Type ^t;
 				DynamicPropertyType ^pPropType;
 				VulpType_e vulpType = (VulpType_e)pVar->type;
 
 				// hardcoded fix
 				if (!strcmp(pVar->name,"entityKey"))
 					vulpType = VULPTYPE_ENTITY_KEY;
 
 				if (vulpType==VULPTYPE_ENUM) 
 				{
 					// create enumeration property type
 					pPropType = ConversionUtils::CreateEngineEnumType(pVar);
 				}
 				else
 				{
 					t = ConversionUtils::ConvertType(vulpType , pVar->hintFlags );
 					if (!t)
 						continue;
 
 					// create property type
 					String ^szDefaultValue = ConversionUtils::VStringToString(pVar->defaultValue);
 					Object ^pDefaultObject = SerializationHelper::GetObjectFromStringData(t, szDefaultValue);
 					pPropType = gcnew DynamicPropertyType(ConversionUtils::VStringToString(pVar->name), t, pDefaultObject, ConversionUtils::VStringToString(pVar->desc), ConversionUtils::VStringToString (pVar->GetDisplayName ()));
 					// add an editor attribute that limits the number of chars - but only if there is no collision with other type editors
 					if (vulpType==VULPTYPE_STRING && (pVar->editorHintString==NULL || pVar->editorHintString[0]==0))
 						pPropType->AddFixedStringSizeAttribute((int)pVar->extraInfo-1);
 				}
 
 				pClassProperties->Add(pPropType);
 				if (pVar->category!=NULL)
 					pPropType->AddCategoryAttribute(ConversionUtils::VStringToString(pVar->category));
 
 				// Make sure the property order is preserved in the property grid
 				iVarIndex++;
 				pPropType->AddPropertyOrderAttribute(iVarIndex);
 				ConversionUtils::AddTypeSpecificAttributesOLD(vulpType, pPropType, pVar->hintFlags, pVar->editorHintString);
 			}
 
 			manager->AddCollectionType(pClassProperties);
 		}
 	}



}
