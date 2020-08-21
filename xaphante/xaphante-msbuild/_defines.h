#pragma once
#include <cstdint>
#include <iostream>

#include <GL/glew.h>

inline void _GLGetError(const char* file, int line, const char* call) {
	while (GLenum error = glGetError()) {
		std::cout << "[Gl Error(" << error << ")]: " << glewGetErrorString(error) << " in " << call << std::endl;
		std::cout << file << ":" << line << std::endl;
	}
}
#ifdef _DEBUG
	#define GL_ERROR ; _GLGetError(__FILE__, __LINE__, __func__);
#else
	#define GL_ERROR ;
#endif

typedef int8_t  Int8;
typedef int16_t Int16;
typedef int32_t Int32;
typedef int64_t Int64;

typedef uint8_t  UInt8;
typedef uint16_t UInt16;
typedef uint32_t UInt32;
typedef uint64_t UInt64;

typedef float  Float32;
typedef double Float64;

struct Vertex {
	Float32 x;
	Float32 y;
	Float32 z;

	Float32 r;
	Float32 g;
	Float32 b;
	Float32 a;
};
