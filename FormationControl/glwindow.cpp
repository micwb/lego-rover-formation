#include "glwindow.h"
#include <QtCore/QTimer>
#include <QtCore/QMutex>
#include <QtCore/QElapsedTimer>
#include "formationcontrol.h"

QElapsedTimer* pElapsedTimer;

GLWindow::GLWindow(QWidget *parent)
	: QGLWidget(parent)
{
	setFocusPolicy(Qt::ClickFocus);
	QTimer* pTimer = new QTimer(this);
	pElapsedTimer = new QElapsedTimer();
	connect(pTimer, SIGNAL(timeout()), this, SLOT(timerFunc()));
	pTimer->start(STI_MS);
	pElapsedTimer->start();
}

GLWindow::~GLWindow()
{

}

void GLWindow::mousePressEvent(QMouseEvent* e)
{

}

void GLWindow::mouseMoveEvent(QMouseEvent* e)
{
	float x = e->pos().x();
	float y = e->pos().y();
}

void GLWindow::keyPressEvent(QKeyEvent* e)
{
	Cell* pCell = env.getCell(sID);
	switch (e->key())
    {
	    case Qt::Key_Left:
			if (pCell)
				pCell->rotateRelative(
					max(90.0f, pCell->maxAngSpeed()));
            break;
		case Qt::Key_Up:
			if (pCell)
				pCell->translateRelative(
					max(0.01f, pCell->maxSpeed()));
            break;
        case Qt::Key_Right:
			if (pCell)
				pCell->rotateRelative(
					-max(90.0f, pCell->maxAngSpeed()));
            break;
        case Qt::Key_Down:
			if (pCell)
				pCell->translateRelative(
					-max(0.01f, pCell->maxSpeed()));
            break;
        default: 
			keyboardPress((unsigned char) e->text().toStdString().c_str()[0], 0, 0);
			break;
    }
}

extern bool gGo;
void GLWindow::timerFunc()
{
	if(gGo)
	{
		if(pElapsedTimer)
		{
			//FormationControl* fc = (FormationControl*) this->parent()->parent();
			//fc->ui.stdoutput->appendPlainText(QString::number(pElapsedTimer->elapsed()));
			pElapsedTimer->restart();
		}
		env.step();
		update();
	}
}

void GLWindow::initializeGL()
{
	initWindow();
	resizeWindow(geometry().width(), geometry().height());
}

void GLWindow::resizeGL(int w, int h)
{
	resizeWindow(w,h);
}

void GLWindow::paintGL()
{
	if(gGo)
		display(this);
}
