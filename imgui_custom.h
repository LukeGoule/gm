#pragma once

#include "imgui/imgui.h"
#include "obfs.h"
#include "Color.h"

#include <string>
#include <vector>

class Combo {
public:
	std::string current_item = "";
	int _n = 0;
	std::string combo_id;
	ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton;

	float w;
	float spacing;
	float button_sz;

	std::vector<std::string> items;

	Combo(int id, std::vector<std::string> items);

	int Draw(std::string label, bool doLabel = true);
};

float		CLAMPF(float f);
int			CLAMPB(int i);
ImVec4		COLOR2IMVEC(int ir, int ig, int ib, int ia = 255);
ImVec4		COLOR2IMVEC(Color c);

void		CustomSliderInt(std::string s, int* mod, int min, int max, const char* format = _("%d"));
void		CustomTextEntry(std::string s, char* buf, size_t buf_size);