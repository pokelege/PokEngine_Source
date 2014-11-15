#pragma once
#include <gtc\quaternion.hpp>
#include <stdarg.h>
//variable arguments (...) are vec3
inline glm::quat rotationBetweenVectors(const glm::vec3& start , const glm::vec3& end, unsigned int numFallbacks = 0, ... )
{
	glm::vec3 startNormal = glm::normalize( start );
	glm::vec3 endNormal = glm::normalize( end );
	float cosTheta = glm::dot( startNormal , endNormal );
	glm::vec3 rotationAxis;
	glm::quat toReturn;
	if ( cosTheta <= -1 )
	{
		va_list ap;
		va_start( ap , numFallbacks );
		bool found = false;
		for ( unsigned int i = 0; i < numFallbacks && !found; ++i )
		{
			rotationAxis = glm::cross(va_arg( ap , glm::vec3 ),start);
			found = glm::length( rotationAxis ) != 0;
		}
		if ( found )
		{
			rotationAxis = glm::normalize( rotationAxis );
			toReturn = glm::angleAxis( glm::radians( 180.0f ) , rotationAxis );
		}
		va_end( ap );
		return toReturn;
	}

	rotationAxis = glm::cross( start , end );
	float s = sqrtf( ( 1 + cosTheta ) * 2 );
	float invs = 1 / s;
	toReturn = glm::quat( s * 0.5f , rotationAxis.x * invs , rotationAxis.y * invs , rotationAxis.z * invs );

	return toReturn;
}