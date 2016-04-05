#include "MeshImporter.h"

MeshImporter::MeshImporter()
{
}

MeshImporter::~MeshImporter()
{
}

ImportData* MeshImporter::loadFromFile( char* filePath )
{
    //cout << "Loading " << file << endl;
    FILE* infile = fopen( filePath, "r" );
    if( !infile ) {
        fprintf( stderr, "Error! File %s could not be read.\nError Code %d: %s\n", filePath, errno, strerror( errno ) );
        return nullptr;
    }
    else
        printf( "File %s Loading...\n", filePath );

    std::vector<vec3> verts, normals, uvs;
    Face faces = Face();

    char line[255];
    while( fscanf( infile, "%[^\n]\n", line ) != EOF ) {

        std::vector<std::string> tokens = tokenize( line, " " );

        if( line[0] == 'v' ) {
            //verts
            if( line[1] == ' ' )
                verts.push_back( vec3( (float)atof( tokens[1].c_str() ), (float)atof( tokens[2].c_str() ), (float)atof( tokens[3].c_str() ) ) );
            //normals
            else if( line[1] == 'n' )
                normals.push_back( vec3( (float)atof( tokens[1].c_str() ), (float)atof( tokens[2].c_str() ), (float)atof( tokens[3].c_str() ) ) );
            //uvs
            else if( line[1] == 't' )
                uvs.push_back( vec3( (float)atof( tokens[1].c_str() ), (float)atof( tokens[2].c_str() ), 0 ) );
        }
        else if( line[0] == 'f' ) {
            for( unsigned int i = 1; i < tokens.size(); i++ ) {
                std::vector<std::string> faceTokens = tokenize( tokens[i], "/" );
                uint32_t v = (uint32_t)atoi( faceTokens[0].c_str() ) - 1
                    , u = (uint32_t)atoi( faceTokens[1].c_str() ) - 1
                    , n = (uint32_t)atoi( faceTokens[2].c_str() ) - 1;
                faces.verts.push_back( v );
                faces.uvs.push_back( u );
                faces.normals.push_back( n );
            }
        }
    }

    printf( "Complete!\n" );
    fclose( infile );
    Mesh* mesh = new Mesh( verts, normals, uvs, faces );

    MeshImportData* importData = new MeshImportData( mesh );
    return importData;
}

Mesh* MeshImporter::loadMesh( char* filePath /*, char* textureFilePath*/ )
{
    ImportData* importData = loadFromFile( filePath );
    MeshImportData* meshImportData = static_cast<MeshImportData*>( importData );
	assert(meshImportData);
    Mesh* mesh = meshImportData->getMesh();

    delete meshImportData;
  //  delete importData;

    return mesh;
}

std::vector<std::string> MeshImporter::tokenize( std::string str, std::string delimiter )
{
    std::vector<std::string> tokens;
    int curr, prev;
    for( curr = str.find( delimiter, 0 ), prev = 0; curr != std::string::npos; prev = curr + 1, curr = str.find( delimiter, prev ) )
        if( curr - prev > 0 )
            tokens.push_back( str.substr( prev, curr - prev ) );

    curr = str.length();
    if( curr - prev > 0 )
        tokens.push_back( str.substr( prev, curr - prev ) );
    return tokens;
}

