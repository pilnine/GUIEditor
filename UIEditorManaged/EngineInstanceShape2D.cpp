/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#include "UIEditorManagedPCH.h"
#include "EngineInstanceShape2D.hpp"

#include "../UIEngine/Source/VUINodePushButton.h"
#include "../UIEngine/Source/VUINodeImage.h"
#include "../UIEngine/Source/VUINodeDialog.h"

using namespace ManagedFramework;
using namespace System::Collections;
#using <mscorlib.dll>


namespace UIEditorManaged
{

  // create the native instance in the constructor
  UIEngineInstanceShape2D::UIEngineInstanceShape2D()
  {
	  m_pUIControlInstance = NULL; 
	  Initialize();	
  }

  UIEngineInstanceShape2D::UIEngineInstanceShape2D(bool Initialize)
  {
	  if ( Initialize == true )
	  {
		  UIEngineInstanceShape2D();
	  }	  
  }

  UIEngineInstanceShape2D::UIEngineInstanceShape2D( String ^sClass )
  {
	  VString vClassName;	
	  ConversionUtils::StringToVString( sClass, vClassName );

	  VType *pType = Vision::GetTypeManager()->GetType(vClassName);
	  if (!pType)
	  {
		  VASSERT(!"VDialogResource::CreateInstance failed because dialog class does not exist in type manager");
		  return;
	  }
	  //m_pUIControlInstance = (VWindowBase *)pType->CreateInstance();	 
	  m_pUIControlInstance = (VWindowBase *)pType->CreateInstance();	 

	  Initialize();	
  }


  UIEngineInstanceShape2D::UIEngineInstanceShape2D( String ^sClass, UIEngineInstanceShape2D^ parentDialog, Rectangle2D ^bound, StringDictionary ^propertyDict, bool bCallInitFunction )
  {
	  VString vClassName;	
	  ConversionUtils::StringToVString( sClass, vClassName );

	  VType *pType = Vision::GetTypeManager()->GetType(vClassName);
	  if (!pType)
	  {
		  VASSERT(!"VDialogResource::CreateInstance failed because dialog class does not exist in type manager");
		  return;
	  }
	  //m_pUIControlInstance = (VWindowBase *)pType->CreateInstance();	 
	  m_pUIControlInstance = (VWindowBase *)pType->CreateInstance();	 

	  UpdateProperties(propertyDict);
  }

  void UIEngineInstanceShape2D::UpdateProperties(StringDictionary ^propertyDict)
  {
	  if (propertyDict)
	  {
		  IEnumerator ^enumerator = propertyDict->GetEnumerator();
		  while (enumerator->MoveNext())
		  {
			  DictionaryEntry ^entry = dynamic_cast<DictionaryEntry ^> (enumerator->Current);
			  VString sKey, sValue;

			//  entry->Value.ToString()

			  ConversionUtils::StringToVString( dynamic_cast<String ^> (entry->Key), sKey );
			  ConversionUtils::StringToVString( dynamic_cast<String ^> (entry->Value), sValue );

			
			  
			  if ( m_pUIControlInstance)
				  m_pUIControlInstance->SetVariable(sKey,sValue);
		  }
	  }
  }

  void UIEngineInstanceShape2D::SetUIKey( String ^sVarName )
  {
 	  VString sKey;
 	  ConversionUtils::StringToVString( sVarName, sKey );
	  int ID = VUINodeMananger_cl::GlobalManager()->GetUIKey(sKey);
	  m_pUIControlInstance->SetUniqueID( ID );
  }

  void UIEngineInstanceShape2D::SetOrder( int iOrder )
  {
	  m_pUIControlInstance->SetOrder( iOrder );

	  if ( m_pUIControlInstance->GetParent() != NULL )
	  {
		  VUINodeDialog* pDlg = (VUINodeDialog *)m_pUIControlInstance->GetParent();
		  pDlg->Items().SortByPriority();
	  }

  }

   void UIEngineInstanceShape2D::Initialize()
   {
	   return; 
   }


   void UIEngineInstanceShape2D::OnAttachToParent(IEngineShapeInstance ^pNewParent)
   {
	   return;
   }

   void UIEngineInstanceShape2D::OnDetachFromParent()
   {
	   return;
   }
  
