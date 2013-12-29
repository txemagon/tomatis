#include <QtGui>

#include "graphicrenderer.h"


#define RENDER_WIDTH0  800.
#define RENDER_HEIGHT0 500.
#define RENDER_XMARGIN 80
#define RENDER_YMARGIN RENDER_XMARGIN
#define TOTAL_RENDER_WIDTH  (RENDER_WIDTH0  + 2 * RENDER_XMARGIN)
#define TOTAL_RENDER_HEIGHT (RENDER_HEIGHT0 + 2 * RENDER_YMARGIN)


const qreal GraphicRenderer::x_range[2] = {16., 30000.};
const qreal GraphicRenderer::y_range[2] = {0., 2.};

GraphicRenderer::GraphicRenderer()
{
    parent = NULL;
    logarithmic = false;
    knob_radius = 7;

    active_point = -1;
    dragging  = false;
}

GraphicRenderer::GraphicRenderer(QWidget *parent,
                                 PlotData plot_points,
                                 VisualizationData visual_data,
                                 qreal knob_radius, bool logarithmic_axis)
{
    logarithmic  = logarithmic_axis;
    this->parent = parent;
    this->knob_radius = knob_radius;
    this->visual_data = visual_data;

    active_point = -1;
}

void GraphicRenderer::set_coords(QPainter &painter)
{
    painter.translate(visual_data.x0(), visual_data.y0());
    painter.translate(0, visual_data.total_height());
    painter.scale(1, -1);
}

qreal GraphicRenderer::interval(const qreal range[2]) {return range[1] - range[0]; }

QPointF GraphicRenderer::range_lower_limit()
{
    QPointF limit;


    if (logarithmic)
        limit.setX((int) qPow( 10, (int) (qLn( x_range[0]) / M_LN10)));
    else
        limit.setX(( (int) x_range[0] / 10 * 10));
    limit.setY(( (int) y_range[0] / 10 * 10));

    return limit;
}

QPointF GraphicRenderer::range_upper_limit()
{
    QPointF limit;

    if (logarithmic)
        limit.setX(linear_limit( x_range[1]));
    else
        limit.setX(linear_limit(x_range[1]));
    limit.setY(linear_limit(y_range[1]));

    return limit;
}

/**
  Calculate appropiate axis padding for linear scales.

  Range  => Axis
  A coarse approx of desired.
  [0, 1] => [0, 1]
  [0, 2] => [0, 2]
  [0, 5] => [0, 5]
  [0, 7] => [0, 7]
  [0, 8] => [0, 10]
  [0, 14] => [0, 20]
  [0, 18] => [0, 20]
  [0, 24] => [0, 30]
  [0, 87] => [0, 100]
  [0, 91] => [0, 100]

  Rule:
  Bigger than 3/4 of the next order => next order
  else order number + 1
  **/
int GraphicRenderer::linear_limit(qreal range)
{
    int order = (int) qLn(range) / M_LN10;
    if (range * 4. / 3 > qPow( 10, order + 1))
        return (int) qPow(10, order + 1);
    int number_order = range / (int) qPow(10, order);
    if ( range == number_order * qPow(10, order) )
        return range;
    return (number_order + 1) * qPow(10, order);
}

// Translate from logical units (0->1) to device units (0->size)
QPointF GraphicRenderer::to_dev(QPointF logical)
{
    qreal factor_x = (qreal) visual_data.width();
    qreal factor_y = (qreal) visual_data.height();

    qreal x_transform = logical.x();
    qreal x_0 = range_lower_limit().x();
    qreal x_interval  = range_upper_limit().x() - x_0;

    if (logarithmic) {
        x_transform = qLn(x_transform);
        x_interval  = qLn(range_upper_limit().x() / x_0);
        x_0 =  qLn(x_0);
        x_transform = ( x_transform - x_0 ) / x_interval;
    } else {
        x_transform -= x_0;
        x_transform /= x_interval;
    }

    x_transform      *= factor_x;
    qreal y_transform = logical.y() * factor_y / linear_limit(interval(y_range));

    return QPointF(x_transform + visual_data.x_margin(),
                   y_transform + visual_data.y_margin());
}

QPointF GraphicRenderer::to_dev(qreal x, qreal y) { return to_dev(QPointF(x, y)); }

QPointF GraphicRenderer::to_logic(QPointF device)
{
    qreal factor_x = visual_data.width();
    qreal x_0 = range_lower_limit().x();
    qreal x_max = range_upper_limit().x();
    qreal x_interval  = x_max - x_0;
    qreal x_transform = device.x();

    x_transform /= factor_x;

    if (logarithmic){
        x_transform *= qLn(x_max / x_0);
        x_transform += qLn(x_0);
        x_transform /= M_LN10;
        x_transform  = qPow(10, x_transform);
    } else {
        x_transform *= x_interval;
        x_transform += x_0;
    }

    qreal factor_y = (qreal) visual_data.height();
    qreal y_0 = range_lower_limit().y();
    qreal y_interval  = range_upper_limit().y() - y_0;
    qreal y_transform = device.y();

    y_transform /= factor_y;
    y_transform *= y_interval;
    y_transform += y_0;

    return QPointF(x_transform, y_transform);
}

