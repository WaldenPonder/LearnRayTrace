

const Vec3 X_AXIS(1.0, 0.0, 0.0);
const Vec3 Y_AXIS(0.0, 1.0, 0.0);
const Vec3 Z_AXIS(0.0, 0.0, 1.0);


using Point = Vec3;
using Vec3f = Vec3;

struct Color
{
	float k = 1.f;
	Vec3 c = Vec3(1.f);

	Vec3 get() const 
	{
		return k * c;
	}
};

namespace g {
	const Vec3  White(255 / 255.f, 255 / 255.f, 255 / 255.f);
	const Vec3  Red(255 / 255.f, 0 / 255.f, 0 / 255.f);
	const Vec3  Green(0 / 255.f, 255 / 255.f, 0 / 255.f);
	const Vec3  Blue(0 / 255.f, 0 / 255.f, 255 / 255.f);
	const Vec3  Magenta(255 / 255.f, 0 / 255.f, 255 / 255.f);
	const Vec3  Cyan(0 / 255.f, 255 / 255.f, 255 / 255.f);
	const Vec3  Yellow(255 / 255.f, 255 / 255.f, 0 / 255.f);
	const Vec3  Black(0 / 255.f, 0 / 255.f, 0 / 255.f);
	const Vec3  Aquamarine(112 / 255.f, 219 / 255.f, 147 / 255.f);
	const Vec3  BakerChocolate(92 / 255.f, 51 / 255.f, 23 / 255.f);
	const Vec3  BlueViolet(159 / 255.f, 95 / 255.f, 159 / 255.f);
	const Vec3  Brass(181 / 255.f, 166 / 255.f, 66 / 255.f);
	const Vec3  BrightGold(217 / 255.f, 217 / 255.f, 25 / 255.f);
	const Vec3  Brown(166 / 255.f, 42 / 255.f, 42 / 255.f);
	const Vec3  Bronze(140 / 255.f, 120 / 255.f, 83 / 255.f);
	const Vec3  BronzeII(166 / 255.f, 125 / 255.f, 61 / 255.f);
	const Vec3  CadetBlue(95 / 255.f, 159 / 255.f, 159 / 255.f);
	const Vec3  CoolCopper(217 / 255.f, 135 / 255.f, 25 / 255.f);
	const Vec3  Copper(184 / 255.f, 115 / 255.f, 51 / 255.f);
	const Vec3  Coral(255 / 255.f, 127 / 255.f, 0 / 255.f);
	const Vec3  CornFlowerBlue(66 / 255.f, 66 / 255.f, 111 / 255.f);
	const Vec3  DarkBrown(92 / 255.f, 64 / 255.f, 51 / 255.f);
	const Vec3  DarkGreen(47 / 255.f, 79 / 255.f, 47 / 255.f);
	const Vec3  DarkGreenCopper(74 / 255.f, 118 / 255.f, 110 / 255.f);
	const Vec3  DarkOliveGreen(79 / 255.f, 79 / 255.f, 47 / 255.f);
	const Vec3  DarkOrchid(153 / 255.f, 50 / 255.f, 205 / 255.f);
	const Vec3  DarkPurple(135 / 255.f, 31 / 255.f, 120 / 255.f);
	const Vec3  DarkSlateBlue(107 / 255.f, 35 / 255.f, 142 / 255.f);
	const Vec3  DarkSlateGrey(47 / 255.f, 79 / 255.f, 79 / 255.f);
	const Vec3  DarkTan(151 / 255.f, 105 / 255.f, 79 / 255.f);
	const Vec3  DarkTurquoise(112 / 255.f, 147 / 255.f, 219 / 255.f);
	const Vec3  DarkWood(133 / 255.f, 94 / 255.f, 66 / 255.f);
	const Vec3  DimGrey(84 / 255.f, 84 / 255.f, 84 / 255.f);
	const Vec3  DustyRose(133 / 255.f, 99 / 255.f, 99 / 255.f);
	const Vec3  Feldspar(209 / 255.f, 146 / 255.f, 117 / 255.f);
	const Vec3  Firebrick(142 / 255.f, 35 / 255.f, 35 / 255.f);
	const Vec3  ForestGreen(35 / 255.f, 142 / 255.f, 35 / 255.f);
	const Vec3  Gold(205 / 255.f, 127 / 255.f, 50 / 255.f);
	const Vec3  Goldenrod(219 / 255.f, 219 / 255.f, 112 / 255.f);
	const Vec3  Grey(192 / 255.f, 192 / 255.f, 192 / 255.f);
	const Vec3  GreenCopper(82 / 255.f, 127 / 255.f, 118 / 255.f);
	const Vec3  GreenYellow(147 / 255.f, 219 / 255.f, 112 / 255.f);
	const Vec3  HunterGreen(33 / 255.f, 94 / 255.f, 33 / 255.f);
	const Vec3  IndianRed(78 / 255.f, 47 / 255.f, 47 / 255.f);
	const Vec3  Khaki(159 / 255.f, 159 / 255.f, 95 / 255.f);
	const Vec3  LightBlue(192 / 255.f, 217 / 255.f, 217 / 255.f);
	const Vec3  LightGrey(168 / 255.f, 168 / 255.f, 168 / 255.f);
	const Vec3  LightSteelBlue(143 / 255.f, 143 / 255.f, 189 / 255.f);
	const Vec3  LightWood(233 / 255.f, 194 / 255.f, 166 / 255.f);
	const Vec3  LimeGreen(50 / 255.f, 205 / 255.f, 50 / 255.f);
	const Vec3  MandarianOrange(228 / 255.f, 120 / 255.f, 51 / 255.f);
	const Vec3  Maroon(142 / 255.f, 35 / 255.f, 107 / 255.f);
	const Vec3  MediumAquamarine(50 / 255.f, 205 / 255.f, 153 / 255.f);
	const Vec3  MediumBlue(50 / 255.f, 50 / 255.f, 205 / 255.f);
	const Vec3  MediumForestGreen(107 / 255.f, 142 / 255.f, 35 / 255.f);
	const Vec3  MediumGoldenrod(234 / 255.f, 234 / 255.f, 174 / 255.f);
	const Vec3  MediumOrchid(147 / 255.f, 112 / 255.f, 219 / 255.f);
	const Vec3  MediumSeaGreen(66 / 255.f, 111 / 255.f, 66 / 255.f);
	const Vec3  MediumSlateBlue(127 / 255.f, 0 / 255.f, 255 / 255.f);
	const Vec3  MediumSpringGreen(127 / 255.f, 255 / 255.f, 0 / 255.f);
	const Vec3  MediumTurquoise(112 / 255.f, 219 / 255.f, 219 / 255.f);
	const Vec3  MediumVioletRed(219 / 255.f, 112 / 255.f, 147 / 255.f);
	const Vec3  MediumWood(166 / 255.f, 128 / 255.f, 100 / 255.f);
	const Vec3  MidnightBlue(47 / 255.f, 47 / 255.f, 79 / 255.f);
	const Vec3  NavyBlue(35 / 255.f, 35 / 255.f, 142 / 255.f);
	const Vec3  NeonBlue(77 / 255.f, 77 / 255.f, 255 / 255.f);
	const Vec3  NeonPink(255 / 255.f, 110 / 255.f, 199 / 255.f);
	const Vec3  NewMidnightBlue(0 / 255.f, 0 / 255.f, 156 / 255.f);
	const Vec3  NewTan(235 / 255.f, 199 / 255.f, 158 / 255.f);
	const Vec3  OldGold(207 / 255.f, 181 / 255.f, 59 / 255.f);
	const Vec3  Orange(255 / 255.f, 127 / 255.f, 0 / 255.f);
	const Vec3  OrangeRed(255 / 255.f, 36 / 255.f, 0 / 255.f);
	const Vec3  Orchid(219 / 255.f, 112 / 255.f, 219 / 255.f);
	const Vec3  PaleGreen(143 / 255.f, 188 / 255.f, 143 / 255.f);
	const Vec3  Pink(188 / 255.f, 143 / 255.f, 143 / 255.f);
	const Vec3  Plum(234 / 255.f, 173 / 255.f, 234 / 255.f);
	const Vec3  Quartz(217 / 255.f, 217 / 255.f, 243 / 255.f);
	const Vec3  RichBlue(89 / 255.f, 89 / 255.f, 171 / 255.f);
	const Vec3  Salmon(111 / 255.f, 66 / 255.f, 66 / 255.f);
	const Vec3  Scarlet(140 / 255.f, 23 / 255.f, 23 / 255.f);
	const Vec3  SeaGreen(35 / 255.f, 142 / 255.f, 104 / 255.f);
	const Vec3  SemiSweetChocolate(107 / 255.f, 66 / 255.f, 38 / 255.f);
	const Vec3  Sienna(142 / 255.f, 107 / 255.f, 35 / 255.f);
	const Vec3  Silver(230 / 255.f, 232 / 255.f, 250 / 255.f);
	const Vec3  SkyBlue(50 / 255.f, 153 / 255.f, 204 / 255.f);
	const Vec3  SlateBlue(0 / 255.f, 127 / 255.f, 255 / 255.f);
	const Vec3  SpicyPink(255 / 255.f, 28 / 255.f, 174 / 255.f);
	const Vec3  SpringGreen(0 / 255.f, 255 / 255.f, 127 / 255.f);
	const Vec3  SteelBlue(35 / 255.f, 107 / 255.f, 142 / 255.f);
	const Vec3  SummerSky(56 / 255.f, 176 / 255.f, 222 / 255.f);
	const Vec3  Tan(219 / 255.f, 147 / 255.f, 112 / 255.f);
	const Vec3  Thistle(216 / 255.f, 191 / 255.f, 216 / 255.f);
	const Vec3  Turquoise(173 / 255.f, 234 / 255.f, 234 / 255.f);
	const Vec3  VeryDarkBrown(92 / 255.f, 64 / 255.f, 51 / 255.f);
	const Vec3  VeryLightGrey(205 / 255.f, 205 / 255.f, 205 / 255.f);
	const Vec3  Violet(79 / 255.f, 47 / 255.f, 79 / 255.f);
	const Vec3  VioletRed(204 / 255.f, 50 / 255.f, 153 / 255.f);
	const Vec3  Wheat(216 / 255.f, 216 / 255.f, 191 / 255.f);
	const Vec3  YellowGreen(153 / 255.f, 204 / 255.f, 50 / 255.f);
}


