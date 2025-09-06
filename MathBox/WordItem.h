#pragma once
#include "MathItem.h"
#include "GlyphRun.h"

class CWordItem : public CMathItem {
   IDWriteFontFace* m_pFontFace;          //external, dnd!
   CGlyphRun        m_GlyphRun;
public:
   //CTOR/INIT
   CWordItem() = delete;
   CWordItem(IDWriteFontFace* pFontFace, const CMathStyle& style, EnumMathItemType eType = eacUNK, float fUserScale = 1.0f);
   bool SetText(PCWSTR szText) {
      vector<UINT32> vUniCodePoints;
      while (*szText) {
         vUniCodePoints.push_back((UINT32)*szText);
         ++szText;
      }
      return SetText(vUniCodePoints);
   }
   bool SetText(const vector<UINT32>& vUniCodePoints);
   bool SetGlyphIndexes(const vector<UINT16>& vGIndexes);
   //CMathItem Implementation
   void Draw(D2D1_POINT_2F ptAnchor, const SDWRenderInfo& dwri) override;
private:
   void OnInit_();
};