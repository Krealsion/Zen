#include <utility_states/build_state.h>

using namespace Zen;

int main(int argc, char *argv[]) {
  GameStateManager manager(new BuildState());
  return 0;
}
