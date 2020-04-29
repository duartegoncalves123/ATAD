#pragma once
#include "statistics.h"

typedef struct date {
	int day, month, year;
}Date;

typedef struct player {
	int id;
	char name[50];
	char team[50];
	Date birthDate;
	char gender;
	Statistics statistics;
}Player;

typedef Player* PtPlayer;

Player createPlayer(int id, char *name, char *team, Date birthdate, char gender);
Date createDate(int day, int month, int year);