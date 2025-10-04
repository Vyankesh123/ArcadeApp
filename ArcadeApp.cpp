#include <iostream>
#include "Utils/Vec2D.h"

using namespace std;

int main(int argc, const char* argv[])
{
	Vec2D aVec(4,8);

	Vec2D resultVec = aVec * 3;

	cout << resultVec << endl;

	return 0;
}