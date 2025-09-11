#include "stdafx.h"
#include "GlueItem.h"

namespace { //static helpers
   // TeX atom spacing matrix in D/T mode; values in muskip units/MuSkipType
   constexpr int16_t ATOM_SPACING_MATRIX[8][8] = {
      //L\R      Ord  Op   Bin  Rel  Open Close Punct Inner
      /*Ord*/  { 0,   1,   2,   3,   0,    0,    0,    1 },   // Ordinary
      /*Op*/   { 1,   1,   0,   3,   0,    0,    0,    1 },   // Large operator  
      /*Bin*/  { 2,   2,   0,   0,   2,    0,    0,    2 },   // Binary operator
      /*Rel*/  { 3,   3,   0,   0,   3,    0,    0,    3 },   // Relation
      /*Open*/ { 0,   0,   0,   0,   0,    0,    0,    0 },   // Opening delimiter
      /*Close*/{ 0,   1,   2,   3,   0,    0,    0,    1 },   // Closing delimiter
      /*Punct*/{ 1,   1,   1,   1,   1,    1,    1,    1 },   // Punctuation
      /*Inner*/{ 1,   1,   2,   3,   1,    0,    1,    1 }    // Inner (fractions, etc.)
   };
}

CGlueItem* CGlueItem::_Create(CMathItem* pPrev, CMathItem* pNext, const CMathStyle& style, float fUserScale) {
   _ASSERT_RET(pPrev != nullptr && pNext != nullptr && pPrev != pNext, nullptr);//snbh!
   MuSkipType eMSType = _GetMuSkipType_(style, pPrev, pNext);
   switch (eMSType) {
   case emstThinMuSkip:
      return new CGlueItem(THINMUSKIP, style, fUserScale);
   case emstMedMuSkip:
      return new CGlueItem(MEDMUSKIP_MIN, MEDMUSKIP, MEDMUSKIP_MAX, style, fUserScale);
   case emstThickMuSkip:
      return new CGlueItem(THICKMUSKIP, THICKMUSKIP, THICKMUSKIP_MAX, style, fUserScale);
   }
   return nullptr; //no glue needed!
}
CGlueItem::MuSkipType CGlueItem::_GetMuSkipType_(const CMathStyle& style, CMathItem* pPrev, CMathItem* pNext) {
   if(pPrev->Type() == eacGLUE || pNext->Type() == eacGLUE)
      return emstNoSpace;//no extra space between glues!
   //map prev/next types to matrix indices 0=Ord, 1=Op, 2=Bin, 3=Rel, 4=Open, 5=Close, 6=Punct, 7=Inner
   int nPrevClass = pPrev->AtomType(false);
   int nNextClass = pNext->AtomType(true);
   if (style.Style() <= etsText)
      return (MuSkipType)ATOM_SPACING_MATRIX[nPrevClass][nNextClass];
   //else
   if(nPrevClass== etaOP && nNextClass == etaBIN)
      return emstThinMuSkip; //in D/T mode, Op-Bin is thin space
   return emstNoSpace; //no space in other cases in D/T mode
}
void CGlueItem::UpdateBox_() {
   float fScale = m_fUserScale * m_Style.StyleScale();
   m_Box.nHeight = F2NEAREST(otfUnitsPerEm * fScale); //fontsize in em = otfUnitsPerEm units
   m_Box.nAscent = F2NEAREST(otfAscent * fScale);
   m_Box.nAdvWidth = F2NEAREST(m_fActual * otfUnitsPerEm * fScale);
}