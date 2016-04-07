#include "SceneEvent.h"

const std::string& SceneEvent::CHANGE = "change";

SceneEvent::SceneEvent( const std::string& name ) :
    Event( name )
{
}

SceneEvent::~SceneEvent()
{
}