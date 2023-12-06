#include "math.h"



Matrix4f::Matrix4f(unsigned int mode, float x, float y, float z) {
	
	switch (mode) {
	case Matrix4f::TRANSLATION_MODE:
		BuildTranslationMatrix(x, y, z);
		break;
	default:
		printf("Matrix4f Error: mode incorrect.\n");
	}
}

void Matrix4f::BuildTranslationMatrix(float x, float y, float z) {

	float* m = Matrix4f::matrix;

	m[0] = 1.0; m[1] = 0.0; m[2] = 0.0; m[3] = x;
	m[4] = 0.0; m[5] = 1.0; m[6] = 0.0; m[7] = y;
	m[8] = 0.0; m[9] = 0.0; m[10] = 1.0; m[11] = z;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1.0;
}