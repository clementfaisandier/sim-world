#pragma once

#include <string>

inline const std::string kComputeShader =
#include "compute-shader.comp"
;

inline const std::string kFragmentShader =
#include "fragment-shader.frag" 
;

inline const std::string kVertexShader =
#include "vertex-shader.vert" 
;
