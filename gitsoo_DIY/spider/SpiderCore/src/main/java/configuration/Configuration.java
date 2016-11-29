package configuration;

/**
 * Created by ivan on 16-5-2.
 */
import java.util.*;
import java.io.*;



public class Configuration {
    private static Configuration instance = new Configuration();
    private Configuration(){}
    private HashMap<String,String> confMap = new HashMap<String,String>();

    public static Configuration getInstance(){
        return instance;
    }
    public HashMap getConfMap(){
        return confMap;
    }
    public void loadConfFile(String filepath){
        try {
            File file = new File(filepath);
            BufferedReader reader = new BufferedReader(new FileReader(file));
            String temp = null;
            while (((temp=reader.readLine())!=null)) {
                if ((!temp.startsWith("#"))&&(!temp.trim().isEmpty())) {
                    int blank = temp.indexOf(" ");
                    confMap.put(temp.substring(0, blank), temp.substring(blank + 1));
                }
            }
        }catch (IOException e){
            e.printStackTrace();
        }
    }




}