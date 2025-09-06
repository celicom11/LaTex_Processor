#pragma once
#include "MathItem.h"

//Factory for a Fraction (with a bar) Item
class CFractionBuilder {
public:
   static CMathItem* BuildFraction(const CMathStyle& style, float fUserScale, CMathItem* pNum, CMathItem* pDenom);
};