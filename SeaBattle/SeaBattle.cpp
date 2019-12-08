// SeaBattle.cpp : Defines the entry point for the console application.
//

//A programming language C. First course
//Game Sea Battle
// Implemented:
// - shaped randoms of the ships
// - turning around, moving the ships
// - the logic of finishing the ships 


#include "stdafx.h"

enum Pole { empty, target, point, ship, kill };//Стан поля
enum palubShip { four = 8 };//к-сть кординат для 4 палубого корабля

struct position_Kursor
{
	short x;
	short y;
};

void zastavka();
void showPole(char arr[][31], const int row, const int col);
void paint(position_Kursor &pos_Kur, short arrLogikPlayr, bool playrCompForPaint, short colir);
void color(short colir);
void allPaint(position_Kursor &pos_Kur, short arrLogikPlayr[][10], const int size, bool playrCompForPaint, short colir);
void killShip(short arrLogikPlayr[][10], short size, short &yComp, short &xComp, short yCompTemp, short xCompTemp);
short cleenPole(short arrLogikPlayr[][10], short size);
short WhatItShip(short arrLogikPlayr[][10], short size, short yPlayr, short xPlayr, short TempPalubShip[]);
short ramkaPoint(short arrLogikPlayr[][10], short size, short yPlayr, short xPlayr, short TempPalubShip[]);
short virtushkaShip(short arrLogikPlayr[][10], short size, short yPlayr, short xPlayr, short TempPalubShip[]);
short rightLeftMoveShip(short arrLogikPlayr[][10], short size, short yPlayr, short &xPlayr, short TempPalubShip[], int right);
short upDownMoveShip(short arrLogikPlayr[][10], short size, short &yPlayr, short xPlayr, short TempPalubShip[], int up);
short randShips(short arrLogikPlayr[][10], short size);
bool chak_for_Gorizont(short arrLogikPlayr[][10], short size, short randY, short randX, int j);
bool chak_for_Vertikal(short arrLogikPlayr[][10], short size, short randY, short randX, int j);
bool chackKillShip(short arrLogikPlayr[][10], short size, short yPlayr, short xPlayr, short TempPalubShip[]);
bool chackForWin(short arrLogikPlayr[][10], short size);
short putiShip(short arrLogikPlayr[][10], short size, short randY, short randX);
short cleenOneShip(short arrLogikPlayr[][10], short size, short TempPalubShip[], short y);
short menuFunction(char arrMenu[][50], const int size, short temp, int user_action);

