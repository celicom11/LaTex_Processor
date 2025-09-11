#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HBoxItemTests
{
   TEST_CLASS(HBoxItemTests) {
      TEST_METHOD(TestAddItemBasic) {
         // Test first item sets m_Box correctly
         // Test glue insertion between items
         // Test m_vGlues.size() == m_vItems.size() - 1
      }

      TEST_METHOD(TestWidthCalculations) {
         // Test NormWidth(), MinWidth(), MaxStretch()
         // Test infinite stretch/shrink detection
         // Test nested HBox width calculations
      }
      TEST_METHOD(TestNaturalWidth) {
         // UpdateLayout(0) should set to NormWidth()
         // All glues should be at Norm() size
      }

      TEST_METHOD(TestFiniteStretch) {
         // Test proportional distribution
         // Create: [Item] [Glue 2pt+1pt] [Item] [Glue 3pt+2pt] [Item]
         // UpdateLayout(NormWidth + 3pt)
         // Verify glues get 1pt and 2pt respectively
      }

      TEST_METHOD(TestInfiniteStretch) {
         // Test equal distribution among fil glues
         // Test higher-order infinity wins (fill beats fil)
      }

      TEST_METHOD(TestOverflow) {
         // Test finite shrinking proportional to shrink capacity
         // Test infinite shrink equal distribution
      }
   };
}
