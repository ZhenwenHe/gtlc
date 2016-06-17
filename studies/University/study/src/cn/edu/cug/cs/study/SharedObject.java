package cn.edu.cug.cs.study;

/**
 * Created by ZhenwenHe on 2016/12/5.
 */
public class SharedObject {

    private double [] data;
    private double sum;
    private int count;

    public SharedObject(int count) {
        this.sum=0;
        this.count=count>0?count:3000;
        data = new double [this.count];
        for(int i=1;i<=this.count;i++){
            data[i-1]=i;
        }
    }

    public double[] getData() {
        return data;
    }

    public double getSum() {
        return sum;
    }

    public synchronized void addSum(double sum) {
        this.sum += sum;
    }

    public int getCount() {
        return count;
    }
}
