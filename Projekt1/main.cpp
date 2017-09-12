#include "main.h"

using namespace std;
#pragma comment(lib, "d2d1")
#pragma comment(lib, "cudart")
#pragma comment(lib, "Dwrite.lib")
Graphics* graphics;

const float resX = 1680.0f;
const float resY = 1050.0f;

WCHAR* sonne = L"Sonne";
WCHAR* merkur = L"Merkur";
WCHAR* venus = L"Venus";
WCHAR* erde = L"Erde";
WCHAR* mars = L"Mars";
WCHAR* jupiter = L"Jupiter";
WCHAR* saturn = L"Saturn";
WCHAR* uranus = L"Uranus";
WCHAR* neptun = L"Neptun";
WCHAR* pluto = L"Pluto";

WCHAR* text = L"Simulation des Sonnensystems\n\nUmlaufzeit\tGeschwindigkeit\t\tDurchmesser\tMasse\nMerkur:\t90 Tage\t\t47,36 km/s\t\t4.879,4 km\t3,301 · 10^23 kg\nVenus:\t225 Tage\t35,02 km/s\t\t12.103,6 km\t4,869 · 10^24 kg\nErde:\t365 Tage\t29,78 km/s\t\t12.756,32 km\t5,974 · 10^24 kg\nMars:\t686,98 Tage\t24,13 km/s\t\t6.792,4 km\t6,419 · 10^23 kg\nJupiter:\t11,86 Jahre\t13,07 km/s\t\t142.984 km\t1,899 · 10^27 kg\nSaturn:\t29,457 Jahre\t9,69 km/s\t\t120.536 km\t5,685 · 10^26 kg\nUranus:\t84, 011 Jahre\t6,81 km/s\t\t51.118 km\t8,683 · 10^25 kg\nNeptun:\t164,79 Jahre\t5,43 km/s\t\t49.528 km\t1,024 · 10^26 kg\nPluto:\t247,93 Jahre\t4,67 km/s\t\t2374 km\t\t1,303 · 10^22 kg";

