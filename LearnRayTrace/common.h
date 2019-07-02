#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"

using std::string;
using std::cout;
using std::endl;


#undef max
#undef min
using std::max;
using std::min;

//MAP里是否有kye为VALUE的
#define map_exist_value(MAP, VALUE)\
(MAP.size() && MAP.find(VALUE) != MAP.end())

#define vector_exist_value(VECTOR, VALUE)\
std::find(VECTOR.begin(), VECTOR.end(), VALUE) != VECTOR.end()

#define list_exist_value(VECTOR, VALUE)\
vector_exist_value(VECTOR, VALUE)

#define begin_name_space(T)  namespace T{
#define end_name_space  }

#define tuple_value(VAR, N) std::get<N>(VAR) 

#define vector_remove_data(VECTOR, DATA)\
VECTOR.erase(std::remove(VECTOR.begin(), VECTOR.end(), DATA), VECTOR.end());


//类名
#define  meta_name(CLASS)\
 public:\
 static  std::string class_name() { return #CLASS; }/*类型*/\
 virtual std::string  className(){  return #CLASS; }\
virtual std::string  className() const { return #CLASS; }\
