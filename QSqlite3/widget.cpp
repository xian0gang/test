#include "widget.h"
#include "ui_widget.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QSqlQuery>
#include <QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QVector<int> tempFromNode;
    QVector<int> tempToNode;
    QSet <int> tempSort;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("cie"); // 数据库名与路径, 此时是放在同目录下
        // db.setDatabaseName("offset.db"); // 数据库名与路径, 此时是放在同目录下
        bool ok = db.open(); // 连接数据库, 然后就可以使用了.
        if(ok){
            qDebug("open success!!!");
            QSqlQuery query;
                if (query.exec("select * from cie;"))   //尝试列出  表的所有记录
                {  //本次查询成功
                    qDebug("select cie success!!!");
                    int numRows = 0;  //询问数据库驱动，是否驱动含有某种特性

                        if (db.driver()->hasFeature(QSqlDriver::QuerySize))
                        {
                            numRows = query.size();  //如果支持结果影响的行数，那么直接记录下来
                        }
                        else
                        {
                            query.last(); //否则定位到结果最后，qt 文档说，这个方法非常慢
                            numRows = query.at() + 1;
                            query.seek(-1);
                        }
                }
                                //尝试列出  表的所有记录
                while (query.next()) {

                    int num =0;
                    /*QString FNode_ID = query.value(4).toString();
                    QString TNode_ID = query.value(5).toString();*/
                    int FNode_ID = query.value(3).toInt();
                    int TNode_ID = query.value(4).toInt();

                    tempFromNode.append(FNode_ID);
                    tempToNode.append(TNode_ID);

                }
            }
            else{
              //qDebug() << "cannot open database.";
//                printf( "cannot open database.");
        }

}

Widget::~Widget()
{
    delete ui;
}
