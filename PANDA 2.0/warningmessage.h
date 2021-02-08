#ifndef WARNINGMESSAGE_H
#define WARNINGMESSAGE_H

#include<QMessageBox>


class WarningMessage : public QMessageBox
{
    Q_OBJECT
public:
    WarningMessage(QString message, QWidget *parent = nullptr);
};

#endif // WARNINGMESSAGE_H
