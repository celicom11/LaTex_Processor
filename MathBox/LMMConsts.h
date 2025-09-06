#pragma once

const INT32 otfUnitsPerEm = 1000; //==designUnitsPerEm?

// MATH CONSTANTS
const float otfScriptPercentScaleDown = 0.7f;
const float otfScriptScriptPercentScaleDown = 0.5f;
#define otfAccentBaseHeight                           450
#define otfAxisHeight                                 250
#define otfDelimitedSubFormulaMinHeight               1300
#define otfDisplayOperatorMinHeight                   1300
#define otfFlattenedAccentBaseHeight                  664
#define otfFractionDenomDisplayStyleGapMin            120
#define otfFractionDenominatorDisplayStyleShiftDown   686
#define otfFractionDenominatorGapMin                  40
#define otfFractionDenominatorShiftDown               345
#define otfFractionNumDisplayStyleGapMin              120
#define otfFractionNumeratorDisplayStyleShiftUp       677
#define otfFractionNumeratorGapMin                    40
#define otfFractionNumeratorShiftUp                   394
#define otfFractionRuleThickness                      40
#define otfLowerLimitBaselineDropMin                  600
#define otfLowerLimitGapMin                           167
#define otfMathLeading                                154
#define otfOverbarExtraAscender                       40
#define otfOverbarRuleThickness                       40
#define otfOverbarVerticalGap                         120
#define otfRadicalDegreeBottomRaisePercent            60
#define otfRadicalDisplayStyleVerticalGap             148
#define otfRadicalExtraAscender                       40
#define otfRadicalKernAfterDegree                     -556
#define otfRadicalKernBeforeDegree                    278
#define otfRadicalRuleThickness                       40
#define otfRadicalVerticalGap                         50
#define otfSkewedFractionHorizontalGap                350
#define otfSkewedFractionVerticalGap                  96
#define otfSpaceAfterScript                           56
#define otfStackBottomDisplayStyleShiftDown           686
#define otfStackBottomShiftDown                       345
#define otfStackDisplayStyleGapMin                    280
#define otfStackGapMin                                120
#define otfStackTopDisplayStyleShiftUp                677
#define otfStackTopShiftUp                            444
#define otfStretchStackBottomShiftDown                600
#define otfStretchStackGapAboveMin                    200
#define otfStretchStackGapBelowMin                    167
#define otfStretchStackTopShiftUp                     111
#define otfSubSuperscriptGapMin                       160
#define otfSubscriptBaselineDropMin                   200
#define otfSubscriptShiftDown                         247
#define otfSubscriptTopMax                            344
#define otfSuperscriptBaselineDropMax                 250
#define otfSuperscriptBottomMaxWithSubscript          344
#define otfSuperscriptBottomMin                       108
#define otfSuperscriptShiftUp                         363
#define otfSuperscriptShiftUpCramped                  289
#define otfUnderbarExtraDescender                     40
#define otfUnderbarRuleThickness                      40
#define otfUnderbarVerticalGap                        120
#define otfUpperLimitBaselineRiseMin                  111
#define otfUpperLimitGapMin                           200

//TeX Glues
#define MU2EM(x) ((x)/18.0f)           //mu to em conversion, 1mu=1/18em
#define THINMUSKIP      MU2EM(3)       //3mu
#define MEDMUSKIP       MU2EM(4)       //4mu
#define MEDMUSKIP_MIN   0
#define MEDMUSKIP_MAX   MU2EM(6)       //6mu
#define THICKMUSKIP     MU2EM(5)       //5mu
#define THICKMUSKIP_MAX MU2EM(10)      //5+5mu

// Radical glyphs variant indexes and its sizes
const UINT16 lmmRadical = 3077;        // base glyph, 0x221A
const UINT16 lmmRadical_btm = 3078;    // radical assembly btm, x23b7
const UINT16 lmmRadical_ex = 3079;     // vert extender, not used!
const UINT16 lmmRadical_tp = 3080;     // extender's top
const UINT16 lmmRadical_v1 = 3081;
const UINT16 lmmRadical_v2 = 3082;
const UINT16 lmmRadical_v3 = 3083;
const UINT16 lmmRadical_v4 = 3084;
// some known radical dimensions 
const INT32 lmmRadicalH_Em = 1000;
const INT32 lmmRadicalH_v1Em = 1200;
const INT32 lmmRadicalH_v2Em = 1800;
const INT32 lmmRadicalH_v3Em = 2400;
const INT32 lmmRadicalH_v4Em = 3000;
const INT32 lmmRadicalBtmH_Em = 1820;
const INT32 lmmRadicalTopH_Em = 620;   // Note: lmmRadicalBtmH_Em+lmmRadicalTopH_Em << lmmRadicalH_v4Em!
const INT32 lmmRadicalBtmL_Em = 702;
const INT32 lmmRadicalBtmR_Em = 742;
//