QPointF GraphicRenderer::to_logic(qreal x, qreal y) { return to_logic(QPointF(x,y)); }


QPoint GraphicRenderer::from_app_to_canvas(QPoint canvas)
{
    canvas  = QPoint( canvas.x() * TOTAL_RENDER_WIDTH  / parent->width()  ,
                      canvas.y() * TOTAL_RENDER_HEIGHT / parent->height() );
    canvas += QPoint(-RENDER_XMARGIN, - TOTAL_RENDER_HEIGHT + RENDER_YMARGIN);
    canvas.setY(-canvas.y());
    return canvas;
}

bool GraphicRenderer::hovers(const QPoint &mouse_abs_pos)
{
    return visual_data.active_area().contains(mouse_abs_pos) ;
}

void GraphicRenderer::draw_vertical_grid(QPainter &painter, int options)
{
    painter.save();

    if (options & MID_POINT) {
        QPen pen_main(Qt::gray, 0.5);
        QPainterPath path;
        qreal min       = range_lower_limit().y();
        qreal interval  = (range_upper_limit() - range_lower_limit()).y();
        qreal mid_point = min + interval / 2;

        path.moveTo(to_dev(range_lower_limit().x(), mid_point));
        path.lineTo(to_dev(range_upper_limit().x(), mid_point));
        painter.strokePath(path, pen_main);

        QPointF text_point = to_dev(range_lower_limit().x(), -mid_point);
        text_point.setX(text_point.x() - visual_data.x_margin() / 2);
        painter.save();
        painter.scale(1, -1);
        painter.drawText(text_point, QString::number(mid_point));
        painter.restore();
    }

    QPen pen_main(Qt::blue, 0.7);
    QPen pen_sub(Qt::blue, 0.5, Qt::DotLine );
    QPen *p_pen = NULL;

    qreal range_init = range_lower_limit().y();
    qreal range_end  = range_upper_limit().y();
    qreal module   = qPow(10, (int) (qLn(range_end - range_init) / M_LN10));
    qreal sub_mod  = module / 10;
    qreal delta = 0.01;

    for (qreal i=range_init; i<=range_end + sub_mod;
         i += sub_mod) {
        QPainterPath path;
        p_pen = &pen_sub;
        bool main_div = false;

        qreal times = i / module;
        int int_times = (int) (i + sub_mod/ module);

        if ( options & MAIN_DIV &&
             times >  int_times - delta &&
             times <  int_times + delta) {
            p_pen = &pen_main;
            painter.save();
            painter.scale(1, -1);
            QPointF text_point = to_dev( range_lower_limit().x(), -i );
            text_point.setX(text_point.x() - RENDER_XMARGIN / 4);
            painter.drawText(text_point, QString::number(i));
            painter.restore();
            main_div = true;
        }

        if (main_div || options & SUB_DIV) {
            path.moveTo( to_dev(range_lower_limit().x(), i) );
            path.lineTo( to_dev(range_upper_limit().x(), i) );
            painter.strokePath(path, *p_pen);
        }

    }

    painter.restore();
}

void GraphicRenderer::draw_horizontal_mid_div(QPainter &painter)
{
    painter.save();

    QPen pen_main(Qt::gray, 0.5);
    QPainterPath path;
    qreal min       = range_lower_limit().x();
    qreal interval  = (range_upper_limit() - range_lower_limit()).x();
    qreal mid_point = min + interval / 2;

    path.moveTo(to_dev(mid_point, range_lower_limit().y()));
    path.lineTo(to_dev(mid_point, range_upper_limit().y()));
    painter.strokePath(path, pen_main);

    QPointF text_point = to_dev(mid_point, range_lower_limit().y());
    text_point.setY(text_point.y() + RENDER_YMARGIN / 4);
    painter.save();
    painter.scale(1, -1);
    painter.drawText(text_point, QString::number(mid_point));
    painter.restore();

    painter.restore();
}