void main()
{
	system("mode con cols=80 lines=35");
	srand(time_t(NULL));

	const int row = 21;
	const int col = 31;
	const int size = 10;
	const short five = 5;

	enum action { up = 72, down = 80, right = 77, left = 75 };//для прицела
	enum ranking { fight = 13, randShip = 32, turnShip = 122 };//для короблів дії

	char arr[row][col];//Ігрове поле
	char arrMenu[3][50];
	short arrLogikPlayr[size][size] = { { 0 } ,{ 0 } };//Поле логікі ігрок
	short arrLogikComp[size][size] = { { 0 } ,{ 0 } };//Поле логікі Comp
	short TempPalubShip[palubShip::four];//зберігаєм кординати корабл

	short xPlayr = 0, yPlayr = 0;
	short xComp = 0, yComp = 0;
	short xCompTemp = 0, yCompTemp = 0;
	short temp = 0;
	int user_action = 0;

	bool takeShip = false;
	bool battle = false;
	bool hitting = false;
	bool smashShip = false; //для добивання корабля(комп)
	bool playrCompForPaint = false; //false - paint Playr;  true - paint Comp
	bool whoWinInBattle = false;

	position_Kursor pos_Kur;

	strcpy_s(arrMenu[0], "New Game");
	strcpy_s(arrMenu[1], "Rule");
	strcpy_s(arrMenu[2], "Exit");

	while (true) //ALL WHILE GAME
	{

		while (user_action != ranking::fight)//Menu Game
		{
			zastavka();
			cout << endl << endl;
			for (int i = 0; i < 3; i++)
			{
				if (temp == i)color(9);
				cout << "\t\t\t\t" << arrMenu[i] << endl << endl;
				color(15);
			}
			cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << "\t\t\t\t\t\t   Copyright company \"Lamaster\"";

			user_action = _getch();
			temp = menuFunction(arrMenu, 3, temp, user_action);

		}//Menu Game


		if (temp == 2)
			exit(0);

		if (temp == 1)
		{
			cout << "\tRULE OF GAME" << endl;
			cout << " 1.Key(Z): Turn ship" << endl;
			cout << " 2.Key(X): Take your ship and you can move" << endl;
			cout << " 3.Key(Space): Rundom your ships or Shot to computer ships" << endl;
			cout << " 4.Key(Escape): Go to back" << endl;
			cout << " 5.Key(Enter): Star game" << endl;

			user_action = _getch();
		}


		while (battle == false) //ALL WHILE BATTLE
		{
			system("cls");
			temp = 0;
			arrLogikPlayr[0][0] = Pole::target;
			showPole(arr, row, col);
			randShips(arrLogikPlayr, size);

			while (battle == false)
			{
				allPaint(pos_Kur, arrLogikPlayr, size, playrCompForPaint, 0);

				user_action = _getch();

				if (user_action == 27 && takeShip == false)//Escepe
				{
					battle = true;
					break;
				}

				if (takeShip == true)
					WhatItShip(arrLogikPlayr, size, yPlayr, xPlayr, TempPalubShip);

				if (user_action == 120 && temp == Pole::ship)//TAKE Ship
				{
					cout << "TAKE Ship\n";
					takeShip = true;
				}

				if (user_action == 99 && takeShip == true)//PUT Ship
				{
					cout << "PUTE Ship\n";
					takeShip = false;
				}


				if (user_action == 224)
				{
					user_action = _getch();

					switch (user_action)
					{
					case action::up:
					{
						if (yPlayr - 1 == -1)break;
						if (takeShip == true)
							upDownMoveShip(arrLogikPlayr, size, yPlayr, xPlayr, TempPalubShip, action::up);
						else
						{
							yPlayr--;
							arrLogikPlayr[yPlayr + 1][xPlayr] = temp;
							temp = arrLogikPlayr[yPlayr][xPlayr];
							arrLogikPlayr[yPlayr][xPlayr] = Pole::target;
						}

						break;
					}
					case action::left:
					{
						if (xPlayr - 1 == -1)break;
						if (takeShip == true)
							rightLeftMoveShip(arrLogikPlayr, size, yPlayr, xPlayr, TempPalubShip, action::left);
						else
						{
							xPlayr--;
							arrLogikPlayr[yPlayr][xPlayr + 1] = temp;
							temp = arrLogikPlayr[yPlayr][xPlayr];
							arrLogikPlayr[yPlayr][xPlayr] = Pole::target;
						}

						break;
					}
					case action::down:
					{
						if (yPlayr + 1 == size)break;
						if (takeShip == true)
							upDownMoveShip(arrLogikPlayr, size, yPlayr, xPlayr, TempPalubShip, action::down);
						else
						{
							yPlayr++;
							arrLogikPlayr[yPlayr - 1][xPlayr] = temp;
							temp = arrLogikPlayr[yPlayr][xPlayr];
							arrLogikPlayr[yPlayr][xPlayr] = Pole::target;
						}

						break;
					}
					case action::right:
					{
						if (xPlayr + 1 == size)break;
						if (takeShip == true)
							rightLeftMoveShip(arrLogikPlayr, size, yPlayr, xPlayr, TempPalubShip, action::right);
						else
						{
							xPlayr++;
							arrLogikPlayr[yPlayr][xPlayr - 1] = temp;
							temp = arrLogikPlayr[yPlayr][xPlayr];
							arrLogikPlayr[yPlayr][xPlayr] = Pole::target;
						}

						break;
					}

					}

				}//if 224

				switch (user_action)//Fight, RandorShip, TurnShip
				{
				case ranking::fight:
				{
					if (takeShip == false && temp != Pole::ship)battle = true;
					break;
				}

				case ranking::randShip:
				{
					if (temp == Pole::ship || takeShip == true)break;

					cleenPole(arrLogikPlayr, size);//Очіщаєм поле Логікі
					randShips(arrLogikPlayr, size);

					break;
				}

				case ranking::turnShip:
				{
					if (temp == Pole::ship)
						virtushkaShip(arrLogikPlayr, size, yPlayr, xPlayr, TempPalubShip);

					break;
				}


				}//switch FOR_SHIP

			}//while for Playar

			arrLogikComp[0][0] = Pole::target;
			arrLogikPlayr[yPlayr][xPlayr] = Pole::empty;
			allPaint(pos_Kur, arrLogikPlayr, size, false, 0);

			randShips(arrLogikComp, size);

			xPlayr = 0, yPlayr = 0;

			while (battle == true)//Battle
			{
				if (user_action == 27)//Escape
				{
					battle = false;
					break;
				}
				allPaint(pos_Kur, arrLogikComp, size, true, 0);

				user_action = _getch();

				if (user_action == 224)
				{
					user_action = _getch();
					switch (user_action)
					{
					case action::up:
					{
						if (yPlayr - 1 == -1)break;
						yPlayr--;
						arrLogikComp[yPlayr + 1][xPlayr] = temp;
						temp = arrLogikComp[yPlayr][xPlayr];
						arrLogikComp[yPlayr][xPlayr] = Pole::target;
						break;
					}
					case action::left:
					{
						if (xPlayr - 1 == -1)break;
						xPlayr--;
						arrLogikComp[yPlayr][xPlayr + 1] = temp;
						temp = arrLogikComp[yPlayr][xPlayr];
						arrLogikComp[yPlayr][xPlayr] = Pole::target;
						break;
					}
					case action::down:
					{
						if (yPlayr + 1 == size)break;
						yPlayr++;
						arrLogikComp[yPlayr - 1][xPlayr] = temp;
						temp = arrLogikComp[yPlayr][xPlayr];
						arrLogikComp[yPlayr][xPlayr] = Pole::target;
						break;
					}
					case action::right:
					{
						if (xPlayr + 1 == size)break;
						xPlayr++;
						arrLogikComp[yPlayr][xPlayr - 1] = temp;
						temp = arrLogikComp[yPlayr][xPlayr];
						arrLogikComp[yPlayr][xPlayr] = Pole::target;
						break;
					}


					}

				}//if 224

				if (user_action == ranking::randShip)
				{
					if (temp == Pole::kill || temp == Pole::point)
					{
						hitting = true;
						continue;
					}

					if (temp == Pole::ship)
						arrLogikComp[yPlayr][xPlayr] = Pole::kill;
					if (temp == Pole::empty)
						arrLogikComp[yPlayr][xPlayr] = Pole::point;

					if (temp == Pole::ship)hitting = true;
					else hitting = false;

					temp = arrLogikComp[yPlayr][xPlayr];

					if (arrLogikComp[yPlayr][xPlayr] == Pole::kill)
						ramkaPoint(arrLogikComp, size, yPlayr, xPlayr, TempPalubShip);

					whoWinInBattle = chackForWin(arrLogikComp, size);
					if (whoWinInBattle == true)
					{
						allPaint(pos_Kur, arrLogikPlayr, size, true, 0);
						cout << "\t\t\t\tYou Win.\n";
						break;
					}

				}

				if (hitting == false && user_action == ranking::randShip)//Ход Компа
				{
					do
					{
						do
						{
							if (smashShip == false)
							{
								yComp = rand() % 10;
								xComp = rand() % 10;
								yCompTemp = yComp;
								xCompTemp = xComp;
							}
							else break;

						} while (arrLogikPlayr[yComp][xComp] == Pole::point);

						if (smashShip == true)//Добівка корабля
							killShip(arrLogikPlayr, size, yComp, xComp, yCompTemp, xCompTemp);

						if (arrLogikPlayr[yComp][xComp] == Pole::empty)
							arrLogikPlayr[yComp][xComp] = Pole::point;

						if (arrLogikPlayr[yComp][xComp] == Pole::ship)
						{
							smashShip = true;
							arrLogikPlayr[yComp][xComp] = Pole::kill;

							smashShip = chackKillShip(arrLogikPlayr, size, yCompTemp, xCompTemp, TempPalubShip);//Первірка на добивання корабля
							ramkaPoint(arrLogikPlayr, size, yComp, xComp, TempPalubShip);
						}

						allPaint(pos_Kur, arrLogikPlayr, size, false, 0);
						whoWinInBattle = chackForWin(arrLogikPlayr, size);
						if (whoWinInBattle == true)
						{
							allPaint(pos_Kur, arrLogikPlayr, size, false, 0);
							cout << "\t\t\t\tYou Lost.\n";
							break;
						}

					} while (arrLogikPlayr[yComp][xComp] == Pole::kill);

				}
				hitting = false;

				if (whoWinInBattle == true)break;
			}//while(battle)

			 //#####################################################################Відновлюєм настройкі
			for (int i = 0; i < size; i++)
				for (int j = 0; j < size; j++)
				{
					arrLogikPlayr[i][j] = 0;
					arrLogikComp[i][j] = 0;
				}
			xPlayr = 0, yPlayr = 0, temp = 0;
			takeShip = false, battle = false, hitting = false, playrCompForPaint = false, smashShip = false;
			//#####################################################################Відновлюєм настройкі
			if (whoWinInBattle == true)
			{
				cout << "If you wont Continue Game puts ENTER. Else puts other key for Exit.\n";
				user_action = _getch();
				if (user_action == 13)
					battle = false;
				else
					break;

				whoWinInBattle = false;
			}

			if (user_action == 27)break;

		}//ALL WHILE BATTLE
	}//ALL WHILE GAME
}

