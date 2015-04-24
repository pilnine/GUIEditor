// TKBMS v1.0 -----------------------------------------------------
//
// PLATFORM       : ALL
// PRODUCT        : VISION 
// VISIBILITY     : PUBLIC
//
// ------------------------------------------------------TKBMS v1.0

#include "UIEnginePluginPCH.h"
#include "vCommonControls.hpp"

#include <Vision/Runtime/Framework/VisionApp/VAppModule.hpp>

VisFontPtr VUISharedData::s_spFont;
float VUISharedData::s_fFontScale = 2.00f;

float VUISharedData::s_fScale = 3.0f;
//float VUISharedData::s_fDialogSpacing = 0.0f;
float VUISharedData::s_fIconSize = 48.0f;
float VUISharedData::s_fScrollBarWidth = 16.0f;
float VUISharedData::s_fTabsHeight = 34.0f;
float VUISharedData::s_fStatusBarHeight = 28.0f;
float VUISharedData::s_fPadding = 4.0f;
float VUISharedData::s_fOuterBorderWidth = 5.0f;
float VUISharedData::s_fBorderWidth = 1.5f;


float VUISharedData::GetUIScalingFactor()
{
	const float fReferenceResolution = 1280.0f;
	const float fReferenceDisplaySize = 4.6f;
	const float fKernscheFakeKonstante = 1.73f;
	const float fBagarscherFakeExponent = 0.2f;

	float fScale = 2.0f;
	const hkvVec2 vRes = hkvVec2((float)Vision::Video.GetXRes(), (float)Vision::Video.GetYRes());

#if defined(_VISION_MOBILE) || defined (_VISION_PSP2)  
	const float fDisplaySizeInInch = vRes.getLength() / Vision::Video.GetDeviceDpi();  

	fScale = (vRes.x / fReferenceResolution) * fKernscheFakeKonstante * hkvMath::pow(fReferenceDisplaySize / fDisplaySizeInInch, fBagarscherFakeExponent);

#elif !defined(WIN32)
	fScale = vRes.x / fReferenceResolution;

#endif

	return hkvMath::Max(fScale, 0.5f);
}

hkvVec2 VUISharedData::GetUIScalingVector()
{
	const float fReferenceResolution = 1280.0f;

	const hkvVec2 vRes = hkvVec2((float)Vision::Video.GetXRes(), (float)Vision::Video.GetYRes());

	float fScaleX = vRes.x / GetfReferenceResolutionX();
	float fScaleY = vRes.y / GetfReferenceResolutionY();

	return hkvVec2(fScaleX,fScaleY);
}


void VUISharedData::Init()
{
  s_fScale = VUISharedData::GetUIScalingFactor();
  s_fFontScale *= s_fScale;
  
  //s_fDialogSpacing *= s_fScale;
  s_fIconSize *= s_fScale;
  s_fScrollBarWidth *= s_fScale;
  s_fTabsHeight *= s_fScale;
  s_fStatusBarHeight *= s_fScale;
  s_fPadding *= s_fScale;
  s_fOuterBorderWidth *= s_fScale;
  s_fBorderWidth = hkvMath::ceil(s_fBorderWidth * s_fScale);

 // s_spFont = &Vision::Fonts.DebugFont();
}

void VUISharedData::DeInit()
{
  s_spFont = NULL;
}
