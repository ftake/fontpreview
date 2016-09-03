#include <iostream>
#include <QtGui/QApplication>
#include <QWidget>
#include "FontPreview.h"

inline FT_Int floor(FT_Int fixed) {
	return fixed >> 6;
}

class MainWidget : public QWidget {
public:
	MainWidget() {
		QString fontFile("/usr/share/fonts/truetype/ipag.ttf");
		//QString fontFile("/usr/share/fonts/truetype/NotoSansCJK.ttc");
		QString text = QString::fromUtf8("日本語漢字 abcd ABCD\n12345 骨");
		int index = 0;
		FontPreview* preview;
		preview = new FontPreview(this);
		preview->setFontSize(16);
		preview->selectFont(fontFile, index);
		preview->setText(text);
		preview->setGeometry(0, 0, 400, 60);
		preview->show();
		preview = new FontPreview(this);
		preview->setFontSize(16);
		preview->selectFont(fontFile, index);
		preview->setText(text);
		preview->setGeometry(0, 100, 400, 60);
		preview->setRenderMode(Light);
		preview->show();
		preview = new FontPreview(this);
		preview->setFontSize(16);
		preview->selectFont(fontFile, index);
		preview->setText(text);
		preview->setForceAutoHint(true);
		preview->setGeometry(0, 200, 400, 60);
		preview->show();
		preview = new FontPreview(this);
		preview->setFontSize(16);
		preview->selectFont(fontFile, index);
		preview->setText(text);
		preview->setHinting(false);
		preview->setGeometry(0, 300, 400, 60);
		preview->show();
	}
private:
//	FontPreview* m_preview;
};

int main(int argc, char** argv) {
	QApplication app(argc, argv);
	MainWidget mainw;
	mainw.resize(640, 480);
	mainw.show();
	return app.exec();
}
