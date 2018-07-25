#ifndef PLOTFORM_H
#define PLOTFORM_H

#include <QWidget>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

namespace Ui {
class plotForm;
}

class plotForm : public QWidget
{
    Q_OBJECT

public:
    explicit plotForm(QWidget *parent = 0);
    ~plotForm();

    void setPlottingData(QString title, QVector<double> xdata, QVector<double> ydata, QwtText xtext, QwtText ytext, QColor color, double xmin, double xmax, double ymin, double ymax);
    void clearPlot();


private:
    Ui::plotForm *ui;
};

#endif // PLOTFORM_H
