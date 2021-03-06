
#define SCENE_1

begin_name_space(g);

void RANDOM_PT(vector<float>& xVec, vector<float>& zVec)
{
	std::default_random_engine			  gen(time(NULL));
	std::uniform_real_distribution<float> distri_x(-40, 40);
	std::uniform_real_distribution<float> distri_z(-300, -30);

#if 0

	std::ofstream OFILE;
	OFILE.open(g::getExeDir() + "/res/random_pt.txt");

	int i = 0;
	float x, z;

	for (i = 0; i < 5000; i++)
	{
		x = distri_x(gen);
		z = distri_z(gen);
		OFILE << x << "\t" << z << "\n";
	}
	OFILE.close();

#else

	std::ifstream IFILE;
	IFILE.open(g::getExeDir() + "/res/random_pt.txt");

	int   i = 0;
	float x, z;

	while (!IFILE.eof())
	{
		IFILE >> x >> z;
		xVec.push_back(x);
		zVec.push_back(z);
	}

	IFILE.close();

#endif
}

end_name_space

#ifdef SCENE_1

	//--------------------------------------------------Y 向上， 右手
	void
	World::buildScene()
{
	static const float cd = 0.85;
	static const float kr = 0.75;

	Transparent* transparent_whitte = new Transparent(g::White, .01, g::White, .01, 100);
	transparent_whitte->set_reflect_color(g::White);
	transparent_whitte->set_refract(1.5f);

	Matte* matte_green = new Matte(g::Green, cd);
	Matte* matte_red   = new Matte(g::Red, cd);
	Matte* matte_blue  = new Matte(g::Blue, cd);
	Matte* matte_white = new Matte(g::White, cd);

	Phong* phong_green = new Phong(g::Green, cd, g::Green, .1, 100);
	Phong* phong_red   = new Phong(g::Red, cd, g::Red, .1, 100);
	Phong* phong_blue  = new Phong(g::Blue, cd, g::Blue, .1, 100);
	Phong* phong_white = new Phong(g::White, cd, g::White, .1, 100);

	Reflective* reflective_green = new Reflective(g::Green, cd, g::Green, .1, 100);
	reflective_green->set_reflect(Color(g::Green, 1));

	Reflective* reflective_red = new Reflective(g::Red, cd, g::Red, .1, 100);
	reflective_red->set_reflect(Color(g::White, kr));

	Reflective* reflective_blue = new Reflective(g::Blue, cd, g::Blue, .1, 100);
	reflective_blue->set_reflect(Color(g::White, kr));

	Reflective* reflective_white = new Reflective(g::White, cd, g::White, .1, 100);
	reflective_white->set_reflect(Color(g::White, 1));

	Plane* plane = nullptr;

	plane			   = new Plane(Vec3(0, 2, 0), Vec3(0, -1, 0));  //上
	plane->objectName_ = "top";
	plane->material_   = matte_blue;

	IBox* box;
	box				 = new IBox;  //灯
	box->objectName_ = "light plane";
	box->material_   = new Emissive;
	box->_minPt		 = Vec3(-.5, 1.85, -5);
	box->_maxPt		 = Vec3(.5, 2.01, -4);

#if 0
	for (int i = 0; i < 10; i++)
	{
		PointLight* pLight = new PointLight;
		pLight->pt_		   = Vec3(rand_float(-.5, .5), 1.8, rand_float(-5, -4));
	}
#else
	PointLight* pLight = new PointLight;
	pLight->pt_ = Vec3(0, 1.8, -4.5);

#endif

	plane			   = new Plane(Vec3(0, -2, 0), Vec3(0, 1, 0));  //下
	plane->objectName_ = "bottom";
	plane->material_   = matte_blue;

	plane			   = new Plane(Vec3(-2, 0, 0), Vec3(1, 0, 0));  //左
	plane->objectName_ = "left";
	plane->material_   = matte_red;

	plane			   = new Plane(Vec3(0, 0, -6), Vec3(0, 0, 1));  //后
	plane->objectName_ = "back";
	plane->material_   = matte_white;

	plane			   = new Plane(Vec3(2, 0, 0), Vec3(-1, 0, 0));  //右
	plane->objectName_ = "right";
	plane->material_   = matte_red;

	//-----------------------------------------------box
	//box = new IBox;
	//box->objectName_ = "box";
	//box->material_ = reflective_white;
	//box->_minPt = Vec3(-.5, -2, -5);
	//box->_maxPt = Vec3(.5, -1, -4);

	//--------------------------------------------------Sphere
	Sphere* sphere;
	sphere = new Sphere(Vec3(-1, -1.5, -4.5), .5);
	sphere->material_ = matte_white;

	sphere	= new Sphere(Vec3(0, -1.5, -5.5), .5);
	sphere->material_ = transparent_whitte;
	   
	sphere			  = new Sphere(Vec3(1, -1.5, -4.5), .5);
	sphere->material_ = matte_white;

	//-------------------------------------------------box
	static Mesh boxMesh(g::getResDir() + "cube.obj");
	MeshObject* boxModel  = new MeshObject(boxMesh);
	boxModel->objectName_ = "box.obj";
	boxModel->matrix_	 = Matrix::rotate(0, Vec3(0, 1, 0)) * Matrix::scale(.7) * Matrix::translate(-.5, -2.01, -4.8);
	boxModel->material_   = transparent_whitte;

	//BoundingBox bb = boxModel->computBBox();
	//cout << "bb" << bb.center() << "\t" << bb.max_len() << "\t" << bb.min_len() << "\tXX";

	//-------------------------------------------------bunny
	//static Mesh bunyMesh(g::getExeDir() + "/res/bunny_big.obj");
	//MeshObject* bunny = new MeshObject(bunyMesh);
	//bunny->matrix_ = Matrix::rotate(0, Vec3(0, 1, 0)) * Matrix::scale(10) * Matrix::translate(0, -2.3, -5);
	//bunny->material_ = transparent_whitte;

	//-------------------------------------------------teapot
	//static Mesh teapotMesh(g::getExeDir() + "../../res/teapot.obj");
	//MeshObject* teapot = new MeshObject(teapotMesh);
	//teapot->matrix_ = Matrix::rotate(-PI_2f, Vec3(1, 0, 0)) * Matrix::scale(1) * Matrix::translate(0, -1.6, -5);
	//teapot->material_ = transparent_whitte;
	//const BoundingBox& bb = teapot->computBBox();
	//cout << bb.max_len() << "\t" << bb.min_len() << "\tXX";
}

