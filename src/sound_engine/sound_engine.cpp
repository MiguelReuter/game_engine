#include "sound_engine.hpp"


using namespace std;

SoundEngine* SoundEngine::m_instance = nullptr;


SoundEngine* SoundEngine::Instance()
{
    if (!m_instance)
        m_instance = new SoundEngine();
    return m_instance;
}


SoundEngine::SoundEngine()
{
    //cout << "Sound engine created" << endl;
}
