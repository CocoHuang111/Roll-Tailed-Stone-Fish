package com.example.lucene;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class LuceneConfig {
     @Value("${lucene.index.path}")
    private String indexPath;

    @Bean
    public Directory directory() throws IOException {
        Path path = Paths.get(indexPath);
        Files.createDirectories(path);
        return FSDirectory.open(path);
    }
}