#endif  // SCENE_1

#ifdef SCENE_2
//--------------------------------------------------Y 向上， 右手
void World::buildScene()
{
	static const float cd				= 0.5;
	static const float kr				= 0.1;
	Reflective*		   reflective_green = new Reflective(g::Green, cd, g::Green, .1, 100);
	reflective_green->set_reflect(Color(g::White, kr));

	Reflective* reflective_red = new Reflective(g::Red, cd, g::Red, .1, 100);
	reflective_red->set_reflect(Color(g::White, kr));

	Reflective* reflective_white = new Reflective(g::White, cd, g::White, .1, 100);
	reflective_white->set_reflect(Color(g::White, kr));

	Transparent* transparent_whitte = new Transparent(g::White, .75, g::White, .75, 100);
	transparent_whitte->set_reflect_color(g::White);
	transparent_whitte->set_refract(1.5f);

	Plane* plane	 = nullptr;
	plane			 = new Plane(Vec3(0, -25, 0), Vec3(0, 1, 0));
	plane->material_ = new Checker;  // (g::Red, g::Green);

	//plane = new Plane(Vec3(0, 35, 0), Vec3(0, -1, 0));
	//plane->material_ = new Checker;// (g::Red, g::Green);

	int i = 0;

	static Mesh bunyMesh(g::getExeDir() + "/res/bunny_big.obj");

	vector<float> xVec, zVec;
	g::RANDOM_PT(xVec, zVec);

	Matte* matte =  //new Matte(g::White, .95);
		new Phong(g::White, .95, g::White, .1, 10);

	//-------------------------------------------------bunny
	for (i = 0; i < 1; i++)
	{
		MeshObject* bunny = new MeshObject(bunyMesh);

		bunny->matrix_		  = Matrix::rotate(0, Vec3(0, 1, 0)) * Matrix::scale(10) * Matrix::translate(0, -2, -5);
		bunny->material_	  = transparent_whitte;
		const BoundingBox& bb = bunny->computBBox();
		//cout << bb.max_len() << "\t" << bb.min_len() << "\tXX";
	}

	//-------------------------------------------------venusm
	//static Mesh venusmMesh(g::getExeDir() + "/res/venusm.obj");
	//MeshObject* venusm = new MeshObject(venusmMesh);

	//venusm->matrix_   = Matrix::rotate(0, Vec3(0, 1, 0)) * Matrix::scale(.01) * Matrix::translate(30, -10, -50);
	//venusm->material_ = matte;

	//-------------------------------------------------ateneam
	//static Mesh ateneammMesh(g::getExeDir() + "/res/ateneam.obj");
	//MeshObject* ateneam = new MeshObject(ateneammMesh);
	//ateneam->matrix_	= Matrix::rotate(0, Vec3(0, 1, 0)) * Matrix::scale(.01) * Matrix::translate(-30, -5, -130);
	//ateneam->material_  = reflective_white;
	//BoundingBox bb		= ateneam->computBBox();
	//cout << "BB " << bb.center() << "\t" << bb.min_len() << "\t" << bb.max_len() << "\n";

	//-------------------------------------------------Sphere
	//Sphere* sphere	= new Sphere(Vec3(20, -7, -60), 8);
	//sphere->material_ = transparent_whitte;

	//sphere			  = new Sphere(Vec3(2, -0, -60), 8);
	//sphere->material_ = transparent_whitte;

	/*sphere = new Sphere(Vec3(-15, -17, -60), 8);
	sphere->material_ = reflective_white;*/

	//PointLight* pLight = new PointLight(Vec3(-22, 100, -98), g::White);
	DirectionLight* dLight = new DirectionLight(Vec3(.33, .33, .33), g::White);
}
#endif  // SCENE_2