  // destroy the native instance of the node as well
   void UIEngineInstanceShape2D::DisposeObject()
   {
		   m_pUIControlInstance = NULL;
   }

  void UIEngineInstanceShape2D::SetPosition(float x,float y,float z)
  {
	  m_pUIControlInstance->SetPosition(x,y);
  }

  void UIEngineInstanceShape2D::SetOrientation(float /*yaw*/,float /*pitch*/,float /*roll*/)
  {
    // no orientation
  }

  void UIEngineInstanceShape2D::SetSize(float x,float y )
  {
	  m_pUIControlInstance->SetSize(x,y);
  }

  bool UIEngineInstanceShape2D::GetPosition(Vector3F %enginePosition)
  {
	  return true;
  }

  void UIEngineInstanceShape2D::GetSize(Vector2F %engineSize)
  {

  }

  Rectangle2D^ UIEngineInstanceShape2D::GetBound()
  {
	  Rectangle2D^ rect = gcnew Rectangle2D();

	  hkvVec2 pos = m_pUIControlInstance->GetPosition();
	  hkvVec2 size = m_pUIControlInstance->GetSize();

	  rect->X1 = pos.x;
	  rect->Y1 = pos.y;
	  rect->X2 = pos.x + size.x;
	  rect->Y2 = pos.y + size.y;

	  return rect;
  }


  bool UIEngineInstanceShape2D::GetLocalBoundingBox(BoundingBox^ %bbox)
  {
    // create a small bounding box for picking
    float fSize = 5.f;
    (*bbox).Set(-fSize,-fSize,-fSize,fSize,fSize,fSize);
    return true;
  }

  void UIEngineInstanceShape2D::TraceShape(Shape3D^ /* ownerShape*/, Vector3F /*rayStart*/,Vector3F /*rayEnd*/, ShapeTraceResult^% /*result*/)
  {
    // don't do anything because the shape code already does the picking
  }

  void UIEngineInstanceShape2D::LinkNode(UIEngineInstanceShape2D ^pOther)
  {
    // link to another instance
//     if (!m_pNode || !pOther->m_pNode) return;
//     if (!m_pNode->IsConnected(pOther->m_pNode)) // might already be linked
//       m_pNode->ConnectPoint(pOther->m_pNode);
  }

  void UIEngineInstanceShape2D::UnLinkNode(UIEngineInstanceShape2D ^pOther)
  {
    // unlink another instance
//     if (!m_pNode || !pOther->m_pNode) return;
//     if (m_pNode->IsConnected(pOther->m_pNode)) // might already be unlinked
//       m_pNode->DisconnectPoint(pOther->m_pNode);
  }

  bool UIEngineInstanceShape2D::OnExport(SceneExportInfo ^info)
  {
    // do the export: Get the export binary archive and serialize into it.
    // Requires the native node class to support serialization (derive from VisTypedEngineObject_cl and 
    // implement Serialize function)
//     VArchive &ar = *((VArchive *)info->NativeShapeArchivePtr.ToPointer());
//     ar.WriteObject(m_pUIControlInstance);
     return true;
  }

  void UIEngineInstanceShape2D::SendValueChangingMessage(String ^propertyName, String ^newValue)
  {
	  ConversionUtils::SendValueChangingMessage( (VisTypedEngineObject_cl*)m_pUIControlInstance ,propertyName, newValue);
  }

  bool UIEngineInstanceShape2D::SetVariable(String ^sVarName, String ^sVarData)
  {
	  // set the variable on the entity
	  VString sEngineVarName, sEngineVarData;
	  ConversionUtils::StringToVString( sVarName, sEngineVarName );
	  ConversionUtils::StringToVString( sVarData, sEngineVarData );

	  // result can be false if the property is from XML template:
	  bool bResult = (((VisTypedEngineObject_cl*)m_pUIControlInstance)->SetVariable(sEngineVarName, sEngineVarData) == TRUE);

	  // inform the entity about the property change
	//  pEntity->SendMsg( pEntity, VIS_MSG_EDITOR_PROPERTYCHANGED, (INT_PTR) sEngineVarName.AsChar(), (INT_PTR) sEngineVarData.AsChar() );

	  return bResult;
  }
}
