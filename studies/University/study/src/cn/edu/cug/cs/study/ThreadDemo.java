package cn.edu.cug.cs.study;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;

/**
 * Created by ZhenwenHe on 2016/12/5.
 */
public class ThreadDemo {
    public static void main(String [] args) throws InterruptedException, ExecutionException {

//        MyThread2 mt1 = new MyThread2("Thread1");
//        MyThread2 mt2 = new MyThread2("Thread2");
//        MyThread2 mt3 = new MyThread2("Thread3");
//        MyThread2 mt4 = new MyThread2("Thread4");
//
//        Thread t1 = new Thread(mt1,mt1.getName());
//        Thread t2 = new Thread(mt2,mt2.getName());
//        Thread t3 = new Thread(mt3,mt3.getName());
//        Thread t4 = new Thread(mt4,mt4.getName());
//        t1.start();
//        t2.start();
//        t3.start();
//        t4.start();
//
//
//        t1.join();
//        t2.join();
//        t3.join();
//        t4.join();


        //第三种实现多线程的方法，采用Callable
        //程序实现了对一个数组的分段计算功能，每个线程负责一部分[start,end)。com

        SharedObject so = new SharedObject(3000);
        MyThread3 mt31= new MyThread3(so,0,1000);
        MyThread3 mt32= new MyThread3(so,1000,2000);
        MyThread3 mt33= new MyThread3(so,2000,3000);
        FutureTask<Double> ft1=new FutureTask<Double>(mt31);
        FutureTask<Double> ft2=new FutureTask<Double>(mt32);
        FutureTask<Double> ft3=new FutureTask<Double>(mt33);
        Thread t1 = new Thread(ft1);
        Thread t2 = new Thread(ft2);
        Thread t3 = new Thread(ft3);
        t1.start();
        t2.start();
        t3.start();
        System.out.println("线程: "+t1.getId()+"= "+ ft1.get() );
        System.out.println("线程: "+t2.getId()+"= "+ ft2.get() );
        System.out.println("线程: "+t3.getId()+"= "+ ft3.get() );

        System.out.println("Total="+so.getSum());
    }
}
