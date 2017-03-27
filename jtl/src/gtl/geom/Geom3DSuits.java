package gtl.geom;


import gtl.common.Variant;
import gtl.math.MathSuits;

/**
 * Created by ZhenwenHe on 2017/3/15.
 */
public class Geom3DSuits extends GeomSuits {


    /**
     * dot product (3D) which  allows vector operations in arguments
     * #define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)
     * #define norm(v)    sqrt(dot(v,v))  // norm = length of  vector
     * #define d(P,Q)     norm(P-Q)        // distance = norm of difference
     * @param P
     * @param Q
     * @return
     */
    public static double distance(Vector P, Vector Q){
        Vector v= Q.subtract(P) ;
        return Math.sqrt(v.dotProduct(v));
        //return MathSuits.distance(a.getCoordinates(),b.getCoordinates());
    }

    /**
     *
     * @param P
     * @param L
     * @return the shortest distance from P to L
     * ref:http://geomalgorithms.com/a02-_lines.html
     */
    public static double perpendicularDistance(Vector P, InfiniteLine L){
        Vector P0=L.startPoint;
        Vector P1=L.endPoint;
        Vector v = P1.subtract(P0);//P1 - P0;
        Vector w = P.subtract(P0);//P - P0;

        double c1 = w.dotProduct(v);//dot(w,v);
        double c2 = v.dotProduct(v);//dot(v,v);
        Scalar b = new Scalar(c1 / c2);
        //double b = c1 / c2;
        //Point Pb = L.P0 + b * v;
        Vector Pb = P0.add(b.multiply(v));
        return distance(P, Pb);
    }

    /**
     *
     * @param P
     * @param L
     * @return return the foot of perpendicular to L
     */
    public Vector perpendicularFoot(Vector P, InfiniteLine L){
        Vector P0=L.startPoint;
        Vector P1=L.endPoint;
        Vector v = P1.subtract(P0);//P1 - P0;
        Vector w = P.subtract(P0);//P - P0;

        double c1 = w.dotProduct(v);//dot(w,v);
        double c2 = v.dotProduct(v);//dot(v,v);
        Scalar b = new Scalar(c1 / c2);
        //double b = c1 / c2;
        //Point Pb = L.P0 + b * v;
        Vector Pb = P0.add(b.multiply(v));
        return Pb;
    }

    /**
     *
     * @param P
     * @param L
     * @return return the foot of perpendicular to S
     */
    public Vector perpendicularFoot(Vector P , LineSegment S){
        Vector P0=S.startPoint;
        Vector P1=S.endPoint;
        Vector v = P1.subtract(P0);//P1 - P0;
        Vector w = P.subtract(P0);//P - P0;

        double c1 = w.dotProduct(v);//dot(w,v);
        double c2 = v.dotProduct(v);//dot(v,v);
        Scalar b = new Scalar(c1 / c2);
        //double b = c1 / c2;
        //Point Pb = S.P0 + b * v;
        Vector Pb = P0.add(b.multiply(v));

        //TODO : Pb is on S

        return Pb;
    }

    /**
     *
     * @param P
     * @param PL
     * @return return the foot of perpendicular to PL
     */

    public static Vector perpendicularFoot( Vector P, Plane PL) {
        double    sn, sd;
        Vector V0=PL.getVertices()[0];
        Vector N =PL.getNormal();
        //sn = -dot( PL.n, (P - PL.V0));
        sn = - N.dotProduct(P.subtract(V0));
        //sd = dot(PL.n, PL.n);
        sd = N.dotProduct(N);
        //*B = P + sb * PL.n;
        Scalar sb=new Scalar(sn / sd);
        return P.add(sb.multiply(N));
    }

