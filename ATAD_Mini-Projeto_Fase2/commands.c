#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "basket.h"
#include "statistics.h"
#include "list.h"
#include "map.h"
#include "values.h"
#include "cluster.h"
#include <math.h>

char** split(char *str, int nFields, const char *delim) {
	char **tokens = malloc(sizeof(char*) * nFields);

	int index = 0;
	char *next_token = NULL;

	char *token = strtok_s(str, delim, &next_token);
	while (token) {
		tokens[index++] = token;
		token = strtok_s(NULL, delim, &next_token);
	}
	return tokens;
}

void importPlayersFromFile(char *filename1, char *filename2, PtList list) {
	FILE *fd;
	int err = fopen_s(&fd, filename1, "r");

	if (err != 0) {
		printf("Nao foi possivel abrir o ficheiro %s ...\n", filename1);
		getchar();
		return;
	}

	char nextline[1024];

	int countPlayers = 0;

	while (fgets(nextline, sizeof(nextline), fd)) {
		if (strlen(nextline) < 1) {
			continue;
		}
		char **tokens = split(nextline, 5, ";");

		int playerId = atoi(tokens[0]);
		int day, month, year;
		sscanf_s(tokens[3], "%d/%d/%d", &day, &month, &year);
		char playerGender = tokens[4][0];

		ListElem elem = createPlayer(playerId, tokens[1], tokens[2], createDate(day, month, year), playerGender);
		listAdd(list, countPlayers, elem);


		free(tokens);
		countPlayers++;
	}

	err = fopen_s(&fd, filename2, "r");

	if (err != 0) {
		printf("Nao foi possivel abrir o ficheiro %s ...\n", filename2);
		getchar();
		return;
	}

	nextline[1024];

	int countGames = 0;


	while (fgets(nextline, sizeof(nextline), fd)) {
		if (strlen(nextline) < 1) {
			continue;
		}
		char **tokens = split(nextline, 7, ";");

		int playerId = atoi(tokens[0]);
		int gameId = atoi(tokens[1]);
		float twoPoints = atof(tokens[2]);
		float threePoints = atof(tokens[3]);
		float assists = atof(tokens[4]);
		float fouls = atof(tokens[5]);
		float blocks = atof(tokens[6]);
		

		int size;
		listSize(list, &size);
		Player player;
		Player player2;
			for (int i = 0; i < size; i++) {
				listGet(list, i, &player2);
				listGet(list, i, &player);
				if (player.id == playerId) {
					player.statistics.twoPoints += twoPoints;
					player.statistics.threePoints += threePoints;
					player.statistics.assists += assists;
					player.statistics.fouls += fouls;
					player.statistics.blocks += blocks;
					player.statistics.gamesPlayed += 1;
					listSet(list, i, player, &player2);
				}
			}
		free(tokens);
		countGames++;
	}
	printf("\nForam lidos %d jogadores e %d informacao sobre jogos \n ", countPlayers, countGames);
	getchar();
	fclose(fd);
}

void clear(PtList list) {
	int size;
	listSize(list, &size);
	listDestroy(&list);
	printf("Foram apagados %d registos\n", size);
}

void show(PtList list) {
	int option = 0;
	printf("\nSHOW\n");
    PtList tempList = listCreate(300);
	int size;
	listSize(list, &size);
	for (int i = 0; i < size; i++) {
	    ListElem elem;
		listGet(list, i, &elem);
		listAdd(tempList, i, elem);
    }
	printf("%18s   %-22s %-18s %-11s %-7s %-5s %-1s %-1s %-1s\n", "Id", "Name", "Team", "BirthDate", "2Pts", "3Pts", "Fouls", "Assists", "Blocks");
	listPrint(tempList);
	free(tempList);
}

