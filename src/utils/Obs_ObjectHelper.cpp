/*
obs-websocket
Copyright (C) 2016-2021 Stephane Lepin <stephane.lepin@gmail.com>
Copyright (C) 2020-2021 Kyle Manning <tt2468@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include <util/config-file.h>

#include "Obs.h"
#include "../obs-websocket.h"
#include "plugin-macros.generated.h"

json Utils::Obs::ObjectHelper::GetStats()
{
	json ret;

	std::string outputPath = Utils::Obs::StringHelper::GetCurrentRecordOutputPath();

	/* Rescaled encoders (Rescale Output) render from mixes of their own,
	 * which obs_get_video() doesn't include. */
	uint32_t outputTotalFrames = 0;
	uint32_t outputSkippedFrames = 0;
	obs_get_video_frame_counts(&outputTotalFrames, &outputSkippedFrames);

	ret["memoryUsage"] = (double)os_get_proc_resident_size() / (1024.0 * 1024.0);
	ret["availableDiskSpace"] = (double)os_get_free_disk_space(outputPath.c_str()) / (1024.0 * 1024.0);
	ret["activeFps"] = obs_get_active_fps();
	ret["averageFrameRenderTime"] = (double)obs_get_average_frame_time_ns() / 1000000.0;
	ret["renderSkippedFrames"] = obs_get_lagged_frames();
	ret["renderTotalFrames"] = obs_get_total_frames();
	ret["outputSkippedFrames"] = outputSkippedFrames;
	ret["outputTotalFrames"] = outputTotalFrames;

	/* GPU acceleration status from the frontend (obs-rust/obs-studio#846):
	 * null when the frontend does not provide it. */
	ret["gpuAcceleration"] = nullptr;
	calldata_t cd = {0};
	if (proc_handler_call(obs_get_proc_handler(), "get_gpu_status", &cd)) {
		const char *status = calldata_string(&cd, "json");
		if (status) {
			json parsed = json::parse(status, nullptr, false);
			if (!parsed.is_discarded())
				ret["gpuAcceleration"] = parsed;
		}
	}
	calldata_free(&cd);

	return ret;
}

json Utils::Obs::ObjectHelper::GetSceneItemTransform(obs_sceneitem_t *item)
{
	json ret;

	obs_transform_info osi;
	obs_sceneitem_crop crop;
	obs_sceneitem_get_info2(item, &osi);
	obs_sceneitem_get_crop(item, &crop);

	OBSSource source = obs_sceneitem_get_source(item);
	float sourceWidth = (float)obs_source_get_width(source);
	float sourceHeight = (float)obs_source_get_height(source);

	ret["sourceWidth"] = sourceWidth;
	ret["sourceHeight"] = sourceHeight;

	ret["positionX"] = osi.pos.x;
	ret["positionY"] = osi.pos.y;

	ret["rotation"] = osi.rot;

	ret["scaleX"] = osi.scale.x;
	ret["scaleY"] = osi.scale.y;

	ret["width"] = osi.scale.x * sourceWidth;
	ret["height"] = osi.scale.y * sourceHeight;

	ret["alignment"] = osi.alignment;

	ret["boundsType"] = osi.bounds_type;
	ret["boundsAlignment"] = osi.bounds_alignment;
	ret["boundsWidth"] = osi.bounds.x;
	ret["boundsHeight"] = osi.bounds.y;

	ret["cropLeft"] = (int)crop.left;
	ret["cropRight"] = (int)crop.right;
	ret["cropTop"] = (int)crop.top;
	ret["cropBottom"] = (int)crop.bottom;

	ret["cropToBounds"] = osi.crop_to_bounds;

	return ret;
}

json Utils::Obs::ObjectHelper::GetCanvasVideoSettings(obs_canvas_t *canvas)
{
	json ret;

	struct obs_video_info ovi;
	if (canvas && obs_canvas_get_video_info(canvas, &ovi)) {
		ret["fpsNumerator"] = ovi.fps_num;
		ret["fpsDenominator"] = ovi.fps_den;
		ret["baseWidth"] = ovi.base_width;
		ret["baseHeight"] = ovi.base_height;
		ret["outputWidth"] = ovi.output_width;
		ret["outputHeight"] = ovi.output_height;
	} else {
		ret["fpsNumerator"] = nullptr;
		ret["fpsDenominator"] = nullptr;
		ret["baseWidth"] = nullptr;
		ret["baseHeight"] = nullptr;
		ret["outputWidth"] = nullptr;
		ret["outputHeight"] = nullptr;
	}

	return ret;
}
