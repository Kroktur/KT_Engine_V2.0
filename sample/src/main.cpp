#include "Tools/Chrono.h"
#include "Tools/Typelist.h"
#include <iostream>
#include "Math/Vector3.h"
#include "Tools/PrimaryTypeTrait.h"
#include "Math/Angle.h"
#include "Core/Factory.h"
#include "Math/OBB.h"
#include "Math/Vector3.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <Eigen/Dense>
#include <iostream>
#include <cmath>
#include <vector>
#include <conio.h>

#include "Core/CollisionSolver.h"
#include "Math/AABB.h"
#include "Math/BoundingSphere.h"
#include "Math/Vector2.h"

template<typename... Types>
struct toto
{
	template<typename... type>
	static int test(type... typ)
	{
		(... && (std::cout << typ));
		std::cout << " test reussit " << std::endl;
		return 3;
	}
	template<typename... type>
	static int test2(type... typ)
	{
		(... && (std::cout << typ));
		std::cout << " test reussit " << std::endl;
		return 3;
	}
	static void totofn()
	{
		std::cout << "reussit " << std::endl;
	}
};

struct base
{
	virtual ~base() = default;
	base(int){}
};
struct derived : public base
{
	derived(int t) : base(t){}
};
struct derived2 : public base
{
	derived2(int t) : base(t) {}
};

static KT::Vector3F rotateX(const KT::Vector3F& v, float angle) {
	return KT::Vector3F(v.x,
		v.y * cos(angle) - v.z * sin(angle),
		v.y * sin(angle) + v.z * cos(angle));
}

// Fonction de rotation autour de l'axe Y
static KT::Vector3F rotateY(const KT::Vector3F& v, float angle) {
	return KT::Vector3F(v.x * cos(angle) + v.z * sin(angle),
		v.y,
		-v.x * sin(angle) + v.z * cos(angle));
}

// Fonction de rotation autour de l'axe Z
static KT::Vector3F rotateZ(const KT::Vector3F& v, float angle) {
	return KT::Vector3F(v.x * cos(angle) - v.y * sin(angle),
		v.x * sin(angle) + v.y * cos(angle),
		v.z);
}

std::vector<KT::Vector3F> getCubeVertices(const KT::Vector3F& center, const KT::Vector3F& half_size, const KT::Vector3F& rotation) {
	std::vector<KT::Vector3F> vertices;

	// Calcul des 8 sommets du cube sans rotation
	KT::Vector3F offsets[8] = {
		KT::Vector3F(-half_size.x, -half_size.y, -half_size.z),
		KT::Vector3F(half_size.x, -half_size.y, -half_size.z),
		KT::Vector3F(-half_size.x,  half_size.y, -half_size.z),
		KT::Vector3F(half_size.x,  half_size.y, -half_size.z),
		KT::Vector3F(-half_size.x, -half_size.y,  half_size.z),
		KT::Vector3F(half_size.x, -half_size.y,  half_size.z),
		KT::Vector3F(-half_size.x,  half_size.y,  half_size.z),
		KT::Vector3F(half_size.x,  half_size.y,  half_size.z)
	};

	// Appliquer la rotation
	for (int i = 0; i < 8; ++i) {
		// Rotation autour des axes X, Y, Z dans l'ordre
		KT::Vector3F rotated =rotateX(offsets[i], rotation.x);
		rotated = rotateY(rotated, rotation.y);
		rotated = rotateZ(rotated, rotation.z);

		// Ajouter le centre
		vertices.push_back(  rotated + center);
	}

	return vertices;
}

KT::Vector2F rotate2D(const KT::Vector2F& point, float angle_deg) {
	// Convertir l'angle en radians
	float angle_rad = angle_deg * 3.14159265358979323846f / 180.0f;

	// Appliquer la matrice de rotation 2D
	float x_new = point.x * std::cos(angle_rad) - point.y * std::sin(angle_rad);
	float y_new = point.x * std::sin(angle_rad) + point.y * std::cos(angle_rad);

	return KT::Vector2F(x_new, y_new);
}