void sort(PtList list) {
	char option;
	printf("Choose the order : Name (n), Date of birth (d), Number of played games (j)\n");
	scanf_s("%c", &option, 1);

	PtList tempList = listCreate(300);
	int size;
	listSize(list, &size); //atribuir o size à lista temporária
	for (unsigned int i = 0; i < size; i++) {
		ListElem elem;
		listGet(list, i, &elem); //vai buscar o elemento
		listAdd(tempList, i, elem); //carrega a nova lista com todos os elementos da lista recebida por parâmetro
	}

	if (option == 'N' || option == 'n') {
		for (unsigned int i = 0; i < size; i++) {

			for (unsigned int j = i + 1; j < size; j++) {
				ListElem fElem;
				listGet(tempList, i, &fElem);
				ListElem sElem;
				listGet(tempList, j, &sElem);
				if (strcmp(fElem.name, sElem.name) > 0) {
					listSet(tempList, i, sElem, &fElem);
					listSet(tempList, j, fElem, &sElem);
				}
			}
		}
		printf("%18s   %-22s %-18s %-11s %-7s %-5s %-1s %-1s %-1s\n", "Id", "Name", "Team", "BirthDate", "2Pts", "3Pts", "Fouls", "Assists", "Blocks");
		listPrint(tempList);
		free(tempList);
	}
	else if (option == 'D' || option == 'd') {
		for (unsigned int i = 0; i < size; i++) {

			for (unsigned int j = i + 1; j < size; j++) {
				ListElem fElem;
				listGet(tempList, i, &fElem);
				ListElem sElem;
				listGet(tempList, j, &sElem);
				if (fElem.birthDate.year > sElem.birthDate.year || fElem.birthDate.year == sElem.birthDate.year && fElem.birthDate.month > sElem.birthDate.month || fElem.birthDate.year == sElem.birthDate.year && fElem.birthDate.month == sElem.birthDate.month && fElem.birthDate.day > sElem.birthDate.day) {
					listSet(tempList, i, sElem, &fElem);
					listSet(tempList, j, fElem, &sElem);
				}
				else if (fElem.birthDate.year == sElem.birthDate.year && fElem.birthDate.month == sElem.birthDate.month && fElem.birthDate.day == sElem.birthDate.day) {
					if (strcmp(fElem.name, sElem.name) > 0) {
						listSet(tempList, i, sElem, &fElem);
						listSet(tempList, j, fElem, &sElem);
					}
				}
			}
		}
		printf("%18s   %-22s %-18s %-11s %-7s %-5s %-1s %-1s %-1s\n", "Id", "Name", "Team", "BirthDate", "2Pts", "3Pts", "Fouls", "Assists", "Blocks");
		listPrint(tempList);
		free(tempList);
	}
	else if (option == 'J' || option == 'j') {
		for (unsigned int i = 0; i < size; i++) {


			for (unsigned int j = i + 1; j < size; j++) {
				ListElem fElem;
				listGet(tempList, i, &fElem);
				ListElem sElem;
				listGet(tempList, j, &sElem);
				if (fElem.statistics.gamesPlayed > sElem.statistics.gamesPlayed) {
					listSet(tempList, i, sElem, &fElem);
					listSet(tempList, j, fElem, &sElem);
				}
				else if (fElem.statistics.gamesPlayed == sElem.statistics.gamesPlayed) {

					if (strcmp(fElem.name, sElem.name) > 0) {
						listSet(tempList, i, sElem, &fElem);
						listSet(tempList, j, fElem, &sElem);
					}
				}
			}
		}
		printf("%18s   %-22s %-18s %-11s %-7s %-5s %-1s %-1s %-1s\n", "Id", "Name", "Team", "BirthDate", "2Pts", "3Pts", "Fouls", "Assists", "Blocks");
		listPrint(tempList);
		free(tempList);
	}
	getchar();
}

