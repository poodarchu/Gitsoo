package spider;

/**
 * Created by ivan on 16-5-2.
 */

import redis.clients.jedis.Jedis;
import us.codecraft.webmagic.Page;
import us.codecraft.webmagic.Site;
import us.codecraft.webmagic.processor.PageProcessor;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class stackoverflowSpider implements PageProcessor{
    public static final String URL_LIST = "http:\\/\\/stackoverflow\\.com\\/questions\\?page=\\d+&sort=.+";

    public static final String URL_POST = "http:\\/\\/stackoverflow\\.com\\/questions\\/\\d+\\/.+";

    public stackoverflowSpider(){
        Jedis jedis = new Jedis("localhost");
        if (jedis.get("stackoverflow.com_pageTotalNum")==null)
            jedis.set("stackoverflow.com_pageTotalNum","0");

        setPageTotalNum(Integer.parseInt(jedis.get("stackoverflow.com_pageTotalNum")));
    }


    int pageTotalNum = 0;

    private Site site = Site
            .me()
            .setDomain("stackoverflow.com")
            .setSleepTime(3000)
            .setUserAgent(
                    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_2) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.65 Safari/537.31");

    public void setPageTotalNum(int num){
        pageTotalNum = num;
    }

    int getPageNum(String url){
        Pattern r = Pattern.compile("(\\d+)");
        Matcher m = r.matcher(url);
        if (m.find())
            return Integer.parseInt(m.group(0));
        else
            return -1;
    }

    String createListUrl(int num){
        return "http://stackoverflow.com/questions?page="+ Integer.toString(num) +"&sort=votes.html";
    }

    @Override
    public void process(Page page) {
        //列表页
        System.out.println(page.getUrl());
        if (page.getUrl().regex(URL_LIST).match()) {
            page.addTargetRequests(page.getHtml().xpath("//div[@id='content']").links().regex(URL_POST).all());

            if(getPageNum(page.getUrl().toString())==1){//读取总共页数
                pageTotalNum = Integer.parseInt(page.getHtml().xpath("//*[@id=\"mainbar\"]/div[4]/a[5]/span/text()").toString());
                Jedis jedis = new Jedis("localhost");
                jedis.set("stackoverflow.com_pageTotalNum",String.valueOf(pageTotalNum));
            }
            if(getPageNum(page.getUrl().toString())<pageTotalNum){
                page.addTargetRequest(createListUrl(getPageNum(page.getUrl().toString())+1));
            }

            page.addTargetRequests(page.getHtml().xpath("//div[@id='content']").links().regex(URL_POST).all());
        } else {
            page.putField("question-header", page.getHtml().xpath("//div[@id='question-header']/allText()").toString());
            page.putField("question", page.getHtml().xpath("//div[@class='question']/allText()").toString());
            page.putField("answer", page.getHtml().xpath("//div[@id='answers']/allText()").toString());

        }
    }

    @Override
    public Site getSite() {
        return site;
    }

}
