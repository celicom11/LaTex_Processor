#pragma once
#include "ContainerItem.h"
#include "GlueItem.h"

class CHBoxItem : public CContainerItem {
   //DATA
   bool               m_bHasInfShrinkGlue{ false };
   uint16_t           m_nInfStretchLevel{ 0 };
   float              m_fWidth{ 0 }; //~const; desired width in EM
   vector<CGlueItem*> m_vGlues;
public:
   //CTOR/DTOR
   CHBoxItem() = delete;
   CHBoxItem(const CMathStyle& style, float fWidth = 0.0f ) :
      CContainerItem(eacHBOX, style), m_fWidth(fWidth) {
   }
   ~CHBoxItem() {
      Clear();
   }
   //ATTS
   float TargetWidth() const { return m_fWidth; }
   float ActualWidth() const;
   float NormWidth() const;
   float MinWidth() const;
   float MaxStretch() const;
   float CalcBadness() const; //>0 -undefill, 0-perfect, <0-overfill
   uint16_t InfShrinkLevel() const {
      return m_bHasInfShrinkGlue ? 1 : 0;
   }
   uint16_t InfStretchLevel() const { return m_nInfStretchLevel; }
   uint16_t CountInfStretchGlues(uint16_t nLevel) const;
   uint16_t CountInfShrinkableGlues() const;
   //METHODS
   void Clear() {
      CContainerItem::Clear();
      for (CGlueItem* pGlue : m_vGlues)
         delete pGlue;
      m_vGlues.clear();
   }
   bool AddItem(CMathItem* pItem);
   //must be called after all items are added
   //adjusts items both vertically and horizontally to fit in m_Box.Width with min Badness
   //returns false items can't fit the box/
   bool UpdateLayout(float fWidth);
private:
   void ShrinkInfGluesBy_(float fShrink);
   void ShrinkBy_(float fRatio);  //level 0 shrink
   void StretchInfGluesBy_(uint16_t nLevel, float fStretch);
   void StretchBy_(float fRatio); //level 0 stretch
};