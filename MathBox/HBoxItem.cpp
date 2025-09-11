#include "stdafx.h"
#include "HBoxItem.h"

float CHBoxItem::NormWidth() const {
   float fWidth = 0;
   for (CMathItem* pItem : m_vItems) {
      if (pItem->Type() == eacGLUE)
         fWidth += ((CGlueItem*)pItem)->Norm();
      else if (pItem->Type() == eacHBOX)
         fWidth += ((CHBoxItem*)pItem)->NormWidth();
      else
         fWidth += pItem->Box().Width();
   }
   for (CGlueItem* pGlue : m_vGlues) {
      if (pGlue)
         fWidth += pGlue->Norm();
   }
   return fWidth;
}
float CHBoxItem::ActualWidth() const {
   float fWidth = 0;
   for (CMathItem* pItem : m_vItems) {
      fWidth += pItem->Box().Width();
   }
   for (CGlueItem* pGlue : m_vGlues) {
      if (pGlue)
         fWidth += pGlue->Actual();
   }
   return fWidth;
}
float CHBoxItem::MinWidth() const {
   if(m_bHasInfShrinkGlue)
      return 0; //infinite shrinkable glue, min width is 0
   float fWidth = 0;
   for (CMathItem* pItem : m_vItems) {
      if (pItem->Type() == eacGLUE)
         fWidth += ((CGlueItem*)pItem)->Min();
      else if (pItem->Type() == eacHBOX)
         fWidth += ((CHBoxItem*)pItem)->MinWidth();
      else
         fWidth += pItem->Box().Width();
   }
   for (CGlueItem* pGlue : m_vGlues) {
      if (pGlue)
         fWidth += pGlue->Min();
   }
   return fWidth;
}
//NOTE: actual width may be MORE than max stretch!
float CHBoxItem::MaxStretch() const {
   if(m_nInfStretchLevel)
      return GLUE_MAX; //arbitrary large value for infinite stretch
   float fWidth = 0;
   for (CMathItem* pItem : m_vItems) {
      if (pItem->Type() == eacGLUE) {
         if(((CGlueItem*)pItem)->InfStretchLevel())
            return GLUE_MAX; //arbitrary large value for infinite stretch
         fWidth += ((CGlueItem*)pItem)->Max();
      }
      else if (pItem->Type() == eacHBOX) {
         if (GLUE_MAX == ((CHBoxItem*)pItem)->MaxStretch())
            return GLUE_MAX; //arbitrary large value for infinite stretch
         fWidth += ((CHBoxItem*)pItem)->MaxStretch();
      }
      else
         fWidth += pItem->Box().Width();
   }
   for (CGlueItem* pGlue : m_vGlues) {
      if (pGlue) {
         if(pGlue->InfStretchLevel())
            return GLUE_MAX; //arbitrary large value for infinite stretch
         fWidth += pGlue->Max();
      }
   }
   return fWidth;

}
uint16_t CHBoxItem::CountInfStretchGlues(uint16_t nLevel) const {
   uint16_t nCount = 0;
   for (CMathItem* pItem : m_vItems) {
      if (pItem->Type() == eacGLUE) {
         if (((CGlueItem*)pItem)->InfStretchLevel() == nLevel)
            ++nCount;
      }
      else if (pItem->Type() == eacHBOX) {
         nCount += ((CHBoxItem*)pItem)->CountInfStretchGlues(nLevel);
      }
   }
   for (CGlueItem* pGlue : m_vGlues) {
      if (pGlue->InfStretchLevel() == nLevel)
         ++nCount;
   }
   return nCount;
}
uint16_t CHBoxItem::CountInfShrinkableGlues() const {
   uint16_t nCount = 0;
   for (CMathItem* pItem : m_vItems) {
      if (pItem->Type() == eacGLUE) {
         if (((CGlueItem*)pItem)->InfShrinkLevel())
            ++nCount;
      }
      else if (pItem->Type() == eacHBOX) {
         nCount += ((CHBoxItem*)pItem)->CountInfShrinkableGlues();
      }
   }
   for (CGlueItem* pGlue : m_vGlues) {
      if (pGlue->InfShrinkLevel())
         ++nCount;
   }
   return nCount;
}

