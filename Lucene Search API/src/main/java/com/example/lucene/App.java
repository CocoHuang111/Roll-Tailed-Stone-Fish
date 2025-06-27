package com.example.lucene;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.ByteBuffersDirectory;
import org.wltea.analyzer.lucene.IKAnalyzer;

import java.util.List;


public class App {
    public static void main(String[] args) throws Exception {
        Analyzer analyzer = new IKAnalyzer();
        Directory directory = new ByteBuffersDirectory();
        IndexWriterConfig config = new IndexWriterConfig(analyzer);
        IndexWriter indexWriter = new IndexWriter(directory, config);
        
        BookLoader loader = new BookLoader();
        List<Book> books = loader.loadBooksFromJson("data/books.json");

        BookIndexer indexer = new BookIndexer();
        indexer.indexBooks(indexWriter, books);
        
        indexWriter.close();

        Searcher searcher = new Searcher(directory, analyzer);
        List<Document> hits = searcher.search("Java 机械", 4);
        int i = 0;
        for (Document doc: hits) {
            i++;
            System.out.println("Book" + i + ": 《" + doc.get("title") + "》, author: " + doc.get("author") + ", description: " + doc.get("description"));
        }
    }
}
