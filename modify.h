#ifndef MODIFY_H
#define MODIFY_H

#include <QDialog>


class AllListing;

namespace Ui {
class Modify;
}

class Modify : public QDialog
{
    Q_OBJECT

public:
    explicit Modify(
            AllListing *allListing,
            QWidget *parent = 0);
    ~Modify();

private slots:
    void on_updateButtonBox_rejected();

    void on_updateButtonBox_accepted();


private:
    Ui::Modify *ui;

    AllListing *allListing;
};

#endif // MODIFY_H
