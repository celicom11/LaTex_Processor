#pragma once
#include "MathItem.h"

//TeX glue wrapper, smart spacing/penalty item
class CGlueItem : public CMathItem {
//TYPES
   enum MuSkipType { emstNoSpace = 0, emstThinMuSkip = 1, emstMedMuSkip = 2, emstThickMuSkip = 3 };

//DATA
   float m_fMin{ 0 };         //shrink size in EM
   float m_fNorm{ 0 };        //normal size in EM
   float m_fMax{ 0 };         //stretch size in EM!
   float m_fActual{ 0 };      //actual size in EM, set during formatting/layout
public:
//CTOR/DTOR
   CGlueItem(float fKernEM, const CMathStyle& style, float fUserScale = 1.0f) : 
      CMathItem(eacGlue, style, fUserScale){
      m_fMin = m_fNorm = m_fMax = m_fActual = fKernEM;
      UpdateBox_();
   }
   CGlueItem(float fMin, float fNorm, float fMax, const CMathStyle& style, float fUserScale = 1.0f) :
      CMathItem(eacGlue, style, fUserScale), m_fMin(fMin), m_fNorm(fNorm),m_fMax(fMax) {
       m_fActual = fNorm;
      UpdateBox_();
   }
//ATTS
   float Min() const { return m_fMin; }
   float Norm() const { return m_fNorm; }
   float Max() const { return m_fMax; }
   float Actual() const { return m_fActual; }
   void SetActual(float fActual) { m_fActual = fActual; UpdateBox_(); }
   bool IsStretchable() const { return m_fMax > m_fNorm; }
   bool IsShrinkable() const { return m_fMin < m_fNorm; }
//METHODS
   //factory for intermediate glue 
   static CGlueItem* _Create(CMathItem* pPrev, CMathItem* pNext, const CMathStyle& style, float fUserScale = 1.0);
//CMathItem implementation
   void Select(bool bSelect) override {} //ntd
   void Draw(D2D1_POINT_2F ptAnchor, const SDWRenderInfo& dwri) override {} //ntd
private:
   static MuSkipType _GetMuSkipType_(const CMathStyle& style, CMathItem* pPrev, CMathItem* pNext);
   void UpdateBox_();
};