PtList averageStatistics(PtList players) {
	PtList tempList = listCreate(300);
	int size;
	listSize(players, &size);
	int position = 0;
	for (int i = 0; i < size; i++) {
		ListElem newElem;
		listGet(players, i, &newElem);
		if (newElem.statistics.gamesPlayed > 0) {
			newElem.statistics.twoPoints = newElem.statistics.twoPoints / (float)newElem.statistics.gamesPlayed;
			newElem.statistics.threePoints = newElem.statistics.threePoints / (float)newElem.statistics.gamesPlayed;
			newElem.statistics.assists = newElem.statistics.assists / (float)newElem.statistics.gamesPlayed;
			newElem.statistics.blocks = newElem.statistics.blocks / (float)newElem.statistics.gamesPlayed;
			newElem.statistics.fouls = newElem.statistics.fouls / (float)newElem.statistics.gamesPlayed;
			listAdd(tempList, position, newElem);
			position++;
		}
	}
	return tempList;
}

void avg(PtList list) {
	PtList tempList = averageStatistics(list);
	int size;
	listSize(tempList, &size);
	Player elem1, elem2;
	for (unsigned int i = 0; i < size; i++) {
		for (unsigned int j = 0; j < size - i - 1; j++) {
			listGet(tempList, j, &elem1);
			listGet(tempList, j + 1, &elem2);
			float avgMvp1 = (3 * elem1.statistics.threePoints) + (2 * elem1.statistics.twoPoints) + elem1.statistics.assists + (2 * elem1.statistics.blocks) - (3 * elem1.statistics.fouls);
			float avgMvp2 = (3 * elem2.statistics.threePoints) + (2 * elem2.statistics.twoPoints) + elem2.statistics.assists + (2 * elem2.statistics.blocks) - (3 * elem2.statistics.fouls);
			if (avgMvp1 < avgMvp2) {
				Player player1, player2;
				listSet(tempList, j, elem2, &player1);
				listSet(tempList, j + 1, player1, &player2);
			}
		}
	}
	printf("%18s   %-22s %-18s %-11s %-7s %-5s %-1s %-1s %-1s\n", "Id", "Name", "Team", "BirthDate", "2Pts", "3Pts", "Fouls", "Assists", "Blocks");
	listPrint(tempList);
	free(tempList);
}

PtList normalizeStatistics(PtList players) {
	PtList tempList = listCreate(300);
	ListElem firstElem;
	listGet(players, 0, &firstElem);
	float minTwoPoints = firstElem.statistics.twoPoints;
	float maxTwoPoints = firstElem.statistics.twoPoints;
	float minThreePoints = firstElem.statistics.threePoints;
	float maxThreePoints = firstElem.statistics.threePoints;
	float minAssists = firstElem.statistics.assists;
	float maxAssists = firstElem.statistics.assists;
	float minBlocks = firstElem.statistics.blocks;
	float maxBlocks = firstElem.statistics.blocks;
	float minFouls = firstElem.statistics.fouls;
	float maxFouls = firstElem.statistics.fouls;

	int size;
	listSize(players, &size);
	for (unsigned int i = 1; i < size; i++) {
		ListElem elem;
		listGet(players, i, &elem);
		if (elem.statistics.twoPoints < minTwoPoints) {
			minTwoPoints = elem.statistics.twoPoints;
		}
		else if (elem.statistics.twoPoints > maxTwoPoints) {
			maxTwoPoints = elem.statistics.twoPoints;
		}
		if (elem.statistics.threePoints < minThreePoints) {
			minThreePoints = elem.statistics.threePoints;
		}
		else if (elem.statistics.threePoints > maxThreePoints) {
			maxThreePoints = elem.statistics.threePoints;
		}
		if (elem.statistics.assists < minAssists) {
			minAssists = elem.statistics.assists;
		}
		else if (elem.statistics.assists > maxAssists) {
			maxAssists = elem.statistics.assists;
		}
		if (elem.statistics.blocks < minBlocks) {
			minBlocks = elem.statistics.blocks;
		}
		else if (elem.statistics.blocks > maxBlocks) {
			maxBlocks = elem.statistics.blocks;
		}
		if (elem.statistics.fouls < minFouls) {
			minFouls = elem.statistics.fouls;
		}
		else if (elem.statistics.fouls > maxFouls) {
			maxFouls = elem.statistics.fouls;
		}
	}

	int position = 0;
	for (unsigned int i = 0; i < size; i++) {
		ListElem newElem;
		listGet(players, i , &newElem);
		if (newElem.statistics.gamesPlayed > 0) {
			newElem.statistics.twoPoints = (float)((float)newElem.statistics.twoPoints - (float)minTwoPoints) / (float)((float)maxTwoPoints - (float)minTwoPoints);
			newElem.statistics.threePoints = (float)((float)newElem.statistics.threePoints - (float)minThreePoints) / (float)((float)maxThreePoints - (float)minThreePoints);
			newElem.statistics.assists = (float)((float)newElem.statistics.assists - (float)minAssists) / (float)((float)maxAssists - (float)minAssists);
			newElem.statistics.blocks = (float)((float)newElem.statistics.blocks - (float)minBlocks) / (float)((float)maxBlocks - (float)minBlocks);
			newElem.statistics.fouls = (float)((float)newElem.statistics.assists - (float)minAssists) / (float)((float)maxAssists - (float)minAssists);

			listAdd(tempList, position, newElem);
			position++;
		}
	}
	return tempList;
}

