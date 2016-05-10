#pragma once

#include <vector>

#include "Scene.h"
#include "GameObject.h"
#include "Mouse.h"

class State
{
public:
    State( Scene* scene )
    {
        _scene = scene;
    }
    virtual ~State() {
		for (auto g : _gameObjects) {
			if (g) {
				delete g;
				g = nullptr;
			}
		}
    };

    virtual void addGameObject( GameObject* g )
    {
        _gameObjects.push_back( g );
    }

    virtual void update( float dt, Mouse* mouse ) 
    {
        for( auto g : _gameObjects )
            g->update( dt, mouse );
    };
    virtual void draw( ID3D11DeviceContext* deviceContext)
    {
        for( auto g : _gameObjects )
            g->draw( deviceContext );
    };

protected:
    Scene* _scene;
    std::vector<GameObject*> _gameObjects;
};

