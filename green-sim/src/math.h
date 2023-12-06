#include <stdio.h>
#include <stdlib.h>


class Matrix4f {

private:

	void BuildTranslationMatrix(float x, float y, float z);

public:

	static const unsigned int TRANSLATION_MODE = 0;

	float matrix[16];

	Matrix4f(unsigned int mode, float x, float y, float z);

};