void norm(PtList list) {
	PtList tempList = listCreate(300);
	tempList = normalizeStatistics(list);
	int size;
	listSize(tempList, &size);
	printf("%18s   %-22s %-18s %-11s %-7s %-5s %-1s %-1s %-1s\n", "Id", "Name", "Team", "BirthDate", "2Pts", "3Pts", "Fouls", "Assists", "Blocks");
	listPrint(tempList);
	free(tempList);
}

void type(PtList list) {
	PtList tempList = listCreate(300);
	tempList = averageStatistics(list);
	PtList shootGuards = listCreate(300);
	PtList pointGuards = listCreate(300);
	PtList allStars = listCreate(300);
	int size;
	listSize(tempList, &size);
	float amountOfTwoPoints = 0;
	float amountOfThreePoints = 0;
	float amountOfAssists = 0;
	float amountOfBlocks = 0;
	for (int i = 0; i < size; i++) {
		ListElem elem;
		listGet(tempList, i, &elem);
		amountOfTwoPoints += (float)elem.statistics.twoPoints;
		amountOfThreePoints += (float)elem.statistics.threePoints;
		amountOfAssists += (float)elem.statistics.assists;
		amountOfBlocks += (float)elem.statistics.blocks;
	}
	float averageTwoPoints = (float)amountOfTwoPoints / (float)size;
	float averageThreePoints = (float)amountOfThreePoints / (float)size;
	float averageAssists = (float)amountOfAssists / (float)size;
	float averageBlocks = (float)amountOfBlocks / (float)size;

	int position1 = 0;
	int position2 = 0;
	int position3 = 0;
	for (int i = 0; i < size; i++) {
		ListElem elem;
		listGet(tempList, i, &elem);
		if (elem.statistics.twoPoints > averageTwoPoints && elem.statistics.threePoints > averageThreePoints && elem.statistics.assists < averageAssists && elem.statistics.blocks < averageBlocks) {
			listAdd(shootGuards, position1, elem);
			position1++;
		}
		else if (elem.statistics.twoPoints < averageTwoPoints && elem.statistics.threePoints < averageThreePoints && elem.statistics.assists > averageAssists && elem.statistics.blocks > averageBlocks) {
			listAdd(pointGuards, position2, elem);
			position2++;
		}
		else if (elem.statistics.twoPoints > averageTwoPoints && elem.statistics.threePoints > averageThreePoints && elem.statistics.assists > averageAssists && elem.statistics.blocks > averageBlocks) {
			listAdd(allStars, position3, elem);
			position3++;
		}
	}

	printf("Tipo Shooting-Guard\n");
	printf("%18s   %-22s %-18s %-11s %-7s %-5s %-1s %-1s %-1s\n", "Id", "Name", "Team", "BirthDate", "2Pts", "3Pts", "Fouls", "Assists", "Blocks");
	listPrint(shootGuards);

	printf("Tipo Point-Guard\n");
	printf("%18s   %-22s %-18s %-11s %-7s %-5s %-1s %-1s %-1s\n", "Id", "Name", "Team", "BirthDate", "2Pts", "3Pts", "Fouls", "Assists", "Blocks");
	listPrint(pointGuards);

	printf("Tipo All-Star\n");
	printf("%18s   %-22s %-18s %-11s %-7s %-5s %-1s %-1s %-1s\n", "Id", "Name", "Team", "BirthDate", "2Pts", "3Pts", "Fouls", "Assists", "Blocks");
	listPrint(allStars);
	free(tempList);
}