void color(short colir)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, colir);
}

void showPole(char arr[][31], const int row, const int col)
{
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			arr[i][j] = char(197);//риска + -образна

	for (int i = 0; i < row; i++)
	{
		if (i % 2 == 0)//риска Т-образна ліва,права сторона
		{
			arr[i][0] = char(195);
			arr[i][30] = char(180);
		}

		for (int i = 0; i < col; i += 3)//риска Т-образна верх,низ сторона
		{
			arr[0][i] = char(194);
			arr[20][i] = char(193);
		}

		if (i % 2 == 0)//риска горизонтальна
			for (int j = 1; j < col; j += 3)
			{
				arr[i][j] = char(196);
				arr[i][j + 1] = char(196);
			}

		if (i % 2 != 0)//риска вертикальна
			for (int j = 0; j < col; j += 3)
				arr[i][j] = char(179);


		if (i % 2 != 0)//пуста клетка
			for (int j = 1; j < col; j += 3)
			{
				arr[i][j] = char(0);
				arr[i][j + 1] = char(0);
			}

	}
	arr[0][0] = char(218);
	arr[0][30] = char(191);
	arr[20][0] = char(192);
	arr[20][30] = char(217);

	color(1);
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
			cout << arr[i][j];

		cout << "\t\t";

		for (int j = 0; j < col; j++)
			cout << arr[i][j];

		cout << endl;
	}
	color(15);

}

