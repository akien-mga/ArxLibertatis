/*
 * Copyright 2011-2012 Arx Libertatis Team (see the AUTHORS file)
 *
 * This file is part of Arx Libertatis.
 *
 * Arx Libertatis is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Arx Libertatis is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Arx Libertatis.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cppunit/TestCase.h>
#include "graphics/Math.h"
#include "graphics/GraphicsUtility.h"

bool checkFloat(float a, float b) {
	return std::fabs(a-b) < 0.00001f;
}

bool expectMatrix(EERIEMATRIX &mat,EERIEMATRIX &other) {
	return  checkFloat(mat._11,other._11) && checkFloat(mat._12,other._12) && checkFloat(mat._13,other._13) && checkFloat(mat._14,other._14) &&
			checkFloat(mat._21,other._21) && checkFloat(mat._22,other._22) && checkFloat(mat._23,other._23) && checkFloat(mat._24,other._24) &&
			checkFloat(mat._31,other._31) && checkFloat(mat._32,other._32) && checkFloat(mat._33,other._33) && checkFloat(mat._34,other._34) &&
			checkFloat(mat._41,other._41) && checkFloat(mat._42,other._42) && checkFloat(mat._43,other._43) && checkFloat(mat._44,other._44);
}

class GraphicsUtilityTest : public CppUnit::TestCase {
public:
  GraphicsUtilityTest(std::string name) : CppUnit::TestCase(name) {}

  void runTest() {
	  EERIE_TRANSFORM transform;
	  EERIEMATRIX matrix;
	  EERIEMATRIX expected;

	  transform.pos = Vec3f(0.f, 0.f, 0.f);
	  transform.updateFromAngle(Anglef(0.f, 0.f, 0.f));
	  Util_SetViewMatrix(matrix, transform);
	  expected.setToIdentity();
	  CPPUNIT_ASSERT(expectMatrix(matrix, expected));

	  transform.pos = Vec3f(0.f, 0.f, 0.f);
	  transform.updateFromAngle(Anglef(90.f, 90.f, 0.f));
	  Util_SetViewMatrix(matrix, transform);
	  expected.setToIdentity();
	  expected._12 = -8.35187172e-23f;
	  expected._13 = 4.37113883e-08f;
	  expected._21 = 4.37113883e-08f;
	  expected._22 = 1.91068547e-15f;
	  expected._23 = -1.f;
	  expected._32 = 1.f;
	  expected._33 = 1.91068547e-15f;
	  CPPUNIT_ASSERT(expectMatrix(matrix, expected));

	  transform.updateFromAngle(Anglef(-90.f, -90.f, 0.f));
	  Util_SetViewMatrix(matrix, transform);
	  expected.setToIdentity();
	  expected._11 = -1.f;
	  expected._12 = 8.35187172e-23f;
	  expected._13 = -4.37113883e-08f;
	  expected._21 = -4.37113883e-08f;
	  expected._22 = -1.91068547e-15f;
	  expected._23 = 1.f;
	  expected._32 = 1.f;
	  expected._33 = 1.91068547e-15f;
	  CPPUNIT_ASSERT(expectMatrix(matrix, expected));

	  transform.updateFromAngle(Anglef(180.f, 180.f, 0.f));
	  Util_SetViewMatrix(matrix, transform);
	  expected.setToIdentity();
	  expected._12 = 7.64274186e-15f;
	  expected._13 = -8.74227766e-08f;
	  expected._23 = 8.74227766e-08f;
	  expected._31 = 8.74227766e-08f;
	  expected._32 = -8.74227766e-08f;
	  CPPUNIT_ASSERT(expectMatrix(matrix, expected));

	  transform.pos = Vec3f(10.f, 200.f, 3000.f);
	  transform.updateFromAngle(Anglef(0.f, 0.f, 0.f));
	  Util_SetViewMatrix(matrix, transform);
	  expected.setToIdentity();
	  expected._41 = -10;
	  expected._42 = 200;
	  expected._43 = -3000;
	  CPPUNIT_ASSERT(expectMatrix(matrix, expected));
  }
};

int main(int argc, char *argv[]) {
  GraphicsUtilityTest test("GraphicsUtilityTest");
  test.runTest();
}
