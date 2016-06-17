package cn.edu.cug.cs.study;

/**
 * Created by ZhenwenHe on 2016/11/30.
 */

public class LamdaDemo {
    public static void main(String [] args){
        fun(
                (s)->{System.out.println(s);},"World"
        );
        fun(new IMessage() {
            @Override
            public void print(String s) {
                System.out.println(s);
            }
        },"Hello");
    }
    public static void fun(IMessage m,String s){
        m.print(s);
    }
}
