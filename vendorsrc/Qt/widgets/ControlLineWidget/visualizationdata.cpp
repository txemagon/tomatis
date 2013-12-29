#include "visualizationdata.h"


#define RENDER_WIDTH0  800.
#define RENDER_HEIGHT0 500.
#define RENDER_XMARGIN 80
#define RENDER_YMARGIN XMARGIN
#define TOTAL_RENDER_WIDTH  (RENDER_WIDTH0  + 2 * RENDER_XMARGIN)
#define TOTAL_RENDER_HEIGHT (RENDER_HEIGHT0 + 2 * RENDER_YMARGIN)



VisualizationData::VisualizationData(): area(), margin()
{
}

VisualizationData::VisualizationData(QRectF rect, qreal x_margin, qreal y_margin):
    area(rect), margin(x_margin, y_margin)
{
}

VisualizationData::VisualizationData(QRectF rect, QPointF margin):
    area(rect), margin(margin)
{
}

VisualizationData::VisualizationData(qreal x0, qreal y0,
                                     qreal total_width, qreal total_height,
                                     qreal x_margin, qreal y_margin):
    area(x0, y0, total_width, total_height),
    margin(x_margin, y_margin)
{
}

qreal VisualizationData::width()  { return active_area().width(); }
qreal VisualizationData::height() { return active_area().height(); }

qreal VisualizationData::x0() { return area.left(); }
qreal VisualizationData::y0() { return area.top(); }

qreal VisualizationData::x1() { return x0() + x_margin(); }
qreal VisualizationData::y1() { return x1() + y_margin(); }

qreal VisualizationData::total_width() { return area.width();  }
qreal VisualizationData::total_height(){ return area.height(); }

qreal VisualizationData::x_margin() { return margin.x(); }
qreal VisualizationData::y_margin() { return margin.y(); }


QRectF VisualizationData::get_area() { return area; }
QRectF VisualizationData::active_area()
{
    QRectF a_area = area.adjusted(  margin.x(),   margin.y(), - margin.x(), - margin.y());
    return a_area;
}
