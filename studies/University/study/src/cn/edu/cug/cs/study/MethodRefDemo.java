package cn.edu.cug.cs.study;

/**
 * Created by ZhenwenHe on 2016/11/30.
 */

import java.lang.invoke.MethodHandle;
import java.util.function.*;



//Interface Consumer<T> void accept(T t)
//Interface Function<T,R> R apply(T t)
//Interface Supplier<T> T get()
//Interface Predicate<T>  boolean test(T t)
public class MethodRefDemo {
    public static void main(String [] args){
        Consumer<String> c=System.out::println;

        Function<Integer,String> mr = String::valueOf;
        String str = mr.apply(1999);
        c.accept(str);

        Supplier<String> mr2="Hello"::toUpperCase;
        str = mr2.get();
        c.accept(str);

        BiFunction<String,String,Integer> mr3=String::compareTo;
        c.accept(mr3.apply("A","B").toString());

        StringBuffer sb= new StringBuffer("Hello World");
        Function<StringBuffer,String> mr4 = String::new;
        str = mr4.apply(sb);
        c.accept(str);
    }
}