short menuFunction(char arrMenu[][50], const int size, short temp, int user_action)
{
	system("cls");
	if (user_action == 224)
	{
		user_action = _getch();
		switch (user_action)
		{
		case 72:
		{
			temp--;
			if (temp < 0)temp = size - 1;
			break;
		}
		case 80:
		{
			temp++;
			if (temp > size - 1)temp = 0;
			break;
		}
		}
	}

	return temp;
}

short cleenPole(short arrLogikPlayr[][10], short size)
{
	for (int i = 0; i < size; i++)//занулили логік масив крім таргет
		for (int k = 0; k < size; k++)
		{
			if (arrLogikPlayr[i][k] == Pole::target)continue;

			arrLogikPlayr[i][k] = NULL;
		}

	return arrLogikPlayr[size][size];
}

void paint(position_Kursor &pos_Kur, short arrLogikPlayr, bool playrCompForPaint, short colir)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD c{ pos_Kur.x, pos_Kur.y };
	SetConsoleCursorPosition(h, c);

	if (arrLogikPlayr == Pole::empty)
		cout << char(0) << char(0);

	if (arrLogikPlayr == Pole::target)
	{
		color(4);
		cout << char(42);
		color(7);
	}


	if (arrLogikPlayr == Pole::point)
		cout << char(249);

	if (playrCompForPaint == false)
	{
		if (arrLogikPlayr == Pole::ship)
			cout << char(219) << char(219);
	}
	else
	{
		if (arrLogikPlayr == Pole::ship)
			cout << char(0) << char(0);//////////////////////////компютеру замал. кораблікі
	}

	if (arrLogikPlayr == Pole::kill)
	{
		color(4);
		cout << char(219) << char(219);
		color(7);
	}
}

void allPaint(position_Kursor &pos_Kur, short arrLogikPlayr[][10], const int size, bool playrCompForPaint, short colir)
{
	if (playrCompForPaint == false)
	{
		pos_Kur.x = 1;
		pos_Kur.y = 1;
	}
	else
	{
		pos_Kur.x = 41;
		pos_Kur.y = 1;
	}

	for (int i = 0; i < size; i++)
	{
		for (int k = 0; k < size; k++)
		{
			paint(pos_Kur, arrLogikPlayr[i][k], playrCompForPaint, 0);
			pos_Kur.x += 3;
		}
		if (playrCompForPaint == false)
			pos_Kur.x = 1;
		else
			pos_Kur.x = 41;

		pos_Kur.y += 2;
	}
	if (playrCompForPaint == false)
		pos_Kur.x = 1;
	else
		pos_Kur.x = 41;

	pos_Kur.y = 1;

	cout << endl << endl;
}

