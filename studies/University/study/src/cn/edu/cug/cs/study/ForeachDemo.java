package cn.edu.cug.cs.study;

/**
 * Created by ZhenwenHe on 2016/11/29.
 */
public class ForeachDemo {
    public static void main(String [] args){
        //foreach 其实是C#提出的一个概念
        int[] data=new int[] {1,2,3,4,5,6,7};
        //从JDK1.5之后提供了foreach的循环方式
        for(int x: data){
            System.out.println(x);
        }
    }
}
