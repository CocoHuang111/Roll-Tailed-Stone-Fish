package com.example.lucene;

import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.store.Directory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/api/books")
public class BookController {
    private final Directory directory;
    private final Analyzer analyzer;
    private final BookLoader loader;

    @Autowired
    public BookController(Directory directory, Analyzer analyzer, BookLoader loader) {
        this.directory = directory;
        this.analyzer = analyzer;
        this.loader = loader;
    }

    // 初始化索引
    @PostMapping("/init")
    public String initIndex(@RequestParam String filePath) throws Exception {
        IndexWriterConfig config = new IndexWriterConfig(analyzer);
        try (IndexWriter writer = new IndexWriter(directory, config)) {
            List<Book> books = loader.loadBooksFromJson(filePath);
            new BookIndexer().indexBooks(writer, books);
            return "索引初始化完成，共加载 " + books.size() + " 本书";
        }
    }

    // 多字段搜索
    @GetMapping("/search")
    public String search(
            @RequestParam String query,
            @RequestParam(defaultValue = "10") int topN,
            @RequestParam(required = false) Double minPrice,
            @RequestParam(required = false) Double maxPrice) throws Exception {
        
        List<Document> docs;
        Searcher searcher = new Searcher(directory, analyzer);
        if (minPrice != null || maxPrice != null) 
            docs = searcher.search(query, topN, minPrice, maxPrice);
        else
            docs = searcher.search(query, topN);
        
        String ret = "";
        for (Document doc: docs) {
            ret = ret + '\n' + "Title: " + "《" + doc.get("title") + "》, author: " + doc.get("author") + ", description: " + doc.get("description");
        }
        return ret;
    }

    // 指定字段搜索
    @PostMapping("/advanced-search")
    public String advancedSearch(
            @RequestBody Map<String, String> fieldQueries,
            @RequestParam(defaultValue = "10") int topN,
            @RequestParam(required = false) Double minPrice,
            @RequestParam(required = false) Double maxPrice) throws Exception {
        
        List<Document> docs;
        Searcher searcher = new Searcher(directory, analyzer);
        if (minPrice != null || maxPrice != null) 
            docs = searcher.search(fieldQueries, topN, minPrice, maxPrice);
        else 
            docs = searcher.search(fieldQueries, topN);

        String ret = "";
        for (Document doc: docs) {
            ret = ret + '\n' + "Title: " + "《" + doc.get("title") + "》, author: " + doc.get("author") + ", description: " + doc.get("description");
        }
        return ret;
    }
}
