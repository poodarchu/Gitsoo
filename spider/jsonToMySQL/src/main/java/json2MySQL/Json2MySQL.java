package json2MySQL;

import java.io.*;
import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import jdbcAction.*;
import java.sql.*;
import configuration.*;
/**
 * Created by ivan on 16-5-18.
 */
public class Json2MySQL {

    private String encoding=null;
    private String URL=null;
    private String parentPageCount=null;
    private String pageWeigh=null;
    private String title=null;
    private String timestamp=null;
    private String author=null;
    private String label=null;
    private String text=null;

    JdbcAction jdbc = null;
    File file = null;

    String ReadFile(){
        BufferedReader reader = null;
        String result="";
        try {
            reader = new BufferedReader(new FileReader(file));
            String temp;
            while ((temp=reader.readLine())!=null)
                result = result+temp;
            //file.delete();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return result;
    }

    void setJSON(JSONObject jo){
        encoding = jo.get("encoding").toString();
        URL = jo.get("URL").toString();
        parentPageCount = jo.get("parentPageCount").toString();
        pageWeigh = jo.get("pageWeigh").toString();
        title = jo.get("title").toString();
        timestamp = jo.get("timestamp").toString();
        author = jo.getString("author").toString();
        label = jo.get("label").toString();
        text = jo.get("text").toString();

    }



    public Json2MySQL(File file){
        this.file = file;
        String jsonStr=ReadFile();
        JSONObject jo = JSON.parseObject(jsonStr);
        setJSON(jo);

    }

    public void insertIntoMySQL(){
        jdbc = JdbcAction.getInstance();
        jdbc.insert("INSERT INTO "+jdbc.getTbName()+" (encoding,URL,parentPageCount,PageWeigh,title,timestamp,author,label,text)VALUES (\'"
                +encoding+"\',\'"+URL+ "\',"+parentPageCount+","+pageWeigh
                +",\'"+title+"\',\'"+timestamp+"\',\'"+author+"\',\'"+label+"\',\'"+text+"\');");

    }

    public void deleteJsonFile(){
        if (file.exists()) {

            try {
                System.out.println(file.getCanonicalFile());
                file.delete();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }


}
