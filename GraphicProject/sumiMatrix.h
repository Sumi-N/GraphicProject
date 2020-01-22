#pragma once
#include <algorithm>
#include <GL/glew.h>

class Matrix {
	GLfloat matrix[16];

public:
	Matrix() {};

	Matrix(const GLfloat *a) {
		std::copy(a, a + 16, matrix);
	}

	const GLfloat *data() const {
		return matrix;
	}

	void loadIdentity() {
		std::fill(matrix, matrix + 16, 0.0f);
		matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0f;
	}

	static Matrix identity() {
		Matrix t;
		t.loadIdentity();
		return t;
	}

	// (x, y, z) だけ平行移動する変換行列を作成する
	static Matrix translate(GLfloat x, GLfloat y, GLfloat z)
	{
		Matrix t;
		t.loadIdentity();
		t.matrix[12] = x;
		t.matrix[13] = y;
		t.matrix[14] = z;
		return t;
	}

	// (x, y, z) 倍に拡大縮小する変換行列を作成する
	static Matrix scale(GLfloat x, GLfloat y, GLfloat z)
	{
		Matrix t;
		t.loadIdentity();
		t.matrix[0] = x;
		t.matrix[5] = y;
		t.matrix[10] = z;
		return t;
	}

	// (x, y, z) を軸に a 回転する変換行列を作成する
	static Matrix rotate(GLfloat a, GLfloat x, GLfloat y, GLfloat z)
	{
		Matrix t;
		const GLfloat d(sqrt(x * x + y * y + z * z));
		if (d > 0.0f)
		{
			const GLfloat l(x / d), m(y / d), n(z / d);
			const GLfloat l2(l * l), m2(m * m), n2(n * n);
			const GLfloat lm(l * m), mn(m * n), nl(n * l);
			const GLfloat c(cos(a)), c1(1.0f - c), s(sin(a));
			t.loadIdentity();
			t.matrix[0] = (1.0f - l2) * c + l2;
			t.matrix[1] = lm * c1 + n * s;
			t.matrix[2] = nl * c1 - m * s;
			t.matrix[4] = lm * c1 - n * s;
			t.matrix[5] = (1.0f - m2) * c + m2;
			t.matrix[6] = mn * c1 + l * s;
			t.matrix[8] = nl * c1 + m * s;
			t.matrix[9] = mn * c1 - l * s;
			t.matrix[10] = (1.0f - n2) * c + n2;
		}
		return t;
	}

	// 乗算
	Matrix operator*(const Matrix &m) const
	{
		Matrix t;
		for (int i = 0; i < 16; ++i)
		{
			const int j(i & 3), k(i & ~3);
			t.matrix[i] =
				matrix[0 + j] * m.matrix[k + 0] +
				matrix[4 + j] * m.matrix[k + 1] +
				matrix[8 + j] * m.matrix[k + 2] +
				matrix[12 + j] * m.matrix[k + 3];
		}
		return t;
	}
};
