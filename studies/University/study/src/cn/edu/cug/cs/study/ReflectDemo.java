package cn.edu.cug.cs.study;

import java.util.Date;

/**
 * Created by ZhenwenHe on 2016/12/1.
 */
public class ReflectDemo {
    public static void main(String [] args) throws ClassNotFoundException {
        /**
         * 第一种类型的Class获取方法
         */
        String s = new String();
        Class<?> cls = s.getClass();
        System.out.println(cls);
        /**
         * 第二种类型的Class获取方法
         */
        System.out.println(String.class);
        /**
         * 第三种类型的Class获取方法
         */
        Class<?> cls2 = Class.forName("java.util.Date");
        System.out.println(cls2);

    }
}
