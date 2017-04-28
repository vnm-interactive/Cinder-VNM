#pragma once

#include <string>
#include "cinder/Vector.h"
#include "cinder/Quaternion.h"
#include "cinder/Color.h"

using std::string;
using ci::vec3;
using ci::quat;
using ci::Color;
using ci::ColorA;

#define GROUP_DEF(grp)
#define ITEM_DEF(type, var, default) extern type var;
#define ITEM_DEF_MINMAX(type, var, default, Min, Max) ITEM_DEF(type, var, default);
#include "item.def"
#undef ITEM_DEF_MINMAX
#undef ITEM_DEF
#undef GROUP_DEF

// item.def is the place to define your global variables
// in the format of
//
/*
ITEM_DEF(int, APP_WIDTH, 882)
ITEM_DEF(int, APP_HEIGHT, 725)
ITEM_DEF(string, NAME, "vinjn")
ITEM_DEF(float, velocity, 3.0f)

ITEM_DEF_MINMAX(float, delay, 3.0f, 0.0f, 10.0f)
*/

void readConfig();
void writeConfig();
void revertToDefaultValues();
void takeScreenShot(); // wait! why is it here?

namespace cinder { namespace params {
class InterfaceGl;
} }

#define ADD_ENUM_TO_INT(param, INT_VALUE, vectorOfEnumNames)    \
    param->removeParam(#INT_VALUE);                             \
    param->addParam(#INT_VALUE, vectorOfEnumNames, &INT_VALUE);

std::shared_ptr<ci::params::InterfaceGl> createConfigUI(const ci::ivec2& size);

