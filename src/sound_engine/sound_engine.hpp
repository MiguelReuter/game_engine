#ifndef sound_hpp
#define sound_hpp

#include <iostream>
#include <stdio.h>

class SoundEngine
{
public:
    static SoundEngine* Instance();
    
private:
    SoundEngine();
    SoundEngine(SoundEngine const&);
    SoundEngine& operator=(SoundEngine const&);
    static SoundEngine* m_instance;
};

#endif /* sound_hpp */
