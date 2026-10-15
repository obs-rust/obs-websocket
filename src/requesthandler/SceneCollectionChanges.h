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

#pragma once

#include <string>
#include <unordered_set>

/* Requests that change data the frontend stores in the scene collection
 * (scenes, inputs, filters, scene items, transitions). After one of them
 * succeeds, the request handler asks the frontend to save, as the OBS UI does
 * after the same change. Without this, the change is only in memory until a
 * clean exit, and a crash or a hard stop loses it.
 *
 * Scene switches (SetCurrentProgramScene, SetCurrentPreviewScene,
 * TriggerStudioModeTransition) are not in the list: the UI does not save on a
 * scene switch either. Profile, global config and output requests store their
 * data through their own code. */
inline bool RequestChangesSceneCollection(const std::string &requestType)
{
	static const std::unordered_set<std::string> requests = {
		// Sources
		"SetSourcePrivateSettings",
		// Scenes
		"CreateScene",
		"RemoveScene",
		"SetSceneName",
		"SetSceneSceneTransitionOverride",
		// Inputs
		"CreateInput",
		"RemoveInput",
		"SetInputName",
		"SetInputSettings",
		"SetInputMute",
		"ToggleInputMute",
		"SetInputVolume",
		"SetInputAudioBalance",
		"SetInputAudioSyncOffset",
		"SetInputAudioMonitorType",
		"SetInputAudioTracks",
		"SetInputDeinterlaceMode",
		"SetInputDeinterlaceFieldOrder",
		// Transitions
		"SetCurrentSceneTransition",
		"SetCurrentSceneTransitionDuration",
		"SetCurrentSceneTransitionSettings",
		// Filters
		"CreateSourceFilter",
		"RemoveSourceFilter",
		"SetSourceFilterName",
		"SetSourceFilterIndex",
		"SetSourceFilterSettings",
		"SetSourceFilterEnabled",
		// Scene items
		"CreateSceneItem",
		"RemoveSceneItem",
		"DuplicateSceneItem",
		"SetSceneItemTransform",
		"SetSceneItemEnabled",
		"SetSceneItemLocked",
		"SetSceneItemIndex",
		"SetSceneItemBlendMode",
		"SetSceneItemPrivateSettings",
	};

	return requests.count(requestType) > 0;
}
