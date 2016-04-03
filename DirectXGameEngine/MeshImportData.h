#pragma once

#include <vector>
#include <DirectXMath.h>
#include "ImportData.h"
#include "Mesh.h"
#include "Vertex.h"

class MeshImportData :
    public ImportData
{
public:
    MeshImportData( Mesh* mesh );
    ~MeshImportData();

    Mesh* getMesh();

private:
    Mesh* _mesh;
};