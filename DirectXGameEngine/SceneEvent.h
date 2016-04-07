#pragma once

#include "Event.h"
#include "Scene.h"

class SceneEvent :
    public Event
{
public:
    static const std::string& CHANGE;

    SceneEvent( const std::string& name );
    ~SceneEvent();
};

