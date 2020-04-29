#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "basket.h"
#include "statistics.h"
#include "commands.h"


Player createPlayer(int id, char *name, char *team, Date birthdate, char gender) {
	Player newPlayer;
	newPlayer.id = id;
	strcpy_s(newPlayer.name, sizeof(newPlayer.name), name);
	strcpy_s(newPlayer.team, sizeof(newPlayer.team), team);
	newPlayer.birthDate = birthdate;
	newPlayer.gender = gender;
	newPlayer.statistics = createStatistics(0, 0, 0, 0, 0, 0);
	return newPlayer;
}

Date createDate(int day, int month, int year) {
	Date newDate;
	newDate.day = day;
	newDate.month = month;
	newDate.year = year;
	return newDate;
}