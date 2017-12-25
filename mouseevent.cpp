#include <glwidget.h>
#include <cmath>
using namespace std;
//全局变量定义
bool polygonIsDrawing = false;
SelectedShape selectedShape;

//definition of private methods
double pointToLine(int x, int y, Line line);
bool isInsideThePolygon(Mypolygon mypolygon, int x, int y);
bool onSegment(PixelPoint p, PixelPoint q, PixelPoint r);
void selectShape(int x, int y);
void pointRotateAroundPoint(PixelPoint &point, PixelPoint center, double angle);
void pointZoomAroundPoint(PixelPoint &point, PixelPoint center, double ratio);

//内部变量定义
static PixelPoint startPoint;

void GLWidget::mousePressEvent(QMouseEvent *event) {
    int index;
    int x = event->x();
    int y = event->y();
    //("click down\n");
    delete cutRect;
    cutRect = NULL;
    switch (currentState) {
    case line:
        index = lines.size()-1;
        lines.push_back(*new Line);
        index++;
        //("Click down\n");
        lines[index].from = *new PixelPoint(x, y);
        lines[index].to = lines[index].from;
        break;
    case circle:
        index = circles.size() - 1;
        circles.push_back(*new Circle);
        index++;
        circles[index].from = *new PixelPoint(x, y);
        circles[index].to = circles[index].from;
        break;
    case oval:
        index = ovals.size()-1;
        ovals.push_back(*new Oval);
        index++;
        ovals[index].from = *new PixelPoint(x, y);
        ovals[index].to = ovals[index].from;
        break;
    case polygon:
        if (event->button() == Qt::LeftButton) {
            if (!polygonIsDrawing) {
                mypolygons.push_back(*new Mypolygon());
                polygonIsDrawing = true;
            }
            index = mypolygons.size() - 1;
            mypolygons[index].points.push_back(*new PixelPoint(x, y));
        }
        else if(event->button() == Qt::RightButton) {
            if(polygonIsDrawing) {
                index = mypolygons.size()-1;
                int num = mypolygons[index].points.size();
                mypolygons.at(index).points.push_back(mypolygons.at(index).points[0]);
                polygonIsDrawing = false;
                int centerX = 0, centerY = 0;
                for(int i = 0; i < num; i++) {
                    centerX += mypolygons[index].points[i].x;
                    centerY += mypolygons[index].points[i].y;
                }
                centerX /= num;
                centerY /= num;
                mypolygons[index].center.x = centerX;
                mypolygons[index].center.y = centerY;
            }
        }
        break;
    case filledPolygon:
        if (event->button() == Qt::LeftButton) {
            if (!polygonIsDrawing) {
                mypolygons.push_back(*new Mypolygon());
                polygonIsDrawing = true;
            }
            index = mypolygons.size() - 1;
            mypolygons[index].points.push_back(*new PixelPoint(x, y));
        }
        else if(event->button() == Qt::RightButton) {
            if(polygonIsDrawing) {
                index = mypolygons.size()-1;
                int num = mypolygons[index].points.size();
                mypolygons[index].whetherFull = true;
                mypolygons.at(index).points.push_back(mypolygons.at(index).points[0]);
                polygonIsDrawing = false;
                int centerX = 0, centerY = 0;
                for(int i = 0; i < num; i++) {
                    centerX += mypolygons[index].points[i].x;
                    centerY += mypolygons[index].points[i].y;
                }
                centerX /= num;
                centerY /= num;
                mypolygons[index].center.x = centerX;
                mypolygons[index].center.y = centerY;
            }
        }
        break;
    case makeFull:
        if(event->button() == Qt::LeftButton) {
            selectShape(x, y);
        }
        if (selectedShape.isSelected == true && selectedShape.shape == polygon)
        {
            //mypolygons[selectedShape.index].whetherFull = ~mypolygons[selectedShape.index].whetherFull;
            if(mypolygons[selectedShape.index].whetherFull)
                mypolygons[selectedShape.index].whetherFull = false;
            else
                mypolygons[selectedShape.index].whetherFull = true;
        }
        break;
    case edit:
        if(event->button() == Qt::LeftButton) {
            //左键选中图形
            selectShape(x, y);
        }
        else if(event->button() == Qt::RightButton) {
            //右键删除图形
            if(selectedShape.isSelected == true) {
                switch (selectedShape.shape) {
                case line:{
                    vector<Line>::iterator lineIterator = lines.begin() + selectedShape.index;
                    lines.erase(lineIterator);
                    break;
                }
                case circle:{
                    vector<Circle>::iterator circleIterator = circles.begin() + selectedShape.index;
                    circles.erase(circleIterator);
                    break;
                }
                case oval:{
                    vector<Oval>::iterator ovalIterator = ovals.begin() + selectedShape.index;
                    ovals.erase(ovalIterator);
                    break;
                }
                case polygon: {
                    vector<Mypolygon>::iterator polygonIterator = mypolygons.begin() + selectedShape.index;
                    mypolygons.erase(polygonIterator);
                    break;
                }
                default:
                    break;
                }
                selectedShape.isSelected = false;
            }
        }
        break;
    case parallel:
        selectShape(x, y);
        if(selectedShape.isSelected) {
            startPoint.x = x;
            startPoint.y = y;
        }
        break;
    case myrotate:
        selectShape(x, y);

        break;
    case zoom:
        selectShape(x, y);
        break;
    case cut:
        if (event->button() == Qt::LeftButton)
            selectShape(x, y);
        else if(event->button() == Qt::RightButton && selectedShape.isSelected && (selectedShape.shape == line || selectedShape.shape == polygon) ){
            cutRect = new Rect();
            cutRect->from = *new PixelPoint(x, y);
            cutRect->to = *new PixelPoint(x, y);
        }
        break;
    default:
        break;
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() == Qt::LeftButton) {
        int x = event->x();
        int y = event->y();
        int index;
        switch (currentState) {
        case line:
            index = lines.size() - 1;
            lines[index].to = *new PixelPoint(x, y);
            break;
        case circle: {
            index = circles.size() - 1;
            circles[index].to = *new PixelPoint(x, y);
            PixelPoint from = circles[index].from;
            PixelPoint to = circles[index].to;
            bool flag = abs(from.x - to.x) > abs(from.y - to.y);  //若生成矩形的x大于y
            circles[index].radius = (flag ? abs(from.x - to.x) : abs(from.y - to.y)) / 2;
            int radius = circles[index].radius;
            PixelPoint centre;
            if (flag){
                centre.x = (int)(from.x + to.x) / 2;
                if (from.y > to.y)
                    centre.y = from.y - radius;
                else
                    centre.y = from.y + radius;
            }
            else{
                centre.y = (int)(from.y + to.y) / 2;
                if (from.x > to.x)
                    centre.x = from.x - radius;
                else
                    centre.x = from.x + radius;
            }
            circles[index].center = centre;
            break;
        }
        case oval:
            index = ovals.size() - 1;
            ovals[index].to = *new PixelPoint(x, y);
            break;
        case parallel:
            if (selectedShape.isSelected) {
                int deltaX = x - startPoint.x;
                int deltaY = y - startPoint.y;
                startPoint.x = x;
                startPoint.y = y;
                int n = 0;
                int index = selectedShape.index;
                switch(selectedShape.shape) {
                case line:
                    lines[index].from.x += deltaX;
                    lines[index].to.x += deltaX;
                    lines[index].from.y += deltaY;
                    lines[index].to.y += deltaY;
                    break;
                case circle:
                    circles[index].from.x += deltaX;
                    circles[index].to.x += deltaX;
                    circles[index].from.y += deltaY;
                    circles[index].to.y += deltaY;
                    circles[index].center.x += deltaX;
                    circles[index].center.y += deltaY;
                    break;
                case oval:
                    ovals[index].from.x += deltaX;
                    ovals[index].to.x += deltaX;
                    ovals[index].from.y += deltaY;
                    ovals[index].to.y += deltaY;
                    ovals[index].center.x += deltaX;
                    ovals[index].center.y += deltaY;
                    break;
                case polygon:
                case filledPolygon:
                    n = mypolygons[selectedShape.index].points.size();
                    for(int i = 0;i < n; i++){
                        mypolygons[selectedShape.index].points[i].x += deltaX;
                        mypolygons[selectedShape.index].points[i].y += deltaY;
                    }
                    mypolygons[selectedShape.index].center.x += deltaX;
                    mypolygons[selectedShape.index].center.y += deltaY;
                    break;
                default:
                    break;
                }
            }
            break;
        default:
            break;
        }
    }
    else if (event->buttons() == Qt::RightButton) {
        int x = event->x();
        int y = event->y();
        switch (currentState) {
        case cut:
            if(cutRect != NULL) {
              cutRect->to.x = x;
              cutRect->to.y = y;
            }
            break;
        default:
            break;
        }
    }

}
void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    int index;
    int x = event->x();
    int y = event->y();
    switch (currentState) {
    case line:
        index = lines.size()-1;
        lines[index].to = *new PixelPoint(x, y);
        //("click up.There are %d lines\n", index+1);
        lines[index].center = *new PixelPoint((x + (int)lines[index].from.x)/2, (int)(y + (int)lines[index].from.y)/2);
        //printf("(%f %f) (%f %f) center(%f %f)\n", lines[index].from.x, lines[index].from.y, lines[index].to.x, lines[index].to.y, lines[index].center.x, lines[index].center.y);
        break;
    case circle: {
        index = circles.size()-1;
        circles[index].to = *new PixelPoint(x, y);
        PixelPoint from = circles[index].from;
        PixelPoint to = circles[index].to;
        bool flag = abs(from.x - to.x) > abs(from.y - to.y);  //若生成矩形的x大于y
        circles[index].radius = (flag ? abs(from.x - to.x) : abs(from.y - to.y)) / 2;
        int radius = circles[index].radius;
        PixelPoint centre;
        if (flag){
            centre.x = (int)(from.x + to.x) / 2;
            if (from.y > to.y)
                centre.y = from.y - radius;
            else
                centre.y = from.y + radius;
        }
        else{
            centre.y = (int)(from.y + to.y) / 2;
            if (from.x > to.x)
                centre.x = from.x - radius;
            else
                centre.x = from.x + radius;
        }
        circles[index].center = centre;
        break;
    }
    case oval:
        index = ovals.size() - 1;
        ovals[index].to = *new PixelPoint(x, y);
        ovals[index].center = *new PixelPoint((x+ovals[index].from.x)/2, (y+ovals[index].from.y)/2);
        break;
    case cut:
        if (event->button() == Qt::RightButton && cutRect != NULL) {
            cutRect->to.x = x;
            cutRect->to.y = y;
        }
        break;
    default:
        break;
    }
}
void GLWidget::wheelEvent(QWheelEvent *event) {
    if(!selectedShape.isSelected)
        return;
    switch (currentState) {
    case myrotate:
        if(event->delta() > 0) {
            switch (selectedShape.shape) {
            case line:{
                Line line = lines[selectedShape.index];
                pointRotateAroundPoint(lines[selectedShape.index].from, line.center, M_PI/18);
                pointRotateAroundPoint(lines[selectedShape.index].to, line.center, M_PI/18);
                break;
            }
            case polygon:
            case filledPolygon:{
                Mypolygon *polygon = &mypolygons[selectedShape.index];
                int num = polygon->points.size();
                for(int i = 0; i < num; i++) {
                    pointRotateAroundPoint(polygon->points[i], polygon->center, M_PI/18);
                }
                break;
            }
            default:
                break;
            }
        }
        else {
            switch (selectedShape.shape) {
            case line:{
                Line line = lines[selectedShape.index];
                pointRotateAroundPoint(lines[selectedShape.index].from, line.center, -M_PI/18);
                pointRotateAroundPoint(lines[selectedShape.index].to, line.center, -M_PI/18);
                break;
            }
            case polygon:
            case filledPolygon:{
                Mypolygon *polygon = &mypolygons[selectedShape.index];
                int num = polygon->points.size();
                for(int i = 0; i < num; i++) {
                    pointRotateAroundPoint(polygon->points[i], polygon->center, -M_PI/18);
                }
                break;
            }
            default:
                break;
            }
        }
        break;
    case zoom:
        if(event->delta() > 0) {
            switch (selectedShape.shape) {
            case line:{
                Line line = lines[selectedShape.index];
                pointZoomAroundPoint(lines[selectedShape.index].from, line.center, 1.1);
                pointZoomAroundPoint(lines[selectedShape.index].to, line.center, 1.1);
                break;
            }
            case circle:{
                Circle circle = circles[selectedShape.index];
                pointZoomAroundPoint(circles[selectedShape.index].from, circle.center, 1.1);
                pointZoomAroundPoint(circles[selectedShape.index].to, circle.center, 1.1);
                circles[selectedShape.index].radius *= 1.1;
                break;
            }
            case oval:{
                Oval oval = ovals[selectedShape.index];
                pointZoomAroundPoint(ovals[selectedShape.index].from, oval.center, 1.1);
                pointZoomAroundPoint(ovals[selectedShape.index].to, oval.center, 1.1);
                break;
            }
            case polygon:
            case filledPolygon:{
                Mypolygon *polygon = &mypolygons[selectedShape.index];
                int num = polygon->points.size();
                for(int i = 0; i < num; i++) {
                    pointZoomAroundPoint(polygon->points[i], polygon->center, 1.1);
                }
                break;
            }
            default:
                break;
            }
        }
        else {
            switch (selectedShape.shape) {
            case line:{
                Line line = lines[selectedShape.index];
                pointZoomAroundPoint(lines[selectedShape.index].from, line.center, 0.9);
                pointZoomAroundPoint(lines[selectedShape.index].to, line.center, 0.9);
                break;
            }
            case circle:{
                Circle circle = circles[selectedShape.index];
                pointZoomAroundPoint(circles[selectedShape.index].from, circle.center, 0.9);
                pointZoomAroundPoint(circles[selectedShape.index].to, circle.center, 0.9);
                circles[selectedShape.index].radius *= 0.9;
                break;
            }
            case oval:{
                Oval oval = ovals[selectedShape.index];
                pointZoomAroundPoint(ovals[selectedShape.index].from, oval.center, 0.9);
                pointZoomAroundPoint(ovals[selectedShape.index].to, oval.center, 0.9);
                break;
            }
            case polygon:
            case filledPolygon:{
                Mypolygon *polygon = &mypolygons[selectedShape.index];
                int num = polygon->points.size();
                for(int i = 0; i < num; i++) {
                    pointZoomAroundPoint(polygon->points[i], polygon->center, 0.9);
                }
                break;
            }
            default:
                break;
            }
        }
        break;
    default:
        break;
    }
}
void GLWidget::keyPressEvent(QKeyEvent *event) {
    switch (currentState) {
    case cut:
        if(cutRect != NULL) {
            if(event->key() == Qt::Key_Escape)  {
                delete cutRect;
                cutRect = NULL;
            }
            else if(event->key() == Qt::Key_Return) {
                switch(selectedShape.shape) {
                case line:
                    cutLine();
                    break;
                case polygon:
                    cutPolygon();
                    break;
                default:
                    break;
                }
                delete cutRect;
                cutRect = NULL;
            }
        }
        selectedShape.isSelected = false;
        break;
    default:
        break;
    }
}

