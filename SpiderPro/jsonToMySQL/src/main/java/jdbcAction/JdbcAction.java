package jdbcAction;

import java.sql.*;

/**
 * Created by ivan on 16-5-19.
 */
public class JdbcAction {
    private static JdbcAction instance = null;
    private JdbcAction() {}
    private String ip;
    private String port;
    private String database;
    private String username;
    private String pwd;
    private String tbname;
    static Connection conn;

    public static JdbcAction getInstance(){
        if (instance==null)
            instance = new JdbcAction();
        return instance;
    }
    public void setArgument(String ip,String port,String database,String username,String pwd,String tbname){
        this.ip = ip;
        this.port = port;
        this.database = database;
        this.username = username;
        this.pwd = pwd;
        this.tbname = tbname;
    }
    public String getTbName(){
        return tbname;
    }
    public Connection getConnection(){
        Connection con = null;

        try {
            Class.forName("com.mysql.jdbc.Driver");
            con = DriverManager.getConnection("jdbc:mysql://"+ip+":"+port+"/"+database,
                    username,pwd);
        }catch (Exception e){
            System.out.println("连接数据库出错");
        }
        return con;
    }
    public void insert(String sql){
        conn = getConnection();
        try {
            Statement stat = conn.createStatement();
            int count = stat.executeUpdate(sql);
            System.out.println("成功插入"+count+"条数据");
            conn.close();
        }catch (SQLException e){
            System.out.println("插入失败："+e.getMessage());
        }
    }



}