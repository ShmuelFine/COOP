#pragma once
#include "object.h"

#define INHERIT(name, base1)  \
DEF_CLASS(name)           	  \
base1 *base;

#define END_DEF(name)      \
#define BASE(...) _this->base->vTable->_ctor(_this->base,__VA_ARGS__);

#define END_INHERIT(name) \
END_DEF(name)   		 	
