extends Node

@export var player: AudioStreamPlayer;

var stream := AudioStreamGenerator.new();
var playback: AudioStreamPlayback;

var xmp := GdXmp.new()

func _ready() -> void:
	var sample_rate := 44100

	xmp.init(sample_rate, "/home/maki/git/godot-libxmp-lite/demo/mods/jt_1999.xm")
	stream.mix_rate = sample_rate;

	player.stream = stream;
	player.play()

	playback = player.get_stream_playback()

var time_since_last_frame: int = 0;

func _process(_delta: float) -> void:
	var now := Time.get_ticks_msec()
	if now - time_since_last_frame < 20:
		return

	time_since_last_frame = now

	var frame := xmp.frame()

	for i in range(len(frame) * 0.5):
		playback.push_frame(Vector2(
			frame[i * 2 + 0], frame[i * 2 + 1]
		))
