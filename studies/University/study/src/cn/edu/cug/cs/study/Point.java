package cn.edu.cug.cs.study;

/**
 * Created by ZhenwenHe on 2016/12/5.
 */
class Point <T extends Number> {
    T x;
    T y;

    public Point(T x, T y) {
        this.x = x;
        this.y = y;
    }

    public T getX() {
        return x;
    }

    public void setX(T x) {
        this.x = x;
    }

    public T getY() {
        return y;
    }

    public void setY(T y) {
        this.y = y;
    }
}