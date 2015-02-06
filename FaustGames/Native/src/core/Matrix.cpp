#include "Matrix.h"

namespace core
{
	Matrix Matrix::identity(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	int MatrixContainer::_id(0);
}