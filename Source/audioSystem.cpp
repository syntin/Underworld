#include "audioSystem.h"
#include "componentManager.h"
#include <iostream> // temporary for debugging/demonstrating

void AudioSystem::Update(ComponentManager& components)
{
	auto& entities = components.GetAudioEntities();

	for (auto& e : entities)
	{
		Audio* a = components.GetAudio(e);
		if (!a) continue;

		if (a->play)
		{
			// this is what the future line will probably look like
			//audioBacken.Play(a->soundName, a->volume, a->pitch, a->loop); 

			// placeholder print, when we have audio playback stuff it goes here
			std::cout << "Playing sound: " << a->soundName
					  << " volume= " << a->volume
					  << " pitch= " << a->pitch << std::endl;

			a->play = false;
		}

		a->dirty = false;
	}
}