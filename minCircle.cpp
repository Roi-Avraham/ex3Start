#include "minCircle.h"
#include <vector>
#include "math.h"

double distance(const Point& a, const Point& b)
{
    return sqrt(((a.x - b.x) * (a.x - b.x))
                + ((a.y - b.y) * (a.y - b.y)));
}

Circle circleFromThreePoints(Point a, Point b, Point c){
    Point middlePointAB((a.x + b.x)/ 2, (a.y +b.y) / 2); // mid point of line AB
    float slopAB = (b.y - a.y) / (b.x - a.x); // the slop of AB
    float pSlopAB = - 1/slopAB; // the perpendicular slop of AB
    Point middlePointBC((b.x + c.x)/ 2, (b.y +c.y) / 2); // mid point of line BC
    float slopBC = (c.y - b.y) / (c.x - b.x); // the slop of BC
    float pSlopBC = - 1/slopBC; // the perpendicular slop of BC
    float x = ((pSlopAB * middlePointAB.x - middlePointAB.y) -pSlopBC*middlePointBC.x + middlePointBC.y)
            / (pSlopAB - pSlopBC);
    float y = -pSlopAB * (middlePointAB.x - x) + middlePointAB.y;
    Point center(x,y);
    float radius=distance(center,a);
    return Circle(center,radius);
}

Circle circleFromTwoPoints(const Point& A, const Point& B)
{
    // Set the center to be the midpoint of A and B
    Point C((A.x + B.x) / 2.0, (A.y + B.y) / 2.0);
    Circle circle(C, distance(A, B) / 2.0 );
    return circle;
}

bool realCircle(const Circle& c,
                     const vector<Point>& P)
{

    // Iterating through all the points
    // to check  whether the points
    // lie inside the circle or not
    for (const Point& p : P)
        if (distance(c.center, p) >= c.radius)
            return false;
    return true;
}

Circle simpleSolution(vector<Point>& points)
{
    if (points.empty()) {
        return Circle(Point(0,0), 0);
    }
    else if (points.size() == 1) {
        return Circle(points[0],0);
    }
    else if (points.size() == 2) {
        return circleFromTwoPoints(points[0], points[1]);
    }

    // To check if MEC can be determined
    // by 2 points only
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {

            Circle c = circleFromTwoPoints(points[i], points[j]);
            if (realCircle(c, points))
                return c;
        }
    }
    return circleFromThreePoints(points[0], points[1], points[2]);
}

Circle complexSolution(Point** points,
                    vector<Point> boundary, int size)
{
    // Base case when all points processed or |boundary| = 3
    if (size == 0 || boundary.size() == 3) {
        return simpleSolution(boundary);
    }

    // Pick a random point randomly
    int index = rand() % size;
    Point p(points[index]->x,points[index]->y);

    // Put the picked point at the end of P
    // since it's more efficient than
    // deleting from the middle of the vector
    swap(points[index], points[size - 1]);

    // Get the MEC circle d from the
    // set of points - {p}
    Circle d = complexSolution(points, boundary, size - 1);

    // If d contains p, return d
    if (distance(d.center, p) <= d.radius) {
        return d;
    }

    // Otherwise, must be on the boundary of the MEC
    boundary.push_back(p);

    // Return the MEC for points - {p} and boundary U {p}
    return complexSolution(points, boundary, size - 1);
}

Circle findMinCircle(Point** points,size_t size) {
    return complexSolution(points, {}, size);
}
