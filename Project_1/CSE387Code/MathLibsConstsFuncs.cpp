#include "MathLibsConstsFuncs.h"
#include <iomanip>

glm::mat4 getRotationMatrixFromTransform(const glm::mat4& transform)
{
	return glm::mat4(glm::mat3(transform));

}

glm::vec3 getPositionVec3FromTransform(const glm::mat4& transform)
{
	float x = transform[3][0];
	float y = transform[3][1];
	float z = transform[3][2];
	return glm::vec3(x, y, z);
}

void setPositionVec3ForTransform(glm::mat4& transform, const glm::vec3& position)
{
	transform[3][0] = position.x;
	transform[3][1] = position.y;
	transform[3][2] = position.z;
}

void setRotationMat3ForTransform(glm::mat4& transform, const glm::mat4& rotation)
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			transform[i][j] = rotation[i][j];
		}
	}
}

glm::vec3 getScaleFromTransform(const glm::mat4& transform)
{
	float x = transform[0][0];
	float y = transform[1][1];
	float z = transform[2][2];
	return glm::vec3(x, y, z);
}

void setScaleForTransform(glm::mat4& transform, const glm::vec3& scale)
{
	transform[0][0] = scale.x;
	transform[1][1] = scale.y;
	transform[2][2] = scale.z;
}

/**
 * @fn	ostream &operator<< (ostream &os, const vec2 &V) { os << "[ " << V.x << " " << V.y << " ]"; return os;
 *
 * @brief	Output stream for vec2.
 *
 * @param [in,out]	os	Output stream.
 * @param 		  	V 	The vector.
 *
 * @returns	The shifted result.
 */
ostream &operator << (ostream &os, const vec2 &V)
{
	os << "[ " << V.x << " " << V.y << " ]";
	return os;
}

/**
 * @fn	ostream &operator<< (ostream &os, const vec3 &V) { os << "[ " << V.x << " " << V.y << " " << V.z << " ]"; return os;
 *
 * @brief	Output stream for vec3.
 *
 * @param [in,out]	os	Output stream.
 * @param 		  	V 	The vector.
 *
 * @returns	The shifted result.
 */
ostream &operator << (ostream &os, const vec3 &V)
{
	os << "[ " << V.x << " " << V.y << " " << V.z << " ]";
	return os;
}

/**
 * @fn	ostream &operator<< (ostream &os, const vec4 &V) { os << "[ " << V.x << " " << V.y << " " << V.z << " " << V.w << " ]"; return os;
 *
 * @brief	Output stream for vec4.
 *
 * @param [in,out]	os	Output stream.
 * @param 		  	V 	The vector.
 *
 * @returns	The shifted result.
 */
ostream &operator << (ostream &os, const vec4 &V)
{
	os << "[ " << V.x << " " << V.y << " " << V.z << " " << V.w << " ]";
	return os;
}

/**
 * @fn	ostream &operator<< (ostream &os, const mat2 &M) { os << " n"; for (int row = 0; row < 2; row++) { os << "| t"; for (int col = 0; col < 2; col++) { os << std::setw(8) << std::setprecision(4) << M[col][row] << " t";
 *
 * @brief	Output stream for mat2.
 *
 * @param [in,out]	os	Output stream.
 * @param 		  	M 	The matrix.
 *
 * @returns	The shifted result.
 */
ostream &operator << (ostream &os, const mat2 &M)
{
	os << "\n";
	for (int row = 0; row < 2; row++) {
		os << "|\t";
		for (int col = 0; col < 2; col++) {
			os << std::setw(8) << std::setprecision(4) << M[col][row] << "\t";
		}
		os << "|\n";
	}
	os << "\n";
	return os;
}

/**
 * @fn	ostream &operator<< (ostream &os, const mat3 &M) { os << " n"; for (int row = 0; row < 3; row++) { os << "| t"; for (int col = 0; col < 3; col++) { os << std::setw(8) << std::setprecision(4) << M[col][row] << " t";
 *
 * @brief	Output stream for mat3.
 *
 * @param [in,out]	os	Output stream.
 * @param 		  	M 	The matrix.
 *
 * @returns	The shifted result.
 */
ostream &operator << (ostream &os, const mat3 &M)
{
	os << "\n";
	for (int row = 0; row < 3; row++) {
		os << "|\t";
		for (int col = 0; col < 3; col++) {
			os << std::setw(8) << std::setprecision(4) << M[col][row] << "\t";
		}
		os << "|\n";
	}
	os << "\n";
	return os;
}

/**
 * @fn	ostream &operator<< (ostream &os, const mat4 &M) { os << " n"; for (int row = 0; row < 4; row++) { os << "| t"; for (int col = 0; col < 4; col++) { os << std::setw(8) << std::setprecision(4) << M[col][row] << " t";
 *
 * @brief	Output stream for mat4.
 *
 * @param [in,out]	os	Output stream.
 * @param 		  	M 	The matrix.
 *
 * @returns	The shifted result.
 */
ostream &operator << (ostream &os, const mat4 &M)
{
	os << "\n";
	for (int row = 0; row < 4; row++) {
		os << "|\t";
		for (int col = 0; col < 4; col++) {
			os << std::setw(8) << std::setprecision(4) << M[col][row] << "\t";
		}
		os << "|\n";
	}
	os << "\n";
	return os;
}


