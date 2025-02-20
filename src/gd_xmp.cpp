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

PackedFloat32Array GdXmp::frame()
{
    if (xmp_play_frame(ctx) != 0) {
        return PackedFloat32Array();
    }

    xmp_frame_info fi;

    xmp_get_frame_info(ctx, &fi);
    // if (fi.loop_count > 0) {
    //     return PackedFloat32Array();
    // }

    PackedFloat32Array frame;
    frame.resize(fi.buffer_size);

    int16_t sample;
    for (int i = 0; i < fi.buffer_size; i++) {
        sample = ((int16_t*)fi.buffer)[i];
        frame[i] = float(sample) / float(0x8000);
    }

    return frame;
}

void GdXmp::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("init"), &GdXmp::init);
    ClassDB::bind_method(D_METHOD("frame"), &GdXmp::frame);
}
