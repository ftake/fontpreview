#include <iostream>
#include <QtGui/QApplication>
#include <QWidget>
#include <QPainter>
#include <QChar>

#include <ft2build.h>
#include FT_FREETYPE_H

enum RenderMode {
	Normal,
	Light,
	LCD
};

class FontPreview : public QWidget {
public:
	FontPreview(QWidget *parent);
	~FontPreview() {
		if (m_face != 0) {
			FT_Done_Face(m_face);
		}
		if (m_library != 0) {
			FT_Done_FreeType(m_library);
		}
	}
	void selectFont(QString &fileName, int index);
	void setText(QString &text);
	void setFontSize(int size);
	void setForceAutoHint(bool useAutoHint);
	void setHinting(bool useHinting);
	void setRenderMode(RenderMode mode);
protected:
	void paintEvent(QPaintEvent* event);
private:
	FT_Library m_library;
	FT_Face m_face;
	QRect m_glyphRect;
	QString m_text;
	int m_fontSize;
	bool m_forceAutoHint;
	bool m_hinting;
	RenderMode m_renderMode;
};

