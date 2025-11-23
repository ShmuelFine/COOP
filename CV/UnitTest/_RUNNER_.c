#include "UnitTestsInfra.h"
#include "GrayImageUnitTest.h"


IMPORT_TESTS(GrayImageTest);

int main() {

	init_global_memory(0, HEAP_BASED_MEMORY);

	RUN_TESTS(GrayImageTest);

	return 0;

}

