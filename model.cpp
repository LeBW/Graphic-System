#include <glwidget.h>
#include <cmath>

void drawEightPoint(PixelPoint *current, PixelPoint centre);
void drawFourPoint(PixelPoint *current, PixelPoint *centre);

//填充算法需要用到的一些结构
//定义活性边表AET和新边表NET
typedef struct XET
{
    float x;
    float dx, ymax;
    XET* next;
} AET, NET;

//算法具体实现

static void ddaDrawLine(PixelPoint from, PixelPoint to){
    double m = (to.y - from.y) / (to.x - from.x); //斜率为m
    double delta = 1;
   // printf("draw line\n");
    if (m < -1 || m > 1){
        if (to.y < from.y)
            delta = -1;
        while (from.y != to.y){
            glBegin(GL_POINTS);
            glVertex2i(from.x, from.y);
            glEnd();
            from.y += delta;
            from.x += delta * 1 / m;
        }
    }
    else if (m> -1 && m < 1){
        if (to.x < from.x)
            delta = -1;
        while (from.x != to.x){
            glBegin(GL_POINTS);
            glVertex2i(from.x, from.y);
            glEnd();
            from.x += delta;
            from.y += delta * m;
        }
    }

}
static void bresenhamDrawCircle(Circle circle){
    //printf("draw circle from (%d, %d) to (%d, %d)", from.x, from.y, to.x, to.y);
    int radius = circle.radius;
    PixelPoint centre = circle.center;
    //确定好圆心和半径，接下来画圆。
    PixelPoint *current = new PixelPoint(0, radius);
    double p = 1.25 - radius;
    drawEightPoint(current, centre);
    while (current->x < current->y) {
        current->x = current->x + 1;
        if (p >= 0)
        {
            current->y = current->y - 1;
            p = p + 2 * current->x + 3 - 2 * current->y + 2;
        }
        else {
            p = p + 2 * current->x + 3;
        }
        drawEightPoint(current, centre);//画出八个八分圆中对应的点

    }
}
static void bresenhamDrawOval(PixelPoint from, PixelPoint to) {
    PixelPoint *centre = new PixelPoint((int)(from.x + to.x) / 2, (int)(from.y + to.y) / 2);
    int rx = abs(from.x - to.x) / 2;
    int ry = abs(from.y - to.y) / 2;
    double sqa = rx*rx;
    double sqb = ry*ry;
    double p = sqb - rx * rx * ry + rx * rx / 4;
    if (1) {
        PixelPoint *current = new PixelPoint(0, ry);
        drawFourPoint(current, centre);
        //区域1
        while (sqb * (current->x + 1) < sqa * (current->y-0.5)) {
            if (p >= 0) {
                p += 2 * sqb * current->x - 2 * sqa * current->y + 2 * sqa + 3 * sqb;
                current->y -= 1;
            }
            else {
                p += 2 * sqb * current->x + 3 * sqb;
            }
            current->x += 1;
            drawFourPoint(current, centre);
        }
        //区域2
        //用区域1最后的点来计算区域2中p的初始值
        p = sqb*(current->x + 0.5)*(current->x + 0.5) + sqa *(current->y - 1)*(current->y - 1) - sqa*sqb;
        while (current->y > 0) {
            if (p > 0) {
                p = p - 2 * sqa*current->y + 3*sqa;
            }
            else{
                p = p - 2 * sqa *current->y + 3*sqa + sqb*(2*current->x+2);
                current->x += 1;
            }
            current->y -= 1;
            drawFourPoint(current, centre);
        }

    }
    else{

    }
}
void drawMypolygon(Mypolygon polygon) {
    std::vector<PixelPoint> points = polygon.points;
    int num = points.size();
    if (num == 1) {
        glBegin(GL_POINTS);
        glVertex2i(points[0].x, points[0].y);
        glEnd();
    }
    for (int i = 0; i < num - 1; i++) {
        ddaDrawLine(points[i], points[i + 1]);
    }
    //填充图元
    if (polygon.whetherFull){
        int maxY = 0;
        int minY = HEIGHT;
        for (int i = 0; i < num; i++) {
            if (points[i].y > maxY)
                maxY = points[i].y;
            if (points[i].y < minY)
                minY = points[i].y;
        }
        //初始化aet表
        AET *pAET = new AET();
        pAET->next = NULL;
        //初始化net表
        NET *pNET[HEIGHT];
        for (int i = minY; i <= maxY; i++) {
            pNET[i] = new NET;
            pNET[i]->next = NULL;
        }
        glBegin(GL_POINTS);
        for (int i = minY; i <= maxY; i++){
            for (int j = 0; j < num; j++){
                if (points[j].y == i) {
                    //一个点跟前面的一个点形成一条线段，跟后面的点也形成线段
                    if (points[(j - 1 + num) % num].y > points[j].y)
                    {
                        NET *p = new NET;
                        p->x = points[j].x;
                        p->ymax = points[(j - 1 + num) % num].y;
                        p->dx = (points[(j - 1 + num) % num].x - points[j].x) / (points[(j - 1 + num) % num].y - points[j].y);
                        p->next = pNET[i]->next;
                        pNET[i]->next = p;
                    }
                    if (points[(j + 1 + num) % num].y>points[j].y)
                    {
                        NET *p = new NET;
                        p->x = points[j].x;
                        p->ymax = points[(j + 1 + num) % num].y;
                        p->dx = (points[(j + 1 + num) % num].x - points[j].x) / (points[(j + 1 + num) % num].y - points[j].y);
                        p->next = pNET[i]->next;
                        pNET[i]->next = p;
                    }
                }
            }
        }
        //建立并更新活化边表
        for (int i = minY; i <= maxY; i++)
        {
            //计算新的交点x,更新AET
            NET *p = pAET->next;
            while (p)
            {
                p->x = p->x + p->dx;
                p = p->next;
            }
            //更新后新AET先排序
            //断表排序,不再开辟空间
            AET *tq = pAET;
            p = pAET->next;
            tq->next = NULL;
            while (p)
            {
                while (tq->next && p->x >= tq->next->x)
                    tq = tq->next;
                NET *s = p->next;
                p->next = tq->next;
                tq->next = p;
                p = s;
                tq = pAET;
            }
            //(改进算法)先从AET表中删除ymax==i的结点
            AET *q = pAET;
            p = q->next;
            while (p)
            {
                if (p->ymax == i)
                {
                    q->next = p->next;
                    delete p;
                    p = q->next;
                }
                else
                {
                    q = q->next;
                    p = q->next;
                }
            }
            //将NET中的新点加入AET,并用插入法按X值递增排序
            p = pNET[i]->next;
            q = pAET;
            while (p)
            {
                while (q->next && p->x >= q->next->x)
                    q = q->next;
                NET *s = p->next;
                p->next = q->next;
                q->next = p;
                p = s;
                q = pAET;
            }
            //配对填充颜色

            p = pAET->next;
            while (p && p->next)
            {
                for (float j = p->x; j <= p->next->x; j++)
                    glVertex2i(static_cast<int>(j), i);
                p = p->next->next;//考虑端点情况
            }
        }
        glEnd();
    }
}



