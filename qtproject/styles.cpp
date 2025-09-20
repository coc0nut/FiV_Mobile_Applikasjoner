#include "styles.h"

Styles::Styles() {}

QString Styles::globalStyle() const {
    return QString(R"(
        QWidget {
            background: %1;
            color: %2;
        }
        QPushButton {
            background: %3;
            color: %4;
            border-radius: 8px;
            font-size: 18px;
            padding: 10px;
        }
        QPushButton:hover {
            background: %1;
            color: %2;
        }
        QLabel#counterLabel {
            font-size: 20px;
            font-weight: bold;
        }
    )").arg(bgColor, textColor, bgColorDark, textColorDark);
}