bool CHBoxItem::AddItem(CMathItem* pItem) {
   _ASSERT_RET(pItem, false);
   if (pItem->Type() == eacGLUE) {
      if (((CGlueItem*)pItem)->InfShrinkLevel())
         m_bHasInfShrinkGlue = true;
      if (((CGlueItem*)pItem)->InfStretchLevel() > m_nInfStretchLevel)
         m_nInfStretchLevel = ((CGlueItem*)pItem)->InfStretchLevel();
   }
   else if (pItem->Type() == eacHBOX) {
      if (((CHBoxItem*)pItem)->InfStretchLevel() > m_nInfStretchLevel)
         m_nInfStretchLevel = ((CHBoxItem*)pItem)->InfStretchLevel();
      if (((CHBoxItem*)pItem)->InfShrinkLevel())
         m_bHasInfShrinkGlue = true;
   }
   if (!m_vItems.empty()) {
      CMathItem* pPrev = m_vItems.back();
      CGlueItem* pGlue = CGlueItem::_Create(pPrev, pItem, m_Style, m_fUserScale);
      m_vGlues.push_back(pGlue); //may be nullptr if no glue needed!
   }
   else //first item: copy STexBox to initialize
      m_Box = pItem->Box();
   m_vItems.push_back(pItem);
   return m_fWidth <= 0 || MinWidth() <= m_fWidth; //if Width is defined, check if new item fits
}

