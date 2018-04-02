#include <QPainter>

#include "ticker.h"

Ticker::Ticker(QWidget *pParent, QString text) :
    QFrame(pParent),
    m_Text(text),
    m_ScrollPos(0)
{
    m_StaticText.setTextFormat(Qt::PlainText);

    m_LeftMargin = static_cast<int>(height() * 0.3);

    setSeparator("|");

    connect(&m_Timer, &QTimer::timeout, this, &Ticker::timerTimeout);
    m_Timer.setInterval(20);
    setStyleSheet("background-color:rgba(20, 20, 20, 130); border:none; border: 1px solid black; color:white; font-size:14px;");
    updateText();
    update();
}

QString Ticker::text() const
{
    return m_Text;
}

void Ticker::setText(QString text)
{
    //m_NewText = text; // for dynamic text change
    m_Text = text;
    updateText();
    update();
}

QString Ticker::separator() const
{
    return m_Separator;
}

void Ticker::setSeparator(QString separator)
{
    m_Separator = separator;
    updateText();
    update();
}

void Ticker::updateText()
{
    m_Timer.stop();

    m_SingleTextWidth = fontMetrics().width(m_Text);
    m_ScrollEnabled = (m_SingleTextWidth > width() - m_LeftMargin);

    if (m_ScrollEnabled)
    {
        m_ScrollPos = -64;
        m_StaticText.setText(m_Text + m_Separator);
        m_Timer.start();
    }
    else
        m_StaticText.setText(m_Text);

    m_StaticText.prepare(QTransform(), font());
    m_WholeTextSize = QSize(fontMetrics().width(m_StaticText.text()), fontMetrics().height());
}

void Ticker::paintEvent(QPaintEvent*)
{
    QPainter p(this);

    if (m_ScrollEnabled)
    {
        m_Buffer.fill(qRgba(0, 0, 0, 0));
        QPainter pb(&m_Buffer);
        pb.setPen(p.pen());
        pb.setFont(p.font());

        int x = qMin(-m_ScrollPos, 0) + m_LeftMargin;
        while (x < width())
        {
            pb.drawStaticText(QPointF(x, ((height() - m_WholeTextSize.height()) * 0.5)), m_StaticText);
            x += m_WholeTextSize.width();
        }

        pb.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        pb.setClipRect(width() - 15, 0, 15, height());
        pb.drawImage(0, 0, m_AlphaChannel);
        pb.setClipRect(0, 0, 55, height());
        if (m_ScrollPos < 0)
            pb.setOpacity(static_cast<qreal>(qMax(-8, m_ScrollPos) + 8) * 0.25);
        pb.drawImage(0, 0, m_AlphaChannel);
        p.drawImage(0, 0, m_Buffer);
    }
    else
    {
        p.drawStaticText(QPointF(m_LeftMargin, (height() - m_WholeTextSize.height()) * 0.5), m_StaticText);
    }
}

void Ticker::resizeEvent(QResizeEvent*)
{
    //When the widget is resized, we need to update the alpha channel.

    m_AlphaChannel = QImage(size(), QImage::Format_ARGB32_Premultiplied);
    m_Buffer = QImage(size(), QImage::Format_ARGB32_Premultiplied);

    //Create Alpha Channel:
    if (width() > 64)
    {
        do
        {
            //create first scanline
            QRgb* scanline1 = reinterpret_cast<QRgb *>(m_AlphaChannel.scanLine(0));
            if (!scanline1)
                break;

            for (int x = 1; x < 16; ++x)
                scanline1[x - 1] = scanline1[width() - x] = qRgba(0, 0, 0, x << 4);
            for (int x = 15; x < width() - 15; ++x)
                scanline1[x] = qRgb(0, 0, 0);
            //copy scanline to the other ones
            for (int y = 1; y < height(); ++y)
                memcpy(m_AlphaChannel.scanLine(y), scanline1, static_cast<uint32_t>(width() * 4));
        } while (false);
    }
    else
        m_AlphaChannel.fill(qRgb(0, 0, 0));


    //Update scrolling state
    bool newScrollEnabled = (m_SingleTextWidth > width() - m_LeftMargin);
    if (newScrollEnabled != m_ScrollEnabled)
        updateText();
}

void Ticker::timerTimeout()
{
    m_ScrollPos = (m_ScrollPos + 2) % m_WholeTextSize.width() - 1;
    update();
}
