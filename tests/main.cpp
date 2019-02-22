#include <Basis/BSplineBasisNode.h>
#include <new>

using namespace casadi;

int main(int argc, char *argv[]) {
  //BSplineEvaluator s(5, 3);
  //FunctionInternal* s = new FunctionInternal("foo");
  FunctionInternal* s = new BSplineEvaluator(5, 3);
  return 0;
}
