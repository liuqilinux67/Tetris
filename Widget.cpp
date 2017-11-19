#include "Widget.h"
#include <QPainter>
#include <QBrush>
#include <QTime>
#include <QDebug>
#include <QKeyEvent>
#include <QtTest/QTest>
#include <QMessageBox>
#include <QTimer>
//定义图案
//田
int item1[4][4] = {
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0},
    {0,0,0,0}
};
//一
int item2[4][4] = {
    {1,1,1,1},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
};
//左L
int item3[4][4] = {
    {1,1,1,0},
    {0,0,1,0},
    {0,0,0,0},
    {0,0,0,0}
};
//右L
int item4[4][4] = {
    {0,1,1,1},
    {0,1,0,0},
    {0,0,0,0},
    {0,0,0,0}
};
//山
int item5[4][4] = {
    {1,1,1,0},
    {0,1,0,0},
    {0,0,0,0},
    {0,0,0,0}
};
//左-|_
int item6[4][4] = {
    {0,1,1,0},
    {0,0,1,1},
    {0,0,0,0},
    {0,0,0,0}
};
//右_|-
int item7[4][4] = {
    {0,1,1,0},
    {1,1,0,0},
    {0,0,0,0},
    {0,0,0,0}
};
Widget::Widget(QWidget *parent)
    : QWidget(parent),xy{{0}}
{
    setFixedSize(COLUMN * BLOCK_SIZE + MARGIN * 4 + BLOCK_SIZE * 4, ROW * BLOCK_SIZE + MARGIN * 2);
    qsrand(QTime::currentTime().second() * 1000 + QTime::currentTime().msec());
    m_mark = 0;
    initButton();
    initGame();

}

void Widget::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    QBrush brush(Qt::white);
    painter.setBrush(brush);
    //画游戏场景边框
    painter.drawRect(MARGIN, MARGIN, COLUMN * BLOCK_SIZE, ROW * BLOCK_SIZE);
    brush.setColor(Qt::blue);
    painter.setBrush(brush);
    //画已经存在方块
    remove();
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 12; j++)
        {
            if(xy[i][j] == 1)
            {
                painter.drawRect(j * BLOCK_SIZE + MARGIN, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);
            }



        }
    }
    //画方块预告
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {

            if(m_curBlock[j][i] == 1)
            {
                int x = (COLUMN / 2 + i - 2 + m_offset_x) * BLOCK_SIZE + MARGIN;
                int y = (j + m_offset_y) * BLOCK_SIZE + MARGIN;
                brush.setColor(color1);
                painter.setBrush(brush);
                painter.drawRect(x, y, BLOCK_SIZE, BLOCK_SIZE);
            }

            if(m_nextBlock[j][i] == 1)
            {
                int x = (COLUMN + i) * BLOCK_SIZE + 2 * MARGIN;
                int y = j * BLOCK_SIZE + MARGIN;
                brush.setColor(color2);
                painter.setBrush(brush);
                painter.drawRect(x, y, BLOCK_SIZE, BLOCK_SIZE);
            }
        }
    }
    //更新坐标，检测碰撞
    //updateCoord();
    //isCollision();
    //绘制得分
    painter.setPen(QPen(Qt::black));
    painter.setFont(QFont("Arial",12));
    painter.drawText(COLUMN * BLOCK_SIZE + MARGIN * 2, 4 *BLOCK_SIZE + MARGIN, 4 * BLOCK_SIZE, 4 * BLOCK_SIZE, Qt::AlignCenter, "SCORE:" + QString::number(m_score));


}
//碰撞检测
bool Widget::isCollision()
{

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(m_curBlock[i][j] == 1)
            {
                int x = j + 4 + m_offset_x;
                int y = i + m_offset_y;
                //qDebug() << "i=" << i << "x=" << x << "y=" << y;
                //左右边界，下边界
                if(x < 0 || x > 11 || y > 19)
                    return true;
                //判断当前块是否占用，遍历
                if(xy[y][x] == 1)
                {
                     return true;
                }

            }
        }
    }
    return false;




}

void Widget::keyPressEvent(QKeyEvent* event)
{

    if(event->key() == Qt::Key_Up)
    {
        int m[4][4];
        copyBlock(m, m_curBlock);
        rotateBlock();
        if(isCollision())
            copyBlock(m_curBlock, m);
        else
            update();
    }
    if(event->key() == Qt::Key_Left)
    {

        m_offset_x--;
        if(isCollision())
            m_offset_x++;
        else
            update();


    }
    if(event->key() == Qt::Key_Right)
    {
        m_offset_x++;
        if(isCollision())
            m_offset_x--;
        else
            update();

    }
    //如果不能向下走，说明到底了。进行下个图形更新，把当前图形更新到全局块。
    if(event->key() == Qt::Key_Down)
    {
        m_offset_y++;
        if(isCollision())
        {
             m_offset_y--;
             //判断游戏结束
             qDebug() << m_offset_y;
             if(m_offset_y == 0)
             {
                 int ret = QMessageBox::warning(this, tr("提示"), tr("游戏结束！"), QMessageBox::Ok);
                 if(ret == QMessageBox::Ok)
                     startGame();
                 return;
             }
             //把当前块添加到全局块
             for(int i = 0; i < 4; i++)
             {
                 for(int j =0; j < 4; j++)
                 {
                     if(m_curBlock[i][j] == 1)
                     {
                         int x = j + 4 + m_offset_x;
                         int y = i + m_offset_y;
                         //qDebug() << x << ":" << y;
                         xy[y][x] = 1;
                     }
                 }
             }
             //更新颜色
             updateColor();
             //更新全局块后
             copyBlock(m_curBlock, m_nextBlock);
             //生成新的next块
             randBlock(m_nextBlock);
             //偏移量值为0
             m_offset_x = m_offset_y = 0;
             update();
        }

        else
            update();

    }


}

