#pragma once
#include "../../util.hpp"
#include <map>

inline ImFont* TabsFont;
inline ImFont* VFont;
inline ImFont* proggy_clean_game;

inline static int keystatus = 0;
inline static int realkey = 0;
inline int aimkey = 2;
inline bool GetKey(int key)
{
	realkey = key;
	return true;
}
inline void ChangeKey(void* blank)
{
	keystatus = 1;
	while (true)
	{
		for (int i = 0; i < 0x87; i++)
		{
			if (GetKeyState(i) & 0x8000)
			{
				aimkey = i;
				keystatus = 0;
				return;
			}
		}
	}
}


inline void DrawBox(float X, float Y, float W, float H, const ImU32& color, int thickness)
{
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
}

static const char* keyNames[] =
{
	"Keybind",
	"Left Mouse",
	"Right Mouse",
	"Cancel",
	"Middle Mouse",
	"Mouse 5",
	"Mouse 4",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"Clear",
	"Enter",
	"",
	"",
	"Shift",
	"Control",
	"Alt",
	"Pause",
	"Caps",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
};

inline static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char* const* items = (const char* const*)data;
	if (out_text)
		*out_text = items[idx];
	return true;
}
inline void HotkeyButton(int aimkey, void* changekey, int status)
{
	const char* preview_value = NULL;
	if (aimkey >= 0 && aimkey < IM_ARRAYSIZE(keyNames))
		Items_ArrayGetter(keyNames, aimkey, &preview_value);
	std::string aimkeys;
	if (preview_value == NULL)
		aimkeys = ("Select Key");
	else
		aimkeys = preview_value;

	if (status == 1)
	{

		aimkeys = ("Press the Key");
	}
	if (ImGui::Button(aimkeys.c_str(), ImVec2(125, 20)))
	{
		if (status == 0)
		{
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
		}
	}
}

inline ImFont* LexendRegular;
inline ImFont* LexendLight;

template<class T, class U>
inline static T clamp(const T& in, const U& low, const U& high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}

inline bool ImGui::Renderingtab(const char* icon, bool selected)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(icon);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size = { 35, 35 };

	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));

	ImGui::ItemSize(size, 0);

	if (!ImGui::ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, NULL);

	int current_tab;

	if (hovered || held)
		ImGui::SetMouseCursor(9);

	float deltatime = 1.5f * ImGui::GetIO().DeltaTime;
	static std::map<ImGuiID, float> hover_animation;
	auto it_hover = hover_animation.find(id)
		;
	if (it_hover == hover_animation.end())
	{
		hover_animation.insert({ id, 0.f });
		it_hover = hover_animation.find(id);
	}

	it_hover->second = clamp(it_hover->second + (1.15f * ImGui::GetIO().DeltaTime * (hovered ? 1.f : -1.f)), 0.0f, 0.0f);
	it_hover->second *= ImGui::GetStyle().Alpha;

	static std::map<ImGuiID, float> filled_animation;
	auto it_filled = filled_animation.find(id);

	if (it_filled == filled_animation.end())
	{
		filled_animation.insert({ id, 0.f });
		it_filled = filled_animation.find(id);
	}

	it_filled->second = clamp(it_filled->second + (2.15f * ImGui::GetIO().DeltaTime * (selected ? 1.f : -1.5f)), it_hover->second, 1.f);
	it_filled->second *= ImGui::GetStyle().Alpha;

	ImGui::GetWindowDrawList()->AddText(ImVec2(bb.Min.x + 20, bb.Min.y + 4), ImColor(64, 64, 64, int(255 * ImGui::GetStyle().Alpha)), icon);

	if (selected)
	{
		ImGui::GetWindowDrawList()->AddText(ImVec2(bb.Min.x + 20, bb.Min.y + 4), ImColor(183, 183, 183, int(255 * it_filled->second)), icon);
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(bb.Min.x - 11, bb.Min.y + 8.75f), ImVec2(bb.Min.x - 7 * it_filled->second, bb.Max.y - 8.75f), ImColor(201, 135, 52, int(255 * it_filled->second)), 10.f, ImDrawCornerFlags_Right);
	}

	return pressed;
}

