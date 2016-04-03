#include "MeshImportData.h"

MeshImportData::MeshImportData( Mesh* mesh )
{
    _mesh = mesh;
}

MeshImportData::~MeshImportData()
{
}

Mesh* MeshImportData::getMesh()
{
    return _mesh;
}
