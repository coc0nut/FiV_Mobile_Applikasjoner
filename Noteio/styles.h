#ifndef STYLES_H
#define STYLES_H

#include <QObject>

class Styles : public QObject
{
    Q_OBJECT
public:
    explicit Styles(QObject *parent = nullptr);

    QString getBgColor() const;
    void setBgColor(const QString &newBgColor);

    QString getTextColor() const;
    void setTextColor(const QString &newTextColor);

    QString getBgColorDark() const;
    void setBgColorDark(const QString &newBgColorDark);

    QString getTextColorDark() const;
    void setTextColorDark(const QString &newTextColorDark);

signals:

    void textColorChanged();

private:
    QString bgColor {"#a6a6a6"},
            textColor {"#393e46"},
            bgColorDark {"#393e46"},
            textColorDark {"#f0ece2"},
            btnBgColor {"#393e46"},
            btnTextColor {"#f0ece2"};

};

#endif // STYLES_H
