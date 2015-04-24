// TKBMS v1.0 -----------------------------------------------------
//
// PLATFORM       : ALL
// PRODUCT        : VISION 
// VISIBILITY     : PUBLIC
//
// ------------------------------------------------------TKBMS v1.0

#ifndef vCommonControls_h__
#define vCommonControls_h__

#include "UIEngineIncludes.hpp"
#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/GUI/VMenuIncludes.hpp>

class VUISharedData
{
public:
  NODE_IMPEXP static void Init();
  NODE_IMPEXP static void DeInit();

  NODE_IMPEXP static VisFont_cl* GetFont() { return s_spFont; }
  NODE_IMPEXP static float GetFontScale() { return s_fFontScale; }

  NODE_IMPEXP static float GetScale() { return s_fScale; }
//  NODE_IMPEXP static float GetDialogSpacing() { return s_fDialogSpacing; }
  NODE_IMPEXP static float GetIconSize() { return s_fIconSize; }
  NODE_IMPEXP static float GetScrollBarWidth() { return s_fScrollBarWidth; }
  NODE_IMPEXP static float GetTabsHeight() { return s_fTabsHeight; }
  NODE_IMPEXP static float GetStatusBarHeight() { return s_fStatusBarHeight; }
  NODE_IMPEXP static float GetPadding() { return s_fPadding; }
  NODE_IMPEXP static float GetOuterBorderWidth() { return s_fOuterBorderWidth; }
  NODE_IMPEXP static float GetBorderWidth() { return s_fBorderWidth; }

  NODE_IMPEXP static float GetUIScalingFactor();
  NODE_IMPEXP static hkvVec2 GetUIScalingVector();

  NODE_IMPEXP static float GetfReferenceResolutionX(){return 1920.0f;};
  NODE_IMPEXP static float GetfReferenceResolutionY(){return 1080.0f;};

private:
  static VisFontPtr s_spFont;
  static float s_fFontScale;
  
  static float s_fScale;
//  static float s_fDialogSpacing;
  static float s_fIconSize;
  static float s_fScrollBarWidth;
  static float s_fTabsHeight;
  static float s_fStatusBarHeight;
  static float s_fPadding;
  static float s_fOuterBorderWidth;
  static float s_fBorderWidth;
};

#endif // vCommonControls_h__
