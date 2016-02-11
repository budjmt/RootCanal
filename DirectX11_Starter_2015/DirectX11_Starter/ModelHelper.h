#pragma once

#include <string.h>
#include <errno.h>

#include "Mesh.h"

Mesh* loadOBJ(const char* file);//loads a .obj

std::vector<std::string> tokenize(std::string str, std::string delimiter);