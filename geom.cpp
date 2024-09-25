#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm> // for sorting

using namespace std;

/* **************************************** */
/* returns the signed area of triangle abc. The area is positive if c
   is to the left of ab, and negative if c is to the right of ab
 */
int signed_area2D(point2d a, point2d b, point2d c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

/* **************************************** */
/* return 1 if p,q,r are collinear, and 0 otherwise */
int collinear(point2d p, point2d q, point2d r) {
    return signed_area2D(p, q, r) == 0;
}

/* **************************************** */
/* return 1 if c is strictly left of ab; 0 otherwise */
int left_strictly(point2d a, point2d b, point2d c) {
    return signed_area2D(a, b, c) > 0;
}

/* return 1 if c is left of ab or on ab; 0 otherwise */
int left_on(point2d a, point2d b, point2d c) {
    return signed_area2D(a, b, c) >= 0;
}

/* Global variable to store the reference point for sorting */
point2d point_start;

/* Utility function to calculate the polar angle for sorting */
bool compare_angle(point2d p1, point2d p2) {
    int area = signed_area2D(point_start, p1, p2);
    if (area == 0) {
        // If points are collinear, the closer point comes first
        return (p1.x - point_start.x) * (p1.x - point_start.x) + (p1.y - point_start.y) * (p1.y - point_start.y) <
               (p2.x - point_start.x) * (p2.x - point_start.x) + (p2.y - point_start.y) * (p2.y - point_start.y);
    }
    return area > 0; // Sort by counterclockwise angle
}

// Compute the convex hull of pts and store the points on the hull in hull
void graham_scan(vector<point2d>& pts, vector<point2d>& hull) {
    printf("hull2d (graham scan): start\n");
    hull.clear(); // Should be empty, but clear it to be safe

    // Step 1: Find the point with the lowest y-coordinate (and lowest x if ties)
    point_start = pts[0];
    for (int i = 1; i < pts.size(); i++) {
        if (pts[i].y < point_start.y || (pts[i].y == point_start.y && pts[i].x < point_start.x)) {
            point_start = pts[i];
        }
    }

    // Step 2: Sort points by polar angle with the reference being point_start
    sort(pts.begin(), pts.end(), compare_angle);

    // Step 3: Build the hull
    hull.push_back(point_start);
    for (int i = 1; i < pts.size(); i++) {
        // Pop the last point from the hull while it does not form a left turn
        while (hull.size() > 1 && !left_strictly(hull[hull.size() - 2], hull.back(), pts[i])) {
            hull.pop_back();
        }
        hull.push_back(pts[i]);
    }

    printf("hull2d (graham scan): end\n");
}
