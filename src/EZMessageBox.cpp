#include "EZMessageBox.h"
#include <QApplication>
#include <QDebug>
#include <QHBoxLayout>
#include <QIcon>
#include <QPainter>
#include <QStyleOption>
#include <QVBoxLayout>

#include <windows.h>
//
#include <WinUser.h>
#include <dwmapi.h>
#include <windowsx.h>

EZMessageBox::EZMessageBox(QWidget *parent)
    : QDialog(parent)
    , d_ptr(new EZMessageBoxPrivate)
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setObjectName("messageBox");
    HWND  hwnd = (HWND) this->winId();
    DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
    ::SetWindowLong(hwnd, GWL_STYLE, style | WS_THICKFRAME);
    const int     num = 4;
    const MARGINS shadow = {num, num, num, num};
    DwmExtendFrameIntoClientArea(HWND(winId()), &shadow);
}

EZMessageBox::~EZMessageBox()
{
    delete d_ptr;
}

void EZMessageBox::initializeUI(const QString         &text,
                                QStyle::StandardPixmap iconStyle,
                                QList<QPushButton *>  &btnList)
{
    Q_D(EZMessageBox);
    d->m_btnList = btnList;

    QIcon icon = QApplication::style()->standardIcon(iconStyle);
    d->m_iconLabel = new QPushButton(icon, "", this);
    d->m_iconLabel->setObjectName("messageBoxIconLabel");
    d->m_iconLabel->setFixedSize(18, 18);
    d->m_iconLabel->setIconSize(QSize(18, 18));

    d->m_textLabel = new QLabel(this);
    d->m_textLabel->setText(text);

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(12);
    hLayout->addWidget(d->m_iconLabel);
    hLayout->addWidget(d->m_textLabel);
    hLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding));
    vLayout->addLayout(hLayout);
    QHBoxLayout *hLayout2 = new QHBoxLayout();
    hLayout2->setSpacing(8);
    hLayout2->setContentsMargins(0, 0, 0, 0);
    hLayout2->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding));

    int index = 0;
    for (auto btnObj : qAsConst(btnList)) {
        if (btnObj) {
            btnObj->setObjectName(QString("messageBtn%1").arg(index));
            btnObj->setFixedSize(60, 25);
            hLayout2->addWidget(btnObj);
            connect(btnObj, &QPushButton::clicked, this, [&, index]() { done(index); });
        }
        index++;
    }
    vLayout->addLayout(hLayout2);
    vLayout->setContentsMargins(16, 16, 16, 16);
    vLayout->setSpacing(18);
    setLayout(vLayout);

    initializeStyle();
    setMinimumSize(350, 112);
}

void EZMessageBox::initializeStyle() {}

bool EZMessageBox::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_D(EZMessageBox);
    MSG *msg = reinterpret_cast<MSG *>(message);
    if (msg->message == WM_NCCALCSIZE) {
        if (msg->wParam == TRUE) {
            // Setting result to 0 indicates that the default non-client area is not needed.
            *result = 0;
            return true;
        }
    }
    if (msg->message == WM_NCHITTEST) {
        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);

        POINT nativeLocalPos{x, y};
        ::ScreenToClient(msg->hwnd, &nativeLocalPos);
        QPoint pt(nativeLocalPos.x, nativeLocalPos.y);
        if (!d->m_iconLabel->rect().contains(d->m_iconLabel->mapFromParent(pt))
            && !d->m_textLabel->rect().contains(d->m_textLabel->mapFromParent(pt))) {
            bool bIn = false;
            for (auto btn : qAsConst(d->m_btnList)) {
                if (btn->rect().contains(btn->mapFromParent(pt))) {
                    bIn = true;
                    break;
                }
            }
            if (!bIn) {
                *result = HTCAPTION;
                return true;
            }
        }
    }
    return false;
}