inline bool ImGui::Renderingsubtab(const char* icon, bool selected)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(std::string(icon + std::string(icon)).c_str());
	const ImVec2 label_size = ImGui::CalcTextSize(icon);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size = { 35, 35 };

	const ImRect bb(pos, ImVec2(pos.x + 35, pos.y + 35));
	ImGui::ItemSize(size, 50);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, NULL);

	if (hovered || held);

	float t = selected ? 1.0f : 0.0f;
	float deltatime = 1.5f * ImGui::GetIO().DeltaTime;
	static std::map<ImGuiID, float> hover_animation;
	auto it_hover = hover_animation.find(id);
	if (it_hover == hover_animation.end())
	{
		hover_animation.insert({ id, 0.f });
		it_hover = hover_animation.find(id);
	}
	it_hover->second = ImClamp(it_hover->second + (0.2f * ImGui::GetIO().DeltaTime * (hovered || ImGui::IsItemActive() ? 1.f : -1.f)), 0.0f, 0.15f);
	it_hover->second *= min(ImGui::GetStyle().Alpha * 1.2, 1.f);

	static std::map<ImGuiID, float> filled_animation;
	auto it_filled = filled_animation.find(id);
	if (it_filled == filled_animation.end())
	{
		filled_animation.insert({ id, 0.f });
		it_filled = filled_animation.find(id);
	}
	it_filled->second = ImClamp(it_filled->second + (2.55f * ImGui::GetIO().DeltaTime * (selected ? 1.f : -1.0f)), it_hover->second, 1.f);
	it_filled->second *= min(ImGui::GetStyle().Alpha * 1.2, 1.f);

	static std::map<ImGuiID, float> fill_animation;
	auto it_fill = fill_animation.find(id);
	if (it_fill == fill_animation.end())
	{
		fill_animation.insert({ id, 0.f });
		it_fill = fill_animation.find(id);
	}
	it_fill->second = ImClamp(it_filled->second + (1.75f * ImGui::GetIO().DeltaTime * (selected ? 1.f : -1.0f)), it_hover->second, 1.f);
	it_fill->second *= min(ImGui::GetStyle().Alpha * 1.2, 1.f);

	ImVec4 text = ImLerp(ImVec4{ 128 / 255.f, 128 / 255.f, 128 / 255.f, ImGui::GetStyle().Alpha }, ImVec4{ 255 / 255.f, 255 / 255.f, 255 / 255.f, ImGui::GetStyle().Alpha }, it_filled->second);
	ImVec4 text2 = ImLerp(ImVec4{ 128 / 255.f, 128 / 255.f, 128 / 255.f, ImGui::GetStyle().Alpha }, ImVec4{ 255 / 255.f, 255 / 255.f, 255 / 255.f, ImGui::GetStyle().Alpha }, it_filled->second);

	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(bb.Min.x + 12, bb.Max.y - it_fill->second * 5), ImVec2(bb.Min.x + 35, bb.Max.y - 1), ImColor(201, 135, 52, int(255 * it_filled->second)), 3, ImDrawCornerFlags_Top);

	ImGui::GetForegroundDrawList()->AddText(ImVec2(bb.Min.x, bb.Min.y + 8), ImColor(text), icon);

	return pressed;
}

