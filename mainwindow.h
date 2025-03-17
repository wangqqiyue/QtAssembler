#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
class QLineEdit;
class QDialog;
class QLabel;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void newFile();   // 新建操作
    bool maybeSave(); // 判断是否需要保存
    bool save();      // 保存操作
    bool saveAs();    // 另存为操作
    bool saveFile(const QString &fileName); // 保存文件
    bool loadFile(const QString &fileName); // 加载文件
    void showPreprocess(QString s);
    void showToken(QString s);
    void showSymbol(QString s);
    void showInst(QString s);
    void showMachine(QString s);

private slots:
    void on_actionNew_N_triggered();

    void on_actionSave_S_triggered();

    void on_actionSaveAs_A_triggered();

    void on_actionOpen_O_triggered();

    void on_actionClose_W_triggered();

    void on_actionExit_E_triggered();

    void on_actionUndo_Z_triggered();

    void on_actionCut_X_triggered();

    void on_actionCopy_C_triggered();

    void on_actionPaste_V_triggered();
protected:
    void closeEvent(QCloseEvent *event); // 关闭事件
private slots:
    void showFindText();
    void on_actionFind_F_triggered();

    void on_actionBuildAll_triggered();

private:
    Ui::MainWindow *ui;
    // 为真表示文件没有保存过，为假表示文件已经被保存过了
    bool isUntitled;
    // 保存当前文件的路径
    QString curFile;
    QLineEdit *findLineEdit;
    QDialog *findDlg;
    QLabel *statusLabel;

};
#endif // MAINWINDOW_H
