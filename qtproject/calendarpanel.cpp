#include "calendarpanel.h"
#include <QHBoxLayout>
#include <QTextCharFormat>
#include <QBrush>
#include <QColor>
#include <QFont>
#include <QAbstractItemView>

CalendarPanel::CalendarPanel(QWidget *parent) : QWidget(parent) {

    calendar_ = new QCalendarWidget(this);
    calendar_->setStyleSheet(
        "QCalendarWidget QWidget#qt_calendar_navigationbar { background-color: #393e46; }"
        "QCalendarWidget QToolButton { background-color: transparent; color: #f0ece2; border: none; }"
        "QCalendarWidget QSpinBox { background-color: transparent; color: #f0ece2; border: none; }"
        "QCalendarWidget QAbstractItemView { color: white; }"
    );
    calendar_->setGridVisible(true);
    calendar_->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    calendar_->setFixedWidth(800);
    calendar_->setFixedHeight(400);
    calendar_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QTextCharFormat headerFmt;
    headerFmt.setBackground(QBrush(QColor(bgColorDark)));
    headerFmt.setForeground(QBrush(QColor(textColorDark)));
    for (int d = Qt::Monday; d <= Qt::Sunday; ++d) {
        calendar_->setWeekdayTextFormat(static_cast<Qt::DayOfWeek>(d), headerFmt);
    }

    QTextCharFormat weekendFmt;
    weekendFmt.setBackground(QBrush(QColor("#393e46")));
    weekendFmt.setForeground(QBrush(QColor("#be3144")));
    calendar_->setWeekdayTextFormat(Qt::Saturday, weekendFmt);
    calendar_->setWeekdayTextFormat(Qt::Sunday, weekendFmt);

    calendar_->installEventFilter(this);
    if (auto view = calendar_->findChild<QAbstractItemView*>()) {
        view->viewport()->installEventFilter(this);
        view->installEventFilter(this);
    }
    if (auto nav = calendar_->findChild<QWidget*>("qt_calendar_navigationbar")) {
        nav->installEventFilter(this);
    }

    events_ = new QListWidget(this);
    events_->setFixedHeight(400);
    events_->setMinimumWidth(200);
    events_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    events_->setUniformItemSizes(true); 
    events_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    auto *lay = new QHBoxLayout(this);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(12);
    lay->addWidget(calendar_, 0);
    lay->addWidget(events_, 1);
    this->setFixedHeight(420);

    connect(calendar_, &QCalendarWidget::selectionChanged, this, &CalendarPanel::onSelectionChanged);
    updateEventList(calendar_->selectedDate());
}

bool CalendarPanel::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::Wheel) {
        if (obj == calendar_) {
            return true;
        }
        if (auto w = qobject_cast<QWidget*>(obj)) {
            if (calendar_->isAncestorOf(w)) {
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void CalendarPanel::setTodos(const QVector<Todo*> &todos) {
    todosByDate_.clear();
    for (Todo *t : todos) {
        QDate d = QDate::fromString(t->due(), Qt::ISODate);
        if (!d.isValid()) continue;
        todosByDate_[d].append(t);
    }
    updateMarks();
    updateEventList(calendar_->selectedDate());
}

void CalendarPanel::clearSelection() {
    calendar_->setSelectedDate(QDate::currentDate());
    calendar_->setFocus();
    updateEventList(calendar_->selectedDate());
}

void CalendarPanel::rebuildMap() {  }

void CalendarPanel::updateMarks() {
    QDate visibleStart = QDate(calendar_->yearShown(), calendar_->monthShown(), 1).addDays(-31);
    QDate visibleEnd = visibleStart.addDays(92);
    for (QDate d = visibleStart; d <= visibleEnd; d = d.addDays(1)) {
        calendar_->setDateTextFormat(d, QTextCharFormat());
    }

    QTextCharFormat headerFmt;
    headerFmt.setBackground(QBrush(QColor(bgColorDark)));
    headerFmt.setForeground(QBrush(QColor(textColorDark)));
    for (int d = Qt::Monday; d <= Qt::Friday; ++d) {
        calendar_->setWeekdayTextFormat(static_cast<Qt::DayOfWeek>(d), headerFmt);
    }
    QTextCharFormat weekendFmt;
    weekendFmt.setBackground(QBrush(QColor(bgColorDark)));
    weekendFmt.setForeground(QBrush(QColor("#be3144")));
    calendar_->setWeekdayTextFormat(Qt::Saturday, weekendFmt);
    calendar_->setWeekdayTextFormat(Qt::Sunday, weekendFmt);

    
    QTextCharFormat todoFmt;
    todoFmt.setBackground(QBrush(QColor("#be3144")));
    todoFmt.setForeground(QBrush(QColor("#ffffff")));
    todoFmt.setFontWeight(QFont::Bold);

    for (auto it = todosByDate_.constBegin(); it != todosByDate_.constEnd(); ++it) {
        QDate date = it.key();
        if (date < visibleStart || date > visibleEnd) continue;
        QTextCharFormat fmt = calendar_->dateTextFormat(date);
        fmt.setBackground(todoFmt.background());
        fmt.setForeground(todoFmt.foreground());
        fmt.setFontWeight(todoFmt.fontWeight());
        calendar_->setDateTextFormat(date, fmt);
    }

    QDate today = QDate::currentDate();
    if (today >= visibleStart && today <= visibleEnd) {
        QTextCharFormat todayFmt = calendar_->dateTextFormat(today);
        todayFmt.setBackground(QBrush(QColor(bgColor)));
        todayFmt.setForeground(QBrush(QColor(textColor)));
        todayFmt.setFontWeight(QFont::Bold);
        calendar_->setDateTextFormat(today, todayFmt);
    }
}

void CalendarPanel::updateEventList(const QDate &date) {
    events_->clear();

    if (date.isValid() && todosByDate_.contains(date)) {
        for (Todo *t : todosByDate_.value(date)) {
            QString label = QString("[%1] %2").arg(t->completed() ? "x" : " ").arg(t->title());
            events_->addItem(label);
        }
    } else {
        bool any = false;
        for (auto it = todosByDate_.constBegin(); it != todosByDate_.constEnd(); ++it) {
            for (Todo *t : it.value()) {
                QString label = QString("[%1] %2").arg(t->completed() ? "x" : " ").arg(t->title());
                events_->addItem(label);
                any = true;
            }
        }
        if (!any) {
            events_->addItem("No todos available");
        }
    }
}

void CalendarPanel::onSelectionChanged() {
    QDate d = calendar_->selectedDate();
    if (d.isValid() && todosByDate_.contains(d)) {
        updateEventList(d);
        emit dateSelected(d);
    } else {
        updateEventList(QDate());
        emit dateSelected(QDate());
    }
}
