#include "cluster.h"

Cluster createCluster(float two, float three, float assists, float blocks, float fouls, PtList list) {
	Cluster newCluster;
	newCluster.meanTwoPoints = two;
	newCluster.meanThreePoints = three;
	newCluster.meanAssists = assists;
	newCluster.meanBlocks = blocks;
	newCluster.meanFouls = fouls;
	newCluster.members = list;
	return newCluster;
}

void setValues(Cluster newCluster, float two, float three, float assists, float blocks, float fouls) {
	newCluster.meanTwoPoints = two;
	newCluster.meanThreePoints = three;
	newCluster.meanAssists = assists;
	newCluster.meanBlocks = blocks;
	newCluster.meanFouls = fouls;
}