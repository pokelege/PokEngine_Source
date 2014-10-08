#pragma once
#include <QtGui\QWidget>
#include <QtGui\QFileDialog>
class QLineEdit;
class Preview;
class ConverterWindow : public QWidget
{
	Q_OBJECT;

	private slots:
	void browse();
	void startConverting();
	void colorTextureButton();
public:
	Preview* preview;
	QLineEdit* fileName;
	QFileDialog dialogbox;
	ConverterWindow();
	~ConverterWindow();
};