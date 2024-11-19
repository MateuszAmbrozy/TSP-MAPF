
#include "taskinputwidget.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QListWidgetItem>

TaskInputWidget::TaskInputWidget(QGraphicsScene *scene, InteractiveTaskRectItem *graphItem, QWidget *parent)
    : QWidget(parent), scene(scene), graphItem(graphItem)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout;

    capacityInput = new QLineEdit(this);
    formLayout->addRow(new QLabel("Capacity:"), capacityInput);
    layout->addLayout(formLayout);

    pointListWidget = new QListWidget(this);
    layout->addWidget(pointListWidget);

    QPushButton *createTaskButton = new QPushButton("Utwórz TaskGroup", this);
    connect(createTaskButton, &QPushButton::clicked, this, &TaskInputWidget::createTaskGroup);
    layout->addWidget(createTaskButton);

    graphItem->drawGraph(scene);

    connect(graphItem, &InteractiveTaskRectItem::pickupPointSelected, this, &TaskInputWidget::addPickupPoint);
    connect(graphItem, &InteractiveTaskRectItem::dropoffPointSelected, this, &TaskInputWidget::setDropoffPoint);
    connect(graphItem, &InteractiveTaskRectItem::pickupPointRemoved, this, &TaskInputWidget::removePickupPoint);
    connect(graphItem, &InteractiveTaskRectItem::dropoffPointRemoved, this, &TaskInputWidget::removeDropoffPoint);
}

void TaskInputWidget::addPickupPoint(const QPoint &point)
{
    PointData pointData = {point, 0};
    pickupPointData.push_back(pointData);
    updatePointList();
}

void TaskInputWidget::setDropoffPoint(const QPoint &point)
{
    dropoffPointData = {point, 0};
    updatePointList();
}

void TaskInputWidget::removePickupPoint(const QPoint &point)
{
    auto it = std::find_if(pickupPointData.begin(), pickupPointData.end(),
                           [&point](const PointData& pd) { return pd.point == point; });

    if (it != pickupPointData.end()) {

        pickupPointData.erase(it);

        updatePointList();
    }
}

void TaskInputWidget::removeDropoffPoint(const QPoint &point)
{

    if (dropoffPointData.point == point)
    {

        dropoffPointData = PointData{QPoint(), 0};


        updatePointList();
    }
}
void TaskInputWidget::onPickupStopTimeChanged(const QPoint &point, int newStopTime)
{
    for (auto &pointData : pickupPointData)
    {
        if (pointData.point == point)
        {
            pointData.value = newStopTime;
            break;
        }
    }
}

void TaskInputWidget::onDropoffStopTimeChanged(const QPoint &point, int newStopTime)
{
    if (dropoffPointData.point == point)
    {
        dropoffPointData.value = newStopTime;
    }
}

void TaskInputWidget::updatePointList()
{
    pointListWidget->clear();

    // Add pickup points
    for (const auto &pointData : pickupPointData)
    {
        auto *itemWidget = new PointListItemWidget("Pickup", pointData.point, pointData.value, this);
        connect(itemWidget, &PointListItemWidget::stopTimeChanged, this, &TaskInputWidget::onPickupStopTimeChanged);
        QListWidgetItem *item = new QListWidgetItem(pointListWidget);
        item->setSizeHint(itemWidget->sizeHint());
        pointListWidget->setItemWidget(item, itemWidget);
    }

    // Add dropoff point
    if (dropoffPointData.point != QPoint())
    {
        auto *itemWidget = new PointListItemWidget("Dropoff", dropoffPointData.point, dropoffPointData.value, this);
        connect(itemWidget, &PointListItemWidget::stopTimeChanged, this, &TaskInputWidget::onDropoffStopTimeChanged);
        QListWidgetItem *item = new QListWidgetItem(pointListWidget);
        item->setSizeHint(itemWidget->sizeHint());
        pointListWidget->setItemWidget(item, itemWidget);
    }
}

void TaskInputWidget::createTaskGroup()
{
    int capacity = capacityInput->text().toInt();

    // Add pickup points to the scene
    if(pickupPointData.empty())
        return;
    for (const auto &pointData : pickupPointData)
    {
        int x = pointData.point.x();
        int y = pointData.point.y();
        int stopTime = 10;
        //int stopTime = pointData.value;
        if (x < 0 || y < 0)
        {
            return;
        }
        map::Cell pickup(x, y);
        pickupPoints.push_back(pickup);
        stopTimes.push_back(stopTime);
    }

    int x = dropoffPointData.point.x();
    int y = dropoffPointData.point.y();
    dropoffPoint = map::Cell(x, y);
    //dropoffStopTime = dropoffPointData.value;
    dropoffStopTime = 10;

    std::shared_ptr<TaskGroup> taskGroup = std::make_unique<TaskGroup>(capacity, pickupPoints, dropoffPoint, stopTimes, dropoffStopTime);


    pickupPoints.clear();
    stopTimes.clear();
    pickupPointData.clear();
    dropoffPointData = PointData{QPoint(), 0};
    pointListWidget->clear();
    graphItem->clear();
    if (capacityInput) {
        capacityInput->clear();
    }

    emit taskGroupCreated(std::move(taskGroup));
}
