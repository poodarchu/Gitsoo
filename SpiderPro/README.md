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
|Field|Type|Null|Key|Default|Extra|
|-----|:-----:|:-----:|:-----:|:-----:|
|pageID|int|NO|PRI|NULL|auto_increment|
|encoding|varchar(20)|NO| |NULL||
|URL|varchar(150)|NO| |NULL||
|parentPageCount|int|NO| |NULL||
|pageWeigh|int|NO| |NULL||
|title|varchar(100)|NO| |NULL||
|timestamp|varchar(12)|NO| |NULL||
|author|varchar(40)|NO| |NULL||
|label|varchar(150)|NO| |NULL||
|text|varchar(20000)|NO| |NULL||
|update|int|NO| |0||
<small>---这只是针对目前所爬的stackoverflow的数据设置的表----</small>
