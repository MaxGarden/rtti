/*Registering of rtti types can be done only in this file. Do not put pragma once or include guards in this file!*/

#ifndef RTTI_GATHERING_TYPES
//Put here declarations
#include "declarations.h"
#endif

REGISTER_RTTI_TYPE(int)
REGISTER_RTTI_TYPE(double)
REGISTER_RTTI_TYPE(float)
REGISTER_RTTI_TYPE(char)
REGISTER_RTTI_TYPE(std::string)
REGISTER_RTTI_TYPE(TestClass);