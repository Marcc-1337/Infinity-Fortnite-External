#pragma once
#pragma warning(disable : 4996)
extern void preform_aimbot(double closest_distance, std::shared_ptr<player_data> closest_player);
inline camera_position_s get_camera_postion() {


	camera_position_s camera;

	auto location_pointer = driver.read<uintptr_t>((u)global->world + 0x110);
	auto rotation_pointer = driver.read<uintptr_t>((u)global->world + 0x120);

	struct FNRot
	{
		double a; //0x0000
		char pad_0008[24]; //0x0008
		double b; //0x0020
		char pad_0028[424]; //0x0028
		double c; //0x01D0
	}fnRot;

	fnRot.a = driver.read<double>(rotation_pointer);
	fnRot.b = driver.read<double>(rotation_pointer + 0x20);
	fnRot.c = driver.read<double>(rotation_pointer + 0x1d0);

	camera.location = driver.read<vector3>(location_pointer);
	camera.rotation.x = asin(fnRot.c) * (180.0 / M_PI);
	camera.rotation.y = ((atan2(fnRot.a * -1, fnRot.b) * (180.0 / M_PI)) * -1) * -1;
	camera.fov = driver.read<float>((uintptr_t)(u)global->player_controller + 0x394) * 90.f;

	return camera;

}
float GRD_TO_BOG(float GRD) {
	return (M_PI / 180) * GRD;
}
float BOG_TO_GRD(float BOG) {
	return (180 / M_PI) * BOG;
}
void actor_loop() {
	//cout << "actor_loop\n";
	if (debug) {
		printf(("\n [+] ActorLoop called"));
	}

	std::unique_lock<std::mutex> lock(global->data_mutex);

	camera_postion = get_camera_postion();

	ImDrawList* draw_list = ImGui::GetForegroundDrawList();
	/*char fpsinfo[64];
	sprintf(fpsinfo, ("fps: %03d"), get_fps());
	draw_list->AddText(ImGui::GetFont(), 14, ImVec2(20, 20), ImColor(0, 225, 0), fpsinfo);*/

	if (draw_fov) {
		draw_list->AddCircle(ImVec2(screen_width / 2, screen_height / 2), fov, ImColor(255, 255, 255, 255), 33, 1);
		draw_list->AddCircle(ImVec2(screen_width / 2, screen_height / 2), fov + 1, ImColor(20, 0, 0, 255), 33, 1);
	}
	if (draw_crosshair) {
		draw_list->AddLine(ImVec2(screen_width / 2 - 2, screen_height / 2), ImVec2(screen_width / 2 - 9, screen_height / 2), ImColor(0, 0, 0), 2);
		draw_list->AddLine(ImVec2(screen_width / 2 + 2, screen_height / 2), ImVec2(screen_width / 2 + 9, screen_height / 2), ImColor(0, 0, 0), 2);
		draw_list->AddLine(ImVec2(screen_width / 2, screen_height / 2 - 2), ImVec2(screen_width / 2, screen_height / 2 - 9), ImColor(0, 0, 0), 2);
		draw_list->AddLine(ImVec2(screen_width / 2, screen_height / 2 + 2), ImVec2(screen_width / 2, screen_height / 2 + 9), ImColor(0, 0, 0), 2);

		draw_list->AddLine(ImVec2(screen_width / 2 - 3, screen_height / 2), ImVec2(screen_width / 2 - 8, screen_height / 2), ImColor(255, 255, 255), 1);
		draw_list->AddLine(ImVec2(screen_width / 2 + 3, screen_height / 2), ImVec2(screen_width / 2 + 8, screen_height / 2), ImColor(255, 255, 255), 1);
		draw_list->AddLine(ImVec2(screen_width / 2, screen_height / 2 - 3), ImVec2(screen_width / 2, screen_height / 2 - 8), ImColor(255, 255, 255), 1);
		draw_list->AddLine(ImVec2(screen_width / 2, screen_height / 2 + 3), ImVec2(screen_width / 2, screen_height / 2 + 8), ImColor(255, 255, 255), 1);
	}
	if (!show_console) {
		ShowWindow(GetConsoleWindow(), SW_HIDE);
	}

	else if (show_console) {
		ShowWindow(GetConsoleWindow(), SW_SHOW);
	}


	if (obs_bypass) {
		SetWindowDisplayAffinity(Overlay::DXOverlay::pOverlay, WDA_EXCLUDEFROMCAPTURE);
	}
	if (!obs_bypass) {
		SetWindowDisplayAffinity(Overlay::DXOverlay::pOverlay, !WDA_EXCLUDEFROMCAPTURE);
	}
	if (nazi_mode) {
		POINT Screen;
		int oodofdfo, jbjfdbdsf;
		Screen.x = GetSystemMetrics(0);
		Screen.y = GetSystemMetrics(1);
		// Middle POINT
		POINT Middle;
		Middle.x = (int)(Screen.x / 2);
		Middle.y = (int)(Screen.y / 2);
		int a = (int)(Screen.y / 2 / 30);
		float gamma = atan(a / a);
		static int faken_rot = 0;
		faken_rot++;
		int Drehungswinkel = faken_rot;

		int i = 0;
		while (i < 4) {
			std::vector<int> p;
			p.push_back(a * sin(GRD_TO_BOG(Drehungswinkel + (i * 90))));                                    // p[0]        p0_A.x
			p.push_back(a * cos(GRD_TO_BOG(Drehungswinkel + (i * 90))));                                    // p[1]        p0_A.y
			p.push_back((a / cos(gamma)) * sin(GRD_TO_BOG(Drehungswinkel + (i * 90) + BOG_TO_GRD(gamma))));    // p[2]        p0_B.x
			p.push_back((a / cos(gamma)) * cos(GRD_TO_BOG(Drehungswinkel + (i * 90) + BOG_TO_GRD(gamma))));    // p[3]        p0_B.y

			// Calculate the hue based on time (you can replace this with any other variable)
			float hue = fmodf(ImGui::GetTime() * 0.1f, 1.0f); // Range from 0.0 to 1.0

			ImU32 lineColor = ImColor::HSV(hue + i * 0.25f, 1.0f, 1.0f); // Radial gradient with rainbow colors
			ImVec2 p0 = ImVec2(Middle.x, Middle.y);
			ImVec2 p1 = ImVec2(Middle.x + p[0], Middle.y - p[1]);
			ImVec2 p2 = ImVec2(Middle.x + p[2], Middle.y - p[3]);

			// Save the current style and modify it to apply the same rainbow effect
			ImGuiStyle& style = ImGui::GetStyle();
			ImVec4 savedColor = style.Colors[ImGuiCol_Text];
			style.Colors[ImGuiCol_Text] = ImVec4(ImColor::HSV(hue + i * 0.25f, 1.0f, 1.0f));

			draw_list->AddLine(p0, p1, lineColor);
			draw_list->AddLine(p1, p2, lineColor);

			// Restore the original style
			style.Colors[ImGuiCol_Text] = savedColor;

			i++;
		}
	}
	if (draw_fps) {
		char fpsinfo[256];
		sprintf_s(fpsinfo, (("[ FPS: %03d ]")), ImGui::GetIO().Framerate);
		draw_list->AddText(ImVec2(960, 1), ImColor(255, 255, 255, 255), fpsinfo);
	}
	if (draw_watermark) {
		ImVec2 viewportSize = ImGui::GetMainViewport()->Size;

		float fontSize = 22.5f; 

		ImVec2 textPosition(0, 0); 

		float hue = fmodf(ImGui::GetTime() * 0.1f, 1.0f);
		ImColor color = ImColor::HSV(hue, 1, 1);

		ImFont* currentFont = ImGui::GetFont();
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, textPosition, color, "[ Infinity.cc Public ]");

		ImGui::PopFont();
	}
	/*
	if (weaponconfigs)
	{
		auto CurrentWeapon = driver::read<uintptr_t>((uint64_t)global->local_pawn + 0x8f8);
		auto ItemData = driver::read<DWORD_PTR>(CurrentWeapon + 0x410);
		auto DisplayName = driver::read<uint64_t>(ItemData + 0x90); //UPlaylistUserOptionBase	OptionDisplayName	0x90	FText
		auto WeaponLength = driver::read<uint32_t>(DisplayName + 0x38);
		wchar_t* WeaponName = new wchar_t[uint64_t(WeaponLength) + 1];

		driver::read_memory(driver::read<PVOID>(DisplayName + 0x30), WeaponName, WeaponLength * sizeof(wchar_t));
		std::string Text = wchar_to_char(WeaponName);

		if (Text == "Red-Eye Assault Rifle" || Text == "Assault Rifle  " || Text == "Tactical Assault Rifle" || Text == "Burst Assault Rifle" || Text == "MK-Seven Assault Rifle"
			|| Text == "MK-Seven Assault Rifle" || Text == "Suppressed Assault Rifle" || Text == "Striker Burst Rifle" || Text == "Heavy Assault Rifle" || Text == "Ranger Assault Rifle"
			|| Text == "Light Machine Gun" || Text == "Combat Assault Rifle" || Text == "Hammer Assault Rifle" || Text == "Infantry Rifle" || Text == "Makeshift Rifle" || Text == "Havoc Suppressed Assault Rifle" || Text == "Highcard's Havoc Suppressed Assault Rifle" || Text == "Overclocked Pulse Rifle") {
			fov = fov1;
			smoothing = smoothness1;
			target_bone = RifleCurrentHitbox;
		}
		if (Text == "Pump Shotgun" || Text == "Thunder Shotgun" || Text == "Tactical Shotgun" || Text == "Striker Pump Shotgun" || Text == "Maven Auto Shotgun" || Text == "Heavy Shotgun"
			|| Text == "Two-Shot Shotgun" || Text == "Ranger Shotgun" || Text == "Double Barrel Shotgun" || Text == "Combat Shotgun" || Text == "Prime Shotgun" || Text == "Charge Shotgun"
			|| Text == "Lever Action Shotgun" || Text == "Auto Shotgun" || Text == "Drum Shotgun" || Text == "Havoc Pump Shotgun") {
			fov = fov2;
			smoothing = smoothness2;
			target_bone = ShotgunCurrentHitbox;
		}
		if (Text == "Twin Mag SMG" || Text == "Combat SMG" || Text == "Stinger SMG" || Text == "Submachine Gun" || Text == "Tactical Submachine Gun" || Text == "Drum Gun"
			|| Text == "Machine SMG" || Text == "Primal SMG" || Text == "Submachine Gun" || Text == "Suppressed Submachine Gun" || Text == "Rapid Fire SMG" || Text == "Charge SMG") {
			fov = fov3;
			smoothing = smoothness3;
			target_bone = SmgCurrentHitbox;
		}
		if (Text == "Hand Cannon" || Text == "Sidearm Pistol" || Text == "Revolver" || Text == "Scoped Revolver" || Text == "Dual Pistols" || Text == "Suppressed Pistol" || Text == "Flint-Knock Pistol"
			|| Text == "Tactical Pistol") {
			fov = fov4;
			smoothing = smoothness5;
			target_bone = PistolCurrentHitbox;
		}
		if (Text == "Hunting Rifle" || Text == "Heavy Sniper Rifle" || Text == "Suppresed Sniper Rifle" || Text == "Storm Scout" || Text == "Bolt-Action Sniper Rifle" || Text == "DMR"
			|| Text == "Hunter Bolt-Action Sniper" || Text == "Scoped Assault Rifle" || Text == "Semi-Auto Sniper Rifle" || Text == "Thermal Scoped Assault Rifle" || Text == "Lever Action Rifle"
			|| Text == "Mechanical Bow" || Text == "Crossbow" || Text == "Fiend Hunter Crossbow") {
			fov = fov5;
			smoothing = smoothness5;
			target_bone = SniperCurrentHitbox;
		}
	}
	*/

	//if (global->players.size() < 1) return;

	//auto mesh = driver.read<uintptr_t>((u)global->local_pawn + 0x318);
	//if (mesh) {
	//	//int is_cached = driver.read<int>(mesh + 0x638);
	//	//std::cout << driver.read<tarray<FTransform>>(mesh + 0x10 * is_cached + 0x5F0).Length() << std::endl;
	//	for (int i = 0; i < 100; i++) {
	//		int is_cached = driver.read<int>((u)mesh + 0x648);
	//		auto bone_transform = driver.read<FTransform>(driver.read<u>((u)mesh + 0x10 * is_cached + 0x600) + 0x60 * i);
	//		std::random_device rd;
	//		std::mt19937 gen(rd());
	//		std::uniform_real_distribution<double> dis(0.0, 360.0);

	//		auto x = dis(gen);
	//		auto y = dis(gen);
	//		auto z = dis(gen);
	//		auto zz = dis(gen);
	//		bone_transform.rot = fquat(x, y, z, zz);
	//		ctx->write<FTransform>(driver.read<u>((u)mesh + 0x10 * is_cached + 0x600) + 0x60 * i, bone_transform);
	//		printf("wrote to memory");
	//	}
	//}


	double closest_distance = DBL_MAX;
	std::shared_ptr<player_data> closest_player = std::make_shared<player_data>();
	for (auto player : global->players) {
		cout << "enterted loop\n";
		vector3 head_3d = player->mesh->get_head();
		vector2 head_2d = w2s(vector3(head_3d.x, head_3d.y, head_3d.z + 20));

		vector3 base_3d = player->mesh->get_base();
		vector2 base_2d = w2s(base_3d);

		float box_hight = abs(head_2d.y - base_2d.y);
		float box_width = box_hight * 0.70;

		if (!head_2d.valid_location())
			continue;

		if (esp) {

			ImColor visible_color;
			if (player->mesh->visible()) {
				visible_color = ImColor(235, 189, 107, 255);
			}
			else if (!player->mesh->visible()) {
				visible_color = ImColor(201, 135, 52, 255);
			}
			if (player->player == global->local_pawn) continue;
			/*
			if (close_distance && player->player_distance < distance69) {
				fov = close_fov;
				smoothing = AimSpeedCloseRange;
				target_bone = CurrentCloseHitbox;
			}
			else {
				fov = norm_fov;
				smoothing = norm_smooth;
				target_bone = norm_hitbox;
			}
			*/

			if (box_esp) {
				if (box_type == 0) {
					if (box_outline) {
						if (box_round)
							draw_list->AddRect(ImVec2(head_2d.x - box_width / 2, head_2d.y), ImVec2((head_2d.x - box_width / 2) + box_width, head_2d.y + box_hight), ImColor(0, 0, 0), 10, ImDrawCornerFlags_All, box_outline_thickness);
						else
							draw_list->AddRect(ImVec2(head_2d.x - box_width / 2, head_2d.y), ImVec2((head_2d.x - box_width / 2) + box_width, head_2d.y + box_hight), ImColor(0, 0, 0), 0, 0, box_outline_thickness);

					}
					if (box_round)
						draw_list->AddRect(ImVec2(head_2d.x - box_width / 2, head_2d.y), ImVec2((head_2d.x - box_width / 2) + box_width, head_2d.y + box_hight), visible_color, 10, ImDrawCornerFlags_All, box_thickness);
					else
						draw_list->AddRect(ImVec2(head_2d.x - box_width / 2, head_2d.y), ImVec2((head_2d.x - box_width / 2) + box_width, head_2d.y + box_hight), visible_color, 0, 0, box_thickness);
				}
				else if (box_type == 1) {
					if (box_outline)
						DrawCornerBox(head_2d.x - box_width / 2, head_2d.y, box_width, box_hight, box_outline_thickness, ImColor(0, 0, 0), box_round);

					DrawCornerBox(head_2d.x - box_width / 2, head_2d.y, box_width, box_hight, box_thickness, visible_color, box_round);
				}
				else if (box_type == 2) {
					if (box_outline)
						DrawThreeDBox(base_3d, head_3d, 100, 100, box_outline_thickness, ImColor(0, 0, 0));
					DrawThreeDBox(base_3d, head_3d, 100, 100, box_thickness, visible_color);
				}
			}

			if (draw_held_weapon) {
				auto CurrentWeapon = driver.read<uintptr_t>((uint64_t)player->player + 0x948); auto ItemData = driver.read<DWORD_PTR>(CurrentWeapon + 0x450);
				auto DisplayName = driver.read<uint64_t>(ItemData + 0x90); auto WeaponLength = driver.read<uint32_t>(DisplayName + 0x38); auto EFortRarity = driver.read<int>(ItemData + 0x2e8);
				wchar_t* WeaponName = new wchar_t[uint64_t(WeaponLength) + 1];  //driver->ReadProcessMemory(read<uint64_t>(DisplayName + 0x30), WeaponName, WeaponLength * sizeof(wchar_t));
				RGBA weapon_color;
				switch (EFortRarity) {
				case(0):
					weapon_color = { 255, 255, 255 };
					break;
				case(1):
					weapon_color = { 0, 255, 39 };
					break;
				case(2):
					weapon_color = { 0, 103, 255 };
					break;
				case(3):
					weapon_color = { 133, 0, 255 };
					break;
				case(4):
					weapon_color = { 255, 217, 0 };
					break;
				case(5):
					weapon_color = { 255, 87, 0 };
					break;
				default:
					weapon_color = { 255, 255, 255 };
					break;
				}

				std::string wep_str = wchar_to_char(WeaponName);
				std::string lastweapon;
				auto AmmoCount = driver.read<int>(CurrentWeapon + 0xb8c);

				if (AmmoCount)
				{
					char buffer[128];
					sprintf_s(buffer, (" %i"), AmmoCount);
					if (buffer != ("?"))
					{
						lastweapon = wep_str + (" | ") + buffer;
					}
					else
					{
						lastweapon = wep_str;
					}
				}
				else
				{
					lastweapon = wep_str;
				}

				if (lastweapon.length() >= 3 && lastweapon.find(("?")) == std::string::npos && wep_str.find(("?")) == std::string::npos)
				{
					ImVec2 TextSize = ImGui::CalcTextSize(wep_str.c_str());
					DrawString(18, (base_2d.x) - (TextSize.x / 2), (head_2d.y - 15), &weapon_color, false, true, wep_str.c_str());
				}
			}

			/*if (draw_platform) {
				DWORD_PTR test_platform = driver.read<DWORD_PTR>((uint64_t)player->player + 0x430); wchar_t platform[64];
				driver::read_memory((void*)test_platform, &platform, sizeof(platform)); std::wstring balls_sus(platform);
				std::string str_platform(balls_sus.begin(), balls_sus.end()); ImVec2 TextSize = ImGui::CalcTextSize(str_platform.c_str()); int offset;

				if (draw_username && draw_held_weapon) offset = 45;
				else if (draw_username && !draw_held_weapon) offset = 30;
				else if (!draw_username && draw_held_weapon) offset = 30;
				else if (!draw_username && !draw_held_weapon) offset = 15;
				if (str_platform.find(("XBL")) != std::string::npos || str_platform.find(("XSX")) != std::string::npos) {
					DrawString(18, (base_2d.x) - (TextSize.x / 2), (head_2d.y - offset), &Col.green, false, true, ("XBOX"));
				}
				else if (str_platform.find(("PSN")) != std::string::npos || str_platform.find(("PS5")) != std::string::npos) {
					DrawString(18, (base_2d.x) - (TextSize.x / 2), (head_2d.y - offset), &Col.blue, false, true, ("PSN"));
				}
				else if (str_platform.find(("WIN")) != std::string::npos) {
					DrawString(18, (base_2d.x) - (TextSize.x / 2), (head_2d.y - offset), &Col.skyblue, false, true, ("WIN"));
				}
				else if (str_platform.find(("SWH")) != std::string::npos) {
					DrawString(18, (base_2d.x) - (TextSize.x / 2), (head_2d.y - offset), &Col.red, false, true, ("SWT"));
				}
			}*/

			if (line) {
				draw_list->AddLine(ImVec2(screen_width / 2, 0), ImVec2(head_2d.x, head_2d.y), visible_color, 2);
			}

			if (skeleton) {
				drawskeleton(player->mesh, true, visible_color);
			}

			if (draw_distance) {
				std::string dist = std::to_string(int(player->player_distance)).c_str(); std::string final = dist + (" m"); ImVec2 TextSize = ImGui::CalcTextSize(final.c_str());
				DrawString(18, (base_2d.x) - (TextSize.x / 2), (base_2d.y + 5), &Col.nothing, false, true, final.c_str());
				DrawString(18, (base_2d.x) - (TextSize.x / 2), (base_2d.y + 5), &Col.SilverWhite, false, true, final.c_str());
			}
		}





		auto dist = GetCrossDistance(head_2d.x, head_2d.y, screen_width / 2, screen_height / 2);
		if (dist < fov && dist < closest_distance && player->player_distance < max_aimbot_distance) {
			closest_distance = dist;
			closest_player = player;
		}

	}
	preform_aimbot(closest_distance, closest_player);
	lock.unlock();


}