short WhatItShip(short arrLogikPlayr[][10], short size, short yPlayr, short xPlayr, short TempPalubShip[])
{
	short temp[palubShip::four];
	short counter = 0;

	for (int i = 0; i < palubShip::four; i++)
		temp[i] = -1;

	for (int j = xPlayr; j < size; j++)//ідем в право і шукаєм кораблік
	{
		if (arrLogikPlayr[yPlayr][j] == Pole::empty || arrLogikPlayr[yPlayr][j] == Pole::point || j == size || j == -1)
			break;

		temp[counter] = yPlayr;
		counter++;
		temp[counter] = j;
		counter++;
	}

	for (int j = yPlayr + 1; j < size; j++)//ідем в низ і шукаєм кораблік
	{
		if (arrLogikPlayr[j][xPlayr] == Pole::empty || arrLogikPlayr[j][xPlayr] == Pole::point || j == size || j == -1)
			break;

		temp[counter] = j;
		counter++;
		temp[counter] = xPlayr;
		counter++;
	}

	for (int j = xPlayr - 1; j >= 0; j--)//ідем в ліво і шукаєм кораблік
	{
		if (arrLogikPlayr[yPlayr][j] == Pole::empty || arrLogikPlayr[yPlayr][j] == Pole::point || j == size || j == -1)
			break;

		temp[counter] = yPlayr;
		counter++;
		temp[counter] = j;
		counter++;
	}

	for (int j = yPlayr - 1; j >= 0; j--)//ідем в вверх і шукаєм кораблік
	{
		if (arrLogikPlayr[j][xPlayr] == Pole::empty || arrLogikPlayr[j][xPlayr] == Pole::point || j == size || j == -1)
			break;

		temp[counter] = j;
		counter++;
		temp[counter] = xPlayr;
		counter++;
	}

	for (int i = 0; i < palubShip::four; i++)
		TempPalubShip[i] = temp[i];

	return TempPalubShip[palubShip::four];
}

void killShip(short arrLogikPlayr[][10], short size, short &yComp, short &xComp, short yCompTemp, short xCompTemp)
{
	short temp = 1;
	bool boolka = true;
	yComp = yCompTemp;
	xComp = xCompTemp;

	while (boolka == true)//right
	{
		if (xCompTemp + temp == size)break;
		if (arrLogikPlayr[yCompTemp][xCompTemp + temp] == Pole::point)break;
		if (arrLogikPlayr[yCompTemp][xCompTemp + temp] == Pole::empty || arrLogikPlayr[yCompTemp][xCompTemp + temp] == Pole::ship)
		{
			xComp += temp;
			boolka = false;
			break;
		}
		temp++;
	}
	temp = 1;

	while (boolka == true)//left
	{
		if (xCompTemp - temp == -1)break;
		if (arrLogikPlayr[yCompTemp][xCompTemp - temp] == Pole::point)break;
		if (arrLogikPlayr[yCompTemp][xCompTemp - temp] == Pole::empty || arrLogikPlayr[yCompTemp][xCompTemp - temp] == Pole::ship)
		{
			xComp -= temp;
			boolka = false;
			break;
		}
		temp++;
	}
	temp = 1;

	while (boolka == true)//up
	{
		if (yCompTemp - temp == -1)break;
		if (arrLogikPlayr[yCompTemp - temp][xCompTemp] == Pole::point)break;
		if (arrLogikPlayr[yCompTemp - temp][xCompTemp] == Pole::empty || arrLogikPlayr[yCompTemp - temp][xCompTemp] == Pole::ship)
		{
			yComp -= temp;
			boolka = false;
			break;
		}
		temp++;
	}
	temp = 1;

	while (boolka == true)//down
	{
		if (yCompTemp + temp == size)break;
		if (arrLogikPlayr[yCompTemp + temp][xCompTemp] == Pole::point)break;
		if (arrLogikPlayr[yCompTemp + temp][xCompTemp] == Pole::empty || arrLogikPlayr[yCompTemp + temp][xCompTemp] == Pole::ship)
		{
			yComp += temp;
			boolka = false;
			break;
		}
		temp++;
	}

}

short ramkaPoint(short arrLogikPlayr[][10], short size, short yPlayr, short xPlayr, short TempPalubShip[])
{
	WhatItShip(arrLogikPlayr, size, yPlayr, xPlayr, TempPalubShip);

	short counter = 0, y = 0, x = 0;
	bool chakShip = true;
	for (int i = 0; i < palubShip::four; i += 2)
	{
		if (TempPalubShip[i] == -1)break;
		counter++;
		y = TempPalubShip[i];
		x = TempPalubShip[i + 1];
		if (arrLogikPlayr[y][x] == Pole::ship)chakShip = false;
	}

	if (chakShip == true)
	{
		for (int i = 0; i < counter * 2; i += 2)
		{
			y = TempPalubShip[i];
			x = TempPalubShip[i + 1];
			for (int j = y - 1; j < y - 1 + 3; j++)
				for (int q = x - 1; q < x - 1 + 3; q++)
				{
					if (j == size || q == size || q == -1 || j == -1 || arrLogikPlayr[j][q] == Pole::point || arrLogikPlayr[j][q] == Pole::kill)continue;
					arrLogikPlayr[j][q] = Pole::point;
				}
		}

	}

	return arrLogikPlayr[size][size];
}