/***********************************private mathods ****************************************/
//计算点到直线的距离
double pointToLine(int x, int y, Line line) {
    double a = line.from.y - line.to.y;
    double b = line.to.x - line.from.x;
    double c = line.from.x * line.to.y - line.to.x * line.from.y;
    return abs(a * x + b * y + c) / sqrt(a*a + b*b);
}
//若p q r满足线性关系，判断q是否落在线段pr上。
bool onSegment(PixelPoint p, PixelPoint q, PixelPoint r)
{
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
            q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
        return true;
    return false;
}
//判断三个点的方向状况。为0时线性，为1时顺时针，为2时逆时针
int orientation(PixelPoint p, PixelPoint q, PixelPoint r) {
    int val = (q.y - p.y)*(r.x- q.x) - (q.x-p.x)*(r.y-q.y);
    if(val == 0) return 0;
    return (val > 0) ? 1 : 2;
}
//如果线段p1q1和p2q2相交，返回true
bool doIntersect(PixelPoint p1, PixelPoint q1, PixelPoint p2, PixelPoint q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);
    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}

bool isInsideThePolygon(Mypolygon mypolygon, int x, int y) {
    int n = mypolygon.points.size();
    PixelPoint p(x, y);
    if(n < 3) return false;
    PixelPoint extreme(WIDTH, y);
    int count = 0, i = 0;
    do {
        int next = (i+1)%n;
        if(doIntersect(mypolygon.points[i], mypolygon.points[next], p, extreme)) {
            if(orientation(mypolygon.points[i], p, mypolygon.points[next]) == 0)
                return onSegment(mypolygon.points[i], p, mypolygon.points[next]);
            count++;
        }
        i = next;
    }while (i != 0);
    return count & 1;
}
void selectShape(int x, int y) {
    selectedShape.isSelected = false;
    for(int i = 0; i < (int)lines.size(); i++) {
        double distance = pointToLine(x, y, lines[i]);
       // ("%f\n", distance);
        //("%f\n",abs(abs(x-lines[i].from.x)*abs(lines[i].from.y-lines[i].to.y) - abs(lines[i].from.x-lines[i].to.x)*abs(y-lines[i].from.y)));
        PixelPoint p(x,y);
        if(distance <= 3 && onSegment(lines[i].from, p, lines[i].to))
        {
            selectedShape.isSelected = true;
            selectedShape.index = i;
            selectedShape.shape = line;
            return;
        }
    }
    for(int i = 0; i < (int)circles.size(); i++) {
        int fromX = circles[i].from.x, fromY = circles[i].from.y;
        int toX = circles[i].to.x, toY = circles[i].to.y;
        bool flag = abs(fromX - toX) > abs(fromY - toY);  //若生成矩形的x大于y
        double radius = (flag ? abs(fromX - toX) : abs(fromY - toY)) / 2;
        PixelPoint centre;
        if (flag){
            centre.x = (int)(fromX + toX) / 2;
            if (fromY > toY)
                centre.y = fromY - radius;
            else
                centre.y = fromY + radius;
        }
        else {
            centre.y = (int)(fromY + toY) / 2;
            if (fromX > toX)
                centre.x = fromX - radius;
            else
                centre.x = fromX + radius;
        }
        if(sqrt((x-centre.x)*(x-centre.x) + (y-centre.y)*(y-centre.y)) < radius) {
            selectedShape.isSelected = true;
            selectedShape.index = i;
            selectedShape.shape = circle;
            return;
        }
    }
    for(int i = 0; i < (int)ovals.size(); i++) {
        int minX = ovals[i].from.x < ovals[i].to.x ? ovals[i].from.x : ovals[i].to.x;
        int maxX = ovals[i].from.x < ovals[i].to.x ? ovals[i].to.x : ovals[i].from.x;
        int minY = ovals[i].from.y < ovals[i].to.y ? ovals[i].from.y : ovals[i].to.y;
        int maxY = ovals[i].from.y < ovals[i].to.y ? ovals[i].to.y : ovals[i].from.y;
        if(x > minX && x < maxX && y > minY && y < maxY) {
            selectedShape.isSelected = true;
            selectedShape.index = i;
            selectedShape.shape = oval;
            return;
        }
    }
    for(int i = 0; i < (int)mypolygons.size(); i++) {
        if (isInsideThePolygon(mypolygons[i], x, y)) {
            selectedShape.isSelected = true;
            selectedShape.index = i;
            selectedShape.shape = polygon;
            return;
        }
    }
}

void pointRotateAroundPoint(PixelPoint &point, PixelPoint center, double angle) {
    int x, y;
    int a = center.x;
    int b = center.y;
    int x0 = point.x;
    int y0 = point.y;
    x = a + (x0-a)*cos(angle) - (y0-b)*sin(angle);
    y = b + (y0-b)*cos(angle) + (x0-a)*sin(angle);
    point.x = x;
    point.y = y;
}
void pointZoomAroundPoint(PixelPoint &point, PixelPoint center, double ratio){
    int x, y;
    int a = center.x;
    int b = center.y;
    int x0 = point.x;
    int y0 = point.y;
    x = a + (x0-a)*ratio;
    y = b + (y0-b)*ratio;
    point.x = x;
    point.y = y;
}
