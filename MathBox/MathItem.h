#pragma once
#include "LMMConsts.h"

#define PTS2DIPS(x) ((x)*96.0f/72.0f)
#define DIPS2PTS(x) ((x)*72.0f/96.0f)
#define F2NEAREST(x) ((int32_t)((x)<0?(x)-0.5f:(x)+0.5f))
#define EM2DIPS(fFontHPts, x) ((x)*PTS2DIPS(fFontHPts)/otfUnitsPerEm)
#define DIPS2EM(fFontHPts, x) F2NEAREST((x)*otfUnitsPerEm/PTS2DIPS(fFontHPts))

//
//forward declarations
struct ID2D1HwndRenderTarget;
struct IDWriteFontFace;
struct DWRITE_FONT_METRICS;
struct ID2D1Brush;

//abstract TexBox. all coordinates are in EM/font design units!!! 
struct STexBox {
   int32_t nX{ 0 }, nY{ 0 };                 // left,top RELATIVE to the container box!
   int32_t nAdvWidth{ 0 }, nHeight{ 0 };     // Ink-Box width = nAdvWidth - side bearings!
   int32_t nAscent{ 0 };                     // distance from TOP to baseline
   int32_t nLBearing{ 0 }, nRBearing{ 0 };   // for glyph boxes!
   int32_t nAxisHeight{ 0 };                 // optional, distance from baseline to math axis, 0: not defined
   //ATTS
   bool IsEmpty() const { return nAdvWidth == 0; }
   bool IsVCenterable() const { return nAxisHeight != 0; }
   int32_t Ascent() const { return nAscent; }
   int32_t Depth() const { return nHeight - nAscent; }
   int32_t Height() const { return nHeight; }
   int32_t Width() const { return nAdvWidth; }
   int32_t LBearing() const { return nLBearing; }
   int32_t RBearing() const { return nRBearing; }
   //abs positions
   int32_t Left() const { return nX; }
   int32_t Right() const { return nX + nAdvWidth; }
   int32_t Top() const { return nY; }
   int32_t Bottom() const { return nY + nHeight; }
   int32_t BaselineY() const { return nY + nAscent; }
   int32_t AxisY() const { return BaselineY() - nAxisHeight; } //absolute Y of math axis
   int32_t InkLeft() const { return Left() + nLBearing; }
   int32_t InkRight() const { return Right() - nRBearing; }
   int32_t InkWidth() const { return InkRight() - InkLeft(); }
   // OPERATIONS
   void MoveTo(int32_t x, int32_t y) { nX = x; nY = y; }
   void MoveBy(int32_t dx, int32_t dy) { nX += dx; nY += dy; }
   void Union(const STexBox& b) {
      if (IsEmpty()) { *this = b; return; }
      if (b.IsEmpty()) return;
      int32_t x0 = min(nX, b.nX);
      int32_t y0 = min(nY, b.nY);
      int32_t x1 = max(Right(), b.Right());
      int32_t y1 = max(Bottom(), b.Bottom());
      // keep current baseline/ascent
      if (b.nY < nY)
         nAscent += (nY - b.nY);
      nX = x0; nY = y0;
      nHeight = y1 - y0;
      nAdvWidth = x1 - x0;
   }
};
struct SDWRenderInfo {
   bool                 bDrawBoxes{ false };       //debugging aid
   float                fFontSizePts{ 0.0f };      //document font size, in points
   ID2D1RenderTarget*   pRT{ nullptr };
   IDWriteFontFace*     pFontFace{ nullptr };
   ID2D1Brush*          pBrush{ nullptr };
   ID2D1Brush*          pSelBrush{ nullptr };      //selection support
};
enum EnumTexStyle {
   etsDisplay = 0,   // display mode
   etsText,          // inline/compact mode
   etsScript,
   etsScriptScript
};
// Extended TeX atom's classes, for typesetting rules
enum EnumMathItemType {
   eacUNK = -1,
   eacEXT = 0,          // -: exention glyphs/fillers, etc.NOTE: non-selectable items!
   eacWORD,             // Ord: variable name or text
   eacBIN,              // Bin: binary operation, e.g. +,-,*,etc.
   eacREL,              // Rel: relation, e.g. =,<,>,etc.
   eacPUNCT,            // Punct: punctuation, e.g. comma,semicolon,etc.
   eacBRACETS,          // Open/Close: (big) delimiters, e.g. (,[,{,|,),],},etc.
   eacINNER,            // Inner: subformula, e.g. nominator in fraction, integrant in integral, etc.
   eacOVER,             // Over: item with an overbrace child
   eacUNDER,            // Under: item with an underbrace child
   eacACCENT,           // Acc: item with an accent child (bar,hat,vec,tilde, etc.)
   eacINDEXED,          // [all]: item with subscript/superscript indexes; its default in TeX, but not here!
   eacRADICAL,          // Rad: radical/root item with base and argument
   eacFRACTION,         // Inner: fraction item with numerator and denominator
   eacINTEGRAL,         // Op: integral item with integrand and limits
   eacSUM,              // Op: sum item with summand and limits
   eacPRODUCT,          // Op: product item with multiplicand and limits
   eacLIMIT,            // Op: limit item with arg and to_target children
   eacMINMAX,           // Op: min/max item with an arg child
   eacGlue,             // -: invisible spacing item of variable width
};
//Tex style helper class
class CMathStyle {
   bool              m_bCramped{ false };      //cramped/compact style (for scripts, radicand,)
   EnumTexStyle      m_eTexStyle{ etsDisplay };
public:
   //CTOR
   CMathStyle(EnumTexStyle eTexStyle = etsDisplay, bool bCramped = false) :
      m_eTexStyle(eTexStyle), m_bCramped(bCramped) {
   }
   //ATTS
   bool IsCramped() const { return m_bCramped; }
   void SetCramped(bool bCramped = true) { m_bCramped = bCramped; }
   EnumTexStyle Style() const { return   m_eTexStyle; }
   void SetStyle(EnumTexStyle eTexStyle) { m_eTexStyle = eTexStyle; }
   float StyleScale() const {
      if (m_eTexStyle == etsScript)
         return 0.7f;//otfScriptPercentScaleDown / 100.0f;
      else if (m_eTexStyle == etsScriptScript)
         return 0.5f; otfScriptScriptPercentScaleDown / 100.0f;
      return 1.0f; //display
   }
   //METHODS
   void Decrease() {
      if (m_eTexStyle < etsScriptScript)
         m_eTexStyle = EnumTexStyle(m_eTexStyle + 1);
   }
};
//base class for math items, abstract!
class CMathItem {
protected:
   bool             m_bSelected{ false };    //selection support
   bool             m_bOneGlyph{ false };    //needed for superscript/subscript positioning
   EnumMathItemType m_eType{ eacUNK };       //~TeX atom type
   CMathStyle       m_Style;                 //Tex style info   
   STexBox          m_Box;
public:
   //CTOR/DTOR
   CMathItem(EnumMathItemType eType, const CMathStyle& style, float fUserScale = 1.0f) :
      m_eType(eType), m_Style(style) {
   }
   virtual ~CMathItem() {}
   //ATTS
   bool IsSelected() const { return m_bSelected; }
   bool IsOneGlyph() const { return m_bOneGlyph; }
   const CMathStyle& Style() { return m_Style; }
   const STexBox& Box() const { return m_Box; }
   //METHODS
   void DenominateBinRel() {
      //make it ordinary atom
      if (m_eType == eacBIN || m_eType == eacREL)
         m_eType = eacWORD;
   }
   void MoveTo(int32_t nX, int32_t nY) {
      m_Box.MoveTo(nX, nY);
   }
   void MoveBy(int32_t nDX, int32_t nDY) {
      m_Box.MoveTo(m_Box.Left() + nDX, m_Box.Top() + nDY);
   }
   //VIRTUALS
   virtual void Select(bool bSelect = true) { m_bSelected = bSelect; } //default
   virtual void Draw(D2D1_POINT_2F ptAnchor, const SDWRenderInfo& dwri) = 0;
};
