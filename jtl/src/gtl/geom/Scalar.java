package gtl.geom;

/**
 * Created by hadoop on 17-3-26.
 */
public class Scalar {
    double scalar;

    public Scalar(double scalar) {
        this.scalar = scalar;
    }

    Scalar multiply(Scalar s){
        return new Scalar(s.scalar*this.scalar);
    }

    Vector multiply(Vector s){
        Vector v = (Vector)s.clone();
        for(int i=0;i<s.getDimension();++i)
            v.setOrdinate(i,s.getOrdinate(i)*this.scalar);
        return v;
    }

    Scalar add(Scalar s){
        return new Scalar(s.scalar+this.scalar);
    }

    Scalar subtract(Scalar s){
        return new Scalar(this.scalar-s.scalar);
    }

    Scalar divide(Scalar s){
        return new Scalar(this.scalar/s.scalar);
    }

}
