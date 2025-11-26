#include "Tools/Chrono.h"
#include "Tools/Typelist.h"
#include <iostream>
#include "Math/Vector3.h"
#include "Tools/PrimaryTypeTrait.h"
#include "Math/Angle.h"
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
	virtual ~base();
};
struct derived : public base
{
};
struct derived2
{
	
};


int main(int argc, char** argv)
{
	KT::Vector3F vec; KT::Vector3F vec2;
	
	for (auto& elem : vec)
	{
		std::cout << elem;
	}
	using myType = KT::typelist<int, char>;
	static_assert(KT::is_base_of_v<base,derived> == true, "test");
	using totoic = KT::unpack_args<myType>::Apply<toto>;
	totoic	t;
	auto totofntest = [&](auto... a)-> int {return t.test2(a...); };
	int toto = KT::unpack_args<myType>::Call<int, totoic::test>(1, 'a');
	int toto2 = KT::unpack_args<myType>::Call(totofntest,1, 'a');
    return 0;
}

 
