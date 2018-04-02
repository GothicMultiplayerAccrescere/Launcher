#pragma once

#include <QFrame>
#include <QString>
#include <QStaticText>
#include <QTimer>

class Ticker : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(QString separator READ separator WRITE setSeparator)

public:
    Ticker(QWidget *pParent = nullptr, QString text = "no connection to server asdfasdfasdf asdfasd fasdfasdf asdfasdf asdf asdf asdf asdflkjasdfjlösfjk  fdalkfja sldfkja as dfjlkaj sdfl asdf asdf ");
    void updateText();

public slots:
    QString text() const;
    QString separator() const;
    void setText(QString text);
    void setSeparator(QString separator);

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *);

private slots:
    virtual void timerTimeout();

private:
    QString m_Text;
    QString m_NewText;
    QString m_Separator;
    QStaticText m_StaticText;
    int m_SingleTextWidth;
    QSize m_WholeTextSize;
    int m_LeftMargin;
    bool m_ScrollEnabled;
    int m_ScrollPos;
    QImage m_AlphaChannel;
    QImage m_Buffer;
    QTimer m_Timer;
};
