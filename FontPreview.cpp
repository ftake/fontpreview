#include "FontPreview.h"

inline FT_Int trunc(FT_Int fixed) {
	return fixed >> 6;
}

inline FT_Int round_trunc(FT_Int fixed) {
	return trunc(fixed + (1 << 5));
}

FontPreview::FontPreview(QWidget* parent)
	: QWidget(parent), m_face(0), m_library(0),
	  m_fontSize(16 << 6), m_forceAutoHint(false),
	  m_hinting(true), m_renderMode(Normal) {
	QPalette pal(palette());
	pal.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);

	FT_Error error = FT_Init_FreeType(&m_library);
	if (error) {
		throw error;
	}
}

void FontPreview::selectFont(QString &fileName, int index) {
	if (m_face != 0) {
		FT_Done_Face(m_face);
	}
	FT_Error error = FT_New_Face(
		m_library,
		fileName.toUtf8().constData(),
		index,
		&m_face);
	if (error) {
		throw error;
	}
	if (m_face != 0) {
		FT_Error error = FT_Set_Char_Size(m_face,
										  0,
										  m_fontSize,
										  physicalDpiX(),
										  physicalDpiY());
		if (error) {
			throw error;
		}
	}
}

void FontPreview::setForceAutoHint(bool useAutoHint) {
	this->m_forceAutoHint = useAutoHint;
	// repaint
}

void FontPreview::setHinting(bool useHinting) {
	this->m_hinting = useHinting;
}

void FontPreview::setFontSize(int size) {
	this->m_fontSize = size << 6;
	if (m_face != 0) {
		FT_Error error = FT_Set_Char_Size(m_face,
										  0,
										  m_fontSize,
										  physicalDpiX(),
										  physicalDpiY());
		if (error) {
			throw error;
		}
	}
	// repaint
}

void FontPreview::setText(QString &text) {
	m_text = QString(text);
	// repaint
}

void FontPreview::setRenderMode(RenderMode mode) {
	m_renderMode = mode;
	// repaint
}

void FontPreview::paintEvent(QPaintEvent* event) {
	QWidget::paintEvent(event);

	if (m_library == 0 && m_face == 0) {
		return;
	}

	FT_Error error = FT_Err_Ok;
	QPainter painter;
	painter.begin(this);

//	painter.translate(-m_glyphRect.x(), -m_glyphRect.y());
	QVector<QRgb> colorTable;
	for (int i = 0; i < 256; ++i) {
		colorTable << qRgba(0, 0, 0, i);
	}

	int pen_x = 0 << 6;
	int pen_y = 48 << 6;

	FT_Int32 loadFlag = FT_LOAD_DEFAULT;
	if (m_hinting) {
		if (m_forceAutoHint) {
			loadFlag = loadFlag | FT_LOAD_FORCE_AUTOHINT;
		}
	} else {
		loadFlag = loadFlag | FT_LOAD_NO_HINTING;
	}

	FT_Render_Mode renderMode;
	switch (m_renderMode) {
	case Light:
		renderMode = FT_RENDER_MODE_LIGHT;
		break;
	case LCD:
		renderMode = FT_RENDER_MODE_LCD;
		break;
	default:
		renderMode = FT_RENDER_MODE_NORMAL;
	}

	for (QString::iterator itr = m_text.begin(), end = m_text.end();
		 itr != end; itr++) {
		QChar c = *itr;
		FT_UInt glyph_index = FT_Get_Char_Index(m_face, c.unicode());
		error = FT_Load_Glyph(m_face, glyph_index,
							  loadFlag);
		if (error) {
			std::cout << "FT error: " << error << std::endl;
			continue;
		}
		FT_GlyphSlot g = m_face->glyph;
		error = FT_Render_Glyph(g, renderMode);

		QImage glyphImage(g->bitmap.buffer,
						  g->bitmap.width,
						  g->bitmap.rows,
						  g->bitmap.pitch,
						  QImage::Format_Indexed8);
		glyphImage.setColorTable(colorTable);
		int x = round_trunc(pen_x) + g->bitmap_left;
		int y = trunc(pen_y) - g->bitmap_top;
		std::cout << (wchar_t)c.unicode() << ": ";
		std::cout << x << ", " << y << std::endl;
		painter.drawImage(QPoint(x, y),
						  glyphImage);
		pen_x += g->advance.x;
	}
	
	

//	painter.translate(m_glyphRect.x(),
//					  m_glyphRect.y());

	painter.end();
}
