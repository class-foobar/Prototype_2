#include "hmain.h"
#include "camera.h"
#include "sprite.h"
#include "physics.h"
#include "text.hpp"
#include "ships.h"
#include "universe.h"
#include "button.h"
#include "text.hpp"
using namespace GAME;
using namespace D2D1;
using namespace std;
namespace DX2D
{
	vector<button*> btnspr;
	controls* currentlycheckingc;
	void GetAcB(physobj* obj1, physobj* obj2)
	{
		physobj* obj = nullptr;
		if (obj1->nameindex != "mouse")
			obj = obj1;
		else if (obj2->nameindex != "mouse")
			obj = obj2;
		if (obj != nullptr)
			btnspr.push_back(currentlycheckingc->buttonSTRmap[obj->nameindex]);
	}
}