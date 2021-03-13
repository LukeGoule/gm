#pragma once

#include <vector>
#include <string>

class RadioStation {
public:

	std::string name, url;

	inline RadioStation(std::string name, std::string url) {
		this->name = name;
		this->url = url;
	}

};

class RadioManager {
public:
	std::vector<RadioStation*> RadioStations;

	int m_iRadioChannel = 0;
	int m_iRadioVolumePercent = 20;

	RadioManager();
	~RadioManager();

	// this will only return true if a station with the specified name doesn't already exist.
	bool AddStation(RadioStation* newStation);
	bool RemoveStation(RadioStation* remStation);

	std::vector<std::string> GetNames();
	std::vector<std::string> GetUrls();

	RadioStation* GetStationByName(std::string name);
	RadioStation* GetStationByURL(std::string url);
	RadioStation* GetCurrentlyPlaying();
};

extern RadioManager* g_pRadio;