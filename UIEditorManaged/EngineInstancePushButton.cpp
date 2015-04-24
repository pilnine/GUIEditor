#include "UIEditorManagedPCH.h"
#include "EngineInstancePushButton.hpp"

#include "../UIEngine/Source/VUINodePushButton.h"

#include "../UIEngine/Source/VUINodeColorButton.h"
#include "../UIEngine/Source/VUINodeStyleButton.h"

using namespace ManagedFramework;

namespace UIEditorManaged
{

  // create the native instance in the constructor
  UIEngineInstancePushButton::UIEngineInstancePushButton()
  {
	  
	
  }

  void UIEngineInstancePushButton::Initialize()
  {
	  //if ( m_className->Equals("VUINodePushButton"))
	//	  m_pUIControlInstance = VUINodePushButton::CreateInstance();
	  //else if ( m_className->Equals("VUINodeColorButton"))
	//	  m_pUIControlInstance = VUINodePushButton::CreateInstance();
	 // else if ( m_className->Equals("VUINodeStyleButton"))
		  m_pUIControlInstance = VUINodePushButton::CreateInstance();
  }

  void UIEngineInstancePushButton::ReplaceButtonControl( String^ className )
  {	
	  m_pUIControlInstance = NULL;

	  if ( className->Equals("VUINodePushButton"))
	  	  m_pUIControlInstance = VUINodePushButton::CreateInstance();
	  else if ( className->Equals("VUINodeColorButton"))
	  	  m_pUIControlInstance = VUINodeColorButton::CreateInstance();
	   else if ( className->Equals("VUINodeStyleButton"))
	  m_pUIControlInstance = VUINodeStyleButton::CreateInstance();
  }


  void UIEngineInstancePushButton::SetClass( String^ className )
  {	 
//	  if ( m_pUIControlInstance != NULL )
//	  {
//		  delete m_pUIControlInstance;
		  m_pUIControlInstance = NULL;		  
//	  }


	  VString sClassName;
	  ConversionUtils::StringToVString(className, sClassName);

	  VType *pType = Vision::GetTypeManager()->GetType(sClassName);
	  if (!pType)
	  {
		  VASSERT(!"VDialogResource::CreateInstance failed because dialog class does not exist in type manager");
		  return;
	  }

	  m_pUIControlInstance = (VWindowBase *)pType->CreateInstance();


  }
}
