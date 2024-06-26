#pragma once

#ifdef __linux__
#define WINDOW_API_X11
#else
#error "Platform not supported"
#endif

using int8 = char;              //< Signed 8-bit integer
using uint8 = unsigned char;    //< Unsigned 8-bit integer

using int32 = int;              //< Signed 32-bit integer
using uint32 = unsigned int;    //< Unsigned 32-bit integer

using int64 = long;             //< Signed 64-bit integer
using uint64 = unsigned long;   //< Unsigned 64-bit integer

using float32 = float;          //< 32-bit floating point
using float64 = double;         //< 64-bit floating point

static constexpr const char* const ENGINE_NAME = "Voxels"; //< The engine name
static constexpr uint32 ENGINE_VERSION_MAJOR = 0; //< The engine major version
static constexpr uint32 ENGINE_VERSION_MINOR = 0; //< The engine minor version
static constexpr uint32 ENGINE_VERSION_PATCH = 1; //< The engine patch version

static constexpr const uint32 MAX_OCT_TREE_DEPTH = sizeof(uint64); //< The max depth of the oct tree.
static constexpr const uint32 CHUNK_LENGTH = 1 << MAX_OCT_TREE_DEPTH; //< The length of the chunk or one oct tree.