void checkType(PtList players) {
	PtList tempList = averageStatistics(players);
	PtList shootGuards = listCreate(300);
	PtList pointGuards = listCreate(300);
	PtList allStars = listCreate(300);
	int size;
	listSize(tempList, &size);
	int amountOfTwoPoints = 0;
	int amountOfThreePoints = 0;
	int amountOfAssists = 0;
	int amountOfBlocks = 0;
	int amountOfFouls = 0;
	for (int i = 0; i < size; i++) {
		ListElem elem;
		listGet(tempList, i, &elem);
		amountOfTwoPoints += (int)elem.statistics.twoPoints;
		amountOfThreePoints += (int)elem.statistics.threePoints;
		amountOfAssists += (int)elem.statistics.assists;
		amountOfBlocks += (int)elem.statistics.blocks;
		amountOfFouls += (int)elem.statistics.fouls;
	}
	float averageTwoPoints = (float)amountOfTwoPoints / (float)size;
	float averageThreePoints = (float)amountOfThreePoints / (float)size;
	float averageAssists = (float)amountOfAssists / (float)size;
	float averageBlocks = (float)amountOfBlocks / (float)size;
	float averageFouls = (float)amountOfFouls / (float)size;

	int position1 = 0;
	int position2 = 0;
	int position3 = 0;
	for (int i = 0; i < size; i++) {
		ListElem elem;
		listGet(tempList, i, &elem);
		if (elem.statistics.twoPoints > averageTwoPoints && elem.statistics.threePoints > averageThreePoints && elem.statistics.assists < averageAssists && elem.statistics.blocks < averageBlocks) {
			listAdd(shootGuards, position1, elem);
			position1++;
		}
		else if (elem.statistics.twoPoints < averageTwoPoints && elem.statistics.threePoints < averageThreePoints && elem.statistics.assists > averageAssists && elem.statistics.blocks > averageBlocks) {
			listAdd(pointGuards, position2, elem);
			position2++;
		}
		else if (elem.statistics.twoPoints > averageTwoPoints && elem.statistics.threePoints > averageThreePoints && elem.statistics.assists > averageAssists && elem.statistics.blocks > averageBlocks) {
			listAdd(allStars, position3, elem);
			position3++;
		}
	}

	PtMap map = mapCreate(size);
	for (int i = 0; i < size; i++) {
		Values value;
		ListElem elem;
		listGet(tempList, i, &elem);
		for (int j = 0; j < size; j++) {
			ListElem shootingGuard;
			ListElem pointGuard;
			ListElem allStar;
			listGet(shootGuards, j, &shootingGuard);
			listGet(pointGuards, j, &pointGuard);
			listGet(allStars, j, &allStar);
			if (elem.id == shootingGuard.id) {
				value = createValue("shooting-guard", elem.statistics.twoPoints, averageTwoPoints, elem.statistics.threePoints, averageThreePoints, elem.statistics.assists, averageAssists, elem.statistics.blocks, averageBlocks, elem.statistics.fouls, averageFouls);
			}
			else if (elem.id == pointGuard.id) {
				value = createValue("point-guard", elem.statistics.twoPoints, averageTwoPoints, elem.statistics.threePoints, averageThreePoints, elem.statistics.assists, averageAssists, elem.statistics.blocks, averageBlocks, elem.statistics.fouls, averageFouls);
			}
			else if (elem.id == allStar.id) {
				value = createValue("all-star", elem.statistics.twoPoints, averageTwoPoints, elem.statistics.threePoints, averageThreePoints, elem.statistics.assists, averageAssists, elem.statistics.blocks, averageBlocks, elem.statistics.fouls, averageFouls);
			}
		}
		mapPut(map, elem.id, value);
	}

	printf("                 Id  Name                  Team                BirthDate   2Pts  3Pts Fouls Assists Blocks\n");
	for (int i = 0; i < size; i++) {
		ListElem elem;
		listGet(tempList, i, &elem);
		printf("At rank %d:   key = %d\n", i, i + 1);
		Values valueToPrint;
		mapGet(map, i + 1, &valueToPrint);
		mapValuePrint(valueToPrint);
	}

	getchar();
	free(tempList);
}

