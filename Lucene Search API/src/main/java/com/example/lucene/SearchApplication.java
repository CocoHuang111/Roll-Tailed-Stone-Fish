package com.example.lucene;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.store.ByteBuffersDirectory;
import org.apache.lucene.store.Directory;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;
import org.wltea.analyzer.lucene.IKAnalyzer;

@SpringBootApplication
public class SearchApplication {
    public static void main(String[] args) {
        SpringApplication.run(SearchApplication.class, args);
    }

    @Bean
    public Analyzer analyzer() {
        return new IKAnalyzer();
    }
    /*
    @Bean
    public Directory directory() {
        return new ByteBuffersDirectory();
    }
     */

    @Bean
    public BookLoader bookLoader() {
        return new BookLoader();
    }
}
