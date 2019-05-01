#include "animatetext.h"
#include <QFont>

animateText::animateText(QString text, int font_size) {
    setFont(QFont(font_family, font_size));
    setPlainText(text);
}
