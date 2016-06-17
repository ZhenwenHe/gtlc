package cn.cug.edu.cs.database.demo;

import java.sql.*;

/**
 * Created by ZhenwenHe on 2016/11/29.
 */
public class ConnectionDemo {
    private static final String DBURL;
    private static final String DBUSER;
    private static final String DBPSW;
    private static final String DBDRIVER;
    static {
        //192.168.137.131 is your virtual machine
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
        System.out.println("Begin main program");
        //first step: load database driver
        Class<?> aClass = Class.forName(DBDRIVER);
        //second step: connect database
        Connection conn = DriverManager.getConnection(DBURL,DBUSER,DBPSW);
        System.out.println(conn);
        //third step: execute query
        String sql ="select employee_id, first_name, last_name from employees where employee_id=?";
        PreparedStatement ps = conn.prepareStatement(sql);
        ps.setInt(1,100);
        ResultSet rs = ps.executeQuery();
        while(rs.next())
            System.out.println(rs.getInt("employee_id") + " " + rs.getString("first_name") + " " + rs.getString("first_name"));
        //fourth step: update operation
        sql="insert into regions (region_id, region_name) values(?,?)";
        ps = conn.prepareStatement(sql);
        ps.setInt(1,30);
        ps.setString(2,"China");
        int rows= ps.executeUpdate();
        System.out.println(rows);
        //fifth  step: close connection
        conn.close();
        System.out.println("End main program");
    }
}
