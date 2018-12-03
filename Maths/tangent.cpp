inline float
isLeft( Point P0, Point P1, Point P2 )
{
    return (P1.x - P0.x)*(P2.y - P0.y) - (P2.x - P0.x)*(P1.y - P0.y);
}


// tests for polygon vertex ordering relative to a fixed point P
#define above(P,Vi,Vj)  (isLeft(P,Vi,Vj) > 0)   // true if Vi is above Vj
#define below(P,Vi,Vj)  (isLeft(P,Vi,Vj) < 0)   // true if Vi is below Vj

// tangent_PointPolyC(): fast binary search for tangents to a convex polygon
//    Input:  P = a 2D point (exterior to the polygon)
//            n = number of polygon vertices
//            V = array of vertices for a 2D convex polygon with V[n] = V[0]
//    Output: *rtan = index of rightmost tangent point V[*rtan]
//            *ltan = index of leftmost tangent point V[*ltan]
void
tangent_PointPolyC( Point P, int n, Point* V, int* rtan, int* ltan )
{
    *rtan = Rtangent_PointPolyC(P, n, V);
    *ltan = Ltangent_PointPolyC(P, n, V);
}


// Rtangent_PointPolyC(): binary search for convex polygon right tangent
//    Input:  P = a 2D point (exterior to the polygon)
//            n = number of polygon vertices
//            V = array of vertices for a 2D convex polygon with V[n] = V[0]
//    Return: index "i" of rightmost tangent point V[i]
int
Rtangent_PointPolyC( Point P, int n, Point* V )
{
    // use binary search for large convex polygons
    int     a, b, c;            // indices for edge chain endpoints
    int     upA, dnC;           // test for up direction of edges a and c

    // rightmost tangent = maximum for the isLeft() ordering
    // test if V[0] is a local maximum
    if (below(P, V[1], V[0]) && !above(P, V[n-1], V[0]))
        return 0;               // V[0] is the maximum tangent point

    for (a=0, b=n;;) {          // start chain = [0,n] with V[n]=V[0]
        c = (a + b) / 2;        // midpoint of [a,b], and 0<c<n
        dnC = below(P, V[c+1], V[c]);
        if (dnC && !above(P, V[c-1], V[c]))
            return c;          // V[c] is the maximum tangent point

        // no max yet, so continue with the binary search
        // pick one of the two subchains [a,c] or [c,b]
        upA = above(P, V[a+1], V[a]);
        if (upA) {                       // edge a points up
            if (dnC)                         // edge c points down
                 b = c;                           // select [a,c]
            else {                           // edge c points up
                 if (above(P, V[a], V[c]))     // V[a] above V[c]
                     b = c;                       // select [a,c]
                 else                          // V[a] below V[c]
                     a = c;                       // select [c,b]
            }
        }
        else {                           // edge a points down
            if (!dnC)                        // edge c points up
                 a = c;                           // select [c,b]
            else {                           // edge c points down
                 if (below(P, V[a], V[c]))     // V[a] below V[c]
                     b = c;                       // select [a,c]
                 else                          // V[a] above V[c]
                     a = c;                       // select [c,b]
            }
        }
    }
}
 


// Ltangent_PointPolyC(): binary search for convex polygon left tangent
//    Input:  P = a 2D point (exterior to the polygon)
//            n = number of polygon vertices
//            V = array of vertices for a 2D convex polygon with V[n]=V[0]
//    Return: index "i" of leftmost tangent point V[i]
int
Ltangent_PointPolyC( Point P, int n, Point* V )
{
    // use binary search for large convex polygons
    int     a, b, c;            // indices for edge chain endpoints
    int     dnA, dnC;           // test for down direction of edges a and c

    // leftmost tangent = minimum for the isLeft() ordering
    // test if V[0] is a local minimum
    if (above(P, V[n-1], V[0]) && !below(P, V[1], V[0]))
        return 0;               // V[0] is the minimum tangent point

    for (a=0, b=n;;) {          // start chain = [0,n] with V[n] = V[0]
        c = (a + b) / 2;        // midpoint of [a,b], and 0<c<n
        dnC = below(P, V[c+1], V[c]);
        if (above(P, V[c-1], V[c]) && !dnC)
            return c;          // V[c] is the minimum tangent point

        // no min yet, so continue with the binary search
        // pick one of the two subchains [a,c] or [c,b]
        dnA = below(P, V[a+1], V[a]);
        if (dnA) {                       // edge a points down
            if (!dnC)                        // edge c points up
                 b = c;                           // select [a,c]
            else {                           // edge c points down
                 if (below(P, V[a], V[c]))     // V[a] below V[c]
                     b = c;                       // select [a,c]
                 else                          // V[a] above V[c]
                     a = c;                       // select [c,b]
            }
        }
        else {                           // edge a points up
            if (dnC)                         // edge c points down
                 a = c;                           // select [c,b]
            else {                           // edge c points up
                 if (above(P, V[a], V[c]))     // V[a] above V[c]
                     b = c;                       // select [a,c]
                 else                          // V[a] below V[c]
                     a = c;                       // select [c,b]
            }
        }
    }
}
//===================================================================