#ifndef GD_XMP_H
#define GD_XMP_H

#include "godot_cpp/classes/ref_counted.hpp"
#include <xmp.h>

using namespace godot;

class GdXmp : public RefCounted {
    GDCLASS(GdXmp, RefCounted);

private:
    xmp_context ctx;

public:
    GdXmp();
    ~GdXmp();

    void init(int sample_rate, String mod_path);

    PackedVector2Array get_frames(int frames_available);

protected:
    static void _bind_methods();
};

#endif // AUDIO_STREAM_GENERATOR_H