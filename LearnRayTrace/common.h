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

const Color  White(255/255.f, 255/255.f, 255/255.f);
const Color  Red(255/255.f, 0/255.f, 0/255.f);
const Color  Green(0/255.f, 255/255.f, 0/255.f);
const Color  Blue(0/255.f, 0/255.f, 255/255.f);
const Color  Magenta(255/255.f, 0/255.f, 255/255.f);
const Color  Cyan(0/255.f, 255/255.f, 255/255.f);
const Color  Yellow(255/255.f, 255/255.f, 0/255.f);
const Color  Black(0/255.f, 0/255.f, 0/255.f);
const Color  Aquamarine(112/255.f, 219/255.f, 147/255.f);
const Color  BakerChocolate(92/255.f, 51/255.f, 23/255.f);
const Color  BlueViolet(159/255.f, 95/255.f, 159/255.f);
const Color  Brass(181/255.f, 166/255.f, 66/255.f);
const Color  BrightGold(217/255.f, 217/255.f, 25/255.f);
const Color  Brown(166/255.f, 42/255.f, 42/255.f);
const Color  Bronze(140/255.f, 120/255.f, 83/255.f);
const Color  BronzeII(166/255.f, 125/255.f, 61/255.f);
const Color  CadetBlue(95/255.f, 159/255.f, 159/255.f);
const Color  CoolCopper(217/255.f, 135/255.f, 25/255.f);
const Color  Copper(184/255.f, 115/255.f, 51/255.f);
const Color  Coral(255/255.f, 127/255.f, 0/255.f);
const Color  CornFlowerBlue(66/255.f, 66/255.f, 111/255.f);
const Color  DarkBrown(92/255.f, 64/255.f, 51/255.f);
const Color  DarkGreen(47/255.f, 79/255.f, 47/255.f);
const Color  DarkGreenCopper(74/255.f, 118/255.f, 110/255.f);
const Color  DarkOliveGreen(79/255.f, 79/255.f, 47/255.f);
const Color  DarkOrchid(153/255.f, 50/255.f, 205/255.f);
const Color  DarkPurple(135/255.f, 31/255.f, 120/255.f);
const Color  DarkSlateBlue(107/255.f, 35/255.f, 142/255.f);
const Color  DarkSlateGrey(47/255.f, 79/255.f, 79/255.f);
const Color  DarkTan(151/255.f, 105/255.f, 79/255.f);
const Color  DarkTurquoise(112/255.f, 147/255.f, 219/255.f);
const Color  DarkWood(133/255.f, 94/255.f, 66/255.f);
const Color  DimGrey(84/255.f, 84/255.f, 84/255.f);
const Color  DustyRose(133/255.f, 99/255.f, 99/255.f);
const Color  Feldspar(209/255.f, 146/255.f, 117/255.f);
const Color  Firebrick(142/255.f, 35/255.f, 35/255.f);
const Color  ForestGreen(35/255.f, 142/255.f, 35/255.f);
const Color  Gold(205/255.f, 127/255.f, 50/255.f);
const Color  Goldenrod(219/255.f, 219/255.f, 112/255.f);
const Color  Grey(192/255.f, 192/255.f, 192/255.f);
const Color  GreenCopper(82/255.f, 127/255.f, 118/255.f);
const Color  GreenYellow(147/255.f, 219/255.f, 112/255.f);
const Color  HunterGreen(33/255.f, 94/255.f, 33/255.f);
const Color  IndianRed(78/255.f, 47/255.f, 47/255.f);
const Color  Khaki(159/255.f, 159/255.f, 95/255.f);
const Color  LightBlue(192/255.f, 217/255.f, 217/255.f);
const Color  LightGrey(168/255.f, 168/255.f, 168/255.f);
const Color  LightSteelBlue(143/255.f, 143/255.f, 189/255.f);
const Color  LightWood(233/255.f, 194/255.f, 166/255.f);
const Color  LimeGreen(50/255.f, 205/255.f, 50/255.f);
const Color  MandarianOrange(228/255.f, 120/255.f, 51/255.f);
const Color  Maroon(142/255.f, 35/255.f, 107/255.f);
const Color  MediumAquamarine(50/255.f, 205/255.f, 153/255.f);
const Color  MediumBlue(50/255.f, 50/255.f, 205/255.f);
const Color  MediumForestGreen(107/255.f, 142/255.f, 35/255.f);
const Color  MediumGoldenrod(234/255.f, 234/255.f, 174/255.f);
const Color  MediumOrchid(147/255.f, 112/255.f, 219/255.f);
const Color  MediumSeaGreen(66/255.f, 111/255.f, 66/255.f);
const Color  MediumSlateBlue(127/255.f, 0/255.f, 255/255.f);
const Color  MediumSpringGreen(127/255.f, 255/255.f, 0/255.f);
const Color  MediumTurquoise(112/255.f, 219/255.f, 219/255.f);
const Color  MediumVioletRed(219/255.f, 112/255.f, 147/255.f);
const Color  MediumWood(166/255.f, 128/255.f, 100/255.f);
const Color  MidnightBlue(47/255.f, 47/255.f, 79/255.f);
const Color  NavyBlue(35/255.f, 35/255.f, 142/255.f);
const Color  NeonBlue(77/255.f, 77/255.f, 255/255.f);
const Color  NeonPink(255/255.f, 110/255.f, 199/255.f);
const Color  NewMidnightBlue(0/255.f, 0/255.f, 156/255.f);
const Color  NewTan(235/255.f, 199/255.f, 158/255.f);
const Color  OldGold(207/255.f, 181/255.f, 59/255.f);
const Color  Orange(255/255.f, 127/255.f, 0/255.f);
const Color  OrangeRed(255/255.f, 36/255.f, 0/255.f);
const Color  Orchid(219/255.f, 112/255.f, 219/255.f);
const Color  PaleGreen(143/255.f, 188/255.f, 143/255.f);
const Color  Pink(188/255.f, 143/255.f, 143/255.f);
const Color  Plum(234/255.f, 173/255.f, 234/255.f);
const Color  Quartz(217/255.f, 217/255.f, 243/255.f);
const Color  RichBlue(89/255.f, 89/255.f, 171/255.f);
const Color  Salmon(111/255.f, 66/255.f, 66/255.f);
const Color  Scarlet(140/255.f, 23/255.f, 23/255.f);
const Color  SeaGreen(35/255.f, 142/255.f, 104/255.f);
const Color  SemiSweetChocolate(107/255.f, 66/255.f, 38/255.f);
const Color  Sienna(142/255.f, 107/255.f, 35/255.f);
const Color  Silver(230/255.f, 232/255.f, 250/255.f);
const Color  SkyBlue(50/255.f, 153/255.f, 204/255.f);
const Color  SlateBlue(0/255.f, 127/255.f, 255/255.f);
const Color  SpicyPink(255/255.f, 28/255.f, 174/255.f);
const Color  SpringGreen(0/255.f, 255/255.f, 127/255.f);
const Color  SteelBlue(35/255.f, 107/255.f, 142/255.f);
const Color  SummerSky(56/255.f, 176/255.f, 222/255.f);
const Color  Tan(219/255.f, 147/255.f, 112/255.f);
const Color  Thistle(216/255.f, 191/255.f, 216/255.f);
const Color  Turquoise(173/255.f, 234/255.f, 234/255.f);
const Color  VeryDarkBrown(92/255.f, 64/255.f, 51/255.f);
const Color  VeryLightGrey(205/255.f, 205/255.f, 205/255.f);
const Color  Violet(79/255.f, 47/255.f, 79/255.f);
const Color  VioletRed(204/255.f, 50/255.f, 153/255.f);
const Color  Wheat(216/255.f, 216/255.f, 191/255.f);
const Color  YellowGreen(153/255.f, 204/255.f, 50/255.f);

end_name_space