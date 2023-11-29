#include "Solver.h"

//355 lines of code : )
int main(int argc, char* argv[])
{
	Solver solver = Solver();
	while (solver.IsRunning())
	{
		solver.handleEvents();
		solver.update();
		solver.render();
	}
	return 0;
}