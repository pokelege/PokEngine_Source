#pragma once
#include <QtGui\QWidget>
#include <QtGui\QFileDialog>
class QLineEdit;
class Preview;
class QComboBox;
class QSpinBox;
class ConverterWindow : public QWidget
{
	Q_OBJECT;


	private slots:
	void browse();
	void startConverting();
	void colorTextureButton();
	void currentIndexChanged( int index );
	void addFrameRange();
	void deleteFrameRange();
	void changeParamsNext();
	void changeParamsStart();
	void changeParamsEnd();
	void save();
public:
	Preview* preview;
	QLineEdit* fileName;
	QComboBox* frameList;
	QSpinBox* nextFrame;
	QSpinBox* startFrame;
	QSpinBox* endFrame;
	QFileDialog dialogbox;
	ConverterWindow();
	~ConverterWindow();
};