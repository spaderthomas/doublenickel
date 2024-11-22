void init_assets() {
	rb_init(&asset_loader.load_requests, 2048);
	rb_init(&asset_loader.completion_queue, 2048);
	
	asset_loader.thread = std::thread(&AssetLoader::process_requests, &asset_loader);
	asset_loader.thread.detach();
}

void update_assets() {
	asset_loader.process_completion_queue();

	arr_for(backgrounds, background) {
		if (background->gpu_ready && !background->gpu_done) {
			background->load_one_to_gpu();

			if (background->gpu_done) {
				background->deinit();
			}

			if (exceeded_frame_time()) break;
		}
	}
}

void AssetLoader::process_requests() {
	while (true) {
		std::unique_lock lock(mutex);

		condition.wait(lock, [this] {
			return load_requests.size > 0;
			});

		auto request = rb_pop(&load_requests);
		lock.unlock();

		if (request.kind == AssetKind::TextureAtlas) {
			auto atlas = request.atlas;
			if (atlas->need_async_build) {
				atlas->build_from_source();
			}
			if (atlas->need_async_load) {
				atlas->load_to_memory();
			}

			lock.lock();
			rb_push(&completion_queue, request);
			lock.unlock();

		}
		else if (request.kind == AssetKind::Background) {
			auto background = request.background;
			if (background->need_async_build) {
				background->build_from_source();
			}
			if (background->need_async_load) {
				background->load_tiles();
			}

			lock.lock();
			rb_push(&completion_queue, request);
			lock.unlock();
		}
	}
}

void AssetLoader::process_completion_queue() {
	int num_assets_loaded = 0;
	auto begin = glfwGetTime();
	while (true) {
		std::unique_lock lock(mutex);
		if (!completion_queue.size) {
			break;
		}

		auto completion = rb_pop(&completion_queue);
		lock.unlock();

		num_assets_loaded++;

		if (completion.kind == AssetKind::Background) {
			auto background = completion.background;
			tdns_log.write(Log_Flags::File, "%s: AssetKind = Background, AssetName = %s", __func__, background->name);
			
			if (background->is_dirty()) {
				background->update_config();
			}

			background->gpu_ready = true;
		}
		else if (completion.kind == AssetKind::TextureAtlas) {
			auto atlas = completion.atlas;
			tdns_log.write(Log_Flags::File, "%s: atlas, %s", __func__, atlas->name);
			
			if (atlas->need_gl_init) {
				atlas->load_to_gpu();
				atlas->need_gl_init = false;
			}

			if (atlas->need_config_update) {
				atlas->write_to_config();
				atlas->need_config_update = false;
			}
			
		}

		if (exceeded_frame_time()) break;
	}

	if (num_assets_loaded) {
		std::unique_lock lock(mutex);
		auto now = glfwGetTime();
		tdns_log.write(Log_Flags::File,
					   "AssetLoader: frame = %d, assets_loaded =  %d, assets_remaining = %d, time_ms =  %f",
					   engine.frame,
					   num_assets_loaded, completion_queue.size,
					   (now - begin) * 1000);
	}
}


void AssetLoader::submit(AssetLoadRequest request) {
	request.id = AssetLoadRequest::next_id++;

	mutex.lock();
	rb_push(&load_requests, request);
	mutex.unlock();

	condition.notify_all();
}
