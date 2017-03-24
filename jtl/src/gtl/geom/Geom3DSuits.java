package gtl.geom;


import gtl.common.Variant;
import gtl.math.MathSuits;

/**
 * Created by ZhenwenHe on 2017/3/15.
 */
public class Geom3DSuits extends GeomSuits {




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

    public static  double angle(Vector o, Vector a , Vector b){
        return o.angle(a,b);
    }

    public static Vector subtract  (Vector a, Vector b){
        return a.subtract(b);
    }
    public static Vector add(Vector a, Vector b){
        return a.add(b);
    }
    public static double distance(Vector a, Vector b){
        return MathSuits.distance(a.getCoordinates(),b.getCoordinates());
    }

    public static double distancePointSegment(Vector p, Vector A, Vector B) {
        // if start = end, then just compute distance2D to one of the endpoints
        if (A.equals(B))
            return distance(p, A);

        // otherwise use comp.graphics.algorithms Frequently Asked Questions method
	    /*
	     * (1) r = AC dot AB 
	     *         --------- 
	     *         ||AB||^2 
	     *         
	     * r has the following meaning: 
	     *   r=0 P = A 
	     *   r=1 P = B 
	     *   r<0 P is on the backward extension of AB 
	     *   r>1 P is on the forward extension of AB 
	     *   0<r<1 P is interior to AB
	     */

        double len2 = (B.getX() - A.getX()) * (B.getX() - A.getX()) + (B.getY() - A.getY()) * (B.getY() - A.getY()) + (B.getZ() - A.getZ()) * (B.getZ() - A.getZ());
        if (Double.isNaN(len2))
            throw new IllegalArgumentException("Ordinates must not be NaN");
        double r = ((p.getX() - A.getX()) * (B.getX() - A.getX()) + (p.getY() - A.getY()) * (B.getY() - A.getY()) + (p.getZ() - A.getZ()) * (B.getZ() - A.getZ()))
                / len2;

        if (r <= 0.0)
            return distance(p, A);
        if (r >= 1.0)
            return distance(p, B);

        // compute closest point q on line segment
        double qx = A.getX() + r * (B.getX() - A.getX());
        double qy = A.getY() + r * (B.getY() - A.getY());
        double qz = A.getZ() + r * (B.getZ() - A.getZ());
        // result is distance2D from p to q
        double dx = p.getX() - qx;
        double dy = p.getY() - qy;
        double dz = p.getZ() - qz;
        return Math.sqrt(dx*dx + dy*dy + dz*dz);
    }

    /**
     * Computes the distance2D between two 3D segments.
     *
     * @param A the start point of the first segment
     * @param B the end point of the first segment
     * @param C the start point of the second segment
     * @param D the end point of the second segment
     * @return the distance2D between the segments
     */
    public static double distanceSegmentSegment(Vector A, Vector B, Vector C, Vector D)
    {
        /**
         * This calculation is susceptible to roundoff errors when 
         * passed large ordinate values.
         * It may be possible to improve this by using {@link DD} arithmetic.
         */
        if (A.equals(B))
            return distancePointSegment(A, C, D);
        if (C.equals(B))
            return distancePointSegment(C, A, B);

        /**
         * Algorithm derived from http://softsurfer.com/Archive/algorithm_0106/algorithm_0106.htm
         */
        double a = dotProduct(A, B, A, B);
        double b = dotProduct(A, B, C, D);
        double c = dotProduct(C, D, C, D);
        double d = dotProduct(A, B, C, A);
        double e = dotProduct(C, D, C, A);

        double denom = a*c - b*b;
        if (Double.isNaN(denom))
            throw new IllegalArgumentException("Ordinates must not be NaN");

        double s;
        double t;
        if (denom <= 0.0) {
            /**
             * The lines are parallel. 
             * In this case solve for the parameters s and t by assuming s is 0.
             */
            s = 0;
            // choose largest denominator for optimal numeric conditioning
            if (b > c)
                t = d/b;
            else
                t = e/c;
        }
        else {
            s = (b*e - c*d) / denom;
            t = (a*e - b*d) / denom;
        }
        if (s < 0)
            return distancePointSegment(A, C, D);
        else if (s > 1)
            return distancePointSegment(B, C, D);
        else if (t < 0)
            return distancePointSegment(C, A, B);
        else if(t > 1) {
            return distancePointSegment(D, A, B);
        }
        /**
         * The closest points are in interiors of segments,
         * so compute them directly
         */
        double x1 = A.getX() + s * (B.getX() - A.getX());
        double y1 = A.getY() + s * (B.getY() - A.getY());
        double z1 = A.getZ() + s * (B.getZ() - A.getZ());

        double x2 = C.getX() + t * (D.getX() - C.getX());
        double y2 = C.getY() + t * (D.getY() - C.getY());
        double z2 = C.getZ() + t * (D.getZ() - C.getZ());

        // length (p1-p2)
        return distance((Vector) createVector(x1, y1, z1), (Vector) createVector(x2, y2, z2));
    }

}
