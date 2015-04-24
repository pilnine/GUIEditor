/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

//  Basic Component Template
//  

#include "UIEnginePluginPCH.h"
#include "UIEventHandlerComponent.h"

//  Register the class in the engine module so it is available for RTTI
V_IMPLEMENT_SERIAL( UIEventHandlerComponent, IVObjectComponent, 0, &g_NodeModule);

//  Start up our manager class, one global instance of our manager
UIEventHandlerComponent_ComponentManager UIEventHandlerComponent_ComponentManager::g_GlobalManager;

//============================================================================================================
//  UIEventHandlerComponent Body
//============================================================================================================
//  Give the component some in Game or vForge Functionality
//

void UIEventHandlerComponent::onStartup( VisTypedEngineObject_cl *pOwner )
{
  //  do the component start up code here.......
  //  [...]

  Vision::Message.Add(1,"UIEventHandlerComponent - onStartup()");
  count = 0;
}

void UIEventHandlerComponent::onRemove(  VisTypedEngineObject_cl *pOwner )
{
  //  do the component Removal code here.......
  //  [...]
  Vision::Message.reset();
  Vision::Message.Add("UIEventHandlerComponent - onRemove()");
}

void UIEventHandlerComponent::onFrameUpdate()
{
  if (!GetOwner())return;
  //  Add code here to be performed every frame update.
  //  [...]

  //Example

  VisBaseEntity_cl *pEntity = (VisBaseEntity_cl*)GetOwner();  

  hkvVec3 vPos = ((VisObject3D_cl *)GetOwner())->GetPosition(); 
  //Vision::Message.DrawMessage3D(pEntity->GetObjectKey(),vPos);

   float x,y,z;   
  Vision::Contexts.GetCurrentContext()->Project2D(vPos, x, y, z);

  VisFont_cl* pInfo =  Vision::Fonts.FindFont("\\Fonts\\Nanumgothic.fnt" );//
  if ( pInfo == NULL )
  {
	  pInfo =  Vision::Fonts.LoadFont("\\Fonts\\Nanumgothic.fnt" ); // .m_spDebugFont = Vision::Fonts.LoadFont("\\Fonts\\Nanumgothic.fnt");
  }

  pInfo->PrintText(NULL, hkvVec2(x, y), pEntity->GetObjectKey(),VColorRef(255,0,0,255)); 
  Vision::Message.Print(1, 10, 100, "Counting : %i", count);
  count ++;
}

//============================================================================================================
//  UIEventHandlerComponent Overrides
//============================================================================================================
//  
void UIEventHandlerComponent::SetOwner( VisTypedEngineObject_cl *pOwner )
{
  IVObjectComponent::SetOwner( pOwner );
  if (pOwner!=NULL)
  {
    UIEventHandlerComponent_ComponentManager::GlobalManager().Instances().AddUnique(this);
    Vision::Message.Add("UIEventHandlerComponent - Component Instance created");
    onStartup( pOwner );
  }
  else
  {
    onRemove( pOwner );
    Vision::Message.Add("UIEventHandlerComponent - Removed Component Instance from component Manager");
    UIEventHandlerComponent_ComponentManager::GlobalManager().Instances().SafeRemove(this);    
  }
}

//----------------------------------------------------------------------------------------------------------  
//  UIEventHandlerComponent::Serialize(..)
//----------------------------------------------------------------------------------------------------------  
//  component's properties are automatically saved and restored on the vForge 
//  side via property reflection, custom code has to be implemented to 
//  serialize the component's properties into the exported vscene file. 
//
//  You should only save what is absolutely necessary for restoring the object's 
//  state and that cannot be recalculated at loading time. 
//----------------------------------------------------------------------------------------------------------
void UIEventHandlerComponent::Serialize( VArchive &ar )
{
  char iLocalVersion = UIEventHandlerComponent_VERSION_CURRENT ;
  IVObjectComponent::Serialize(ar);
  if (ar.IsLoading())
  {
    ar >> iLocalVersion;
    VASSERT_MSG(iLocalVersion == UIEventHandlerComponent_VERSION_CURRENT , "Invalid local version.");

    //  add your property variables here
    //  Load Data
    ar >> boolDemo;
    ar >> floatDemo;
    ar >> intDemo;
    ar >> colorDemo;    
  } 
  else
  {
    ar << iLocalVersion;
    
    //  Save Data
    ar << boolDemo;
    ar << floatDemo;
    ar << intDemo; 
    ar << colorDemo;   
  }
}


//============================================================================================================
//  Variable Table - Property variables can be exposed by the programmer 
//  and edited per instance by the artist  
//============================================================================================================
//
START_VAR_TABLE(UIEventHandlerComponent,IVObjectComponent,    "Component Description to show up in the editor", 
                          VVARIABLELIST_FLAGS_NONE, 
                          "UIEventHandlerComponentWidget" )
  
  DEFINE_VAR_BOOL    (UIEventHandlerComponent, boolDemo,    "This is an example of using bool, initialises to true", "TRUE", 0, NULL);
  DEFINE_VAR_ENUM    (UIEventHandlerComponent, enumDemo,    "test the enum", "Two", "One,Two,Three", 0, 0);
  DEFINE_VAR_FLOAT  (UIEventHandlerComponent, floatDemo,  "Demo of the float", "5.0", 0, NULL);
  DEFINE_VAR_INT    (UIEventHandlerComponent, intDemo,    "Show the int in use", "3", 0, 0);
  DEFINE_VAR_COLORREF  (UIEventHandlerComponent, colorDemo,  "show a color selection", "255/255/255/255", 0, 0);
  //DEFINE_VAR_STRING  (UIEventHandlerComponent, stringDemo,  "String value", "hello", 0, 0,NULL);


END_VAR_TABLE

/*
 * Havok SDK - Base file, BUILD(#20130717)
 * 
 * Confidential Information of Havok.  (C) Copyright 1999-2013
 * Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
 * Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
 * rights, and intellectual property rights in the Havok software remain in
 * Havok and/or its suppliers.
 * 
 * Use of this software for evaluation purposes is subject to and indicates
 * acceptance of the End User licence Agreement for this product. A copy of
 * the license is included with this software and is also available from salesteam@havok.com.
 * 
 */
