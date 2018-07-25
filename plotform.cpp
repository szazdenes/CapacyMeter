#include "plotform.h"
#include "ui_plotform.h"

plotForm::plotForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::plotForm)
{
    ui->setupUi(this);
}

plotForm::~plotForm()
{
    delete ui;
}

void plotForm::setPlottingData(QString title, QVector<double> xdata, QVector<double> ydata, QwtText xtext, QwtText ytext, QColor color, double xmin, double xmax, double ymin, double ymax)
{
    QwtPlotCurve *curve = new QwtPlotCurve();
    QwtText titleText = title;
    QFont font;
    font.setBold(true);
    font.setPointSize(12);
    titleText.setFont(font);

    ui->plotFrame->detachItems();
    ui->plotFrame->setTitle(titleText);
    ui->plotFrame->setAxisTitle(QwtPlot::xBottom, xtext);
    ui->plotFrame->setAxisTitle(QwtPlot::yLeft, ytext);
    curve->setSamples(xdata, ydata);
    curve->setPen(QPen(color, 1));
    curve->attach(ui->plotFrame);
    ui->plotFrame->setAxisScale(QwtPlot::xBottom, xmin, xmax);
    ui->plotFrame->setAxisScale(QwtPlot::yLeft, ymin, ymax);
    ui->plotFrame->setCanvasBackground(Qt::white);
    ui->plotFrame->replot();
}

void plotForm::clearPlot()
{
    ui->plotFrame->detachItems();
    ui->plotFrame->replot();
}
