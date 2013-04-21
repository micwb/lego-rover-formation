#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QtOpenGL/QGLWidget>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtCore/QElapsedTimer>
#include "../ross/Simulator.h"
#include "types.h"

class GLWindow : public QGLWidget
{
	Q_OBJECT

public:
	GLWindow(QWidget *parent);
	~GLWindow();

protected:
	void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

protected:
	void mousePressEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void keyPressEvent(QKeyEvent* e);

private slots:
	void timerFunc();

private:
	
	
};

#endif // GLWINDOW_H
