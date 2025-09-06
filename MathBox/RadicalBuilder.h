#pragma once
#include "MathItem.h"

//Radical's Factory
class CRadicalBuilder {
   IDWriteFontFace* m_pFontFace{ nullptr };	//external, dnd!
public:
   //CTOR
   CRadicalBuilder() = delete;
   CRadicalBuilder(IDWriteFontFace* pFontFace) : m_pFontFace(pFontFace) {}
   CMathItem* BuildRadical(const CMathStyle& style, float fUserScale, CMathItem* pRadicand, CMathItem* pRadDegree = nullptr);
private:
   CMathItem* BuildSimpleRadical_(const CMathStyle& style, float fUserScale, CMathItem* pRadicand,
                                  UINT16 nRadicalVariant, CMathItem* pRadDegree);
   CMathItem* AssembleRadical_(const CMathStyle& style, float fUserScale, CMathItem* pRadicand, CMathItem* pRadDegree);
};