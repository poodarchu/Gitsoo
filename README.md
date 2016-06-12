server A通过JSON-RPC将用户查询以ｊｓｏｎ格式发送给Main
Main接收用户输入的字符串序列
将其传递给分词器
分词器将分词完毕的[token]传递给Searcher.
Searcher从My查找每个搜索键对应的文档然后进行逻辑操作（归并求交集）得到一个精简的文档列表，
然后Searcher对文档进行评分（scoring）、筛选和排序，
根据筛选出的docID从文档数据库中提取处对应的url, timestamp, content, title,以json的形式返回给Main
然后将排好序的文档的通过指定的信道发送给主协程，
主协程将结果通过JSON-RPC返回给用户。

Spider从网络上爬取数据，规范格式之后，存储到文档数据库中，表的格式与ｊｓｏｎ内包含的信息相对应
Indexer从文档数据库中提取处doc_id和对应的text,建立
tokenID: DocID, [positions], ...
格式的倒排列表。
然后tokenID不需要压缩，将倒排文件亚索后存数倒排索引数据库。

Ｓｐｉｄｅｒ存储文档数据时，先根据doc_id找到文档数据库中对应的doc,更新相关信息，并将updated属性置为１
Indexer定期检查文档数据库，并将updated为１的ｄｏｃ更新倒排索引，然后将ｕｐｄａｔｅｄ置为０