void GraphicRenderer::draw_horizontal_grid(QPainter &painter, int options)
{
    painter.save();

    if (options & MID_POINT)
        draw_horizontal_mid_div(painter);

    QPen pen_main(Qt::blue, 0.7);
    QPen pen_sub(Qt::blue, 0.5, Qt::DotLine );
    QPen *p_pen = NULL;

    qreal range_init = range_lower_limit().x();
    qreal range_end  = range_upper_limit().x();
    qreal module   = qPow(10, (int) (qLn(range_end - range_init) / M_LN10));
    qreal sub_mod  = module / 10;
    qreal delta = 0.01;

    for (qreal i=range_init; i<range_end;
         i += sub_mod) {
        QPainterPath path;
        p_pen = &pen_sub;
        bool main_div = false;

        qreal times = i / module;
        int int_times = (int) (i / module);
        if ( options & MAIN_DIV &&
             times >  int_times - delta &&
             times <  int_times + delta) {
            p_pen = &pen_main;
            painter.save();
            painter.scale(1, -1);
            QPointF text_point = to_dev( i, y_range[0] );
            text_point.setY(text_point.y() + RENDER_YMARGIN / 4);
            painter.drawText(text_point, QString::number(i));
            painter.restore();
            main_div = true;
        }

        if (main_div || options & SUB_DIV) {
            path.moveTo( to_dev(i, y_range[0]) );
            path.lineTo( to_dev(i, linear_limit(y_range[1])) );
        }
        painter.strokePath(path, *p_pen);
    }

    painter.restore();
}

void GraphicRenderer::draw_horizontal_log_grid(QPainter &painter, int options)
{
    painter.save();

    if (options & MID_POINT)
        draw_horizontal_mid_div(painter);

    qreal delta = .2;
    QPen pen_main(Qt::blue, 0.7);
    QPen pen_sub(Qt::blue, 0.5, Qt::DotLine );
    QPen *p_pen = NULL;

    // The closest power of 10 to the interval start.
    int range_init = range_lower_limit().x();
    int range_end  = range_upper_limit().x();

    for (int i = range_init; i<= range_end;
         i+= (int) qPow( 10, (int) (qLn(i+1) / M_LN10) ) ){
        QPainterPath path;
        if ( i == range_init ||
             (qLn(i+1) / M_LN10 < (int) (qLn(i+1) / M_LN10) + delta &&
              qLn(i+1) / M_LN10 > (int) (qLn(i+1) / M_LN10) - delta) ){
           p_pen = &pen_main;
           painter.save();
           painter.scale(1, -1);
           QPointF text_point = to_dev(i, y_range[0]);
           text_point.setY(text_point.y() + RENDER_YMARGIN / 4);
           painter.drawText( text_point,
                            QString::number(i));

           painter.restore();
        } else
            p_pen = &pen_sub;

        path.moveTo( to_dev(i, y_range[0]) );
        path.lineTo( to_dev(i, linear_limit(y_range[1])) );
        painter.strokePath(path, *p_pen);
    }
    painter.restore();
}

void GraphicRenderer::paint(QPainter &painter)
{
    QPainterPath path;
    QPen pen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    path.moveTo(to_dev(control_points.at(0)));
    for (int i=1; i<control_points.size(); i++)
        path.lineTo(to_dev(control_points.at(i)));
    painter.strokePath(path, pen);

    painter.setPen(QColor(50, 100, 120, 200));
    painter.setBrush(QColor(200, 200, 210, 120));
    for (int i=0; i<control_points.size(); i++)
        painter.drawEllipse(to_dev(control_points.at(i)), knob_radius, knob_radius);

}


void GraphicRenderer::setup_canvas(QPainter &painter)
{
    // White background rectangle
    painter.fillRect(0, 0,
                     visual_data.total_width(),
                     visual_data.total_height(),
                     Qt::white);

    // White canvas
    painter.drawRect(visual_data.x_margin(), visual_data.y_margin(),
                     visual_data.width(), visual_data.height());
    painter.fillRect(visual_data.x_margin(), visual_data.y_margin(),
                     visual_data.width(), visual_data.height(), QBrush(Qt::white));

    logarithmic ? draw_horizontal_log_grid(painter) : draw_horizontal_grid(painter);
    draw_vertical_grid(painter, MID_POINT);
}

void GraphicRenderer::decide_dragging(const QPoint &mouse_pos)
{
    mouse_pressed_pos = from_app_to_canvas(mouse_pos);
    for (int i=0; i<control_points.size(); ++i)
        if ((to_dev(control_points.at(i)) - mouse_pressed_pos).manhattanLength() < 2 * knob_radius )
            active_point = i;
}

void GraphicRenderer::update_dragging(const QPoint &mouse_pos)
{
    QPoint mouse_now = from_app_to_canvas(mouse_pos);
    if (!dragging &&
        ( mouse_pressed_pos -
          mouse_now
        ).manhattanLength() > 25 )
        dragging = true;

    if (dragging && active_point >= 0){
        QPointF new_position = to_logic(mouse_now);
        QPointF *selected = &control_points[active_point];
        selected->setX(new_position.x());
        selected->setY(new_position.y());
        // parent->update();
    }

}

void GraphicRenderer::stop_dragging()
{
    dragging     = false;
    active_point = -1;

}

