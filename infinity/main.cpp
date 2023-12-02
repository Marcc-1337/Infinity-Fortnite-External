#include "util.hpp"
#include "cheat/actors/esp.hpp"
#include "cheat/actors/aimbot.hpp"

Overlay::DXOverlay* pOverlay = nullptr;
HWND game_wndw;

/*
  Infinity.cc Leaked Source Code
  Selling this will end bad for you !

  Give me a star on my github repo and follow me for more leaks.
  https://github.com/Marcc-1337

  Status : detected on EAC & BE, lasts for 5 cpu cycles..

  Enjoy Pasting!

*/

int main() {
	SetConsoleTitleA("Infinity.cc Public");
	system("color 5");
	Sleep(1000);

	if (!driver.init())
	{
		Sleep(3500);
		exit(0);
	}

	process_id = _GetProcessId(("FortniteClient-Win64-Shipping.exe"));
	driver.attach(process_id);
	virtualaddy = driver.get_process_base(process_id);
	std::cout << "base address -> " << virtualaddy << "\n";
	system(("cls"));

	printf("\n [i] Waiting for FortniteClient-Win64-Shipping.exe");

	// using 3 times FindWindow is crazy, why not just using the first definition? or even better using a better funktion?

	game_wndw = FindWindow(0, ("Fortnite  "));
	while (!game_wndw) {
		game_wndw = FindWindow(0, ("Fortnite  "));
		Sleep(400);
	}


	Overlay::DXOverlay::GetWindow(FindWindow(0, ("Fortnite  ")));
	auto InitOverlay = pOverlay->InitOverlay();

	std::cout << ("\n [i] Loading dependencies");
	std::thread t(cache_players);
	t.detach();

	std::cout << ("\n [i] Initializing overlay");
	Sleep(1000);
	printf("\n [+] Thank you for choosing Infinity.cc!");
	Sleep(3000);

	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	switch (InitOverlay)
	{
	case 0: { break; }
	case 1:
	{
		MouseController::Init();
		while (pOverlay->MainLoop(actor_loop)) {
		}
		break;
	}
	}

}