/**
 * Authors:
 *  name: Asaf Mesilaty.
 *  ID: 318646072.
 *
 *  name: Dan Marom.
 *  ID: 208995035.
 */

#include "minCircle.h"
#include "random"

/**
 * This function returns the distance between two points following this  formula:
 * distance((x1,y1), (x2,y2)) = sqrt( (x2-x1)^2 + (y2-y1)^2 )
 *
 * @param a is the 1st point.
 * @param b is the 2nd point.
 * @return the distance between a and b.
 */
float distance(Point a, Point b)
{
    float x2=(a.x-b.x)*(a.x-b.x);
    float y2=(a.y-b.y)*(a.y-b.y);
    return sqrtf(x2+y2);
}

/**
 * This function checks whether a point lies inside or on the boundaries of the circle.
 * by checking if point p is at most radius + 1 away from C.center.
 *
 * @param c is the circle we check with.
 * @param p is the point we examine if inside the circle c.
 * @return true if it is inside, else false.
 */
bool isInside(Point p, Circle c)
{
    return distance(p, c.center) <= c.radius;
}


/*
    The following two functions are used
    To find the equation of the circle when
    three points are given.
 */
/**
 * Helper function to get a circle defined by 3 points.
 * given three points that are not on the same line - there is a unique circle which those three points on it's edge.
 * (x1 - h)^2 + (y1 - k)^2 = r^2
 * (x2 - h)^2 + (y2 - k)^2 = r^2
 * (x3 - h)^2 + (y3 - k)^2 = r^2
 * by solving for h,k we get the center of the circle (h,k).
 *
 * @param bx stands for x-Axis difference between Point B and Point A (B.x - A.x).
 * @param by stands for y-Axis difference between Point B and Point A (B.y - A.y).
 * @param cx stands for x-Axis difference between Point C and Point A (C.x - A.x).
 * @param cy stands for y-Axis difference between Point C and Point A (C.y - A.y).
 * @return the center of the circle generated with A, B and C.
 */
Point getCircleCenter(float bx, float by, float cx, float cy)
{
    float B = bx * bx + by * by;
    float C = cx * cx + cy * cy;
    float D = bx * cy - by * cx;
    float centerX = ((cy * B - by * C) / (2 * D));
    float centerY = ((bx * C - cx * B) / (2 * D));
    return {centerX, centerY};
}

/**
 * This function returns a unique circle that intersects three points.
 * For further info about the method itself, check getCircleCenter documentation.
 *
 * @param A is the 1st Point.
 * @param B is the 2nd Point.
 * @param C is the 3rd Point.
 * @return Circle generated by A, B and C.
 */
Circle circle_from(Point A, Point B, Point C) {
    Point I = getCircleCenter(B.x - A.x, B.y - A.y,
                              C.x - A.x, C.y - A.y);

    I.x += A.x;
    I.y += A.y;
    return {I,((distance(I, A)))};
}

/**
 *  This function returns the smallest circle that intersects with 2 points.
 *
 * @param a is the 1st Point.
 * @param b is the 2nd Point.
 * @return the circle that intersects with A and B.
 */
Circle circle_from(Point a, Point b)
{
    // Set the center to be the midpoint of A and B
    float x = (a.x+b.x)/2;
    float y = (a.y+b.y)/2;

    // Set the radius to be half the distance AB
    float r = distance(a, b) / 2;
    return {Point(x,y),r};
}

/**
 * This function returns the minimum enclosing circle for N <= 3, P is originally R from findMinCircleHelper.
 *
 * @param P is a set of points.
 * @return circle matching above documentation description.
 */
Circle baseCase(vector<Point>& P)
{

    if (P.empty())
        return { { 0, 0 }, 0 };
    else if (P.size() == 1)
        return { P[0], 0 };
    else if (P.size() == 2)
        return circle_from(P[0], P[1]);

    // Check if MEC can be determined by 2 points only
    Circle c = circle_from(P[0],P[1]);
    if(isInside(P[2], c))
        return c;
    c = circle_from(P[0],P[2]);
    if(isInside(P[1], c))
        return c;
    c = circle_from(P[1],P[2]);
    if(isInside(P[0], c))
        return c;

    // if not - return the unique circle defined by 3 points
    return circle_from(P[0], P[1], P[2]);
}


/**
* This function returns the Minimum Enclosing Circle (MEC) of a given list of points,
* using Welzl's algorithm.
* Each step of the recursion we randomly pick a point p from P, and create MEC without it.
* If the MEC includes p, then we return this MEC.
* Otherwise, p is on MEC boundaries, therefore added to R.
*
* @param P set of input points
* @param R set of points on the circle boundary.
* @param n number of points in P that are yet to be processed.
* @return  P'S minimum enclosing circle.
*/
Circle findMinCircleHelper(Point** P,vector<Point> R, size_t n){
    if(n==0 || R.size()==3){
        return baseCase(R);
    }

    // remove random point p
    // swap is more efficient than remove
    int i = rand()%n;
    Point p(P[i]->x,P[i]->y);
    swap(P[i],P[n-1]);

    Circle c = findMinCircleHelper(P,R,n-1);

    if(isInside(p, c))
        return c;

    R.push_back(p);

    return findMinCircleHelper(P,R,n-1);
}

/**
 * This function find Minimum Enclosing Circle (MEC) of a given set of Points.
 *
 * @param points is the given set of points to find their MEC.
 * @param size is the size of the set of points.
 * @return MEC of points.
 */
Circle findMinCircle(Point** points, size_t size)
{
    return findMinCircleHelper(points, {}, size);
}

