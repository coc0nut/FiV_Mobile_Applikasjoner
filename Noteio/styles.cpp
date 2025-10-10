#include "styles.h"

Styles::Styles(QObject *parent)
    : QObject{parent}
{}

QString Styles::getBgColor() const
{
    return bgColor;
}

void Styles::setBgColor(const QString &newBgColor)
{
    bgColor = newBgColor;
}

QString Styles::getTextColor() const
{
    return textColor;
}

void Styles::setTextColor(const QString &newTextColor)
{
    if (textColor == newTextColor)
        return;
    textColor = newTextColor;
    emit textColorChanged();
}

QString Styles::getBgColorDark() const
{
    return bgColorDark;
}

void Styles::setBgColorDark(const QString &newBgColorDark)
{
    bgColorDark = newBgColorDark;
}

QString Styles::getTextColorDark() const
{
    return textColorDark;
}

void Styles::setTextColorDark(const QString &newTextColorDark)
{
    textColorDark = newTextColorDark;
}