using std::string;
using std::cout;
using std::endl;
using std::vector;

class ShadeInfo;

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
virtual std::string  className() const { return #CLASS; }

class Shape;
class World;
class Material;
class ShadeInfo;
class Ray;
class Extent;
class MeshObject;



template<typename T>
vector<T>& operator <<(vector<T>& v, const T& i)
{
	v.push_back(i);
	return v;
}

template<typename T, typename T2>
vector<T*>& operator <<(vector<T*>& v, T2* i)
{
	v.push_back(i);
	return v;
}

template<typename T>
vector<T*>& operator <<(vector<T*>& v, nullptr_t i)
{
	v.push_back(i);
	return v;
}


inline  std::ostream& operator<< (std::ostream& stm, const Vec3& v)
{
	stm << std::string("  [") << v[0] << " , " << v[1] << " , " << v[2] << "] ";

	return stm;
}


template<typename T>
class InstancePool
{
public:
	InstancePool()
	{
		pool_.push_back(static_cast<T*>(this));
	}

	virtual ~InstancePool()
	{
		vector_remove_data(pool_, static_cast<T*>(this));
	}

	static vector<T*> pool() { return pool_; }

	template<typename T2>
	static vector<T2*> get_type()
	{
		vector<T2*> res;

		for (int i = 0; i < pool_.size(); i++)
		{
			if (pool_[i]->className() == T2::class_name())
				res.push_back(dynamic_cast<T2*>(pool_[i]));
		}
		return res;
	}

private:
	static vector<T*> pool_;
};

template<typename T>
vector<T*> InstancePool<T>::pool_;