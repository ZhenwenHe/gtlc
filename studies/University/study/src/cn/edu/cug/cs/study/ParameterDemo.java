package cn.edu.cug.cs.study;

/**
 * Created by ZhenwenHe on 2016/11/29.
 */
public class ParameterDemo {
    public static void main(String [] args){
        System.out.println("begin main program");
        System.out.println(add(1,2,3,4,5,6));
        System.out.println("end main program");
    }
    public static int add(int ... v){
        int sum=0;
        for (int x=0;x<v.length;x++)
            sum += v[x];
        return sum;
    }
}
