package cn.edu.cug.cs.study;

/**
 * Created by ZhenwenHe on 2016/11/29.
 */

public class GenericDemo {
    public static void main(String [] args){
        Point<Integer> p = new Point<Integer>(10,20);
        //等价于，后一个参数可以省略
        //Point<Integer> p = new <>(10,20）；
        System.out.println("X="+p.getX()+" Y="+p.getY());
        /*
        1.泛型的参数必须是引用类型，这里是错误的
        Point<int> ps = new Point<int> (10,20);
        */
        //如果不写泛型的参数，默认的是Object类型
        Point p1= new Point(10,20);
        System.out.println("X="+p1.getX()+" Y="+p1.getY());


        Message2<String> m = new MessageImpl<String>();
        m.fun();
        m.fun2("Hello");
    }
}
