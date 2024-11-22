// Infos: chunks of contiguous memory that are initialized and then static
#define FONT_INFO_SIZE 64
Array<FontInfo> font_infos;
#define GLYPH_INFO_SIZE FONT_INFO_SIZE * 128
Array<GlyphInfo> glyph_infos;
#define VX_INFO_SIZE GLYPH_INFO_SIZE * 6
Array<Vector2>   text_vx_data;
#define TEXT_UV_DATA_SIZE GLYPH_INFO_SIZE * 6
Array<Vector2>   text_uv_data;
#define TC_INFO_SIZE VX_INFO_SIZE
Array<Vector2>   tc_data;
#define IMAGE_INFO_SIZE 256
Array<Texture> image_infos;
#define SPRITE_INFO_SIZE 1024
Array<Sprite> sprite_infos;
#define ATLAS_INFO_SIZE 64
Array<TextureAtlas> atlas_infos;
#define SOUND_INFO_SIZE 1024
Array<SoundInfo> sound_infos;
#define ACTIVE_SOUND_SIZE 64
Array<ActiveSound> active_sounds;
#define PARTICLE_SYSTEMS_SIZE 64
Array<ParticleSystem> particle_systems;


void init_buffers() { 
	tdns_log.write(Log_Flags::File, "initializing buffers");
	
	arr_init(&font_infos,        FONT_INFO_SIZE);
	arr_init(&glyph_infos,       GLYPH_INFO_SIZE);
	arr_init(&text_vx_data,      VX_INFO_SIZE);
	arr_init(&text_uv_data,      TEXT_UV_DATA_SIZE);
	arr_init(&image_infos,       IMAGE_INFO_SIZE);
	arr_init(&tc_data,           TC_INFO_SIZE);
	arr_init(&sprite_infos,      SPRITE_INFO_SIZE);
	arr_init(&atlas_infos,       ATLAS_INFO_SIZE);
	arr_init(&sound_infos,       SOUND_INFO_SIZE);
	arr_init(&active_sounds,     ACTIVE_SOUND_SIZE);
	arr_init(&particle_systems,  PARTICLE_SYSTEMS_SIZE);
}

