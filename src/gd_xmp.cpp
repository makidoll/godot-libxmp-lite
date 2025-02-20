#include "gd_xmp.h"

GdXmp::GdXmp()
{
    ctx = xmp_create_context();
}

GdXmp::~GdXmp()
{
    xmp_end_player(ctx);
    xmp_release_module(ctx);
    xmp_free_context(ctx);
}

void GdXmp::init(int sample_rate, String mod_path)
{
    if (xmp_load_module(ctx, mod_path.utf8()) < 0) {
        print_error("Failed to load mod file");
    }

    if (xmp_start_player(ctx, sample_rate, 0) != 0) {
        print_error("Failed to start player");
    }

    struct xmp_module_info mi;
    xmp_get_module_info(ctx, &mi);
    // printf("%s (%s)\n", mi.mod->name, mi.mod->type);

    print_line(mi.mod->name);
    print_line(mi.mod->type);
}

PackedVector2Array GdXmp::get_frames(int frames_available)
{
    // TODO: use XMP_MAX_FRAMESIZE. is this in bytes?
    int16_t frames[frames_available * 2];

    int frames_available_bytes = frames_available * 2 * sizeof(int16_t);

    if (xmp_play_buffer(ctx, &frames, frames_available_bytes, 0) < 0) {
        return PackedVector2Array();
    }

    PackedVector2Array gd_frames;
    gd_frames.resize(frames_available);

    for (int i = 0; i < frames_available; i++) {
        gd_frames[i] = Vector2(
            float(frames[i * 2]) / float(0x8000),
            float(frames[i * 2 + 1]) / float(0x8000));
    }

    return gd_frames;
}

void GdXmp::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("init"), &GdXmp::init);
    ClassDB::bind_method(D_METHOD("get_frames"), &GdXmp::get_frames);
}
