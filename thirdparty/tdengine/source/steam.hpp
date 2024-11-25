struct SteamAnalytics {
	Steam::ApiCall api_call;
	Steam::HttpRequest handle;
	char* key;
	char* data;
};

struct SteamManager {
	static constexpr int32 text_input_size = 64;
	char text_input [text_input_size + 1] = {0};
	bool unread_text_input = false;
	

	static constexpr int auth_ticket_len = GetTicketForWebApiResponse_t::k_nCubTicketMaxLength;
	static constexpr int auth_ticket_hex_len = GetTicketForWebApiResponse_t::k_nCubTicketMaxLength * 2 + 1;

	char auth_ticket     [auth_ticket_len] = { 0 };
	char auth_ticket_hex [auth_ticket_hex_len] = { 0 };
	bool auth_ticket_wait = false;
	milliseconds http_shutdown_timeout = milliseconds(2000);
	std::vector<SteamAnalytics> awaiting_ticket_requests;
	std::vector<SteamAnalytics> in_flight_requests;

	STEAM_CALLBACK(SteamManager, OnDismissTextInput, GamepadTextInputDismissed_t);
	STEAM_CALLBACK(SteamManager, OnGetTicketForWebApiResponse, GetTicketForWebApiResponse_t);
};
SteamManager steam;

void init_steam();
void update_steam();
void shutdown_steam();

DN_API void open_steam_page(const char* utm);
DN_API void show_text_input(const char* description, const char* existing_text);
DN_API bool is_text_input_dirty();
DN_API const char* read_text_input();
DN_API bool is_steam_deck();