    /**
     *
     * @param P
     * @param S
     * @return the shortest distance from P to S
     * ref:http://geomalgorithms.com/a02-_lines.html
     */
    public static double shortestDistance(Vector P , LineSegment S){
        Vector P0 = S.startPoint;
        Vector P1 = S.endPoint;

        Vector v = P1.subtract(P0);
        Vector w = P.subtract(P0);

        double c1 = w.dotProduct(v);
        if ( c1 <= 0 )
            return distance(P, P0);

        double c2 = v.dotProduct(v);
        if ( c2 <= c1 )
            return distance(P, P1);

        //double b = c1 / c2;
        Scalar b= new Scalar(c1/c2);
        Vector Pb = P0.add(b.multiply(v));
        return distance(P, Pb);
    }

    /**
     *
     * @param L1
     * @param L2
     * @return the shortest distance between L1 and L2
     * ref:http://geomalgorithms.com/a07-_distance.html
     */
    public static double shortestDistance(InfiniteLine L1, InfiniteLine L2){
        Vector   u = L1.endPoint.subtract(L1.startPoint);//L1.P1 - L1.P0;
        Vector   v = L2.endPoint.subtract(L2.startPoint);//L2.P1 - L2.P0;
        Vector   w = L1.startPoint.subtract(L2.startPoint);//L1.P0 - L2.P0;
        double    a = dotProduct(u,u);         // always >= 0
        double    b = dotProduct(u,v);
        double    c = dotProduct(v,v);         // always >= 0
        double    d = dotProduct(u,w);
        double    e = dotProduct(v,w);
        double    D = a*c - b*b;        // always >= 0
        double    sc, tc;

        // compute the line parameters of the two closest points
        if (D < MathSuits.EPSILON) {          // the lines are almost parallel
            sc = 0.0;
            tc = (b>c ? d/b : e/c);    // use the largest denominator
        }
        else {
            sc = (b*e - c*d) / D;
            tc = (a*e - b*d) / D;
        }
        Scalar scScalar=new Scalar(sc);
        Scalar tcScalar=new Scalar(tc);
        // get the difference of the two closest points
        //Vector   dP = w + (sc * u) - (tc * v);  // =  L1(sc) - L2(tc)
        Vector   dP = w.add(scScalar.multiply(u)).subtract(tcScalar.multiply(v));
        return normalize(dP);   // return the closest distance
    }

    /**
     *
     * @param S1
     * @param S2
     * @return the shortest distance between S1 and S2
     * ref:http://geomalgorithms.com/a07-_distance.html
     */
    public static double shortestDistance(LineSegment S1,LineSegment S2){
        Vector   u = subtract(S1.endPoint,S1.startPoint);//S1.P1 - S1.P0;
        Vector   v = subtract(S2.endPoint,S2.startPoint);//S2.P1 - S2.P0;
        Vector   w = subtract(S1.startPoint,S2.startPoint);//S1.P0 - S2.P0;
        double    a = dotProduct(u,u);         // always >= 0
        double    b = dotProduct(u,v);
        double    c = dotProduct(v,v);         // always >= 0
        double    d = dotProduct(u,w);
        double    e = dotProduct(v,w);
        double    D = a*c - b*b;        // always >= 0
        double    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
        double    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0

        // compute the line parameters of the two closest points
        if (D < MathSuits.EPSILON) { // the lines are almost parallel
            sN = 0.0;         // force using point P0 on segment S1
            sD = 1.0;         // to prevent possible division by 0.0 later
            tN = e;
            tD = c;
        }
        else {                 // get the closest points on the infinite lines
            sN = (b*e - c*d);
            tN = (a*e - b*d);
            if (sN < 0.0) {        // sc < 0 => the s=0 edge is visible
                sN = 0.0;
                tN = e;
                tD = c;
            }
            else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
                sN = sD;
                tN = e + b;
                tD = c;
            }
        }

