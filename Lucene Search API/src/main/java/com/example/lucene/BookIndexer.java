package com.example.lucene;

import org.apache.lucene.document.*;
import org.apache.lucene.index.IndexWriter;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.util.List;

@Service
public class BookIndexer {
    public void indexBooks(IndexWriter writer, List<Book> books) throws IOException {
        for (Book book: books) {
            Document doc = new Document();

            if (book.getTitle() != null) 
                doc.add(new TextField("title", book.getTitle(), Field.Store.YES));

            if (book.getDescription() != null)
                doc.add(new TextField("description", book.getDescription(), Field.Store.YES));
            
            if (book.getAuthor() != null)
                doc.add(new TextField("author", book.getAuthor(), Field.Store.YES));

            if (book.getPublisher() != null)
                doc.add(new TextField("publisher", book.getPublisher(), Field.Store.YES));

            if (book.getPublishYear() != null)
                doc.add(new TextField("publishYear", book.getPublishYear(), Field.Store.YES));

            if (book.getIsbn() != null)
                doc.add(new TextField("isbn", book.getIsbn(), Field.Store.YES));
            
            if (book.getPrice() != null) {
                doc.add(new DoublePoint("price", Double.parseDouble(book.getPrice())));
                doc.add(new StoredField("price", book.getPrice()));
            }

            if (book.getTagsArray() != null)
                doc.add(new TextField("tags", String.join(" ", book.getTagsArray()), Field.Store.YES));

            if (book.getSellerID() != null)
                doc.add(new TextField("sellerID", book.getSellerID(), Field.Store.YES));
            
            if (book.getListedTime() != null)
            doc.add(new TextField("listedTime", book.getListedTime(), Field.Store.YES));

            if (book.getCoverImage() != null) 
                doc.add(new StoredField("coverImage", book.getCoverImage()));

            writer.addDocument(doc);
        }
    }
}
