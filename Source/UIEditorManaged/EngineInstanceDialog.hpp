#pragma once

using namespace CSharpFramework;
using namespace CSharpFramework::Math;
using namespace CSharpFramework::Shapes;
using namespace CSharpFramework::Scene;
using namespace CSharpFramework::Math;



#include "EngineInstanceShape2D.hpp"
#include "../UIEngine/Source/VUINodeDialog.h"

namespace UIEditorManaged
{
  /// <summary>
  /// EngineInstanceNode : class that manages the native class
  /// </summary>
  public ref class UIEngineInstanceDialog : public UIEngineInstanceShape2D
  {
  public:     

    UIEngineInstanceDialog();


	VOVERRIDE void Initialize() override; 
	VOVERRIDE void DisposeObject() override;

	VOVERRIDE bool BuildFromXML(String^ filename );
	VOVERRIDE bool BuildToXML(String^ filename );

	VOVERRIDE bool CreateBasicDialogInstance( String^ className , String^ InstanceName , UIEngineInstanceDialog^ parentDialog);

	VOVERRIDE int GetChildUIInstanceCount();
	VOVERRIDE System::String^ GetChildUIInstanceName(int iIndex);
	VOVERRIDE Rectangle2D^ GetChildUIInstanceRect(int iIndex);

	VOVERRIDE VUINodeDialog* GetDialog() { return static_cast<VUINodeDialog*>( m_pUIControlInstance ); }


	VOVERRIDE void SetVisibleDialog(bool bShow);
	VOVERRIDE void ShowDialog(bool bShow);
	VOVERRIDE void SetFullScreen(bool bFullScreen);

//	VOVERRIDE void SetTexture(System::String^ filename);
	VOVERRIDE void SetBackgroundColor( UINT color );	

	VOVERRIDE void SetFadeIn(bool bFadeIn , float fFadeInPos , float fFadeInTime );
	VOVERRIDE void SetFadeOut(bool bFadeOut , UINT color , float fBackFadePos , float fFadeOutTime);

	

	VOVERRIDE void OnAttachToParent(IEngineShapeInstance ^pNewParent) override;
	VOVERRIDE void OnDetachFromParent() override;

	VOVERRIDE void AddControl( IEngineShapeInstance ^control );
	VOVERRIDE void RemoveControl( IEngineShapeInstance ^control );

	VOVERRIDE bool OnExport(SceneExportInfo ^info) override;

	//  VOVERRIDE void SetUIKey( String ^sVarName ) override;
  };
}
