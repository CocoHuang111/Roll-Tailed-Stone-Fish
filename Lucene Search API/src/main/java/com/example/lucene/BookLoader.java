package com.example.lucene;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

import java.io.FileReader;
import java.util.ArrayList;
import java.util.List;

import org.springframework.stereotype.Service;

// 读取Json文件，得到Book结构信息
// 数据量更大时考虑缓存
@Service
public class BookLoader {
    public List<Book> loadBooksFromJson(String filePath) throws Exception {
        JsonArray array = (JsonArray) JsonParser.parseReader(new FileReader(filePath));
        List<Book> books = new ArrayList<>();
        Gson gson = new Gson();

        for (var element: array) {
            JsonObject obj = element.getAsJsonObject();
            Book book = gson.fromJson(obj, Book.class);
            books.add(book);
        }
        
        return books;
    }
}
