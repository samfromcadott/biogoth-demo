#include "audio.hh"

std::map<std::string, raylib::Sound> sound_list;
raylib::Music music;

void load_sound(std::string name) {
	sound_list[name] = raylib::Sound("assets/audio/sfx/" + name + ".wav");
}

void play_sound(std::string name, float volume, float pitch) {
	if ( !sound_list.count(name) ) return; // Exit if there is no sound with that name

	sound_list[name].SetVolume(volume);
	sound_list[name].SetPitch(pitch);
	sound_list[name].Play();
}

void play_music() {
	music.Update();
}

void stop_music() {
	music.Stop();
}

void set_music(const std::string filename) {
	music = raylib::Music(filename);
	music.Play();
	music.SetLooping(true);
}
