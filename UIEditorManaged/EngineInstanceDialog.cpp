#include "UIEditorManagedPCH.h"
#include "EngineInstanceDialog.hpp"
#include "EngineInstanceTextLabel.hpp"
#include "EngineInstanceImage.hpp"


using namespace ManagedFramework;


namespace UIEditorManaged
{


  // create the native instance in the constructor
  UIEngineInstanceDialog::UIEngineInstanceDialog()
  {	

  }

  bool UIEngineInstanceDialog::CreateBasicDialogInstance( String^ className , String^ InstanceName , UIEngineInstanceDialog^ parentDialog )
  {
	  VString sClassName;
	  ConversionUtils::StringToVString(className, sClassName);

	  VString sInstanceName;
	  ConversionUtils::StringToVString(InstanceName, sInstanceName);


	  VType *pType = Vision::GetTypeManager()->GetType(sClassName);
	  if (!pType)
	  {
		  VASSERT(!"VDialogResource::CreateInstance failed because dialog class does not exist in type manager");
		  return false;
	  }

	  m_pUIControlInstance = (VDialog *)pType->CreateInstance();

	  if (!m_pUIControlInstance->IsOfType(Vision::GetTypeManager()->GetType("VDialog")))
		  hkvLog::FatalError("class '%s' is not derived from base class VDialog",className);

	// VUINodeDialog::CreateInstance( (VUINodeDialog*)m_pUIControlInstance );

	// m_pUIControlInstance->InitDialog(pContext, this, (VDialog *)parentDialog->m_pUIControlInstance, DIALOGFLAGS_NONE);

	 return true;
  }

  void UIEngineInstanceDialog::SetFullScreen(bool bFullScreen)
  {
	  GetDialog()->SetFullScreen( bFullScreen );
	  GetDialog()->OnSizeChanged();
  }

  void UIEngineInstanceDialog::Initialize()
  {
	//  m_pUIControlInstance = VUINodeDialog::CreateInstance(NULL);
  }

  bool UIEngineInstanceDialog::BuildFromXML(String^ filename)
  {
	  VString sFilename;
	  ConversionUtils::StringToVString(filename, sFilename);

	  TiXmlDocument doc;
	  bool bResult = doc.LoadFile(sFilename.GetSafeStr());
	  if (!bResult)
	  {
		  hkvLog::Warning("GUI dialog resource '%s' failed to load : %s",sFilename.GetSafeStr(),doc.ErrorDesc());
		  return false;		
	  }

	  TiXmlElement *pDlgNode = XMLHelper::SubNode(doc.RootElement(),"DIALOG",false);
	  if (!pDlgNode)
		  return false;

	  char szPath[FS_MAX_PATH];
	  VFileHelper::GetFileDir( sFilename.GetSafeStr() , szPath );

	  return GetDialog()->Build( pDlgNode ,szPath, false );
  }

  bool UIEngineInstanceDialog::BuildToXML(String^ filename)
  {
	  VString sFilename;
	  ConversionUtils::StringToVString(filename, sFilename);


	  TiXmlDocument doc;
	  TiXmlElement root("root");
	  TiXmlNode *pRoot = doc.InsertEndChild(root);
	  TiXmlElement pDlgNode("DIALOG");
	  TiXmlNode *pAdded = pRoot->InsertEndChild(pDlgNode);
	
	  char szPath[FS_MAX_PATH];
	  VFileHelper::GetFileDir( sFilename.GetSafeStr() , szPath );

	  if ( GetDialog()->SaveToBinaryFile( sFilename ) == false )
		  return false;

 	  if ( GetDialog()->BuildToXML( sFilename ) == false )
 		  return false;

	//  doc.InsertEndChild(*pDlgNode);

	  // finally write doc to file
	//  if (doc.SaveFile(sFilename,NULL) == false )
	//  {  
	//	  return false;
	//  }

	  return true;
  }

  int UIEngineInstanceDialog::GetChildUIInstanceCount()
  {
	  return GetDialog()->Items().Count();
  }

  System::String^ UIEngineInstanceDialog::GetChildUIInstanceName(int iIndex)
  {
	  VDlgControlBase *control = GetDialog()->Items().GetAt(iIndex);
	  System::String^ name = ConversionUtils::VStringToString(control->GetTypeId()->m_lpszClassName);
	  return name;
  }

