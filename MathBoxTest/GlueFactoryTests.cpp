#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GlueFactoryTests
{
   TEST_CLASS(GlueFactoryTests) {
      TEST_METHOD(TestAtomSpacingMatrix) {
         // Verify CGlueItem::_Create() returns correct MuSkipType
         // Test all 8×8 combinations from ATOM_SPACING_MATRIX
         // Compare: Ord-Op, Op-Bin, Rel-Rel, etc.
      }

      TEST_METHOD(TestDisplayVsTextMode) {
         // Test style.Style() <= etsText behavior
         // Test Op-Bin spacing difference in display vs text
      }

      TEST_METHOD(TestNoGlueCases) {
         // Test adjacent glues return nullptr
         // Test cases that should return emstNoSpace
      }
   };
}
