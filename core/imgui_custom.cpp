#include "imgui_custom.h"
#include "options.h"

float CLAMPF(float f) {
	if (f >= 0.f && f <= 1.f) {
		return f;
	}
	else {
		if (f < 0.f) {
			return 0;
		}

		if (f > 1.f) {
			return 1.f;
		}
	}
}

int CLAMPB(int i) {
	if (i >= 0 && i <= 255) {
		return i;
	}
	else {
		if (i < 0) {
			return 0;
		}

		if (i > 255) {
			return 255;
		}
	}
}

ImVec4 COLOR2IMVEC(int ir, int ig, int ib, int ia) {
	Color c = Color(ir, ig, ib, ia);

	float fr = (float)c.GetR() / 255.f;
	float fg = (float)c.GetG() / 255.f;
	float fb = (float)c.GetB() / 255.f;
	float fa = (float)c.GetA() / 255.f;

	return ImVec4(fr, fg, fb, fa);
}

ImVec4 COLOR2IMVEC(Color c) {
	Color c2;
	c2.SetR(CLAMPB(c.GetR()));
	c2.SetG(CLAMPB(c.GetG()));
	c2.SetB(CLAMPB(c.GetB()));
	c2.SetA(CLAMPB(c.GetA()));

	float r = (float)c2.GetR() / 255.f;
	float g = (float)c2.GetG() / 255.f;
	float b = (float)c2.GetB() / 255.f;
	float a = (float)c2.GetA() / 255.f;

	return ImVec4(r, g, b, a);
}


Combo::Combo(int id, std::vector<std::string> items) {
	this->items = items;
	this->combo_id = std::string(_("##custom combo #") + std::to_string(id));
}

int Combo::Draw(std::string label, bool doLabel) {
	ImGuiStyle& style = ImGui::GetStyle();

	flags = ImGuiComboFlags_NoArrowButton;
	w = ImGui::CalcItemWidth();
	spacing = style.ItemInnerSpacing.x;
	button_sz = ImGui::GetFrameHeight();

	ImGui::PushStyleColor(ImGuiCol_FrameBg, o.vecMenuColorScheme);
	if (ImGui::BeginCombo(this->combo_id.c_str(), current_item.c_str(), ImGuiComboFlags_NoArrowButton))
	{
		for (int n = 0; n < this->items.size(); n++)
		{
			bool is_selected = (current_item == items[n]);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			if (ImGui::Selectable(this->items[n].c_str(), is_selected)) {
				current_item = this->items[n];
				_n = n;
			}
			ImGui::PopStyleVar();

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
	ImGui::PopStyleColor();

	if (doLabel) {
		ImGui::SameLine(0, style.ItemInnerSpacing.x);
		ImGui::Text(_("%s"), label.c_str());
	}
	return _n;
}

void CustomSliderInt(std::string s, int* mod, int min, int max, const char* format) {
	ImGui::PushStyleColor(ImGuiCol_FrameBg, o.vecMenuColorScheme);
	{
		ImGui::SliderInt(s.c_str(), mod, min, max, format);
	}
	ImGui::PopStyleColor();
}

void CustomTextEntry(std::string s, char* buf, size_t buf_size) {
	ImGui::PushStyleColor(ImGuiCol_FrameBg, o.vecMenuColorScheme);
	{
		ImGui::InputText(s.c_str(), buf, buf_size);
	}
	ImGui::PopStyleColor();
}