package cn.cug.edu.cs.database.demo;

import java.sql.*;
import java.util.Arrays;

import static java.lang.System.*;

/**
 * Created by ZhenwenHe on 2016/11/29.
 */
public class BatchDemo {
    private static final String DBURL;
    private static final String DBUSER;
    private static final String DBPSW;
    private static final String DBDRIVER;
    static {
        //192.168.137.131 is your virtual machine ip
        //xe is the oracle database instance running on your virtual machine
        //1521 is the listener port
        DBURL = "jdbc:oracle:thin:@192.168.137.131:1521:xe";
        //user name
        DBUSER = "HR";
        //password
        DBPSW = "cug";
        //you should import ojdbc7.jar or ojdbc6.jar in your java project.
        DBDRIVER="oracle.jdbc.driver.OracleDriver";
    }
    public static void main(String [] args) throws SQLException, ClassNotFoundException {
        out.println("Begin main program");
        //first step: load database driver
        Class<?> aClass = Class.forName(DBDRIVER);
        //second step: connect database
        Connection conn = DriverManager.getConnection(DBURL,DBUSER,DBPSW);
        out.println(conn);
        //third step: execute query and update
        Statement s = conn.createStatement();
        conn.setAutoCommit(false);
        try {
            String sql ="create table testtab(id number(6), name varchar2(20))";
            s.addBatch(sql);
            sql = "insert into testtab( id,name) values (1,'1')";
            s.addBatch(sql);
            sql = "insert into testtab( id,name) values (2,'2')";
            s.addBatch(sql);
            sql = "insert into testtab( id,name) values (3,'3')";
            s.addBatch(sql);
            sql = "insert into testtab( id,name) values (4,'4')";
            s.addBatch(sql);
            sql = "insert into testtab( id,name) values (5,'5')";
            s.addBatch(sql);
            int[] rows = s.executeBatch();
            out.println(Arrays.toString(rows));
            conn.commit();
        }
        catch (Exception e){
            e.printStackTrace();
            conn.rollback();
        }

        String sql ="select id,name  from testtab where id=?";
        PreparedStatement ps = conn.prepareStatement(sql);
        ps.setInt(1,3);
        ResultSet rs = ps.executeQuery();
        while(rs.next())
            System.out.println(rs.getInt("id") + " " + rs.getString("name"));

        //fifth  step: close connection
        conn.close();
        out.println("End main program");
    }
}
