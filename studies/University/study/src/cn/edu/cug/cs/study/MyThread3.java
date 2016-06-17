package cn.edu.cug.cs.study;

import java.util.concurrent.Callable;

/**
 * Created by ZhenwenHe on 2016/12/5.
 */
public class MyThread3 implements Callable<Double>{
    private SharedObject sharedObject;
    private int start;
    private int end;

    public MyThread3(SharedObject sharedObject, int start, int end) {
        this.sharedObject = sharedObject;
        this.start = start;
        this.end = end;
        if(end>this.sharedObject.getCount()){
            System.out.println("Error: End is larger than count!");
        }
    }

    /**
     * Computes a result, or throws an exception if unable to do so.
     *
     * @return computed result
     * @throws Exception if unable to compute a result
     */
    @Override
    public Double call() throws Exception {

        double [] data = this.sharedObject.getData();
        double sum = 0;
        for(int i=start;i<end;i++){
            sum+=data[i];
        }
        this.sharedObject.addSum(sum);
        return sum;

//        sum += this.sharedObject.getSum();
//        this.sharedObject.setSum(sum);
//        return this.sharedObject;
    }
}
