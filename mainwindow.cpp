#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>
#include <QLineEdit>
#include <QDialog>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QLabel>
#include "assembler.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化文件为未保存状态
    isUntitled = true;
    // 初始化文件名为"未命名.txt"
    curFile = tr("未命名.txt");
    // 初始化窗口标题为文件名
    setWindowTitle(curFile);
    findDlg = new QDialog(this);
    findDlg->setWindowTitle(tr("查找"));
    findLineEdit = new QLineEdit(findDlg);
    QPushButton *btn= new QPushButton(tr("查找下一个"), findDlg);
    QVBoxLayout *layout= new QVBoxLayout(findDlg);
    layout->addWidget(findLineEdit);
    layout->addWidget(btn);
    findLineEdit->setStyleSheet("QLineEdit { background-color: white; }"
                                "QLineEdit::selection { background-color: yellow; color: black; }");
    connect(btn, SIGNAL(clicked()), this, SLOT(showFindText()));
    statusLabel = new QLabel;
    statusLabel->setMinimumSize(150, 20); // 设置标签最小大小
    statusLabel->setFrameShape(QFrame::WinPanel); // 设置标签形状
    statusLabel->setFrameShadow(QFrame::Sunken); // 设置标签阴影
    ui->statusbar->addWidget(statusLabel);
    statusLabel->setText(tr("欢迎使用MIPS汇编器！"));

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::showPreprocess(QString s) {
    // 创建一个 QDialog 窗口
    QDialog* dialog = new QDialog(this);  // 设置父窗口为主窗口
    dialog->setWindowTitle("1. 预处理结果");
    dialog->setAttribute(Qt::WA_DeleteOnClose);  // 确保窗口关闭时自动删除

    // 创建一个只读的 QTextEdit 来显示内容
    QTextEdit* textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    textEdit->setPlainText(s);

    // 创建一个布局并将 QTextEdit 添加到布局中
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->addWidget(textEdit);

    // 设置布局
    dialog->setLayout(layout);

    // 调整窗口大小
    dialog->resize(400, 300);

    // 调整窗口位置，实现堆叠效果
    QPoint offset(20 , 20 );  // 每个窗口偏移 (20, 20)
    dialog->move(this->pos() + offset);

    // 显示窗口
    dialog->show();
}
void MainWindow::showToken(QString s){
    // 创建一个 QDialog 窗口
    QDialog* dialog = new QDialog(this);  // 设置父窗口为主窗口
    dialog->setWindowTitle("2. 分词结果");
    dialog->setAttribute(Qt::WA_DeleteOnClose);  // 确保窗口关闭时自动删除

    // 创建一个只读的 QTextEdit 来显示内容
    QTextEdit* textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    textEdit->setPlainText(s);

    // 创建一个布局并将 QTextEdit 添加到布局中
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->addWidget(textEdit);

    // 设置布局
    dialog->setLayout(layout);

    // 调整窗口大小
    dialog->resize(400, 300);

    // 调整窗口位置，实现堆叠效果
    QPoint offset(40 , 40 );
    dialog->move(this->pos() + offset);

    // 显示窗口
    dialog->show();
}
void MainWindow::showSymbol(QString s){
    // 创建一个 QDialog 窗口
    QDialog* dialog = new QDialog(this);  // 设置父窗口为主窗口
    dialog->setWindowTitle("3. 符号表");
    dialog->setAttribute(Qt::WA_DeleteOnClose);  // 确保窗口关闭时自动删除

    // 创建一个只读的 QTextEdit 来显示内容
    QTextEdit* textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    textEdit->setPlainText(s);

    // 创建一个布局并将 QTextEdit 添加到布局中
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->addWidget(textEdit);

    // 设置布局
    dialog->setLayout(layout);

    // 调整窗口大小
    dialog->resize(400, 300);

    // 调整窗口位置，实现堆叠效果
    QPoint offset(60 , 60 );
    dialog->move(this->pos() + offset);

    // 显示窗口
    dialog->show();
}

