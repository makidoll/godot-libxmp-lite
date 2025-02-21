#ifndef XMP_PLAYER_H
#define XMP_PLAYER_H

#include "godot_cpp/classes/audio_stream_generator.hpp"
#include "godot_cpp/classes/audio_stream_generator_playback.hpp"
#include "godot_cpp/classes/audio_stream_player.hpp"
#include "godot_cpp/classes/node.hpp"

#include <xmp.h>

using namespace godot;

class XmpPlayer : public Node {
    GDCLASS(XmpPlayer, Node);

private:
    String mod_path;
    NodePath player_path;

    xmp_context ctx;

    AudioStreamPlayer* player;
    AudioStreamGenerator* generator;
    AudioStreamGeneratorPlayback* playback;

    void cleanup();

public:
    XmpPlayer();
    ~XmpPlayer();

    String get_mod_path();
    void set_mod_path(String mod_path);

    NodePath get_player_path();
    void set_player_path(NodePath player_path);

    void _ready() override;
    void _process(double delta) override;
    void _exit_tree() override;

protected:
    static void _bind_methods();
};

#endif