Widget::~Widget()
{

}

void Widget::initGame()
{

    m_offset_x = 0;
    m_offset_y = 0;
    m_score = 0;

    randBlock(m_curBlock);
    randBlock(m_nextBlock);


}

void Widget::rotateBlock()
{
    int m[4][4] = {0};
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(i == 0)
                m[i][j] = 0;
            else
                m[i][j] = m_curBlock[i - 1][j];
        }
    }
    //逆时针旋转
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            m[3 - j][i] = m_curBlock[i][j];
        }
    }
    //向上查找空格
    for(int i = 0; i < 4; i++)
    {
        int mark = 0;
        for(int j = 0; j < 4; j++)
        {
            if(m[i][j] == 0)
                mark++;
            else
                break;
        }
        if(mark == 4)
        {
            //向上移动
            for(int i = 0; i < 4; i++)
            {
                for(int j = 0; j < 4; j++)
                {
                     if(i == 3)
                         m[i][j] = 0;
                     else
                         m[i][j] = m[i + 1][j];
                }
            }
            i--;
        }
        else
            break;
    }
    //copy到m_curBlock;
    copyBlock(m_curBlock, m);

}

void Widget::randBlock(int block[4][4])       //生成随机块
{

    int m = qrand() % 6 + 1;

    //qDebug() << m;
    switch(m)
    {
    case 1:
        copyBlock(block, item1);
        break;
    case 2:
        copyBlock(block, item2);
        break;
    case 3:
        copyBlock(block, item3);
        break;
    case 4:
        copyBlock(block, item4);
        break;
    case 5:
        copyBlock(block, item5);
        break;
    case 6:
        copyBlock(block, item6);
        break;
    default:
        copyBlock(block, item7);
        break;

    }

}

void Widget::copyBlock(int a[4][4], int b[4][4])
{
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            a[i][j] = b[i][j];
}

void Widget::remove()
{

    //从底层行开始遍历，查找该行是否全为1
    for(int i = 19; i >= 0; i--)
    {
        int mark = 0;
        int row;
        for(int j = 0; j < 12; j++)
        {
            if(xy[i][j] == 0)
                break;
            else
                mark++;
        }
        //若该行全为1
        if(mark == 12)
        {
            //qDebug() << "mark == 12";
            row = i;
            //删除当前行,增加分数
            m_score++;
            //上面行往下面移动
            for(int i = row - 1; i >= 0; i--)
            {
                //如果当前行全部为0，不在循环查找。
                int mark = 0;
                for(int j = 0; j < 12; j++)
                {
                    if(xy[i][j] == 1)
                        break;
                    else
                        break;
                }
                if(mark == 12)
                {
                    for(int j = 0; j < 12; j++)
                        xy[i + 1][j] = 0;
                    break;
                }

                for(int j = 0; j < 12; j++)
                {
                    xy[i + 1][j] = xy[i][j];
                }
            }
            i++;

        }

    }

}

void Widget::updateColor()
{
    color1 = static_cast<Qt::GlobalColor>(qrand() % 16 + 1);
    color2 = static_cast<Qt::GlobalColor>(qrand() % 16 + 1);
}

void Widget::initButton()
{
    m_startBtn.setParent(this);
    m_pauseBtn.setParent(this);
    m_quitBtn.setParent(this);
    //m_rankBtn.setParent(this);
    m_startBtn.setText("start/restart");
    m_pauseBtn.setText("pause");
    m_quitBtn.setText("quit");
    m_rankBtn.setText("rank list");
    //固定布局
    m_startBtn.move(320, 310);
    m_pauseBtn.move(320, 360);
    m_rankBtn.move(320, 410);
    m_quitBtn.move(320, 460);

    m_timer.setParent(this);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    connect(&m_startBtn, SIGNAL(clicked(bool)), this ,SLOT(startGame()));
    connect(&m_pauseBtn, SIGNAL(clicked(bool)), this, SLOT(pauseGame()));
    connect(&m_quitBtn, SIGNAL(clicked(bool)), this ,SLOT(close()));
}

void Widget::startGame()
{
    for(int i = 0; i < 20; i++)
        for(int j = 0; j < 12; j++)
            xy[i][j] = 0;
    if(m_mark)
        initGame();
    m_mark++;
    m_timer.start(1000);
    setFocus();
}

void Widget::pauseGame()
{
    m_timer.stop();
    int ret = QMessageBox::information(this, tr("提示"), tr("暂停中！"), QMessageBox::Ok);
    if(ret == QMessageBox::Ok)
            m_timer.start(1000);
    setFocus();

}

void Widget::handleTimeout()
{
    QTest::keyClick(this, Qt::Key_Down);
}


