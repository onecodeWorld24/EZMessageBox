#ifndef EZMESSAGEBOX_H
#define EZMESSAGEBOX_H

#include <QDialog>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QStyle>

class EZMessageBoxPrivate;

class EZMessageBox : public QDialog
{
    Q_OBJECT
public:
    explicit EZMessageBox(QWidget *parent = nullptr);
    ~EZMessageBox();

    template<typename firstArg, typename... btnArgs>
    static int warning(QWidget *parent, const QString &text, firstArg first, btnArgs... args)
    {
        QList<QPushButton *> btnList;
        QPushButton         *btn = new QPushButton(first);
        btnList.append(btn);
        appendBtn(btnList, args...);

        EZMessageBox dlg(parent);
        dlg.setWindowFlag(Qt::Dialog);
        dlg.initializeUI(text, QStyle::SP_MessageBoxWarning, btnList);
        return dlg.exec();
    }

    template<typename firstArg, typename... btnArgs>
    static int information(QWidget *parent, const QString &text, firstArg first, btnArgs... args)
    {
        QList<QPushButton *> btnList;
        QPushButton         *btn = new QPushButton(first);
        btnList.append(btn);
        appendBtn(btnList, args...);

        EZMessageBox dlg(parent);
        dlg.setWindowFlag(Qt::Dialog);
        dlg.initializeUI(text, QStyle::SP_MessageBoxInformation, btnList);
        return dlg.exec();
    }

    template<typename firstArg, typename... btnArgs>
    static int question(QWidget *parent, const QString &text, firstArg first, btnArgs... args)
    {
        QList<QPushButton *> btnList;
        QPushButton         *btn = new QPushButton(first);
        btnList.append(btn);
        appendBtn(btnList, args...);

        EZMessageBox dlg(parent);
        dlg.setWindowFlag(Qt::Dialog);
        dlg.initializeUI(text, QStyle::SP_MessageBoxQuestion, btnList);
        return dlg.exec();
    }

    template<typename firstArg, typename... btnArgs>
    static int critical(QWidget *parent, const QString &text, firstArg first, btnArgs... args)
    {
        QList<QPushButton *> btnList;
        QPushButton         *btn = new QPushButton(first);
        btnList.append(btn);
        appendBtn(btnList, args...);

        EZMessageBox dlg(parent);
        dlg.setWindowFlag(Qt::Dialog);
        dlg.initializeUI(text, QStyle::SP_MessageBoxCritical, btnList);
        return dlg.exec();
    }

protected:
    void initializeUI(const QString         &text,
                      QStyle::StandardPixmap iconStyle,
                      QList<QPushButton *>  &btnList);
    void initializeStyle();

    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

    /**
     * @brief Recursive termination function, used to handle cases where the parameter list of appendBtn is empty.
     * @param btnList Button list, which will not be modified by this function.
     */
    static void appendBtn(QList<QPushButton *> &btnList) {}

    template<typename firstArg, typename... btnArgs>
    static void appendBtn(QList<QPushButton *> &btnList, firstArg first, btnArgs... args)
    {
        QPushButton *btn = new QPushButton(first);
        btn->setMinimumSize(56, 24);
        btnList.append(btn);
        appendBtn(btnList, args...);
    }

private:
    EZMessageBoxPrivate *d_ptr;
    Q_DECLARE_PRIVATE(EZMessageBox)
};

class EZMessageBoxPrivate
{
public:
    QList<QPushButton *> m_btnList;
    QPushButton         *m_iconLabel;
    QLabel              *m_textLabel;
};

#endif // EZMESSAGEBOX_H
