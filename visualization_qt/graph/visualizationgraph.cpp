#include "visualizationgraph.h"

VisualizationGraph::VisualizationGraph(map::Graph graph, QGraphicsItem* parent)
    : QGraphicsRectItem(parent), graph(graph) {
}
