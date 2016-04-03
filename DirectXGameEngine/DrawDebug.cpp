#include "DrawDebug.h"
#include <iostream>
DrawDebug::DrawDebug() {
#if DEBUG
    DXInfo& d = DXInfo::getInstance();

    D3D11_RASTERIZER_DESC rdesc = d.rasterDesc;

    //set up render modes
    rdesc.FillMode = D3D11_FILL_WIREFRAME;
    d.device->CreateRasterizerState( &rdesc, &wireframe );
    assert( wireframe != nullptr );
    rdesc.FillMode = D3D11_FILL_SOLID;
    d.device->CreateRasterizerState( &rdesc, &fill );
    assert( fill != nullptr );

    shaderSetup( d );
    bufferSetup( d );
#endif
}

DrawDebug::~DrawDebug() {
#if DEBUG
    delete sphere; delete arrow;
    sphere = arrow = nullptr;
    svb->Release(); sib->Release(); sinstb->Release();
    avb->Release(); aib->Release(); ainstb->Release();
    vvb->Release();
    svb = sib = sinstb = avb = aib = ainstb = vvb = nullptr;
#endif
}

void DrawDebug::shaderSetup( DXInfo& d ) {
    vecVert = new SimpleVertexShader( d.device, d.deviceContext );
    vecVert->LoadShaderFile( L"DebugVecVertex.cso" );
    vecPixel = new SimplePixelShader( d.device, d.deviceContext );
    vecPixel->LoadShaderFile( L"DebugVecPixel.cso" );

    std::vector<D3D11_INPUT_ELEMENT_DESC> meshDescs;
    meshDescs.push_back( D3D11_INPUT_ELEMENT_DESC{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 } );
    meshDescs.push_back( D3D11_INPUT_ELEMENT_DESC{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 } );
    auto mat = D3D11_INPUT_ELEMENT_DESC{ "WORLD_MAT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
    meshDescs.push_back( mat );
    mat.SemanticIndex = 1; meshDescs.push_back( mat );
    mat.SemanticIndex = 2; meshDescs.push_back( mat );
    mat.SemanticIndex = 3; meshDescs.push_back( mat );
    meshDescs.push_back( D3D11_INPUT_ELEMENT_DESC{ "COLOR",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 } );

    meshVert = new SimpleVertexShader( d.device, d.deviceContext );
    meshVert->LoadShaderFile( L"DebugMeshVertex.cso" );
    HRESULT hr = d.device->CreateInputLayout( &meshDescs[0], meshDescs.size(), meshVert->GetShaderBlob()->GetBufferPointer(), meshVert->GetShaderBlob()->GetBufferSize(), &( meshVert->inputLayout ) );

    meshPixel = new SimplePixelShader( d.device, d.deviceContext );
    meshPixel->LoadShaderFile( L"DebugMeshPixel.cso" );
}

void DrawDebug::bufferSetup( DXInfo& d ) {
    D3D11_BUFFER_DESC vbd, ibd;
    D3D11_SUBRESOURCE_DATA initialVertexData, initialIndexData;

    //vector setup
    vbd.Usage = D3D11_USAGE_DYNAMIC;
    vbd.ByteWidth = sizeof( DebugVector ) * MAX_VECTORS;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vbd.MiscFlags = 0; vbd.StructureByteStride = 0;
    HR( d.device->CreateBuffer( &vbd, nullptr, &vvb ) );

    //arrow setup
    MeshImporter importer;
    arrow = importer.loadMesh( "Assets/_debug/arrow.obj" );
    assert( arrow != nullptr );
    vec3 c = arrow->getCentroid();
    if( vec3::length( c ) > FLT_EPSILON )
        arrow->adjustLoc( c * -1.f );
    DebugMeshBuffer dm = genDebugMeshArrays( arrow );

    //vertex buffer
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof( DebugVertex ) * dm.meshArray.size();
    vbd.CPUAccessFlags = 0;

    initialVertexData.pSysMem = &dm.meshArray[0];
    HR( d.device->CreateBuffer( &vbd, &initialVertexData, &avb ) );

    //instance buffer
    ibd.Usage = D3D11_USAGE_DYNAMIC;
    ibd.ByteWidth = sizeof( DebugMesh ) * MAX_VECTORS / 2;
    ibd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    ibd.MiscFlags = 0; ibd.StructureByteStride = 0;
    HR( d.device->CreateBuffer( &ibd, nullptr, &ainstb ) );

    //index buffer
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof( uint32_t ) * dm.meshElementArray.size();
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; ibd.CPUAccessFlags = 0;

    initialIndexData.pSysMem = &dm.meshElementArray[0];
    HR( d.device->CreateBuffer( &ibd, &initialIndexData, &aib ) );

    //sphere setup
    sphere = importer.loadMesh( "Assets/_debug/sphere.obj" );
    assert( sphere != nullptr );
    c = sphere->getCentroid();
    if( vec3::length( c ) > FLT_EPSILON )
        sphere->adjustLoc( c * -1.f );
    dm = genDebugMeshArrays( sphere );

    //vertex buffer
    vbd.ByteWidth = sizeof( DebugVertex ) * dm.meshArray.size();
    initialVertexData.pSysMem = &dm.meshArray[0];
    HR( d.device->CreateBuffer( &vbd, &initialVertexData, &svb ) );

    //index buffer
    ibd.ByteWidth = sizeof( uint32_t ) * dm.meshElementArray.size();
    initialIndexData.pSysMem = &dm.meshElementArray[0];
    HR( d.device->CreateBuffer( &ibd, &initialIndexData, &sib ) );

    //instance buffer
    ibd.Usage = D3D11_USAGE_DYNAMIC;
    ibd.ByteWidth = sizeof( DebugMesh ) * MAX_SPHERES;
    ibd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    HR( d.device->CreateBuffer( &ibd, nullptr, &sinstb ) );
}

DrawDebug& DrawDebug::getInstance() {
    static DrawDebug instance;
    return instance;
}

void DrawDebug::camera( Camera** c ) { cam = c; }

void DrawDebug::draw() {
#if DEBUG
    drawVectors();
    drawSpheres();
    DXInfo& d = DXInfo::getInstance();
    d.deviceContext->RSSetState( d.rasterState );
    d.deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
#endif
}

void DrawDebug::drawVectors() {
    vecBufferData.push_back( DebugVector{ DirectX::XMFLOAT4{ 0,0,0,0 }, DirectX::XMFLOAT4{ 0,0,0,0 } } );
    vecBufferData.push_back( DebugVector{ DirectX::XMFLOAT4{ 0,0,0,0 }, DirectX::XMFLOAT4{ 0,0,0,0 } } );
    arrowBufferData.push_back( DebugMesh{ DirectX::XMFLOAT4X4( 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ), DirectX::XMFLOAT4{ 0,0,0,0 } } );

    for( int i = 0, numVecs = debugVectors.size(); i < numVecs; i += 4 ) {
        vec3 s = debugVectors[i], sc = debugVectors[i + 1], e = debugVectors[i + 2], ec = debugVectors[i + 3];
        DirectX::XMFLOAT4 xmec = DirectX::XMFLOAT4{ ec.x, ec.y, ec.z, 1 };
        vecBufferData.push_back( DebugVector{
            DirectX::XMFLOAT4{ s.x, s.y, s.z, 1 },
            DirectX::XMFLOAT4{ sc.x, sc.y, sc.z, 1 }
        } );
        vecBufferData.push_back( DebugVector{
            DirectX::XMFLOAT4{ e.x, e.y, e.z, 1 },
            xmec
        } );

        vec3 dir = e - s;
        dir /= vec3::length( dir );
        mat4 arrowMat = mat4::scale( vec3( 0.05f, 0.05f, 0.05f ) ) * ( mat4::lookAt( vec3(), dir, vec3( 0, 0, 1 ) ) * mat4::translate( e ) );
        //mat4 arrowMat = mat4::scale(vec3(0.05f, 0.05f, 0.05f)) * mat4::translate(e);
        arrowBufferData.push_back( DebugMesh{ DirectX::XMFLOAT4X4( &arrowMat[0][0] ), xmec } );
    }

    DXInfo& d = DXInfo::getInstance();
    if( ( *cam ) != nullptr ) { ( *cam )->updateCamMat( vecVert ); ( *cam )->updateCamMat( meshVert ); }

    //upload and draw vector part
    UINT vstride = sizeof( DebugVector ), istride;
    UINT voffset = 0, ioffset = 0;
    d.deviceContext->IASetVertexBuffers( 0, 1, &vvb, &vstride, &voffset );

    //upload
    D3D11_MAPPED_SUBRESOURCE mappedRes;
    ZeroMemory( &mappedRes, sizeof( D3D11_MAPPED_SUBRESOURCE ) );
    d.deviceContext->Map( vvb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes );
    memcpy( mappedRes.pData, &vecBufferData[0], vecBufferData.size() * vstride );
    d.deviceContext->Unmap( vvb, 0 );

    //draw
    vecVert->SetShader( true );
    vecPixel->SetShader( true );
    d.deviceContext->RSSetState( wireframe );
    d.deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );
    d.deviceContext->Draw( vecBufferData.size(), 0 );

    //upload and draw arrow part
    vstride = sizeof( DebugVertex ); istride = sizeof( DebugMesh );
    ID3D11Buffer* buffs[] = { avb, ainstb };
    UINT strides[] = { vstride, istride }, offsets[] = { voffset, ioffset };
    d.deviceContext->IASetVertexBuffers( 0, 2, buffs, strides, offsets );
    d.deviceContext->IASetIndexBuffer( aib, DXGI_FORMAT_R32_UINT, 0 );

    //upload
    ZeroMemory( &mappedRes, sizeof( D3D11_MAPPED_SUBRESOURCE ) );
    d.deviceContext->Map( ainstb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes );
    memcpy( mappedRes.pData, &arrowBufferData[0], arrowBufferData.size() * istride );
    d.deviceContext->Unmap( ainstb, 0 );

    //draw
    meshVert->SetShader( true );
    meshPixel->SetShader( true );
    d.deviceContext->RSSetState( fill );
    d.deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    d.deviceContext->DrawIndexedInstanced( arrow->meshBuffer().meshElementArray.size(), arrowBufferData.size(), 0, 0, 0 );

    debugVectors = std::vector<vec3>();
    vecBufferData = std::vector<DebugVector>();
    arrowBufferData = std::vector<DebugMesh>();
}

