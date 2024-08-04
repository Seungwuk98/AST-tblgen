#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "TestUtils.hpp"
#include "ast-tblgen/Components.hpp"
#include "ast-tblgen/Context.hpp"
#include "doctest.h"

namespace astgen {
TEST_CASE("runtest") {
  Context context;

  /*
   *
   * class IdE {
   * public:
   *
   *  std::tuple<std::string> Data;
   *
   * }
   *
   * */

  /// std::tuple<std::string>
  auto StringT = context.alloc<Type>("std::string", std::nullopt);

  auto TupleT = context.alloc<Type>("std::tuple", StringT);

  /// std::tuple<std::string> Data;

  auto Data = context.alloc<Var>("Data", TupleT);

  /// public:
  ///
  /// std::tuple<std::string> Data;

  auto ClassRegion =
      context.alloc<Region>(true, std::variant<Method *, Var *>(Data));

  /// class
  auto IdE = context.alloc<Class>("IdE", nullptr, ClassRegion);

  std::string str;
  raw_string_ostream ss(str);

  Printer printer(ss);
  IdE->print(printer);

  TRIM_EQ(str, R"(
class IdE {
public:
  std::tuple<std::string> Data;
};
)");
}
} // namespace astgen
