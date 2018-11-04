#pragma once
#include <string>
#include <iostream>
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"

using std::string;
using std::cout;
using std::endl;


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

begin_name_space(g)

const Color  White(255, 255, 255);
const Color  Red(255, 0, 0);
const Color  Green(0, 255, 0);
const Color  Blue(0, 0, 255);
const Color  Magenta(255, 0, 255);
const Color  Cyan(0, 255, 255);
const Color  Yellow(255, 255, 0);
const Color  Black(0, 0, 0);
const Color  Aquamarine(112, 219, 147);
const Color  BakerChocolate(92, 51, 23);
const Color  BlueViolet(159, 95, 159);
const Color  Brass(181, 166, 66);
const Color  BrightGold(217, 217, 25);
const Color  Brown(166, 42, 42);
const Color  Bronze(140, 120, 83);
const Color  BronzeII(166, 125, 61);
const Color  CadetBlue(95, 159, 159);
const Color  CoolCopper(217, 135, 25);
const Color  Copper(184, 115, 51);
const Color  Coral(255, 127, 0);
const Color  CornFlowerBlue(66, 66, 111);
const Color  DarkBrown(92, 64, 51);
const Color  DarkGreen(47, 79, 47);
const Color  DarkGreenCopper(74, 118, 110);
const Color  DarkOliveGreen(79, 79, 47);
const Color  DarkOrchid(153, 50, 205);
const Color  DarkPurple(135, 31, 120);
const Color  DarkSlateBlue(107, 35, 142);
const Color  DarkSlateGrey(47, 79, 79);
const Color  DarkTan(151, 105, 79);
const Color  DarkTurquoise(112, 147, 219);
const Color  DarkWood(133, 94, 66);
const Color  DimGrey(84, 84, 84);
const Color  DustyRose(133, 99, 99);
const Color  Feldspar(209, 146, 117);
const Color  Firebrick(142, 35, 35);
const Color  ForestGreen(35, 142, 35);
const Color  Gold(205, 127, 50);
const Color  Goldenrod(219, 219, 112);
const Color  Grey(192, 192, 192);
const Color  GreenCopper(82, 127, 118);
const Color  GreenYellow(147, 219, 112);
const Color  HunterGreen(33, 94, 33);
const Color  IndianRed(78, 47, 47);
const Color  Khaki(159, 159, 95);
const Color  LightBlue(192, 217, 217);
const Color  LightGrey(168, 168, 168);
const Color  LightSteelBlue(143, 143, 189);
const Color  LightWood(233, 194, 166);
const Color  LimeGreen(50, 205, 50);
const Color  MandarianOrange(228, 120, 51);
const Color  Maroon(142, 35, 107);
const Color  MediumAquamarine(50, 205, 153);
const Color  MediumBlue(50, 50, 205);
const Color  MediumForestGreen(107, 142, 35);
const Color  MediumGoldenrod(234, 234, 174);
const Color  MediumOrchid(147, 112, 219);
const Color  MediumSeaGreen(66, 111, 66);
const Color  MediumSlateBlue(127, 0, 255);
const Color  MediumSpringGreen(127, 255, 0);
const Color  MediumTurquoise(112, 219, 219);
const Color  MediumVioletRed(219, 112, 147);
const Color  MediumWood(166, 128, 100);
const Color  MidnightBlue(47, 47, 79);
const Color  NavyBlue(35, 35, 142);
const Color  NeonBlue(77, 77, 255);
const Color  NeonPink(255, 110, 199);
const Color  NewMidnightBlue(0, 0, 156);
const Color  NewTan(235, 199, 158);
const Color  OldGold(207, 181, 59);
const Color  Orange(255, 127, 0);
const Color  OrangeRed(255, 36, 0);
const Color  Orchid(219, 112, 219);
const Color  PaleGreen(143, 188, 143);
const Color  Pink(188, 143, 143);
const Color  Plum(234, 173, 234);
const Color  Quartz(217, 217, 243);
const Color  RichBlue(89, 89, 171);
const Color  Salmon(111, 66, 66);
const Color  Scarlet(140, 23, 23);
const Color  SeaGreen(35, 142, 104);
const Color  SemiSweetChocolate(107, 66, 38);
const Color  Sienna(142, 107, 35);
const Color  Silver(230, 232, 250);
const Color  SkyBlue(50, 153, 204);
const Color  SlateBlue(0, 127, 255);
const Color  SpicyPink(255, 28, 174);
const Color  SpringGreen(0, 255, 127);
const Color  SteelBlue(35, 107, 142);
const Color  SummerSky(56, 176, 222);
const Color  Tan(219, 147, 112);
const Color  Thistle(216, 191, 216);
const Color  Turquoise(173, 234, 234);
const Color  VeryDarkBrown(92, 64, 51);
const Color  VeryLightGrey(205, 205, 205);
const Color  Violet(79, 47, 79);
const Color  VioletRed(204, 50, 153);
const Color  Wheat(216, 216, 191);
const Color  YellowGreen(153, 204, 50);

end_name_space