#pragma once
#include "MathItem.h"
#define GLUE_MAX 10000000.0f //arbitrary large value for infinite stretch

//TeX \hskip glue wrapper: spacing+penalty functionality
class CGlueItem : public CMathItem {
//TYPES
   enum MuSkipType { emstNoSpace = 0, emstThinMuSkip = 1, emstMedMuSkip = 2, emstThickMuSkip = 3 };

//DATA
   uint16_t  m_nInfShrink{ 0 };        //infinite shrink level, 0=normal, 1=fil
   uint16_t  m_nInfStretch{ 0 };       //infinite stretch level, 0=normal, 1=fil, 2=fill, 3=filll
   float    m_fMin{ 0 };               //shrink size in EM, ignored if InfShrink>0
   float    m_fNorm{ 0 };              //normal size in EM
   float    m_fMax{ 0 };               //stretch size in EM, ignored if InfStretch>0
   float    m_fActual{ 0 };            //actual size in EM, set during formatting/layout
public:
//CTOR/DTOR
   CGlueItem(float fKernEM, const CMathStyle& style, float fUserScale = 1.0f, 
             uint16_t nInfShrink = 0, uint16_t nInfStretchLevel = 0) :
      CMathItem(eacGLUE, style, fUserScale) {
      m_fMin = m_fNorm = m_fMax = m_fActual = fKernEM;
      m_nInfShrink = nInfShrink;
      m_nInfStretch = nInfStretchLevel;
      UpdateBox_();
   }
   CGlueItem(float fMin, float fNorm, float fMax, const CMathStyle& style, float fUserScale = 1.0f) :
      CMathItem(eacGLUE, style, fUserScale), m_fMin(fMin), m_fNorm(fNorm),m_fMax(fMax) {
       m_fActual = fNorm;
      UpdateBox_();
   }
//ATTS
   uint16_t InfShrinkLevel() const { return m_nInfShrink; }
   uint16_t InfStretchLevel() const { return m_nInfStretch; } //0=normal, 1=fil, 2=fill, 3=filll
   float Min() const { return m_nInfShrink ?-GLUE_MAX : m_fMin; }
   float Norm() const { return m_fNorm; }
   float Max() const { return m_nInfStretch? GLUE_MAX :m_fMax; }
   float Actual() const { return m_fActual; }
   float StretchCapacity() const { return m_nInfStretch ? GLUE_MAX : m_fMax - m_fNorm;}
   float ShrinkCapacity() const { return m_nInfShrink ? GLUE_MAX : m_fNorm - m_fMin; }
   void SetActual(float fActual) { m_fActual = fActual; UpdateBox_(); }
   bool IsStretchable() const { return m_nInfStretch || m_fMax > m_fNorm; }
   bool IsShrinkable() const { return m_nInfShrink || m_fMin < m_fNorm; }
//METHODS
   void StretchByRatio(float fRatio) {
      _ASSERT_RET(0 == m_nInfStretch, );
      m_fActual = m_fNorm + (m_fMax - m_fNorm) * fRatio;
      UpdateBox_();
   }
   //factory for intermediate glue 
   static CGlueItem* _Create(CMathItem* pPrev, CMathItem* pNext, const CMathStyle& style, float fUserScale = 1.0);
//CMathItem implementation
   void Select(bool bSelect) override {} //ntd
   void Draw(D2D1_POINT_2F ptAnchor, const SDWRenderInfo& dwri) override {} //ntd
private:
   static MuSkipType _GetMuSkipType_(const CMathStyle& style, CMathItem* pPrev, CMathItem* pNext);
   void UpdateBox_();
};