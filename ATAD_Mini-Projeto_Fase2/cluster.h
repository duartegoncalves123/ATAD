#pragma once
#include "basket.h"
#include "list.h"

typedef struct cluster {
	float meanTwoPoints;
	float meanThreePoints;
	float meanAssists;
	float meanFouls;
	float meanBlocks;
	PtList members;
}Cluster;

Cluster createCluster(float two, float three, float assists, float blocks, float fouls, PtList list);
void setValues(Cluster newCluster, float two, float three, float assists, float blocks, float fouls);