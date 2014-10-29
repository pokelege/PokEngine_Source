#pragma once
#pragma warning ( disable : 4201)
#pragma warning ( disable : 4127)
#pragma warning ( disable : 4251)
#pragma warning ( disable : 4239)
#ifdef DEBUG_SHAPES
#include <PokEngineDebugExportHeader.h>
#include <glm.hpp>

#define MAXDEBUGBUFFERSIZE (sizeof(char) * 1048576)

class POKENGINEDEBUGTOOLS_SHARED DebugDrawManager
{
	struct POKENGINEDEBUGTOOLS_SHARED DebugShape
	{
		float duration;
		bool depthEnabled;
		bool isAlreadyPerspective;
		virtual void draw() = 0;
		virtual bool update( const float dt ) = 0;
	};

	struct POKENGINEDEBUGTOOLS_SHARED DebugPoint : public DebugShape
	{
		glm::mat4 transformation;
		DebugPoint( const glm::mat4& transformation = glm::mat4() , const float duration = 0 , const bool depthEnabled = true , const bool isAlreadyPerspective = false );
		void draw( );
		bool update(const float dt);
	};

	struct POKENGINEDEBUGTOOLS_SHARED DebugLine : public DebugShape
	{
		glm::mat4 transformation;
		glm::vec4 color;
		DebugLine( const glm::vec3& from = glm::vec3() , const glm::vec3& to = glm::vec3( 1.0f , 0 , 0 ) , const glm::vec4& color = glm::vec4( 1 , 1 , 1 , 1 ) , const float duration = 0 , const bool depthEnabled = true , const bool isAlreadyPerspective = false );
		void draw( );
		bool update( const float dt );
	};

	struct POKENGINEDEBUGTOOLS_SHARED DebugUnitVector : public DebugShape
	{
		glm::mat4 transformation;
		glm::vec4 color;
		DebugUnitVector( const glm::vec3& from = glm::vec3() , const glm::vec3& to = glm::vec3( 1.0f , 0 , 0 ) , const glm::vec4& color = glm::vec4( 1 , 1 , 1 , 1 ) , const float duration = 0 , const bool depthEnabled = true , const bool isAlreadyPerspective = false );
		void draw();
		bool update( const float dt );
	};

	struct POKENGINEDEBUGTOOLS_SHARED DebugCube: public DebugShape
	{
		glm::mat4 transformation;
		glm::vec4 color;
		DebugCube( const glm::mat4& transformation = glm::mat4() , const glm::vec4& color = glm::vec4( -1 , -1 , -1 , -1 ) , const float duration = 0 , const bool depthEnabled = true , const bool isAlreadyPerspective = false );
		void draw();
		bool update( const float dt );
	};

	struct POKENGINEDEBUGTOOLS_SHARED DebugSphere : public DebugShape
	{
		glm::mat4 transformation;
		glm::vec4 color;
		DebugSphere( const glm::mat4& transformation = glm::mat4() , const glm::vec4& color = glm::vec4( -1 , -1 , -1 , -1 ) , const float duration = 0 , const bool depthEnabled = true , const bool isAlreadyPerspective = false );
		void draw();
		bool update( const float dt );
	};

	struct POKENGINEDEBUGTOOLS_SHARED DebugVector : public DebugShape
	{
		glm::mat4 cylTransformation;
		glm::mat4 coneTransformation;
		glm::vec4 color;
		float size;
		DebugVector( const glm::vec3& from = glm::vec3() , const glm::vec3& to = glm::vec3( 1.0f , 0 , 0 ) , const glm::vec4& color = glm::vec4( 1 , 1 , 1 , 1 ) , const float headLocation = 0, const float size = 1, const float duration = 0 , const bool depthEnabled = true , const bool isAlreadyPerspective = false );
		void draw();
		bool update( const float dt );
	};

	static DebugShape** shapes;
	static unsigned int currentArraySize;
	static unsigned int physicalArraySize;

	static unsigned int bufferID;

	static unsigned int cubeID;
	static unsigned int cubeIndex;
	static unsigned int cubeNumIndices;

	static unsigned int sphereID;
	static unsigned int sphereIndex;
	static unsigned int sphereNumIndices;

	static unsigned int cylID;
	static unsigned int cylIndex;
	static unsigned int cylNumIndices;

	static unsigned int coneID;
	static unsigned int coneIndex;
	static unsigned int coneNumIndices;

	static unsigned int lineID;
	static unsigned int lineIndex;
	static unsigned int lineNumIndex;

	static unsigned int shader;

	static void addDebugShape( DebugShape* shape );
	static void deleteDebugShape( unsigned int id );

public:

	static void initialize( const char* objectsLocation = 0 );

	static void addPoint( const glm::mat4& centerTransform ,
						  const float duration = 0.0f ,
						  const bool depthEnabled = true,
						  const bool isAlreadyPerspective = false);

