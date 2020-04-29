#pragma once

#include "basket.h"
#include "statistics.h"
#include "list.h"

char** split(char *str, int nFields, const char *delim);
void importPlayersFromFile(char *filename1, char *filename2, PtList list);
void clear(PtList list);
void show(PtList list);
void sort(PtList);
PtList averageStatistics(PtList players);
void avg(PtList list);
PtList normalizeStatistics(PtList players);
void norm(PtList list);
void type(PtList list);
void checkType(PtList players);
void kmeans(PtList list, int k, int maxIterations, float deltaError);