short virtushkaShip(short arrLogikPlayr[][10], short size, short yPlayr, short xPlayr, short TempPalubShip[])
{
	WhatItShip(arrLogikPlayr, size, yPlayr, xPlayr, TempPalubShip);

	short y = 0;
	bool chakGoruzont = true;
	bool chakVertukal = true;

	for (int i = 0; i < palubShip::four; i++)
	{
		if (TempPalubShip[i] < 0)break;
		y++;
	}

	chakGoruzont = chak_for_Gorizont(arrLogikPlayr, size, yPlayr - 1, xPlayr + 1, y / 2);
	if (TempPalubShip[0] == TempPalubShip[2])chakGoruzont = false;

	if (chakGoruzont == true)
	{
		cleenOneShip(arrLogikPlayr, size, TempPalubShip, y);

		for (int i = xPlayr; i < xPlayr + y / 2; i++)
			putiShip(arrLogikPlayr, size, yPlayr, i);

		return arrLogikPlayr[size][size];
	}

	chakVertukal = chak_for_Vertikal(arrLogikPlayr, size, yPlayr + 1, xPlayr - 1, y / 2);
	if (chakVertukal == true)
	{
		cleenOneShip(arrLogikPlayr, size, TempPalubShip, y);

		for (int i = yPlayr; i < yPlayr + y / 2; i++)
			putiShip(arrLogikPlayr, size, i, xPlayr);



		return arrLogikPlayr[size][size];
	}

	return arrLogikPlayr[size][size];
}

short cleenOneShip(short arrLogikPlayr[][10], short size, short TempPalubShip[], short y)
{
	short x = 0, q = 0;

	for (int i = 0; i < y; i += 2)
	{
		q = TempPalubShip[i];
		x = TempPalubShip[i + 1];
		arrLogikPlayr[q][x] = Pole::empty;
	}

	return arrLogikPlayr[size][size];
}

short randShips(short arrLogikPlayr[][10], short size)
{
	short randY = 0, randX = 0;

	for (int i = 0; i < 4; )
	{
		for (int j = 4 - i; j > 0; )
		{
			randY = rand() % 10;
			randX = rand() % 10;

			bool chakGoruzont = true;
			bool chakVertukal = true;

			chakGoruzont = chak_for_Gorizont(arrLogikPlayr, size, randY - 1, randX - 1, j + 2);
			if (chakGoruzont == true)
			{
				for (int i = randX; i < randX + j; i++)
					putiShip(arrLogikPlayr, size, randY, i);

				j--;
			}

			chakVertukal = chak_for_Vertikal(arrLogikPlayr, size, randY - 1, randX - 1, j + 2);
			if (chakGoruzont == false && chakVertukal == true)
			{
				for (int i = randY; i < randY + j; i++)
					putiShip(arrLogikPlayr, size, i, randX);

				j--;
			}

		}
		i++;

	}

	return arrLogikPlayr[size][size];
}

