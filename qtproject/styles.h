#ifndef STYLES_H
#define STYLES_H

#include <QString>

class Styles
{
public:
    Styles();
    ~Styles() = default;
    QString globalStyle() const;

private:
    QString bgColor {"#a6a6a6"}, textColor {"#393e46"};
    QString bgColorDark {"#393e46"}, textColorDark {"#f0ece2"};
    QString btnBgColor {"#393e46"}, btnTextColor {"#f0ece2"};
};

#endif // STYLES_H
