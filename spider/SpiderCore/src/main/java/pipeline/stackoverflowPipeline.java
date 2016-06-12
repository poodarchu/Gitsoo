package pipeline;

/**
 * Created by ivan on 16-5-17.
 */
import com.alibaba.fastjson.JSON;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

import org.apache.commons.codec.digest.DigestUtils;
import timer.Timer;
import us.codecraft.webmagic.ResultItems;
import us.codecraft.webmagic.Task;
import us.codecraft.webmagic.pipeline.Pipeline;
import java.util.Date;
import java.text.SimpleDateFormat;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import configuration.*;

public class stackoverflowPipeline implements Pipeline{
    Configuration conf = Configuration.getInstance();
    Timer timer = Timer.getInstance();
    String path = "";
    private SimpleDateFormat TimeStampFormat = new SimpleDateFormat("yyyyMMddHHmm");


    public stackoverflowPipeline(){
        path = (String)conf.getConfMap().get("jsonDirectory");
        File dir = new File(path);
        if (!dir.exists()){
            dir.mkdir();
        }
    }

    boolean isPost(String url){
        Pattern r = Pattern.compile("http:\\/\\/stackoverflow\\.com\\/questions\\/\\d+\\/.+");
        Matcher m = r.matcher(url);
        if(m.matches())
            return true;
        else
            return false;
    }

    //从原始数据中提取单词数字
    String  processRawData(String raw){
        String regex = "[A-Za-z0-9]+";
        Pattern pattern = Pattern.compile(regex);
        Matcher ma = pattern.matcher(raw);
        String result = "";
        while (ma.find()){
                result = result + " "+ma.group();
        }
        return result;
    }


    String getLabels(String list){
        String temp = list.replaceAll("\\s+",'"'+","+'"');
        return "["+'"'+temp+'"'+"]";
        }

    @Override
    public void process(ResultItems resultItems,Task task) {
        if (isPost(resultItems.getRequest().getUrl())) {

            Map<String, String> result = new HashMap<String, String>();
            result.put("encoding", "utf-8");
            result.put("URL", resultItems.getRequest().getUrl());
            result.put("parentPageCount", "0");
            result.put("pageWeigh", "1");
            result.put("title", processRawData((String) resultItems.getAll().get("title")));
            result.put("timestamp", TimeStampFormat.format(new Date()));
            result.put("author", "");
            result.put("label", getLabels((String) resultItems.getAll().get("labels")));
            result.put("text", processRawData((String) resultItems.getAll().get("question")+(String) resultItems.getAll().get("answer")));
            try {
                PrintWriter e = new PrintWriter(new FileWriter(path+"/" + DigestUtils.md5Hex(resultItems.getRequest().getUrl()) + ".json"));
                e.write(JSON.toJSONString(result));
                e.close();
                if (timer.isTimeOut(System.currentTimeMillis()))
                    System.exit(0);
            } catch (IOException var) {
                System.out.println("write file error" + var);

            }
        }
    }
}
