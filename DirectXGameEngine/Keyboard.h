#pragma once

#include <Windows.h>

class Keyboard
{
public:
    // Singleton pattern
    static Keyboard& getInstance()
    {
        static Keyboard instance;
        return instance;
    }

    bool isDown( char key )
    {
        return ( GetAsyncKeyState( key ) & 0x8000 ) != 0;
    }

private:
    Keyboard() {}
    Keyboard( Keyboard const& ) = delete;
    void operator=( Keyboard const& ) = delete;
};