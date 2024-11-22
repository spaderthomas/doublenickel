void init_steam() {
	SteamErrMsg steam_error = { 0 };
	
	if (SteamAPI_InitEx(&steam_error) == k_ESteamAPIInitResult_OK) {
		engine.steam = true;
		tdns_log.write("%s: successfully initialized Steam", __func__);
	} else {
		engine.steam = false;
		tdns_log.write("Could not initialize Steam. Error: %s", steam_error);
	}
}

void update_steam() {
	if (engine.steam) {
		SteamAPI_RunCallbacks();

		// Because Steam's CCallResult is not POD and I can't stick it in any kind of list, I just poll for whether HTTP requests
		// are done every frame.
		for (auto it = steam.in_flight_requests.begin(); it != steam.in_flight_requests.end(); ) {
			auto& request = *it;

			bool failure = false;
			if (SteamUtils()->IsAPICallCompleted(request.api_call, &failure)) {
				SteamHTTP()->ReleaseHTTPRequest(request.handle);
				
				it = steam.in_flight_requests.erase(it);
			}
			else {
				it++;
			}
		}
	}
}

void shutdown_steam() {
	if (!engine.steam) return;

	bool http_done = steam.in_flight_requests.empty() && steam.awaiting_ticket_requests.empty();
	if (!http_done) {
		auto start_time = steady_clock::now();
		
		while (true) {
			update_steam();

			http_done = steam.in_flight_requests.empty() && steam.awaiting_ticket_requests.empty();
			bool timeout = duration_cast<milliseconds>(steady_clock::now() - start_time) > steam.http_shutdown_timeout;
			if (http_done || timeout) break;

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
	
	if (steam_input.steam) steam_input.steam->Shutdown();
	if (engine.steam) SteamAPI_Shutdown();
}

void SteamManager::OnDismissTextInput(GamepadTextInputDismissed_t* callback) {
	if (!callback->m_bSubmitted) return;

	unread_text_input = true;

	auto length = SteamUtils()->GetEnteredGamepadTextLength();
    bool success = SteamUtils()->GetEnteredGamepadTextInput(steam.text_input, length);
    if (!success) {
		tdns_log.write("%s: failed to get steam text input data", __func__);
		return;
	}
}

void hex_encode(char* destination, const char* data, size_t len) {
	char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	for (int i = 0; i < len; i++) {
		char const byte = data[i];

		int base_index = i * 2;
		destination[base_index] = hex_chars[(byte & 0xF0) >> 4];
		destination[base_index + 1]     = hex_chars[(byte & 0x0F) >> 0];
	}
}

void SteamManager::OnGetTicketForWebApiResponse(GetTicketForWebApiResponse_t* callback) {
	auth_ticket_wait = false;
	
	if (callback->m_eResult != k_EResultOK) {
		tdns_log.write("%s: auth ticket callback failed, result = %d", __func__, callback->m_eResult);
		return;
	}

	std::memcpy(auth_ticket, callback->m_rgubTicket, sizeof(char) * callback->m_cubTicket);
	hex_encode(auth_ticket_hex, auth_ticket, GetTicketForWebApiResponse_t::k_nCubTicketMaxLength);

	// Since this is all single threaded, every request we've collected is waiting on the API ticket. Now that
	// we have it, we can send all of them in one go.
	for (auto it = awaiting_ticket_requests.begin(); it != awaiting_ticket_requests.end(); ) {
		auto& request = *it;
		
		in_flight_requests.push_back(request);
		it = awaiting_ticket_requests.erase(it);
	}
}

	
void open_steam_page(const char* utm) {
	const char* url_plain = "https://store.steampowered.com/app/480";
	const char* url_utm = "https://store.steampowered.com/app/480?utm_campaign=%s";

	auto url = bump_allocator.alloc_path();
	if (utm) {
		snprintf(url, MAX_PATH_LEN, url_utm, utm);
	}
	else {
		snprintf(url, MAX_PATH_LEN, url_plain);
	}


	if (engine.steam) {
		SteamFriends()->ActivateGameOverlayToWebPage(url);
	}
	else {
		std::string command = "start ";
		command += url;
		system(command.c_str());
	}

}

void show_text_input(const char* description, const char* existing_text) {
	if (!engine.steam) return;
	
	auto line_mode = k_EFloatingGamepadTextInputModeModeSingleLine;
	SteamUtils()->ShowFloatingGamepadTextInput(line_mode, 100, 100, 600, 600);
}

const char* read_text_input() {
	steam.unread_text_input = false;
	return steam.text_input;
}

bool is_text_input_dirty() {
	return steam.unread_text_input;
}

bool is_steam_deck() {
	if (!engine.steam) return false;
	return SteamUtils()->IsSteamRunningOnSteamDeck();
}