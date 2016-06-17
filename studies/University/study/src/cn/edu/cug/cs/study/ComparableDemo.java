package cn.edu.cug.cs.study;

import java.util.Arrays;

/**
 * Created by ZhenwenHe on 2016/12/1.
 */
public class ComparableDemo {
    public static void main(String [] args){
        Book [] books ={
                new Book("Java",78.9),
                new Book("C++",88.9),
                new Book("Python",68.9),
                new Book("Go",89.9)
        };
        Arrays.sort(books);
        for(Book i:books)
            System.out.println(i);

        Student [] ss= {
            new Student(1,"Tom",19),
            new Student(2,"David",18),
            new Student(4,"Greece",20),
            new Student(3,"Vincent",17)
        };
        Arrays.sort(ss,new StudentComparator());
        for(Student s:ss)
            System.out.println(s);
    }
}

