#include "stdafx.h"
#include "..\stdafx.h"
#include "WordItem.h"

CWordItem::CWordItem(IDWriteFontFace* pFontFace, const CMathStyle& style, EnumMathItemType eType, float fUserScale) :
   CMathItem(eType, style, fUserScale) {
   m_pFontFace = pFontFace;
   m_eType = eType;
   m_fUserScale = fUserScale;
}
bool CWordItem::SetText(const vector<UINT32>& vUniCodePoints) {
   HRESULT hr = m_GlyphRun.SetGlyphs(vUniCodePoints, m_pFontFace);
   if (FAILED(hr))
      return false;
   OnInit_();
   return true;
}
bool CWordItem::SetGlyphIndexes(const vector<UINT16>& vGIndexes) {
   HRESULT hr = m_GlyphRun.SetGlyphIndices(vGIndexes, vector<UINT32>(vGIndexes.size(), 0), m_pFontFace);
   if (FAILED(hr))
      return false;
   OnInit_();
   return true;
}
//CMathItem Implementation
void CWordItem::Draw(D2D1_POINT_2F ptAnchor, const SDWRenderInfo& dwri) {
   float fScale = m_fUserScale * m_Style.StyleScale();
   //draw at the box position, baseline aligned
   D2D1_POINT_2F ptfBaseOrigin = {
      ptAnchor.x + EM2DIPS(dwri.fFontSizePts, m_Box.Left()),
      ptAnchor.y + EM2DIPS(dwri.fFontSizePts, m_Box.BaselineY())
   };
   m_GlyphRun.Draw(dwri, ptfBaseOrigin, fScale);
}

void CWordItem::OnInit_() {
   float fScale = m_fUserScale * m_Style.StyleScale();
   m_Box.nHeight = F2NEAREST((m_GlyphRun.Bounds().nMaxY - m_GlyphRun.Bounds().nMinY) * fScale);
   m_Box.nAscent = -F2NEAREST(m_GlyphRun.Bounds().nMinY * fScale);
   UINT32 nAdvWidth = 0;
   for (const SDWGlyph& glyph : m_GlyphRun.Glyphs())
      nAdvWidth += glyph.metrics.advanceWidth;
   m_Box.nAdvWidth = F2NEAREST(nAdvWidth * fScale);
   m_Box.nLBearing = F2NEAREST(m_GlyphRun.Glyphs().front().metrics.leftSideBearing * fScale);
   m_Box.nRBearing = F2NEAREST(m_GlyphRun.Glyphs().back().metrics.rightSideBearing * fScale);
   //more updates
   m_bOneGlyph = (m_GlyphRun.Glyphs().size() == 1);

   if (m_eType == eacUNK) {
      //TMP: deduce type from the first glyph only
      //TODO: load LatinModernMathGlyphs.csv with 
      //      nCodePoint,sName,sLaTex,eTexClass,nTopAccentX,nItalicCorr mappings 
      m_eType = eacWORD; //default
      if (m_bOneGlyph) {
         UINT32 nCP = m_GlyphRun.Glyphs().front().codepoint;
         if (nCP == L'+' || nCP == L'-' || nCP == L'*' || nCP == L':')
            m_eType = eacBIN;
         else if (nCP == L'=' || nCP == L'<' || nCP == L'>' || nCP == L'\x2264' || nCP == L'\x2265' || nCP == L'\x2260')
            m_eType = eacREL;
         else if (nCP == L',' || nCP == L';' || nCP == L'.')
            m_eType = eacPUNCT;
      }
   }
}