void MainWindow::showInst(QString s){
    // 创建一个 QDialog 窗口
    QDialog* dialog = new QDialog(this);  // 设置父窗口为主窗口
    dialog->setWindowTitle("4. 指令解析结果");
    dialog->setAttribute(Qt::WA_DeleteOnClose);  // 确保窗口关闭时自动删除

    // 创建一个只读的 QTextEdit 来显示内容
    QTextEdit* textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    textEdit->setPlainText(s);

    // 创建一个布局并将 QTextEdit 添加到布局中
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->addWidget(textEdit);

    // 设置布局
    dialog->setLayout(layout);

    // 调整窗口大小
    dialog->resize(400, 300);

    // 调整窗口位置，实现堆叠效果
    QPoint offset(80 , 80 );
    dialog->move(this->pos() + offset);

    // 显示窗口
    dialog->show();
}
void MainWindow::showMachine(QString s) {
    // 创建一个 QDialog 窗口
    QDialog* dialog = new QDialog(this);  // 设置父窗口为主窗口
    dialog->setWindowTitle("5. 机器指令");
    dialog->setAttribute(Qt::WA_DeleteOnClose);  // 确保窗口关闭时自动删除

    // 创建一个只读的 QTextEdit 来显示内容
    QTextEdit* textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    textEdit->setPlainText(s);

    // 创建一个按钮用于保存内容
    QPushButton* saveButton = new QPushButton("保存到文件", dialog);

    // 创建一个布局并将 QTextEdit 和按钮添加到布局中
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->addWidget(textEdit);
    layout->addWidget(saveButton);

    // 设置布局
    dialog->setLayout(layout);

    // 调整窗口大小
    dialog->resize(400, 300);

    // 调整窗口位置，实现堆叠效果
    QPoint offset(100, 100);
    dialog->move(this->pos() + offset);

    // 为按钮添加点击事件
    connect(saveButton, &QPushButton::clicked, [dialog, textEdit]() {
        // 弹出文件选择对话框，让用户选择保存位置
        QString filePath = QFileDialog::getSaveFileName(dialog, "保存文件", "", "Text Files (*.txt)");
        if (!filePath.isEmpty()) {
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << textEdit->toPlainText();
                file.close();
                QMessageBox::information(dialog, "保存成功", "内容已成功保存到文件中！");
            } else {
                QMessageBox::critical(dialog, "保存失败", "无法打开文件进行写入！");
            }
        }
    });

    // 显示窗口
    dialog->show();
}


void MainWindow::newFile()
{
    if (maybeSave()) {
        isUntitled = true;
        curFile = tr("未命名.txt");
        setWindowTitle(curFile);
        ui->textEdit->clear();
        ui->textEdit->setVisible(true);
    }
}

bool MainWindow::maybeSave()
{
    // 如果文档被更改了
    if (ui->textEdit->document()->isModified()) {
        // 自定义一个警告对话框
        QMessageBox box;
        box.setWindowTitle(tr("警告"));
        box.setIcon(QMessageBox::Warning);
        box.setText(curFile + tr(" 尚未保存，是否保存？"));
        QPushButton *yesBtn = box.addButton(tr("是(&Y)"),
                                            QMessageBox::YesRole);
        box.addButton(tr("否(&N)"), QMessageBox::NoRole);
        QPushButton *cancelBut = box.addButton(tr("取消"),
                                               QMessageBox::RejectRole);
        box.exec();
        if (box.clickedButton() == yesBtn)
            return save();
        else if (box.clickedButton() == cancelBut)
            return false;
    }
    // 如果文档没有被更改，则直接返回true
    return true;
}

bool MainWindow::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("另存为"),
                                                    "../..",
                                                    tr("文本文件 *.txt"));
    if (fileName.isEmpty()) return false;
    return saveFile(fileName);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {

        // %1和%2分别对应后面arg两个参数，/n起换行的作用
        QMessageBox::warning(this, tr("多文档编辑器"),
                             tr("无法写入文件 %1：/n %2")
                                 .arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    // 鼠标指针变为等待状态
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << ui->textEdit->toPlainText();
    // 鼠标指针恢复原来的状态
    QApplication::restoreOverrideCursor();
    isUntitled = false;
    // 获得文件的标准路径
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    return true;
}
bool MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName); // 新建QFile对象
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("多文档编辑器"),
                             tr("无法读取文件 %1:\n%2.")
                                 .arg(fileName).arg(file.errorString()));
        return false; // 只读方式打开文件，出错则提示，并返回false
    }
    QTextStream in(&file); // 新建文本流对象
    QApplication::setOverrideCursor(Qt::WaitCursor);
    // 读取文件的全部文本内容，并添加到编辑器中
    ui->textEdit->setPlainText(in.readAll());      QApplication::restoreOverrideCursor();

    // 设置当前文件
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // 如果maybeSave()函数返回true，则关闭程序
    if (maybeSave()) {
        event->accept();
    } else {   // 否则忽略该事件
        event->ignore();
    }
}

