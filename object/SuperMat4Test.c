
#include "SuperMat4Test.h"
 
DEF_DRIVED_CTOR(SuperMat4Test, Mat4Test, FLOAT_TYPE hight, FLOAT_TYPE width, int step) SUPER, hight, width ME
{
	_this->step = step;
}
END_DRIVED_CTOR

DEF_DRIVED_DTOR(SuperMat4Test, Mat4Test)
{

}
END_DRIVED_DTOR

INIT_DRIVED_CLASS(SuperMat4Test,Mat4Test)
END_INIT_CLASS
