#include "xmp_player.h"

#include <godot_cpp/classes/engine.hpp>

XmpPlayer::XmpPlayer()
{
}

XmpPlayer::~XmpPlayer()
{
}

String XmpPlayer::get_mod_path()
{
    return mod_path;
}

void XmpPlayer::set_mod_path(String mod_path)
{
    this->mod_path = mod_path;
}

NodePath XmpPlayer::get_player_path()
{
    return player_path;
}

void XmpPlayer::set_player_path(NodePath player_path)
{
    this->player_path = player_path;
    // TODO: if this changes, update generator?
}

void XmpPlayer::cleanup()
{
    if (ctx != nullptr) {
        xmp_end_player(ctx);
        xmp_release_module(ctx);
        xmp_free_context(ctx);
    }
}

void XmpPlayer::_ready()
{
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    const int sample_rate = 44100;

    cleanup();
    ctx = xmp_create_context();

    if (xmp_load_module(ctx, mod_path.utf8()) < 0) {
        print_error("Failed to load mod file");
    }

    if (xmp_start_player(ctx, sample_rate, 0) != 0) {
        print_error("Failed to start player");
    }

    struct xmp_module_info mi;
    xmp_get_module_info(ctx, &mi);
    // printf("%s (%s)\n", mi.mod->name, mi.mod->type);

    print_line(vformat("Loaded: %s (%s)", mi.mod->name, mi.mod->type));

    // setup generator

    player = get_node<AudioStreamPlayer>(player_path);
    if (player == nullptr) {
        print_error("No Audio Stream Player set");
        return;
    }

    generator = memnew(AudioStreamGenerator);
    generator->set_mix_rate(sample_rate);

    player->set_stream(generator);
    player->play();

    playback = (AudioStreamGeneratorPlayback*)player->get_stream_playback().ptr();
}

void XmpPlayer::_process(double delta)
{
    if (Engine::get_singleton()->is_editor_hint() || playback == nullptr) {
        return;
    }

    auto frames_available = playback->get_frames_available();

    // TODO: use XMP_MAX_FRAMESIZE. is this in bytes?
    int16_t frames[frames_available * 2];

    int frames_available_bytes = frames_available * 2 * sizeof(int16_t);

    if (xmp_play_buffer(ctx, &frames, frames_available_bytes, 0) < 0) {
        return;
    }

    PackedVector2Array gd_frames;
    gd_frames.resize(frames_available);

    for (int i = 0; i < frames_available; i++) {
        gd_frames[i] = Vector2(
            float(frames[i * 2]) / float(0x8000),
            float(frames[i * 2 + 1]) / float(0x8000));
    }

    playback->push_buffer(gd_frames);
}

void XmpPlayer::_exit_tree()
{
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    cleanup();
}

void XmpPlayer::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_mod_path"), &XmpPlayer::get_mod_path);
    ClassDB::bind_method(D_METHOD("set_mod_path", "mod_path"), &XmpPlayer::set_mod_path);
    ClassDB::add_property("XmpPlayer", PropertyInfo(Variant::STRING, "mod_path"), "set_mod_path", "get_mod_path");

    ClassDB::bind_method(D_METHOD("get_player_path"), &XmpPlayer::get_player_path);
    ClassDB::bind_method(D_METHOD("set_player_path", "player_path"), &XmpPlayer::set_player_path);
    ClassDB::add_property("XmpPlayer",
        PropertyInfo(Variant::NODE_PATH, "audio_stream_player", PROPERTY_HINT_NODE_TYPE, "AudioStreamPlayer"),
        "set_player_path", "get_player_path");
}