        if (tN < 0.0) {            // tc < 0 => the t=0 edge is visible
            tN = 0.0;
            // recompute sc for this edge
            if (-d < 0.0)
                sN = 0.0;
            else if (-d > a)
                sN = sD;
            else {
                sN = -d;
                sD = a;
            }
        }
        else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
            tN = tD;
            // recompute sc for this edge
            if ((-d + b) < 0.0)
                sN = 0;
            else if ((-d + b) > a)
                sN = sD;
            else {
                sN = (-d +  b);
                sD = a;
            }
        }
        // finally do the division to get sc and tc
        sc = (Math.abs(sN) < MathSuits.EPSILON ? 0.0 : sN / sD);
        tc = (Math.abs(tN) < MathSuits.EPSILON ? 0.0 : tN / tD);
        Scalar scScalar=new Scalar(sc);
        Scalar tcScalar=new Scalar(tc);
        // get the difference of the two closest points
        //Vector   dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)
        Vector   dP = w.add(scScalar.multiply(u)).subtract(tcScalar.multiply(v));
        return normalize(dP);   // return the closest distance
    }

    /**
     * dist_Point_to_Plane(): get distance (and perp base) from a point to a plane
     * Input:  P  = a 3D point
     * PL = a  plane with point V0 and normal n
     * Output: B = base point on PL of perpendicular from P
     * Return: the distance from P to the plane PL
     * @param P
     * @param PL
     * @param basePoint base point on PL of perpendicular from P
     * @return the distance from P to the plane PL
     */
    public static double perpendicularDistance( Vector P, Plane PL, Vector basePoint) {
        double    sn, sd;
        Vector V0=PL.getVertices()[0];
        Vector N =PL.getNormal();
        //sn = -dot( PL.n, (P - PL.V0));
        sn = - N.dotProduct(P.subtract(V0));
        //sd = dot(PL.n, PL.n);
        sd = N.dotProduct(N);
        //*B = P + sb * PL.n;
        Scalar sb=new Scalar(sn / sd);
        Vector B = P.add(sb.multiply(N));
        if(basePoint!=null) B.copyTo(basePoint);
        return distance(P,B);
    }

    /**求 C 点在直线 AB 上的垂线距离
     *
     */
    public static double perpendicularDistance(Vector vA, Vector vB, Vector vC){
        int dims=java.lang.Math.min(java.lang.Math.min(vA.getDimension(),vB.getDimension()),vC.getDimension());
        double [] v1=new double[dims];
        double [] v2=new double[dims];
        double [] P =new double[dims];
        double [] A = vA.getCoordinates();
        double [] B = vB.getCoordinates();
        double [] C = vC.getCoordinates();
        for(int i=0;i<dims;++i){
            v1[i]=A[i]-B[i];
            v2[i]=C[i]-B[i];
        }
        double t = MathSuits.dotProduct(A,B);
        if ( Math.abs(t)<MathSuits.EPSILON ) {
            t=0;
        }
        else {
            t = MathSuits.dotProduct(v1, v2)/t;
        }
        for(int i=0;i<dims;++i){
            P[i] = B[i] + v1[i]* t;
        }
        return MathSuits.distance(C, P);
    }

    /**
     * 求 C 点在直线 AB 上的垂足点P，并返回垂直距离，
     * @param vA
     * @param vB
     * @param vC
     * @param vP
     * @param onAB 如果点P在线段AB上，返回真，否则返回假
     * @return
     */
    public static double perpendicularDistance(Vector vA, Vector vB, Vector vC, Vector vP, Variant onAB){
        int dims=java.lang.Math.min(java.lang.Math.min(vA.getDimension(),vB.getDimension()),vC.getDimension());
        double [] v1=new double[dims];
        double [] v2=new double[dims];
        double [] P =new double[dims];
        double [] A = vA.getCoordinates();
        double [] B = vB.getCoordinates();
        double [] C = vC.getCoordinates();
        for(int i=0;i<dims;++i){
            v1[i]=A[i]-B[i];
            v2[i]=C[i]-B[i];
        }
        double t = MathSuits.dotProduct(A,B);
        if ( Math.abs(t)<MathSuits.EPSILON ) {
            t=0;
        }
        else {
            t = MathSuits.dotProduct(v1, v2)/t;
        }
        for(int i=0;i<dims;++i){
            P[i] = B[i] + v1[i]* t;
        }

        if(vP!=null){
            vP.reset(P);
        }
        if(onAB!=null){
            if ( (t<0)||(t>1) ){
                onAB.reset(false);
            }
            else {
                onAB.reset(true);
            }
        }
        return MathSuits.distance(C, P);
    }

    /**
     * // return the original vector length
     * @return
     */
    public static double normalize(Vector v){
        return v.normalize();
    }

    /**
     *
     * @param v
     * @param v2
     * @return
     */
    public static double dotProduct(Vector v, Vector v2){
        return v.dotProduct(v2);
    }
    /**
     * Computes the dot product of the 3D vectors AB and CD.
     *
     * @param A
     * @param B
     * @param C
     * @param D
     * @return the dot product
     */
    public static double dotProduct(Vector A, Vector B, Vector C, Vector D){
        Vector v1=B.subtract(A);
        Vector v2=D.subtract(C);
        return v1.dotProduct(v2);
    }


    /**
     * // a X  b
     * @param a
     * @param b
     * @return
     */
    public static Vector crossProduct(Vector a, Vector b){
        return a.crossProduct(b);
    }

    /**
     *
     * @param o
     * @param a
     * @param b
     * @return
     */
    public static  double angle(Vector o, Vector a , Vector b){
        return o.angle(a,b);
    }

    /**
     *
     * @param a
     * @param b
     * @return
     */
    public static Vector subtract  (Vector a, Vector b){
        return a.subtract(b);
    }

    /**
     *
     * @param a
     * @param b
     * @return
     */
    public static Vector add(Vector a, Vector b){
        return a.add(b);
    }

    /**
     * Closest Point of Approach (CPA)
     * The "Closest Point of Approach" refers to the positions at which two dynamically moving objects reach their closest possible distance.
     * This is an important calculation for collision avoidance. In many cases of interest, the objects, referred to as "tracks",
     * are points moving in two fixed directions at fixed speeds. That means that the two points are moving along two lines in space.
     * However, their closest distance is not the same as the closest distance between the lines since the distance between the points must be computed at the same moment in time.
     * So, even in 2D with two lines that intersect, points moving along these lines may remain far apart. But if one of the tracks is stationary,
     * then the CPA of another moving track is at the base of the perpendicular from the first track to the second's line of motion.
     * cpa_time(): compute the time of CPA for two tracks
     * Input:  two tracks Tr1 and Tr2
     *
     * @param Tr1
     * @param Tr2
     * @return the time at which the two tracks are closest
     * ref:http://geomalgorithms.com/a07-_distance.html
     */
    public  static  double cpaTime( Track Tr1, Track Tr2 ) {
        Vector   dv = subtract(Tr1.velocity ,Tr2.velocity);

        double    dv2 = dotProduct(dv,dv);
        if (dv2 < MathSuits.EPSILON)      // the  tracks are almost parallel
            return 0.0;             // any time is ok.  Use time 0.

        Vector   w0 = subtract(Tr1.origin , Tr2.origin);
        double    cpatime = -dotProduct(w0,dv) / dv2;

        return cpatime;             // time of CPA
    }

    /**
     * Closest Point of Approach (CPA)
     * The "Closest Point of Approach" refers to the positions at which two dynamically moving objects reach their closest possible distance.
     * This is an important calculation for collision avoidance. In many cases of interest, the objects, referred to as "tracks",
     * are points moving in two fixed directions at fixed speeds. That means that the two points are moving along two lines in space.
     * However, their closest distance is not the same as the closest distance between the lines since the distance between the points must be computed at the same moment in time.
     * So, even in 2D with two lines that intersect, points moving along these lines may remain far apart. But if one of the tracks is stationary,
     * then the CPA of another moving track is at the base of the perpendicular from the first track to the second's line of motion.
     * cpa_distance(): compute the distance at CPA for two tracks
     * @param Tr1
     * @param Tr2
     * @return the distance for which the two tracks are closest
     * ref: http://geomalgorithms.com/a07-_distance.html
     */
    public static double cpaDistance( Track Tr1, Track Tr2 ) {
        double    ctime = cpaTime( Tr1, Tr2);
        Vector    P1 = add(Tr1.origin , Tr1.velocity.multiply(ctime));
        Vector    P2 = add(Tr2.origin , Tr2.velocity.multiply(ctime));

        return distance(P1,P2);            // distance at CPA
    }

    /**
     *  find the 3D intersection of a segment and a plane
     *  S = a segment, and Pn = a plane = {Point V0;  Vector n;}
     * @param S
     * @param Pn
     * @param outPoint  the intersect point (when it exists)
     * @return  0 = disjoint (no intersection)
     *           1 =  intersection in the unique point *I0
     *           2 = the segment lies in the plane
     */
    public static int intersection(LineSegment S, Plane Pn,Vector outPoint){
        Vector SP1 = S.endPoint;
        Vector SP0=S.startPoint;
        Vector PnV0=Pn.getVertices()[0];
        Vector Pnn=Pn.getNormal();
        //Vector u = S.P1 - S.P0;
        Vector u = SP1.subtract(SP0);
        //Vector w = S.P0 - Pn.V0;
        Vector w=SP0.subtract(PnV0);
        double D = dotProduct(Pnn, u);
        double N = -dotProduct(Pnn, w);

        if (Math.abs(D) < MathSuits.EPSILON) { // segment is parallel to plane
            if (N == 0) // segment lies in plane
                return 2;
            else
            return 0;// no intersection
        }
        // they are not parallel
        // compute intersect param
        double sI = N / D;
        if (sI < 0 || sI > 1)
            return 0; // no intersection

        //*I = S.P0 + sI * u; // compute segment intersect point
        outPoint.copyFrom(u.multiply(sI).add(SP0));
        return 1;
    }

    /**
     *find the 3D intersection of two planes
     * @param Pn1
     * @param Pn2
     * @param outLine  the intersection line (when it exists)
     * @return 0 = disjoint (no intersection)
     *          1 = the two  planes coincide
     *          2 =  intersection in the unique line *L
     */
    public static int intersection(Plane Pn1,Plane Pn2,InfiniteLine outLine){
        Vector Pn1V0=Pn1.getVertices()[0];
        Vector Pn2V0=Pn2.getVertices()[0];
        Vector Pn1n=Pn1.getNormal();
        Vector Pn2n=Pn2.getNormal();
        Vector u = crossProduct(Pn1n,Pn2n); // cross product
        double ax = (u.getX() >= 0 ? u.getX() : -u.getX());
        double ay = (u.getY() >= 0 ? u.getY() : -u.getY());
        double az = (u.getZ() >= 0 ? u.getZ() : -u.getZ());

        // test if the two planes are parallel
        if ((ax+ay+az) < MathSuits.EPSILON) { // Pn1 and Pn2 are near parallel
        // test if disjoint or coincide
            Vector  v = Pn2V0.subtract(Pn1V0);
            if (dotProduct(Pn1n, v) == 0) // Pn2V0 lies in Pn1
                return 1;  // Pn1 and Pn2 coincide
            else
                return 0; // Pn1 and Pn2 are disjoint
        }
        // Pn1 and Pn2 intersect in a line
        // first determine max abs coordinate of cross product
        int maxc=0; // max coordinate
        if (ax > ay) {
            if (ax > az)
                maxc =1;
            else
                maxc = 3;
        }
        else {
            if (ay > az)
                maxc = 2;
            else
                maxc = 3;
        }
        // next, to get a point on the intersect line
        // zero the max coord, and solve for the other two
        double x=0,y=0,z=0;//Vector iP;  // intersect point
        double d1, d2; // the constants in the 2 plane equations
        d1 = -dotProduct(Pn1n, Pn1V0); // note: could be pre-stored  with plane
        d2 = -dotProduct(Pn2n, Pn2V0); // ditto
        switch (maxc) { // select max coordinate
            case 1:// intersect with x=0
            {
                x = 0;
                y = (d2*Pn1n.getZ() - d1*Pn2n.getZ()) /u.getX();
                z = (d1*Pn2n.getY() - d2*Pn1n.getY()) /u.getX();
                break;
            }
            case 2: // intersect with y=0
            {
                x = (d1 * Pn2n.getZ() - d2 * Pn1n.getZ()) / u.getY();
                y = 0;
                z = (d2 * Pn1n.getX() - d1 * Pn2n.getX()) / u.getY();
                break;
            }
            case 3:// intersect with z=0
            {
                x = (d2 * Pn1n.getY() - d1 * Pn2n.getY()) / u.getZ();
                y = (d1 * Pn2n.getX() - d2 * Pn1n.getX()) / u.getZ();
                z = 0;
                break;
            }
        }
        Vector iP = new VectorImpl(x,y,z);
        outLine.reset(iP,iP.add(u));
        return 2;
    }

    /**
     * find the 3D intersection of a ray with a triangle
     * @param R
     * @param T
     * @param outPoint intersection point (when it exists)
     * @return -1 = triangle is degenerate (a segment or point)
     *          0 =  disjoint (no intersect)
     *          1 =  intersect in unique point outPoint
     *          2 =  are in the same plane
     */
    public static int intersection(Ray R , Triangle T, Vector outPoint){
        Vector TV0=T.getVertex(0);
        Vector TV1=T.getVertex(1);
        Vector TV2=T.getVertex(2);
        Vector RP0=R.startPoint;
        Vector RP1=R.endPoint;
        Vector u, v, n;// triangle vectors
        Vector dir, w0, w;// ray vectors
        double r, a, b;// params to calc ray-plane intersect

        // get triangle edge vectors and plane normal
        u = subtract(TV1 ,TV0);
        v = subtract(TV2,TV0);
        n = crossProduct(u , v); // cross product
        if (n.equals(new VertexImpl(0,0,0)) || n.length()==0)  // triangle is degenerate
            return -1; // do not deal with this case

        dir = subtract(RP1,RP0);// ray direction vector
        w0 = subtract(RP0,TV0);
        a = -dotProduct(n,w0);
        b = dotProduct(n,dir);
        if (Math.abs(b) < MathSuits.EPSILON) {// ray is  parallel to triangle plane
            if (a == 0)// ray lies in triangle plane
                return 2;
            else
                return 0;// ray disjoint from plane
        }

        // get intersect point of ray with triangle plane
        r = a / b;
        if (r < 0.0)// ray goes away from triangle
            return 0; // => no intersect
        // for a segment, also test if (r > 1.0) => no intersect
        //*I = RP0 + r * dir;// intersect point of ray and plane
        outPoint.copyFrom(dir.multiply(r).add(RP0));

        // is I inside T?
        double uu, uv, vv, wu, wv, D;
        uu = dotProduct(u,u);
        uv = dotProduct(u,v);
        vv = dotProduct(v,v);
        //w = *I - TV0;
        w=outPoint.subtract(TV0);
        wu = dotProduct(w,u);
        wv = dotProduct(w,v);
        D = uv * uv - uu * vv;

        // get and test parametric coords
        double s, t;
        s = (uv * wv - vv * wu)/D;
        if (s < 0.0 || s > 1.0) // I is outside T
            return 0;
        t = (uv * wu - uu * wv) / D;
        if (t < 0.0 || (s + t) > 1.0) // I is outside T
            return 0;

        return 1;// I is in T
    }
}
