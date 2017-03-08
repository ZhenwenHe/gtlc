package gtl.math;

import gtl.common.Variant;
import gtl.math.geometry.Vertex;

import java.util.Optional;

/**
 * Created by ZhenwenHe on 2017/3/13.
 */
public class MathSuits {
    public static final double EPSILON = 1.0E-308;
    public static final double M_PI_2 =1.57079632679489661922;

    public static double dotProduct(double[] A, double[] B){
        double dRtn = 0;
        int dims=java.lang.Math.min(A.length,B.length);
        for(int i=0;i<dims;++i){
            dRtn += A[i]*B[i] ;
        }
        return dRtn;
    }
    public static double distance(double [] A, double[] B){
        double dRtn = 0;
        int dims=java.lang.Math.min(A.length,B.length);
        for(int i=0;i<dims;++i){
            dRtn += (A[i]-B[i])* (A[i]-B[i]);
        }
        return java.lang.Math.sqrt(dRtn);
    }
    /**求 C 点在直线 AB 上的垂线距离
     *
     */
    public static double perpendicularDistance(Vertex vA, Vertex vB, Vertex vC){
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
    public static double perpendicularDistance(Vertex vA, Vertex vB, Vertex vC, Vertex vP, Variant  onAB){
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
}
