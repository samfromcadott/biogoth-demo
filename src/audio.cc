#include "audio.hh"

std::map<std::string, raylib::Sound> sound_list;

void load_sound(std::string name) {
	sound_list[name] = raylib::Sound("assets/audio/sfx/" + name + ".wav");
}

void play_sound(std::string name, float volume, float pitch) {
	sound_list[name].SetVolume(volume);
	sound_list[name].SetPitch(pitch);
	sound_list[name].Play();
}
