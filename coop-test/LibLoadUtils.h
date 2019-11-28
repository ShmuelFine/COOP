#pragma once
#include <Windows.h>
#define DEF_PTR_TO_FUNC(name) decltype(name) * name ##_ptr;
#define LOAD_FUNC(name) name ##_ptr = (decltype(name) *) GetProcAddress(coopLib, #name);
