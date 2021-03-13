#include "radiomanager.h"
#include "utils.h"
#include "obfs.h"

RadioManager::RadioManager() {
	RadioStations = {
		new RadioStation(_("Off"),			_(" ")),															// off
		new RadioStation(_("Anime"),		_("http://144.217.203.184:8972/listen.pls?sid=1&t=.m3u")),			// "anime"
		new RadioStation(_("Hardcore"),		_("http://listen.technobase.fm/dsl.pls")),							// "techno"
		new RadioStation(_("Big Time"),		_("http://streams.bigfm.de/bigfm-deutschrap-128-aac")),			// "big time"
		new RadioStation(_("Techno"),		_("http://mp3.stream.tb-group.fm/tt.mp3")),						// "techno"
		new RadioStation(_("Club"),			_("http://mp3.stream.tb-group.fm/clt.mp3")),						// "club"
		new RadioStation(_("House"),		_("http://mp3.stream.tb-group.fm/ht.mp3")),						// "house"
		new RadioStation(_("Pop"),			_("http://www.iloveradio.de/iloveradio.m3u")),						// "radio"
		new RadioStation(_("Rock"),			_("http://www.rockantenne.de/webradio/channels/alternative.m3u")),	// "rock
	};
}

RadioManager::~RadioManager() {

}

// this will only return true if a station with the specified name doesn't already exist.
bool RadioManager::AddStation(RadioStation* newStation) {
	if (!GetStationByName(newStation->name))
		this->RadioStations.push_back(newStation);
	else
		return false;

	return true;
}

std::vector<std::string> RadioManager::GetNames() {
	std::vector<std::string> out;
	for (auto it : this->RadioStations) {
		out.push_back(it->name);
	}
	return out;
}

std::vector<std::string> RadioManager::GetUrls() {
	std::vector<std::string> out;
	for (auto it : this->RadioStations) {
		out.push_back(it->url);
	}
	return out;
}

RadioStation* RadioManager::GetStationByName(std::string name) {
	for (auto it : this->RadioStations) {
		if (it->name == name) {
			return it;
		}
	}

	return nullptr;
}

RadioStation* RadioManager::GetStationByURL(std::string url) {
	for (auto it : this->RadioStations) {
		if (it->url == url) {
			return it;
		}
	}

	return nullptr;
}

RadioStation* RadioManager::GetCurrentlyPlaying() {
	auto ret = this->RadioStations[this->m_iRadioChannel];
	if (ret) {
		return ret;
	} else {
		Utils::ConsolePrint(_("[RadioManager] Error! Currently playing a bad station?\n"));
		return nullptr;
	}
}