package cn.edu.cug.cs.study;

/**
 * Created by ZhenwenHe on 2016/12/5.
 */
class MessageImpl<T> implements Message2<T>{
    @Override
    public void print() {
        System.out.println("print");
    }

    @Override
    public void fun2(T t) {
        System.out.println("fun2 "+t);
    }
}