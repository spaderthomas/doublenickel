#define SOUND_INFO_SIZE 1024
dn_array_t<dn_audio_info_t> sound_infos;
#define ACTIVE_SOUND_SIZE 64
dn_array_t<dn_audio_instance_t> active_sounds;
#define PARTICLE_SYSTEMS_SIZE 64
dn_array_t<ParticleSystem> particle_systems;


void init_buffers() { 
	dn_log_flags(DN_LOG_FLAG_FILE, "initializing buffers");
	
	dn_array_init(&sound_infos,       SOUND_INFO_SIZE);
	dn_array_init(&active_sounds,     ACTIVE_SOUND_SIZE);
	dn_array_init(&particle_systems,  PARTICLE_SYSTEMS_SIZE);
}