std::vector<KT::Vector2F> GetCarre(const KT::Vector2F& center, const KT::Vector2F& half_size, const float& rotation)
{
	std::vector<KT::Vector2F> vertices;
	KT::Vector2F offsets[4] = {
		KT::Vector2F(-half_size.x, -half_size.y),
		KT::Vector2F(half_size.x, -half_size.y),
		KT::Vector2F(-half_size.x,  half_size.y),
		KT::Vector2F(half_size.x,  half_size.y),
	};
	for (int i = 0; i < 4; ++i) {
		offsets[i] = rotate2D(offsets[i], rotation);
		vertices.push_back(offsets[i] + center);
	}
	return vertices;
}

void DrawOBB2D(sf::RenderWindow& window, const std::vector<KT::Vector2F>& points, sf::Color color = sf::Color::Red)
{
	// On suppose que points contient les 4 coins de l'OBB dans un ordre cohérent
	if (points.size() != 4) return;

	sf::VertexArray lines(sf::PrimitiveType::LineStrip, 5); // 4 coins + retour au premier

	for (size_t i = 0; i < 4; ++i)
	{
		lines[i].position = sf::Vector2f(points[i].At(0), points[i].At(1));
		lines[i].color = color;
	}
	// Boucler pour fermer le rectangle
	lines[4].position = sf::Vector2f(points[0].At(0), points[0].At(1));
	lines[4].color = color;

	window.draw(lines);
}


std::vector<KT::Vector2F> getPts( std::vector<sf::RectangleShape> rect)
{
	std::vector<KT::Vector2F> resultPts;
	for (auto& shape : rect)
	{
		for (int i = 0; i < 4 ; ++i )
		{
			auto local = shape.getPoint(i);
			sf::Vector2f global = shape.getTransform().transformPoint(local);
			resultPts.push_back(KT::Vector2F(global.x, global.y));
		}
	}
	return resultPts;
}

struct testRobot
{
	sf::RectangleShape rectangle;
	KT::AABB2DF GetBox() const;
	KT::BoundingSphere2DF getSphere() const
	{
		auto center = rectangle.getGeometricCenter();
		auto global = rectangle.getTransform().transformPoint(center);
		KT::BoundingSphere2DF box(KT::Vector2F(global.x,global.y),0);
		for (int i = 0; i < rectangle.getPointCount(); ++i)
		{
			auto local = rectangle.getPoint(i);
			sf::Vector2f global = rectangle.getTransform().transformPoint(local);
				box.Expand(KT::Vector2F(global.x, global.y));
			
		}
		return box;
	}
	KT::OBB2DF getBoxOriented() const
	{
		std::vector<KT::Vector2F> pts;
		for (int i = 0; i < 4; ++i)
		{
			auto local = rectangle.getPoint(i);
			sf::Vector2f global = rectangle.getTransform().transformPoint(local);
			pts.push_back(KT::Vector2F(global.x,global.y ));
		}
		return  KT::OBB2DF(pts);
	}
 
	void prossesInput();
};

KT::AABB2DF testRobot::GetBox() const
{
	std::optional<KT::AABB2DF> box;
	for (int i = 0; i < 4; ++i)
	{
		auto local = rectangle.getPoint(i);
		sf::Vector2f global = rectangle.getTransform().transformPoint(local);
		if (box.has_value())
			box->Expand(KT::Vector2F(global.x,global.y ));
		else
		{
			box = KT::AABB2DF(KT::Vector2F(global.x, global.y), KT::Vector2F(global.x, global.y));
		}
	}
	return box.value();
}

void testRobot::prossesInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		rectangle.move(sf::Vector2f{ 1 * 1/60.f ,0 });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
	{
		rectangle.move(sf::Vector2f{ -1 * 1 / 60.f,0 });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
	{
		rectangle.move(sf::Vector2f{ 0 ,-1 * 1 / 60.f });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		rectangle.move(sf::Vector2f{ 0 ,1 * 1 / 60.f });
	}
}