void drawEightPoint(PixelPoint *current, PixelPoint centre) {
    PixelPoint *another = new PixelPoint(current->y, current->x);
    glBegin(GL_POINTS);
    glVertex2i(current->x + centre.x, current->y + centre.y);
    glVertex2i(-current->x + centre.x, current->y + centre.y);
    glVertex2i(current->x + centre.x, -current->y + centre.y);
    glVertex2i(-current->x + centre.x, -current->y + centre.y);
    glVertex2i(another->x + centre.x, another->y + centre.y);
    glVertex2i(-another->x + centre.x, another->y + centre.y);
    glVertex2i(another->x + centre.x, -another->y + centre.y);
    glVertex2i(-another->x + centre.x, -another->y + centre.y);
    glEnd();
}
void drawFourPoint(PixelPoint *current, PixelPoint *centre) {
    glBegin(GL_POINTS);
    glVertex2i(current->x + centre->x, current->y + centre->y);
    glVertex2i(-current->x + centre->x, current->y + centre->y);
    glVertex2i(current->x + centre->x, -current->y + centre->y);
    glVertex2i(-current->x + centre->x, -current->y + centre->y);
    glEnd();
}
//通用方法
void drawLines(){

    for (int i = 0; i < (int)lines.size(); i++)
    {
        ddaDrawLine(lines[i].from, lines[i].to);
        /*
        glBegin(GL_LINES);
        glVertex2f(lines[i].from.x / (WIDTH / 2) - 1, -lines[i].from.y / (HEIGHT / 2) + 1);
        glVertex2f(lines[i].to.x / (WIDTH / 2) - 1, -lines[i].to.y / (HEIGHT / 2) + 1);
        glEnd();
        */

    }
}
void drawCircles(){
    for (int i = 0; i < (int)circles.size(); i++) {
        bresenhamDrawCircle(circles[i]);
    }
}
void drawOvals(){
    for (int i = 0; i < (int)ovals.size(); i++) {
        bresenhamDrawOval(ovals[i].from, ovals[i].to);
    }
}
void drawMypolygons() {
    for (int i = 0; i < (int)mypolygons.size(); i++) {
        drawMypolygon(mypolygons[i]);
    }
}
void drawHighligh() {
    glPointSize(2);
    switch(selectedShape.shape) {
    case line:
        ddaDrawLine(lines[selectedShape.index].from, lines[selectedShape.index].to);
        break;
    case circle:
        bresenhamDrawCircle(circles[selectedShape.index]);
        break;
    case oval:
        bresenhamDrawOval(ovals[selectedShape.index].from, ovals[selectedShape.index].to);
        break;
    case polygon:
    case filledPolygon:
        drawMypolygon(mypolygons[selectedShape.index]);
        break;
    default:
        break;
    }
    glPointSize(1);
}
