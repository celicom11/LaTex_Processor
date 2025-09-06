#pragma once
//LMM OTF
const float otfScriptPercentScaleDown =          0.7f;
const float otfScriptScriptPercentScaleDown =    0.5f;
const INT32 otfAxisHeight =                      250;

// RADICAL CONSTANTS
const INT32 otfRadicalVerticalGap = 50;
const INT32 otfRadicalRuleThickness = 40;
const INT32 otfRadicalDisplayStyleVerticalGap = 148;
const INT32 otfRadicalExtraAscender = 40;
const INT32 otfRadicalKernAfterDegree = -556;
const INT32 otfRadicalKernBeforeDegree = 278;
const INT32 otfRadicalDegreeBottomRaisePercent = 60;
const INT32 otfUnitsPerEm = 1000; //==designUnitsPerEm?

//radical glyphs variant indexes and its sizes
const UINT16 lmmRadical = 3077;   //base glyph, 0x221A
const UINT16 lmmRadical_btm = 3078;   //radical assembly btm, x23b7
const UINT16 lmmRadical_ex = 3079;   //vert extender, not used!
const UINT16 lmmRadical_tp = 3080;   //extender's top
const UINT16 lmmRadical_v1 = 3081;
const UINT16 lmmRadical_v2 = 3082;
const UINT16 lmmRadical_v3 = 3083;
const UINT16 lmmRadical_v4 = 3084;
//glyph heights
const INT32 lmmRadicalH_Em = 1000;
const INT32 lmmRadicalH_v1Em = 1200;
const INT32 lmmRadicalH_v2Em = 1800;
const INT32 lmmRadicalH_v3Em = 2400;
const INT32 lmmRadicalH_v4Em = 3000;
const INT32 lmmRadicalBtmH_Em = 1820;
const INT32 lmmRadicalTopH_Em = 620; //note: lmmRadicalBtmH_Em+lmmRadicalTopH_Em << lmmRadicalH_v4Em!

const INT32 lmmRadicalBtmL_Em = 702;
const INT32 lmmRadicalBtmR_Em = 742;
//
//FRACTION CONSTANTS
const INT32 otfFractionRuleThickness = 40;
const INT32 otfFractionNumDisplayStyleGapMin = 120;
const INT32 otfFractionNumeratorDisplayStyleShiftUp = 677;
const INT32 otfFractionNumeratorGapMin = 40;
const INT32 otfFractionNumeratorShiftUp = 394;
const INT32 otfFractionDenomDisplayStyleGapMin = 120;
const INT32 otfFractionDenominatorDisplayStyleShiftDown = 686;
const INT32 otfFractionDenominatorGapMin = 40;
const INT32 otfFractionDenominatorShiftDown = 345;