int main(int argc, char** argv)
{
	//KT::CT_Factory_Cast<base, KT::Pointer, KT::typelist<derived, derived2>> ft;

	//ft.Create<derived>(1);
	//KT::Vector3F vec; KT::Vector3F vec2;
	//for (auto& elem : vec)
	//{
	//	std::cout << elem;
	//}
	//using myType = KT::typelist<int, char>;
	//static_assert(KT::is_base_of_v<base,derived> == true, "test");
	//using totoic = KT::unpack_args<myType>::Apply<toto>;
	//totoic	t;
	//auto totofntest = [&](auto... a)-> int {return t.test2(a...); };
	//int toto = KT::unpack_args<myType>::Call<int, totoic::test>(1, 'a');
	//int toto2 = KT::unpack_args<myType>::Call(totofntest,1, 'a');
	//auto vertices = getCubeVertices(KT::Vector3F(1, 1, 1), KT::Vector3F(4, 2, 2), KT::Vector3F(0,0, 45.0f));
	//KT::Vector3F test = KT::GetCenter(vertices);
	//KT::Vector3F test2 = test;
	////KT::VectorND <float, 3> p = vertices[0];
	//KT::OBB<float, 3, KT::Vector3 > oobb(vertices);

	sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML + Eigen Cube");
	std::vector<sf::RectangleShape> allRect;

	sf::RectangleShape t1;
	t1.setPosition({ 300, 300 });
	t1.setSize({ 49, 10 });
	t1.setRotation(sf::degrees(10));
	allRect.push_back(t1);

	// ---- RECTANGLE 2 (touching / near touching) ----
	sf::RectangleShape t2;
	t2.setPosition({ 340, 305 });       // proche de t1
	t2.setSize({ 60, 15 });
	t2.setRotation(sf::degrees(-5));
	allRect.push_back(t2);

	// ---- RECTANGLE 3 ----
	sf::RectangleShape t3;
	t3.setPosition({ 380, 320 });
	t3.setSize({ 50, 20 });
	t3.setRotation(sf::degrees(35));
	allRect.push_back(t3);

	// ---- RECTANGLE 4 ----
	sf::RectangleShape t4;
	t4.setPosition({ 420, 310 });
	t4.setSize({ 40, 30 });
	t4.setRotation(sf::degrees(70));
	allRect.push_back(t4);

	// ---- RECTANGLE 5 (slightly overlapping others) ----
	sf::RectangleShape t5;
	t5.setPosition({ 360, 280 });
	t5.setSize({ 75, 50 });
	//t5.setRotation(sf::degrees(70));
	allRect.push_back(t5);

	// ---- RECTANGLE 6 (farther for contrast) ----
	sf::RectangleShape t6;
	t6.setPosition({ 200, 150 });
	t6.setSize({ 50, 50 });
	allRect.push_back(t6);

	sf::CircleShape t5c;
	t5c.setPosition({ 360, 280 });
	t5c.setRadius(25);
	t5.setRotation(sf::degrees(70));
	allRect.push_back(t5);

	// ---- RECTANGLE 6 (farther for contrast) ----
	sf::CircleShape t6c;
	t6c.setPosition({ 200, 150 });
	t6c.setRadius(25);
	allRect.push_back(t6);

	auto pts = getPts(allRect);
	KT::OBB<float, 2, KT::Vector2> obb(pts);
	auto ptstoDraw = obb.GetPts();

	testRobot r1;
	r1.rectangle = t5;
	testRobot r2;

		r2.rectangle = t6;

	std::cout << obb.axes[0] << " \n";
	std::cout << obb.axes[1] << " \n";
	while (window.isOpen())
	{
		while (auto event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
			
		}
		// Input
		r1.prossesInput();

		//Update
		auto result = KT::CollisionSolver::SolveOBB(r1.getBoxOriented(),r2.getBoxOriented());

		if (result.isColliding)
		{
			r1.rectangle.move({ -result.mvt.x,-result.mvt.y });

		}
		// Effacer la fenêtre
		window.clear();

		window.draw(r1.rectangle);

		window.draw(r2.rectangle);


		// Afficher la fenêtre
		window.display();
	}

	return 0;
}

 