void kmeans(PtList list1, int k, int maxIt, float deltaError) {
	PtList list = normalizeStatistics(list1);
	PtList clusterToPrint1 = listCreate(300);
	PtList clusterToPrint2 = listCreate(300);
	PtList clusterToPrint3 = listCreate(300);
	int numberOfClusters = 0;
	int maxIterations = 0;
	if (k > 2) { numberOfClusters = k; }
	if (maxIt > 1) { maxIterations = maxIt; }

	int size;
	listSize(list, &size);

	ListElem elem1;
	listGet(list, 3, &elem1);
	ListElem elem2;
	listGet(list, 5, &elem2);
	ListElem elem3;
	listGet(list, 6, &elem3);

	Cluster cluster1 = createCluster(elem1.statistics.twoPoints, elem1.statistics.threePoints, elem1.statistics.assists, elem1.statistics.blocks, elem1.statistics.fouls, list);
	Cluster cluster2 = createCluster(elem2.statistics.twoPoints, elem2.statistics.threePoints, elem2.statistics.assists, elem2.statistics.blocks, elem2.statistics.fouls, list);
	Cluster cluster3 = createCluster(elem3.statistics.twoPoints, elem3.statistics.threePoints, elem3.statistics.assists, elem3.statistics.blocks, elem3.statistics.fouls, list);

	int iterationNumber = 1;
	float prevError = 0;
	float iterationError = 0;

	ListElem first1;
	listGet(cluster1.members, 0, &first1);
	float distance1 = sqrt((pow((cluster1.meanTwoPoints - first1.statistics.twoPoints), 2)) + (pow((cluster1.meanThreePoints - first1.statistics.threePoints), 2)) + (pow((cluster1.meanAssists - first1.statistics.assists), 2)) + (pow((cluster1.meanBlocks - first1.statistics.blocks), 2)) + (pow((cluster1.meanFouls - first1.statistics.fouls), 2)));
	ListElem shortestDistance1 = first1;

	ListElem first2;
	listGet(cluster2.members, 0, &first2);
	float distance2 = sqrt((pow((cluster2.meanTwoPoints - first2.statistics.twoPoints), 2)) + (pow((cluster2.meanThreePoints - first2.statistics.threePoints), 2)) + (pow((cluster2.meanAssists - first2.statistics.assists), 2)) + (pow((cluster2.meanBlocks - first2.statistics.blocks), 2)) + (pow((cluster2.meanFouls - first2.statistics.fouls), 2)));
	ListElem shortestDistance2 = first2;

	ListElem first3;
	listGet(cluster3.members, 0, &first3);
	float distance3 = sqrt((pow((cluster3.meanTwoPoints - first3.statistics.twoPoints), 2)) + (pow((cluster3.meanThreePoints - first3.statistics.threePoints), 2)) + (pow((cluster3.meanAssists - first3.statistics.assists), 2)) + (pow((cluster3.meanBlocks - first3.statistics.blocks), 2)) + (pow((cluster3.meanFouls - first3.statistics.fouls), 2)));
	ListElem shortestDistance3 = first3;

	int position = 0;
	while ((iterationNumber < maxIterations) && abs(prevError - iterationError) > deltaError) {
		if (iterationNumber > 1) {
			prevError = iterationError;
		}

		setValues(cluster1, shortestDistance1.statistics.twoPoints, shortestDistance1.statistics.threePoints, shortestDistance1.statistics.assists, shortestDistance1.statistics.blocks, shortestDistance1.statistics.fouls);
		setValues(cluster2, shortestDistance2.statistics.twoPoints, shortestDistance2.statistics.threePoints, shortestDistance2.statistics.assists, shortestDistance2.statistics.blocks, shortestDistance2.statistics.fouls);
		setValues(cluster3, shortestDistance3.statistics.twoPoints, shortestDistance3.statistics.threePoints, shortestDistance3.statistics.assists, shortestDistance3.statistics.blocks, shortestDistance3.statistics.fouls);

		float error = 0;
		for (unsigned int i = 1; i < size; i++) {
			ListElem cl1;
			listGet(cluster1.members, i, &cl1);
			float tempDistance1 = sqrt((pow((cluster1.meanTwoPoints - cl1.statistics.twoPoints), 2)) + (pow((cluster1.meanThreePoints - cl1.statistics.threePoints), 2)) + (pow((cluster1.meanAssists - cl1.statistics.assists), 2)) + (pow((cluster1.meanBlocks - cl1.statistics.blocks), 2)) + (pow((cluster1.meanFouls - cl1.statistics.fouls), 2)));
			if (tempDistance1 < distance1) {
				distance1 = tempDistance1;
				error += distance1;
				shortestDistance1 = cl1;
			}

			ListElem cl2;
			listGet(cluster2.members, i, &cl2);
			float tempDistance2 = sqrt((pow((cluster2.meanTwoPoints - cl2.statistics.twoPoints), 2)) + (pow((cluster2.meanThreePoints - cl2.statistics.threePoints), 2)) + (pow((cluster2.meanAssists - cl2.statistics.assists), 2)) + (pow((cluster2.meanBlocks - cl2.statistics.blocks), 2)) + (pow((cluster2.meanFouls - cl2.statistics.fouls), 2)));
			if (tempDistance2 < distance2) {
				distance2 = tempDistance2;
				error += distance2;
				shortestDistance2 = cl2;
			}

			ListElem cl3;
			listGet(cluster3.members, i, &cl3);
			float tempDistance3 = sqrt((pow((cluster3.meanTwoPoints - cl3.statistics.twoPoints), 2)) + (pow((cluster3.meanThreePoints - cl3.statistics.threePoints), 2)) + (pow((cluster3.meanAssists - cl3.statistics.assists), 2)) + (pow((cluster3.meanBlocks - cl3.statistics.blocks), 2)) + (pow((cluster3.meanFouls - cl3.statistics.fouls), 2)));
			if (tempDistance3 < distance3) {
				distance3 = tempDistance3;
				error += distance3;
				shortestDistance3 = cl3;
			}
		}
		

		listAdd(clusterToPrint1, position, shortestDistance1);
		listAdd(clusterToPrint2, position, shortestDistance2);
		listAdd(clusterToPrint3, position, shortestDistance3);

		position++;

		iterationError = error;
		iterationNumber++;
	}

	printf("%18s   %-22s %-18s %-11s %-7s %-5s %-1s %-1s %-1s\n", "Id", "Name", "Team", "BirthDate", "2Pts", "3Pts", "Fouls", "Assists", "Blocks");
	listPrint(clusterToPrint1);

	printf("%18s   %-22s %-18s %-11s %-7s %-5s %-1s %-1s %-1s\n", "Id", "Name", "Team", "BirthDate", "2Pts", "3Pts", "Fouls", "Assists", "Blocks");
	listPrint(clusterToPrint2);

	printf("%18s   %-22s %-18s %-11s %-7s %-5s %-1s %-1s %-1s\n", "Id", "Name", "Team", "BirthDate", "2Pts", "3Pts", "Fouls", "Assists", "Blocks");
	listPrint(clusterToPrint3);
}