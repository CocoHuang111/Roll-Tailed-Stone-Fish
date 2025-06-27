package com.example.lucene;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.DoublePoint;
import org.apache.lucene.queryparser.classic.MultiFieldQueryParser;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.*;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.store.Directory;

import java.io.IOException;
import java.util.*;


// 无索引多域搜索 & 有索引指定域搜索，可能是两个搜索界面
// 均支持FuzzySearch，WildCard，价格filter
public class Searcher {
    private final Directory directory;
    private final Analyzer analyzer;

    public Searcher(Directory directory, Analyzer analyzer) {
        this.directory = directory;
        this.analyzer = analyzer;
    }


    // 无索引多域搜索，设置默认权重排序
    // @param queryText 搜索文字
    // @param topN 需要返回的Document数量
    // @param minPrice, maxPrice 价格区间
    // @return List<Document> topN个搜索到的Document
    public List<Document> search(String queryText, int topN) throws IOException, ParseException {
        String[] defaultFields = {"title", "author", "description", "tags", "publisher", "publishYear"};
        Map<String, Float> boosts = new HashMap<>();
        boosts.put("title", 3.0f);
        boosts.put("author", 2.5f);
        boosts.put("description", 2.0f);
        boosts.put("tags", 1.5f);
        boosts.put("publisher", 1.0f);
        boosts.put("publishYear", 1.0f);

        IndexReader reader = DirectoryReader.open(directory);
        IndexSearcher searcher = new IndexSearcher(reader);

        MultiFieldQueryParser parser = new MultiFieldQueryParser(defaultFields, analyzer, boosts);
        parser.setDefaultOperator(QueryParser.Operator.OR);
        parser.setFuzzyMinSim(0.5f);
        parser.setFuzzyPrefixLength(2);
        parser.setAllowLeadingWildcard(true);

        Query query = parser.parse(queryText);

        TopDocs results = searcher.search(query, topN);
        List<Document> docs = new ArrayList<>();
        for (ScoreDoc scoreDoc: results.scoreDocs) {
            docs.add(searcher.doc(scoreDoc.doc));
        }
        reader.close();
        return docs;
    }

    public List<Document> search(String queryText, int topN, Double minPrice, Double maxPrice) throws IOException, ParseException {
        String[] defaultFields = {"title", "author", "description", "tags", "publisher", "publishYear"};
        Map<String, Float> boosts = new HashMap<>();
        boosts.put("title", 3.0f);
        boosts.put("author", 2.5f);
        boosts.put("description", 2.0f);
        boosts.put("tags", 1.5f);
        boosts.put("publisher", 1.0f);
        boosts.put("publishYear", 1.0f);

        IndexReader reader = DirectoryReader.open(directory);
        IndexSearcher searcher = new IndexSearcher(reader);

        MultiFieldQueryParser parser = new MultiFieldQueryParser(defaultFields, analyzer, boosts);
        parser.setDefaultOperator(QueryParser.Operator.OR);
        parser.setFuzzyMinSim(0.5f);
        parser.setFuzzyPrefixLength(2);
        parser.setAllowLeadingWildcard(true);

        Query query = parser.parse(queryText);

        BooleanQuery.Builder finalBuilder = new BooleanQuery.Builder();
        finalBuilder.add(query, BooleanClause.Occur.MUST);

        boolean hasMin = (minPrice != null);
        boolean hasMax = (maxPrice != null);
        if (hasMin || hasMax) {
            Query priceFilter;
            if (hasMin && hasMax) 
                priceFilter = DoublePoint.newRangeQuery("price", minPrice, maxPrice);
            else if (hasMin) 
                priceFilter = DoublePoint.newRangeQuery("price", minPrice, Double.POSITIVE_INFINITY);
            else 
                priceFilter = DoublePoint.newRangeQuery("price", Double.NEGATIVE_INFINITY, maxPrice);

            finalBuilder.add(priceFilter, BooleanClause.Occur.MUST); 
        }

        TopDocs results = searcher.search(finalBuilder.build(), topN);
        List<Document> docs = new ArrayList<>();
        for (ScoreDoc scoreDoc: results.scoreDocs) {
            docs.add(searcher.doc(scoreDoc.doc));
        }
        reader.close();
        return docs;
    }
    

    // 有索引指定域搜索
    // @param fieldQueries 以field为键，query为值
    // @param topN 
    // @param minPrice, maxPrice 
    // @return List<Document> 
    public List<Document> search(Map<String, String> fieldQueries, int topN) throws IOException, ParseException {
        IndexReader reader = DirectoryReader.open(directory);
        IndexSearcher searcher = new IndexSearcher(reader);
    
        BooleanQuery.Builder builder = new BooleanQuery.Builder();

        for (Map.Entry<String, String> entry: fieldQueries.entrySet()) {
            String field = entry.getKey();
            String queryText = entry.getValue();
            
            QueryParser parser = new QueryParser(field, analyzer);
            Query parsedQuery = parser.parse(queryText);

            builder.add(parsedQuery, BooleanClause.Occur.SHOULD);
        }

        Query query = builder.build();
        TopDocs results = searcher.search(query, topN);
        List<Document> docs = new ArrayList<>();
        for (ScoreDoc scoreDoc: results.scoreDocs) {
            docs.add(searcher.doc(scoreDoc.doc));
        }
        reader.close();
        return docs;
    }
    public List<Document> search(Map<String, String> fieldQueries, int topN, Double minPrice, Double maxPrice) throws IOException, ParseException {
        IndexReader reader = DirectoryReader.open(directory);
        IndexSearcher searcher = new IndexSearcher(reader);
    
        BooleanQuery.Builder builder = new BooleanQuery.Builder();

        for (Map.Entry<String, String> entry: fieldQueries.entrySet()) {
            String field = entry.getKey();
            String queryText = entry.getValue();
            
            QueryParser parser = new QueryParser(field, analyzer);
            Query parsedQuery = parser.parse(queryText);

            builder.add(parsedQuery, BooleanClause.Occur.SHOULD);
        }
        
        boolean hasMin = (minPrice != null);
        boolean hasMax = (maxPrice != null);
        if (hasMin || hasMax) {
            Query priceFilter;
            if (hasMin && hasMax) 
                priceFilter = DoublePoint.newRangeQuery("price", minPrice, maxPrice);
            else if (hasMin) 
                priceFilter = DoublePoint.newRangeQuery("price", minPrice, Double.POSITIVE_INFINITY);
            else 
                priceFilter = DoublePoint.newRangeQuery("price", Double.NEGATIVE_INFINITY, maxPrice);

            builder.add(priceFilter, BooleanClause.Occur.MUST); 
        }

        Query query = builder.build();
        TopDocs results = searcher.search(query, topN);
        List<Document> docs = new ArrayList<>();
        for (ScoreDoc scoreDoc: results.scoreDocs) {
            docs.add(searcher.doc(scoreDoc.doc));
        }
        reader.close();
        return docs;
    }
}