void MainWindow::showFindText()
{
    QString str = findLineEdit->text();
    QTextDocument *textDocument = ui->textEdit->document();

    // 如果是第一次查找，从文档开头开始
    static QTextCursor lastCursor;
    if (lastCursor.isNull())
    {
        lastCursor = QTextCursor(textDocument);
    }

    // 先清除所有之前的高亮格式
    QTextCursor cursor(textDocument);
    QTextCharFormat plainFormat; // 默认格式
    plainFormat.setBackground(Qt::white);
    plainFormat.setForeground(Qt::black); // 恢复默认文本颜色
    cursor.select(QTextCursor::Document); // 选择整个文档
    cursor.mergeCharFormat(plainFormat); // 应用默认格式


    // 从上次找到的位置继续查找
    lastCursor = textDocument->find(str, lastCursor);

    if (lastCursor.isNull())
    {
        QMessageBox::warning(this, tr("查找"),
                             tr("找不到%1").arg(str));
        lastCursor = QTextCursor(); // 重置查找位置
    }
    else
    {
        QTextCharFormat format;
        format.setBackground(Qt::yellow);
        format.setForeground(Qt::red); // 设置找到的文本颜色为红色
        lastCursor.mergeCharFormat(format); // 应用格式
    }
}

void MainWindow::on_actionNew_N_triggered()
{
    newFile();
}


void MainWindow::on_actionSave_S_triggered()
{
    save();
}


void MainWindow::on_actionSaveAs_A_triggered()
{
    saveAs();
}


void MainWindow::on_actionOpen_O_triggered()
{
    if (maybeSave()) {

        QString fileName = QFileDialog::getOpenFileName(this);
        qDebug()  << fileName;
        // 如果文件名不为空，则加载文件
        if (!fileName.isEmpty()) {
            loadFile(fileName);
            ui->textEdit->setVisible(true);
        }
    }
}


void MainWindow::on_actionClose_W_triggered()
{
    if (maybeSave()) {
        ui->textEdit->setVisible(false);
    }
}


void MainWindow::on_actionExit_E_triggered()
{
    // 先执行关闭操作，再退出程序
    // qApp是指向应用程序的全局指针
    on_actionClose_W_triggered();
    qApp->quit();
}


void MainWindow::on_actionUndo_Z_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionCut_X_triggered()
{
    ui->textEdit->cut();
}



void MainWindow::on_actionCopy_C_triggered()
{
    ui->textEdit->copy();
}


void MainWindow::on_actionPaste_V_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionFind_F_triggered()
{
    findDlg->show();
}

// 将 TokenType 转换为字符串
QString tokenTypeToString(TokenType type) {
    switch (type) {
    case IDENTIFIER: return "IDENTIFIER";
    case NUMBER: return "NUMBER";
    case REGISTER: return "REGISTER";
    case COMMA: return "COMMA";
    case COLON: return "COLON";
    case LPAREN: return "LPAREN";
    case RPAREN: return "RPAREN";
    case DOT: return "DOT";
    case NEWLINE: return "NEWLINE";
    case UNKNOWN: return "UNKNOWN";
    default: return "UNKNOWN";
    }
}

void MainWindow::on_actionBuildAll_triggered()
{
        // 获取用户输入的汇编代码
        QString origin = ui->textEdit->toPlainText();

        try{
        // 调用汇编器函数将代码转换为机器指令
        string code = preprocess(origin.toStdString());

        showPreprocess(QString::fromStdString(code));

        // 词法分析
        vector<Token> tokens = tokenize(code);
        QString tokenStr;


        // 遍历 tokens 并组装字符串
        for (const auto& token : tokens) {
            tokenStr += QString::fromStdString(token.value) + ": " +
                        tokenTypeToString(token.type) + "\n";
        }

        showToken(tokenStr);

        // 构建符号表
        buildSymbolTable(tokens);
        QString symbolStr="Symbol \t Address \n";

        // 遍历符号表并组装字符串
        for (const auto& s : symbolTable) {
            symbolStr += QString::fromStdString(s.first) + "\t" +
                         QString("0x%1").arg(s.second, 8, 16, QLatin1Char('0')).toUpper() + "\n";
        }

        showSymbol(symbolStr);

        // 语法分析
        vector<Instruction> instructions = parse(tokens);
        QString instStr = "Instruction \t Operator \n";

        // 遍历指令并组装字符串
        for (const auto& instr : instructions) {
            QString opcode = QString::fromStdString(instr.opcode);
            QString operands;

            // 遍历操作数并拼接
            for (const auto& operand : instr.operands) {
                operands += QString::fromStdString(operand) + " ";
            }

            // 去掉最后一个空格
            if (!operands.isEmpty()) {
                operands.chop(1);
            }

            // 拼接指令和操作数
            instStr += opcode + "\t" + operands + "\n";
        }


        showInst(instStr);

        // 代码生成
        QString machineCode;
        for (const auto& instr : instructions) {
            machineCode += QString::fromStdString(generateMachineCode(instr));
            machineCode += "\n";
        }

        showMachine(machineCode);
        }
        catch (const runtime_error& e) {
              QMessageBox::critical(nullptr, "Error", QString::fromStdString(e.what()));
        }
}