short rightLeftMoveShip(short arrLogikPlayr[][10], short size, short yPlayr, short &xPlayr, short TempPalubShip[], int right)
{
	short minX = TempPalubShip[1], maxX = TempPalubShip[1], minY = TempPalubShip[0], maxY = TempPalubShip[0], counter = 0;
	bool boolka = true, bobolk = true;

	for (int i = 1; i < palubShip::four; i += 2)
	{
		if (maxX < TempPalubShip[i])maxX = TempPalubShip[i];
		if (maxY < TempPalubShip[i - 1])maxY = TempPalubShip[i - 1];
		counter++;
		if (TempPalubShip[i + 2] == -1)break;
	}

	for (int i = 1; i < palubShip::four; i += 2)
	{
		if (minX > TempPalubShip[i])minX = TempPalubShip[i];
		if (minY > TempPalubShip[i - 1])minY = TempPalubShip[i - 1];
		if (TempPalubShip[i + 2] == -1)break;
	}

	if (right == 77)//Right //For Gorrizontal and Vertikal Ship
	{
		boolka = chak_for_Gorizont(arrLogikPlayr, size, yPlayr - 1, maxX + 1, 2);
		if (TempPalubShip[0] == TempPalubShip[2] || TempPalubShip[3] == -1)
			if (boolka == true)
			{
				cleenOneShip(arrLogikPlayr, size, TempPalubShip, counter * 2);
				for (int i = minX; i < maxX + 1; i++)
					putiShip(arrLogikPlayr, size, yPlayr, i + 1);

				xPlayr++;
				return arrLogikPlayr[size][size];
			}

		boolka = chak_for_Gorizont(arrLogikPlayr, size, minY - 1, maxX + 1, 2);
		bobolk = chak_for_Gorizont(arrLogikPlayr, size, maxY - 1, maxX + 1, 2);
		if (TempPalubShip[1] == TempPalubShip[3])
			if (boolka == true && bobolk == true)
			{
				cleenOneShip(arrLogikPlayr, size, TempPalubShip, counter * 2);
				for (int i = minY; i < maxY + 1; i++)
					putiShip(arrLogikPlayr, size, i, xPlayr + 1);

				xPlayr++;
				return arrLogikPlayr[size][size];
			}

	}

	boolka = true;
	bobolk = true;
	if (right == 75)//Left //For Gorrizontal and Vertikal Ship
	{
		boolka = chak_for_Gorizont(arrLogikPlayr, size, minY - 1, minX - 2, 2);
		if (minX == 0)boolka = false;
		if (TempPalubShip[0] == TempPalubShip[2] || TempPalubShip[3] == -1)
			if (boolka == true)
			{
				cleenOneShip(arrLogikPlayr, size, TempPalubShip, counter * 2);
				for (int i = minX; i < maxX + 1; i++)
					putiShip(arrLogikPlayr, size, yPlayr, i - 1);

				xPlayr--;
				return arrLogikPlayr[size][size];
			}

		boolka = chak_for_Gorizont(arrLogikPlayr, size, minY - 1, maxX - 2, 2);
		bobolk = chak_for_Gorizont(arrLogikPlayr, size, maxY - 1, maxX - 2, 2);
		if (TempPalubShip[1] == TempPalubShip[3])
			if (boolka == true && bobolk == true)
			{
				cleenOneShip(arrLogikPlayr, size, TempPalubShip, counter * 2);
				for (int i = minY; i < maxY + 1; i++)
					putiShip(arrLogikPlayr, size, i, xPlayr - 1);

				xPlayr--;
				return arrLogikPlayr[size][size];
			}

	}

	return arrLogikPlayr[size][size];
}

short upDownMoveShip(short arrLogikPlayr[][10], short size, short &yPlayr, short xPlayr, short TempPalubShip[], int up)
{
	short minX = TempPalubShip[1], maxX = TempPalubShip[1], minY = TempPalubShip[0], maxY = TempPalubShip[0], counter = 0;
	bool boolka = true, bobolk = true;

	for (int i = 1; i < palubShip::four; i += 2)
	{
		if (maxX < TempPalubShip[i])maxX = TempPalubShip[i];
		if (maxY < TempPalubShip[i - 1])maxY = TempPalubShip[i - 1];
		counter++;
		if (TempPalubShip[i + 2] == -1)break;
	}

	for (int i = 1; i < palubShip::four; i += 2)
	{
		if (minX > TempPalubShip[i])minX = TempPalubShip[i];
		if (minY > TempPalubShip[i - 1])minY = TempPalubShip[i - 1];
		if (TempPalubShip[i + 2] == -1)break;
	}

	if (up == 72)//UP //For Gorrizontal and Vertikal Ship
	{
		boolka = chak_for_Vertikal(arrLogikPlayr, size, minY - 2, minX - 1, 2);//Vertikal Ship
		if (TempPalubShip[1] == TempPalubShip[3])
			if (boolka == true)
			{
				cleenOneShip(arrLogikPlayr, size, TempPalubShip, counter * 2);
				for (int i = minY; i < maxY + 1; i++)
					putiShip(arrLogikPlayr, size, i - 1, minX);

				yPlayr--;
				return arrLogikPlayr[size][size];
			}

		boolka = chak_for_Vertikal(arrLogikPlayr, size, minY - 2, minX - 1, 2);
		bobolk = chak_for_Vertikal(arrLogikPlayr, size, minY - 2, maxX - 1, 2);
		if (TempPalubShip[0] == TempPalubShip[2] || TempPalubShip[3] == -1)// Gorizontal Ship
			if (boolka == true && bobolk == true)
			{
				cleenOneShip(arrLogikPlayr, size, TempPalubShip, counter * 2);
				for (int i = minX; i < maxX + 1; i++)
					putiShip(arrLogikPlayr, size, minY - 1, i);

				yPlayr--;
				return arrLogikPlayr[size][size];
			}

	}

	boolka = true;
	bobolk = true;
	if (up == 80)//DOWN //For Gorrizontal and Vertikal Ship
	{
		boolka = chak_for_Vertikal(arrLogikPlayr, size, maxY + 1, minX - 1, 2);//Vertikal Ship
		if (TempPalubShip[1] == TempPalubShip[3])
			if (boolka == true)
			{
				cleenOneShip(arrLogikPlayr, size, TempPalubShip, counter * 2);
				for (int i = minY; i < maxY + 1; i++)
					putiShip(arrLogikPlayr, size, i + 1, minX);

				yPlayr++;
				return arrLogikPlayr[size][size];
			}

		boolka = chak_for_Vertikal(arrLogikPlayr, size, minY + 1, minX - 1, 2);
		bobolk = chak_for_Vertikal(arrLogikPlayr, size, maxY + 1, maxX - 1, 2);
		if (TempPalubShip[0] == TempPalubShip[2] || TempPalubShip[3] == -1)// Gorizontal Ship
			if (boolka == true && bobolk == true)
			{
				cleenOneShip(arrLogikPlayr, size, TempPalubShip, counter * 2);
				for (int i = minX; i < maxX + 1; i++)
					putiShip(arrLogikPlayr, size, minY + 1, i);

				yPlayr++;
				return arrLogikPlayr[size][size];
			}

	}

	return arrLogikPlayr[size][size];
}

