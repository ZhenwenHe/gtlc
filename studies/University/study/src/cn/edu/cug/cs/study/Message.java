package cn.edu.cug.cs.study;

/**
 * Created by ZhenwenHe on 2016/12/5.
 */
interface Message<T>{
    public void print();
    public default void fun(){
        System.out.println("Fun");
    }
}