double s = 0.0025;
double sr = 1.0;
double sg = 0.6; 
int z = 0;
bool te;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_DESTROY) { PostQuitMessage(0); return 0; }

	/*if (uMsg == WM_PAINT) {
		graphics->BeginDraw();
		graphics->ClearScreen(0.0f,0.0f,0.5f);
		graphics->DrawCircle(100, 100, 50, 1.0f, 0.0, 0.0, 1.0);
		graphics->EndDraw();
	}*/

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmd, int nCmdShow) {
	WNDCLASSEX windowclass;
	ZeroMemory(&windowclass, sizeof(WNDCLASSEX));
	windowclass.cbSize = sizeof(WNDCLASSEX);
	windowclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowclass.hInstance = hInstance;
	windowclass.lpfnWndProc = WindowProc;
	windowclass.lpszClassName = "MainWindow";
	windowclass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&windowclass);
	RECT rect = { 0,0,1680,1050 };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);
	HWND windowshandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "MainWindow", "DirectX", WS_OVERLAPPEDWINDOW, 100, 100, 
		rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, 0);

	if (!windowshandle) return -1;
	graphics = new Graphics();

	if (!graphics->Init(windowshandle)) {
		delete graphics;
		return -1;
	}

	ShowWindow(windowshandle, nCmdShow);

	
	/*double gravitation = 2.5;
	double speed = 7778;
	double xNeu = 640000;
	double yNeu = 0;*/
	//mond erde merkur venus mars jupiter saturn uranus neptun pluto 
	long double dist[] = {  400000 , 149000000, 57000000, 108160000, 227990000, 778360000, 1433500000, 2872400000, 4498400000, 5906400000 };
	long double speed[] = {  1023, 29780, 47360, 35020, 24130, 13070, 9690, 6810, 5430, 4670 }; //m/s
	long double xNeu[] = {  400000 , 149000000, 57000000, 108160000, 227990000, 778360000, 1433500000, 2872400000, 4498400000, 5906400000 }; //km
	long double yNeu[] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
	long double gravitation[] = { 1.30815911089, 2.97600132, 19.675168537, 5.66938040, 1.27693516, 0.10973394, 0.0327506, 0.0080729, 0.0032768, 0.0018463 };
	
	dreieck* dSpeed = new dreieck();
	dreieck* dGravitaton = new dreieck();
	vector<dreieck*> dSteigung;
	for (int i = 0; i < 10; i++) {
		dreieck* d = new dreieck();
		dSteigung.push_back(d);
	}
	dreieck* dZw = new dreieck();
	bool w = true;
	bool test[] = { true,true,true,true,true,true,true,true,true,true };

	MSG message;
	message.message = WM_NULL;

	while (message.message != WM_QUIT) {
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			DispatchMessage(&message);
		}
		else {
			for (int i = 0; i < 10; i++) {


				dSpeed->reset();
				dZw->reset();



				if (yNeu[i] == 0 || xNeu[i] == 0) {
					dSpeed->setGegenkatete(speed[i]);
					if (yNeu[i] == 0) {
						dSpeed->setAnkatete(xNeu[i]);
					}
					else if (xNeu[i] == 0) {
						dSpeed->setAnkatete(yNeu[i]);
					}
					if (i == 1) {
						dSpeed->hypotenuseBerechnen();
					}
					dSpeed->hypotenuseBerechnen();
					dSpeed->sinAlphaBerechnen();
					dSpeed->sinBetaBerechnen();
					yNeu[i] = yNeu[i] + dSpeed->getGegenkatete();
					cout << dSpeed->toString() << endl;
					cout << "x: " << xNeu[i] << " y: " << yNeu[i] << endl;

				}
				else if (yNeu[i] > xNeu[i]) {
					if (i == 1) {
						dSpeed->setHypotenuse(speed[i]);
					}
					dSpeed->setHypotenuse(speed[i]);
					dSpeed->setAlpha(dSteigung.at(i)->getAlpha());
					dSpeed->betaBerechnen();
					dSpeed->ankateteMitWinkelBerechnen();
					dSpeed->gegenkateteBerechnen();
					if (xNeu[i] < 0 && yNeu[i]>0) {
						xNeu[i] = xNeu[i] - dSpeed->getGegenkatete();
						yNeu[i] = yNeu[i] - dSpeed->getAnkatete();
					}
					else if (xNeu[i] > 0 && yNeu[i] > 0) {
						xNeu[i] = xNeu[i] - dSpeed->getGegenkatete();
						yNeu[i] = yNeu[i] + dSpeed->getAnkatete();
					}
					else if (xNeu[i] < 0 && yNeu[i] < 0) {
						xNeu[i] = xNeu[i] + dSpeed->getGegenkatete();
						yNeu[i] = yNeu[i] - dSpeed->getAnkatete();
					}
					else if (xNeu[i] > 0 && yNeu[i] < 0) {
						xNeu[i] = xNeu[i] + dSpeed->getGegenkatete();
						yNeu[i] = yNeu[i] + dSpeed->getAnkatete();
					}
					cout << "\nSpeed" << endl;
					cout << dSpeed->toString() << endl;
					cout << "x: " << xNeu[i] << " y: " << yNeu[i] << endl;
					if (yNeu[i] > xNeu[i]) {
						dZw->setAnkatete(xNeu[i]);
						dZw->setGegenkatete(yNeu[i]);
						dZw->hypotenuseBerechnen();
						dZw->sinAlphaBerechnen();
						dZw->sinBetaBerechnen();
						cout << "\ndZw" << endl;
						cout << dZw->toString() << endl;
						cout << "x: " << xNeu[i] << " y: " << yNeu[i] << endl;
					}
					else {
						dZw->setAnkatete(yNeu[i]);
						dZw->setGegenkatete(xNeu[i]);
						/*if(xNeu<0&&yNeu<0){
						dZw->setAnkatete(xNeu);
						dZw->setGegenkatete(yNeu);
						}else{
						dZw->setAnkatete(yNeu);
						dZw->setGegenkatete(xNeu);
						}*/
						dZw->hypotenuseBerechnen();
						dZw->sinAlphaBerechnen();
						dZw->sinBetaBerechnen();
						cout << "\ndZw" << endl;
						cout << dZw->toString() << endl;
						cout << "x: " << xNeu[i] << " y: " << yNeu[i] << endl;
					}
				}
				else if (yNeu[i] < xNeu[i]) {
					if (i == 1) {
						dSpeed->setHypotenuse(speed[i]);
					}
					dSpeed->setHypotenuse(speed[i]);
					dSpeed->setAlpha(dSteigung.at(i)->getAlpha());
					dSpeed->gegenkateteMitWinkelBerechnen();
					dSpeed->ankateteBerechnen();/////////////////////////////////////////
					//xNeu = xNeu - dSpeed->getGegenkatete();
					//yNeu = yNeu + dSpeed->getAnkatete();
					if (xNeu[i] < 0 && yNeu[i]>0) {
						xNeu[i] = xNeu[i] - dSpeed->getGegenkatete();
						yNeu[i] = yNeu[i] - dSpeed->getAnkatete();
					}
					else if (xNeu[i] > 0 && yNeu[i] > 0) {
						xNeu[i] = xNeu[i] - dSpeed->getGegenkatete();
						yNeu[i] = yNeu[i] + dSpeed->getAnkatete();
					}
					else if (xNeu[i] < 0 && yNeu[i] < 0) {
						xNeu[i] = xNeu[i] + dSpeed->getGegenkatete();
						yNeu[i] = yNeu[i] - dSpeed->getAnkatete();
					}
					else if (xNeu[i] > 0 && yNeu[i] < 0) {
						xNeu[i] = xNeu[i] + dSpeed->getGegenkatete();
						yNeu[i] = yNeu[i] + dSpeed->getAnkatete();
					}
					cout << "\nSpeed" << endl;
					cout << dSpeed->toString() << endl;
					cout << "x: " << xNeu[i] << " y: " << yNeu[i] << endl;
					if (yNeu[i] < xNeu[i]) {
						//dZw->setAnkatete(xNeu);
						//dZw->setGegenkatete(yNeu);
						if (xNeu[i] < 0 && yNeu[i] < 0) {
							dZw->setAnkatete(yNeu[i]);
							dZw->setGegenkatete(xNeu[i]);
						}
						else if (xNeu[i] > 0 && yNeu[i] < 0) {
							dZw->setAnkatete(yNeu[i]);
							dZw->setGegenkatete(xNeu[i]);
						}
						else {
							dZw->setAnkatete(xNeu[i]);
							dZw->setGegenkatete(yNeu[i]);
						}
						dZw->hypotenuseBerechnen();
						long double diff;
						if (i == 1) {
							if (dist[i] > dZw->getHypotenuse()) {
								diff = dist[i] - dZw->getHypotenuse();
							}
							else {
								diff = dZw->getHypotenuse() - dist[i];
							}
						}
						dZw->sinAlphaBerechnen();
						dZw->sinBetaBerechnen();
						cout << "\ndZw" << endl;
						cout << dZw->toString() << endl;
						cout << "x: " << xNeu[i] << " y: " << yNeu[i] << endl;
					}
					else {
						dZw->setAnkatete(yNeu[i]);
						dZw->setGegenkatete(xNeu[i]);
						dZw->hypotenuseBerechnen();
						dZw->sinAlphaBerechnen();
						dZw->sinBetaBerechnen();
						cout << "\ndZw" << endl;
						cout << dZw->toString() << endl;
						cout << "x: " << xNeu[i] << " y: " << yNeu[i] << endl;
					}

				}

				dGravitaton->reset();
				if (dZw->getAnkatete() == 0.0) {
					dGravitaton->setHypotenuse(gravitation[i]);
					dGravitaton->setAlpha(dSpeed->getAlpha());
					dGravitaton->setBeta(dSpeed->getBeta());
					dGravitaton->ankateteMitWinkelBerechnen();
					dGravitaton->gegenkateteMitWinkelBerechnen();
					//xNeu = xNeu - dGravitaton->getAnkatete();
					//yNeu = yNeu - dGravitaton->getGegenkatete();
					if (dGravitaton->getAlpha() < 45) {
						xNeu[i] = xNeu[i] - dGravitaton->getAnkatete();
						yNeu[i] = yNeu[i] - dGravitaton->getGegenkatete();
					}
					else {
						xNeu[i] = xNeu[i] - dGravitaton->getGegenkatete();
						yNeu[i] = yNeu[i] - dGravitaton->getAnkatete();
					}
					cout << "\nGravitation" << endl;
					cout << dGravitaton->toString() << endl;
					cout << "x: " << xNeu[i] << " y: " << yNeu[i] << endl;

				}
				else {
					dGravitaton->setHypotenuse(gravitation[i]);
					dGravitaton->setAlpha(dZw->getAlpha());
					dGravitaton->setBeta(dZw->getBeta());
					dGravitaton->ankateteMitWinkelBerechnen();
					dGravitaton->gegenkateteMitWinkelBerechnen();
					//xNeu = xNeu - dGravitaton->getGegenkatete();
					//yNeu = yNeu - dGravitaton->getAnkatete();
					if (dGravitaton->getAlpha() < 45) {
						if (xNeu[i] < 0 && yNeu[i]>0) {
							test[i] = false;
							xNeu[i] = xNeu[i] + dGravitaton->getAnkatete();
							yNeu[i] = yNeu[i] - dGravitaton->getGegenkatete();
						}
						else if (xNeu[i] > 0 && yNeu[i] > 0) {
							if (test[i]) {
								xNeu[i] = xNeu[i] - dGravitaton->getGegenkatete();
								yNeu[i] = yNeu[i] - dGravitaton->getAnkatete();
							}
							else {
								xNeu[i] = xNeu[i] - dGravitaton->getAnkatete();
								yNeu[i] = yNeu[i] - dGravitaton->getGegenkatete();
							}
						}
						else if (xNeu[i] < 0 && yNeu[i] < 0) {
							if (xNeu[i] > yNeu[i]) {
								xNeu[i] = xNeu[i] + dGravitaton->getGegenkatete();
								yNeu[i] = yNeu[i] + dGravitaton->getAnkatete();
							}
							else {
								xNeu[i] = xNeu[i] + dGravitaton->getAnkatete();
								yNeu[i] = yNeu[i] + dGravitaton->getGegenkatete();
							}
						}
						else if (xNeu[i] > 0 && yNeu[i] < 0) {
							xNeu[i] = xNeu[i] - dGravitaton->getGegenkatete();
							yNeu[i] = yNeu[i] + dGravitaton->getAnkatete();
						}
					}
					else {
						if (xNeu[i] < 0 && yNeu[i]>0) {
							xNeu[i] = xNeu[i] + dGravitaton->getAnkatete();
							yNeu[i] = yNeu[i] - dGravitaton->getGegenkatete();
						}
						else if (xNeu[i] > 0 && yNeu[i] > 0) {
							xNeu[i] = xNeu[i] - dGravitaton->getAnkatete();
							yNeu[i] = yNeu[i] - dGravitaton->getGegenkatete();
						}
						else if (xNeu[i] < 0 && yNeu[i] < 0) {
							xNeu[i] = xNeu[i] + dGravitaton->getAnkatete();
							yNeu[i] = yNeu[i] + dGravitaton->getGegenkatete();
						}
						else if (xNeu[i] > 0 && yNeu[i] < 0) {
							xNeu[i] = xNeu[i] - dGravitaton->getGegenkatete();
							yNeu[i] = yNeu[i] + dGravitaton->getAnkatete();
						}
					}
					cout << "\nGravitation" << endl;
					cout << dGravitaton->toString() << endl;
					cout << "x: " << xNeu[i] << " y: " << yNeu[i] << endl;
				}

				dSteigung.at(i)->reset();
				dSteigung.at(i)->setAnkatete(xNeu[i]);
				dSteigung.at(i)->setGegenkatete(yNeu[i]);
				dSteigung.at(i)->hypotenuseBerechnen();
				dSteigung.at(i)->sinAlphaBerechnen();
				cout << "\nSteigung" << endl;
				cout << dSteigung.at(i)->toString() << endl;
				cout << "x: " << xNeu[i] << " y: " << yNeu[i] << endl;

			}

			graphics->BeginDraw();
			graphics->ClearScreen(0.0f, 0.0f, 0.1f);
			
			if (sr <= 0.8) {
				te = true;
			}
			if(sr >= 1.0 ){
				te = false;
			}
			if(te){
				sr += s;
				sg += s;
			}else {
				sr -= s;
				sg -= s;
			}
			z++;
			graphics->DrawCircle(resX / 2, resY / 2, 20.0f, sr, sg, 0.0f, 10.0f);
			graphics->DrawTextS(resX / 2, resY / 2, sonne);
			long double tmpx = (resX / 2) + (xNeu[9] / 7000000);
			int tmpy = (resX / 2) + (xNeu[9] / 7000000);
			//graphics->DrawCircle((resX / 2) + xNeu[0] , (resY / 2) + yNeu[0] , 5.0f, 1.0f, 1.0f, 1.0f, 10.f);//erde
			graphics->DrawCircle((resX / 2) + (xNeu[1] / 1000000)  + (xNeu[0] / 50000), (resY / 2) + (yNeu[1] / 1000000) + (yNeu[0] / 50000), 2.0f, 0.5f, 0.5f, 0.0f, 10.f);//mond
			graphics->DrawCircle((resX / 2) + (xNeu[1] / 1000000), (resY / 2) + (yNeu[1] / 1000000), 5.0f, 0.0f, 0.0f, 0.8f, 10.f);//erde
			graphics->DrawTextS((resX / 2) + (xNeu[1] / 1000000), (resY / 2) + (yNeu[1] / 1000000), erde);
			
			graphics->DrawCircle((resX / 2) + (xNeu[2] / 1000000), (resY / 2) + (yNeu[2] / 1000000), 3.0f, 0.8f, 0.2f, 0.0f, 10.f);//merkur
			graphics->DrawTextS((resX / 2) + (xNeu[2] / 1000000), (resY / 2) + (yNeu[2] / 1000000), merkur);
			
			graphics->DrawCircle((resX / 2) + (xNeu[3] / 1000000), (resY / 2) + (yNeu[3] / 1000000), 5.0f, 0.5f, 0.2f, 0.0f, 10.f);//venus
			graphics->DrawTextS((resX / 2) + (xNeu[3] / 1000000), (resY / 2) + (yNeu[3] / 1000000), venus);
			
			graphics->DrawCircle((resX / 2) + (xNeu[4] / 1200000), (resY / 2) + (yNeu[4] / 1200000), 5.0f, 0.5f, 0.2f, 0.0f, 10.f);//mars
			graphics->DrawTextS((resX / 2) + (xNeu[4] / 1200000), (resY / 2) + (yNeu[4] / 1200000), mars);
			
			graphics->DrawCircle((resX / 2) + (xNeu[5] / 3200000), (resY / 2) + (yNeu[5] / 3200000), 10.0f, 0.5f, 0.5f, 0.5f, 10.f);//jupiter
			graphics->DrawTextS((resX / 2) + (xNeu[5] / 3200000), (resY / 2) + (yNeu[5] / 3200000), jupiter);
			
			graphics->DrawCircle((resX / 2) + (xNeu[6] / 4700000), (resY / 2) + (yNeu[6] / 4700000), 10.0f, 1.0f, 0.3f, 0.0f, 10.f);//saturn
			graphics->DrawTextS((resX / 2) + (xNeu[6] / 4700000), (resY / 2) + (yNeu[6] / 4700000), saturn);
			graphics->DrawOval((resX / 2) + (xNeu[6] / 4700000), (resY / 2) + (yNeu[6] / 4700000), 12.0f, 2.0f , 1.0f, 0.5f, 0.2f, 10.f);//saturn
			
			graphics->DrawCircle((resX / 2) + (xNeu[7] / 7500000), (resY / 2) + (yNeu[7] / 7500000), 7.0f, 0.0f, 0.0f, 0.5f, 10.f);//uranus
			graphics->DrawTextS((resX / 2) + (xNeu[7] / 7500000), (resY / 2) + (yNeu[7] / 7500000), uranus);
			
			graphics->DrawCircle((resX / 2) + (xNeu[8] / 8000000), (resY / 2) + (yNeu[8] / 8000000), 7.0f, 0.1f, 0.0f, 0.5f, 10.f);//neptun
			graphics->DrawTextS((resX / 2) + (xNeu[8] / 8000000), (resY / 2) + (yNeu[8] / 8000000), neptun);
			
			graphics->DrawCircle((resX / 2) + (xNeu[9] / 8000000), (resY / 2) + (yNeu[9] / 8000000), 2.0f, 0.5f, 0.5f, 0.5f, 10.f);//pluto
			graphics->DrawTextS((resX / 2) + (xNeu[9] / 8000000), (resY / 2) + (yNeu[9] / 8000000), pluto);
			

			graphics->DrawRect(0, resY-250, 300, 700, 0.1, 0.1, 0.1, 0.7);
			graphics->DrawTextS(350, resY - 250, text);
			graphics->EndDraw();
		}
	}


	/*MSG message;
	while (GetMessage(&message, NULL, 0, 0)) {
		DispatchMessage(&message);
	}*/

	delete graphics;

	return 0;
}