inline int MenuTab;
inline static float switch_alpha[3], open_alpha = 0;
inline void DrawMenu() {
	RECT screen_rect;
	::GetWindowRect(::GetDesktopWindow(), &screen_rect);

	ImGui::PushFont(VFont);
	static ImVec4 inactive = ImColor(0, 136, 255, 255);
	static ImVec4 symbol = ImColor(0.150f, 0.361f, 0.696f, 1.000f);

	static int tabs = 0;

	static int selected = 0;
	static int sub_selected = 0;

	open_alpha = ImClamp(open_alpha + (2.f * ImGui::GetIO().DeltaTime * (menu_open ? 1.5f : -1.5f)), 0.f, 1.f);
	if (open_alpha > 0.01f)
	{
		if (menu_open && open_alpha > 0.8f || !menu_open)
			switch_alpha[0] = ImClamp(switch_alpha[0] + (2.f * ImGui::GetIO().DeltaTime * (menu_open ? 1.f : -5.f)), 0.f, 1.f);
		ImGui::SetNextWindowSize({ 565, 395 });
		ImGui::Begin(("Infinity.cc"), nullptr, ImGuiWindowFlags_NoDecoration);
		auto draw = ImGui::GetWindowDrawList();
		ImVec2 pos = ImGui::GetWindowPos();
		ImVec2 size = ImGui::GetWindowSize();

		draw->AddRectFilled(pos, ImVec2(pos.x + 100, pos.y + size.y), ImColor(20, 20, 20, 255));
		draw->AddRectFilled(ImVec2(pos.x + 100, pos.y), ImVec2(pos.x + size.x, pos.y + 35), ImColor(20, 20, 20, 255));

		draw->AddLine(ImVec2(pos.x + 0, pos.y + 35), ImVec2(pos.x + size.x, pos.y + 35), ImColor(255, 255, 255, 15));
		draw->AddLine(ImVec2(pos.x + 100, pos.y), ImVec2(pos.x + 100, pos.y + size.y), ImColor(255, 255, 255, 15));

		// Load Fonts
		ImFontConfig font_config;
		font_config.PixelSnapH = false;
		font_config.OversampleH = 5;
		font_config.OversampleV = 5;
		font_config.RasterizerMultiply = 1.2f;

		static const ImWchar ranges[] =
		{
			0x0020, 0x00FF, // Basic Latin + Latin Supplement
			0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
			0x2DE0, 0x2DFF, // Cyrillic Extended-A
			0xA640, 0xA69F, // Cyrillic Extended-B
			0xE000, 0xE226, // icons
			0,
		};

		font_config.GlyphRanges = ranges;
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\verdana.ttf"), 14.0f, &font_config, ranges);

		ImGui::SetCursorPos(ImVec2(30, 13));
		ImGui::Text(("Infinity"));
		
		ImGui::SetCursorPos(ImVec2(15, 40));
		ImGui::BeginGroup();	
		ImGui::PushFont(TabsFont);
		{
			if (ImGui::Renderingtab(("Aimbot"), 0 == selected)) {
				selected = 0; sub_selected = 0;
			}
			if (ImGui::Renderingtab(("Visual"), 1 == selected)) {
				selected = 1; sub_selected = 2;
			}
			if (ImGui::Renderingtab(("Misc"), 2 == selected)) {
				selected = 2; sub_selected = 5;
			}
		}
		ImGui::PopFont();
		ImGui::EndGroup();

		if (selected == 0) {
			ImGui::BeginGroup();
			ImGui::SetCursorPos(ImVec2(110, 1));
			if (ImGui::Renderingsubtab((" Aimbot"), 0 == sub_selected)) {
				sub_selected = 0;
			}
			ImGui::EndGroup();


			if (sub_selected == 0) {
				ImGui::SetCursorPos(ImVec2(110, 44));
				ImGui::BeginChild(("Aim"), ImVec2(219, 342));
				{
					ImGui::Spacing();
					ImGui::Checkbox(("Aimbot"), &aimbot);
					ImGui::Checkbox(("Triggerbot (Shotungs only)"), &triggerbot);
					ImGui::Checkbox(("Draw FOV Cirlce"), &draw_fov);
					ImGui::Checkbox(("Draw Targetline"), &targetline);
					ImGui::SliderInt("Max Distance", &max_aimbot_distance, 1, 800);
				}
				ImGui::EndChild();
				ImGui::SetCursorPos(ImVec2(291 + 45, 44));
				ImGui::BeginChild(("Settings"), ImVec2(219, 342));
				{
					ImGui::Spacing();
					ImGui::Text((("Aim Key ->    "))); ImGui::SameLine();
					HotkeyButton(aimkey, ChangeKey, keystatus);
					if (!weaponconfigs)
					{
						ImGui::PushItemWidth(150);
						static const char* bonename[] =
						{
							"Head",
							"Neck",
							"Chest"
						};
						ImGui::Combo(("Target Bone"), &target_bone, bonename, IM_ARRAYSIZE(bonename));
						ImGui::PopItemWidth();

						ImGui::PushItemWidth(350);
						ImGui::SliderInt(("Fov Size:"), &fov, 50, 800);
						ImGui::SameLine(); ImGui::Text(std::to_string(fov).c_str());
						ImGui::SliderFloat(("Smoothing:"), &smoothing, 1, 30);
						ImGui::SameLine(); ImGui::Text(std::to_string(smoothing).c_str());
						ImGui::PopItemWidth();
					}
					ImGui::SliderInt(("Aim Delay:"), &aimdelay, 0, 10);
					ImGui::SameLine(); ImGui::Text(std::to_string(aimdelay).c_str());
				}
				ImGui::EndChild();
			}
		}

		if (selected == 1) {
			ImGui::BeginGroup();
			ImGui::SetCursorPos(ImVec2(110, 1));
			if (ImGui::Renderingsubtab((" Players"), 2 == sub_selected)) {
				sub_selected = 2;
			}
			ImGui::SameLine();
			if (ImGui::Renderingsubtab(("     Misc"), 4 == sub_selected)) {
				sub_selected = 4;
			}
			ImGui::EndGroup();


			if (sub_selected == 2) {
				ImGui::SetCursorPos(ImVec2(110, 44));
				ImGui::BeginChild(("Visuals"), ImVec2(219, 342));
				{
					ImGui::Spacing();
					ImGui::Checkbox(("Draw Box"), &box_esp);
					if (box_esp)
					{
						ImGui::Checkbox(("Outlined Box"), &box_outline);
						if (box_outline) {
							ImGui::SliderFloat("Outline Box Thickness", &box_outline_thickness, 1, 5);
						}
					}
					if (box_outline_thickness == 0) {
						ImGui::Checkbox("Round Box", &box_round);
					}
					static const char* box_type_name[] =
					{
						"Full",
						"Cornered",
						"Three-Dimensional"
					};
					ImGui::Combo("Box Type", &box_type, box_type_name, IM_ARRAYSIZE(box_type_name));
					ImGui::Checkbox(("Draw Snaplines"), &line);
					//ImGui::Checkbox(("Draw Username"), &draw_username);
					ImGui::Checkbox(("Draw Platform"), &draw_platform);
					ImGui::Checkbox(("Draw Current Weapon"), &draw_held_weapon);
					ImGui::Checkbox(("Draw Distance"), &draw_distance);
					ImGui::Checkbox(("Draw Skeleton"), &skeleton);
					ImGui::SliderInt("Max Distance", &max_esp_distance, 1, 800);
					//ImGui::SliderFloat("Skeleton Thickness", &SkeletonThickness, 1, 10);
				}
				ImGui::EndChild();

				ImGui::SetCursorPos(ImVec2(291 + 45, 44));
				ImGui::BeginChild(("Preview"), ImVec2(219, 342), ImGuiWindowFlags_NoScrollbar);
				{
					ImGui::PushFont(proggy_clean_game);
					if (draw_held_weapon) {
						ImGui::SetCursorPos(ImVec2(57, 31));
						ImGui::Text(("Harvesting Tool"));
					}

					if (draw_username) {
						ImGui::SetCursorPos(ImVec2(57 + 27, 48));
						ImGui::Text(("seized"));
					}

					if (draw_distance) {
						ImGui::SetCursorPos(ImVec2(57 + 30, 243));
						ImGui::Text(("1254m"));
					}

					if (draw_platform) {
						ImGui::SetCursorPos(ImVec2(57 + 35, 14));
						ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 136, 255, 255));
						ImGui::Text(("WIN"));
						ImGui::PopStyleColor();
					}

					if (box_esp)
					{
						ImDrawList* draw_list = ImGui::GetForegroundDrawList();
						ImGui::SetCursorPos(ImVec2(57, 65));
						ImVec2 pos1 = ImGui::GetCursorScreenPos();
						DrawBox(pos1.x, pos1.y, 100, 175, ImGui::GetColorU32({ 190 / 255.0F, 244 / 255.0F, 17 / 255.0F, 255 / 255.0F }), box_thickness);
					}
					ImGui::PopFont();
				}
				ImGui::EndChild();
			}

			else if (sub_selected == 4) {
				ImGui::SetCursorPos(ImVec2(110, 44));

				ImGui::BeginChild(("Misc"), ImVec2(219 + 226, 342));
				{
					ImGui::Spacing();
				}
				ImGui::EndChild();
			}


		}

		if (selected == 2) {
			ImGui::BeginGroup();
			ImGui::SetCursorPos(ImVec2(110, 1));
			if (ImGui::Renderingsubtab(("   Misc"), 5 == sub_selected)) {
				sub_selected = 5;
			}
			ImGui::EndGroup();

			if (sub_selected == 5) {
				ImGui::SetCursorPos(ImVec2(110, 44));
				ImGui::BeginChild(("Misc"), ImVec2(219, 342));
				{
					ImGui::Spacing();
					ImGui::Checkbox(("Draw Crosshair"), &draw_crosshair);
					//ImGui::Checkbox("Vsync (Recommended)", &v_sync);
					ImGui::Checkbox(("Draw FPS"), &draw_fps);
					ImGui::Checkbox(("Show Console"), &show_console);
					ImGui::Checkbox(("OBS Bypass"), &obs_bypass);
					ImGui::Checkbox(("Kelper mode"), &nazi_mode);
					if (ImGui::Button("Exit", { 65.f,35.f }))
					{
						exit(0);
					}
					/*ImGui::SliderFloat("Res X", &Resx, 1, 3000);
					ImGui::SliderFloat("Res Y", &Resy, 1, 3000);*/
					// ImGui::Checkbox(Encrypt("Vsync"), &var->vsync);

				}
				ImGui::EndChild();

				/*ImGui::SetCursorPos(ImVec2(291 + 45, 44));
				ImGui::BeginChild(("Account"), ImVec2(219, 342));
				{
					ImGui::Spacing();
					ImGui::Text((("License duration: ")));
					ImGui::Text((("Status:")));
					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
					ImGui::Text(("undetected"));
					ImGui::PopStyleColor();
					ImGui::Text(("Menu Key -> [F2]"));
					ImGui::Text(("Panic Key -> [F4]"));
				}
				ImGui::EndChild();*/
			}
		}
		ImGui::End();
	}
}