	static void addLine( const glm::vec3& fromPosition ,
						 const glm::vec3& toPosition ,
						 const glm::vec4& color ,
						 const float duration = 0.0f ,
						 const bool depthEnabled = true ,
						 const bool isAlreadyPerspective = false );

	static void addUnitVector( const glm::vec3& fromPosition ,
							   const glm::vec3& toPosition ,
							   const glm::vec4& color ,
							   const float duration = 0.0f ,
							   const bool depthEnabled = true ,
							   const bool isAlreadyPerspective = false );

	static void addSphere( const glm::mat4 centerTransform ,
						   const glm::vec4 color ,
						   const float duration = 0.0f ,
						   const bool depthEnabled = true ,
						   const bool isAlreadyPerspective = false );

	static void addCube( const glm::mat4 centerTransform ,
						 const glm::vec4 color ,
						 const float duration = 0.0f ,
						 const bool depthEnabled = true ,
						 const bool isAlreadyPerspective = false );

	static void addVector( const glm::vec3& fromPosition ,
						   const glm::vec3& toPosition ,
						   const glm::vec4& color ,
						   const float headLocation = 0 ,
						   const float size = 1 ,
						   const float duration = 0.0f ,
						   const bool depthEnabled = true ,
						   const bool isAlreadyPerspective = false );

	static void update( const float dt );
	static void draw( glm::mat4& viewMatrix = glm::mat4() , glm::mat4& perspectiveMatrix = glm::mat4() );
	static void cleanUp();
};

#define DebugDrawInitialize DebugDrawManager::initialize()
#define DebugDrawInitializeF(dir) DebugDrawManager::initialize(dir)
#define DebugDrawUpdate(dt) DebugDrawManager::update(dt)
#define DebugDrawDraw DebugDrawManager::draw()
#define DebugDrawDrawD(view, perp) DebugDrawManager::draw(view, perp)
#define DebugDrawAddLine(from, to, color, duration, depth) DebugDrawManager::addLine(from, to, color, duration, depth)
#define DebugDrawAddLineD(from, to, color, duration, depth, perp) DebugDrawManager::addLine(from, to, color, duration, depth, perp)
#define DebugDrawAddPoint(transform, duration, depth) DebugDrawManager::addPoint(transform, duration, depth)
#define DebugDrawAddPointD(transform, duration, depth, perp) DebugDrawManager::addPoint(transform, duration, depth, perp)
#define DebugDrawAddUnitVector(from,to,color,duration,depth) DebugDrawManager::addUnitVector(from,to,color,duration,depth)
#define DebugDrawAddUnitVectorD(from,to,color,duration,depth, perp) DebugDrawManager::addUnitVector(from,to,color,duration,depth, perp)
#define DebugDrawAddSphere(transform, color, duration, depth) DebugDrawManager::addSphere(transform, color, duration, depth)
#define DebugDrawAddSphereD(transform, color, duration, depth, perp) DebugDrawManager::addSphere(transform, color, duration, depth, perp)
#define DebugDrawAddCube(transform, color, duration, depth) DebugDrawManager::addCube(transform, color, duration, depth)
#define DebugDrawAddCubeD(transform, color, duration, depth,perp) DebugDrawManager::addCube(transform, color, duration, depth,perp)
#define DebugDrawAddtVector(from,to,color, headPos, size ,duration,depth) DebugDrawManager::addVector(from,to,color, headPos, size,duration,depth)
#define DebugDrawAddVectorD(from,to,color, headPos, size,duration,depth, perp) DebugDrawManager::addVector(from,to,color,  headPos, size, duration,depth, perp)
#define DebugDrawCleanUp DebugDrawManager::cleanUp()
#else
#define DebugDrawInitialize
#define DebugDrawInitializeF(dir)
#define DebugDrawUpdate(dt)
#define DebugDrawDraw
#define DebugDrawDrawD(view, perp)
#define DebugDrawAddLine(from, to, color, duration, depth)
#define DebugDrawAddLineD(from, to, color, duration, depth, perp)
#define DebugDrawAddPoint(transform, duration, depth)
#define DebugDrawAddPointD(transform, duration, depth, perp)
#define DebugDrawAddUnitVector(from,to,color,duration,depth)
#define DebugDrawAddUnitVectorD(from,to,color,duration,depth, perp)
#define DebugDrawAddSphere(transform, color, duration, depth)
#define DebugDrawAddSphereD(transform, color, duration, depth, perp)
#define DebugDrawAddCube(transform, color, duration, depth)
#define DebugDrawAddCubeD(transform, color, duration, depth,perp)
#define DebugDrawAddtVector(from,to,color, headPos, size ,duration,depth)
#define DebugDrawAddVectorD(from,to,color, headPos, size,duration,depth, perp)
#define DebugDrawCleanUp
#endif