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

func _process(_delta: float) -> void:
	if playback == null:
		return
		
	var frames_available: int = playback.get_frames_available()
	var frames := xmp.get_frames(frames_available)
	playback.push_buffer(frames)
