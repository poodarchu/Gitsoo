#Gitsoo Academic

Currently, I crawled researchers and publications info from microsoft academic.

* the spider is written in python, using scrapy.
* the storage is mongodb enbaling slicing.
* the indexer is on my own.
* the search service is solr.
* the login service: CAS
* others:Struts2 + Jquery + Bootstrap + D3


the src tree:
/terminal: this is a offline version of the whole search engine core, include read data from xml, and store it into sqlite3. and read data from it to create postlists, as well as input queries to search the structured data and get feedback. all of this is done in terminal, for more detalis, please read the readme in ./terminal/

/myspider: this dir include the academic spider.

/engine: this dir include all other units of the system.