//adjusts items both vertically and horizontally to fit in m_Box.Width with min Badness
//returns false items can't fit the box/
bool CHBoxItem::UpdateLayout(float fWidth) {
   float fNormW = NormWidth();
   if (fWidth > 0)
      m_fWidth = fWidth;
   else
      m_fWidth = fNormW; //ideal width
   _ASSERT_RET(m_fWidth >= 0, false);//can it be negative?

   float fMinW = MinWidth();
   if(fMinW > m_fWidth)
      return false; //can't fit even with max shrink

   //1. adust items vertically by the baseline of the SBox AND update box top/bottom
   const int32_t nAscent = m_Box.Ascent();
   for (CMathItem* pItem : m_vItems) {
      int32_t nItemAscent = pItem->Box().Ascent();
      int32_t nDY = nAscent - nItemAscent;
      pItem->MoveBy(0, nDY);
      if (pItem->Type() == eacGLUE)
         ((CGlueItem*)pItem)->SetActual(((CGlueItem*)pItem)->Norm());
      else if (pItem->Type() == eacHBOX)
         ((CHBoxItem*)pItem)->UpdateLayout(0); //update sub-boxes too

      m_Box.Union(pItem->Box());//only top/bottom are valid now!
   }
   //2.Horizontal adjustment
   //2.1 if underfill, stretch glues
   if (m_fWidth >= fNormW) {
      if(m_nInfStretchLevel) //2.1.1 infinite stretch glues
         StretchInfGluesBy_(m_nInfStretchLevel, (m_fWidth - fNormW)/ CountInfStretchGlues(m_nInfStretchLevel));
      else {//2.1.2 finite stretch glues
         float fDelta = m_fWidth - fNormW;//0 is ok!
         float fStretchCapacity = MaxStretch() - fNormW;
         //if no stretch needed => Norm width
         if (fStretchCapacity > 0 || fDelta == 0.0f) {
            float fRatio = fDelta == 0.0f ? 0.0f : fDelta / fStretchCapacity;//maybe > 1.0!
            StretchBy_(fRatio);
         }
      }
   }
   else { //2.2 overflow, shrink glues
      if(m_bHasInfShrinkGlue) //2.2.1 infinite shrink glues
         ShrinkInfGluesBy_((fNormW - m_fWidth) / CountInfShrinkableGlues());
      else //2.2.2 finite shrink glues
      {
         float fDelta = fNormW - m_fWidth; //>0
         float fFiniteCapacity = fNormW - fMinW;
         if (fFiniteCapacity > 0) {
            float fRatio = fDelta / fFiniteCapacity; //<0
            ShrinkBy_(fRatio);
         }
      }
   }
   //3. position items 
   float fX = (float)m_Box.Left(); m_Box.nAdvWidth = 0; //reset width
   for(UINT nIdx = 1; nIdx < (UINT)m_vItems.size(); ++nIdx) {
      CMathItem* pItem = m_vItems[nIdx];
      CMathItem* pPrevItem = m_vItems[nIdx-1];
      CGlueItem* pGlue = m_vGlues[nIdx - 1];
      fX += pPrevItem->Box().Width();
      if(pGlue)
         fX += pGlue->Actual();
      pItem->MoveTo(F2NEAREST(fX), pItem->Box().Top());
   }
   //4. update box width
   m_Box.nAdvWidth = m_vItems.back()->Box().Right() - m_Box.Left();
   return true;
}
void CHBoxItem::ShrinkInfGluesBy_(float fShrink) {
   for (CMathItem* pItem : m_vItems) {
      if (pItem->Type() == eacGLUE) {
         if (((CGlueItem*)pItem)->InfShrinkLevel())
            ((CGlueItem*)pItem)->SetActual(((CGlueItem*)pItem)->Norm() - fShrink); //negative fShrink ok
      }
      else if (pItem->Type() == eacHBOX)
         ((CHBoxItem*)pItem)->ShrinkInfGluesBy_(fShrink);
      
   }
   for (CGlueItem* pGlue : m_vGlues) {
      if (pGlue->InfStretchLevel())
         pGlue->SetActual(pGlue->Norm() - fShrink); //negative fShrink ok
   }
}
//level 0 shrink
void CHBoxItem::ShrinkBy_(float fRatio) {
   for (CMathItem* pItem : m_vItems) {
      if (pItem->Type() == eacGLUE && ((CGlueItem*)pItem)->IsShrinkable())
        ((CGlueItem*)pItem)->SetActual(((CGlueItem*)pItem)->Norm()*(1-fRatio) + ((CGlueItem*)pItem)->Min()*fRatio);
      else if (pItem->Type() == eacHBOX)
         ((CHBoxItem*)pItem)->ShrinkBy_(fRatio);
      
   }
   for (CGlueItem* pGlue : m_vGlues) {
      if (pGlue->IsShrinkable())
         pGlue->SetActual(pGlue->Norm() * (1 - fRatio) + (pGlue->Min() * fRatio));
   }
}
void CHBoxItem::StretchInfGluesBy_(uint16_t nLevel, float fStretch) {
   _ASSERT(nLevel > 0);
   for (CMathItem* pItem : m_vItems) {
      if (pItem->Type() == eacGLUE) {
         if (((CGlueItem*)pItem)->InfStretchLevel() == nLevel)
            ((CGlueItem*)pItem)->SetActual(((CGlueItem*)pItem)->Norm() + fStretch);
      }
      else if (pItem->Type() == eacHBOX)
         ((CHBoxItem*)pItem)->StretchInfGluesBy_(nLevel, fStretch);
      
   }
}
void CHBoxItem::StretchBy_(float fRatio) {
   for (CMathItem* pItem : m_vItems) {
      if (pItem->Type() == eacGLUE && ((CGlueItem*)pItem)->IsStretchable())
         ((CGlueItem*)pItem)->StretchByRatio(fRatio);
      else if (pItem->Type() == eacHBOX)
         ((CHBoxItem*)pItem)->StretchBy_(fRatio);
   }
   for (CGlueItem* pGlue : m_vGlues) {
      if (pGlue->IsStretchable())
         pGlue->StretchByRatio(fRatio);
   }
}
//todo: test me!
float CHBoxItem::CalcBadness() const {
   float fDelta = 0, fFiniteCapacity = 0;
   float fNormW = NormWidth();
   float fMinW = MinWidth();

   if (fNormW < m_fWidth) { // underfill
      float fMaxW = MaxStretch();
      if(fMaxW == GLUE_MAX)
         return 0; //infinite stretchable glue, no badness
      //otherwise the sum of the finite stretchable glues is
      fDelta = m_fWidth - fNormW;
      fFiniteCapacity = fMaxW - fNormW;
   }
   else if (fNormW > m_fWidth) { // overfill
      // Check for infinite shrink (you'd need to add this logic to MinWidth())
      if (m_bHasInfShrinkGlue)
         return 0; //infinite shrinkable glue, no badness
      fDelta = m_fWidth - fNormW; //<0 to indicate overfill
      fFiniteCapacity = fNormW - MinWidth();
   }
   else
      return 0; //no badness if fits or smaller
   if (fFiniteCapacity <= 0)
      return float(fDelta > 0 ? GLUE_MAX : -GLUE_MAX); // no glue available, maximum badness

   float ratio = fDelta / fFiniteCapacity;
   return min(100.0f * ratio * ratio * ratio, 10000.0f);
}

