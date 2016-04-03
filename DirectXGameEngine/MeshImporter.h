#pragma once

#include <string.h>
#include <errno.h>
#include "MarchMath.h"

#include "Mesh.h"

#include "FileImporter.h"
#include "MeshImportData.h"

class MeshImporter :
    public FileImporter
{
public:
    MeshImporter();
    virtual ~MeshImporter();

    virtual ImportData* loadFromFile( char* filePath );
    virtual Mesh* loadMesh( char* filePath /*, char* textureFilePath*/ );

private:
    std::vector<std::string> tokenize( std::string str, std::string delimiter );
};
