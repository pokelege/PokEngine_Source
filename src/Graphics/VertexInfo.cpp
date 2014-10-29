#include <Graphics\VertexInfo.h>
#include <Graphics\PokEngineModelDataMap.h>

unsigned int VertexInfo::POSITION_OFFSET = 0;
unsigned int VertexInfo::UV_OFFSET = sizeof(glm::vec3);
unsigned int VertexInfo::NORMAL_OFFSET = sizeof( glm::vec3 ) + sizeof(glm::vec2);
unsigned int VertexInfo::TANGENT_OFFSET = (sizeof( glm::vec3 ) * 2) + sizeof( glm::vec2 );
unsigned int VertexInfo::BITANGENT_OFFSET = ( sizeof( glm::vec3 ) * 3 ) + sizeof( glm::vec2 );
unsigned int VertexInfo::BLENDINGINDEX_OFFSET = ( sizeof( glm::vec3 ) * 4 ) + sizeof( glm::vec2 );
unsigned int VertexInfo::BLENDINGWEIGHT_OFFSET = ( sizeof( glm::vec3 ) * 4 ) + sizeof( glm::vec2 ) + sizeof(glm::ivec4);
unsigned int VertexInfo::STRIDE = ( sizeof( glm::vec3 ) * 4 ) + sizeof( glm::vec2 ) + sizeof( glm::ivec4 ) + sizeof(glm::vec4);