void DrawDebug::drawSpheres() {
    DXInfo& d = DXInfo::getInstance();
    if( ( *cam ) != nullptr ) ( *cam )->updateCamMat( meshVert );

    sphereBufferData.push_back( DebugMesh{ DirectX::XMFLOAT4X4( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ), DirectX::XMFLOAT4{ 0,0,0,0 } } );
    for( int i = 0, numSpheres = debugSpheres.size(); i < numSpheres; i++ ) {
        Sphere s = debugSpheres[i];
        mat4 translate, scale;
        translate = mat4::translate( s.center );
        scale = mat4::scale( vec3( 1, 1, 1 ) * ( s.rad * 2 ) );
        sphereBufferData.push_back( DebugMesh{ DirectX::XMFLOAT4X4( &( scale * translate )[0][0] ), DirectX::XMFLOAT4{ 0.2f, 0.7f, 0.9f, 0.3f } } );
    }
    UINT vstride = sizeof( DebugVertex ), istride = sizeof( DebugMesh );
    UINT voffset = 0, ioffset = 0;
    ID3D11Buffer* buffs[] = { svb, sinstb };
    UINT strides[] = { vstride, istride }, offsets[] = { voffset, ioffset };
    d.deviceContext->IASetVertexBuffers( 0, 2, buffs, strides, offsets );
    d.deviceContext->IASetIndexBuffer( sib, DXGI_FORMAT_R32_UINT, 0 );

    //upload
    D3D11_MAPPED_SUBRESOURCE mappedRes;
    ZeroMemory( &mappedRes, sizeof( D3D11_MAPPED_SUBRESOURCE ) );
    d.deviceContext->Map( sinstb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes );
    memcpy( mappedRes.pData, &sphereBufferData[0], sphereBufferData.size() * istride );
    d.deviceContext->Unmap( sinstb, 0 );

    //draw
    meshVert->SetShader( true );
    meshPixel->SetShader( true );
    d.deviceContext->RSSetState( fill );
    d.deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    d.deviceContext->DrawIndexedInstanced( sphere->meshBuffer().meshElementArray.size(), sphereBufferData.size(), 0, 0, 0 );

    debugSpheres = std::vector<Sphere>();
    sphereBufferData = std::vector<DebugMesh>();
}

