# SpiderPro
***
##spider
**Main：**主函数 包含导入配置文件和调用爬虫
**spider:** 爬虫爬取程序(包含抓取的逻辑)
**pipeline:** 处理爬取(包含将抓取到的数据存进json文件)
**time:**定时器
**configuration:**管理配置文件

爬取时间及json文件存放地址配置在配置文件Spider.conf中

保存的json文件的格式：
`{"encoding":"",
"URL":"",
"parentPageCount":"",
"pageWeigh":"",
"title":"",
"timestamp":"",
"author":"",
"label":"",
"text":""}`
*更新parentPageCount未完成*
***
##jsonToMySQL
**json2MySQL:**处理json文件
**jdbcAction:**与MySQL交互的相关操作
**configuration:**管理配置文件

MySQL的IP、端口等配置在jsonToMySQL.conf中配置

**数据库中使用的表的格式**

<table class="table table-bordered table-striped table-condensed">
<tr>
    <th>Field</th>
    <th>Type</th>
    <th> NULL </th>
    <th> KEY </th>
    <th>Default</th>
    <th>Extra</th>
</tr>
<tr>
    <td>pageID</td>
    <td>int</td>
    <td>NO</td>
    <td>PRI</td>
    <td>NULL</td>
    <td>auto_increment</td>
</tr>
<tr>
    <td>encoding</td>
    <td>varchar(20)</td>
    <td>NO</td>
    <td></td>
    <td>NULL</td>
    <td></td>
</tr>
<tr>
    <td>URL</td>
    <td>varchar(150)</td>
    <td>NO</td>
    <td></td>
    <td>NULL</td>
    <td></td>
</tr>
<tr>
    <td>pageWeigh</td>
    <td>int</td>
    <td>NO</td>
    <td></td>
    <td>NULL</td>
    <td></td>
</tr>
<tr>
    <td>parentPageCount</td>
    <td>int</td>
    <td>NO</td>
    <td></td>
    <td>NULL</td>
    <td></td>
</tr>
<tr>
    <td>title</td>
    <td>varchar(100)</td>
    <td>NO</td>
    <td></td>
    <td>NULL</td>
    <td></td>
</tr>
<tr>
    <td>timestamp</td>
    <td>varchar(12)</td>
    <td>NO</td>
    <td></td>
    <td>NULL</td>
    <td></td>
</tr>
<tr>
    <td>author</td>
    <td>varchar(40)</td>
    <td>NO</td>
    <td></td>
    <td>NULL</td>
    <td></td>
</tr>
<tr>
    <td>label</td>
    <td>varchar(150)</td>
    <td>NO</td>
    <td></td>
    <td>NULL</td>
    <td></td>
</tr>
<tr>
    <td>text</td>
    <td>varchar(20000)</td>
    <td>NO</td>
    <td></td>
    <td>NULL</td>
    <td></td>
</tr>
<tr>
    <td>update</td>
    <td>int</td>
    <td>NO</td>
    <td></td>
    <td>0</td>
    <td></td>
</tr>
</table>

<small>---这只是针对目前所爬的stackoverflow的数据设置的表----</small>
