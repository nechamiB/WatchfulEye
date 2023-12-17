#ifdef _WIN32
#include "þþConfigurationCamera.h"
#else
#include "ConfigurationCamera.h"
#endif

cameraMembers ConfigurationCamera::getCameraMembers()
{
	std::ifstream configFile(FILE_CONFIG_PATH);
	if (configFile.is_open()) {
		json config;
		configFile >> config;
		cameraMembers cameraMember;
		cameraMember.cameraThreshold = config["camera_threshold"];
		cameraMember.cameraIP = config["camera_ip"];
		cameraMember.cameraPort = config["camera_port"];
		cameraMember.cameraDiffPixels = config["camera_diff_pixels"];
		cameraMember.windowsVideoPath = config["windows_video_path"];
		cameraMember.linuxVideoPath = config["windows_video_path"];

		configFile.close();
		return cameraMember;
	}
	else {
		Logging::writeTolog(spdlog::level::err, "Unable to open config file.");
		throw std::runtime_error("Unable to open config file.");
	}
}
