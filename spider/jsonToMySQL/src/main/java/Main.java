/**
 * Created by ivan on 16-5-18.
 */
//import configuration.*;

import  configuration.Configuration;
import jdbcAction.JdbcAction;
import java.io.*;
import json2MySQL.Json2MySQL;
public class Main {


    public static void main(String[] args){


        Configuration conf = Configuration.getInstance();
        conf.loadConfFile("jsonToMySQL.conf");

        JdbcAction jdbc = JdbcAction.getInstance();
        jdbc.setArgument((String)conf.getConfMap().get("ip"),(String)conf.getConfMap().get("port"),(String)conf.getConfMap().get("database"),
                (String)conf.getConfMap().get("username"),(String)conf.getConfMap().get("password"),(String)conf.getConfMap().get("tbname"));

        String dirPath = (String)conf.getConfMap().get("jsonDir");
        File dir = new File(dirPath);
        File[] fileArr = null;
        if (dir.isDirectory()){
            fileArr = dir.listFiles();
        }
        for (File file:fileArr) {
            Json2MySQL j2m = new Json2MySQL(file);
            j2m.insertIntoMySQL();

        }


    }

}
