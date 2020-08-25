#pragma once
#include <bitset>
#include <cstdint>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <SDL.h>

inline void _GLGetError(const char* file, int line, const char* call) {
	while (GLenum error = glGetError()) {
		std::cout << "[Gl Error(" << error << ")]: " << glewGetErrorString(error) << " in " << call << std::endl;
		std::cout << file << ":" << line << std::endl;
	}
}

inline static const char* humanSize(uint64_t bytes) {
	const char* suffix[] = {"B", "KB", "MB", "GB", "TB"};
	char        length   = sizeof(suffix) / sizeof(suffix[0]);

	int    i        = 0;
	double dblBytes = bytes;

	if (bytes > 1024) {
		for (i = 0; (bytes / 1024) > 0 && i < length - 1; i++, bytes /= 1024) dblBytes = bytes / 1024.0;
	}

	static char output[200];
	sprintf_s(output, "%.02lf %s", dblBytes, suffix[i]);
	return output;
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

typedef uint8_t        UInt8;
typedef uint16_t       UInt16;
typedef uint32_t       UInt32;
typedef uint64_t       UInt64;
typedef std::bitset<8> Byte;

typedef float  Float32;
typedef double Float64;

struct Vertex {
	Float32 x;
	Float32 y;
	Float32 z;

	Float32 u;
	Float32 v;

	Float32 r;
	Float32 g;
	Float32 b;
	Float32 a;

	std::string XYZ() {
		return "{"+ std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "}";
	}
};