  Rectangle2D^ UIEngineInstanceDialog::GetChildUIInstanceRect(int iIndex)
  {
	  VDlgControlBase *control = GetDialog()->Items().GetAt(iIndex);

	  Rectangle2D^ rect = gcnew Rectangle2D();

	  hkvVec2 pos = control->GetPosition();
	  hkvVec2 size = control->GetSize();

	  rect->X1 = pos.x;
	  rect->Y1 = pos.y;
	  rect->X2 = pos.x + size.x;
	  rect->Y2 = pos.y + size.y;

	  return rect;
  }

  void UIEngineInstanceDialog::DisposeObject()
  {
	  if ( VUINodeMananger_cl::GlobalManager()->GetGUIContext()->GetOpenDialogs().Contains( GetDialog() )  == true )
	  {
		  VUINodeMananger_cl::GlobalManager()->GetGUIContext()->CloseDialog( GetDialog() );		
	  }

	    m_pUIControlInstance = NULL;
	//  UIEngineInstanceShape2D::DisposeObject();
  }

  void UIEngineInstanceDialog::SetVisibleDialog(bool bShow)
  {
	  GetDialog()->SetVisible(bShow);
	  GetDialog()->m_bVisible = bShow;
  }

  void UIEngineInstanceDialog::ShowDialog(bool bShow)
  {
	  if ( bShow )
	  {
		  if ( GetDialog() == NULL )
		  {
			  VASSERT(!"VDialogResource::CreateInstance failed because dialog class does not exist in type manager");
			  return;
		  }

		  VUINodeMananger_cl::GlobalManager()->ShowDialog( GetDialog() );	
	  }
	  else
	  {
		  VUINodeMananger_cl::GlobalManager()->CloseDialog( GetDialog() );	
		  m_pUIControlInstance = NULL;
	  }
  }

  void UIEngineInstanceDialog::SetBackgroundColor( UINT color )
  {
	  VColorRef vColor;
	  vColor.SetRGBA( color );
	  GetDialog()->SetBackgroundColor(vColor);	  
  }

  void UIEngineInstanceDialog::SetFadeIn(bool bFadeIn , float fFadeInPos , float fFadeInTime )
  {
	  if ( GetDialog() != NULL) GetDialog()->SetFadeIn(bFadeIn, fFadeInPos,fFadeInTime);
  }

  void UIEngineInstanceDialog::SetFadeOut(bool bFadeOut , UINT color , float fBackFadePos , float fFadeOutTime)
  {
	  VColorRef vColor;
	  vColor.SetRGBA( color );
	  if ( GetDialog() != NULL) GetDialog()->SetFadeOut( bFadeOut,vColor,fBackFadePos,fFadeOutTime);
  }

  void UIEngineInstanceDialog::OnAttachToParent(IEngineShapeInstance ^pNewParent)
  {
	  return;
  }

  void UIEngineInstanceDialog::OnDetachFromParent()
  {
	  return;
  }

  void UIEngineInstanceDialog::AddControl( IEngineShapeInstance ^control )
  {
	  GetDialog()->AddControl( (VDlgControlBase*)(((UIEngineInstanceShape2D^)control)->m_pUIControlInstance) );	
  }
  
  void UIEngineInstanceDialog::RemoveControl( IEngineShapeInstance ^control )
  {
	  GetDialog()->Items().Remove( ((VDlgControlBase*)((UIEngineInstanceShape2D^)control)->m_pUIControlInstance) );
  }


  bool UIEngineInstanceDialog::OnExport(SceneExportInfo ^info)
  {
	  // do the export: Get the export binary archive and serialize into it.
	  // Requires the native node class to support serialization (derive from VisTypedEngineObject_cl and 
	  // implement Serialize function)
	  VArchive &ar = *((VArchive *)info->NativeShapeArchivePtr.ToPointer());

	  VUINodeDialog* m_pDialog =  (VUINodeDialog*) m_pUIControlInstance;	
	  ar.WriteObject( m_pDialog );
	  
	  return true;
  }
}