bool chak_for_Gorizont(short arrLogikPlayr[][10], short size, short randY, short randX, int j)
{
	bool boolka = true;
	for (int i = randX; i < randX + j; i++)
		for (int q = randY; q < randY + 3; q++)
		{
			if (i == size || q == size || q == -1 || i == -1)continue;
			if (arrLogikPlayr[q][i] != Pole::empty)boolka = false;
		}

	if (randX - 1 > size - j)boolka = false;

	return boolka;
}
bool chak_for_Vertikal(short arrLogikPlayr[][10], short size, short randY, short randX, int j)
{
	bool boolka = true;
	for (int q = randY; q < randY + j; q++)
		for (int i = randX; i < randX + 3; i++)
		{
			if (i == size || q == size || q == -1 || i == -1)continue;
			if (arrLogikPlayr[q][i] != Pole::empty)boolka = false;
		}

	if (randY - 1 > size - j)boolka = false;

	return boolka;
}
bool chackForWin(short arrLogikPlayr[][10], short size)
{
	short temp = 0;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (arrLogikPlayr[i][j] == Pole::ship)temp++;

	if (temp == 0)
		return true;
	else
		return false;
}
bool chackKillShip(short arrLogikPlayr[][10], short size, short yPlayr, short xPlayr, short TempPalubShip[])
{
	WhatItShip(arrLogikPlayr, size, yPlayr, xPlayr, TempPalubShip);

	short y = 0, x = 0;
	bool chakShip = false;
	for (int i = 0; i < palubShip::four; i += 2)
	{
		if (TempPalubShip[i] == -1)break;
		y = TempPalubShip[i];
		x = TempPalubShip[i + 1];
		if (arrLogikPlayr[y][x] == Pole::ship)chakShip = true;
	}

	return chakShip;
}

short putiShip(short arrLogikPlayr[][10], short size, short randY, short randX)
{
	arrLogikPlayr[randY][randX] = Pole::ship;
	return arrLogikPlayr[size][size];
}

void zastavka()
{
	char Ship[16][255];
	strcpy_s(Ship[0], " \t                              __\n");
	strcpy_s(Ship[1], " \t                             | -'\n");
	strcpy_s(Ship[2], " \t                             |\n");
	strcpy_s(Ship[3], " \t               p           _\\_/_           p\n");
	strcpy_s(Ship[4], " \t               |         .\'  |  \'.         |\n");
	strcpy_s(Ship[5], " \t               |       /     |    \\        |\n");
	strcpy_s(Ship[6], " \t              .T\\     /======+=====\\      /T.\n");
	strcpy_s(Ship[7], " \t            .\' | \\  /        |      \\    / | \'.\n");
	strcpy_s(Ship[8], " \t          .\'   |  \\/         |        \\/   |    \'.\n");
	strcpy_s(Ship[9], " \t        .\'     |   \\         |         \\   |      \'.\n");
	strcpy_s(Ship[10], "\t      .\'       |    \\        |          \\  |        \'.\n");
	strcpy_s(Ship[11], "\t    <_________ | _____>_____ | _________ > | _________ >\n");
	strcpy_s(Ship[12], "\t  (___________ | ___________ | ___________ | ___________)\n");
	strcpy_s(Ship[13], "\t  \\;;;Victory;;; o;;;;; o;;;;; o;;;;; o;;;;; o;;;;; o;;;/\n");
	strcpy_s(Ship[14], "\t ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	strcpy_s(Ship[15], "\t  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	for (int i = 0; i < 16; i++)
		cout << Ship[i];
}