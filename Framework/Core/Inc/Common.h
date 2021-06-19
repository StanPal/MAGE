#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

//Included internally by the library your library might use a bunch of stuff you don't want another 
//person or user to grap the library you're using
//Add a facade header library.h(everything we depend on is included) 
//Everything in the inc folder are also included

// Win32 headers
#include <objbase.h>
#include <Windows.h>

// Standard headers
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

// External headers
#include <RapidJSON/Inc/document.h>
#include <RapidJSON/Inc/filereadstream.h>
#include <RapidJSON/Inc/filewritestream.h>