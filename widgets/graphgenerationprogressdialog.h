#ifndef GRAPHGENERATIONPROGRESSDIALOG_H
#define GRAPHGENERATIONPROGRESSDIALOG_H

#include <QDialog>
#include <QVariant>

namespace Ui {
class GraphGenerationProgressDialog;
}

class GraphGenerationProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GraphGenerationProgressDialog(QWidget *parent = nullptr);
    ~GraphGenerationProgressDialog();
    void setStepTextAndProgressValue(uint step = 0, QVariant otherInfo1 = QVariant(), QVariant otherInfo2 = QVariant(), QVariant otherInfo3 = QVariant() ,QVariant otherInfo4 = QVariant());


protected:
    void changeEvent(QEvent* event) override;

private:
    Ui::GraphGenerationProgressDialog *ui;
};

#endif // GRAPHGENERATIONPROGRESSDIALOG_H