void DrawDebug::drawDebugVector( vec3 start, vec3 end, vec3 color ) {
#if DEBUG
    debugVectors.push_back( start );
    debugVectors.push_back( color );
    debugVectors.push_back( end );
    debugVectors.push_back( color );
#endif
}

void DrawDebug::drawDebugSphere( vec3 pos, float rad ) {
#if DEBUG
    Sphere s = { pos, rad };
    DrawDebug& d = DrawDebug::getInstance();
    debugSpheres.push_back( s );
    //drawDebugVector(pos, pos + vec3(rad, 0, 0));
#endif
}

DebugMeshBuffer DrawDebug::genDebugMeshArrays( Mesh* m ) {
#if DEBUG
    DebugMeshBuffer dm;
    std::vector<vec3> combs;
    auto faces = m->faces();
    auto verts = m->verts(), normals = m->normals();
    for( uint32_t i = 0, numfaceVerts = faces.verts.size(); i < numfaceVerts; i++ ) {
        bool inArr = false;
        uint32_t index;
        //TODO: fix this bottleneck! It's super duper slow!
        uint32_t numCombs = combs.size();
        for( index = 0; !inArr && index < numCombs; index++ ) {
            if( (uint32_t)combs[index].x == faces.verts[i]
                && (uint32_t)combs[index].z == faces.normals[i] ) {
                inArr = true;
                index--;
            }
        }
        if( !inArr ) {
            combs.push_back( vec3( (float)faces.verts[i], 0, (float)faces.normals[i] ) );
            dm.meshArray.push_back( DebugVertex{
                DirectX::XMFLOAT3( verts[faces.verts[i]].x, verts[faces.verts[i]].y, verts[faces.verts[i]].z ),
                DirectX::XMFLOAT3( normals[faces.normals[i]].x, normals[faces.normals[i]].y, normals[faces.normals[i]].z )
            } );
        }
        dm.meshElementArray.push_back( index );
    }
    return dm;
#endif
}